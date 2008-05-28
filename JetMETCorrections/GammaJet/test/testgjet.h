//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Dec  7 11:38:21 2007 by ROOT version 5.14/00f
// from TTree pippo/Analysis tree
// found on file: output.root
//////////////////////////////////////////////////////////

#ifndef testgjet_h
#define testgjet_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TProfile.h>
#include <TTree.h>

class testgjet {
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
   Float_t         pxJet [44];   //[nJet]
   Float_t         pyJet [44];   //[nJet]
   Float_t         pzJet [44];   //[nJet]
   Float_t         eJet  [44];   //[nJet]
   Float_t         etaJet[44];   //[nJet]
   Float_t         phiJet[44];   //[nJet]
   Int_t           nJetGen;
   Float_t         pxJetGen [69];   //[nJetGen]
   Float_t         pyJetGen [69];   //[nJetGen]
   Float_t         pzJetGen [69];   //[nJetGen]
   Float_t         eJetGen  [69];   //[nJetGen]
   Float_t         etaJetGen[69];   //[nJetGen]
   Float_t         phiJetGen[69];   //[nJetGen]
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
   TBranch        *b_nJet;   //!
   TBranch        *b_pxJet ;   //!
   TBranch        *b_pyJet ;   //!
   TBranch        *b_pzJet ;   //!
   TBranch        *b_eJet  ;   //!
   TBranch        *b_etaJet;   //!
   TBranch        *b_phiJet;   //!
   TBranch        *b_nJetGen;   //!
   TBranch        *b_pxJetGen ;   //!
   TBranch        *b_pyJetGen ;   //!
   TBranch        *b_pzJetGen ;   //!
   TBranch        *b_eJetGen  ;   //!
   TBranch        *b_etaJetGen;   //!
   TBranch        *b_phiJetGen;   //!
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
   TH1D*    bestptphot;
   TH1D*    bestptphotlarge;
   TH1D*    allptjet;
   TH1D*    bestptjet;
   TH1D*    resophot;
   TH1D*    biasphotvspt_fitted;
   TH1D*    resophotvspt_fitted;
   TH1D*    resojet;
   TH1D*    biasjetvspt_fitted;
   TH1D*    resojetvspt_fitted;
   TH1D*    resojetMC;
   TH1D*    biasjetMCvspt_fitted;
   TH1D*    resojetMCvspt_fitted;
   TH1D*    bestptphotgen;
   TH1D*    bestptphotgen_etapt;
   TH1D*    bestptphotgenlarge;
   TH1D*    deltapt;
   TH1D*    deltaptnorm;
   TH1D*    ratioptnorm;
   TH1D*    ratioptnorm_loose;
   TH1D*    deltaptnormbar;
   TH1D*    biasdeltaptnormvspt_fitted;
   TH1D*    resodeltaptnormvspt_fitted;
   TH1D*    biasdeltaptnormvseta_fitted;
   TH1D*    resodeltaptnormvseta_fitted;
   TH1D*    deltaptjet;
   TH1D*    deltaptjetnorm;
   TH1D*    deltaptjetnorm_etaptcut;
   TH1D*    deltaptjetnorm_deltaphi;
   TH1D*    deltaptjetnormbar;
   TH1D*    deltaptjetquark;
   TH1D*    deltaptjetquarknorm;
   TH1D*    deltaptjetquarknorm_etaptcut;
   TH1D*    deltaptjetquarknorm_deltaphi;
   TH1D*    biasdeltaptjetnormvspt_fitted;
   TH1D*    resodeltaptjetnormvspt_fitted;
   TH1D*    deltaptjetquarknormbar;
   TH1D*    biasdeltaptjetquarknormvspt_fitted;
   TH1D*    resodeltaptjetquarknormvspt_fitted;
   TH1D*    alldeltaphi;
   TH1D*    bestdeltaphi;
   TH1D*    alldeltar;
   TH1D*    alldeltae;
   TH2D*    etaphi0    ;
   TH2D*    etaphi1 ;
   TH2D*    etaphi2 ;
   TH2D*    etaphi3 ;
   TH2D*    etaphi4 ;
   TH2D*    etaphi5 ;
   TH2D*    etaphi6 ;
   TH2D*    etaphi7 ;
   TH2D*    etaphi8 ;
   TH2D*    etaphi9 ;
   TH1D*    bestdeltaphijet;
   TH1D*    bestdeltaphijetzoom;
   TH1D*    deltarjet;
   TH1D*    deltaejet;
   TH1D*    bestdeltaphijetquark;
   TH1D*    bestdeltaphijetquarkzoom     ;
   TH1D*    deltarjetquark;
   TH1D*    deltaejetquark;
   TH2D*    etaphibestphot0   ;
   TH2D*    etaphibestphot1;
   TH2D*    etaphibestphot2;
   TH2D*    etaphibestphot3;
   TH2D*    etaphibestphot4;
   TH2D*    etaphibestphot5;
   TH2D*    etaphibestphot6;
   TH2D*    etaphibestphot7;
   TH2D*    etaphibestphot8;
   TH2D*    etaphibestphot9;
   TH2D*    etaphibestjet0 ;
   TH2D*    etaphibestjet1;
   TH2D*    etaphibestjet2;
   TH2D*    etaphibestjet3;
   TH2D*    etaphibestjet4;
   TH2D*    etaphibestjet5;
   TH2D*    etaphibestjet6;
   TH2D*    etaphibestjet7;
   TH2D*    etaphibestjet8;
   TH2D*    etaphibestjet9;
   TH1D*    bestdeltaphizoom;
   TH2D*    resophotvspt;
   TH2D*    resojetvspt;
   TH2D*    resojetMCvspt;
   TH2D*    deltaptnormvspt;
   TH2D*    ratioptnormvspt;
   TH2D*    ratioptnormvspt_loose;
   TH2D*    deltaptnormvseta;
   TH2D*    deltaptnormvsdeltaphi;
   TH2D*    deltaptjetnormvspt;
   TH2D*    deltaptjetnormvspt_etaptcut;
   TH2D*    deltaptjetnormvspt_deltaphi;
   TH2D*    deltaptnormvspt_nocut;
   TH2D*    deltaptnormvspt_cut;
   TH2D*    deltaptnormvspt_cut1;
   TH2D*    deltaptnormvspt_cut2;
   TH2D*    deltaptnormvspt_cut3;
   TH2D*    deltaptnormvspt_cut4;
   TH2D*    deltaptnormvspt_cut5;
   TH2D*    deltaptnormvspt_cut6;
   TH2D*    deltaptnormvspt_cut7;
   TH2D*    deltaptnormvspt_all;

