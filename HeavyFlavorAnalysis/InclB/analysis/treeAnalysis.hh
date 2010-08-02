#ifndef TREEANALYSIS_H
#define TREEANALYSIS_H

#include <iostream>
#include <algorithm>
#include <vector>

#include <TROOT.h>
#include <TString.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
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
const double mpion = 0.13957018;


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

  int         GetProcess();
  bool        IsTtbarevent();
  int         GetEventOrigin();
  int         GetMuonOrigin(TGenCand* muon); 
  double      GetSumPt(TAnaCand* muon, double radius);   

  void        ScaleRECOJets();
  void        CountBQuarksinJet();
  void        CorrectGenJets(); 
  void        CalculateEtGenJets();
  void        CorrectTrackJets();
  int         IsGlobalMuonTrack(int track_index);

  int         NumberOfGENMuons();
  int         NumberOfRECOMuons();
  int         NumberOfRECOTracks();
  int         NumberOfRECOTrackJets();
  int         NumberOfTriggerMuons(int level);
  int         GetClosestRECOJettoGENJet(TAnaJet* genjet, double etmin_jet); 
  int         GetClosestGENJettoRECOJet(TAnaJet* recojet, double etmin_jet);
  int         GetHighestPtGENMuon(double ptmin_muon =0 , double eta_muon_lo =-1, double eta_muon_hi =99999);
  int         GetHighestPtRECOMuon(double ptmin_muon =0 , double eta_muon_lo =-1, double eta_muon_hi =99999);
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
  TLorentzVector    GetTrackJetWithoutMu(TAnaJet* jet);  

  void        GENFillEventHistogram(int i, int j, double weight);
  void        GENFillHistogram(TAnaCand* muon, int i, int j, double weight);
  void        GENFillAllHistogram(TAnaCand* muon, int i, double weight);
  void        RECOFillEventHistogram(int i, int j, double weight);
  void        RECOFillHistogram(TAnaCand* muon, int i, int j, double weight, int aod = 0);
  void        RECOFillAllHistogram(TAnaCand* muon, int i, double weight);
  void        RECOFillHistogramResolution(TAnaCand* genmuon, TAnaCand* muon, int i, int j, double weight);
  void        FillAllHistogram(TAnaCand* muon, int i, double weight);
  void        FillAllEventHistogram(int i, double weight);
  
  void        L1FillHistogram(TAnaTrack* l1muon, int i, int j, double weight);
  void        L1FillHistogramResolution(TAnaCand* genmuon, TAnaTrack* hltmuon, int i, int j, double weight); 
  void        HLTL2FillHistogram(TAnaTrack* hltmuon, int i, int j, double weight);
  void        HLTL2FillHistogramResolution(TAnaCand* genmuon, TAnaTrack* hltmuon, int i, int j, double weight);  
  void        HLTL3FillHistogram(TAnaTrack* hltmuon, int i, int j, double weight);
  void        HLTL3FillHistogramResolution(TAnaCand* genmuon, TAnaTrack* hltmuon, int i, int j, double weight);
  void        TriggerFillAllHistogram(TAnaCand* muon, int i, double weight);

  double      RECOGetTrackJetMaxDeltaR(TAnaJet* jet); 
  double      RECOGetTrackJetMinDeltaR(TAnaJet* jet);

  int         RECOGetJetToWhichMuonBelongs(TAnaCand* cand);
  

  void        AnalyzeGENEvent();
  void        AnalyzeRECOEvent();
  void        AnalyzeRECOEvent2();
  void        AnalyzeTrigger();
  void        AnalyzeAODEvent(int mc=0);

 



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

  int fDebug;
 
  // -- Cut values 
  double PTLOMUON, ETAMUON, ETAMUONLO, ETAMUONHI, PTLOGENMUON, HLTMUON; 
  double CHIMUON, DZMUON, D0MUON, TRKHITMUON;
  double ETLOJET, ETAJET, ETCALOJET, ETTRKJET, ETGENJET, JETNTRK;
  double CONEMUJET;
  double PTHATMIN, PTHATMAX;
  double ALPHA; 
  double L1MATCH, HLTMATCH, JETMATCH;
  double PIXLAY, TRKLAY, DZCUT, VTXNDOF;
   
  // -- Variables 
  int fNgenmuons, fNrecomuons, fNrecotracks, fNrecotrackjets;
  int fNL1muons, fNHLTL2muons, fNHLTL3muons;
  int ihighGENmuon0, ihighGENmuon1;
  int ihighRECOmuon0, ihighRECOmuon1; 



  bool first;
  
  TRandom3 *Random;
 
 


};



#endif
