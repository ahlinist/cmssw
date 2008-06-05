//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Dec  7 11:38:21 2007 by ROOT version 5.14/00f
// from TTree pippo/Analysis tree
// found on file: output.root
//////////////////////////////////////////////////////////

#ifndef gjettree_h
#define gjettree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TProfile.h>
#include <TTree.h>

class gjettree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leave types
   Int_t           event;
   Int_t           nMC;
   Int_t           pdgIdMC[1446];   //[nMC]
   Int_t           statusMC[1446];   //[nMC]
   Int_t           motherIDMC[1446];   //[nMC]
   Float_t         pxMC [1446];   //[nMC]
   Float_t         pyMC [1446];   //[nMC]
   Float_t         pzMC [1446];   //[nMC]
   Float_t         eMC  [1446];   //[nMC]
   Float_t         etaMC[1446];   //[nMC]
   Float_t         phiMC[1446];   //[nMC]
   Int_t           nPhot;
   Float_t         pxPhot [25];   //[nPhot]
   Float_t         pyPhot [25];   //[nPhot]
   Float_t         pzPhot [25];   //[nPhot]
   Float_t         ePhot  [25];   //[nPhot]
   Float_t         etaPhot[25];   //[nPhot]
   Float_t         phiPhot[25];   //[nPhot]
   Float_t         ptiso02Phot[25];   //[nPhot]
   Int_t           ntrkiso02Phot[25];   //[nPhot]
   Float_t         ptiso025Phot[25];   //[nPhot]
   Int_t           ntrkiso025Phot[25];   //[nPhot]
   Float_t         ptiso03Phot[25];   //[nPhot]
   Int_t           ntrkiso03Phot[25];   //[nPhot]
   Float_t         ptiso035Phot[25];   //[nPhot]
   Int_t           ntrkiso035Phot[25];   //[nPhot]
   Float_t         ptiso04Phot[25];   //[nPhot]
   Int_t           ntrkiso04Phot[25];   //[nPhot]
   Float_t         ptisoatecal02Phot[15];   //[nPhot]
   Int_t           ntrkisoatecal02Phot[15];   //[nPhot]
   Float_t         ptisoatecal025Phot[15];   //[nPhot]
   Int_t           ntrkisoatecal025Phot[15];   //[nPhot]
   Float_t         ptisoatecal03Phot[15];   //[nPhot]
   Int_t           ntrkisoatecal03Phot[15];   //[nPhot]
   Float_t         ptisoatecal035Phot[15];   //[nPhot]
   Int_t           ntrkisoatecal035Phot[15];   //[nPhot]
   Float_t         ptisoatecal04Phot[15];   //[nPhot]
   Float_t         hcalovecal01Phot[25];   //[nPhot]
   Float_t         hcalovecal02Phot[25];   //[nPhot]
   Float_t         hcalovecal025Phot[25];   //[nPhot]
   Float_t         hcalovecal03Phot[25];   //[nPhot]
   Float_t         hcalovecal04Phot[25];   //[nPhot]
   Float_t         hcalovecal05Phot[25];   //[nPhot]
   Float_t         ecaliso02Phot[25];   //[nPhot]
   Float_t         ecaliso03Phot[25];   //[nPhot]
   Float_t         ecaliso035Phot[25];   //[nPhot]
   Float_t         ecaliso04Phot[25];   //[nPhot]
   Float_t         ecaliso05Phot[25];   //[nPhot]
   Float_t         LATPhot[15];   //[nPhot]
   Float_t         sMajMajPhot[15];   //[nPhot]
   Float_t         sMinMinPhot[15];   //[nPhot]
   Float_t         FisherPhot[15];   //[nPhot]
   Int_t           nJet;
   Float_t         pxJet [100];   //[nJet]
   Float_t         pyJet [100];   //[nJet]
   Float_t         pzJet [100];   //[nJet]
   Float_t         eJet  [100];   //[nJet]
   Float_t         etaJet[100];   //[nJet]
   Float_t         phiJet[100];   //[nJet]
   Int_t           nJet_ite;
   Float_t         pxJet_ite [100];   //[nJet]
   Float_t         pyJet_ite [100];   //[nJet]
   Float_t         pzJet_ite [100];   //[nJet]
   Float_t         eJet_ite  [100];   //[nJet]
   Float_t         etaJet_ite[100];   //[nJet]
   Float_t         phiJet_ite[100];   //[nJet]
   Int_t           nJet_kt;
   Float_t         pxJet_kt [100];   //[nJet]
   Float_t         pyJet_kt [100];   //[nJet]
   Float_t         pzJet_kt [100];   //[nJet]
   Float_t         eJet_kt  [100];   //[nJet]
   Float_t         etaJet_kt[100];   //[nJet]
   Float_t         phiJet_kt[100];   //[nJet]
   Int_t           nJet_sis;
   Float_t         pxJet_sis [100];   //[nJet]
   Float_t         pyJet_sis [100];   //[nJet]
   Float_t         pzJet_sis [100];   //[nJet]
   Float_t         eJet_sis  [100];   //[nJet]
   Float_t         etaJet_sis[100];   //[nJet]
   Float_t         phiJet_sis[100];   //[nJet]
   Int_t           nJetGen;
   Float_t         pxJetGen [100];   //[nJetGen]
   Float_t         pyJetGen [100];   //[nJetGen]
   Float_t         pzJetGen [100];   //[nJetGen]
   Float_t         eJetGen  [100];   //[nJetGen]
   Float_t         etaJetGen[100];   //[nJetGen]
   Float_t         phiJetGen[100];   //[nJetGen]
   Int_t           nJetGen_ite;
   Float_t         pxJetGen_ite [100];   //[nJetGen]
   Float_t         pyJetGen_ite [100];   //[nJetGen]
   Float_t         pzJetGen_ite [100];   //[nJetGen]
   Float_t         eJetGen_ite  [100];   //[nJetGen]
   Float_t         etaJetGen_ite[100];   //[nJetGen]
   Float_t         phiJetGen_ite[100];   //[nJetGen]
   Int_t           nJetGen_kt;
   Float_t         pxJetGen_kt [100];   //[nJetGen]
   Float_t         pyJetGen_kt [100];   //[nJetGen]
   Float_t         pzJetGen_kt [100];   //[nJetGen]
   Float_t         eJetGen_kt  [100];   //[nJetGen]
   Float_t         etaJetGen_kt[100];   //[nJetGen]
   Float_t         phiJetGen_kt[100];   //[nJetGen]
   Int_t           nJetGen_sis;
   Float_t         pxJetGen_sis [100];   //[nJetGen]
   Float_t         pyJetGen_sis [100];   //[nJetGen]
   Float_t         pzJetGen_sis [100];   //[nJetGen]
   Float_t         eJetGen_sis  [100];   //[nJetGen]
   Float_t         etaJetGen_sis[100];   //[nJetGen]
   Float_t         phiJetGen_sis[100];   //[nJetGen]
   Float_t         pxMet ;
   Float_t         pyMet ;
   Float_t         pzMet ;
   Float_t         eMet  ;
   Float_t         etaMet;
   Float_t         phiMet;
   Float_t         pxMetGen ;
   Float_t         pyMetGen ;
   Float_t         pzMetGen ;
   Float_t         eMetGen  ;
   Float_t         etaMetGen;
   Float_t         phiMetGen;

   // List of branches
   TBranch        *b_event;   //!
   TBranch        *b_nMC;   //!
   TBranch        *b_pdgIdMC;   //!
   TBranch        *b_statusMC;   //!
   TBranch        *b_motherIDMC;   //!
   TBranch        *b_pxMC ;   //!
   TBranch        *b_pyMC ;   //!
   TBranch        *b_pzMC ;   //!
   TBranch        *b_eMC  ;   //!
   TBranch        *b_etaMC;   //!
   TBranch        *b_phiMC;   //!
   TBranch        *b_nPhot;   //!
   TBranch        *b_pxPhot ;   //!
   TBranch        *b_pyPhot ;   //!
   TBranch        *b_pzPhot ;   //!
   TBranch        *b_ePhot  ;   //!
   TBranch        *b_etaPhot;   //!
   TBranch        *b_phiPhot;   //!
   TBranch        *b_ptiso02Phot;   //!
   TBranch        *b_ntrkiso02Phot;   //!
   TBranch        *b_ptiso025Phot;   //!
   TBranch        *b_ntrkiso025Phot;   //!
   TBranch        *b_ptiso03Phot;   //!
   TBranch        *b_ntrkiso03Phot;   //!
   TBranch        *b_ptiso035Phot;   //!
   TBranch        *b_ntrkiso035Phot;   //!
   TBranch        *b_ptiso04Phot;   //!
   TBranch        *b_ntrkiso04Phot;   //!
   TBranch        *b_ptisoatecal02Phot;   //!
   TBranch        *b_ntrkisoatecal02Phot;   //!
   TBranch        *b_ptisoatecal025Phot;   //!
   TBranch        *b_ntrkisoatecal025Phot;   //!
   TBranch        *b_ptisoatecal03Phot;   //!
   TBranch        *b_ntrkisoatecal03Phot;   //!
   TBranch        *b_ptisoatecal035Phot;   //!
   TBranch        *b_ntrkisoatecal035Phot;   //!
   TBranch        *b_ptisoatecal04Phot;   //!
   TBranch        *b_hcalovecal01Phot;   //!
   TBranch        *b_hcalovecal02Phot;   //!
   TBranch        *b_hcalovecal025Phot;   //!
   TBranch        *b_hcalovecal03Phot;   //!
   TBranch        *b_hcalovecal04Phot;   //!
   TBranch        *b_hcalovecal05Phot;   //!
   TBranch        *b_ecaliso02Phot;   //!
   TBranch        *b_ecaliso03Phot;   //!
   TBranch        *b_ecaliso035Phot;   //!
   TBranch        *b_ecaliso04Phot;   //!
   TBranch        *b_ecaliso05Phot;   //!
    TBranch        *b_LATPhot;   //!
   TBranch        *b_sMajMajPhot;   //!
   TBranch        *b_sMinMinPhot;   //!
   TBranch        *b_FisherPhot;   //!
   TBranch        *b_nJet_ite;   //!
   TBranch        *b_pxJet_ite ;   //!
   TBranch        *b_pyJet_ite ;   //!
   TBranch        *b_pzJet_ite ;   //!
   TBranch        *b_eJet_ite  ;   //!
   TBranch        *b_etaJet_ite;   //!
   TBranch        *b_phiJet_ite;   //!
   TBranch        *b_nJetGen_ite;   //!
   TBranch        *b_pxJetGen_ite ;   //!
   TBranch        *b_pyJetGen_ite ;   //!
   TBranch        *b_pzJetGen_ite ;   //!
   TBranch        *b_eJetGen_ite  ;   //!
   TBranch        *b_etaJetGen_ite;   //!
   TBranch        *b_phiJetGen_ite;   //!
   TBranch        *b_nJet_kt;   //!
   TBranch        *b_pxJet_kt ;   //!
   TBranch        *b_pyJet_kt ;   //!
   TBranch        *b_pzJet_kt ;   //!
   TBranch        *b_eJet_kt  ;   //!
   TBranch        *b_etaJet_kt;   //!
   TBranch        *b_phiJet_kt;   //!
   TBranch        *b_nJetGen_kt;   //!
   TBranch        *b_pxJetGen_kt ;   //!
   TBranch        *b_pyJetGen_kt ;   //!
   TBranch        *b_pzJetGen_kt ;   //!
   TBranch        *b_eJetGen_kt  ;   //!
   TBranch        *b_etaJetGen_kt;   //!
   TBranch        *b_phiJetGen_kt;   //!
   TBranch        *b_nJet_sis;   //!
   TBranch        *b_pxJet_sis ;   //!
   TBranch        *b_pyJet_sis ;   //!
   TBranch        *b_pzJet_sis ;   //!
   TBranch        *b_eJet_sis  ;   //!
   TBranch        *b_etaJet_sis;   //!
   TBranch        *b_phiJet_sis;   //!
   TBranch        *b_nJetGen_sis;   //!
   TBranch        *b_pxJetGen_sis ;   //!
   TBranch        *b_pyJetGen_sis ;   //!
   TBranch        *b_pzJetGen_sis ;   //!
   TBranch        *b_eJetGen_sis  ;   //!
   TBranch        *b_etaJetGen_sis;   //!
   TBranch        *b_phiJetGen_sis;   //!
   TBranch        *b_pxMet;   //!
   TBranch        *b_pyMet;   //!
   TBranch        *b_pzMet;   //!
   TBranch        *b_eMet;   //!
   TBranch        *b_etaMet;   //!
   TBranch        *b_phiMet;   //!
   TBranch        *b_pxMetGen;   //!
   TBranch        *b_pyMetGen;   //!
   TBranch        *b_pzMetGen;   //!
   TBranch        *b_eMetGen;   //!
   TBranch        *b_etaMetGen;   //!
   TBranch        *b_phiMetGen;   //!

   TH1D*    allptphot;;
   TH1D*    allptjet;
   TH1D*    alldeltaphi;
   TH1D*    alldeltar;
   TH1D*    alldeltae;
   TH2D*    alldeltardeltae;

   Int_t ntrkisos;
   Float_t pts;
   Float_t ptisos;
   Float_t ptisoatecals;
   Float_t hcalovecals;
   Float_t sMajMajs;
   Float_t sMinMins;
   Float_t ecalisos;
   Int_t ntrkisob;
   Float_t ptb;
   Float_t ptisob;
   Float_t ptisoatecalb;
   Float_t hcalovecalb;
   Float_t sMajMajb;
   Float_t sMinMinb;
   Float_t ecalisob;

   Int_t isphoton;
   Int_t issignal;
   Float_t weight;
   Float_t nniso;
   Float_t nniso_int;
   Float_t ptph;
   Float_t ptj;
   Float_t etaph;
   Float_t etaj;
   Float_t phiph;
   Float_t phij;
   Float_t pt2jet;
   Float_t ptphottrue;
   Float_t ptjettrue;
   Float_t ptquarktrue;
   Float_t phiphottrue;
   Float_t phijettrue;
   Float_t phiquarktrue;


   TFile* hOutputFile ;
   TTree * S_tree ;
   TTree * B_tree ;
   TTree * ana_tree ;

   int npb;

   gjettree(TTree *tree=0, char * outputname = "test.root");
   virtual ~gjettree();
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop(double cross = 1., int algo = 1, int NEVT = 10000000);
   virtual void     BookHistos();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef gjettree_cxx
