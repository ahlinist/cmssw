#ifndef TNPREADER_H
#define TNPREADER_H

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

#include "treeReaderTNP.hh"


#define DR      57.29577951
#define PIPMASS 0.13957
#define ELMASS  0.000511
#define MUMASS  0.10567


class tnpReader: public treeReaderTNP {
public:
  tnpReader(TChain *tree, TString evtClassName);
  ~tnpReader();

  void        bookHist();
  void        startAnalysis();
  void        eventProcessing();
  void        fillHist();
  void        readCuts(TString filename, int dump = 1);
  bool        isMatchedToTrig(TAnaTrack *pTag, TString Label);
  bool        isPathFired(TString path);
  int         pq, tq;
  float	      pp, pf, pe, pm, pP, tp, tP, tf, te, dR; 
  float	      up, uf, ue, um;
  float	      posmuoneta, posmuonpt, negmuoneta, negmuonpt;
  int  	      fRes;
  
  // Cut values
  int TYPE;
  int RESTYPE;
  TString HLTPATH;
  TString HLTLABEL;
  
  int MUTYPE1;
  int MUTYPE2;
  double CHI2;
  double PT1;
  double PT2;

  
  
};

#endif
