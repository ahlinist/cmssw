///////////////////////////////////////////////////////////////////////////////
//
// CaloJetIDAnalyzer
// -----------------
//
// 10/06/2008 Amnon Harel            <amnon.harel@cern.ch>
//            Philipp Schieferdecker <philipp.schieferdecker@cern.ch> (base structure + jet trigger matching)
//
////////////////////////////////////////////////////////////////////////////////


#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
 
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/JetTracksAssociation.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/METReco/interface/CaloMET.h" // it's declared but not defined in CaloMETCollection
#include "DataFormats/JetReco/interface/BasicJet.h"

#include "DataFormats/METReco/interface/HcalNoiseSummary.h"

#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

#include "PhysicsTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"

#include "JetMETCorrections/Objects/interface/JetCorrector.h"

#include <TTree.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TMath.h>
#include <TVector3.h>

#include <memory>
#include <string>
#include <sstream>
#include <cmath>
#include <numeric>
#include <functional>

#include "RecoJets/JetAlgorithms/interface/JetIDHelper.h"
#include "PhysicsTools/PatUtils/interface/JetIDSelectionFunctor.h"

using std::endl; 
using std::cout;
using std::cerr;
using std::string;
using std::vector;
using std::pair;

//#include <boost/lambda/lambda.hpp>
//#include <boost/lambda/bind.hpp>
// there's a clash internal to boost, with global _1 and boost::lambda::_1, so
// can't just do: using namespace boost::lambda;
//                using boost::lambda::_1;

using trigger::TriggerObject;
using trigger::TriggerObjectCollection;

// =================================== start of the "real" CaloJetIDAnalyzer ======================================

bool subtower_has_greater_E( reco::helper::JetIDHelper::subtower i, 
			     reco::helper::JetIDHelper::subtower j ) { return i.E > j.E; }

// Helper class

// I want to sort jets by corrected pT, but do almost anything else with raw pT
class jetWithScale : public reco::CaloJet 
{
private:
  double fScale;
  const reco::CaloJet* fpOrg; // needed for association (e.g. track-jet), as they require the original collection

public:
  jetWithScale ()
    : reco::CaloJet ()
  {
    fScale = 1.;
    fpOrg = this; // sort of defeats the purpose of fpOrg :-(
  }

  jetWithScale (const reco::CaloJet& jet, const double xscale = 1.)
    : reco::CaloJet (jet)
  {
    fScale = xscale;
    fpOrg = &jet;
  }

  double scale () const {return fScale;}
  double corrPt() const {return fScale * pt();}
  double rawPt() const {return pt();}
  const reco::CaloJet& orgJet() const {return *fpOrg;}

  friend bool operator < (const jetWithScale& lhs, const jetWithScale& rhs)
  {
    return lhs.corrPt() < rhs.corrPt();
  }

}; // class jetWithScale

////////////////////////////////////////////////////////////////////////////////
// class definition
////////////////////////////////////////////////////////////////////////////////

class CaloJetIDAnalyzer : public edm::EDAnalyzer
{
private:
  // typedefs
  typedef edm::Handle<trigger::TriggerEvent>   TrgEvtHandle_t;
  typedef edm::Handle<edm::TriggerResults>     TrgResHandle_t;
  typedef edm::Handle<reco::CaloJetCollection> CaloJetHandle_t;
  
public:
  // construction/destruction
  explicit CaloJetIDAnalyzer(const edm::ParameterSet& iConfig);
  virtual ~CaloJetIDAnalyzer();

private:
  // member functions
  void beginJob(const edm::EventSetup&);
  void beginRun(const edm::Run&,const edm::EventSetup&);
  void analyze(const edm::Event& event,const edm::EventSetup&);
  void endJob();
  void getCalComponents (const edm::Event& event, const reco::CaloJet &jet, 
			 vector< double > &HPD_energies,  vector< double > &RBX_energies, 
			 vector< double > &HBEF_energies, vector< double > &HO_energies,
			 vector< double > &ECal_energies,
			 vector< double > &HB_problem_energies, vector< int > &HB_problem_counts,
			 vector< double > &HE_problem_energies, vector< int > &HE_problem_counts,
			 vector< double > &HF_problem_energies, vector< int > &HF_problem_counts,
			 vector< double > &HO_problem_energies, vector< int > &HO_problem_counts,
			 vector< double > &EB_problem_energies, vector< int > &EB_problem_counts,
			 vector< double > &EE_problem_energies, vector< int > &EE_problem_counts,
			 double& pTrel, unsigned int& n50, unsigned int& n90, 
			 unsigned int& nHadHits, unsigned int& nEMHits,
			 unsigned int& nHadTowers, unsigned int& nEMTowers, unsigned int& nSD, int iDbg = 0);
  int calcMyIEta (int iEta, int depth); // untangles HB & HE readouts
  bool prettyClose (double x, double y);
 
  void classifyJetTowers( const edm::Event& event, const reco::CaloJet &jet, 
			  std::vector< reco::helper::JetIDHelper::subtower > &subtowers,      
			  std::vector< reco::helper::JetIDHelper::subtower > &Ecal_subtowers, 
			  std::vector< reco::helper::JetIDHelper::subtower > &Hcal_subtowers, 
			  std::vector< reco::helper::JetIDHelper::subtower > &HO_subtowers,
			  std::vector< double > &HPD_energies,  
			  std::vector< double > &RBX_energies,
			  unsigned int& n_bad_towers, unsigned int& n_bad_subtowers,
			  unsigned int& n_bad_ecal_cells, unsigned int& n_bad_hcal_cells,
			  unsigned int& n_recovered_towers, unsigned int& n_recovered_subtowers, 
			  unsigned int& n_recovered_ecal_cells, unsigned int& n_recovered_hcal_cells,
			  unsigned int& n_prob_towers, unsigned int& n_prob_subtowers, 
			  unsigned int& n_prob_ecal_cells, unsigned int& n_prob_hcal_cells,
			  double& E_bad_tower, double& E_bad_ecal, double& E_bad_hcal,
			  double& E_recovered_tower, double& E_recovered_ecal, double& E_recovered_hcal,
			  double& E_prob_tower, double& E_prob_ecal, double& E_prob_hcal,
			  const int iDbg = 0);


  // constants
  static int n_HBHE_flags, n_HF_flags, n_HO_flags, n_ECAL_flags;
  
private:
  // this method update the matching with information about matches to a particular trigger slot
  void matchTriggerObjsToJets(unsigned int itr,
			      const trigger::Keys& keys,
			      const TrgEvtHandle_t& triggerEvent,
			      const vector<jetWithScale>& jetsWithScale);
  
  void analyzeNonjetTriggers (const TrgEvtHandle_t& triggerEvent, const TrgResHandle_t& triggerResults);
  // select2nd exists only in some std and boost implementations, so let's control our own fate
  // and it can't be a non-static member function.
  static double select2nd (std::map<int,double>::value_type const &pair) {return pair.second;};
  static const reco::Track* getTrackPointer (reco::TrackRefVector::iterator::value_type const &pRefTrack) {
    return &(*pRefTrack);
  };

  // can't be a non-static member function (I think)
  static bool compareByPt (const TriggerObject& a, const TriggerObject& b) {return a.pt() < b.pt();};


  // from JetIDHelper's private functions (should probably make them public)
  enum Region{
    unknown_region = -1,
    HFneg, HEneg, HBneg, HBpos, HEpos, HFpos };
  int HBHE_oddness( int iEta );
  Region region( int iEta );

private:

  // This is (among other things) a di-jet tag & probe selector. Hence the number 2 is special. 
  // To increase readability whenever two is used because of di-jets, will use the constant;
  const static unsigned int two_ = 2;


  // Configuration options
  // ---------------------
  string            moduleName_;
  bool              useTracking_;
  bool              requireRandomTrigger_;

  edm::InputTag     srcTriggerResults_;
  edm::InputTag     srcTriggerEvent_;
  edm::InputTag     srcPrimVertex_;
  edm::InputTag     srcCaloJets_;
  edm::InputTag     srcCaloJetTrkAssoc_;
  edm::InputTag     srcTracks_; // for activity vetoes
  edm::InputTag     srcTrackJets_;
  edm::InputTag     srcHCALnoiseRemoval_;

  string            jecTag_;
  string            hltProcessName_;
  double            deltaRTrgMatch_;
  vector<string>    triggerNames_;
  vector<float>     triggerDeltaR_;
  HLTConfigProvider hltConfig_;

  bool use_hcal_noise_summary_;

  reco::helper::JetIDHelper jet_ID_helper_;

  // di jet sample(s) definitions
  double            maxTrackJetDeltaR_; // so the track jet will be matched to the probe jet
  double            max3rdJetPt_; // set to the highest conceivable value and tighten when analyzing tree
  double            maxDelta_; // ditto. Delta is defined as \pi - \delta_\phi
  double            minMatchTrackPt_; // set to the lowest practical value and tighten when analyzing tree
  double            minTrackPt_; // to be used to calculate properties. 1GeV is well motivated.
  // 3rd jet sample definitions
  double            maxLeadEta_;
  double            maxLeadDelta_;
  double            minLeadDPhi_;
  double            max4thPt_;
  // too many sample definitions
  unsigned int      tooManyIs_; // how many jets count as "too many"
  double            minJetPt_;  // (starting from which pT)
  // booking decisions
  bool              requireProbe_; // if so, only events with a bookable probe are booked.
  bool              bookAll_; // option to book "all" jets, for non-collision data used as a "bad" jet sample
  bool              avoidMuon_; // option to reduce "all" jets, requiring Pi/4 separation from trigger muon (axis of?)
  bool              bookTooMany_; // book the "too many jets" sample?
  bool              book3rdJet_; // book the "3rd jet" sample?
  bool              bookDiJets_; // book di jet tag&probe (with no tracking requirements)?
  bool              bookByTrackJet_; // book di jet tag&probe with a track-jet as part of the tag

  vector<double>    mcXsecs_;
  vector<double>    ptHatBins_;

  // Summaries
  // ---------
  
  vector<int>       triggerFired_;
  std::map<string, int>  nSamples_;
  std::set<string>  triggersSeen_, triggersUsedAsMu_;
  vector<TriggerObject> triggerMuObjects_;

  // Other member variables
  // ----------------------
  
  TH1F**            hTrgDeltaR_;
  TH1F**            hTrgObjCount_;
  TH1F*             pCutFlowHist0_;
  TH1F*             pCutFlowHist1_;
  TH1F*             pPtHatHist_;
  TH1F*             pPtHatBin_;
  TH2F*             pGoodBadJets_;
  
  int nDbgTrkPrintsLeft;

  unsigned int nJets_; // number of jets 
  bool isRandomTrigger_;

  // the output tree
  // ---------------
  const static unsigned int mpv_ = 100; // max # of P.V. to book
  const static unsigned int mmt_ = 100; // max # of (suspected) muon triggers to book
  const static unsigned int mjt_ = 100; // max # of jets to examine
  TTree*            tree_;
  unsigned int      run_;
  unsigned int      event_;
  unsigned int      trg_;
  float             pthat_;
  unsigned int      pthatbin_;
  float             xsec_;
  float             wmc_; // to be updated later
  unsigned int      hcal_word_;
  int               hcal_mhpd_;
  int               hcal_mrbx_;

  unsigned char     npv_;
  float             pvz_[mpv_];
  unsigned int      pvntrk_[mpv_];
  float             pvsumptsq_[mpv_];
  float             pvsumlogpt_[mpv_];

  unsigned char     nmt_;
  int               mtid_[mmt_];
  float             mtpt_[mmt_];
  float             mteta_[mmt_];
  float             mtphi_[mmt_];
 
  // tag and probe pairs
  unsigned char     njt_; // # of tag & probe pairs (to be stored in tree).
  // probe fields
  unsigned int      jtrank_[mjt_];
  float             jte_    [mjt_];   // uncorrected!
  float             jtpt_   [mjt_];
  float             jtrawpt_[mjt_];
  float             jtjes_  [mjt_];
  float             jteta_[mjt_];
  float             jtdeta_[mjt_];
  float             jtphi_[mjt_];
  float             jtemf_[mjt_];
  unsigned int      jtn90_[mjt_];
  unsigned int      jthn90_[mjt_]; // recHit (cell) level n90
  unsigned int      jthn50_[mjt_]; // ditto for 50%
  unsigned int      jtntwrs_[mjt_];
  unsigned int      jtnhits_[mjt_];
  unsigned int      jtnhh_[mjt_]; // # Hadronic hits
  unsigned int      jtneh_[mjt_]; // # EM        "
  unsigned int      jtnht_[mjt_]; // # Hadronic towers
  unsigned int      jtnet_[mjt_]; // # EM        "
  float             jtetaeta_[mjt_];
  float             jtphiphi_[mjt_];
  float             jtetaphi_[mjt_];
  float             jtptrel_[mjt_]; // a measure of the jet width linear in angle. Sum tower pT relative to jet axis
  unsigned int      jtntrk_[mjt_]; // conceptually part of the tag, but need to keep fields apart
  float             jtchf_[mjt_];  // conceptually part of the tag, but need to keep fields apart
  float             jthef_[mjt_];
  float             jthof_[mjt_]; // HO fraction
  float             jtf1hpd_[mjt_];
  float             jtf2hpd_[mjt_];
  float             jtf1rbx_[mjt_];
  float             jtf1h_[mjt_]; // E fraction in hardest Hcal recHit
  float             jtf2h_[mjt_]; // 2nd hardest                      
  float             jtf1e_[mjt_]; // ditto for ECal
  float             jtf2e_[mjt_]; // ...
  float             jtminhade_[mjt_]; 
  unsigned int      jttrg_[mjt_];   // not for the main study!
  float             jttrgpt_[mjt_]; //    ditto
  float             jttrgds_[mjt_]; //    ditto
  // from jet ID helper, AOD variations
  float             jtaodhpd_[mjt_];
  float             jtaodrbx_[mjt_];
  unsigned int      jtnhn90_[mjt_];
  //  float             jtebq1e_[mjt_]; // E fraction marked with EB quality bit #1
  unsigned int      jtnsd_[mjt_]; // number of contributing sub detectors
  float             jtfq_[mjt_];  // marked by quality bits
  unsigned int      jtnq_[mjt_];
  unsigned int      jtnsdq_[mjt_]; // number of subdetectors reporting problems
  float             jtfoot_[mjt_]; // Out Of Time
  unsigned int      jtnoot_[mjt_];
  unsigned int      jtnsdoot_[mjt_]; // # of subdetectors reporting Out Of Time issues
  unsigned int      jtnsat_[mjt_]; // # of saturated rec hits
  float             jtfsat_[mjt_]; // fraction of energy in saturated rec hits
  unsigned int      jtnsdsat_[mjt_]; 
  float             jtfls_[mjt_]; // energy fraction marked long-short (HF) bad
  unsigned int      jtnls_[mjt_]; // # of recHits marked long-short (HF) bad
  // tower level info
  unsigned int      jtnbt_[mjt_]; // # bad towers
  unsigned int      jtnbst_[mjt_]; // # bad sub-towers
  unsigned int      jtnbe_[mjt_]; // # of bad ECAL cels
  unsigned int      jtnbh_[mjt_]; // # of bad HCAL cels
  float             jtfbt_[mjt_]; // energy fraction in bad towers
  float             jtfbe_[mjt_]; // energy fraction in bad ECAL towers
  float             jtfbh_[mjt_]; // energy fraction in bad HCAL towers
  unsigned int      jtnrt_[mjt_]; // # recovered towers
  unsigned int      jtnrst_[mjt_]; // # recovered sub-towers
  unsigned int      jtnre_[mjt_]; // # of recovered ECAL cels
  unsigned int      jtnrh_[mjt_]; // # of recovered HCAL cels
  float             jtfrt_[mjt_]; // energy fraction in recovered towers
  float             jtfre_[mjt_]; // energy fraction in recovered ECAL towers
  float             jtfrh_[mjt_]; // energy fraction in recovered HCAL towers
  unsigned int      jtnpt_[mjt_]; // # problematic towers
  unsigned int      jtnpst_[mjt_]; // # problematic sub-towers
  unsigned int      jtnpe_[mjt_]; // # of problematic ECAL cels
  unsigned int      jtnph_[mjt_]; // # of problematic HCAL cels
  float             jtfpt_[mjt_]; // energy fraction in problematic towers
  float             jtfpe_[mjt_]; // energy fraction in problematic ECAL towers
  float             jtfph_[mjt_]; // energy fraction in problematic HCAL towers
  unsigned int      jtnft_[mjt_]; // # problematic towers
  unsigned int      jtnfst_[mjt_]; // # flagged sub-towers
  unsigned int      jtnfe_[mjt_]; // # of flagged ECAL cels
  unsigned int      jtnfh_[mjt_]; // # of flagged HCAL cels
  float             jtfft_[mjt_]; // energy fraction in flagged towers
  float             jtffe_[mjt_]; // energy fraction in flagged ECAL towers
  float             jtffh_[mjt_]; // energy fraction in flagged HCAL towers
  // tag fields on the probe side are named by side (though conceptually part of the tag)
  float             jtposf_[mjt_]; // might be useless
  unsigned int      jtntrkjet_[mjt_]; // number of track jets. Should be 1 in normal cases
  // these fields describe the leading track jet in the jet's region (may be part of tag)
  unsigned int      jttrkjetntrk_[mjt_]; // number of tracks (in leading track jet)
  float             jttrkjetpt_[mjt_];
  float             jttrkjeteta_[mjt_]; 
  float             jttrkjetphi_[mjt_];
  float             jttrkjetDR_[mjt_]; // relative to the jet itself
  float             jtmintrkjetDR_[mjt_]; // relative to the jet itself, for all track jets