   TH1D*    pt_nocut;
   TH1D*    pt_cut;
   TH1D*    pt_cut1;
   TH1D*    pt_cut2;
   TH1D*    pt_cut3;
   TH1D*    pt_cut4;
   TH1D*    pt_cut5;
   TH1D*    pt_cut6;
   TH1D*    pt_cut7;
   TH1D*    pt_all;
   TH1D*    pt_loose;

   TH1D*    pt_nocut_matched;
   TH1D*    pt_cut_matched;
   TH1D*    pt_cut1_matched;
   TH1D*    pt_cut2_matched;
   TH1D*    pt_cut3_matched;
   TH1D*    pt_cut4_matched;
   TH1D*    pt_cut5_matched;
   TH1D*    pt_cut6_matched;
   TH1D*    pt_cut7_matched;
   TH1D*    pt_all_matched;

   TProfile*    deltaptvseta;
   TH2D*    deltaptjetquarknormvspt;
   TH2D*    deltaptjetquarknormvspt_etaptcut;
   TH2D*    deltaptjetquarknormvspt_deltaphi;
   TH2D*    alldeltardeltae;
   TH2D*    deltardeltaejet;
   TProfile*    deltaptvsetajet;
   TH2D*    deltardeltaejetquark;
   TProfile*    deltaptvsetajetquark;
   TH2D*    deltaptjetquarknormvsdeltaphi;

   TH1D* sMajMajphot;
   TH1D* sMinMinphot;
   TH2D* sMinMinsMajMajphot;
   TH1D* Latphot;
   TH1D* fisherphot;
   TH1D* ptiso03phot;
   TH1D* ntrkiso03phot;
   TH1D* ptisoatecal03phot;
   TH2D* ptisovsatecal03relphot;
   TH1D* ntrkisoatecal03phot;
   TH1D* ecaliso04phot;
   TH1D* ptiso02relphot;
   TH1D* ptiso025relphot;
   TH1D* ptiso03relphot;
   TH1D* ptiso035relphot;
   TH1D* ptiso04relphot;
   TH1D* ptisoatecal02relphot;
   TH1D* ptisoatecal025relphot;
   TH1D* ptisoatecal03relphot;
   TH1D* ptisoatecal035relphot;
   TH1D* ptisoatecal04relphot;
   TH1D* hcalovecal01phot;
   TH1D* hcalovecal02phot;
   TH1D* hcalovecal025phot;
   TH1D* hcalovecal03phot;
   TH1D* hcalovecal04phot;
   TH1D* hcalovecal05phot;
   TH1D* ecaliso015relphot;
   TH1D* ecaliso02relphot;
   TH1D* ecaliso025relphot;
   TH1D* ecaliso03relphot;
   TH1D* ecaliso035relphot;
   TH1D* ecaliso04relphot;
   TH1D* ecaliso02recrelphot;
   TH1D* ecaliso03recrelphot;
   TH1D* ecaliso035recrelphot;
   TH1D* ecaliso04recrelphot;
   TH1D* ecaliso05recrelphot;

