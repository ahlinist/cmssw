#ifndef MYREADER01_H
#define MYREADER01_H

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

#include "../rootio/TAna01Event.hh"
#include "../rootio/TGenCand.hh"
#include "../rootio/TAnaCand.hh"
#include "../rootio/TAnaTrack.hh"
#include "../rootio/TAnaJet.hh"
#include "../rootio/TAnaVertex.hh"

#include "treeReader01.hh"


#define DR      57.29577951
#define PIPMASS 0.13957
#define ELMASS  0.000511
#define MUMASS  0.10567


class myReader01: public treeReader01 {
public:
  myReader01(TChain *tree, TString evtClassName);
  ~myReader01();

  void        bookHist();
  void        startAnalysis();
  void        eventProcessing();
  void        fillHist();
  void        readCuts(TString filename, int dump = 1);
  int         pq, tq;
  float	      pp, pf, pe, pm, pP, tp, tP, tf, te, dR; 
  float	      up, uf, ue, um;
  int  	      fRes;

};

#endif
