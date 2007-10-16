#ifndef UnderlyingEventAnalyzer_H
#define UnderlyingEventAnalyzer_H

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "DataFormats/Common/interface/Ref.h" 
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/TrackCandidate/interface/TrackCandidate.h"
#include "DataFormats/TrackCandidate/interface/TrackCandidateCollection.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/BasicJet.h"
#include "DataFormats/JetReco/interface/BasicJetCollection.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleCandidate.h"
#include "TProfile.h"

// forward declarations
class TFile;
class TH1D;

using namespace edm;
using namespace std;
using namespace reco;

class UnderlyingEventAnalyzer : public edm::EDAnalyzer
{
  
public:
  
  //
  explicit UnderlyingEventAnalyzer( const edm::ParameterSet& ) ;
  virtual ~UnderlyingEventAnalyzer() {} // no need to delete ROOT stuff
  // as it'll be deleted upon closing TFile
  
  virtual void analyze( const edm::Event&, const edm::EventSetup& );
  virtual void beginJob( const edm::EventSetup& );
  virtual void endJob();
  
  void mcAnalysis(vector<math::XYZTLorentzVector>&,vector<GenJet>&);
  void tkAnalysis(vector<math::XYZTLorentzVector>&,vector<BasicJet>&);

  float_t delR(const float_t&,const float_t&,const float_t&,const float_t&);
  
  
private:
  
  //
  std::string fOutputFileName ;
  std::string mcEvent;
  std::string chgJetCollName;
  std::string tracksJetCollName;
  std::string recoCaloJetCollName;
  std::string chgGenPartCollName;
  std::string tracksCollName;
  bool mcInfo;
  bool tkInfo;
  double ptTrack;
  bool triggerSim;
  bool back2back;
  double triggerCut;
  double etaRegion;
  float_t piG;

  TFile*      fOutputFile;

  TH1D*       fHistNumbCJB;
  TH1D*       fHistNumbCJA;

  TH1D*       fHistPtDistMC;
  TH1D*       fHistEtaDistMC;
  TH1D*       fHistPhiDistMC;
  TProfile*   pavgPt_vs_NchgMC;
  TProfile*   pdN_vs_dphiMC;
  TProfile*   pdPt_vs_dphiMC;
  TProfile*   pdN_vs_dphiTransMC;
  TProfile*   pdPt_vs_dphiTransMC;
  TProfile*   pdN_vs_etaMC;
  TProfile*   pdN_vs_ptMC;
  
  TProfile*   pdN_vs_ptJTransMC_LLG;
  TProfile*   pdN_vs_ptJTransMaxMC_LLG;
  TProfile*   pdN_vs_ptJTransMinMC_LLG;
  TProfile*   pdPt_vs_ptJTransMC_LLG;
  TProfile*   pdPt_vs_ptJTransMaxMC_LLG;
  TProfile*   pdPt_vs_ptJTransMinMC_LLG;
  TProfile*   pdN_vs_ptJTowardMC_LLG;
  TProfile*   pdN_vs_ptJAwayMC_LLG;
  TProfile*   pdPt_vs_ptJTowardMC_LLG;
  TProfile*   pdPt_vs_ptJAwayMC_LLG;
  
  TProfile*   pdN_vs_ptJTransMC_LHG;
  TProfile*   pdN_vs_ptJTransMaxMC_LHG;
  TProfile*   pdN_vs_ptJTransMinMC_LHG;
  TProfile*   pdPt_vs_ptJTransMC_LHG;
  TProfile*   pdPt_vs_ptJTransMaxMC_LHG;
  TProfile*   pdPt_vs_ptJTransMinMC_LHG;
  TProfile*   pdN_vs_ptJTowardMC_LHG;
  TProfile*   pdN_vs_ptJAwayMC_LHG;
  TProfile*   pdPt_vs_ptJTowardMC_LHG;
  TProfile*   pdPt_vs_ptJAwayMC_LHG;
  
  TProfile*   pdN_vs_ptJTransMC_HLG;
  TProfile*   pdN_vs_ptJTransMaxMC_HLG;
  TProfile*   pdN_vs_ptJTransMinMC_HLG;
  TProfile*   pdPt_vs_ptJTransMC_HLG;
  TProfile*   pdPt_vs_ptJTransMaxMC_HLG;
  TProfile*   pdPt_vs_ptJTransMinMC_HLG;
  TProfile*   pdN_vs_ptJTowardMC_HLG;
  TProfile*   pdN_vs_ptJAwayMC_HLG;
  TProfile*   pdPt_vs_ptJTowardMC_HLG;
  TProfile*   pdPt_vs_ptJAwayMC_HLG;
  
