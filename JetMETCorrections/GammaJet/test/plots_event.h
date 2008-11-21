//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Nov 20 14:55:42 2008 by ROOT version 5.18/00a
// from TTree AnaTree/Reduced tree for final analysis
// found on file: output_QCD_eff/output_QCD_300_380.root
//////////////////////////////////////////////////////////

#ifndef plots_event_h
#define plots_event_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

class plots_event {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           photonid;
   Int_t           isgamma;
   Int_t           ismatched;
   Float_t         weight;
   Float_t         nniso;
   Float_t         nniso_int;
   Float_t         ptphot;
   Float_t         ptjet;
   Float_t         etaphot;
   Float_t         etajet;
   Float_t         phiphot;
   Float_t         phijet;
   Float_t         pt2jet;
   Float_t         pt2sum;
   Float_t         pt2vecsum;
   Float_t         pt2phot;
   Float_t         ptphottrue;
   Float_t         ptjettrue;
   Float_t         ptquarktrue;
   Float_t         phiphottrue;
   Float_t         phijettrue;
   Float_t         phiquarktrue;

   // List of branches
   TBranch        *b_photonid;   //!
   TBranch        *b_isgamma;   //!
   TBranch        *b_ismatched;   //!
   TBranch        *b_weight;   //!
   TBranch        *b_nniso;   //!
   TBranch        *b_nniso_int;   //!
   TBranch        *b_ptph;   //!
   TBranch        *b_ptj;   //!
   TBranch        *b_etaph;   //!
   TBranch        *b_etaj;   //!
   TBranch        *b_phiph;   //!
   TBranch        *b_phij;   //!
   TBranch        *b_pt2jet;   //!
   TBranch        *b_pt2sum;   //!
   TBranch        *b_pt2vecsum;   //!
   TBranch        *b_pt2phot;   //!
   TBranch        *b_ptphottrue;   //!
   TBranch        *b_ptjettrue;   //!
   TBranch        *b_ptquarktrue;   //!
   TBranch        *b_phiphottrue;   //!
   TBranch        *b_phijettrue;   //!
   TBranch        *b_phiquarktrue;   //!

   plots_event(TTree *tree=0);
   virtual ~plots_event();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
   virtual float    delta_phi(float phi1, float phi2);
   virtual float    delta_eta(float eta1, float eta2);
};

#endif
   
#ifdef plots_event_cxx
plots_event::plots_event(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("output_QCD_eff/output_QCD_300_380.root");
      if (!f) {
         f = new TFile("output_QCD_eff/output_QCD_300_380.root");
      }
      tree = (TTree*)gDirectory->Get("AnaTree");

   }
   Init(tree);
}

plots_event::~plots_event()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t plots_event::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t plots_event::LoadTree(Long64_t entry)
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

void plots_event::Init(TTree *tree)
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

   fChain->SetBranchAddress("photonid", &photonid, &b_photonid);
   fChain->SetBranchAddress("isgamma", &isgamma, &b_isgamma);
   fChain->SetBranchAddress("ismatched", &ismatched, &b_ismatched);
   fChain->SetBranchAddress("weight", &weight, &b_weight);
   fChain->SetBranchAddress("nniso", &nniso, &b_nniso);
   fChain->SetBranchAddress("nniso_int", &nniso_int, &b_nniso_int);
   fChain->SetBranchAddress("ptphot", &ptphot, &b_ptph);
   fChain->SetBranchAddress("ptjet", &ptjet, &b_ptj);
   fChain->SetBranchAddress("etaphot", &etaphot, &b_etaph);
   fChain->SetBranchAddress("etajet", &etajet, &b_etaj);
   fChain->SetBranchAddress("phiphot", &phiphot, &b_phiph);
   fChain->SetBranchAddress("phijet", &phijet, &b_phij);
   fChain->SetBranchAddress("pt2jet", &pt2jet, &b_pt2jet);
   fChain->SetBranchAddress("pt2sum", &pt2sum, &b_pt2sum);
   fChain->SetBranchAddress("pt2vecsum", &pt2vecsum, &b_pt2vecsum);
   fChain->SetBranchAddress("pt2phot", &pt2phot, &b_pt2phot);
   fChain->SetBranchAddress("ptphottrue", &ptphottrue, &b_ptphottrue);
   fChain->SetBranchAddress("ptjettrue", &ptjettrue, &b_ptjettrue);
   fChain->SetBranchAddress("ptquarktrue", &ptquarktrue, &b_ptquarktrue);
   fChain->SetBranchAddress("phiphottrue", &phiphottrue, &b_phiphottrue);
   fChain->SetBranchAddress("phijettrue", &phijettrue, &b_phijettrue);
   fChain->SetBranchAddress("phiquarktrue", &phiquarktrue, &b_phiquarktrue);
   Notify();
}

Bool_t plots_event::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void plots_event::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t plots_event::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef plots_event_cxx
