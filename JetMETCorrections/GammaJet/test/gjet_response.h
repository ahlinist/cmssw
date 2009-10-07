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

// Bias corrections are defined in L3Corr.C
#include "L3Corr.hpp"
//#include "deltac.C"

// Link this later from gjettree.h
struct photoncandidate {
  double e;
  double pt;
  double eta;
  double phi;
};

// Container for PhotonID parameters
// Link this later from gjettree.h
struct photonidcuts {
  int tracknb;
  float trackiso_rel;
  float trackiso_abs;
  float trackiso_log;
  float trackiso5;
  float trackiso7;
  float hcaliso_rel;
  float hcaliso_abs;
  float hcaliso_log;
  float ecaliso_rel;
  float ecaliso_abs;
  float ecaliso_log;
  float sminmin;
  float smajmaj;
  float hovere;
  float setaeta;
  float emf_rel;
  float emf_abs;
};

// Histograms for making factorized efficiency/rejection plots
// Link this later from gjettree.h
class gjetefficiency_histos {

 public:
  int failcounter;
  TH2D *idvars;

  TH2D *id_beforetopo;
  TH2D *idsim;
  TH2D *id_aftertopo;
  TH2D *topo_beforeid;
  TH2D *toposim;
  TH2D *topo_afterid;

  TH2D *id_beforetopo_vsptgen;
  TH2D *idsim_vsptgen;
  TH2D *id_aftertopo_vsptgen;
  TH2D *topo_beforeid_vsptgen;
  TH2D *toposim_vsptgen;
  TH2D *topo_afterid_vsptgen;

  TH2D *id_beforetopo_vspthat;
  TH2D *idsim_vspthat;
  TH2D *id_aftertopo_vspthat;
  TH2D *topo_beforeid_vspthat;
  TH2D *toposim_vspthat;
  TH2D *topo_afterid_vspthat;

  /*
  TH2D *ecid_beforetopo;
  TH2D *ecid_aftertopo;
  TH2D *ectopo_beforeid;
  TH2D *ectopo_afterid;
  */

  gjetefficiency_histos(bool wide = false);
  ~gjetefficiency_histos();
  void Write();
};

class gjet_response {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain



   // Declaration of leave types
   Int_t           photonid;
   Int_t           isgamma;
   Int_t           ismatched;
   Float_t         weight;
   Float_t         pthat;
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
   Float_t         ptphotgen;
   Float_t         ptjettrue;
   Float_t         ptquarktrue;
   Float_t         phiphottrue;
   Float_t         phijettrue;
   Float_t         phiquarktrue;
   Float_t         etaphottrue;
   Float_t         etajettrue;
   Float_t         etaquarktrue;
   Float_t         mpf;
   Float_t         mpftrue;

   // Photon ID tree
   Float_t         pt;
   Int_t           ntrkiso;
   Float_t         ptiso;
   Float_t         ptiso5;
   Float_t         ptiso7;
   Float_t         ecaliso;
   Float_t         hcaliso;
   Float_t         sMajMaj;
   Float_t         sMinMin;
   Float_t         hovere;
   Float_t         setaeta;
   Float_t         r9;

   int XBINS, YBINS;
   double cut2jet;
   double cutetajet;

   // List of branches
   TBranch        *b_photonid;   //!
   TBranch        *b_isgamma;   //!
   TBranch        *b_ismatched;   //!
   TBranch        *b_weight;   //!
   TBranch        *b_pthat;   //!
   TBranch        *b_ptph;   //!
   TBranch        *b_ptj;   //!
   TBranch        *b_etaph;   //!
   TBranch        *b_etaj;   //!
   TBranch        *b_phiph;   //!
   TBranch        *b_phij;   //!
   TBranch        *b_emfj;   //! new
   TBranch        *b_pflavor;   //! new
   TBranch        *b_flavor;   //! new
   TBranch        *b_pt2jet;   //!
   TBranch        *b_pt2sum;   //!
   TBranch        *b_pt2vecsum;   //!
   TBranch        *b_pt2phot;   //!
   TBranch        *b_ptphottrue;   //!
   TBranch        *b_ptphotgen;   //! new
   TBranch        *b_ptjettrue;   //!
   TBranch        *b_ptquarktrue;   //!
   TBranch        *b_phiphottrue;   //!
   TBranch        *b_phijettrue;   //!
   TBranch        *b_phiquarktrue;   //!
   TBranch        *b_etaphottrue;   //!
   TBranch        *b_etajettrue;   //!
   TBranch        *b_etaquarktrue;   //!
   TBranch        *b_mpf;   //!
   TBranch        *b_mpftrue;   //!