  // tag fields
  //   1) what tag conditions does this probe jet have?
  bool              bydijet_   [mjt_];
  bool              bytrackjet_[mjt_];
  bool              byall_     [mjt_]; // just in case files from different runs are chained together
  bool              by3rd_     [mjt_];
  bool              bytoomany_ [mjt_];
  //   2) tag details (which ones are meaningfull depends on bools above
  float             tagdelta_[mjt_]; // pi - deltaPhi (tag, probe), which is conceptually part of the tag
  // unneeded?  float             tagDR_[mjt_]; // i.e. dR(Track-jet, probe-jet) = jttrkjetDR_ for probes by track jet
  unsigned int      tagtrg_[mjt_];
  float             tagtrgpt_[mjt_]; // the tag's trigger
  float             tagtrgds_[mjt_]; // the tag's trigger
  float             tage_[mjt_];
  float             tagpt_[mjt_];
  float             tagrawpt_[mjt_];
  float             tageta_[mjt_];
  float             tagphi_[mjt_];
  float             tagemf_[mjt_];
  unsigned int      tagntrk_[mjt_];
  float             tagchf_[mjt_];
  // other activity in probe region (same phi, other eta):
  float             omaxjetpt_[mjt_]; // max pT of other jets in probe region
  float             omaxtrkpt_ [mjt_]; // max pT of tracks in veto region
  float             otrkptsum_[mjt_]; //  see above
  unsigned int      ontrk_[mjt_];
  unsigned int      ontrkjet_[mjt_];
  float             omaxtrkjetpt_[mjt_];
  float             otrkjet1eta_[mjt_]; // eta of leading "other" track jet
  // veto fields that are the same for both possible pairs:
  float             vjet3pt_; // max pT of jets in veto region (i.e. all other jets)
  float             vjet3eta_;   // of same jet
  float             vjet3rawpt_; // ditto
  float             vjet3jes_;   // ditto
  float             set_; // raw SET excluding tag jets (e.g. the two tag & probe jets)
  float             met_; // raw MET excl....
  float             mett_; // ditto along the perp direction (see vjetptperp)
  float             nset_; // raw SET NoHF excl....
  float             nmet_; // raw MET  "     "     
  float             nmett_; // ditto along the perp direction (see vjetptperp)
  float             iset_;  //  .
  float             imet_;  //   .
  float             imett_; //    . the same, but including all tag jts
  float             inset_; //    ' that is, the uncorrected MET
  float             inmet_; //   '
  float             inmett_;//  ' 
  float             vjetptperp_;// sum pT perpendicular to tag-probe axis of jets in veto region (i.e. all other jets)
  float             vjetptlong_;// sum pT along tag-probe axis of jets in veto region (i.e. all other jets)
  float             vmaxtrkpt_ ; // max pT of tracks in veto region
  float             vtrkptperp_; //  see above
  float             vtrkptlong_; //  see above
  unsigned int      vntrkjet_[mjt_];
  float             vmaxtrkjetpt_[mjt_];
  float             vtrkjet1eta_[mjt_]; // of leading track jet in veto region
  float             vtrkjet1phi_[mjt_]; // ditto

  // internal book keeping for trigger matching (old: of two leading jets)
  vector<unsigned int> jetTriggerBits_; // LDBit for random trigger, others for jet triggers
  vector<float>        jetTriggerPt_;
  vector<float>        jetTriggerDs_;

  // internal book keeping: which track jet best matches each jet?
  // NYI (refactoring for 3rd jet) bool hasTrack_ [mjt_]; // jet is matched to a track
  int iMatchingTrackJet_ [mjt_]; // hardest track jet with min pT within dR cone
  int nMatchingTrackJets_ [mjt_]; // number of .... 
  // These variables are less stable than leading track jet kind of variables. Used only as sanity checks
  int iMinDRTrackJet_ [mjt_];


  // temporary track branches for debugging track selections
  const static unsigned int mtrk_ = 20;
  unsigned char ntrk_;
  float         trkpt_    [mtrk_];
  float         trkpterr_ [mtrk_];
  float         trketa_   [mtrk_];
  float         trkphi_   [mtrk_];
  unsigned int  trklyr_   [mtrk_]; // # of layers
  unsigned int  trkfound_ [mtrk_]; // # valid hits
  unsigned int  trklost_  [mtrk_];  // # invalid hits on track
  float         trkchisq_ [mtrk_]; 
  int           trkndf_   [mtrk_];  // of the chi^2
  float         trkdxy_   [mtrk_];  
  float         trkdxyerr_[mtrk_];
  float         trkdz_    [mtrk_];  
  float         trkdzerr_ [mtrk_];
  int           trkqual_  [mtrk_];
 

  // temporary track jet branches for debugging track jets
  const static unsigned int mtj_ = 20;
  unsigned     ntj_;
  unsigned int tjntrk_  [mtj_];
  float        tjpt_    [mtj_];
  float        tjeta_   [mtj_];
  float        tjphi_   [mtj_];
  unsigned int tjn90_   [mtj_];
 
};

int CaloJetIDAnalyzer::n_HBHE_flags = 14;
int CaloJetIDAnalyzer::n_HF_flags = 10;
int CaloJetIDAnalyzer::n_HO_flags = 4;
int CaloJetIDAnalyzer::n_ECAL_flags = 11;

////////////////////////////////////////////////////////////////////////////////
// coonstruction/destruction
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
CaloJetIDAnalyzer::CaloJetIDAnalyzer(const edm::ParameterSet& iConfig)
  : moduleName_(iConfig.getParameter<string>               ("@module_label"))
  , useTracking_(iConfig.getParameter<bool>                ("useTracking"))
  , requireRandomTrigger_(iConfig.getParameter<bool>       ("requireRandomTrigger"))
  , srcTriggerResults_(iConfig.getParameter<edm::InputTag> ("srcTriggerResults"))
  , srcTriggerEvent_(iConfig.getParameter<edm::InputTag>   ("srcTriggerEvent"))
  , srcPrimVertex_(iConfig.getParameter<edm::InputTag>     ("srcPrimVertex"))
  , srcCaloJets_(iConfig.getParameter<edm::InputTag>       ("srcCaloJets"))
  , srcCaloJetTrkAssoc_(iConfig.getParameter<edm::InputTag>("srcCaloJetTrkAssoc"))
  , srcTracks_(iConfig.getParameter<edm::InputTag>         ("srcTracks"))
  , srcTrackJets_(iConfig.getParameter<edm::InputTag>      ("srcTrackJets"))
  , srcHCALnoiseRemoval_(iConfig.getParameter<edm::InputTag>("srcHCALnoiseRemoval"))
  , jecTag_(iConfig.getParameter<string>                   ("jecTag"))
  , hltProcessName_(iConfig.getParameter<string>           ("hltProcessName"))
  , deltaRTrgMatch_(iConfig.getParameter<double>           ("deltaRTrgMatch"))
  , triggerNames_(iConfig.getParameter< vector<string> >   ("triggerNames"))
  , maxTrackJetDeltaR_(iConfig.getParameter<double>        ("maxTrackJetDeltaR"))
  , max3rdJetPt_(iConfig.getParameter<double>              ("max3rdJetPt"))
  , maxDelta_(iConfig.getParameter<double>                 ("maxDelta"))
  , minMatchTrackPt_(iConfig.getParameter<double>          ("minMatchTrackPt"))
  , minTrackPt_(iConfig.getParameter<double>               ("minTrackPt"))
  , maxLeadEta_(iConfig.getParameter<double>               ("maxLeadEta"))
  , maxLeadDelta_(iConfig.getParameter<double>             ("maxLeadDelta"))
  , minLeadDPhi_(iConfig.getParameter<double>              ("minLeadDPhi"))
  , max4thPt_(iConfig.getParameter<double>                 ("max4thJetPt"))
  , tooManyIs_(iConfig.getParameter<unsigned int>          ("tooManyIs"))
  , minJetPt_(iConfig.getParameter<double>                 ("minJetPt"))
  , requireProbe_(iConfig.getParameter<bool>               ("requireProbe"))
  , bookAll_(iConfig.getParameter<bool>                    ("bookAll"))
  , avoidMuon_(iConfig.getParameter<bool>                  ("avoidMuon"))
  , bookTooMany_(iConfig.getParameter<bool>                ("bookTooMany"))
  , book3rdJet_(iConfig.getParameter<bool>                 ("book3rd"))
  , bookDiJets_(iConfig.getParameter<bool>                 ("bookDiJets"))
  , bookByTrackJet_(iConfig.getParameter<bool>             ("bookByTrackJet"))
  , mcXsecs_(iConfig.getParameter< vector<double> >        ("mcXsecs"))
  , ptHatBins_(iConfig.getParameter< vector<double> >      ("ptHatBins"))
{
  nDbgTrkPrintsLeft = 20;

  use_hcal_noise_summary_ = true;
  edm::ParameterSet jet_ID_helper_config = iConfig.getParameter<edm::ParameterSet>( "jetIDHelperConfig" );
  jet_ID_helper_ = reco::helper::JetIDHelper( jet_ID_helper_config );

  cout<<",------------- CaloJetIDAnalyzer [ "<<moduleName_<<" ] --------------------"
      <<"\n book dijets? "<<bookDiJets_<<", by trackjet? "<<bookByTrackJet_
      <<"\n 3rd jet? "<<book3rdJet_<<"; too many? "<<bookTooMany_<<"; all? "<<bookAll_;
  if( bookAll_) cout<<", avoidMu? "<<avoidMuon_;
  if( bookTooMany_) cout<<", tooManyIs: "<<tooManyIs_<<" with pT>"<<minJetPt_;
  if( book3rdJet_) cout<<"\n Third jet definitions: |eta_{1,2}|<"<<maxLeadEta_<<", minDPhi: "<<minLeadDPhi_
		       <<", maxLeadDelta: "<<maxLeadDelta_<<", max4thJetPt: "<<max4thPt_;
  cout<<"\n jet ID helper config: "<<jet_ID_helper_config;
  cout<<"\n Trigger results source: \""<<srcTriggerResults_<<"\""
      <<"\n                 event: \""<<srcTriggerEvent_<<"\""
      <<"\n requireRandomTrigger? "<<requireRandomTrigger_<<", jet-trigger deltaR: "<<deltaRTrgMatch_
      <<"\n HLT process: \""<<hltProcessName_<<"\", trigger names: ";
  for (unsigned int i=0; i<triggerNames_.size(); ++i) cout<<triggerNames_[i]<<" ";
  cout<<"\n Jet source: \""<<srcCaloJets_<<"\""
      <<"\n Jet correction service: \""<<jecTag_<<"\""<<endl;
  if( useTracking_) {
    cout<<" P.V. source: \""<<srcPrimVertex_<<"\""<<endl;
    cout<<" Track jet source: \""<<srcTrackJets_<<"\"\n maxTrackJetDeltaR: "<<maxTrackJetDeltaR_<<endl;
    cout<<" Tracks source: \""<<srcTracks_<<"\""<<endl;
    if( ! (srcCaloJetTrkAssoc_ == edm::InputTag("")))  // prevents copying (& disabling) others, but can be disabled itself
      cout<<" track-jet associations source: \""<<srcCaloJetTrkAssoc_<<"\""
	  <<"\n         minPt: "<<minTrackPt_<<", for match: "<<minMatchTrackPt_<<endl;
  }
  if( use_hcal_noise_summary_ ) cout<<"Using hcal noise summaries from source: \""<<srcHCALnoiseRemoval_<<"\""<<endl;
  cout<<"\n max3rdJetPt: "<<max3rdJetPt_<<", maxDelta: "<<maxDelta_<<", requireProbe (to book)? "<<requireProbe_
      <<"\n mcXsecs: ";
  for (unsigned int i=0; i<mcXsecs_.size(); ++i) cout<<mcXsecs_[i]<<" ";
  cout<<"\n ptHatBins: ";
  for (unsigned int i=0; i<ptHatBins_.size(); ++i) cout<<ptHatBins_[i]<<" ";
  cout<<"\n._____________________________________________________________________________"
      <<endl;

  // quick sanity checks
  if( tooManyIs_ < 1) edm::LogError("BadInput")<<"tooManyIs must be positive";
  if( two_ != 2) edm::LogError("CodeAssumptionsViolated")<<"Bug! two_ != 2, but code assumes 2 throughout";
  if( ptHatBins_.size() > 0 && ptHatBins_.size() != 1 + mcXsecs_.size()) {
    edm::LogError("BadInput")<<"Inconsistant MC bins:"<<ptHatBins_.size()<<"bin edges, but"<<mcXsecs_.size()<<"bins";
  }
  for (unsigned int ibin = 1; ibin < ptHatBins_.size(); ++ibin)
    if( ptHatBins_[ibin] <= ptHatBins_[ibin-1]) 
      edm::LogError("BadInput")<<"ptHatBins must increase monotonously";
  nSamples_[string("dijet")] = nSamples_[string("trackjet")] = nSamples_[string("toomany")] 
    = nSamples_[string("3rdjet")] = nSamples_[string("all")] = nSamples_[string("randomTrigger")]
    = nSamples_[string("analyzed")] = 0;
}


//______________________________________________________________________________
CaloJetIDAnalyzer::~CaloJetIDAnalyzer()
{
  
}