gjettree::gjettree(TTree *tree, char * outputname)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("output.root");
      if (!f) {
         f = new TFile("output.root");
      }
      tree = (TTree*)gDirectory->Get("pippo");

   }
   Init(tree);
   hOutputFile   = new TFile(outputname , "RECREATE" ) ;
}

gjettree::~gjettree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
   S_tree->Write();
   B_tree->Write();
   ana_tree->Write();
   hOutputFile->Write() ;
   hOutputFile->Close() ;
   hOutputFile->Delete();
}

Int_t gjettree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}

Long64_t gjettree::LoadTree(Long64_t entry)
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

void gjettree::Init(TTree *tree)
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

   fChain->SetBranchAddress("event", &event, &b_event);
   fChain->SetBranchAddress("nMC", &nMC, &b_nMC);
   fChain->SetBranchAddress("pdgIdMC", pdgIdMC, &b_pdgIdMC);
   fChain->SetBranchAddress("statusMC", statusMC, &b_statusMC);
   fChain->SetBranchAddress("motherIDMC", motherIDMC, &b_motherIDMC);
   fChain->SetBranchAddress("pxMC ", pxMC , &b_pxMC );
   fChain->SetBranchAddress("pyMC ", pyMC , &b_pyMC );
   fChain->SetBranchAddress("pzMC ", pzMC , &b_pzMC );
   fChain->SetBranchAddress("eMC  ", eMC  , &b_eMC  );
   fChain->SetBranchAddress("etaMC", etaMC, &b_etaMC);
   fChain->SetBranchAddress("phiMC", phiMC, &b_phiMC);
   fChain->SetBranchAddress("nPhot", &nPhot, &b_nPhot);
   fChain->SetBranchAddress("pxPhot ", pxPhot , &b_pxPhot );
   fChain->SetBranchAddress("pyPhot ", pyPhot , &b_pyPhot );
   fChain->SetBranchAddress("pzPhot ", pzPhot , &b_pzPhot );
   fChain->SetBranchAddress("ePhot  ", ePhot  , &b_ePhot  );
   fChain->SetBranchAddress("etaPhot", etaPhot, &b_etaPhot);
   fChain->SetBranchAddress("phiPhot", phiPhot, &b_phiPhot);
   fChain->SetBranchAddress("ptiso02Phot", ptiso02Phot, &b_ptiso02Phot);
   fChain->SetBranchAddress("ntrkiso02Phot", ntrkiso02Phot, &b_ntrkiso02Phot);
   fChain->SetBranchAddress("ptiso025Phot", ptiso025Phot, &b_ptiso025Phot);
   fChain->SetBranchAddress("ntrkiso025Phot", ntrkiso025Phot, &b_ntrkiso025Phot);
   fChain->SetBranchAddress("ptiso03Phot", ptiso03Phot, &b_ptiso03Phot);
   fChain->SetBranchAddress("ntrkiso03Phot", ntrkiso03Phot, &b_ntrkiso03Phot);
   fChain->SetBranchAddress("ptiso035Phot", ptiso035Phot, &b_ptiso035Phot);
   fChain->SetBranchAddress("ntrkiso035Phot", ntrkiso035Phot, &b_ntrkiso035Phot);
   fChain->SetBranchAddress("ptiso04Phot", ptiso04Phot, &b_ptiso04Phot);
   fChain->SetBranchAddress("ntrkiso04Phot", ntrkiso04Phot, &b_ntrkiso04Phot);
   fChain->SetBranchAddress("ptisoatecal02Phot", ptisoatecal02Phot, &b_ptisoatecal02Phot);
   fChain->SetBranchAddress("ntrkisoatecal02Phot", ntrkisoatecal02Phot, &b_ntrkisoatecal02Phot);
   fChain->SetBranchAddress("ptisoatecal025Phot", ptisoatecal025Phot, &b_ptisoatecal025Phot);
   fChain->SetBranchAddress("ntrkisoatecal025Phot", ntrkisoatecal025Phot, &b_ntrkisoatecal025Phot);
   fChain->SetBranchAddress("ptisoatecal03Phot", ptisoatecal03Phot, &b_ptisoatecal03Phot);
   fChain->SetBranchAddress("ntrkisoatecal03Phot", ntrkisoatecal03Phot, &b_ntrkisoatecal03Phot);
   fChain->SetBranchAddress("ptisoatecal035Phot", ptisoatecal035Phot, &b_ptisoatecal035Phot);
   fChain->SetBranchAddress("ntrkisoatecal035Phot", ntrkisoatecal035Phot, &b_ntrkisoatecal035Phot);
   fChain->SetBranchAddress("ptisoatecal04Phot", ptisoatecal04Phot, &b_ptisoatecal04Phot);
   fChain->SetBranchAddress("hcalovecal01Phot", hcalovecal01Phot, &b_hcalovecal01Phot);
   fChain->SetBranchAddress("hcalovecal02Phot", hcalovecal02Phot, &b_hcalovecal02Phot);
   fChain->SetBranchAddress("hcalovecal025Phot", hcalovecal025Phot, &b_hcalovecal025Phot);
   fChain->SetBranchAddress("hcalovecal03Phot", hcalovecal03Phot, &b_hcalovecal03Phot);
   fChain->SetBranchAddress("hcalovecal04Phot", hcalovecal04Phot, &b_hcalovecal04Phot);
   fChain->SetBranchAddress("hcalovecal05Phot", hcalovecal05Phot, &b_hcalovecal05Phot);
   fChain->SetBranchAddress("ecaliso02Phot", ecaliso02Phot, &b_ecaliso02Phot);
   fChain->SetBranchAddress("ecaliso03Phot", ecaliso03Phot, &b_ecaliso03Phot);
   fChain->SetBranchAddress("ecaliso035Phot", ecaliso035Phot, &b_ecaliso035Phot);
   fChain->SetBranchAddress("ecaliso04Phot", ecaliso04Phot, &b_ecaliso04Phot);
   fChain->SetBranchAddress("ecaliso05Phot", ecaliso05Phot, &b_ecaliso05Phot);
   fChain->SetBranchAddress("LATPhot", LATPhot, &b_LATPhot);
   fChain->SetBranchAddress("sMajMajPhot", sMajMajPhot, &b_sMajMajPhot);
   fChain->SetBranchAddress("sMinMinPhot", sMinMinPhot, &b_sMinMinPhot);
   fChain->SetBranchAddress("FisherPhot", FisherPhot, &b_FisherPhot);
   fChain->SetBranchAddress("nJet_ite", &nJet_ite, &b_nJet_ite);
   fChain->SetBranchAddress("pxJet_ite ", pxJet_ite , &b_pxJet_ite );
   fChain->SetBranchAddress("pyJet_ite ", pyJet_ite , &b_pyJet_ite );
   fChain->SetBranchAddress("pzJet_ite ", pzJet_ite , &b_pzJet_ite );
   fChain->SetBranchAddress("eJet_ite  ", eJet_ite  , &b_eJet_ite  );
   fChain->SetBranchAddress("etaJet_ite", etaJet_ite, &b_etaJet_ite);
   fChain->SetBranchAddress("phiJet_ite", phiJet_ite, &b_phiJet_ite);
   fChain->SetBranchAddress("nJetGen_ite", &nJetGen_ite, &b_nJetGen_ite);
   fChain->SetBranchAddress("pxJetGen_ite ", pxJetGen_ite , &b_pxJetGen_ite );
   fChain->SetBranchAddress("pyJetGen_ite ", pyJetGen_ite , &b_pyJetGen_ite );
   fChain->SetBranchAddress("pzJetGen_ite ", pzJetGen_ite , &b_pzJetGen_ite );
   fChain->SetBranchAddress("eJetGen_ite  ", eJetGen_ite  , &b_eJetGen_ite  );
   fChain->SetBranchAddress("etaJetGen_ite", etaJetGen_ite, &b_etaJetGen_ite);
   fChain->SetBranchAddress("phiJetGen_ite", phiJetGen_ite, &b_phiJetGen_ite);
   fChain->SetBranchAddress("nJet_kt", &nJet_kt, &b_nJet_kt);
   fChain->SetBranchAddress("pxJet_kt ", pxJet_kt , &b_pxJet_kt );
   fChain->SetBranchAddress("pyJet_kt ", pyJet_kt , &b_pyJet_kt );
   fChain->SetBranchAddress("pzJet_kt ", pzJet_kt , &b_pzJet_kt );
   fChain->SetBranchAddress("eJet_kt  ", eJet_kt  , &b_eJet_kt  );
   fChain->SetBranchAddress("etaJet_kt", etaJet_kt, &b_etaJet_kt);
   fChain->SetBranchAddress("phiJet_kt", phiJet_kt, &b_phiJet_kt);
   fChain->SetBranchAddress("nJetGen_kt", &nJetGen_kt, &b_nJetGen_kt);
   fChain->SetBranchAddress("pxJetGen_kt ", pxJetGen_kt , &b_pxJetGen_kt );
   fChain->SetBranchAddress("pyJetGen_kt ", pyJetGen_kt , &b_pyJetGen_kt );
   fChain->SetBranchAddress("pzJetGen_kt ", pzJetGen_kt , &b_pzJetGen_kt );
   fChain->SetBranchAddress("eJetGen_kt  ", eJetGen_kt  , &b_eJetGen_kt  );
   fChain->SetBranchAddress("etaJetGen_kt", etaJetGen_kt, &b_etaJetGen_kt);
   fChain->SetBranchAddress("phiJetGen_kt", phiJetGen_kt, &b_phiJetGen_kt);
   fChain->SetBranchAddress("nJet_sis", &nJet_sis, &b_nJet_sis);
   fChain->SetBranchAddress("pxJet_sis ", pxJet_sis , &b_pxJet_sis );
   fChain->SetBranchAddress("pyJet_sis ", pyJet_sis , &b_pyJet_sis );
   fChain->SetBranchAddress("pzJet_sis ", pzJet_sis , &b_pzJet_sis );
   fChain->SetBranchAddress("eJet_sis  ", eJet_sis  , &b_eJet_sis  );
   fChain->SetBranchAddress("etaJet_sis", etaJet_sis, &b_etaJet_sis);
   fChain->SetBranchAddress("phiJet_sis", phiJet_sis, &b_phiJet_sis);
   fChain->SetBranchAddress("nJetGen_sis", &nJetGen_sis, &b_nJetGen_sis);
   fChain->SetBranchAddress("pxJetGen_sis ", pxJetGen_sis , &b_pxJetGen_sis );
   fChain->SetBranchAddress("pyJetGen_sis ", pyJetGen_sis , &b_pyJetGen_sis );
   fChain->SetBranchAddress("pzJetGen_sis ", pzJetGen_sis , &b_pzJetGen_sis );
   fChain->SetBranchAddress("eJetGen_sis  ", eJetGen_sis  , &b_eJetGen_sis  );
   fChain->SetBranchAddress("etaJetGen_sis", etaJetGen_sis, &b_etaJetGen_sis);
   fChain->SetBranchAddress("phiJetGen_sis", phiJetGen_sis, &b_phiJetGen_sis);
   fChain->SetBranchAddress("pxMet ", &pxMet , &b_pxMet);
   fChain->SetBranchAddress("pyMet ", &pyMet , &b_pyMet);
   fChain->SetBranchAddress("pzMet ", &pzMet , &b_pzMet);
   fChain->SetBranchAddress("eMet  ", &eMet  , &b_eMet);
   fChain->SetBranchAddress("etaMet", &etaMet, &b_etaMet);
   fChain->SetBranchAddress("phiMet", &phiMet, &b_phiMet);
   fChain->SetBranchAddress("pxMetGen ", &pxMetGen , &b_pxMetGen);
   fChain->SetBranchAddress("pyMetGen ", &pyMetGen , &b_pyMetGen);
   fChain->SetBranchAddress("pzMetGen ", &pzMetGen , &b_pzMetGen);
   fChain->SetBranchAddress("eMetGen  ", &eMetGen  , &b_eMetGen);
   fChain->SetBranchAddress("etaMetGen", &etaMetGen, &b_etaMetGen);
   fChain->SetBranchAddress("phiMetGen", &phiMetGen, &b_phiMetGen);
   Notify();

   npb = 1000;

}

Bool_t gjettree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normaly not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void gjettree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
#endif // #ifdef gjettree_cxx
