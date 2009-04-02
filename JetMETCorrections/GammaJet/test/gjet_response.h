//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Mar 20 16:52:30 2008 by ROOT version 5.14/00f
// from TTree AnaTree/Reduced tree for final analysis
// found on file: /u1/delre/trees/120_170.root
//////////////////////////////////////////////////////////

#ifndef gjet_response_h
#define gjet_response_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TDirectory.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TProfile.h>
#include <TGraphErrors.h>
#include <TTree.h>

#include "Config.hpp"

#include <string>
#include <vector>

#include "deltac.C"

class gjet_response {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain



   // Declaration of leave types
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

   int XBINS, YBINS;
   double cutnn;
   double cut2jet;
   double cutetajet;

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

   TProfile *ptmean_mix;
   TProfile *ptmean_sig;
   TProfile *ptmean_bkg;
   TH1D *purity;
   TH1D *sovb;
   TH1D *efficiency;
   TH1D *effid1;
   TH1D *effid2;
   TH1D *rejection;

   TH1D* ptphot_nocut;
   TH1D* ptphot_mix;
   TH1D* response_nocut;
   TH1D* response_mix;
   TH2D* responsevspt_nocut;
   TH2D* responsevspt_mix;
   TH1D* response_mix_reg1;
   TH1D* response_mix_reg2;
   TH1D* response_mix_reg3;
   TH1D* response_mix_reg4;
   TH1D* ptphot_nocut_sig;
   TH1D* ptphot_sig;
   TH1D* response_nocut_sig;
   TH1D* response_sig;
   TH2D* responsevspt_nocut_sig;
   TH2D* responsevspt_sig;
   TH1D* response_nocut_sig_reg1;
   TH1D* response_nocut_sig_reg2;
   TH1D* response_nocut_sig_reg3;
   TH1D* response_nocut_sig_reg4;
   TH1D* response_sig_reg1;
   TH1D* response_sig_reg2;
   TH1D* response_sig_reg3;
   TH1D* response_sig_reg4;
   TH1D* ptphot_nocut_bkg;
   TH1D* ptphot_bkg;
   TH1D* response_nocut_bkg;
   TH1D* response_bkg;
   TH2D* responsevspt_nocut_bkg;
   TH2D* responsevspt_bkg;
   TH1D* response_bkg_reg1;
   TH1D* response_bkg_reg2;
   TH1D* response_bkg_reg3;
   TH1D* response_bkg_reg4;

   TH1D* ptphot_topocut_sig;
   TH1D* ptphot_topocut_bkg;
   TH1D* ptphot_idcut_sig;
   TH1D* ptphot_idcut_bkg;

   TH2D* sob_vs_eff_1;
   TH2D* sob_vs_eff_2;
   TH2D* sob_vs_eff_3;
   TH2D* sob_vs_eff_4;

   TH1D* nn_reg1_sig;
   TH1D* nn_reg2_sig;
   TH1D* nn_reg3_sig;
   TH1D* nn_reg4_sig;

   TH1D* nn_reg1_bkg;
   TH1D* nn_reg2_bkg;
   TH1D* nn_reg3_bkg;
   TH1D* nn_reg4_bkg;

   TFile* hOutputFile ;
   TDirectory* hFitDir;
   TDirectory* curdir;

   gjet_response(TTree *tree=0, char * outputname = "test.root", int xbin = 100, int ybin = 100, const char* cfg = "final");
   virtual ~gjet_response();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop(const std::string idtype = "medium");
   virtual void     BookHistos();
   virtual Bool_t   Notify();
   virtual void     Fit(bool arithmetic=false);
   virtual void     ResetHistos(); 
   virtual void     Optimum();  
   virtual void     Show(Long64_t entry = -1);

   //  calculate phi1-phi2 keeping value between 0 and pi
   inline float delta_phi(float phi1, float phi2);
   // calculate eta1-eta2 keeping eta2 positive
   inline float delta_eta(float eta1, float eta2);
   // Photon ID cuts on neural network output
   bool NNID(float nnval);

   void Configure(const char *cfg = "final");
   void DumpConfig();

 private:
   std::string _algoname;
   std::string _idtype;

   float _photetacut;
   float _photptcut;
   float _jetetacut;
   float _jet2_minpt;
   float _jet2_maxfrac;
   float _deltaphi;
   float _deltaeta;

   std::string _mctruthfunc;
   std::string _mctruthcsa07;

   float _lowptreco;
   bool _lowptbias;
   float _fitptmin;

   float _xmin, _xmax;
   int _rebin;

};

#endif

#ifdef gjet_response_cxx
gjet_response::gjet_response(TTree *tree, char *outputname, int xbin, int ybin,
			     const char *cfg)
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

   tree->SetCacheSize(0);

   XBINS = xbin;
   YBINS = ybin;
   Init(tree);
   curdir = gDirectory;
   hOutputFile   = new TFile(outputname , "RECREATE" ) ;
   hFitDir = hOutputFile->mkdir("Fits");
   curdir->cd();

   Configure(cfg);
   DumpConfig();
}

gjet_response::~gjet_response()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
   hOutputFile->Write() ;
   hOutputFile->Close() ;
   hOutputFile->Delete();
}

Int_t gjet_response::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t gjet_response::LoadTree(Long64_t entry)
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

void gjet_response::Init(TTree *tree)
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

Bool_t gjet_response::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normaly not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void gjet_response::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t gjet_response::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef gjet_response_cxx