   TH1D* nnphot_old;
   TH1D* nnphot;

   TH1D* sMajMajbkg;
   TH1D* sMinMinbkg;
   TH2D* sMinMinsMajMajbkg;
   TH1D* Latbkg;
   TH1D* fisherbkg;
   TH1D* ptiso03bkg;
   TH1D* ntrkiso03bkg;
   TH1D* ptisoatecal03bkg;
   TH2D* ptisovsatecal03relbkg;
   TH1D* ntrkisoatecal03bkg;
   TH1D* ecaliso04bkg;

   TH1D* ptiso02relbkg;
   TH1D* ptiso025relbkg;
   TH1D* ptiso03relbkg;
   TH1D* ptiso035relbkg;
   TH1D* ptiso04relbkg;
   TH1D* ptisoatecal02relbkg;
   TH1D* ptisoatecal025relbkg;
   TH1D* ptisoatecal03relbkg;
   TH1D* ptisoatecal035relbkg;
   TH1D* ptisoatecal04relbkg;
   TH1D* hcalovecal01bkg;
   TH1D* hcalovecal02bkg;
   TH1D* hcalovecal025bkg;
   TH1D* hcalovecal03bkg;
   TH1D* hcalovecal04bkg;
   TH1D* hcalovecal05bkg;
   TH1D* ecaliso015relbkg;
   TH1D* ecaliso02relbkg;
   TH1D* ecaliso025relbkg;
   TH1D* ecaliso03relbkg;
   TH1D* ecaliso035relbkg;
   TH1D* ecaliso04relbkg;
   TH1D* ecaliso02recrelbkg;
   TH1D* ecaliso03recrelbkg;
   TH1D* ecaliso035recrelbkg;
   TH1D* ecaliso04recrelbkg;
   TH1D* ecaliso05recrelbkg;
   TH1D* nnbkg_old;
   TH1D* nnbkg;
   
   TH1D* etaphot_all;
   TH1D* etajet_all;
   TH1D* deltaphi_all;
   TH1D* njetoverth_all;
   TH1D* ptjet2ndovptphot_all;
   TH1D* ptjet2_all;

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

   testgjet(TTree *tree=0, char * outputname = "test.root");
   virtual ~testgjet();
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop(int NEVT = 10000000);
   virtual void     BookHistos();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef testgjet_cxx
testgjet::testgjet(TTree *tree, char * outputname)
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

testgjet::~testgjet()
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

Int_t testgjet::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}

Long64_t testgjet::LoadTree(Long64_t entry)
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

void testgjet::Init(TTree *tree)
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
   fChain->SetBranchAddress("nJet", &nJet, &b_nJet);
   fChain->SetBranchAddress("pxJet ", pxJet , &b_pxJet );
   fChain->SetBranchAddress("pyJet ", pyJet , &b_pyJet );
   fChain->SetBranchAddress("pzJet ", pzJet , &b_pzJet );
   fChain->SetBranchAddress("eJet  ", eJet  , &b_eJet  );
   fChain->SetBranchAddress("etaJet", etaJet, &b_etaJet);
   fChain->SetBranchAddress("phiJet", phiJet, &b_phiJet);
   fChain->SetBranchAddress("nJetGen", &nJetGen, &b_nJetGen);
   fChain->SetBranchAddress("pxJetGen ", pxJetGen , &b_pxJetGen );
   fChain->SetBranchAddress("pyJetGen ", pyJetGen , &b_pyJetGen );
   fChain->SetBranchAddress("pzJetGen ", pzJetGen , &b_pzJetGen );
   fChain->SetBranchAddress("eJetGen  ", eJetGen  , &b_eJetGen  );
   fChain->SetBranchAddress("etaJetGen", etaJetGen, &b_etaJetGen);
   fChain->SetBranchAddress("phiJetGen", phiJetGen, &b_phiJetGen);
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
}

Bool_t testgjet::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normaly not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void testgjet::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
#endif // #ifdef testgjet_cxx