////////////////////////////////////////////////////////////////////////////////
// implementation of member functions
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
void CaloJetIDAnalyzer::beginJob(const edm::EventSetup&)
{
  // interpret trigger names and set trigger object matching requirements
  for (unsigned int itrg=0;itrg<triggerNames_.size();itrg++) {
    string trigger = triggerNames_[itrg];
    unsigned int pos = trigger.find(":");
    if( pos!=string::npos) {
      double dr; std::stringstream ss; ss<<trigger.substr(pos+1); ss>>dr;
      triggerNames_[itrg]=trigger.substr(0,pos);
      triggerDeltaR_.push_back(dr);
    }
    else {
      triggerDeltaR_.push_back(deltaRTrgMatch_);
    }
  }
  
  // retrieve file service for output root files
  edm::Service<TFileService> fs;
  if( !fs) throw edm::Exception(edm::errors::Configuration,
				"TFileService missing from configuration!");
  
  // book histograms
  pPtHatHist_    = fs->make<TH1F> ("hPtHat", "#hat{p_{T}}", 5000, 0, 5000);
  pCutFlowHist0_ = fs->make<TH1F> ("cutFlow0", "Tag & Probe cut flow [leading jet]",    10, 0.5, 10.5);
  pCutFlowHist1_ = fs->make<TH1F> ("cutFlow1", "Tag & Probe cut flow [subleading jet]", 10, 0.5, 10.5);
  pGoodBadJets_  = fs->make<TH2F> ("hGoodBadJets", "#matched vs. #unmatched jets {too many jets}", 
				   31, -0.5, 30.5, 31, -0.5, 30.5);
  hTrgDeltaR_   = new TH1F*[triggerNames_.size()];
  hTrgObjCount_ = new TH1F*[triggerNames_.size()];
  for (unsigned int ihist=0;ihist<triggerNames_.size();ihist++) {
    triggerFired_.push_back(0);
    string hname = "hTrgDeltaR_"+triggerNames_[ihist];
    hTrgDeltaR_[ihist] = fs->make<TH1F>(hname.c_str(),"",200,0,1.0);
    hname = "hTrgObjCount_"+triggerNames_[ihist];
    hTrgObjCount_[ihist] = fs->make<TH1F>(hname.c_str(),"",11,-0.5,10.5);
  }
  
  // create and define output tree
  tree_=fs->make<TTree>("t","t");
  
  tree_->Branch("run",       &run_,      "run/i");
  tree_->Branch("event",     &event_,    "event/i");
  tree_->Branch("trg",       &trg_,      "trg/i");
  tree_->Branch("pthat",     &pthat_,    "pthat/F");
  tree_->Branch("pthatbin",  &pthatbin_, "pthatbin/i");
  tree_->Branch("xsec",      &xsec_,     "xsec/F");
  tree_->Branch("w",         &wmc_,      "w/F");
  tree_->Branch("hcal",      &hcal_word_,"hcal/i");
  tree_->Branch("mhpd",      &hcal_mhpd_,"mhpd/I");
  tree_->Branch("mrbx",      &hcal_mrbx_,"mrbx/I");
  
  tree_->Branch("npv",       &npv_,      "npv/b");
  tree_->Branch("pvz",       pvz_,       "pvz[npv]/F");
  tree_->Branch("pvntrk",    pvntrk_,    "pvntrk[npv]/i");
  tree_->Branch("pvsumptsq", pvsumptsq_, "pvsumptsq[npv]/F");
  tree_->Branch("pvsumlogpt",pvsumlogpt_,"pvsumlogpt[npv]/F");
  
  tree_->Branch("nmt",       &nmt_,      "nmt/b");
  tree_->Branch("mtid",      mtid_,      "mtid[nmt]/I");
  tree_->Branch("mtpt",      mtpt_,      "mtpt[nmt]/F");
  tree_->Branch("mteta",     mteta_,     "mteta[nmt]/F");
  tree_->Branch("mtphi",     mtphi_,     "mtphi[nmt]/F");
  
  tree_->Branch("njt",       &njt_,      "njt/b");
  // probe fields
  tree_->Branch("jtrank",    jtrank_,    "jtrank[njt]/i");
  tree_->Branch("jte",       jte_,       "jte[njt]/F");
  tree_->Branch("jtpt",      jtpt_,      "jtpt[njt]/F");
  tree_->Branch("jtrawpt",   jtrawpt_,   "jtrawpt[njt]/F");
  tree_->Branch("jtjes",     jtjes_,     "jtjes[njt]/F");
  tree_->Branch("jteta",     jteta_,     "jteta[njt]/F");
  tree_->Branch("jtdeta",    jtdeta_,    "jtdeta[njt]/F");
  tree_->Branch("jtphi",     jtphi_,     "jtphi[njt]/F");
  tree_->Branch("jtemf",     jtemf_,     "jtemf[njt]/F");
  tree_->Branch("jtn90",     jtn90_,     "jtn90[njt]/i");
  tree_->Branch("jthn90",    jthn90_,    "jthn90[njt]/i");
  tree_->Branch("jthn50",    jthn50_,    "jthn50[njt]/i");
  tree_->Branch("jtntwrs",   jtntwrs_,   "jtntwrs[njt]/i");
  tree_->Branch("jtnhits",   jtnhits_,   "jtnhits[njt]/i");
  tree_->Branch("jtnhh",     jtnhh_,     "jtnhh[njt]/i");
  tree_->Branch("jtneh",     jtneh_,     "jtneh[njt]/i");
  tree_->Branch("jtnht",     jtnht_,     "jtnht[njt]/i");
  tree_->Branch("jtnet",     jtnet_,     "jtnet[njt]/i");
  tree_->Branch("jtetaeta",  jtetaeta_,  "jtetaeta[njt]/F");
  tree_->Branch("jtphiphi",  jtphiphi_,  "jtphiphi[njt]/F");
  tree_->Branch("jtetaphi",  jtetaphi_,  "jtetaphi[njt]/F");
  tree_->Branch("jtptrel",   jtptrel_,   "jtptrel[njt]/F");
  tree_->Branch("jtntrk",    jtntrk_,    "jtntrk[njt]/i");
  tree_->Branch("jtchf",     jtchf_,     "jtchf[njt]/F");
  tree_->Branch("jthef",     jthef_,     "jthef[njt]/F");
  tree_->Branch("jthof",     jthof_,     "jthof[njt]/F");
  tree_->Branch("jtf1hpd",   jtf1hpd_,   "jtf1hpd[njt]/F");
  tree_->Branch("jtf2hpd",   jtf2hpd_,   "jtf2hpd[njt]/F");
  tree_->Branch("jtf1rbx",   jtf1rbx_,   "jtf1rbx[njt]/F");
  tree_->Branch("jtf1h",     jtf1h_,     "jtf1h[njt]/F");
  tree_->Branch("jtf2h",     jtf2h_,     "jtf2h[njt]/F");
  tree_->Branch("jtf1e",     jtf1e_,     "jtf1e[njt]/F");
  tree_->Branch("jtf2e",     jtf2e_,     "jtf2e[njt]/F");
  tree_->Branch("jtminhade", jtminhade_, "jtminhade[njt]/F");
  tree_->Branch("jttrg",     jttrg_,     "jttrg[njt]/i");
  tree_->Branch("jttrgpt",   jttrgpt_,   "jttrgpt[njt]/F");
  tree_->Branch("jttrgds",   jttrgds_,   "jttrgds[njt]/F");
  tree_->Branch("jtaodhpd",  jtaodhpd_,  "jtaodhpd[njt]/F");
  tree_->Branch("jtaodrbx",  jtaodrbx_,  "jtaodrbx[njt]/F");
  tree_->Branch("jtnhn90",   jtnhn90_,   "jtnhn90[njt]/i");
  tree_->Branch("jtnsd",     jtnsd_,     "jtnsd[njt]/i");
  tree_->Branch("jtfq",      jtfq_,      "jtfq[njt]/F");
  tree_->Branch("jtnq",      jtnq_,      "jtnq[njt]/i");
  tree_->Branch("jtnsdq",    jtnsdq_,    "jtnsdq[njt]/i");
  tree_->Branch("jtfoot",    jtfoot_,    "jtfoot[njt]/F");
  tree_->Branch("jtnoot",    jtnoot_,    "jtnoot[njt]/i");
  tree_->Branch("jtnsdoot",  jtnsdoot_,  "jtnsdoot[njt]/i");
  tree_->Branch("jtnsat",    jtnsat_,    "jtnsat[njt]/i");
  tree_->Branch("jtfsat",    jtfsat_,    "jtfsat[njt]/F");
  tree_->Branch("jtnsdsat",  jtnsdsat_,  "jtnsdsat[njt]/i"); 
  tree_->Branch("jtfls",     jtfls_,     "jtfls[njt]/F");
  tree_->Branch("jtnls",     jtnls_,     "jtnls[njt]/i");
  tree_->Branch("jtnbt",     jtnbt_,     "jtnbt[njt]/i");
  tree_->Branch("jtnbst",    jtnbst_,    "jtnbst[njt]/i");
  tree_->Branch("jtnbe",     jtnbe_,     "jtnbe[njt]/i");
  tree_->Branch("jtnbh",     jtnbh_,     "jtnbh[njt]/i");
  tree_->Branch("jtfbt",     jtfbt_,     "jtfbt[njt]/F");
  tree_->Branch("jtfbe",     jtfbe_,     "jtfbe[njt]/F");
  tree_->Branch("jtfbh",     jtfbh_,     "jtfbh[njt]/F");
  tree_->Branch("jtnrt",     jtnrt_,     "jtnrt[njt]/i");
  tree_->Branch("jtnrst",    jtnrst_,    "jtnrst[njt]/i");
  tree_->Branch("jtnre",     jtnre_,     "jtnre[njt]/i");
  tree_->Branch("jtnrh",     jtnrh_,     "jtnrh[njt]/i");
  tree_->Branch("jtfrt",     jtfrt_,     "jtfrt[njt]/F");
  tree_->Branch("jtfre",     jtfre_,     "jtfre[njt]/F");
  tree_->Branch("jtfrh",     jtfrh_,     "jtfrh[njt]/F");
  tree_->Branch("jtnpt",     jtnpt_,     "jtnpt[njt]/i");
  tree_->Branch("jtnpst",    jtnpst_,    "jtnpst[njt]/i");
  tree_->Branch("jtnpe",     jtnpe_,     "jtnpe[njt]/i");
  tree_->Branch("jtnph",     jtnph_,     "jtnph[njt]/i");
  tree_->Branch("jtfpt",     jtfpt_,     "jtfpt[njt]/F");
  tree_->Branch("jtfpe",     jtfpe_,     "jtfpe[njt]/F");
  tree_->Branch("jtfph",     jtfph_,     "jtfph[njt]/F");
  tree_->Branch("jtnft",     jtnft_,     "jtnft[njt]/i");
  tree_->Branch("jtnfst",    jtnfst_,    "jtnfst[njt]/i");
  tree_->Branch("jtnfe",     jtnfe_,     "jtnfe[njt]/i");
  tree_->Branch("jtnfh",     jtnfh_,     "jtnfh[njt]/i");
  tree_->Branch("jtfft",     jtfft_,     "jtfft[njt]/F");
  tree_->Branch("jtffe",     jtffe_,     "jtffe[njt]/F");
  tree_->Branch("jtffh",     jtffh_,     "jtffh[njt]/F");
  tree_->Branch("jtposf",    jtposf_,    "jtposf[njt]/F"); // = sum pt in pos tracks / sum pt in all tracks
  tree_->Branch("ntj",       jtntrkjet_, "ntj[njt]/i");
  tree_->Branch("tjntrk",    jttrkjetntrk_, "tjntrk[njt]/i"); // these fields are for the leading track jet
  tree_->Branch("tjpt",      jttrkjetpt_  , "tjpt[njt]/F");
  tree_->Branch("tjeta",     jttrkjeteta_ , "tjeta[njt]/F");
  tree_->Branch("tjphi",     jttrkjetphi_ , "tjphi[njt]/F");
  tree_->Branch("dr",        jttrkjetDR_  , "dr[njt]/F");
  tree_->Branch("mindr",     jtmintrkjetDR_  , "mindr[njt]/F"); // this one is for all track jets
  // tag fields
  tree_->Branch("dj",        bydijet_,   "dj[njt]/O");
  tree_->Branch("tj",        bytrackjet_,"tj[njt]/O");
  tree_->Branch("all",       byall_,     "all[njt]/O");
  tree_->Branch("j3",        by3rd_,     "j3[njt]/O");
  tree_->Branch("tm",        bytoomany_, "tm[njt]/O");
  tree_->Branch("tagdelta",  tagdelta_,  "tagdelta[njt]/F");
  tree_->Branch("tagtrg",    tagtrg_,    "tagtrg[njt]/i");
  tree_->Branch("tagtrgpt",  tagtrgpt_,  "tagtrgpt[njt]/F");
  tree_->Branch("tagtrgds",  tagtrgds_,  "tagtrgds[njt]/F");
  tree_->Branch("tage",      tage_,      "tage[njt]/F");
  tree_->Branch("tagpt",     tagpt_,     "tagpt[njt]/F");
  tree_->Branch("tagrawpt",  tagrawpt_,  "tagrawpt[njt]/F");
  tree_->Branch("tageta",    tageta_,    "tageta[njt]/F");
  tree_->Branch("tagphi",    tagphi_,    "tagphi[njt]/F");
  tree_->Branch("tagemf",    tagemf_,    "tagemf[njt]/F");
  tree_->Branch("tagntrk",   tagntrk_,   "tagntrk[njt]/i");
  tree_->Branch("tagchf",    tagchf_,    "tagchf[njt]/F");
  // other activity in probe region fields
  tree_->Branch("omaxjetpt", omaxjetpt_, "omaxjetpt[njt]/F");
  tree_->Branch("omaxtrkpt", omaxtrkpt_, "omaxtrkpt[njt]/F");
  tree_->Branch("otrkptsum", otrkptsum_, "otrkptsum[njt]/F");
  tree_->Branch("ontrk",     ontrk_,     "ontrk[njt]/i");
  tree_->Branch("ontj",      ontrkjet_,  "ontj[njt]/i");
  tree_->Branch("omaxtjpt",  omaxtrkjetpt_, "omaxtjpt[njt]/F");
  tree_->Branch("otj1eta",   otrkjet1eta_ , "otj1eta[njt]/F");
  // veto fields
  tree_->Branch("vjet3pt",     &vjet3pt_,    "vjet3pt/F");
  tree_->Branch("vjet3eta",    &vjet3eta_,   "vjet3eta/F");
  tree_->Branch("vjet3rawpt",  &vjet3rawpt_, "vjet3rawpt/F");
  tree_->Branch("vjet3jes",    &vjet3jes_,   "vjet3jes/F");
  tree_->Branch("set",         &set_,        "set/F");
  tree_->Branch("met",         &met_,        "met/F");
  tree_->Branch("mett",        &mett_,       "mett/F");
  tree_->Branch("nset",        &nset_,       "nset/F");
  tree_->Branch("nmet",        &nmet_,       "nmet/F");
  tree_->Branch("nmett",       &nmett_,      "nmett/F");
  tree_->Branch("iset",        &iset_,       "iset/F");
  tree_->Branch("imet",        &imet_,       "imet/F");
  tree_->Branch("imett",       &imett_,      "imett/F");
  tree_->Branch("inset",       &inset_,      "inset/F");
  tree_->Branch("inmet",       &inmet_,      "inmet/F");
  tree_->Branch("inmett",      &inmett_,     "inmett/F");
  tree_->Branch("vjetperp",    &vjetptperp_, "vjetperp/F");
  tree_->Branch("vjetlong",    &vjetptlong_, "vjetlong/F");
  tree_->Branch("vmaxtrkpt",   &vmaxtrkpt_,  "vmaxtrkpt/F");
  tree_->Branch("vtrkperp",    &vtrkptperp_, "vtrkperp/F");
  tree_->Branch("vtrklong",    &vtrkptlong_, "vtrklong/F");


  // debug tracking dump
  tree_->Branch("ntrk",      &ntrk_,     "ntrk/b");
  tree_->Branch("trkpt",     trkpt_,     "trkpt[ntrk]/F");
  tree_->Branch("trkpterr",  trkpterr_,  "trkpterr[ntrk]/F");
  tree_->Branch("trketa",    trketa_,    "trketa[ntrk]/F");
  tree_->Branch("trkphi",    trkphi_,    "trkphi[ntrk]/F");
  tree_->Branch("trklyr",    trklyr_,    "trklyr[ntrk]/i");
  tree_->Branch("trkfound",  trkfound_,  "trkfound[ntrk]/i");
  tree_->Branch("trklost",   trklost_,   "trklost[ntrk]/i");
  tree_->Branch("trkchisq",  trkchisq_,  "trkchisq[ntrk]/F");
  tree_->Branch("trkndf",    trkndf_,    "trkndf[ntrk]/I");
  tree_->Branch("trkdxy",    trkdxy_,    "trkdxy[ntrk]/F");
  tree_->Branch("trkdxyerr", trkdxyerr_, "trkdxyerr[ntrk]/F");
  tree_->Branch("trkdz",     trkdz_,     "trkdz[ntrk]/F");
  tree_->Branch("trkdzerr",  trkdzerr_,  "trkdzerr[ntrk]/F");
  tree_->Branch("trkqual",   trkqual_,   "trkqual[ntrk]/I");

  //debug track jet dump
  tree_->Branch("dntj",      &ntj_,     "dntj/b");
  tree_->Branch("dtjpt",     tjpt_,     "dtjpt[dntj]/F");
  tree_->Branch("dtjeta",    tjeta_,    "dtjeta[dntj]/F");
  tree_->Branch("dtjphi",    tjphi_,    "dtjphi[dntj]/F");
  tree_->Branch("dtjntrk",   tjntrk_,   "dtjntrk[dntj]/i");
  tree_->Branch("dtjn90",    tjn90_,    "dtjn90[dntj]/i");
}

//______________________________________________________________________________
void CaloJetIDAnalyzer::beginRun(const edm::Run&,const edm::EventSetup&)
{
  if( hltConfig_.init(hltProcessName_)) {
    unsigned int ntrg    = triggerNames_.size();
    unsigned int ntrgtot = hltConfig_.size();
    for (unsigned int itrg=0;itrg<ntrg;itrg++) {
      string       trigger = triggerNames_[itrg];
      unsigned int itrghlt = hltConfig_.triggerIndex(trigger);
      if( itrghlt>=ntrgtot)
	edm::LogWarning("CaloJetIDAnalyzer")<<"trigger '"<<trigger
					    <<"' not available!";
      else
	LogDebug("CaloJetIDAnalyzer")<<"trigger '"<<trigger<<"' found.";
    }
    //LogDebug("Trigger")<<"configured HLT path in this run:";
    LogTrace("DebugTrigger")<<"DBG: configured HLT path in this run: "<<std::flush;
    for (unsigned int itrg=0; itrg<ntrgtot; ++itrg) {
      //LogTrace("Trigger")<<itrg<<")"<<hltConfig_.triggerName(itrg);
      LogTrace("DebugTrigger")<<"("<<itrg<<") "<<hltConfig_.triggerName(itrg)<<", ";
    }
    LogTrace("DebugTrigger")<<endl;
  }
  else {
    edm::LogError("CaloJetIDAnalyzer")<<"failed to extract HLT config info for "
				      <<"process '"<<hltProcessName_<<"'";
  }
}


