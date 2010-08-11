#ifndef XSREADER_H
#define XSREADER_H

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

#include "treeReaderXS.hh"


#define DR      57.29577951
#define PIPMASS 0.13957
#define ELMASS  0.000511
#define MUMASS  0.10567


class xsReader: public treeReaderXS {
public:
  xsReader(TChain *tree, TString evtClassName);
  ~xsReader();

  void        bookHist();
  void        startAnalysis();
  void        eventProcessing();
  void        fillHist();
  void        fillCandHist();
  void        MCstudy();
  void        calculateWeights(int mode);
  void        readCuts(TString filename, int dump = 1);
  void        candidateSelection(int mode);
  bool        isMatchedToTrig(TAnaTrack *pTag, TString Label);
  
  // -- Cut values
  int TYPE;
  int MUTYPE1;
  int MUTYPE2;
  double CHI2;
  int RESTYPE;
  double PT1;
  double PT2;
  
  // -- Variables
  TAnaCand    *fpCand; 
  double      fCandPt, fCandMass, fCandY;
  TGenCand    *fgCand;
  double      fGenCandPt, fGenCandY;
  double      fGenMuon1Pt, fGenMuon1Eta, fMuon1Eta, fMuon1Pt;
  double      fGenMuon2Pt, fGenMuon2Eta, fMuon2Eta, fMuon2Pt;
  
  double      fWeight;
  static const int  fNpt = 6;
  static const int  fNy = 4;
  double      fPTbin[fNpt+1], fYbin[fNy+1];
};


#endif
