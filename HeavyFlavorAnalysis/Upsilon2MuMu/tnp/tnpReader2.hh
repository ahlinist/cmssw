#ifndef TNPREADER2_H
#define TNPREADER2_H

#include <iostream>

#include <TROOT.h>
#include <TString.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TTree.h>

#include "../../../AnalysisDataFormats/HeavyFlavorObjects/rootio/TAna01Event.hh"
#include "../../../AnalysisDataFormats/HeavyFlavorObjects/rootio/TGenCand.hh"
#include "../../../AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaCand.hh"
#include "../../../AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaTrack.hh"
#include "../../../AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaJet.hh"
#include "../../../AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaVertex.hh"
#include "../../../AnalysisDataFormats/HeavyFlavorObjects/rootio/JSON.hh"

#include "treeReaderTNP.hh"


#define DR      57.29577951
#define PIPMASS 0.13957
#define ELMASS  0.000511
#define MUMASS  0.10567


class tnpReader2: public treeReaderTNP {
public:
  tnpReader2(TChain *tree, TString evtClassName);
  ~tnpReader2();

  void        bookHist();
  void        startAnalysis();
  void        eventProcessing();
  void        fillHist(TAnaCand *pCand,  int mode, bool CombineEndCaps);
  bool        truthMatch(TAnaCand *pCand);
  void        readCuts(TString filename, int dump = 1);
  bool        isPathPreScaled(TString path);
  void        TagSelection();
  void        ProbeSelection();
  bool        TrackSelection(TAnaTrack *pTrack, int mode);  // mode = 1 for Tag, mode = 2 for Probe 
  void        MCTruth(int mode);
  void        Info();
  bool        isGoodProbe(TAnaCand *pCand);
  void        freePointers();
  bool        isMatchedToTrig(TAnaCand *pCand, TString Label, int mode);
  bool        isRecTrackMatchedToTrig(TAnaTrack *pTrack, TString Label);
  bool        isPathFired(TString path);
  
  bool        isMatchedToDoubleTrig(TAnaCand *pCand, TString Label, int mode);
  
  int         pq, tq;
  float	      pp, pf, pe, pm, pP, tp, tP, tf, te, dR; 
  float	      up, uf, ue, um;
  float	      posmuoneta, posmuonpt, negmuoneta, negmuonpt;
  int  	      fRes;
  int         Trigger;
    
  // Cut values
  int SAMPLE;
  int TYPE;
  int RESTYPE;
  TString HLTPATH_TAG;
  TString HLTPATH_PROBE;
  TString HLTLABEL_TAG;
  TString HLTLABEL_PROBE;
  int MUTYPE1;
  int MUTYPE2;
  double PT_TAG;
  double ETALO_TAG;
  double ETAHI_TAG;
  int MODE;
  double PT_PROBE;
  double MASSLO;
  double MASSHI;
  double MAXDOCA;
  double DPHI;  
  double DETA;
  int BIN;
  
  double CHI2;
  
    // -- Variables
  vector<TAnaCand*> fCand;
  vector<TAnaCand*> fCandTT;
  vector<TAnaCand*> fCandTT2;
  vector<TAnaCand*> fCandPS;
  vector<TAnaCand*> fCandTP;
  vector<TAnaCand*> fCandnotTP;
  vector<TAnaCand*> fCandGP;
  vector<TAnaCand*> fCandnotGP;  
  
  static const int  fNpt = 4;
  static const int  fNeta = 5;
  static const int  fNq = 1;
  double      fPTbin[fNpt+1], fEtabin[fNeta+1];
  int fQ[fNq+1];
  int fBin;
  double fMassLow, fMassHigh;

  JSON   *fpJSON;
};

#endif