   // Photon ID tree
   TBranch        *b_pt;
   TBranch        *b_ntrkiso;
   TBranch        *b_ptiso;
   TBranch        *b_ptiso5;
   TBranch        *b_ptiso7;
   TBranch        *b_ecaliso;
   TBranch        *b_hcaliso;
   TBranch        *b_jetiso;
   TBranch        *b_sMajMaj;
   TBranch        *b_sMinMin;
   TBranch        *b_hovere;
   TBranch        *b_setaeta;
   TBranch        *b_r9;

   TProfile *ptmean_mix;
   TProfile *ptmean_sig;
   TProfile *ptmean_bkg;
   TProfile *ptjet_mix;
   TProfile *ptjet_sig;
   TProfile *ptjet_bkg;
   TProfile *ptgen_mix;
   TProfile *ptgen_sig;
   TProfile *ptgen_bkg;
   TH1D *purity;
   TH1D *sovb;
   TH1D *efficiency;
   TH1D *effid1;
   TH1D *effid2;
   TH1D *rejection;

   TProfile *ptmean_mix_w;
   TProfile *ptmean_sig_w;
   TProfile *ptmean_bkg_w;
   TH1D *purity_w;
   TH1D *sovb_w;
   TH1D *efficiency_w;
   TH1D *rejection_w;


   // Narrow bins
   TH1D* ptphot_nocut_mix;
   TH1D* ptphot_mix;
   TH1D* response_nocut_mix;
   TH1D* response_mix;
   TH2D* responsevspt_nocut_mix;
   TH2D* responsevspt_mix;
   TH2D* responsevsptgen_mix;
   // Wide bins
   TH1D* ptphot_nocut_mix_w;
   TH1D* ptphot_mix_w;
   TH2D* responsevspt_nocut_mix_w;
   TH2D* responsevspt_mix_w;
   /*
   TH1D* response_mix_reg1;
   TH1D* response_mix_reg2;
   TH1D* response_mix_reg3;
   TH1D* response_mix_reg4;
   */
   // Narrow bins
   TH1D* ptphot_nocut_sig;
   TH1D* ptphot_sig;
   TH1D* response_nocut_sig;
   TH1D* response_sig;
   TH2D* responsevspt_nocut_sig;
   TH2D* responsevspt_sig;
   TH2D* responsevsptgen_sig;
   TProfile* jetmatchvsptgen_sig;
   TProfile* photmatchvsptgen_sig;
   // Wide bins
   TH1D* ptphot_nocut_sig_w;
   TH1D* ptphot_sig_w;
   TH2D* responsevspt_nocut_sig_w;
   TH2D* responsevspt_sig_w;
   /*
   TH1D* response_nocut_sig_reg1;
   TH1D* response_nocut_sig_reg2;
   TH1D* response_nocut_sig_reg3;
   TH1D* response_nocut_sig_reg4;
   TH1D* response_sig_reg1;
   TH1D* response_sig_reg2;
   TH1D* response_sig_reg3;
   TH1D* response_sig_reg4;
   */
   // Narrow bins
   TH1D* ptphot_nocut_bkg;
   TH1D* ptphot_bkg;
   TH1D* response_nocut_bkg;
   TH1D* response_bkg;
   TH2D* responsevspt_nocut_bkg;
   TH2D* responsevspt_bkg;
   TH2D* responsevsptgen_bkg;
   // Wide bins
   TH1D* ptphot_nocut_bkg_w;
   TH1D* ptphot_bkg_w;
   TH2D* responsevspt_nocut_bkg_w;
   TH2D* responsevspt_bkg_w;

   TH1D* ptphot_topocut_sig;
   TH1D* ptphot_topocut_bkg;
   TH1D* ptphot_idcut_sig;
   TH1D* ptphot_idcut_bkg;