//______________________________________________________________________________
void CaloJetIDAnalyzer::analyze(const edm::Event& event,const edm::EventSetup& eventSetup)
{
  ++nSamples_["analyzed"];

  // get the per-event inputs
  // ------------------------
  // trigger
  trg_=0;
  TrgResHandle_t triggerResults;
  TrgEvtHandle_t triggerEvent;
  event.getByLabel(srcTriggerResults_,triggerResults);
  event.getByLabel(srcTriggerEvent_,  triggerEvent);
  unsigned int ntrg=triggerNames_.size();
  unsigned int ntrgtot=hltConfig_.size();

  analyzeNonjetTriggers (triggerEvent, triggerResults);
  if( requireRandomTrigger_ && ! isRandomTrigger_) return;
  if( isRandomTrigger_) ++nSamples_["randomTrigger"];

  hcal_word_ = 0;
  hcal_mrbx_ = hcal_mhpd_ = 0;
  if( use_hcal_noise_summary_ ) {
    edm::Handle<HcalNoiseSummary> hcal_noise_summary_handle;
    bool OK = event.getByLabel( srcHCALnoiseRemoval_, hcal_noise_summary_handle);
    if( !OK || !hcal_noise_summary_handle.isValid() ) 
      cerr<<"Failed to get the HCAL noise summary: "<<srcHCALnoiseRemoval_<<". OK? "<<OK<<endl;
    const HcalNoiseSummary hcal_noise_summary = *hcal_noise_summary_handle;
    ++hcal_word_;
    if( hcal_noise_summary.passLooseNoiseFilter() ) hcal_word_ |= 0x2;
    if( hcal_noise_summary.passTightNoiseFilter() ) hcal_word_ |= 0x4;
    hcal_mhpd_ = hcal_noise_summary.maxHPDHits();
    hcal_mrbx_ = hcal_noise_summary.maxRBXHits();
  }

  // must keep track of pT hat of every event (for MC weights)
  edm::Handle<double> genEventScale;
  bool OK = event.getByLabel("genEventScale",genEventScale);
  pthat_ = OK ? (*genEventScale) : -666.;
  pPtHatHist_ -> Fill (pthat_);
  pthatbin_ = 0; // this is a ROOT like index, so bin=0 is underflow
  for (; pthatbin_ < ptHatBins_.size() && ptHatBins_[pthatbin_] < pthat_; ++pthatbin_);
  xsec_ = (pthatbin_ > 0 && pthatbin_ <= mcXsecs_.size()) 
    ? mcXsecs_ [pthatbin_ - 1] 
    : 0; // to be used as a weight when plotting MC: 0 = ignore.
  wmc_ = xsec_; // will be updated later using # of events
    
  reco::JetTracksAssociation::Container caloJetsToTrks;
  if( useTracking_ && ! (srcCaloJetTrkAssoc_ == edm::InputTag(""))) {
    edm::Handle< reco::JetTracksAssociation::Container > handleToCaloJetsToTrks;
    event.getByLabel(srcCaloJetTrkAssoc_, handleToCaloJetsToTrks);
    caloJetsToTrks = *handleToCaloJetsToTrks;
  }
  // primary vertex
  edm::Handle< reco::VertexCollection> primVertices; // left in its illegal state if tracking is not used
  if( useTracking_) event.getByLabel(srcPrimVertex_, primVertices);
  // tracks
  edm::Handle<reco::TrackCollection> tracks;
  if( useTracking_) event.getByLabel(srcTracks_, tracks);
// MET & SET
  edm::Handle<CaloMETCollection> caloMETs;
  event.getByLabel ("met", caloMETs ); // "met" is raw or uncorrected CaloMETCollection
  //version 1 - didn't work:  CaloMET* met = (CaloMET*)METColl[0]; // get the object from the wrapper collection
  //version 2 - works but ugly:
  //  const CaloMETCollection *pMETColl = caloMETs.product();
  //  const CaloMET met (pMETColl->front()); // get the object from the wrapper collection
  // verstion 3:
  const CaloMET met (caloMETs->at(0)); // get the first MET out of the METs (pointed to by the smart pointer edm::Handle)
  //double missing_transverse_energy = met.pt();
  //double missing_transverse_energy_phi = met.phi();
  //double scalar_transverse_energy = met.sumEt();
  //double em_fraction_of_event = met.emEtFraction();  
  edm::Handle<CaloMETCollection> caloMETnoHFs;
  event.getByLabel ("metNoHF", caloMETnoHFs ); // raw or uncorrected CaloMETCollection excluding HF
  const CaloMETCollection *pMETCollNoHF = caloMETnoHFs.product();
  const CaloMET metNoHF (pMETCollNoHF->front()); // get the object from the wrapper collection
  //double missing_transverse_energy_noHF = metNoHF.pt();
  //double missing_transverse_energy_noHF_phi = metNoHF.phi();
  //double scalar_transverse_energy_noHF = metNoHF.sumEt();
  //double em_fraction_of_event_noHF = metNoHF.emEtFraction();  
  
  // track jets stuff taken from https://twiki.cern.ch/twiki/bin/view/CMS/TrackJets (but modified for optional use)
  edm::Handle<BasicJetCollection> trackJets; 
  bool useTrackJets = useTracking_ && ! (srcTrackJets_ == std::string("") );
  cout<<"AHDBG useTracking_: "<<useTracking_<<" srcTrackJets_: "<<srcTrackJets_<<" == \"\"? "<<useTrackJets<<endl;
  if( useTrackJets ) event.getByLabel (srcTrackJets_, trackJets);

  LogDebug("Flow")<<"read event inputs (except caloJets)";

  vector<jetWithScale> jetsWithScale;
  //  edm::Handle< reco::CaloJetCollection >               caloJets; // temp. for testing assoc. problems
  { // this block exists to make the caloJets local
    edm::Handle< reco::CaloJetCollection >               caloJets; // temp. see above.
    event.getByLabel(srcCaloJets_,       caloJets);
    nJets_ = caloJets->size();
    if( nJets_ <= 0) return; // nothing to probe in this event
    { // a quick sanity check
      double prevPt = -1;
      for (unsigned int ijt=0; ijt < caloJets->size(); ijt++) {
	double curPt = caloJets->at(ijt).pt();
	if( ijt >= 1 && curPt > prevPt) edm::LogError("UnexpectedEventContents")<<
					  "Code assumes input caloJets are sorted in pT in descending order";
	prevPt = curPt;
	if( curPt <= 0) edm::LogError("UnexpectedEventContents")<<"Code assumes input caloJets have positive pT";
      }
    }
    LogDebug("eventNumber")<<"run:"<<event.id().run()<<", event:"<<event.id().event()
			   <<", #jets:"<<nJets_
			   <<", #tracks:"    <<(useTracking_ ? (int)    tracks->size() : -666)
			   <<", #track jets:"<<(useTrackJets ? (int) trackJets->size() : -666);

    // Find jet energy corrections
    const JetCorrector* corrector = JetCorrector::getJetCorrector (jecTag_, eventSetup);
    for (CaloJetCollection::const_iterator jet = caloJets->begin(); jet != caloJets->end(); ++jet) {
      // calculate the correction without applying it (since no jet object is passed to corrector....)
      double scale = corrector->correction(jet->p4());
      if( scale <= 0) {
	edm::LogError("BadJetCorrection")<<"Code assumes jet corrections are positive!";
	scale = 0.000001;
      }
      // create my "jetWithScale" object
      jetsWithScale.push_back (jetWithScale(*jet, scale));
    }

    // Sort jets by their corrected energies
    std::sort (jetsWithScale.rbegin(), jetsWithScale.rend()); // note operator < is defined for this class...
    { // a quick sanity check
      if( jetsWithScale.size() != caloJets->size()) edm::LogError("Bug")<<"size mismatch between jetsWithScale and caloJets";
      double prevPt = -1;
      for (unsigned int ijt=0; ijt < jetsWithScale.size(); ijt++) {
	double curPt = jetsWithScale.at(ijt).corrPt();
	if( ijt >= 1 && curPt > prevPt) edm::LogError("Bug")<<"Sorting the jetsWithScale failed"<<ijt<<")"
							    <<jetsWithScale.at(ijt).rawPt()<<"*"
							    <<jetsWithScale.at(ijt).scale()<<"="<<curPt<<">"<<prevPt;
	prevPt = curPt;
	if( curPt <= 0) edm::LogError("Bug")<<"Corrected pT is non positive";
      }
    }
  }
  LogTrace("Flow")<<"DBG: prepared jetsWithScale "<<endl;


  // Find (suspected) muon triggers
  nmt_ = TMath::Min (mmt_, triggerMuObjects_.size());
  for (int imt=0; imt < nmt_; ++imt) {
    mtid_ [imt] = triggerMuObjects_[imt].id();
    mtpt_ [imt] = triggerMuObjects_[imt].pt();
    mteta_[imt] = triggerMuObjects_[imt].eta();
    mtphi_[imt] = triggerMuObjects_[imt].phi();
  }

  // figure out the trigger matching
  // -------------------------------
  jetTriggerBits_.clear();
  jetTriggerPt_  .clear();
  jetTriggerDs_  .clear();
  //Trigger information for the soft jets may be useful in "all" and "toomany" modes
  //unsigned int nJetsToDo = TMath::Min (two_, jetsWithScale.size());
  unsigned int nJetsToDo = jetsWithScale.size();

  for (unsigned int ijt=0; ijt < nJetsToDo; ++ijt) {
    jetTriggerBits_.push_back (isRandomTrigger_ ? 1 : 0);
    jetTriggerPt_  .push_back (-666); // magic number ( :-( ) used in matchTriggerObjsToJets
    jetTriggerDs_  .push_back (-666); // ditto
  }

  for (unsigned int itrg=0;itrg<ntrg;itrg++) {
    string       trigger = triggerNames_[itrg];
    unsigned int itrghlt = hltConfig_.triggerIndex(trigger);
    if( itrghlt>=ntrgtot) continue;

    if( triggerResults->accept(itrghlt)) {

      triggerFired_[itrg]++;
      trg_ |= (unsigned int)std::pow(2.0,(double)itrg);
      
      unsigned int   nmod    = triggerResults->index(itrghlt);
      vector<string> modules = hltConfig_.moduleLabels(itrghlt);
      for (unsigned int imod=0;imod<=nmod;imod++) {
	string        module  = modules[imod];
	edm::InputTag itfilter(module,"",hltProcessName_); 
	unsigned int  nfilter = triggerEvent->sizeFilters();
	unsigned int  ifilter = triggerEvent->filterIndex(itfilter);
	if( ifilter<nfilter) {
	  const trigger::Keys& keys = triggerEvent->filterKeys(ifilter); 
	  matchTriggerObjsToJets(itrg,keys,triggerEvent,jetsWithScale);
	}
      }
    } // if trigger accepted
  } // loop on trigger names
  LogTrace("Flow")<<"DBG: after trigger matching"<<endl;

  // figure out track jet matching
  // -----------------------------
  unsigned int nJetsToMatch = TMath::Min (mjt_, nJets_);
  for (unsigned int ijet=0; ijet < nJetsToMatch; ++ijet) {

    iMatchingTrackJet_[ijet] = iMinDRTrackJet_[ijet] = -1;
    nMatchingTrackJets_[ijet] = 0;
    float minDR = 666;

    if( useTrackJets ) {
      for (unsigned int itj=0; itj < trackJets->size(); ++itj) {
	const reco::BasicJet &tj = trackJets->at(itj);

	double deltaR = reco::deltaR (tj, jetsWithScale.at(ijet));
	if( deltaR < minDR) {minDR = deltaR; iMinDRTrackJet_[ijet] = itj;}
	
	if( deltaR > maxTrackJetDeltaR_) continue;

	++nMatchingTrackJets_[ijet];

	if( iMatchingTrackJet_[ijet] < 0 || tj.pt() > trackJets->at(iMatchingTrackJet_[ijet]).pt()) {
	  iMatchingTrackJet_[ijet] = itj;
	}
      } // track jet loop
    } // if using tracking
  } // jet loop
 
  LogTrace("DebugTrigger")<<"DBG: after track jet matching"<<endl;
 

  // Prepare parts of tree that are independent of tag and probe
  // -----------------------------------------------------------

  run_   = event.id().run();
  event_ = event.id().event();
  if( useTracking_) {
    npv_   = TMath::Min (mpv_, primVertices->size());
    for (unsigned int ipv=0;ipv<npv_;ipv++) {
      const reco::Vertex& primVertex = primVertices->at(ipv);
      pvz_[ipv]        = primVertex.z();
      pvntrk_[ipv]     = primVertex.tracksSize();
      pvsumptsq_[ipv]  = 0.0;
      pvsumlogpt_[ipv] = 0.0;
      reco::Vertex::trackRef_iterator ittrk = primVertex.tracks_begin();
      for (;ittrk!=primVertex.tracks_end();++ittrk) {
	double trkpt = (*ittrk)->pt();
	if( trkpt > 0) { // protect against NaN etc.
	  pvsumptsq_[ipv] += trkpt*trkpt;
	  pvsumlogpt_[ipv] += std::log(trkpt);
	}
      }
    }
  } else {
    npv_ = 0;
  }

  // tracking debug info.
  ntrk_ = 0;
  if( useTracking_) ntrk_ = TMath::Min (tracks->size(), mtrk_);
  for (int itrk=0; itrk < ntrk_; ++itrk) {
    const reco::Track &trk = tracks->at(itrk);
    trkpt_    [itrk] = trk.pt();
    trkpterr_ [itrk] = trk.ptError();
    trketa_   [itrk] = trk.eta();
    trkphi_   [itrk] = trk.phi();
    trklyr_   [itrk] = trk.hitPattern().trackerLayersWithMeasurement();
    trkfound_ [itrk] = trk.numberOfValidHits();
    trklost_  [itrk] = trk.numberOfLostHits();
    trkchisq_ [itrk] = trk.chi2();
    trkndf_   [itrk] = int (0.5 + trk.ndof());
    trkdxy_   [itrk] = trk.dxy();
    trkdxyerr_[itrk] = trk.dxyError();
    trkdz_    [itrk] = trk.dz();
    trkdzerr_ [itrk] = trk.dzError();
    trkqual_  [itrk] = trk.qualityMask();
    /* Both these tests worked and gave the same results (on 10 events)
    for (int itr2 = 0; itr2 < ntr01; ++itr2) {
      const reco::Track& trk2 = (itr2 < ntr0) ? (*(trackRefs0[itr2])) : (*(trackRefs1[itr2-ntr0]));
      if( &trk == &trk2) cout<<"AHDBG: address match itrk: "<<itrk<<" == ref["<<itr2<<"][0]"<<endl;
      if( prettyClose (trk.pt(), trk2.pt()) && prettyClose (trk.eta(), trk2.eta()) 
      && prettyClose (trk.phi(), trk2.phi())) 
      cout<<"AHDBG: direction match itrk: "<<itrk<<" == ref["<<itr2<<"][0]"<<endl;
      }  */
  }

  ntj_ = 0;
  if( useTrackJets ) ntj_ = TMath::Min (trackJets->size(), mtj_);
  for (unsigned int itj=0; itj < ntj_; ++itj) {
    const reco::BasicJet &tj = trackJets->at(itj);
    tjpt_   [itj] = tj.pt();
    tjeta_  [itj] = tj.eta();
    tjphi_  [itj] = tj.phi();
    tjntrk_ [itj] = tj.nConstituents();
    tjn90_  [itj] = tj.nCarrying(0.9);
  }

  LogTrace("Flow")<<"DBG: after debug track jet dump"<<endl;
   
  if( nJets_ > two_) {
    vjet3pt_ = jetsWithScale.at(two_).corrPt();
    vjet3rawpt_ = jetsWithScale.at(two_).rawPt();
    vjet3eta_ = jetsWithScale.at(two_).eta();
    vjet3jes_ = jetsWithScale.at(two_).scale();
  } else {
    vjet3pt_ = vjet3rawpt_ = 0;
    vjet3eta_ = vjet3jes_ = -666.;
  }

  // is there a better way to concatenate RefVectors? At least this way the dereferencing mess was only done once
  vector<const reco::Track*> vpTrack01;
  if( useTracking_) {
    const reco::TrackRefVector& trackRefs0 = reco::JetTracksAssociation::getValue(caloJetsToTrks, 
										  jetsWithScale.at(0).orgJet());
    LogTrace("Flow")<<"DBG: after first association"<<endl; 
    std::transform (trackRefs0.begin(), trackRefs0.end(), 
		    std::inserter (vpTrack01, vpTrack01.end()), CaloJetIDAnalyzer::getTrackPointer);
    if( nJets_ > 1) {
      const reco::TrackRefVector& trackRefs1 = reco::JetTracksAssociation::getValue(caloJetsToTrks, 
										    jetsWithScale.at(1).orgJet());
      std::transform (trackRefs1.begin(), trackRefs1.end(), 
		      std::inserter (vpTrack01, vpTrack01.end()), CaloJetIDAnalyzer::getTrackPointer);
    }
  }
  LogTrace("Flow")<<"DBG: after vpTrack01"<<endl;
  
  // Tag and probe selection
  // ----------------------
  vector<int> nStepsPassed (two_); // initialized to default value: 0
  vector<unsigned int> iDijetProbes; // index into jetsWithScale. Always two leading jets --> iTag is always 1-iProbe
  vector<unsigned int> iTrackJetProbes; // ditto
  vector<unsigned int> iLeadingTrackJets; // index into the track jet collection. One for each DIJET probe.
  float delta1 = 666;
  if( nJets_ >= two_) {
    for (unsigned int itag=0; itag < two_; itag++) { // they are sorted by pT: examine only two leading jets!
      const jetWithScale& tag = jetsWithScale.at(itag).orgJet();
    
      // no cuts on tag and probe pT!
      // cut on probe will bias results, cut on tag will be implied by trigger match and should be applied later
    
      // require trigger match
      if( jetTriggerBits_ [itag] <= 1) continue;
      ++ nStepsPassed[itag];
    
      // require tag-side track
      if( useTracking_) {
	const reco::TrackRefVector& trackRefs = reco::JetTracksAssociation::getValue(caloJetsToTrks, tag.orgJet());
	bool foundTrack = false;
	for (int itr = 0; itr < int(trackRefs.size()); ++itr) {
	  const reco::Track& trk = *(trackRefs[itr]);
	  if( (trk.qualityMask() & 0x4) && 
	      trk.pt() > minMatchTrackPt_) {
	    foundTrack = true;
	    break;
	  }
	}
	if( !foundTrack) continue;
      }
      ++ nStepsPassed[itag];


      // calorimetry veto (done here only to get meaningful nStepsPassed and allow booking events with no pairs)
      if( vjet3pt_ > max3rdJetPt_) continue;
      ++ nStepsPassed [itag];

      // look for possible probe jet & calculate delta1 (to get omaxjetpt, ignoring leading 2 jets requirement for now)
      vector<int> iProbes;
      omaxjetpt_[itag] = 0; 
      for (unsigned int iprobe=0; iprobe < nJets_; iprobe++) { // over ALL jets...
	if( iprobe == itag) continue;                          // ...except tag
      
	const jetWithScale& probe = jetsWithScale.at(iprobe);
	
	// is it opposite?
	double delta = M_PI - TMath::Abs (reco::deltaPhi (tag.phi(), probe.phi()));
	if( delta > maxDelta_) continue;
	
	// probe is good enough
	if( iprobe >= two_) omaxjetpt_[itag] = TMath::Max ((double) omaxjetpt_[itag], probe.corrPt());
	if( delta1 > M_PI) delta1 = delta; // only set it once
	iProbes.push_back (iprobe);
      }

      // book the other leading jet if it's a possible probe jet
      int iprobe = 1 - itag;
      if( iProbes.size() < 1 || iProbes[0] != iprobe) continue; // no possible prob jets / wrong leading one
      ++nStepsPassed[itag];

      // do not require opposite track-jet, calorimeter vetoes, or anything else
      iDijetProbes.push_back (iprobe);

      if( useTrackJets ) {
	// does it also have a track jet verifying the probe jet?
	// - find leading track jet in probe region
	int iLeadingTrackJet = -1;
	for (unsigned int itj=0; itj < trackJets->size(); ++itj) {
	  const reco::BasicJet &tj = trackJets->at(itj);
	  
	  // is it opposite?
	  double delta = M_PI - TMath::Abs (reco::deltaPhi (tag.phi(), tj.phi()));
	  if( delta > maxDelta_) continue;
	  
	  // is it the hardest so far?
	  float curPt = tj.pt();
	  if( iLeadingTrackJet < 0 || curPt > trackJets->at(iLeadingTrackJet).pt()) {
	    iLeadingTrackJet = itj;
	  }
	}
	iLeadingTrackJets.push_back (iLeadingTrackJet);
	// when maxTrackJetDeltaR_ < delta, it's possible for the matching track jet to be outside the probe area
	// but we don't want these rare pathological jets in the samples used to calibrate MC efficiencies....
	if( nMatchingTrackJets_[iprobe] == 0 || iLeadingTrackJet != iMatchingTrackJet_[iprobe]) continue;
	/* overkill:
	// is it close enough?
	double deltaR = 666;
	if( iLeadingTrackJet >= 0) deltaR = reco::deltaR (trackJets.at(iLeadingTrackJet), probe);
	if( deltaR > maxTrackJetDeltaR_) continue;
	*/
	++nStepsPassed[itag];
	iTrackJetProbes.push_back (iprobe);
      }
    } // loop on tags
  } // nJets >= 2
  LogTrace("Flow")<<"DBG: after dijet selection"<<endl;

  // 3rd jet selection    (always relative to the other 2 of the three leading jets, which are the "tags")
  // -----------------
  vector<unsigned int> iThirdJetProbes; // always two leading jets --> iTag is always 1-iProbe
  if( nJets_ > 2) {
    for (unsigned int iprobe=2; iprobe==2; ++iprobe) { // using a for statement to enable "continue" statements below
      const jetWithScale& tag1 = jetsWithScale.at(0);
      const jetWithScale& tag2 = jetsWithScale.at(1);
      const jetWithScale& probe = jetsWithScale.at(iprobe);
      
      // are the two leading jets back to back?
      double delta = M_PI - TMath::Abs (reco::deltaPhi (tag1.phi(), tag2.phi()));
      if( delta > maxLeadDelta_) continue;
      
      // and central enough?
      if( TMath::Abs (tag1.eta()) > maxLeadEta_ || TMath::Abs (tag2.eta()) > maxLeadEta_) continue;
      
      // and track confirmed
      
      // is 3rd jet in odd angle?
      double dPhi1 = TMath::Abs (reco::deltaPhi (tag1.phi(), probe.phi()));
      double dPhi2 = TMath::Abs (reco::deltaPhi (tag2.phi(), probe.phi()));
      if( TMath::Min (dPhi1, dPhi2) < minLeadDPhi_) continue;
      
      // and is not track confirmed
      
      // veto additional activity: MET, jets and track jets
      // NYI: METT cut
      if( nJets_ > 3 && jetsWithScale.at(3).pt() > max4thPt_) continue;
      // NYI: track jet cuts
      
      // then it qualifies
      iThirdJetProbes.push_back (iprobe);
      if( dPhi1 + dPhi2 > M_PI) delta *= -1; // encode whether tag jets lean towards (+) or away (-) from probe jet
      delta1 = delta; // delta1 gets booked...
    }
  }
  LogTrace("Flow")<<"DBG: after dijet selection"<<endl;

  // too many jets selection   (all other jets are the tag)
  // -----------------------
  vector<unsigned int> iTooManyProbes; // 
  vector<unsigned int> iTooManyTags;   // the track matched jets
  if( nJets_ >= tooManyIs_) {
    // are too many jets track-matched with enough pT?
    unsigned int nSelJets = 0;
    for (unsigned int ijet=0; ijet < nJets_; ++ijet) {
      const jetWithScale& jet = jetsWithScale.at(ijet);
      if( jet.corrPt() < minJetPt_) continue;
      ++nSelJets;

      bool trackMatched = false;
      if( useTracking_) {
	const reco::TrackRefVector& trackRefs = reco::JetTracksAssociation::getValue(caloJetsToTrks, jet.orgJet());
	trackMatched = trackRefs.size() > 0; // try different stuff here?
      }

      if( trackMatched) {
	iTooManyTags.push_back (ijet);
      } else {
	iTooManyProbes.push_back (ijet);
      }
    }
    if( nSelJets < tooManyIs_) { // not too many afterall
      iTooManyProbes.clear();
      iTooManyTags.clear();
    } else {
      pGoodBadJets_->Fill (iTooManyTags.size(), iTooManyProbes.size());
    }
  }
  LogTrace("Flow")<<"DBG: after multijet selection"<<endl;

  JetIDSelectionFunctor jetIDLoose( JetIDSelectionFunctor::CRAFT08,
                                    JetIDSelectionFunctor::LOOSE );

  JetIDSelectionFunctor jetIDTight( JetIDSelectionFunctor::CRAFT08,
                                    JetIDSelectionFunctor::TIGHT );
  // book jets
  // ---------
  njt_ = 0; // i.e. how many aready booked
  for (unsigned int ijet=0; ijet < nJets_; ++ijet) { // loop over all jets
    vector<unsigned int>::const_iterator curDiJetProbe    = std::find (iDijetProbes.begin(),    iDijetProbes.end(),    ijet);
    vector<unsigned int>::const_iterator curTrackJetProbe = std::find (iTrackJetProbes.begin(), iTrackJetProbes.end(), ijet);
    vector<unsigned int>::const_iterator curTooManyProbe  = std::find (iTooManyProbes.begin(),  iTooManyProbes.end(),  ijet);
    vector<unsigned int>::const_iterator curThirdJetProbe = std::find (iThirdJetProbes.begin(), iThirdJetProbes.end(), ijet);
    
    bydijet_   [njt_] = bookDiJets_     && (curDiJetProbe    != iDijetProbes.end()   );
    bytrackjet_[njt_] = bookByTrackJet_ && (curTrackJetProbe != iTrackJetProbes.end());
    bytoomany_ [njt_] = bookTooMany_    && (curTooManyProbe  != iTooManyProbes.end() );
    by3rd_     [njt_] = book3rdJet_     && (curThirdJetProbe != iThirdJetProbes.end());
    byall_     [njt_] = bookAll_;
    
    if( ! (byall_[njt_] || bydijet_[njt_]|| bytrackjet_[njt_] || bytoomany_[njt_]|| by3rd_[njt_] )  ) 
      continue; // no reason to book it
	
    if( bydijet_   [njt_]) ++nSamples_["dijet"];
    if( bytrackjet_[njt_]) ++nSamples_["trackjet"];
    if( bytoomany_ [njt_]) ++nSamples_["toomany"];
    if( by3rd_     [njt_]) ++nSamples_["3rdjet"];
    if( byall_     [njt_]) ++nSamples_["all"];

    bool byADijet = bydijet_[njt_]|| bytrackjet_[njt_];
	
    vector<int> iJetsToIgnore;
	
    // fill tag fields in tree
    if( byADijet) {
      int itag = 1 - ijet;
      if( ijet >= two_ || itag >= int(two_)) throw edm::Exception(edm::errors::LogicError,
							     "Only leading jets allowed in dijet tag&probe");
      const jetWithScale& tag = jetsWithScale.at(itag);
      iJetsToIgnore.push_back (itag);
      iJetsToIgnore.push_back (ijet);
      tagdelta_ [njt_] = delta1;
      tagtrg_  [njt_] = jetTriggerBits_[itag];
      tagtrgpt_[njt_] = jetTriggerPt_  [itag];
      tagtrgds_[njt_] = jetTriggerDs_  [itag];
      tage_    [njt_] = tag.energy(); // uncorrected
      tagpt_   [njt_] = tag.corrPt();
      tagrawpt_[njt_] = tag.rawPt();
      tageta_  [njt_] = tag.eta();
      tagphi_  [njt_] = tag.phi();
      tagemf_  [njt_] = tag.emEnergyFraction();
      tagntrk_ [njt_] = useTracking_ ? reco::JetTracksAssociation::tracksNumber (caloJetsToTrks, tag.orgJet()) : -666;
      tagchf_  [njt_] = (useTracking_ && tag.corrPt() > 0)                     // charged fraction
	? reco::JetTracksAssociation::tracksP4 (caloJetsToTrks, tag.orgJet()).pt() / tag.corrPt() 
	: -666;
    } else {
      if( by3rd_[njt_]) {
	// On second thought, it's interesting to see how the 3rd jet balanced the two main ones!
	//iJetsToIgnore.push_back(0); 
	//iJetsToIgnore.push_back(1);
      } else if( bytoomany_[njt_]) {
	std::copy (iTooManyProbes.begin(), iTooManyProbes.end(), std::back_inserter(iJetsToIgnore));
      }
	
      // NYI: put something in tag fields for 3rd and toomany samples?
      if( by3rd_[njt_]) {
	tagdelta_ [njt_] = delta1;
      }
    }

    // find prefered axis, e.g., of leading two jets (note: axisPhi is meaningful only mod Pi)
    // Note: this definition is good enough even for tags with trackjet (we still require 2 leading jets)
    double axisPhi = 0;
    if( nJets_ == 1) {
      axisPhi = jetsWithScale.at(0).phi();
    } else if( byADijet|| by3rd_[njt_]) {
      axisPhi = M_PI / 2 + (jetsWithScale.at(0).phi() + jetsWithScale.at(1).phi()) / 2;
    } else {
      // note, can't use tags in too many samples, since it's possible non exists (all are probes).
      // So both in that sample, and in the "all" sample, use the probe direction :-(
      axisPhi = jetsWithScale.at(ijet).phi();
    }
  

    // veto region is only meaningful for the di-jet variants
    vjetptlong_ = vjetptperp_ = 0;
    for (unsigned int ijt=two_; ijt < nJets_; ijt++) { // they are sorted by pT: examine jets outside the leading pair
      const jetWithScale& jet = jetsWithScale.at(ijt);
      vjetptlong_ += TMath::Abs (jet.corrPt()*TMath::Cos(jet.phi() - axisPhi));
      vjetptperp_ += TMath::Abs (jet.corrPt()*TMath::Sin(jet.phi() - axisPhi));
    }


    vtrkptlong_ = vtrkptperp_ = vmaxtrkpt_ = 0;
    if( useTracking_) {
      for (unsigned int itrk=0; itrk < tracks->size(); ++itrk) {
	const reco::Track &trk = tracks->at(itrk);
	if( trk.pt() < minTrackPt_ || 0 == (trk.qualityMask() & 0x4)) continue;
	
	bool matchedToTwoLeadingJets = false;
	for (unsigned int i01=0; i01 < vpTrack01.size(); ++i01) {
	  if( vpTrack01[i01] == &trk) {matchedToTwoLeadingJets = true; break;}
	}
	if( matchedToTwoLeadingJets) continue;
	
	vtrkptlong_ += TMath::Abs (trk.pt()*TMath::Cos(trk.phi() - axisPhi));
	vtrkptperp_ += TMath::Abs (trk.pt()*TMath::Sin(trk.phi() - axisPhi));
	vmaxtrkpt_ = TMath::Max (double (vmaxtrkpt_), trk.pt());
      }
    }
    
    // fill probe fields in tree
    const jetWithScale& probe = jetsWithScale.at(ijet);
    jtrank_[njt_]   = ijet;
    jte_[njt_]      = probe.energy(); // uncorrected!
    jtpt_[njt_]     = probe.corrPt();
    jtrawpt_[njt_]  = probe.rawPt();
    // physicsEta was removed from RecoJet in CMSSW_3 (can still be done via static method in Jet)
    // since it proved to be a nuisance anyway, stopped using it
    jteta_[njt_]    = probe.eta(); // Was: (npv_==0) ? probe.eta() : probe.physicsEta(pvz_[0]);
    jtdeta_[njt_]   = probe.eta();
    jtphi_[njt_]    = probe.phi();
    jtemf_[njt_]    = probe.emEnergyFraction();
    jtn90_[njt_]    = probe.n90();
    jtntwrs_[njt_]  = probe.nConstituents();
    jtetaeta_[njt_] = probe.etaetaMoment();
    jtphiphi_[njt_] = probe.phiphiMoment();
    jtetaphi_[njt_] = probe.etaphiMoment();
    jtntrk_[njt_]   = useTracking_ ?
      reco::JetTracksAssociation::tracksNumber(caloJetsToTrks,probe.orgJet()) : 0;
    jtchf_[njt_]    =  (useTracking_ && probe.corrPt() > 0) ?                                 // charged fraction
      (reco::JetTracksAssociation::tracksP4(caloJetsToTrks,probe.orgJet()).pt() / probe.corrPt()) : -666;
    jthef_[njt_]    = (probe.energy () > 0) ? probe.hadEnergyInHE() / probe.energy () : -666; // HE fraction
    double pTallTracks = 0, pTposTracks = 0;
    jtposf_[njt_]   =  -666;
    if( useTracking_) {
      const reco::TrackRefVector& trackRefs = reco::JetTracksAssociation::getValue(caloJetsToTrks, probe.orgJet());
      for (int itr = 0; itr < int(trackRefs.size()); ++itr) {
	const reco::Track& trk = *(trackRefs[itr]);
	pTallTracks += trk.pt();
	if( trk.charge() > 0) pTposTracks += trk.pt();
      }
      if( pTallTracks > 0) jtposf_[njt_] = pTposTracks / pTallTracks; // track pT fraction in positive tracks
    }
	    
    jtnsd_[njt_] = 0;
    // sensible values for when these fields don't get filled later on (which is legit.)
    jtf1hpd_[njt_] = jtf2hpd_[njt_] = jtf1rbx_[njt_] = 0;
    jtf1h_[njt_] = jtf2h_[njt_] = jtf1e_[njt_] = jtf2e_[njt_] = jtminhade_[njt_] = 0;
    // senseless ("unphysical") values for these fields, for when they don't get filled later on (which is odd)
    jtptrel_[njt_] = -666.;
    jthn50_ [njt_] = jthn90_ [njt_] = 0;
    jtnbt_[njt_] = jtnbst_[njt_] = jtnbh_ [njt_] = jtnbe_ [njt_] = 0;
    jtnrt_[njt_] = jtnrst_[njt_] = jtnrh_ [njt_] = jtnre_ [njt_] = 0;
    jtnpt_[njt_] = jtnpst_[njt_] = jtnph_ [njt_] = jtnpe_ [njt_] = 0;
    jtnft_[njt_] = jtnfst_[njt_] = jtnfh_ [njt_] = jtnfe_ [njt_] = 0;
    jtfbt_[njt_] = jtfbe_ [njt_] = jtfbh_ [njt_] = 0;
    jtfrt_[njt_] = jtfre_ [njt_] = jtfrh_ [njt_] = 0;
    jtfpt_[njt_] = jtfpe_ [njt_] = jtfph_ [njt_] = 0;
    jtfft_[njt_] = jtffe_ [njt_] = jtffh_ [njt_] = 0;
    
    if( probe.energy() > 0) {

      {
	// -----------------------
	//  tower based variables    (for AOD)
	// -----------------------
	vector<reco::helper::JetIDHelper::subtower> subtowers, Ecal_subtowers, Hcal_subtowers, HO_subtowers;
	vector<double> HPD_energies, RBX_energies;
	unsigned int n_bad_towers, n_bad_subtowers, n_bad_ecal_cells, n_bad_hcal_cells;
	unsigned int n_recovered_towers, n_recovered_subtowers, n_recovered_ecal_cells, n_recovered_hcal_cells;
	unsigned int n_prob_towers, n_prob_subtowers, n_prob_ecal_cells, n_prob_hcal_cells;
	double E_bad_tower, E_bad_ecal, E_bad_hcal;
	double E_recovered_tower, E_recovered_ecal, E_recovered_hcal;
	double E_prob_tower, E_prob_ecal, E_prob_hcal;
	
	classifyJetTowers( event, probe, 
			   subtowers, Ecal_subtowers, Hcal_subtowers, HO_subtowers, 
			   HPD_energies, RBX_energies, 
			   n_bad_towers, n_bad_subtowers, n_bad_ecal_cells, n_bad_hcal_cells,
			   n_recovered_towers, n_recovered_subtowers, n_recovered_ecal_cells, n_recovered_hcal_cells,
			   n_prob_towers, n_prob_subtowers, n_prob_ecal_cells, n_prob_hcal_cells,
			   E_bad_tower,  E_bad_ecal,  E_bad_hcal,
			   E_recovered_tower,  E_recovered_ecal,  E_recovered_hcal,
			   E_prob_tower,  E_prob_ecal,  E_prob_hcal,
			   1 );
	cout<<"DBG - n_bad_towers: "<<n_bad_towers<<", st: "<<n_bad_subtowers<<endl; 
	jtnbt_ [njt_] = n_bad_towers;
	jtnbst_[njt_] = n_bad_subtowers;
	jtnbh_ [njt_] = n_bad_hcal_cells;
	jtnbe_ [njt_] = n_bad_ecal_cells;
	jtnrt_ [njt_] = n_recovered_towers;
	jtnrst_[njt_] = n_recovered_subtowers;
	jtnrh_ [njt_] = n_recovered_hcal_cells;
	jtnre_ [njt_] = n_recovered_ecal_cells;
	jtnpt_ [njt_] = n_prob_towers;
	jtnpst_[njt_] = n_prob_subtowers;
	jtnph_ [njt_] = n_prob_hcal_cells;
	jtnpe_ [njt_] = n_prob_ecal_cells;
	jtnft_ [njt_] = n_bad_towers + n_recovered_towers + n_prob_towers;
	jtnfst_[njt_] = n_bad_subtowers + n_recovered_subtowers + n_prob_subtowers;
	jtnfh_ [njt_] = n_bad_hcal_cells + n_recovered_hcal_cells + n_prob_hcal_cells;
	jtnfe_ [njt_] = n_bad_ecal_cells + n_recovered_ecal_cells + n_prob_ecal_cells;
	jtfbt_ [njt_] = E_bad_tower / probe.energy();
	jtfbe_ [njt_] = E_bad_ecal / probe.energy();
	jtfbh_ [njt_] = E_bad_hcal / probe.energy();
	jtfrt_ [njt_] = E_recovered_tower / probe.energy();
	jtfre_ [njt_] = E_recovered_ecal / probe.energy();
	jtfrh_ [njt_] = E_recovered_hcal / probe.energy();
	jtfpt_ [njt_] = E_prob_tower / probe.energy();
	jtfpe_ [njt_] = E_prob_ecal / probe.energy();
	jtfph_ [njt_] = E_prob_hcal / probe.energy();
	jtfft_ [njt_] = ( E_bad_tower + E_recovered_tower + E_prob_tower ) / probe.energy();
	jtffe_ [njt_] = ( E_bad_ecal + E_recovered_ecal + E_prob_ecal ) / probe.energy();
	jtffh_ [njt_] = ( E_bad_hcal + E_recovered_hcal + E_prob_hcal ) / probe.energy();
      }
      
      // ------------------------
      //  RecHit based variables
      // ------------------------

      vector< double > HPD_energies, RBX_energies, HBEF_energies, HO_energies, ECal_energies;
      vector< double > HB_problem_energies, HE_problem_energies, HF_problem_energies, HO_problem_energies;
      vector< double > EB_problem_energies, EE_problem_energies;
      vector< int > HB_problem_counts, HE_problem_counts, HF_problem_counts, HO_problem_counts;
      vector< int > EB_problem_counts, EE_problem_counts;
      double pTrel = -666;
      unsigned int hn50 = 0, hn90 = 0, nSD = 0, nHadHits = 0, nEMHits = 0, nHadTowers = 0, nEMTowers = 0;
      unsigned int dumpReason = 0;
      getCalComponents( event, probe, HPD_energies, RBX_energies, HBEF_energies, HO_energies, ECal_energies,
			HB_problem_energies, HB_problem_counts, 
			HE_problem_energies, HE_problem_counts,
			HF_problem_energies, HF_problem_counts,
			HO_problem_energies, HO_problem_counts,
			EB_problem_energies, EB_problem_counts,
			EE_problem_energies, EE_problem_counts,
			pTrel, hn50, hn90, nHadHits, nEMHits, nHadTowers, nEMTowers, 
			nSD, 0); // normally, off:  );
      if( HPD_energies.size() > 0 ) jtf1hpd_[njt_] = HPD_energies[0] / probe.energy();
      if( HPD_energies.size() > 1 ) jtf2hpd_[njt_] = HPD_energies[1] / probe.energy();
      if( RBX_energies.size() > 0 ) jtf1rbx_[njt_] = RBX_energies[0] / probe.energy();
      if( RBX_energies.size() > 0 && RBX_energies[0] > ( 1-probe.emEnergyFraction() ) * probe.energy() + 0.001 ) {
	dumpReason |= 1;
      }
      if( HBEF_energies.size() > 0 ) jtf1h_[njt_] = HBEF_energies[0] / probe.energy();
      if( HBEF_energies.size() > 1 ) jtf2h_[njt_] = HBEF_energies[1] / probe.energy();
      if( HBEF_energies.size() > 0 ) jtminhade_[njt_] = *(HBEF_energies.rbegin());
      if( ECal_energies.size() > 0 ) jtf1e_[njt_] = ECal_energies[0] / probe.energy();
      if( ECal_energies.size() > 1 ) jtf2e_[njt_] = ECal_energies[1] / probe.energy();
      jthn50_ [njt_] = hn50;
      jthn90_ [njt_] = hn90;
      jtptrel_[njt_] = pTrel;
      jtnhits_ [njt_] = nHadHits + nEMHits;
      jtnhh_   [njt_] = nHadHits;
      jtneh_   [njt_] = nEMHits;
      jtnht_   [njt_] = nHadTowers;
      jtnet_   [njt_] = nEMTowers;
      jtnsd_   [njt_] = nSD;
      if( probe.emEnergyFraction() < 1 && HPD_energies.size() == 0 && TMath::Abs(probe.eta())<2.6) dumpReason |= 2;
      if( hn50 == 0 || hn90 == 0) dumpReason |= 4;
      if( probe.emEnergyFraction() + jtf1h_[njt_] + jtf2h_[njt_] > 1.001 && TMath::Abs(probe.eta())<2.6) 
	dumpReason |= 8;
      double total_HO_energy = 0;
      for( unsigned int ihit = 0; ihit < HO_energies.size(); ++ihit ) total_HO_energy += HO_energies[ ihit ];
      jthof_[njt_] = total_HO_energy / probe.energy ();

      // the following bits of code should be refactors into a subroutine at some point...
      double eBad = HB_problem_energies[ n_HBHE_flags ] + HE_problem_energies[ n_HBHE_flags ] 
	+ HF_problem_energies[ n_HF_flags ] + HO_problem_energies[ n_HO_flags ]
	+ EB_problem_energies[ n_ECAL_flags ] + EE_problem_energies[ n_ECAL_flags ];
      int nBad = HB_problem_counts[ n_HBHE_flags ] + HE_problem_counts[ n_HBHE_flags ] 
	+ HF_problem_counts[ n_HF_flags ] + HO_problem_counts[ n_HO_flags ]
	+ EB_problem_counts[ n_ECAL_flags ] + EE_problem_counts[ n_ECAL_flags ];
      int nSDBad = 0;
      if( HB_problem_counts[ n_HBHE_flags ] ) ++nSDBad;
      if( HE_problem_counts[ n_HBHE_flags ] ) ++nSDBad;
      if( HF_problem_counts[ n_HF_flags ] ) ++nSDBad;
      if( HO_problem_counts[ n_HO_flags ] ) ++nSDBad;
      if( EB_problem_counts[ n_ECAL_flags ] ) ++nSDBad;
      if( EE_problem_counts[ n_ECAL_flags ] ) ++nSDBad;
      jtfq_  [njt_] = eBad / probe.energy();
      jtnq_  [njt_] = nBad;
      jtnsdq_[njt_] = nSDBad;
      

      double eOOT = HB_problem_energies[ 14 ] + HE_problem_energies[ 14 ] + HF_problem_energies[ 9 ] 
	+ HO_problem_energies[ 0 ] + HO_problem_energies[ 1 ] + HO_problem_energies[ 2 ]
	+ EB_problem_energies[ 10 ] + EE_problem_energies[ 10 ];
      int nOOT = HB_problem_counts[ 14 ] + HE_problem_counts[ 14 ] + HF_problem_counts[ 9 ] 
	+ HO_problem_counts[ 0 ] + HO_problem_counts[ 1 ] + HO_problem_counts[ 2 ]
	+ EB_problem_counts[ 10 ] + EE_problem_counts[ 10 ];
      int nSDOOT = 0;
      if( HB_problem_counts[ 14 ] ) ++nSDOOT;
      if( HE_problem_counts[ 14 ] ) ++nSDOOT;
      if( HF_problem_counts[ 9 ] ) ++nSDOOT;
      if( HO_problem_counts[ 0 ] + HO_problem_counts[ 1 ] + HO_problem_counts[ 2 ] ) ++nSDOOT;
      if( EB_problem_counts[ 10 ] ) ++nSDOOT;
      if( EE_problem_counts[ 10 ] ) ++nSDOOT;
      jtfoot_  [njt_] = eOOT / probe.energy();
      jtnoot_  [njt_] = nOOT;
      jtnsdoot_[njt_] = nSDOOT;

      double eSAT = HB_problem_energies[ 3 ] + HE_problem_energies[ 3 ] 
	+ HF_problem_energies[ 3 ] + HO_problem_energies[ 3 ]
	+ EB_problem_energies[ 5 ] + EB_problem_energies[ 6 ]
	+ EE_problem_energies[ 5 ] + EE_problem_energies[ 6 ];
      int nSAT = HB_problem_counts[ 3 ] + HE_problem_counts[ 3 ] 
	+ HF_problem_counts[ 3 ] + HO_problem_counts[ 3 ]
	+ EB_problem_counts[ 5 ] + EB_problem_counts[ 6 ]
	+ EE_problem_counts[ 5 ] + EE_problem_counts[ 6 ];
      int nSDSAT = 0;
      if( HB_problem_counts[ 3 ] ) ++nSDSAT;
      if( HE_problem_counts[ 3 ] ) ++nSDSAT;
      if( HF_problem_counts[ 3 ] ) ++nSDSAT;
      if( HO_problem_counts[ 3 ] ) ++nSDSAT;
      if( EB_problem_counts[ 5 ] + EB_problem_counts[ 6 ] ) ++nSDSAT;
      if( EE_problem_counts[ 5 ] + EE_problem_counts[ 6 ] ) ++nSDSAT;
      jtfsat_  [njt_] = eSAT / probe.energy();
      jtnsat_  [njt_] = nSAT;
      jtnsdsat_  [njt_] = nSDSAT;


      double eLS = HF_problem_energies[ 4 ];
      int nLS = HF_problem_counts[ 4 ];
      jtfls_[njt_] = eLS / probe.energy();
      jtnls_[njt_] = nLS;
 
      jet_ID_helper_.calculate( event, probe ); //dynamic_cast<reco::CaloJet const &> probe );
      const reco::helper::JetIDHelper & idh = jet_ID_helper_;
      if( idh.n90Hits() != (int)hn90 ||
	  idh.nHCALTowers() != (int)nHadTowers ||
	  idh.nECALTowers() != (int)nEMTowers ||
	  (! prettyClose( idh.fHPD(), jtf1hpd_[njt_]) ) ||
	  (! prettyClose( idh.fRBX(), jtf1rbx_[njt_]) ) ) dumpReason |= 0x10;
      if( ! prettyClose( idh.restrictedEMF(), probe.emEnergyFraction() ) ) dumpReason |= 0x20;
      jtaodhpd_[njt_] = idh.approximatefHPD();
      jtaodrbx_[njt_] = idh.approximatefRBX();
      jtnhn90_[njt_] = idh.hitsInN90();

      // a rather elaborate calculation to limit printing (from OPAL code)
      int jentry = nSamples_["analyzed"];
      int JTEMP = TMath::Nint (TMath::Power (10., Double_t (TMath::Floor (TMath::Log10 ( Double_t (1+jentry) )))));
      if(  ((1+jentry)%JTEMP) == 0) {cout<<"entry #"<<jentry<<"..."<<endl; dumpReason |= 0x40;} // tmp debug
 
      if( dumpReason > 0) {
	//edm::LogWarning("PossibleBug")<<"DumpReason:"<<dumpReason;
	cout<<"Possible bug - dump reason: "<<dumpReason<<" rerunning calculation..."<<endl;
	jet_ID_helper_.calculate( event, probe, 5 ); //dynamic_cast<reco::CaloJet const &> probe );
	cout<<"`--------- done with calculation"<<endl;
	cout<<"Jet eta: "<<probe.eta()<<", E: "<<probe.energy()<<" emf: "<<probe.emEnergyFraction()
	    <<" -> E_H: "<<(1-probe.emEnergyFraction())*probe.energy()<<", pTrel: "<<pTrel<<endl;
	cout<<"old - fPHD: "<<jtf1hpd_[njt_]<<", fRBX: "<<jtf1rbx_[njt_]<<"; hn90: "<<hn90<<","
	    <<"\n       nht: "<<nHadTowers<<", net: "<<nEMTowers<<", emf: "<<probe.emEnergyFraction()
	    <<", (hn50: "<<hn50<<")"<<endl;
	cout<<"idh - fHPD: "<<idh.fHPD()<<", fRBX: "<<idh.fRBX()<<"; hn90: "<<idh.n90Hits()<<","
	    <<"\n       nht: "<<idh.nHCALTowers()<<", net: "<<idh.nECALTowers()<<", rfem: "<<idh.restrictedEMF()
	    <<"; subdets: "<<idh.fSubDetector1()<<", "<<idh.fSubDetector2()<<", "<<idh.fSubDetector3()
	    <<", "<<idh.fSubDetector4()<<endl;
	cout<<"ECal_E:";
	for (unsigned int i=0; i<ECal_energies.size(); ++i) cout<<" "<<ECal_energies[i];
	cout<<";\nHCal_E:";
	for (unsigned int i=0; i<HBEF_energies.size(); ++i) cout<<" "<<HBEF_energies[i];
	cout<<";\nHPD_E:";
	for (unsigned int i=0; i<HPD_energies.size(); ++i) cout<<" "<<HPD_energies[i];
	cout<<";\nRBX_E:";
	for (unsigned int i=0; i<RBX_energies.size(); ++i) cout<<" "<<RBX_energies[i];
	cout<<";"<<endl;
	getCalComponents (event, probe, HPD_energies, RBX_energies, HBEF_energies, HO_energies, ECal_energies,
			  HB_problem_energies, HB_problem_counts,
			  HE_problem_energies, HE_problem_counts,
			  HF_problem_energies, HF_problem_counts,
			  HO_problem_energies, HO_problem_counts,
			  EB_problem_energies, EB_problem_counts,
			  EE_problem_energies, EE_problem_counts,
			  pTrel, hn50, hn90, nHadHits, nEMHits, nHadTowers, nEMTowers, nSD
			  , 7); // run it again with debug prints
      }
    }
    
    jttrg_  [njt_] = jetTriggerBits_[ijet];
    jttrgpt_[njt_] = jetTriggerPt_  [ijet];
    jttrgds_[njt_] = jetTriggerDs_  [ijet];

    jtntrkjet_    [njt_] = nMatchingTrackJets_[ijet];
    jttrkjetntrk_ [njt_] = (iMatchingTrackJet_[ijet] >= 0)
      ? trackJets->at(iMatchingTrackJet_[ijet]).nConstituents()
      : -666;
    jttrkjetpt_   [njt_] = (iMatchingTrackJet_[ijet] >= 0) 
      ? trackJets->at(iMatchingTrackJet_[ijet]).pt()
      : -666;
    jttrkjeteta_  [njt_] = (iMatchingTrackJet_[ijet] >= 0) 
      ? trackJets->at(iMatchingTrackJet_[ijet]).eta()
      : -666;
    jttrkjetphi_  [njt_] = (iMatchingTrackJet_[ijet] >= 0) 
      ? trackJets->at(iMatchingTrackJet_[ijet]).phi()
      : -666; 
    jttrkjetDR_   [njt_] = (iMatchingTrackJet_[ijet] >= 0) 
      ? reco::deltaR (probe, trackJets->at(iMatchingTrackJet_[ijet]))
      : -666; 
    jtmintrkjetDR_[njt_] = (iMinDRTrackJet_[ijet] >= 0)
      ? reco::deltaR (probe, trackJets->at(iMinDRTrackJet_[ijet]))
      : -666; 

    if( byADijet) {
      // fill probe region fields in tree (omaxjetpt_ already filled above)
      omaxtrkpt_[njt_] = otrkptsum_[njt_] = 0;
      ontrk_[njt_] = 0;
      int itag = 1 - ijet;
      const jetWithScale& tag = jetsWithScale.at(itag); // no need for scale - only angles used
      if( useTracking_) {
	for (unsigned int itrk=0; itrk < tracks->size(); ++itrk) {
	  const reco::Track &trk = tracks->at(itrk);
	  if( trk.pt() < minTrackPt_ || 0 == (trk.qualityMask() & 0x4)) continue;
	  
	  // is it opposite?
	  double delta = M_PI - TMath::Abs (reco::deltaPhi (trk.phi(), tag.phi()));
	  if( delta < maxDelta_) continue;
	
	  // is it in the probe jet?
	  bool matchedToProbe = false;
	  const reco::TrackRefVector& trackRefs = reco::JetTracksAssociation::getValue(caloJetsToTrks, probe.orgJet());
	  for (unsigned int itrj = 0; itrj < trackRefs.size(); ++itrj) {
	    const reco::Track& probeTrack = *(trackRefs[itrj]);
	    if( &probeTrack == &trk) {matchedToProbe = true; break;}
	  }
	  if( matchedToProbe) continue;
	
	  ++ontrk_[njt_];
	  otrkptsum_[njt_] += trk.pt();
	  omaxtrkpt_[njt_] = TMath::Max (double (omaxtrkpt_[njt_]), trk.pt());
	} // loop on tracks
      } // if using tracking
    } // if di-jet

    // fill "inclusive" MET, before ignoring jets
    double corrSet = met.sumEt(), corrSetNoHF = metNoHF.sumEt();
    double corrMEX = met.pt() * TMath::Cos (met.phi());
    double corrMEY = met.pt() * TMath::Sin (met.phi());
    double corrMEXNoHF = metNoHF.pt() * TMath::Cos (metNoHF.phi());
    double corrMEYNoHF = metNoHF.pt() * TMath::Sin (metNoHF.phi());
    iset_   = corrSet;
    imet_   = TMath::Sqrt (corrMEX*corrMEX + corrMEY*corrMEY);
    imett_  = TMath::Abs (corrMEX*TMath::Cos(axisPhi) + corrMEY*TMath::Sin(axisPhi));
    inset_  = corrSetNoHF;
    inmet_  = TMath::Sqrt (corrMEXNoHF*corrMEXNoHF + corrMEYNoHF*corrMEYNoHF);
    inmett_ = TMath::Abs (corrMEXNoHF*TMath::Cos(axisPhi) + corrMEYNoHF*TMath::Sin(axisPhi));

    // "cookie cutter" MET corrections for jets that are part of the tag & should be ignored for MET
    // this ignores the JES. Better performance may be possible by somehow taking it into account.
    for (unsigned int ijt=0; ijt < iJetsToIgnore.size(); ++ijt) {
      const jetWithScale& jet = jetsWithScale.at(iJetsToIgnore[ijt]);
      double px = jet.et() * TMath::Cos (jet.phi());
      double py = jet.et() * TMath::Sin (jet.phi());
      // estimate HF contribution to MET from non-HF part of jet...
      double etNoHF = 0;
      if( jet.energy() > 0) etNoHF = (1 - (jet.hadEnergyInHF() + jet.emEnergyInHF()) / jet.energy()) * jet.et();
      double pxNoHF = etNoHF * TMath::Cos (jet.phi());
      double pyNoHF = etNoHF * TMath::Sin (jet.phi());
      corrMEX += px; // since MET = - sum cellE, to remove jet must add its energy back in
      corrMEY += py;
      corrMEXNoHF += pxNoHF;
      corrMEYNoHF += pyNoHF;
    }
    // fill "corrected" (really exclusive) MET
    set_   = corrSet;
    met_   = TMath::Sqrt (corrMEX*corrMEX + corrMEY*corrMEY);
    mett_  = TMath::Abs (corrMEX*TMath::Cos(axisPhi) + corrMEY*TMath::Sin(axisPhi));
    nset_  = corrSetNoHF;
    nmet_  = TMath::Sqrt (corrMEXNoHF*corrMEXNoHF + corrMEYNoHF*corrMEYNoHF);
    nmett_ = TMath::Abs (corrMEXNoHF*TMath::Cos(axisPhi) + corrMEYNoHF*TMath::Sin(axisPhi));

    // book this pair (later)!
    ++ njt_;
    
  } // loop on all jets

  pCutFlowHist0_ -> Fill (nStepsPassed[0]);
  if( nJets_ >= 1) pCutFlowHist1_ -> Fill (nStepsPassed[1]);

  // fill event without tag & probe pairs?
  if( njt_ > 0 || !requireProbe_) tree_->Fill();
  LogTrace("Flow")<<"DBG: finished processing event"<<endl;
}


