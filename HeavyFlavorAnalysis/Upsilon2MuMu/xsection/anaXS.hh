#ifndef ANAXS
#define ANAXS

#include "TLatex.h"
#include "TLegend.h"
#include "TLegendEntry.h"
#include "TLine.h"
#include "TArrow.h"
#include "TBox.h"


#include "TString.h"
#include "TObject.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TVirtualPad.h"  // access to gPad

#include "../../../AnalysisDataFormats/HeavyFlavorObjects/rootio/PidTable.hh"

#include <iostream>
#include <fstream>
#include <vector>

class anaXS: public TObject {

public:

  anaXS(const char *dir = "/data/ursl/muonID-rootfiles", int i = 1);

  // -- initialization and setup
  // ---------------------------
  void init(const char *dir, int i = 0);
  void loadFiles(const char *dir, int i = 0);

  void combineUpsilons();

  // -- main methods
  // --------------
  void makeAll(int channels = 3);
  void makeAllMC(int channels = 3);
  void makeAllDATA(int channels = 3);
  void readHistograms(TFile *f, 
		      const char *s1 = "mm", const char *s2 = "mt", const char *s3 = "mmbar", 
		      const char *sm = "Matched", const char *binning = "mt,pt-eta");
  void ReadHistograms(TFile *f, 
		      const char *s1 = "UpsilonMass", 
		      const char *s2 = "AnaEff_1S", const char *s3 = "AnaEff_2S", const char *s4 = "AnaEff_3S",
		      const char *s5 = "MuIDEff_1S", const char *s6 = "MuIDEff_2S", const char *s7 = "MuIDEff_3S",
		      const char *s8 = "TrigEff_1S", const char *s9 = "TrigEff_2S", const char *s10 = "TrigEff_3S",
		      const char *binning = "mt,pt-eta");
  
  void ReadHistogramsDATA0(TFile *f,  const char *s2 = "AnaEff_1S", const char *binning = "mt,pt-eta");
  
  void ReadHistogramsDATA1(TFile *f, const char *s1 = "UpsilonMass",  const char *s5 = "MuIDEff_1S", 
		      const char *s8 = "TrigEff_1S", const char *binning = "mt,pt-eta");
  
  
  
  void readPidTables(const char *sample = "jpsi");

  void addBackground(std::vector<TH1D> &vec, double s2b = 2., double p0 = 1., double p1 = 0.);
  void fitJpsi(int mode);
  void fitUpsilon(int mode);
  void FITUpsilon();
  void McpYields(); 
  void fillPidTables();
  void CorrectedYields(int mode); // 1 - MC, 2 - DATA
  void plotAcceptance();
  void GetAnaEff();
  void GetMuIDEff();
  void GetTrigEff();
  void GetPreSelEff();
  
  void integerEntries(TH1D  *h);
  void validation();
  void projections();
  void PlotProjections(int mode); // 1 - MC, 2 - DATA
  void allDifferences(int jpsiOnly = 0); 
  void ptDifference(const char* a, const char* b, double MIN, double MAX, const char *fname = "ptDifference.pdf"); 
  
  void HistoFlip2D(TH2D *H);
  void totalMass();
  void biasPlots(const char* fname = "/data/ursl/muonID-rootfiles/biased/upsilon/ups_biasstudy.root", 
		 const char *psname = "upsilon-bias.png", int mode = 15); 

  // -- Utilities and helper methods
  // -------------------------------
  void setFunctionParameters(TH1D *h, TF1 *f, int mode); 
  bool getBinCenters(std::string hname, double &eta, double &pT, int &Q);
  bool GetBinCenters(std::string hname, double &eta, double &pT);

  int  wait();
  void makeCanvas(int i = 3);

  // -- Files for Signal, Data, Monte Carlo, and Control Samples
  TFile *fD[10], *fM[20];

  int fShow; 
  TString fFile; 

  // -- lumi values
  double lD[10], lM[20];

  // -- Histograms
  TH2D *fHbinning;

  // -- vectors containing the fitted histograms: S1 =UpsilonMass, S2 =AnaEff_1S, S3 = AnaEff_2S, S4 = AnaEff_3S, 
  // S5 =MuIDEff_1S, S6 = MuIDEff_2S, S7 = MuIDEff_3S, S8 =TrigEff_1S, S9 = TrigEff_2S, S10 = TrigEff_3S
  std::vector<TH1D> fS1Vector, fS2Vector, fS3Vector, fS4Vector, fS5Vector, fS6Vector, fS7Vector, fS8Vector, fS9Vector, fS10Vector;
  
  //-- 2d histograms for the (fitted) event yields
  TH2D *fS1Yield, *fAllGenRes, *fRecoGenRes,  *fPreSelBefore, *fPreSelAfter ,*fAnaEff, *fS1YieldCorrected, *fS1YieldComparison , *fAcceptance, *fMuIDEff, *fTrigEff, *fPreSelEff;
  
  //-- 1d histograms for the (fitted) event yields
  TH1D *fAcceptanceProjPt, *fS1YieldPt, *fAllGenResPt, *fS1YieldEta, *fAllGenResEta; 
  
  // -- vectors containing the fitted histograms: S1 =mm , S2 = mt, S3 = mmbar
  std::vector<TH1D> fS1VectorPos,    fS1VectorNeg,    fS2VectorPos,    fS2VectorNeg,    fS3VectorPos,    fS3VectorNeg; 
  std::vector<TH1D> fS1VectorMcpPos, fS1VectorMcpNeg, fS2VectorMcpPos, fS2VectorMcpNeg, fS3VectorMcpPos, fS3VectorMcpNeg; 

  //-- 2d histograms for the (fitted) event yields
  TH2D *fS1YieldPos, *fS1YieldNeg, *fS2YieldPos, *fS2YieldNeg, *fS3YieldPos, *fS3YieldNeg;
  TH2D *fS1MctPos,   *fS1MctNeg,   *fS2MctPos,   *fS2MctNeg; // MC truth
  TH2D *fS1McpPos,   *fS1McpNeg,   *fS2McpPos,   *fS2McpNeg; // MC probe

  // -- total mass histograms
  TH1D *fS1TotalPos, *fS1TotalNeg,  *fS2TotalPos, *fS2TotalNeg; 

  PidTable *fPtTnpNeg, *fPtTnpPos, 
    *fPtMctNeg, *fPtMctPos, 
    *fPtMcpNeg, *fPtMcpPos,
    *fPtMmbNeg, *fPtMmbPos, *fPtTrigCorr, *fPtMuidCorr;

  // -- functions
  TF1 *f0, *f1, *f2, *f3, *f4, *f5, *f6, *f7, *f8, *f9, *f10, *f11, *f12, *f13; 

  int fMode; 

  // -- Display utilities
  int fFont; 
  TCanvas *c0, *c1, *c2, *c3, *c4, *c5;
  TLatex *tl; 
  TBox *box;
  TArrow *pa; 
  TLine *pl; 
  TLegend *legg;
  TLegendEntry *legge;

  char line[200];
  int fFixFit; 

  std::string fDirectory;
  std::string fPtDirectory;
  std::string fNumbersFileName;
  std::string fSample; 
  char fSuffix[100]; 

  ClassDef(anaXS,1) //Testing anaXS
};


#endif

