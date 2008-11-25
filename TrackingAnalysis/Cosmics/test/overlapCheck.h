//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Aug  1 10:43:17 2008 by ROOT version 5.18/00a
// from TTree Overlaps/Overlaps
// found on file: output.root
//////////////////////////////////////////////////////////

#ifndef overlapCheck_h
#define overlapCheck_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TH2.h>
#include <TProfile.h>
#include <TF1.h>

#include <iostream>
#include <algorithm>
#include <vector>
#include "overlapHelper.C"

#define HISTOS2D

class overlapCheck {
public :
  typedef std::pair<unsigned int, unsigned int> DetIdPair;
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   UInt_t          detids[2];

   // List of branches
   TBranch        *b_id;   //!

   overlapCheck(TTree *tree=0);
   overlapCheck(TString fileName);
   virtual ~overlapCheck();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);

  void searchDetId(unsigned int detId);
  /**
   * Get the 2 detIds of an overlap from the tree
   */
  DetIdPair getPair(int pairNbr);
  /**
   * For an overlap, print the summary info and diaply all 
   * the distributions
   */
  void getPairInfo(int pairNbr);

  /**
   * Display all the distributions for an overlap
   */
  void displayPair(int pairNbr, AdjacentType type, TString tag = "");
  void drawFunc(int pairNbr, TH1* resultSlopeHisto, TH1* resultOffsetHisto);
  /**
   * Print the full module info for the pair
   */
  void idPrint(DetIdPair idPair);
  /**
   * For a specific overlap, load all the distributions from the file
   */ 
  void loadHistosPair(int pairNbr);


  /**
   * Load all the summary plots from the file
   */
  void loadSummaryPlots();
  /**
   * Display all the summary plots
   */
  void displaySummary();
  /**
   * Print the summary info for a specific overlap
   */
  void printSummaryPair(int pairNbr, AdjacentType type);

  /**
   * Print all the summary plots to a ps file
   */
  void printSummaryToFile(TString name);


  /**
   * Recreate the summary plots for all pairs from the distributions.
   * These will be written to a new file
   * It can be global, or only for a particular sub-detector, side, or layer
   */
  void recreateSummaryPlots(TString file, unsigned int det = 0, int sign = 0, unsigned int layer = 0);
  /**
   * Redo the computation of all the quantities for a specific pair 
   * from the different distributions
   */
  void computeSummaryPair(int pairNbr);

  void overlapCheck::spotProblems();

  /**
   * Plot the positions of the overlap modules
   */
  void scan();

  void presentation (TH1* h, bool wErr = true);
  void checkAll (TString name);
  void addLayer (TH1* h);
  void sigmas();


  Long64_t entries() { return fChain->GetEntriesFast();}


    TH1* resHistos;
    TH1* resHistosPlusY;
    TH1* resHistosMinusY;
    TH1* resHistosPlusX;
    TH1* resHistosMinusX;
    TH1* predErrHistos;
    TH1* predErrHistosFirst;
    TH1* predErrHistosSecond;
    TH1* hitErrHistos;
    TH1* hitErrHistosFirst;
    TH1* hitErrHistosSecond;
    TH1* simRecHistos;
    TH1* simTrkHistos;
    TH1* dxdzHistos;
    TH1* posHistos[3];
    TH1* posHistosLocal[2];
    TH1* doublePullHistos;
    TH1* hitPullHistos;
    TH1* predPullHistos;
#ifdef HISTOS2D
    TH2* resVsAngleHistos;
    TH2* ddVsLocalXHistos;
    TH2* ddVsLocalYHistos;
    TH2* ddVsDxdzHistos;
    TH2* ddVsDydzHistos;
    TH2* localXVsLocalYHistos;
    TH2* dxdzVsDydzHistos;
    TH2* localXVsDxdzHistos;
    TH2* localYVsDxdzHistos;
    TH2* localXVsDydzHistos;
    TH2* localYVsDydzHistos;
    TH2* dPreddSimVsdHitdSimHistos;
#endif

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
  TH1 *ddVsLocalXSlope, *ddVsLocalXOffset, *ddVsDydzSlope, *ddVsDydzOffset;
  TF1 *f1;
#endif

private:


  TH2F *rzScan, *xyScan;

  TCanvas *c1, *c2, *c3, *c4, *c5, *c6;
  TCanvas *s1, *s2, *s3, *s4, *s5, *s6;
  bool canvasUpdated, statUpdate, summaryOK;
  TDirectory* baseDir;
  bool partialPlots;

};

#endif

#ifdef overlapCheck_cxx



overlapCheck::overlapCheck(TTree *tree)
{
  canvasUpdated = false;statUpdate = false;
  summaryOK = false;

// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
//       TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("output.root");
//       if (!f) {
//          f = new TFile("output.root");
//       }
      tree = (TTree*)gDirectory->Get("Overlaps");

   }
   Init(tree);
  baseDir = gDirectory;