//______________________________________________________________________________
void CaloJetIDAnalyzer::endJob()
{
  std::stringstream ss, ss2, ss3, ss4;

  for (unsigned int itrg=0;itrg<triggerNames_.size();itrg++)
    ss<<triggerNames_[itrg]<<":\t"<<triggerFired_[itrg]<<"\n";

  for (std::map<string,int>::const_iterator it=nSamples_.begin(); it != nSamples_.end(); ++it)
    ss2<<Form("%10s: %d", it->first.c_str(), it->second)<<"\n";

  for (std::set<string>::const_iterator it=triggersSeen_.begin(); it != triggersSeen_.end(); ++it)
    ss3<<" "<<*it<<",";

  for (std::set<string>::const_iterator it=triggersUsedAsMu_.begin(); it != triggersUsedAsMu_.end(); ++it)
    ss4<<" "<<*it<<",";

  edm::LogPrint("Summary")
    <<"++++++++++++++++++++++++++++++++++++++++++++++++++"
    <<"\n"<<moduleName_<<"(CaloJetIDAnalyzer) SUMMARY:"
    <<"\n\nBooked "<<tree_->GetEntries()<<" events in tree."
    <<"\n"<<ss.str()
    <<"\n"<<ss2.str()
    <<"\n All triggers seen in events:"<<ss3.str()
    <<"\n Those used as muon triggers:"<<ss4.str()
    <<"\n++++++++++++++++++++++++++++++++++++++++++++++++++";
}


