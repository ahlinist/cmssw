#ifndef TREEANALYSIS_H
#define TREEANALYSIS_H

#include <iostream>

#include <TROOT.h>
#include <TString.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TF1.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TRandom3.h>

#include "../rootio/TAna00Event.hh"
#include "../rootio/TGenCand.hh"
#include "../rootio/TAnaTrack.hh"
#include "../rootio/TAnaCand.hh"
#include "../rootio/TAnaVertex.hh"
#include "../rootio/TAnaJet.hh"


using namespace std;

const double pi = 3.14159265;
const double mmuon = 0.10565837;


class treeAnalysis {
public:
  treeAnalysis(TChain *tree, TString evtClassName);
  ~treeAnalysis();
  void        init(TString evtClassName);

  void        InitCutVariables();
  void        InitVariables();
  void        readCuts(TString filename, int dump = 1);
  void        openHistFile(TString filename);
  void        closeHistFile();
  void        chainFileName(const char *name);
  void        bookHist(); 
  void        setTitles(TH1 *h, const char *sx, const char *sy,
			float size = 0.06, float xoff = 1.2, float yoff = 1.7, float lsize = 0.06, int font = 42);
  void        cdDirectory(int i, int j); 
  void        startAnalysis(); 
  void        endAnalysis();
  int         loop(int nevents = 1, int start = -1, int option=0, double weightlow =1., double weight020 =1., double weight2030 =1., double weight3050 =1., double weight5080 =1., double weight80120 =1., double weight120170 =1., double weight170 =1., double pthat =0.); 

  void        eventProcessing(int option=0, double weightlow =1., double weight020 =1., double weight2030 =1., double weight3050 =1., double weight5080 =1., double weight80120 =1., double weight120170 =1., double weight170 =1., double pthat =0. ); 
  
  void        debugLevel(int i) {fDebug = i;}
  void        dumpEvent();
 
  void        CorrectTrackJets();
  int         IsGlobalMuonTrack(int track_index);
 
  int         GetEventOrigin();
  int         GetMuonOrigin(TGenCand* muon);  

  int         NumberOfGENMuons();
  int         NumberOfRECOMuons();
  int         NumberOfTriggerMuons(int level);
  int         NumberOfRECOTracks();
 
  int         GetHighestPtRECOMuon(double ptmin_muon = 0, double eta_muon = 99999);
  int         NumberOfCaloJetsInCone(TAnaCand* muon, double rmin, double etmin_jet);
  int         NumberOfTrackJetsInCone(TAnaCand* muon, double rmin, double etmin_jet);

  void        FillGENMuons();
  int         GENGetQuark(TGenCand* muon);
  int         GENGetClosestGenJet(TGenCand* cand, double etmin_jet);
  int         GENGetTrueJet(TGenCand* muon);
  int         GENGetClosestL1(TGenCand* cand);
  int         GENGetClosestHLTL2(TGenCand* cand);
  int         GENGetClosestHLTL3(TGenCand* cand);
  int         GENGetRecoTrack(TGenCand* cand);
  double      GENDeltaR(TGenCand* cand, int index);
  double      GENPtRel(TGenCand* cand, int index);
  TLorentzVector   GetGenJetWithoutNu(TAnaJet* jet); 
  TLorentzVector   GetGenJetWithoutMu(TAnaJet* jet);  
 
  

  void        FillRECOMuons(double etmin_jet, double etmin_calojet);
  int         RECOGetClosestCaloJet(TAnaCand* cand, double etmin_jet);
  int         RECOGetClosestCaloJet(TAnaTrack* track, double etmin_jet); 
  int         RECOGetClosestTrackJet(TAnaCand* cand, double etmin_jet);
  int         RECOGetClosestL1(TAnaCand* cand);
  int         RECOGetClosestHLTL2(TAnaCand* cand); 
  int         RECOGetClosestHLTL3(TAnaCand* cand);
  int         RECOGetGenMuonCandIndex(int genindex);
  double      RECODeltaR1(TAnaCand* cand, int index);
  double      RECOPtRel1(TAnaCand* cand, int index);
  double      RECODeltaR2(TAnaCand* cand, int index);
  double      RECOPtRel2(TAnaCand* cand, int index);
 
  int         RECOGetJetToWhichMuonBelongs(TAnaCand* cand);

  double      GetSumPt(TAnaCand* muon, double radius);
  int         GetNTracksInCone(TAnaCand* muon, double radius);
  
private:

  TChain     *fpChain;        // pointer to the analyzed TTree or TChain
  TFile      *fpHistFile;     // for output histograms and reduced trees
  TString     fCutFile;       // contains file with the cut definitions
  TString     fChannel;       // Channel signature (used for rare background)
  TString     fChainFileName; // the name of the chain file
  int         fNentries;      // number of events in chain
  int         fEvent;         // current event number

  TAna00Event*fpEvt; 

  // -- Histogram pointers 
  TTree *fGenTree;
  TTree *fRecoTree;
 
  int fDebug;
 


  // -- Cut values 
  double PTLOMUON, ETAMUON, PTLOGENMUON, HLTMUON; 
  double CHIMUON, DZMUON, D0MUON, TRKHITMUON;
  double ETLOJET, ETAJET, ETCALOJET, ETTRKJET, ETGENJET;
  double CONEMUJET;
  double PTHATMIN, PTHATMAX;
  double ALPHA; 
  double L1MATCH, HLTMATCH, JETMATCH;
  double PIXLAY, TRKLAY, DZCUT;
   
  // -- Variables 
  int fNrecomuons, fNrecotracks;
  int fNcalojets, fNtrkjets;
  int fNL1muons, fNHLTL2muons, fNHLTL3muons;
  int ihighRECOmuon;

  //variables for reco event
  double recomuon_pt, recomuon_phi, recomuon_eta;
  double genmuon_pt, genmuon_phi, genmuon_eta;
  double genjet_et, genjet_phi, genjet_eta;
  int    recomuon_q, recomuon_istm;  
  double recomuon_ip, recomuon_ipe;
  double recomuon_lip, recomuon_lipe; 
  int    recomuon_mcid;
 
  double trkjet_et, trkjet_phi, trkjet_eta;
  int    trkjet_ntrks;
  double reco_deltaR, reco_ptrel;
 
  double l1muon_pt, l1muon_phi, l1muon_eta;
  double hltl2muon_pt, hltl2muon_phi, hltl2muon_eta;
  double hltl3muon_pt, hltl3muon_phi, hltl3muon_eta; 

  int eventtag, muontag;
   
  double recomuon_gl_chi, recomuon_sumpt, recomuon_tr_chi;
  int recomuon_gl_ndof, recomuon_gl_hits, recomuon_gl_cschits, recomuon_gl_dthits, recomuon_gl_rpchits, recomuon_gl_pixhits, recomuon_gl_trkhits, recomuon_gl_pixlyrs, recomuon_gl_trklyrs, recomuon_tr_ndof, recomuon_tr_pixhits, recomuon_tr_trkhits, recomuon_tr_pixlyrs, recomuon_tr_trklyrs, recomuon_ntrks; 

  double track1_ip, track1_ips, track1_lip, track1_lips, track1_pt;
  double track2_ip, track2_ips, track2_lip, track2_lips, track2_pt;
  double track3_ip, track3_ips, track3_lip, track3_lips, track3_pt;
 
 

  bool first;
  
  TRandom3 *Random;
 
 


};



#endif
