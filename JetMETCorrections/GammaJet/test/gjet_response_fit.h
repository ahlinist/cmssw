//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Mar 20 16:52:30 2008 by ROOT version 5.14/00f
// from TTree AnaTree/Reduced tree for final analysis
// found on file: /u1/delre/trees/120_170.root
//////////////////////////////////////////////////////////

#ifndef gjet_response_fit_h
#define gjet_response_fit_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TProfile.h>
#include <TTree.h>

class gjet_response_fit {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leave types
   Int_t           isphoton;
   Int_t           issignal;
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
   Float_t         ptphottrue;
   Float_t         ptjettrue;
   Float_t         ptquarktrue;
   Float_t         phiphottrue;
   Float_t         phijettrue;
   Float_t         phiquarktrue;

   int XBINS, YBINS;
   double cutnn;
   double cut2jet;
   double cutetajet;

   // List of branches
   TBranch        *b_isphoton;   //!
   TBranch        *b_issignal;   //!
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
   TBranch        *b_ptphottrue;   //!
   TBranch        *b_ptjettrue;   //!
   TBranch        *b_ptquarktrue;   //!
   TBranch        *b_phiphottrue;   //!
   TBranch        *b_phijettrue;   //!
   TBranch        *b_phiquarktrue;   //!

   TH1D* ptphot_nocut;
   TH1D* ptphot_allcut;
   TH1D* response_nocut;
   TH1D* response_allcut;
   TH2D* responsevspt_nocut;
   TH2D* responsevspt_allcut;
   TH1D* response_allcut_region1;
   TH1D* response_allcut_region2;
   TH1D* response_allcut_region3;
   TH1D* response_allcut_region4;
   TH1D* ptphot_nocut_sig;
   TH1D* ptphot_allcut_sig;
   TH1D* response_nocut_sig;
   TH1D* response_allcut_sig;
   TH2D* responsevspt_nocut_sig;
   TH2D* responsevspt_allcut_sig;
   TH1D* response_nocut_sig_region1;
   TH1D* response_nocut_sig_region2;
   TH1D* response_nocut_sig_region3;
   TH1D* response_nocut_sig_region4;
   TH1D* response_allcut_sig_region1;
   TH1D* response_allcut_sig_region2;
   TH1D* response_allcut_sig_region3;
   TH1D* response_allcut_sig_region4;
   TH1D* ptphot_nocut_bkg;
   TH1D* ptphot_allcut_bkg;
   TH1D* response_nocut_bkg;
   TH1D* response_allcut_bkg;
   TH2D* responsevspt_nocut_bkg;
   TH2D* responsevspt_allcut_bkg;
   TH1D* response_allcut_bkg_region1;
   TH1D* response_allcut_bkg_region2;
   TH1D* response_allcut_bkg_region3;
   TH1D* response_allcut_bkg_region4;

   TH2D* sob_vs_eff_1;
   TH2D* sob_vs_eff_2;
   TH2D* sob_vs_eff_3;
   TH2D* sob_vs_eff_4;

   TH1D* nn_region1_sig;
   TH1D* nn_region2_sig;
   TH1D* nn_region3_sig;
   TH1D* nn_region4_sig;

   TH1D* nn_region1_bkg;
   TH1D* nn_region2_bkg;
   TH1D* nn_region3_bkg;
   TH1D* nn_region4_bkg;

   TFile* hOutputFile ;
   
   gjet_response_fit(TTree *tree=0, char * outputname = "test.root", int xbin = 100, int ybin = 100);
   virtual ~gjet_response_fit();
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop(bool usew);
   virtual void     BookHistos();
   virtual Bool_t   Notify();
   virtual void     Fit();
   virtual void     ResetHistos();  
   virtual void     Optimum();  
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef gjet_response_fit_cxx
gjet_response_fit::gjet_response_fit(TTree *tree, char * outputname, int xbin, int ybin)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/u1/delre/trees/120_170.root");
      if (!f) {
         f = new TFile("/u1/delre/trees/120_170.root");
      }
      tree = (TTree*)gDirectory->Get("AnaTree");

   }
   XBINS = xbin;
   YBINS = ybin;
   Init(tree);
   hOutputFile   = new TFile(outputname , "RECREATE" ) ;
}

gjet_response_fit::~gjet_response_fit()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
   hOutputFile->Write() ;
   hOutputFile->Close() ;
   hOutputFile->Delete();
}

Int_t gjet_response_fit::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t gjet_response_fit::LoadTree(Long64_t entry)
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

void gjet_response_fit::Init(TTree *tree)
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

   fChain->SetBranchAddress("isphoton", &isphoton, &b_isphoton);
   fChain->SetBranchAddress("issignal", &issignal, &b_issignal);
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
   fChain->SetBranchAddress("ptphottrue", &ptphottrue, &b_ptphottrue);
   fChain->SetBranchAddress("ptjettrue", &ptjettrue, &b_ptjettrue);
   fChain->SetBranchAddress("ptquarktrue", &ptquarktrue, &b_ptquarktrue);
   fChain->SetBranchAddress("phiphottrue", &phiphottrue, &b_phiphottrue);
   fChain->SetBranchAddress("phijettrue", &phijettrue, &b_phijettrue);
   fChain->SetBranchAddress("phiquarktrue", &phiquarktrue, &b_phiquarktrue);

   cutnn = 0.94;
   cut2jet = 0.2;
   cutetajet = 1.2;
     
   Notify();
}

Bool_t gjet_response_fit::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normaly not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void gjet_response_fit::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
#endif // #ifdef gjet_response_fit_cxx
