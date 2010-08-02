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
  void        bookHist(int option); 
  void        setTitles(TH1 *h, const char *sx, const char *sy,
			float size = 0.06, float xoff = 1.2, float yoff = 1.7, float lsize = 0.06, int font = 42);
  void        cdDirectory(int i, int j); 
  void        startAnalysis(); 
  void        endAnalysis();
  int         loop(int nevents = 1, int start = -1, int option=0, double weightlow =1., double weight020 =1., double weight2030 =1., double weight3050 =1., double weight5080 =1., double weight80120 =1., double weight120170 =1., double weight170 =1., double pthat =0.); 

  void        eventProcessing(int option=0, double weightlow =1., double weight020 =1., double weight2030 =1., double weight3050 =1., double weight5080 =1., double weight80120 =1., double weight120170 =1., double weight170 =1., double pthat =0. ); 
  
  void        debugLevel(int i) {fDebug = i;}
  void        dumpEvent();

  int         IsGlobalMuonTrack(int track_index);
  double      GetWeight (double pt, double eta);

  int         NumberOfGENMuons();
  int         NumberOfRECOMuons();
  int         NumberOfTriggerMuons(int level);
  int         NumberOfRECOTracks();
  int         GetHighestPtGENMuon(double ptmin_muon = 0, double eta_muon = 99999);
  int         GetHighestPtRECOMuon(double ptmin_muon = 0, double eta_muon = 99999);
  int         GetHighestPtFakeMuon(double ptmin_muon = 0, double eta_muon = 99999);
  int         IsFakeMuon(double pt_track, double eta_track);
 
  void        FillRECOFakeMuons(TAnaTrack* tr, double etmin_jet);
  int         RECOGetClosestCaloJet(TAnaCand* cand, double etmin_jet);
  int         RECOGetClosestCaloJet(TAnaTrack* track, double etmin_jet); 
  int         RECOGetClosestTrackJet(TAnaCand* cand, double etmin_jet);
  int         RECOGetClosestL1(TAnaCand* cand);
  int         RECOGetClosestHLTL2(TAnaCand* cand); 
  int         RECOGetClosestHLTL3(TAnaCand* cand);
  double      RECODeltaR1(TAnaCand* cand, int index);
  double      RECOPtRel1(TAnaCand* cand, int index);
  double      RECODeltaR2(TAnaCand* cand, int index);
  double      RECOPtRel2(TAnaCand* cand, int index);
  TLorentzVector    GetTrackJetWithoutMu(TAnaJet* jet);  
 
  void        RECOFillEventHistogram(int i, int j, double weight);
  void        RECOFillHistogram(TAnaCand* muon, int i, int j, double weight);


  void        AnalyzeAODEvent(TAnaCand* muon, double weight);

  void        CorrectTrackJets(int trackindex);
  void        StoreOriginalPlabTrackJets();
  void        ResetTrackJets();

  int         GetMuonOrigin(TGenCand* muon);
  int         GetEventOrigin();
  int         RECOGetClosestHLTL3(int muon_index);
  int         GetGlobalMuonIndex(int track_index);
  int         IsTriggeredGlobalMuonTrack(int track_index);
  void        FakeRateFillHistogram(TAnaTrack* track, int fakemuonindex, int i, int j, double weight);
 
  void        NewIndicesForRecTracks();
  int         GetRecTrackIndex(int newindex);

  int         RECOGetJetToWhichMuonBelongs(TAnaCand* cand);

  double      GetSumPt(TAnaCand* muon, double radius);

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
  TTree *fTree;
  TF1 *fa; TF1 *fb; TF1 *fc;
  int ra, rb, rc;

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
  int fNgenmuons, fNrecomuons;
  int fNL1muons, fNHLTL2muons, fNHLTL3muons, fNCands;
  int ihighGENmuon0, ihighGENmuon1;
  int ihighRECOmuon0, ihighRECOmuon1; 



  bool first;
  
  TRandom3 *Random;
 
 


};



#endif
