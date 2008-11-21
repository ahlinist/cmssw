//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Nov 19 17:52:47 2008 by ROOT version 5.18/00a
// from TTree TreeS/Reduced tree for photon studies: S
// found on file: output_PhotonJets_eff/output_PhotonJets_120_170.root
//////////////////////////////////////////////////////////

#ifndef plots_h
#define plots_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

class plots {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           isgamma;
   Float_t         pt;
   Float_t         ptiso;
   Int_t           ntrkiso;
   Float_t         ptisoatecal;
   Float_t         hcalovecal;
   Float_t         sMajMaj;
   Float_t         sMinMin;
   Float_t         ecaliso;
   Float_t         weight;

   // List of branches
   TBranch        *b_isgammas;   //!
   TBranch        *b_pts;   //!
   TBranch        *b_ptisos;   //!
   TBranch        *b_ntrkisos;   //!
   TBranch        *b_ptisoatecals;   //!
   TBranch        *b_hcalovecal0s;   //!
   TBranch        *b_sMajMajs;   //!
   TBranch        *b_sMinMins;   //!
   TBranch        *b_ecalisos;   //!
   TBranch        *b_weights;   //!

   plots(TTree *tree=0);
   virtual ~plots();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef plots_cxx
plots::plots(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("output_PhotonJets_eff/output_PhotonJets_120_170.root");
      if (!f) {
         f = new TFile("output_PhotonJets_eff/output_PhotonJets_120_170.root");
      }
      tree = (TTree*)gDirectory->Get("TreeS");

   }
   Init(tree);
}

plots::~plots()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t plots::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t plots::LoadTree(Long64_t entry)
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

void plots::Init(TTree *tree)
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

   fChain->SetBranchAddress("isgamma", &isgamma, &b_isgammas);
   fChain->SetBranchAddress("pt", &pt, &b_pts);
   fChain->SetBranchAddress("ptiso", &ptiso, &b_ptisos);
   fChain->SetBranchAddress("ntrkiso", &ntrkiso, &b_ntrkisos);
   fChain->SetBranchAddress("ptisoatecal", &ptisoatecal, &b_ptisoatecals);
   fChain->SetBranchAddress("hcalovecal", &hcalovecal, &b_hcalovecal0s);
   fChain->SetBranchAddress("sMajMaj", &sMajMaj, &b_sMajMajs);
   fChain->SetBranchAddress("sMinMin", &sMinMin, &b_sMinMins);
   fChain->SetBranchAddress("ecaliso", &ecaliso, &b_ecalisos);
   fChain->SetBranchAddress("weight", &weight, &b_weights);
   Notify();
}

Bool_t plots::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void plots::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t plots::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef plots_cxx
