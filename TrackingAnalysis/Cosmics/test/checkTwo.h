#ifndef checkTwo_h
#define checkTwo_h

#include "overlapCheck.h"
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TLegend.h>

#include <iostream>
#include <algorithm>
#include <map>

class checkTwo {
public :
  typedef std::pair<unsigned int, unsigned int> DetIdPair;
  typedef std::pair<unsigned int, int> IntPair;
  typedef std::map<DetIdPair, IntPair> IImap;
  checkTwo(TString fileN1, TString fileN2);

  void defineLegend(TString l0, TString l1)
    {label0 = l0; label1 = l1;}

  void compareRaw(int pair);
  void compareDD();

  void comparePlots(int which, bool wErr = true);
  void compare(TH1F *res[2], TH1* sum[2]);
  overlapCheck *oC1, *oC0;
  void putLegend(TH1 *h0, TH1* h1);
  void addLayer (TH1* h);

private:
  TFile *file0, *file1;
  IImap idMap;
  TString label0, label1;


  TH1* predErrMeans;
  TH1* predErrMeansFirst;
  TH1* predErrMeansSecond;
  TH1* simRec;
  TH1* simTrk;
  TH1* meanDiffs;
  TH1* meanDiffsPlus;
  TH1* meanDiffsMinus;
  TH1* sigmaDiffs;
  TH1* hitErrMeans;
  TH1* dxdzMeans;
  TH1* layerHisto;
  TH1* stereoHisto;
  TH1* radHisto;
  TH1* phiHisto;
  TH1* zHisto;
  TH1* xHisto;
  TH1* yHisto;
  TH1* statHisto;
  TH1* doublePulls;
  TH1* hitPulls;
  TH1* predPulls;

#ifdef HISTOS2D
  TH1* ddVsLocalYSlope;
  TH1* ddVsLocalYOffset;
  TH1* ddVsDxdzSlope;
  TH1* ddVsDxdzOffset;
#endif
};

#endif