//______________________________________________________________________________
void CaloJetIDAnalyzer::matchTriggerObjsToJets(unsigned int itrg,
					       const trigger::Keys& keys,
					       const TrgEvtHandle_t& triggerEvent,
					       const vector<jetWithScale>& jetsWithScale)
{
  hTrgObjCount_[itrg]->Fill(keys.size());

  const TriggerObjectCollection& triggerObjs = triggerEvent->getObjects();
  
  for (unsigned int ijt=0; ijt < jetsWithScale.size(); ++ijt) {
    
    const reco::CaloJet& caloJet   = jetsWithScale.at(ijt); // no need for scale - trigger is at raw level
    double               deltaRMin = 99.9;
    double               jttrgpt   =  0.0;

    if( caloJet.pt() <= 0) {edm::LogWarning("IllegalInput")<<"found non positive jet pT"; continue;}
    
    for (unsigned int iobj=0; iobj < keys.size(); ++iobj) {
      
      const TriggerObject& trgObj = triggerObjs[keys[iobj]];
      double deltaR = reco::deltaR(trgObj,caloJet);
      if( deltaR < deltaRMin) { deltaRMin = deltaR; jttrgpt = trgObj.pt(); }
    }
    if( deltaRMin < 1.0) hTrgDeltaR_[itrg]->Fill(deltaRMin);
    if( deltaRMin < triggerDeltaR_[itrg]) {
      jetTriggerBits_[ijt]  |= 1 << (1+itrg); // LSBit is reserved, so mark it in bit 1+itrg
      if( jetTriggerPt_[ijt] == -666) { // fill it just for the first trigger name (& for the best match)
	jetTriggerPt_[ijt] = jttrgpt;
	jetTriggerDs_[ijt] = (jttrgpt - caloJet.pt()) / caloJet.pt();
      }
    }
  } // loop on jetsWithScale
}

//______________________________________________________________________________
// The HCAL generic flags are always in the 4 LSB bits, the sub-detector
// specific flags are listed afterwards.
// Internal empty bits are skipped.
// Small integers are reencoded as a flag for each value
// Otherwise, the order of the fields is the same as the official one, taken from:
// HCAL flags from https://twiki.cern.ch/twiki/bin/view/CMS/HcalRecHitFlagAssignments
// ECAL flags from https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideEcalRecoLocalReco
// circa 09/2009 = CMSSW3_2_X

// Rewrite to use constants from RecoLocalCalo/HcalRecAlgos/interface/HcalCaloFlagLabels.h ???


int32_t HO2jetIDflags( int32_t HO_flags ) 
{
  //not needed yet: bool timingSummary = (HO_flags & 0x70000) > 0;
  return HO_flags >> 16;
}