  TProfile*   pdN_vs_ptJTransMC_HHG;
  TProfile*   pdN_vs_ptJTransMaxMC_HHG;
  TProfile*   pdN_vs_ptJTransMinMC_HHG;
  TProfile*   pdPt_vs_ptJTransMC_HHG;
  TProfile*   pdPt_vs_ptJTransMaxMC_HHG;
  TProfile*   pdPt_vs_ptJTransMinMC_HHG;
  TProfile*   pdN_vs_ptJTowardMC_HHG;
  TProfile*   pdN_vs_ptJAwayMC_HHG;
  TProfile*   pdPt_vs_ptJTowardMC_HHG;
  TProfile*   pdPt_vs_ptJAwayMC_HHG;
  
  TH1D*       temp1MC;
  TH1D*       temp2MC;
  TH1D*       temp3MC;
  TH1D*       temp4MC;
  
  TH1D*       fHistPtDistRECO;
  TH1D*       fHistEtaDistRECO;
  TH1D*       fHistPhiDistRECO;
  TProfile*   pavgPt_vs_NchgRECO;
  TProfile*   pdN_vs_dphiRECO;
  TProfile*   pdPt_vs_dphiRECO;
  TProfile*   pdN_vs_dphiTransRECO;
  TProfile*   pdPt_vs_dphiTransRECO;
  TProfile*   pdN_vs_etaRECO;
  TProfile*   pdN_vs_ptRECO;
  
  TProfile*   pdN_vs_ptJTransRECO_LLG;
  TProfile*   pdN_vs_ptJTransMaxRECO_LLG;
  TProfile*   pdN_vs_ptJTransMinRECO_LLG;
  TProfile*   pdPt_vs_ptJTransRECO_LLG;
  TProfile*   pdPt_vs_ptJTransMaxRECO_LLG;
  TProfile*   pdPt_vs_ptJTransMinRECO_LLG;
  TProfile*   pdN_vs_ptJTowardRECO_LLG;
  TProfile*   pdN_vs_ptJAwayRECO_LLG;
  TProfile*   pdPt_vs_ptJTowardRECO_LLG;
  TProfile*   pdPt_vs_ptJAwayRECO_LLG;
  
  TProfile*   pdN_vs_ptJTransRECO_LHG;
  TProfile*   pdN_vs_ptJTransMaxRECO_LHG;
  TProfile*   pdN_vs_ptJTransMinRECO_LHG;
  TProfile*   pdPt_vs_ptJTransRECO_LHG;
  TProfile*   pdPt_vs_ptJTransMaxRECO_LHG;
  TProfile*   pdPt_vs_ptJTransMinRECO_LHG;
  TProfile*   pdN_vs_ptJTowardRECO_LHG;
  TProfile*   pdN_vs_ptJAwayRECO_LHG;
  TProfile*   pdPt_vs_ptJTowardRECO_LHG;
  TProfile*   pdPt_vs_ptJAwayRECO_LHG;
  
  TProfile*   pdN_vs_ptJTransRECO_HLG;
  TProfile*   pdN_vs_ptJTransMaxRECO_HLG;
  TProfile*   pdN_vs_ptJTransMinRECO_HLG;
  TProfile*   pdPt_vs_ptJTransRECO_HLG;
  TProfile*   pdPt_vs_ptJTransMaxRECO_HLG;
  TProfile*   pdPt_vs_ptJTransMinRECO_HLG;
  TProfile*   pdN_vs_ptJTowardRECO_HLG;
  TProfile*   pdN_vs_ptJAwayRECO_HLG;
  TProfile*   pdPt_vs_ptJTowardRECO_HLG;
  TProfile*   pdPt_vs_ptJAwayRECO_HLG;
  
  TProfile*   pdN_vs_ptJTransRECO_HHG;
  TProfile*   pdN_vs_ptJTransMaxRECO_HHG;
  TProfile*   pdN_vs_ptJTransMinRECO_HHG;
  TProfile*   pdPt_vs_ptJTransRECO_HHG;
  TProfile*   pdPt_vs_ptJTransMaxRECO_HHG;
  TProfile*   pdPt_vs_ptJTransMinRECO_HHG;
  TProfile*   pdN_vs_ptJTowardRECO_HHG;
  TProfile*   pdN_vs_ptJAwayRECO_HHG;
  TProfile*   pdPt_vs_ptJTowardRECO_HHG;
  TProfile*   pdPt_vs_ptJAwayRECO_HHG;
  
  TH1D*       temp1RECO;
  TH1D*       temp2RECO;
  TH1D*       temp3RECO;
  TH1D*       temp4RECO;
  
};

#endif
