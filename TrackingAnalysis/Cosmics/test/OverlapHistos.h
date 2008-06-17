//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Oct 22 18:16:22 2007 by ROOT version 5.14/00b
// from TTree Overlaps/Overlaps
// found on file: MyAnalyzer.root
//////////////////////////////////////////////////////////

#ifndef OverlapHistos_h
#define OverlapHistos_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2F.h>

#include <map>
#include <vector>

// #define HISTOS2D

class OverlapHistos {
private:
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leave types
   UShort_t        found;
   UShort_t        lost;
   UShort_t        matched;
   Float_t         chi2;
   Float_t         path;
   UInt_t          detids[2];
   Float_t         gX[2];
   Float_t         gY[2];
   Float_t         gZ[2];
   Float_t         predQP[2];
   Float_t         predDX[2];
   Float_t         predDY[2];
   Float_t         predX[2];
   Float_t         predY[2];
   Float_t         predEQP[2];
   Float_t         predEDX[2];
   Float_t         predEDY[2];
   Float_t         predEX[2];
   Float_t         predEY[2];
   Float_t         predEDeltaX;
   Char_t          relSignX;
   Float_t         hitX[2];
   Float_t         hitEX[2];
   Float_t         simX[2];

   // List of branches
   TBranch        *b_hitCounts;   //!
   TBranch        *b_chi2;   //!
   TBranch        *b_path;   //!
   TBranch        *b_id;   //!
   TBranch        *b_predPos;   //!
   TBranch        *b_predPar;   //!
   TBranch        *b_predErr;   //!
   TBranch        *b_sigDeltaX;   //!
   TBranch        *b_relSignX;   //!
   TBranch        *b_hitX;   //!
   TBranch        *b_hitEX;   //!
   TBranch        *b_simX;   //!
  //
  typedef std::pair<unsigned int, unsigned int> DetIdPair;
  //
  // id pairs and histograms after "cleaning"
  //
  std::vector<DetIdPair> detIdPairs_;
  std::vector<TH1*> residualHistos_[2];
  std::vector<TH1*> residualHistosPlusX_[2];
  std::vector<TH1*> residualHistosMinusX_[2];
  std::vector<TH1*> residualHistosPlusY_[2];
  std::vector<TH1*> residualHistosMinusY_[2];
  std::vector<TH1*> predErrHistos_[2];
  std::vector<TH1*> predErrHistosFirst_[2];
  std::vector<TH1*> predErrHistosSecond_[2];
  std::vector<TH1*> hitErrHistos_[2];
  std::vector<TH1*> hitErrHistosFirst_[2];
  std::vector<TH1*> hitErrHistosSecond_[2];
  std::vector<TH1*> simRecHistos_[2];
  std::vector<TH1*> simTrkHistos_[2];
  std::vector<TH1*> dxdzHistos_[2];
  std::vector<TH1*> radHistos_[2];
  std::vector<TH1*> phiHistos_[2];
  std::vector<TH1*> zHistos_[2];
  std::vector<TH1*> xHistos_[2];
  std::vector<TH1*> yHistos_[2];
  std::vector<TH1*> statHistos_[2];
  std::vector<TH1*> doublePullHistos_[2];
  std::vector<TH1*> hitPullHistos_[2];
  std::vector<TH1*> predPullHistos_[2];
#ifdef HISTOS2D
  std::vector<TH2*> resVsAngleHistos_[2];
  std::vector<TH2*> ddVsLocalXHistos_[2];
  std::vector<TH2*> ddVsLocalYHistos_[2];
  std::vector<TH2*> ddVsDxdzHistos_[2];
  std::vector<TH2*> ddVsDydzHistos_[2];
  std::vector<TH2*> localXVsLocalYHistos_[2];
  std::vector<TH2*> dxdzVsDydzHistos_[2];
  std::vector<TH2*> localXVsDxdzHistos_[2];
  std::vector<TH2*> localYVsDxdzHistos_[2];
  std::vector<TH2*> localXVsDydzHistos_[2];
  std::vector<TH2*> localYVsDydzHistos_[2];
  std::vector<TH2*> dPreddSimVsdHitdSimHistos_[2];
#endif

public:
   OverlapHistos(TTree *tree=0);
   virtual ~OverlapHistos();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
  /// total number of module pairs
  unsigned int nrOfPairs () const {return detIdPairs_.size();}
  /// subdet & layer info for one index
  void showPair (unsigned int i) const;
  /** access to the histogram for the double-difference 
      (in the order / reverse order of the id pair) */
  TH1* residualHistogram (unsigned int i, bool first = true) const;
  /// decoding of a (barrel strip) DetId
  void decode (unsigned int id, unsigned int& subdet, 
	       unsigned int& layer, unsigned int& stereo)  const;
  /// fill histogram bin with mean and rms
  void fillMean (int ibin, TH1* resultHisto, TH1* inputHisto,
		 float scale = 1.) const;
  /// fill histogram bin with mean and rms and systematic
  void fillMeanWithSyst (int ibin, TH1* resultHisto, TH1* inputHisto,
		 TH1* inputSystHisto, float scale = 1.) const;
  void fillSlope(int ibin, TH1* resultHisto, TH1* resultHisto,
                 TH2* inputHisto) const;
  void fillWidth (int ibin, TH1* resultHisto, TH1* inputHisto,
		 float scale = 1.) const;
};

#endif

#ifdef OverlapHistos_cxx
OverlapHistos::OverlapHistos(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("MyAnalyzer.root");
      if (!f) {
	f = new TFile("/uscmst1b_scratch/lpc1/lpctrk/kaulmer/CMSSW_1_7_7/src/crab_CTF_MCposC_newMC_TIF_Pass4_fix.root");
	//f = new TFile("/uscmst1b_scratch/lpc1/lpctrk/kaulmer/CMSSW_1_7_5/src/MCposA_pass4_withSimHit.root");
      }
      tree = (TTree*)gDirectory->Get("Overlaps");

   }
   Init(tree);
}

OverlapHistos::~OverlapHistos()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t OverlapHistos::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t OverlapHistos::LoadTree(Long64_t entry)
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

void OverlapHistos::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normaly not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("hitCounts", &found, &b_hitCounts);
   fChain->SetBranchAddress("chi2", &chi2, &b_chi2);
   fChain->SetBranchAddress("path", &path, &b_path);
   fChain->SetBranchAddress("detids", detids, &b_id);
   fChain->SetBranchAddress("predPos", gX, &b_predPos);
   fChain->SetBranchAddress("predPar", predQP, &b_predPar);
   fChain->SetBranchAddress("predErr", predEQP, &b_predErr);
   fChain->SetBranchAddress("predEDeltaX", &predEDeltaX, &b_sigDeltaX);
   fChain->SetBranchAddress("relSignX", &relSignX, &b_relSignX);
   fChain->SetBranchAddress("hitX", hitX, &b_hitX);
   fChain->SetBranchAddress("hitEX", hitEX, &b_hitEX);
   fChain->SetBranchAddress("simX", simX, &b_simX);
   Notify();
}

Bool_t OverlapHistos::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normaly not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void OverlapHistos::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t OverlapHistos::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef OverlapHistos_cxx