int32_t HF2jetIDflags( int32_t HF_flags ) 
{
  int HFTimingTrustBits = (HF_flags >> 6) & 0x03;
  bool HFLongShort = HF_flags & 0x01;
  bool HFDigiTime = HF_flags & 0x02;
  int generic = ( HF_flags >> 16 ) & 0xF;
  bool timingSummary = ((HF_flags & 0x70002) > 0) || (HFTimingTrustBits > 0);
  return generic | ( HFLongShort ? 0x10 : 0 ) | ( HFDigiTime ? 0x20 : 0 ) |
    ( ( HFTimingTrustBits > 0 ) ? ( 0x20 << HFTimingTrustBits ) : 0 ) |
    ( timingSummary ? 0x200 : 0);  
}

int32_t HBHE2jetIDflags( int32_t HBHE_flags ) 
{
  int basic = HBHE_flags & 0x3F;
  int HBHETimingShapedCutsBits = ( HBHE_flags >> 8 ) & 0x07;
  int generic = ( HBHE_flags >> 16 ) & 0xF;
  bool timingSummary = (HBHE_flags & 0x7073E) > 0;
  return generic | ( basic << 4 ) | ( HBHETimingShapedCutsBits << 10 ) | (timingSummary ? 0x4000 : 0);
}

int32_t ECAL2jetIDflags( int32_t ECAL_flags )
{
  bool timingSummary = (ECAL_flags & 0x6) > 0;
  return ECAL_flags | (timingSummary ? 0x400 : 0 );
}

// jet_flag_counts and jet_flag_energies are cumulative for all recHits in the jet
// nFlags is the number of individual flags tracked for this subdetector. Must be <31 !
//   an additional "some problem exists" flag will be added last
// flags and hitE are for the current recHit
void markFlags( vector< int >& jet_flag_counts, vector< double >& jet_flag_energies, 
		int nFlags, uint32_t flags, double hitE )
{
  if( flags == 0 ) return;
  ++jet_flag_counts[ nFlags ];
  jet_flag_energies[ nFlags ] += hitE;

  for( int ibit = 0; ibit < nFlags; ++ibit ) {
    if( ( flags & ( 1 << ibit ) ) > 0 ) {
      ++jet_flag_counts[ ibit ];
      jet_flag_energies[ ibit ] += hitE;
    }
  }
}

//______________________________________________________________________________
void CaloJetIDAnalyzer::getCalComponents( const edm::Event& event, const reco::CaloJet &jet, 
					  vector< double > &HPD_energies,  vector< double > &RBX_energies, 
					  vector< double > &HBEF_energies, vector< double > &HO_energies,
					  vector< double > &ECal_energies,
					  vector< double > &HB_problem_energies, vector< int > &HB_problem_counts,
					  vector< double > &HE_problem_energies, vector< int > &HE_problem_counts,
					  vector< double > &HF_problem_energies, vector< int > &HF_problem_counts,
					  vector< double > &HO_problem_energies, vector< int > &HO_problem_counts,
					  vector< double > &EB_problem_energies, vector< int > &EB_problem_counts,
					  vector< double > &EE_problem_energies, vector< int > &EE_problem_counts,
					  double& pTrel, unsigned int& n50, unsigned int& n90, 
					  unsigned int& nHadHits, unsigned int& nEMHits,
					  unsigned int& nHadTowers, unsigned int& nEMTowers, 
					  unsigned int& nSD, int iDbg )
{
  HPD_energies.clear(); RBX_energies.clear(); HBEF_energies.clear(); HO_energies.clear(); ECal_energies.clear();
  HB_problem_energies.clear(); HB_problem_counts.clear();
  HE_problem_energies.clear(); HE_problem_counts.clear();
  HF_problem_energies.clear(); HF_problem_counts.clear();
  HO_problem_energies.clear(); HO_problem_counts.clear();
  EB_problem_energies.clear(); EB_problem_counts.clear();
  EE_problem_energies.clear(); EE_problem_counts.clear();
  HB_problem_energies.resize( 1 + n_HBHE_flags ); HB_problem_counts.resize( 1 + n_HBHE_flags ); // initializes with 0s :-)
  HE_problem_energies.resize( 1 + n_HBHE_flags ); HE_problem_counts.resize( 1 + n_HBHE_flags ); // initializes with 0s :-)
  HF_problem_energies.resize( 1 + n_HF_flags ); HF_problem_counts.resize( 1 + n_HF_flags );
  HO_problem_energies.resize( 1 + n_HO_flags ); HO_problem_counts.resize( 1 + n_HO_flags );
  EB_problem_energies.resize( 1 + n_ECAL_flags ); EB_problem_counts.resize( 1 + n_ECAL_flags );
  EE_problem_energies.resize( 1 + n_ECAL_flags ); EE_problem_counts.resize( 1 + n_ECAL_flags );
  std::map<int,double> HPD_energy_map, RBX_energy_map;
  pTrel = 0;
  n50 = n90 = nSD = nHadHits = nEMHits = nHadTowers = nEMTowers = 0;

  // the jet only contains DetIds, so first read recHit collection
  edm::Handle<HBHERecHitCollection> HBHERecHits;
  event.getByLabel( "hbhereco", HBHERecHits );
  edm::Handle<HORecHitCollection> HORecHits;
  event.getByLabel( "horeco", HORecHits );
  edm::Handle<HFRecHitCollection> HFRecHits;
  event.getByLabel( "hfreco", HFRecHits );
  edm::Handle<EBRecHitCollection> EBRecHits;
  event.getByLabel( "ecalRecHit", "EcalRecHitsEB", EBRecHits );
  edm::Handle<EERecHitCollection> EERecHits;
  event.getByLabel( "ecalRecHit", "EcalRecHitsEE", EERecHits );
  double totHCalE = 0;
  if( iDbg > 2) cout<<"# of rechits found - HBHE: "<<HBHERecHits->size()
		    <<", HO: "<<HORecHits->size()<<", HF: "<<HFRecHits->size()
		    <<", EB: "<<EBRecHits->size()<<", EE: "<<EERecHits->size()<<endl;
  
  const reco::Particle::LorentzVector& jetP4 = jet.p4();
  TVector3 jetP3 (jetP4.px(), jetP4.py(), jetP4.pz());
  double jetP = jetP3.Mag();
  unsigned int SDbits = 0;
  if( iDbg > 1) cout<<"jetP: "<<jetP<<" eta: "<<jetP4.eta()<<endl;
  
  vector <CaloTowerPtr> towers = jet.getCaloConstituents ();
  int nTowers = towers.size();
  for (int iTower = 0; iTower <nTowers ; iTower++) {
    const reco::Particle::LorentzVector& towerP4 = towers[iTower]->p4();

    if( jetP > 0) {
      TVector3 towerP3 (towerP4.px(), towerP4.py(), towerP4.pz());
      pTrel += towerP3.Perp( jetP3 );
    }
    
    const vector<DetId>& cellIDs = towers[iTower]->constituents();  // cell == recHit
    int nCells = cellIDs.size();
    if( iDbg) cout<<"tower #"<<iTower<<" has "<<nCells<<" cells. "
		  <<"It's at iEta: "<<towers[iTower]->ieta()<<", iPhi: "<<towers[iTower]->iphi()<<endl;
    
    bool towerContainsHad = false, towerContainsEM = false;
    
    for (int iCell = 0; iCell < nCells; ++iCell) {
      DetId::Detector detNum = cellIDs[iCell].det();
      uint32_t raw_flags, my_flags;
      double hitE = 0;
      if( detNum == DetId::Hcal) {
	towerContainsHad = true;
	++nHadHits;
	HcalDetId HcalID = cellIDs[iCell];
	HcalSubdetector HcalNum = HcalID.subdet();
	if( HcalNum == HcalOuter ) {
	  HORecHitCollection::const_iterator theRecHit=HORecHits->find(HcalID);
	  if( theRecHit == HORecHits->end() ) {cerr<<"Debug Error. Can't find HO recHit ID:"<<HcalID<<endl; continue;}
	  hitE = theRecHit->energy();
	  raw_flags = theRecHit->flags();
	  my_flags = HO2jetIDflags( raw_flags );
	  markFlags( HO_problem_counts, HO_problem_energies, n_HO_flags, my_flags, hitE );
	  HO_energies.push_back( hitE );
	  SDbits |= 0x8;
	  if( iDbg > 11 ) cout<< "hit #"<<iCell<<" is HO, E: "<<hitE<<" iEta: "<<theRecHit->id().ieta()
			      <<", depth: "<<theRecHit->id().depth()<<", iPhi: "
			      <<theRecHit->id().iphi()<<endl;
	  continue;
	  
	} else if( HcalNum == HcalForward ) {
	  
	  HFRecHitCollection::const_iterator theRecHit=HFRecHits->find(HcalID);	    
	  if( theRecHit == HFRecHits->end() ) {cerr<<"Debug Error. Can't find HF recHit"<<endl; continue;}
	  hitE = theRecHit->energy();
	  raw_flags = theRecHit->flags();
	  my_flags = HF2jetIDflags( raw_flags );
	  markFlags( HF_problem_counts, HF_problem_energies, n_HF_flags, my_flags, hitE );
	  SDbits |= 0x4;
	  if( iDbg > 11 ) cout<< "hit #"<<iCell<<" is HF with E: "<<hitE<<" iEta: "<<theRecHit->id().ieta()
			      <<", depth: "<<theRecHit->id().depth()<<", iPhi: "
			      <<theRecHit->id().iphi()<<", flags: "<<raw_flags<<endl;
	  
	} else { // HBHE
	  
	  // not needed, get it through theRecHit HcalDetId HcalID = cellIDs[iCell]; // so we can access depth, etc.
	  // translate DetIds into the actual recHits
	  HBHERecHitCollection::const_iterator theRecHit = HBHERecHits->find(cellIDs[iCell]); //HcalID);	    
	  if( theRecHit == HBHERecHits->end()) {cerr<<"Debug Error. Can't find (presumed) HBHE recHit"<<endl; continue;}

	  if( HcalNum == HcalBarrel ) SDbits |= 0x1;
	  if( HcalNum == HcalEndcap ) SDbits |= 0x2;


	  int myIEta = calcMyIEta (theRecHit->id().ieta(), theRecHit->id().depth());
	  hitE = theRecHit->energy();
	  raw_flags = theRecHit->flags();
	  my_flags = HBHE2jetIDflags( raw_flags );
	  if( TMath::Abs( myIEta ) <= 16 ) {
	    cout<<"AHDBG HB problem: "<<my_flags<<" / "<<n_HF_flags<<endl;
	    markFlags( HB_problem_counts, HB_problem_energies, n_HBHE_flags, my_flags, hitE );
	  } else {
	    markFlags( HE_problem_counts, HE_problem_energies, n_HBHE_flags, my_flags, hitE );
	  }
	  int hitIPhi = theRecHit->id().iphi();
	  if( iDbg>1) cout<<"hit #"<<iCell<<" is HBHE with E: "<<hitE<<" iEta: "<<theRecHit->id().ieta()
			  <<", depth: "<<theRecHit->id().depth()<<" -> "<<myIEta<<", iPhi: "
			  <<theRecHit->id().iphi();
	  int absIEta = TMath::Abs (theRecHit->id().ieta());
	  if( theRecHit->id().depth() == 3 && (absIEta == 28 || absIEta == 29)) {
	    hitE /= 2; // Depth 3 at the HE forward edge is split over tower 28 & 29, and jet reco. assigns half each
	  }

	
	  int iHPD = 0;
	  int iRBX = ((hitIPhi + 1) % 72) / 4; // 71,72,1,2 are in the same RBX module
	  
	  if( myIEta >= -16 && myIEta < 0) {iHPD += 100; iRBX += 100;}
	  if( myIEta <=  16 && myIEta > 0) {iHPD += 200; iRBX += 200;}
	  if( myIEta >=  17)               {iHPD += 300; iRBX += 300;}
	  
	  if( std::abs (myIEta) >= 23) { // that is, tower 21 and up
	    if( (0x1 & hitIPhi) == 0) {
	      edm::LogError("CodeAssumptionsViolated")<<"Bug?! code assumes no even iPhi recHits at HE edges";
	      return;
	    }
	    bool oddnessIEta = std::abs (myIEta) & 0x1;
	    bool upperIPhi = ((hitIPhi%4) == 1 || (hitIPhi%4) == 2); // the upper iPhi indices in the HE wedge
	    // remap the iPhi so it fits the one in the inner HE regions, change in needed in two cases:
	    // 1) in the upper iPhis of the module, the even IEtas belong to the higher iPhi
	    // 2) in the loewr iPhis of the module, the odd  IEtas belong to the higher iPhi
	    if( upperIPhi != oddnessIEta) ++hitIPhi; 
	    // note that hitIPhi could not be 72 before, so it's still in the legal range [1,72]
	  }
	  iHPD += hitIPhi;
	  
	  // book the energies
	  HPD_energy_map [iHPD] += hitE;
	  RBX_energy_map [iRBX] += hitE;
	  if( iDbg > 5) cout<<" --> H["<<iHPD<<"]="<<HPD_energy_map [iHPD]
			    <<", R["<<iRBX<<"]="<<RBX_energy_map[iRBX];
	  if( iDbg > 1) cout<<endl;
	  totHCalE += hitE;
	} // if HBHE
	if( hitE == 0) edm::LogWarning("SuspiciousEventContent")<<"HCal hitE==0? (or unknown subdetector?)";
	HBEF_energies.push_back (hitE); // don't book the HO energies
	
      } // if HCAL 
      else if( detNum == DetId::Ecal) {
	
	towerContainsEM = true;
	++nEMHits;
	int EcalNum =  cellIDs[iCell].subdetId();
	if( EcalNum == 1 ){
	  EBDetId EcalID = cellIDs[iCell];
	  EBRecHitCollection::const_iterator theRecHit=EBRecHits->find(EcalID);
	  hitE = theRecHit->energy();
	  raw_flags = theRecHit->flags();
	  my_flags = ECAL2jetIDflags( raw_flags );
	  markFlags( EB_problem_counts, EB_problem_energies, n_ECAL_flags, my_flags, hitE );
	  SDbits |= 0x10;
	} else if(  EcalNum == 2 ){
	  EEDetId EcalID = cellIDs[iCell];
	  EERecHitCollection::const_iterator theRecHit=EERecHits->find(EcalID);
	  hitE = theRecHit->energy();
	  raw_flags = theRecHit->flags();
	  my_flags = ECAL2jetIDflags( raw_flags );
	  markFlags( EE_problem_counts, EE_problem_energies, n_ECAL_flags, my_flags, hitE );
	  SDbits |= 0x20;
	}
	if( iDbg > 6) cout<<"EcalNum: "<<EcalNum<<" hitE: "<<hitE<<endl;
	ECal_energies.push_back (hitE);
      }
      
      if( ( raw_flags || my_flags ) && iDbg > 0 ) {
	cout<<"DBG flags "<<raw_flags<<" --> "<<my_flags<<", E: "<<hitE<<", counts:\nHB:  ";
	std::copy( HB_problem_counts.begin(), HB_problem_counts.end(), 
		   std::ostream_iterator<int> ( cout, " / " ) ); cout<<"\nHE: ";
	std::copy( HE_problem_counts.begin(), HE_problem_counts.end(), 
		   std::ostream_iterator<int> ( cout, " / " ) ); cout<<"\nEB: ";
	std::copy( EB_problem_counts.begin(), EB_problem_counts.end(), 
		   std::ostream_iterator<int> ( cout, " / " ) ); cout<<endl;
      }
    } // loop on cells
    if( towerContainsEM) ++nEMTowers;
    if( towerContainsHad) ++nHadTowers;
  } // loop on towers

  double expHCalE = jet.energy() * (1-jet.emEnergyFraction());
  if( TMath::Abs (totHCalE - expHCalE) > 0.01 && (totHCalE - expHCalE) / (totHCalE + expHCalE) > 0.0001 ) {
    edm::LogWarning("CodeAssumptionsViolated")<<"failed to account for all HCal energies"
					      <<totHCalE<<"!="<<expHCalE;
  }    

  // sort the energies
  std::sort (HBEF_energies.begin(), HBEF_energies.end(), greater<double>());
  std::sort (ECal_energies.begin(), ECal_energies.end(), greater<double>());

  // put the energy sums (the 2nd entry in each pair of the maps) into the output vectors and sort them
  std::transform (HPD_energy_map.begin(), HPD_energy_map.end(), 
		  std::inserter (HPD_energies, HPD_energies.end()), CaloJetIDAnalyzer::select2nd); 
  //		  std::select2nd<std::map<int,double>::value_type>());
  std::sort (HPD_energies.begin(), HPD_energies.end(), greater<double>());
  std::transform (RBX_energy_map.begin(), RBX_energy_map.end(), 
		  std::inserter (RBX_energies, RBX_energies.end()), CaloJetIDAnalyzer::select2nd);
  //		  std::select2nd<std::map<int,double>::value_type>());
  std::sort (RBX_energies.begin(), RBX_energies.end(), greater<double>());

  vector<double> recHit_energies (HBEF_energies.begin(), HBEF_energies.end());
  if( iDbg>7) cout<<"\n DBG 1 # recHit_energies "<<recHit_energies.size();
  recHit_energies.insert (recHit_energies.end(), ECal_energies.begin(), ECal_energies.end());
  std::sort (recHit_energies.begin(), recHit_energies.end(), greater<double>());
  double totalE = 0;
  for (unsigned int i = 0; i < recHit_energies.size(); ++i) totalE += recHit_energies[i];
  double runningE = 0;
  n50 = n90 = recHit_energies.size();
  for (unsigned int i = recHit_energies.size(); i > 0; --i) {
    runningE += recHit_energies[i-1];
    if( runningE < 0.1 * totalE) n90 = i-1;
    if( runningE < 0.5 * totalE) n50 = i-1;
    if( iDbg>18) cout<<"DBG i: "<<i<<" E[i-1]: "<<recHit_energies[i-1]<<" runningE: "<<runningE
		    <<" vs. totalE: "<<totalE<<" -> n50: "<<n50<<", n90: "<<n90<<endl;
  }
  if( ( SDbits & 0x1 ) > 0 ) ++nSD;
  if( ( SDbits & 0x2 ) > 0 )++nSD;
  if( ( SDbits & 0x4 ) > 0 ) ++nSD;
  if( ( SDbits & 0x8 ) > 0 ) ++nSD;
  if( ( SDbits & 0x10 ) > 0 ) ++nSD;
  if( ( SDbits & 0x20 ) > 0 ) ++nSD;
  if( iDbg>1) cout<<"Exiting getCalComponents"<<endl;
  return;
}