//   TFile *ff = new TFile("summary_mc.root");
// 
  ddVsDxdzSlope=0;


//   ff->Close();
}

overlapCheck::overlapCheck(TString fileName)
{
  canvasUpdated = false;statUpdate = false;
  summaryOK = false;

  TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(fileName);
  if (!f) {
     f = new TFile(fileName);
  }
  TTree *tree = (TTree*)gDirectory->Get("Overlaps");
  Init(tree);
  baseDir = gDirectory;

//   TFile *ff = new TFile("summary_mc.root");
// 
  ddVsDxdzSlope=0;


//   ff->Close();
}


void overlapCheck::loadSummaryPlots()
{
  gFile->cd();
  predErrMeans = (TH1*) gDirectory->Get("sigMean");
  predErrMeansFirst = (TH1*) gDirectory->Get("sigMeanFirst");
  predErrMeansSecond = (TH1*) gDirectory->Get("sigMeanSecond");
  simRec = (TH1*) gDirectory->Get("simRec");
  simTrk = (TH1*) gDirectory->Get("simTrk");
  meanDiffs = (TH1*) gDirectory->Get("mean");
  meanDiffsPlus = (TH1*) gDirectory->Get("meanPlus");
  meanDiffsMinus = (TH1*) gDirectory->Get("meanMinus");
  sigmaDiffs = (TH1*) gDirectory->Get("width");
  hitErrMeans = (TH1*) gDirectory->Get("hitSigX");
  dxdzMeans = (TH1*) gDirectory->Get("dxdz");
  layerHisto = (TH1*) gDirectory->Get("layer");
  stereoHisto = (TH1*) gDirectory->Get("stereo");
  radHisto = (TH1*) gDirectory->Get("radius");
  phiHisto = (TH1*) gDirectory->Get("phi");
  zHisto = (TH1*) gDirectory->Get("z");
  xHisto = (TH1*) gDirectory->Get("x");
  yHisto = (TH1*) gDirectory->Get("y");
  statHisto = (TH1*) gDirectory->Get("stat");
  doublePulls = (TH1*) gDirectory->Get("doublePull");
  hitPulls = (TH1*) gDirectory->Get("hitPull");
  predPulls = (TH1*) gDirectory->Get("predPull");

  hitErrMeans->GetYaxis()->SetTitle("#sigma(#Deltax_{hit}) [#mum]");
  dxdzMeans->GetYaxis()->SetTitle("Local dx/dz");
  layerHisto->GetYaxis()->SetTitle("Layer");
  stereoHisto->GetYaxis()->SetTitle("Stereo");
  radHisto->GetYaxis()->SetTitle("Radius [cm]");
  phiHisto->GetYaxis()->SetTitle("#phi");
  zHisto->GetYaxis()->SetTitle("z [cm]");
  xHisto->GetYaxis()->SetTitle("Local x [cm]");
  yHisto->GetYaxis()->SetTitle("Local y [cm]");
  statHisto->GetYaxis()->SetTitle("Entries");
  doublePulls->GetYaxis()->SetTitle("Pull DD");
  hitPulls->GetYaxis()->SetTitle("Pull (#Deltax_{hit})");
  predPulls->GetYaxis()->SetTitle("Pull (#Deltax_{pred})");

#ifdef HISTOS2D
  ddVsLocalYSlope  = (TH1*) gDirectory->Get("ddYslope");
  ddVsLocalYOffset = (TH1*) gDirectory->Get("ddYoffset");
  ddVsLocalXSlope = (TH1*) gDirectory->Get("ddXslope");
  ddVsLocalXOffset= (TH1*) gDirectory->Get("ddXoffset");

  ddVsDxdzSlope  = (TH1*) gDirectory->Get("ddDxslope");
  ddVsDxdzOffset = (TH1*) gDirectory->Get("ddDxoffset");
  ddVsDydzSlope  = (TH1*) gDirectory->Get("ddDyslope");
  ddVsDydzOffset = (TH1*) gDirectory->Get("ddDyoffset");
  f1 = new TF1("f1","pol1",-10,10);

#endif
 summaryOK = true;
//   ff->Close();
}
overlapCheck::~overlapCheck()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t overlapCheck::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t overlapCheck::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (!fChain->InheritsFrom(TChain::Class()))  return centry;
   TChain *chain = (TChain*)fChain;
   if (chain->GetTreeNumber() != fCurrent) {
      fCurrent = chain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void overlapCheck::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("detids", detids, &b_id);
   Notify();
}

Bool_t overlapCheck::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void overlapCheck::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t overlapCheck::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef overlapCheck_cxx