   // photon/Z+jet combination studies
   std::vector<int> pt2cuts;
   std::vector<TProfile*> ptphot_pthat;
   std::vector<TProfile*> ptphot_ptpho;
   std::vector<TProfile*> ptresp_pthat;
   std::vector<TProfile*> ptresp_ptpho;
   std::vector<TH2D*> ptresp_pthat_2d;
   std::vector<TH2D*> ptresp_ptpho_2d;
   std::vector<TProfile*> parton_pthat;
   std::vector<TProfile*> parton_ptpho;
   std::vector<TProfile*> pscale_pthat;
   std::vector<TProfile*> pscale_ptpho;
   std::vector<TProfile*> kjet_pthat;
   std::vector<TProfile*> kjet_ptpho;

   TFile* hOutputFile ;
   TDirectory* hFitDir;
   TDirectory* curdir;

   // Bias corrections are defined in L3Corr
   L3Corr *l3corr;
   L3Corr::PhotonID phoID;

   // Photon ID cut containers
   photonidcuts _looseid;
   photonidcuts _mediumid;
   photonidcuts _tightid;

   // Efficiency stuff
   std::map<std::string, gjetefficiency_histos*> effs;
   void FillEffHistos(std::string dirname);
   void WriteHistos();
   bool cutID(photonidcuts const& pid, std::vector<bool> *vpass = 0,
	      std::vector<float> *vpassv = 0, bool sim = false);

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
   //virtual void     Optimum();  
   virtual void     Show(Long64_t entry = -1);

   //  calculate phi1-phi2 keeping value between 0 and pi
   inline float delta_phi(float phi1, float phi2);
   // calculate eta1-eta2 keeping eta2 positive
   inline float delta_eta(float eta1, float eta2);

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
   std::string _mctruthrmsfunc;
   std::string _mcmeanfunc;
   std::string _mcrmsfunc;

   float _lowptreco;
   bool _lowptbias;
   float _fitptmin;

   float _xmin, _xmax;
   //int _rebin;
   std::vector<float> _rebin;

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
   fChain->SetBranchAddress("pthat", &pthat, &b_pthat);
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
   fChain->SetBranchAddress("ptphotgen", &ptphotgen, &b_ptphotgen);
   fChain->SetBranchAddress("ptjettrue", &ptjettrue, &b_ptjettrue);
   fChain->SetBranchAddress("ptquarktrue", &ptquarktrue, &b_ptquarktrue);
   fChain->SetBranchAddress("phiphottrue", &phiphottrue, &b_phiphottrue);
   fChain->SetBranchAddress("phijettrue", &phijettrue, &b_phijettrue);
   fChain->SetBranchAddress("phiquarktrue", &phiquarktrue, &b_phiquarktrue);
   fChain->SetBranchAddress("etaphottrue", &etaphottrue, &b_etaphottrue);
   fChain->SetBranchAddress("etajettrue", &etajettrue, &b_etajettrue);
   fChain->SetBranchAddress("etaquarktrue", &etaquarktrue, &b_etaquarktrue);

   // PhotonID tree, added as a friend
   fChain->SetBranchAddress("pt", &pt, &b_pt);
   fChain->SetBranchAddress("ntrkiso", &ntrkiso, &b_ntrkiso);
   fChain->SetBranchAddress("ptiso", &ptiso, &b_ptiso);
   fChain->SetBranchAddress("ptiso5", &ptiso5, &b_ptiso5);
   fChain->SetBranchAddress("ptiso7", &ptiso7, &b_ptiso7);
   fChain->SetBranchAddress("ecaliso", &ecaliso, &b_ecaliso);
   fChain->SetBranchAddress("hcaliso", &hcaliso, &b_hcaliso);
   fChain->SetBranchAddress("sMajMaj", &sMajMaj, &b_sMajMaj);
   fChain->SetBranchAddress("sMinMin", &sMinMin, &b_sMinMin);
   fChain->SetBranchAddress("hovere", &hovere, &b_hovere);
   fChain->SetBranchAddress("setaeta", &setaeta, &b_setaeta);

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