void CaloJetIDAnalyzer::classifyJetTowers( 
  const edm::Event& event, const reco::CaloJet &jet, 
  std::vector< reco::helper::JetIDHelper::subtower > &subtowers,      
  std::vector< reco::helper::JetIDHelper::subtower > &Ecal_subtowers, 
  std::vector< reco::helper::JetIDHelper::subtower > &Hcal_subtowers, 
  std::vector< reco::helper::JetIDHelper::subtower > &HO_subtowers,
  std::vector< double > &HPD_energies,  
  std::vector< double > &RBX_energies,
  unsigned int& n_bad_towers, unsigned int& n_bad_subtowers,
  unsigned int& n_bad_ecal_cells, unsigned int& n_bad_hcal_cells,
  unsigned int& n_recovered_towers, unsigned int& n_recovered_subtowers, 
  unsigned int& n_recovered_ecal_cells, unsigned int& n_recovered_hcal_cells,
  unsigned int& n_prob_towers, unsigned int& n_prob_subtowers, 
  unsigned int& n_prob_ecal_cells, unsigned int& n_prob_hcal_cells,
  double& E_bad_tower, double& E_bad_ecal, double& E_bad_hcal,
  double& E_recovered_tower, double& E_recovered_ecal, double& E_recovered_hcal,
  double& E_prob_tower, double& E_prob_ecal, double& E_prob_hcal,
  const int iDbg )
{
  subtowers.clear(); Ecal_subtowers.clear(); Hcal_subtowers.clear(); HO_subtowers.clear();
  HPD_energies.clear(); RBX_energies.clear();
  n_bad_towers = n_bad_subtowers = n_bad_ecal_cells = n_bad_hcal_cells = 0;
  n_recovered_towers = n_recovered_subtowers = n_recovered_ecal_cells = n_recovered_hcal_cells = 0;
  n_prob_towers = n_prob_subtowers = n_prob_ecal_cells = n_prob_hcal_cells = 0;
  E_bad_tower = E_bad_ecal = E_bad_hcal = 0;
  E_recovered_tower = E_recovered_ecal = E_recovered_hcal = 0;
  E_prob_tower = E_prob_ecal = E_prob_hcal = 0;

  std::map< int, double > HPD_energy_map, RBX_energy_map;

  vector< CaloTowerPtr > towers = jet.getCaloConstituents ();
  int nTowers = towers.size();
  if( iDbg > 9 ) cout<<"classifyJetTowers started. # of towers found: "<<nTowers<<endl;

  for( int iTower = 0; iTower <nTowers ; iTower++ ) {

    CaloTowerPtr& tower = towers[iTower];

    int nEM = 0, nHad = 0, nHO = 0;
    const vector< DetId >& cellIDs = tower->constituents();  // cell == recHit
    int nCells = cellIDs.size();
    if( iDbg ) cout<<"tower #"<<iTower<<" has "<<nCells<<" cells. "
		   <<"It's at iEta: "<<tower->ieta()<<", iPhi: "<<tower->iphi()<<endl;
  
    for( int iCell = 0; iCell < nCells; ++iCell ) {
      DetId::Detector detNum = cellIDs[iCell].det();
      if( detNum == DetId::Hcal ) {
	HcalDetId HcalID = cellIDs[ iCell ];
	HcalSubdetector HcalNum = HcalID.subdet();
	if( HcalNum == HcalOuter ) {
	  ++nHO;
	} else {
	  ++nHad;
	}
      }	else if (detNum == DetId::Ecal) {
	++nEM;
      }
    }

    double E_em = tower->emEnergy();
    if( E_em != 0 ) Ecal_subtowers.push_back( reco::helper::JetIDHelper::subtower( E_em, nEM ) );
      
    double E_HO = tower->outerEnergy();
    if( E_HO != 0 ) HO_subtowers.push_back( reco::helper::JetIDHelper::subtower( E_HO, nHO ) );
      
    double E_had = tower->hadEnergy();
    if( E_had != 0 ) {
      Hcal_subtowers.push_back( reco::helper::JetIDHelper::subtower( E_had, nHad ) );
      // totHcalE += E_had;
      
      int iEta = tower->ieta();
      Region reg = region( iEta );
      int iPhi = tower->iphi();
      if( iDbg>3 ) cout<<"tower has E_had: "<<E_had<<" iEta: "<<iEta
		       <<", iPhi: "<<iPhi<<" -> "<<reg;
      
      if( reg == HEneg || reg == HBneg || reg == HBpos || reg == HEpos ) {
	int oddnessIEta = HBHE_oddness( iEta );
	if( oddnessIEta < 0 ) break; // can't assign this tower to a single readout component
	
	int iHPD = 100 * reg;
	int iRBX = 100 * reg + ((iPhi + 1) % 72) / 4; // 71,72,1,2 are in the same RBX module
	
	if(( reg == HEneg || reg == HEpos ) && std::abs( iEta ) >= 21 ) { // at low-granularity edge of HE
	  if( (0x1 & iPhi) == 0 ) {
	    edm::LogError("CodeAssumptionsViolated")<<
	      "Bug?! Jet ID code assumes no even iPhi recHits at HE edges";
	    return;
	  }
	  bool boolOddnessIEta = oddnessIEta;
	  bool upperIPhi = (( iPhi%4 ) == 1 || ( iPhi%4 ) == 2); // the upper iPhi indices in the HE wedge
	  // remap the iPhi so it fits the one in the inner HE regions, change in needed in two cases:
	  // 1) in the upper iPhis of the module, the even IEtas belong to the higher iPhi
	  // 2) in the loewr iPhis of the module, the odd  IEtas belong to the higher iPhi
	  if( upperIPhi != boolOddnessIEta ) ++iPhi; 
	  // note that iPhi could not be 72 before, so it's still in the legal range [1,72]
	} // if at low-granularity edge of HE
	iHPD += iPhi;
	
	// book the energies
	HPD_energy_map[ iHPD ] += E_had;
	RBX_energy_map[ iRBX ] += E_had;
	if( iDbg > 5 ) cout<<" --> H["<<iHPD<<"]="<<HPD_energy_map[iHPD]
			   <<", R["<<iRBX<<"]="<<RBX_energy_map[iRBX];
      } // HBHE
    } // E_had > 0

    if( tower->numBadHcalCells() > 0 || tower->numBadEcalCells() > 0 ) {
      ++n_bad_towers;
      E_bad_tower += tower->energy();
      if( iDbg ) cerr<<"saw bad cells. E_bad_tower: "<<E_bad_tower<<endl;
    }
    if( tower->numBadHcalCells() > 0 ) {
      ++n_bad_subtowers;
      E_bad_hcal += E_had;
      if( iDbg ) cerr<<"saw bad HCAL cells. n_bad_subtowers: "<<n_bad_subtowers<<endl;
    }
    if( tower->numBadEcalCells() > 0 ) {
      ++n_bad_subtowers;
      E_bad_ecal += E_em;
      if( iDbg ) cerr<<"saw bad ECAL cells. n_bad_subtowers: "<<n_bad_subtowers<<endl;
    }
    n_bad_ecal_cells += tower->numBadEcalCells();
    n_bad_hcal_cells += tower->numBadHcalCells();
    
    if( tower->numRecoveredHcalCells() > 0 || tower->numRecoveredEcalCells() > 0 ) {
      ++n_recovered_towers;
      E_recovered_tower += tower->energy();
      if( iDbg ) cerr<<"saw recovered cells. E_recovered_tower: "<<E_recovered_tower<<endl;
    }
    if( tower->numRecoveredHcalCells() > 0 ) {
      ++n_recovered_subtowers;
      E_recovered_hcal += E_had;
    }
    if( tower->numRecoveredEcalCells() > 0 ) {
      ++n_recovered_subtowers;
      E_recovered_ecal += E_em;
    }
    n_recovered_ecal_cells += tower->numRecoveredEcalCells();
    n_recovered_hcal_cells += tower->numRecoveredHcalCells();
    
    if( tower->numProblematicHcalCells() > 0 || tower->numProblematicEcalCells() > 0 ) {
      ++n_prob_towers;
      E_prob_tower += tower->energy();
      if( iDbg ) cerr<<"saw problematic cells. E_prob_tower: "<<E_prob_tower<<endl;
    }
    if( tower->numProblematicHcalCells() > 0 ) {
      ++n_prob_subtowers;
      E_prob_hcal += E_had;
    }
    if( tower->numProblematicEcalCells() > 0 ) {
      ++n_prob_subtowers;
      E_prob_ecal += E_em;
    }
    n_prob_ecal_cells += tower->numProblematicEcalCells();
    n_prob_hcal_cells += tower->numProblematicHcalCells();
    
  } // loop on towers

  // sort the subtowers
  std::sort( Hcal_subtowers.begin(), Hcal_subtowers.end(), subtower_has_greater_E );
  std::sort( Ecal_subtowers.begin(), Ecal_subtowers.end(), subtower_has_greater_E );

  // put the energy sums (the 2nd entry in each pair of the maps) into the output vectors and sort them
  std::transform( HPD_energy_map.begin(), HPD_energy_map.end(), 
		  std::inserter (HPD_energies, HPD_energies.end()), select2nd ); 
  //		  std::select2nd<std::map<int,double>::value_type>());
  std::sort( HPD_energies.begin(), HPD_energies.end(), greater<double>() );
  std::transform( RBX_energy_map.begin(), RBX_energy_map.end(), 
		  std::inserter (RBX_energies, RBX_energies.end()), select2nd );
  //		  std::select2nd<std::map<int,double>::value_type>());
  std::sort( RBX_energies.begin(), RBX_energies.end(), greater<double>() );

  subtowers.insert( subtowers.end(), Hcal_subtowers.begin(), Hcal_subtowers.end() );
  subtowers.insert( subtowers.end(), Ecal_subtowers.begin(), Ecal_subtowers.end() );
  subtowers.insert( subtowers.end(), HO_subtowers.begin(), HO_subtowers.end() );
  std::sort( subtowers.begin(), subtowers.end(), subtower_has_greater_E );
}

//______________________________________________________________________________
bool CaloJetIDAnalyzer::prettyClose (double x, double y) 
{
  if( x == 0 && y == 0) return true;
  if( TMath::Abs (x - y) < 4 * std::numeric_limits<double>::epsilon()) return true;
  if( x == 0 || y == 0) return false;
  if( TMath::Abs ((x-y) / x) < 1E-6) return true;
  return false;
}

 //______________________________________________________________________________
int CaloJetIDAnalyzer::calcMyIEta (int iEta, int depth) // negative depth = ECal, positive = HCal depths
{
 int ae = TMath::Abs (iEta);
  if( ae == 29 && depth == 1) ae += 1; // this is what works: hits are at depths 1 & 2, 1 goes with the even pattern
  if(  (ae == 15 && depth == 3) || (ae == 16 && depth == 3) || ae >= 17) ae += 2;
  // note: |myIeta|==17 is for depth 3 in iEta=15 which is HO, and so far unused.
  return TMath::Sign (ae, iEta);
}

//______________________________________________________________________________
// 
void CaloJetIDAnalyzer::analyzeNonjetTriggers (const TrgEvtHandle_t& triggerEvent, 
						const TrgResHandle_t& triggerResults)
{
  isRandomTrigger_ = false;
  triggerMuObjects_.clear();
  //
  // Code based on HLTrigger/HLTcore/plugins/HLTEventAnalyzerAOD.cc
  //

  if( !triggerResults.isValid() || !triggerEvent.isValid()) {
    edm::LogWarning("BadInput")<<"trigger results (and/or event) not available";
    return;
  }

  // sanity check
  assert (triggerResults->size()==hltConfig_.size());  

  // just to warm up, use my old code to mark all triggers seen in the events 
  for (trigger::size_type ifilter=0; ifilter < triggerEvent->sizeFilters(); ++ifilter) {
    const trigger::Keys& keys = triggerEvent->filterKeys(ifilter); 
    if( keys.size() > 0) {
      string sTag = triggerEvent->filterTag(ifilter).encode();
      triggersSeen_.insert (sTag);
      LogTrace("DebugTrigger")<<"DBG - ifilter: "<<ifilter<<" is: "<<sTag<<", #K: "<<keys.size()<<endl;
    }
  }

  // back to official trigger usage :-)
  const TriggerObjectCollection& TOC (triggerEvent->getObjects());
  std::set<int> iSelectedObjects;

  const unsigned int nConfiguredTriggers = hltConfig_.size();
  LogTrace("DebugTrigger")<<"DBG - nObj: "<<TOC.size()<<" nConfiguredTriggers: "<<nConfiguredTriggers<<endl;
  for (unsigned int ict=0; ict < nConfiguredTriggers; ++ict) { // loop on ALL configured triggers 
    // ict = what the original analyzeTrigger called triggerIndex
    const string triggerName (hltConfig_.triggerName(ict));
    
    LogTrace("DebugTrigger")<<"DBG - ict: "<<ict<<" is: "<<triggerName<<" run? "<<triggerResults->wasrun(ict)
			    <<" accept? "<<triggerResults->accept(ict)<<endl;

    // only interested in triggers that actually fired
    if(  ! (triggerResults->wasrun(ict) && triggerResults->accept(ict)) ) continue;

    if( triggerName.find ("RandomPath") != string::npos) isRandomTrigger_ = true;

    // and may be muon triggers  (there's one called PhysicsNoMuonPath, which probably doesn't require muons)
    if( triggerName.find ("Mu") == string::npos
	|| triggerName.find("NoMuon") != string::npos
	|| triggerName.find("BTagMu") != string::npos) continue;

    // and are not used as jet triggers
    bool cppStyle = (std::find (triggerNames_.begin(), triggerNames_.end(), triggerName) != triggerNames_.end());
    bool cStyle = false;
    for (unsigned int ijt=0; ijt<triggerNames_.size(); ++ijt) {
      if( triggerName == triggerNames_[ijt]) cStyle = true;
    }
    assert (cppStyle == cStyle);
    LogTrace("DebugTrigger")<<"DBG - known jet trigger? "<<cppStyle<<endl;
    if( std::find (triggerNames_.begin(), triggerNames_.end(), triggerName) != triggerNames_.end()) continue;

    // OK, will use all the objects from this one
    triggersUsedAsMu_.insert (triggerName);

    // modules on this trigger path
    const unsigned int nModules = hltConfig_.size(ict);
    const vector<string>& moduleLabels (hltConfig_.moduleLabels(ict));
    // Get index (slot position) of module giving the decision of the "ict"th path
    const unsigned int moduleIndex = triggerResults->index(ict);
    LogTrace("DebugTrigger")<<"DBG - ict: "<<ict<<" Last active module - label/type: "
	 << moduleLabels[moduleIndex] << "/" << hltConfig_.moduleType(moduleLabels[moduleIndex])
	 << " [" << moduleIndex << " out of 0-" << (nModules-1) << " on this path]" << endl; 
    assert (moduleIndex < nModules);

    // Results from TriggerEvent product - Attention: must look only for
    // modules actually run in this path for this event!
    for (unsigned int iMod=0; iMod<=moduleIndex; ++iMod) {
      const string& moduleLabel(moduleLabels[iMod]);
      const string  moduleType(hltConfig_.moduleType(moduleLabel));
      // check whether the module is packed up in TriggerEvent product
      const unsigned int filterIndex = triggerEvent->filterIndex(edm::InputTag(moduleLabel,"",hltProcessName_));
      if( filterIndex < triggerEvent->sizeFilters()) {
	LogTrace("DebugTrigger")<<"DBG - 'L3' filter in slot " << iMod 
				<< " - label/type " << moduleLabel << "/" << moduleType << endl;
	const trigger::Vids& VIDS (triggerEvent->filterIds(filterIndex));
	const trigger::Keys& KEYS (triggerEvent->filterKeys(filterIndex));
	const trigger::size_type nI(VIDS.size());
	const trigger::size_type nK(KEYS.size());
	assert (nI == nK);
	LogTrace("DebugTrigger")<<"DBG - " << nK  << " accepted 'L3' objects found: " << endl;
	for (trigger::size_type iObj=0; iObj < nK; ++iObj) {
	  iSelectedObjects.insert (KEYS[iObj]);
	  const TriggerObject& TO(TOC[KEYS[iObj]]);
	  LogTrace("DebugTrigger")<<"    -  " << iObj << " " << VIDS[iObj] << "/" << KEYS[iObj] << ": "
				  << TO.id() << " " << TO.pt() << " " << TO.eta() 
				  << " " << TO.phi() << " " << TO.mass() << endl;
	} // loop on trigger objects
      } // if filter found
    } // loop on modules in trigger path
  } // loop on configured triggers

  for (std::set<int>::const_iterator it=iSelectedObjects.begin(); it != iSelectedObjects.end(); ++it) 
    triggerMuObjects_.push_back (TOC.at(*it));

  std::sort (triggerMuObjects_.begin(), triggerMuObjects_.end(), compareByPt);

  LogTrace("DebugTrigger")<<"DBG - found "<<triggerMuObjects_.size()<<" objects: ";
  for (vector<TriggerObject>::const_iterator it=triggerMuObjects_.begin(); it != triggerMuObjects_.end(); ++it) {
    LogTrace("DebugTrigger")<<it->pt()<<" "<<it->phi()<<" "<<it->eta()<<" ";
  }
  LogTrace("DebugTrigger")<<endl;
  //std::copy (triggerMuObjects_.begin(), triggerMuObjects_.end(), std::ostream_iterator<TriggerObject>(cout, "\n"));
  //for_each (triggerMuObjects_.begin(), triggerMuObjects_.end(), cout<<_1.pt()<<" "<<_1.phi()<<" "<<_1.eta()<<" ";
}

// From JetIDHelper, until these functions are made public


int CaloJetIDAnalyzer::HBHE_oddness( int iEta )
{
 int ae = TMath::Abs( iEta );
 if( ae == 29 ) return -1; // can't figure it out without RecHits
 return ae & 0x1;
}

CaloJetIDAnalyzer::Region CaloJetIDAnalyzer::region( int iEta )
{
  if( iEta == 16 || iEta == -16 ) return unknown_region; // both HB and HE cells belong to these towers
  if( iEta == 29 || iEta == -29 ) return unknown_region; // both HE and HF cells belong to these towers
  if( iEta <= -30 ) return HFneg;
  if( iEta >=  30 ) return HFpos;
  if( iEta <= -17 ) return HEneg;
  if( iEta >=  17 ) return HEpos;
  if( iEta < 0 ) return HBneg;
  return HBpos;
}


////////////////////////////////////////////////////////////////////////////////
// define CaloJetIDAnalyzer as a plugin
////////////////////////////////////////////////////////////////////////////////

DEFINE_FWK_MODULE(CaloJetIDAnalyzer);
