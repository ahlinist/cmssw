//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Dec  7 11:38:21 2007 by ROOT version 5.14/00f
// from TTree pippo/Analysis tree
// found on file: output.root
//////////////////////////////////////////////////////////

#ifndef gjetefficiency_h
#define gjetefficiency_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TProfile.h>
#include <TTree.h>

#include <TMVAPhotons_MLP.class.C>
#include <TMVAPhotons_1_MLP.class.C>
#include <TMVAPhotons_2_MLP.class.C>
#include <TMVAPhotons_3_MLP.class.C>
#include <TMVAPhotons_4_MLP.class.C>

#include "Config.hpp"

#include <map>
#include <string>

struct photoncandidate {
  double e;
  double pt;
  double eta;
  double phi;
};

struct jetcandidate {
  double e;
  double pt;
  double eta;
  double phi;
};

struct photonidcuts {
  float hcaliso;
  float ecaliso;
  float trackiso;
  int tracknb;
  float sminmin;
  float smajmaj;
};

class gjetefficiency {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           event;
   Float_t         genpt;
   Int_t           nMC;
   Int_t           pdgIdMC[100];   //[nMC]
   Int_t           statusMC[100];   //[nMC]
   Int_t           motherIDMC[100];   //[nMC]
   Float_t         pxMC [100];   //[nMC]
   Float_t         pyMC [100];   //[nMC]
   Float_t         pzMC [100];   //[nMC]
   Float_t         eMC  [100];   //[nMC]
   Float_t         etaMC[100];   //[nMC]
   Float_t         phiMC[100];   //[nMC]
   Int_t           nPhot;
   Float_t         pxPhot [8];   //[nPhot]
   Float_t         pyPhot [8];   //[nPhot]
   Float_t         pzPhot [8];   //[nPhot]
   Float_t         ePhot  [8];   //[nPhot]
   Float_t         escPhot  [8];   //[nPhot]
   Float_t         etaPhot[8];   //[nPhot]
   Float_t         phiPhot[8];   //[nPhot]
   Int_t           nconvPhot;
   Float_t         chi2convPhot[1];   //[nconvPhot]
   Float_t         ndofconvPhot[1];   //[nconvPhot]
   Float_t         xconvPhot[1];   //[nconvPhot]
   Float_t         yconvPhot[1];   //[nconvPhot]
   Float_t         zconvPhot[1];   //[nconvPhot]
   Int_t           ntrkconvPhot[1];   //[nconvPhot]
   Float_t         eovpconvPhot[1];   //[nconvPhot]
   Float_t         etaecalconvPhot[1];   //[nconvPhot]
   Float_t         phiecalconvPhot[1];   //[nconvPhot]
   Float_t         eecalconvPhot[1];   //[nconvPhot]
   Float_t         ptiso0015Phot[8];   //[nPhot]
   Int_t           ntrkiso0015Phot[8];   //[nPhot]
   Float_t         ptiso035Phot[8];   //[nPhot]
   Int_t           ntrkiso035Phot[8];   //[nPhot]
   Float_t         ptiso05Phot[8];   //[nPhot]
   Int_t           ntrkiso05Phot[8];   //[nPhot]
   Float_t         ptiso07Phot[8];   //[nPhot]
   Int_t           ntrkiso07Phot[8];   //[nPhot]
   Float_t         ptiso1Phot[8];   //[nPhot]
   Int_t           ntrkiso1Phot[8];   //[nPhot]
   Float_t         hcalovecal01Phot[8];   //[nPhot]
   Float_t         hcalovecal015Phot[8];   //[nPhot]
   Float_t         hcalovecal04Phot[8];   //[nPhot]
   Float_t         hcalovecal05Phot[8];   //[nPhot]
   Float_t         hcalovecal07Phot[8];   //[nPhot]
   Float_t         hcalovecal1Phot[8];   //[nPhot]
   Float_t         ecaliso01Phot[8];   //[nPhot]
   Float_t         ecaliso015Phot[8];   //[nPhot]
   Float_t         ecaliso04Phot[8];   //[nPhot]
   Float_t         ecaliso05Phot[8];   //[nPhot]
   Float_t         ecaliso07Phot[8];   //[nPhot]
   Float_t         ecaliso1Phot[8];   //[nPhot]
   Float_t         ecaliso04oldPhot[8];   //[nPhot]
   Float_t         LATPhot[8];   //[nPhot]
   Float_t         sMajMajPhot[8];   //[nPhot]
   Float_t         sMinMinPhot[8];   //[nPhot]
   Float_t         alphaPhot[8];   //[nPhot]
   Float_t         sEtaEtaPhot[8];   //[nPhot]
   Float_t         sPhiPhiPhot[8];   //[nPhot]
   Float_t         E1Phot[8];   //[nPhot]
   Float_t         E9Phot[8];   //[nPhot]
   Float_t         E25Phot[8];   //[nPhot]
   Float_t         FisherPhot[8];   //[nPhot]
   Float_t         sMajMajoldPhot[8];   //[nPhot]
   Float_t         sMinMinoldPhot[8];   //[nPhot]
   Int_t           nJet;
   Float_t         *eJet;
   Float_t         *etaJet;
   Float_t         *phiJet;
   Int_t           nJet_ite;
   Float_t         pxJet_ite [31];   //[nJet_ite]
   Float_t         pyJet_ite [31];   //[nJet_ite]
   Float_t         pzJet_ite [31];   //[nJet_ite]
   Float_t         eJet_ite  [31];   //[nJet_ite]
   Float_t         etaJet_ite[31];   //[nJet_ite]
   Float_t         phiJet_ite[31];   //[nJet_ite]
   Int_t           nJet_kt4;
   Float_t         pxJet_kt4 [45];   //[nJet_kt4]
   Float_t         pyJet_kt4 [45];   //[nJet_kt4]
   Float_t         pzJet_kt4 [45];   //[nJet_kt4]
   Float_t         eJet_kt4  [45];   //[nJet_kt4]
   Float_t         etaJet_kt4[45];   //[nJet_kt4]
   Float_t         phiJet_kt4[45];   //[nJet_kt4]
   Int_t           nJet_kt6;
   Float_t         pxJet_kt6 [37];   //[nJet_kt6]
   Float_t         pyJet_kt6 [37];   //[nJet_kt6]
   Float_t         pzJet_kt6 [37];   //[nJet_kt6]
   Float_t         eJet_kt6  [37];   //[nJet_kt6]
   Float_t         etaJet_kt6[37];   //[nJet_kt6]
   Float_t         phiJet_kt6[37];   //[nJet_kt6]
   Int_t           nJet_sis5;
   Float_t         pxJet_sis5 [41];   //[nJet_sis5]
   Float_t         pyJet_sis5 [41];   //[nJet_sis5]
   Float_t         pzJet_sis5 [41];   //[nJet_sis5]
   Float_t         eJet_sis5  [41];   //[nJet_sis5]
   Float_t         etaJet_sis5[41];   //[nJet_sis5]
   Float_t         phiJet_sis5[41];   //[nJet_sis5]
   Int_t           nJet_sis7;
   Float_t         pxJet_sis7 [35];   //[nJet_sis7]
   Float_t         pyJet_sis7 [35];   //[nJet_sis7]
   Float_t         pzJet_sis7 [35];   //[nJet_sis7]
   Float_t         eJet_sis7  [35];   //[nJet_sis7]
   Float_t         etaJet_sis7[35];   //[nJet_sis7]
   Float_t         phiJet_sis7[35];   //[nJet_sis7]
   Int_t           nJet_pfite;
   Float_t         pxJet_pfite [39];   //[nJet_pfite]
   Float_t         pyJet_pfite [39];   //[nJet_pfite]
   Float_t         pzJet_pfite [39];   //[nJet_pfite]
   Float_t         eJet_pfite  [39];   //[nJet_pfite]
   Float_t         etaJet_pfite[39];   //[nJet_pfite]
   Float_t         phiJet_pfite[39];   //[nJet_pfite]
   Int_t           nJetGen;
   Float_t         *eJetGen;
   Float_t         *etaJetGen;
   Float_t         *phiJetGen;
   Int_t           nJetGen_ite;
   Float_t         pxJetGen_ite [61];   //[nJetGen_ite]
   Float_t         pyJetGen_ite [61];   //[nJetGen_ite]
   Float_t         pzJetGen_ite [61];   //[nJetGen_ite]
   Float_t         eJetGen_ite  [61];   //[nJetGen_ite]
   Float_t         etaJetGen_ite[61];   //[nJetGen_ite]
   Float_t         phiJetGen_ite[61];   //[nJetGen_ite]
   Int_t           nJetGen_kt4;
   Float_t         pxJetGen_kt4 [100];   //[nJetGen_kt4]
   Float_t         pyJetGen_kt4 [100];   //[nJetGen_kt4]
   Float_t         pzJetGen_kt4 [100];   //[nJetGen_kt4]
   Float_t         eJetGen_kt4  [100];   //[nJetGen_kt4]
   Float_t         etaJetGen_kt4[100];   //[nJetGen_kt4]
   Float_t         phiJetGen_kt4[100];   //[nJetGen_kt4]
   Int_t           nJetGen_kt6;
   Float_t         pxJetGen_kt6 [68];   //[nJetGen_kt6]
   Float_t         pyJetGen_kt6 [68];   //[nJetGen_kt6]
   Float_t         pzJetGen_kt6 [68];   //[nJetGen_kt6]
   Float_t         eJetGen_kt6  [68];   //[nJetGen_kt6]
   Float_t         etaJetGen_kt6[68];   //[nJetGen_kt6]
   Float_t         phiJetGen_kt6[68];   //[nJetGen_kt6]
   Int_t           nJetGen_sis5;
   Float_t         pxJetGen_sis5 [100];   //[nJetGen_sis5]
   Float_t         pyJetGen_sis5 [100];   //[nJetGen_sis5]
   Float_t         pzJetGen_sis5 [100];   //[nJetGen_sis5]
   Float_t         eJetGen_sis5  [100];   //[nJetGen_sis5]
   Float_t         etaJetGen_sis5[100];   //[nJetGen_sis5]
   Float_t         phiJetGen_sis5[100];   //[nJetGen_sis5]
   Int_t           nJetGen_sis7;
   Float_t         pxJetGen_sis7 [78];   //[nJetGen_sis7]
   Float_t         pyJetGen_sis7 [78];   //[nJetGen_sis7]
   Float_t         pzJetGen_sis7 [78];   //[nJetGen_sis7]
   Float_t         eJetGen_sis7  [78];   //[nJetGen_sis7]
   Float_t         etaJetGen_sis7[78];   //[nJetGen_sis7]
   Float_t         phiJetGen_sis7[78];   //[nJetGen_sis7]
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
   Int_t           hltCount;
   Int_t           hltNamesLen;
   Char_t          HLTNames[2973];   //[hltNamesLen]
   Bool_t          HLTResults[159];   //[hltCount]

   // List of branches
   TBranch        *b_event;   //!
   TBranch        *b_genpt;   //!
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
   TBranch        *b_escPhot  ;   //!
   TBranch        *b_etaPhot;   //!
   TBranch        *b_phiPhot;   //!
   TBranch        *b_nconvPhot;   //!
   TBranch        *b_chi2convPhot;   //!
   TBranch        *b_ndofconvPhot;   //!
   TBranch        *b_xconvPhot;   //!
   TBranch        *b_yconvPhot;   //!
   TBranch        *b_zconvPhot;   //!
   TBranch        *b_ntrkconvPhot;   //!
   TBranch        *b_eovpconvPhot;   //!
   TBranch        *b_etaecalconvPhot;   //!
   TBranch        *b_phiecalconvPhot;   //!
   TBranch        *b_eecalconvPhot;   //!
   TBranch        *b_ptiso0015Phot;   //!
   TBranch        *b_ntrkiso0015Phot;   //!
   TBranch        *b_ptiso035Phot;   //!
   TBranch        *b_ntrkiso035Phot;   //!
   TBranch        *b_ptiso05Phot;   //!
   TBranch        *b_ntrkiso05Phot;   //!
   TBranch        *b_ptiso07Phot;   //!
   TBranch        *b_ntrkiso07Phot;   //!
   TBranch        *b_ptiso1Phot;   //!
   TBranch        *b_ntrkiso1Phot;   //!
   TBranch        *b_hcalovecal01Phot;   //!
   TBranch        *b_hcalovecal015Phot;   //!
   TBranch        *b_hcalovecal04Phot;   //!
   TBranch        *b_hcalovecal05Phot;   //!
   TBranch        *b_hcalovecal07Phot;   //!
   TBranch        *b_hcalovecal1Phot;   //!
   TBranch        *b_ecaliso01Phot;   //!
   TBranch        *b_ecaliso015Phot;   //!
   TBranch        *b_ecaliso04Phot;   //!
   TBranch        *b_ecaliso05Phot;   //!
   TBranch        *b_ecaliso07Phot;   //!
   TBranch        *b_ecaliso1Phot;   //!
   TBranch        *b_ecaliso04oldPhot;   //!
   TBranch        *b_LATPhot;   //!
   TBranch        *b_sMajMajPhot;   //!
   TBranch        *b_sMinMinPhot;   //!
   TBranch        *b_alphaPhot;   //!
   TBranch        *b_sEtaEtaPhot;   //!
   TBranch        *b_sPhiPhiPhot;   //!
   TBranch        *b_E1Phot;   //!
   TBranch        *b_E9Phot;   //!
   TBranch        *b_E25Phot;   //!
   TBranch        *b_FisherPhot;   //!
   TBranch        *b_sMajMajoldPhot;   //!
   TBranch        *b_sMinMinoldPhot;   //!
   TBranch        *b_nJet_ite;   //!
   TBranch        *b_pxJet_ite ;   //!
   TBranch        *b_pyJet_ite ;   //!
   TBranch        *b_pzJet_ite ;   //!
   TBranch        *b_eJet_ite  ;   //!
   TBranch        *b_etaJet_ite;   //!
   TBranch        *b_phiJet_ite;   //!
   TBranch        *b_nJet_kt4;   //!
   TBranch        *b_pxJet_kt4 ;   //!
   TBranch        *b_pyJet_kt4 ;   //!
   TBranch        *b_pzJet_kt4 ;   //!
   TBranch        *b_eJet_kt4  ;   //!
   TBranch        *b_etaJet_kt4;   //!
   TBranch        *b_phiJet_kt4;   //!
   TBranch        *b_nJet_kt6;   //!
   TBranch        *b_pxJet_kt6 ;   //!
   TBranch        *b_pyJet_kt6 ;   //!
   TBranch        *b_pzJet_kt6 ;   //!
   TBranch        *b_eJet_kt6  ;   //!
   TBranch        *b_etaJet_kt6;   //!
   TBranch        *b_phiJet_kt6;   //!
   TBranch        *b_nJet_sis5;   //!
   TBranch        *b_pxJet_sis5 ;   //!
   TBranch        *b_pyJet_sis5 ;   //!
   TBranch        *b_pzJet_sis5 ;   //!
   TBranch        *b_eJet_sis5  ;   //!
   TBranch        *b_etaJet_sis5;   //!
   TBranch        *b_phiJet_sis5;   //!
   TBranch        *b_nJet_sis7;   //!
   TBranch        *b_pxJet_sis7 ;   //!
   TBranch        *b_pyJet_sis7 ;   //!
   TBranch        *b_pzJet_sis7 ;   //!
   TBranch        *b_eJet_sis7  ;   //!
   TBranch        *b_etaJet_sis7;   //!
   TBranch        *b_phiJet_sis7;   //!
   TBranch        *b_nJet_pfite;   //!
   TBranch        *b_pxJet_pfite ;   //!
   TBranch        *b_pyJet_pfite ;   //!
   TBranch        *b_pzJet_pfite ;   //!
   TBranch        *b_eJet_pfite  ;   //!
   TBranch        *b_etaJet_pfite;   //!
   TBranch        *b_phiJet_pfite;   //!
   TBranch        *b_nJetGen_ite;   //!
   TBranch        *b_pxJetGen_ite ;   //!
   TBranch        *b_pyJetGen_ite ;   //!
   TBranch        *b_pzJetGen_ite ;   //!
   TBranch        *b_eJetGen_ite  ;   //!
   TBranch        *b_etaJetGen_ite;   //!
   TBranch        *b_phiJetGen_ite;   //!
   TBranch        *b_nJetGen_kt4;   //!
   TBranch        *b_pxJetGen_kt4 ;   //!
   TBranch        *b_pyJetGen_kt4 ;   //!
   TBranch        *b_pzJetGen_kt4 ;   //!
   TBranch        *b_eJetGen_kt4  ;   //!
   TBranch        *b_etaJetGen_kt4;   //!
   TBranch        *b_phiJetGen_kt4;   //!
   TBranch        *b_nJetGen_kt6;   //!
   TBranch        *b_pxJetGen_kt6 ;   //!
   TBranch        *b_pyJetGen_kt6 ;   //!
   TBranch        *b_pzJetGen_kt6 ;   //!
   TBranch        *b_eJetGen_kt6  ;   //!
   TBranch        *b_etaJetGen_kt6;   //!
   TBranch        *b_phiJetGen_kt6;   //!
   TBranch        *b_nJetGen_sis5;   //!
   TBranch        *b_pxJetGen_sis5 ;   //!
   TBranch        *b_pyJetGen_sis5 ;   //!
   TBranch        *b_pzJetGen_sis5 ;   //!
   TBranch        *b_eJetGen_sis5  ;   //!
   TBranch        *b_etaJetGen_sis5;   //!
   TBranch        *b_phiJetGen_sis5;   //!
   TBranch        *b_nJetGen_sis7;   //!
   TBranch        *b_pxJetGen_sis7 ;   //!
   TBranch        *b_pyJetGen_sis7 ;   //!
   TBranch        *b_pzJetGen_sis7 ;   //!
   TBranch        *b_eJetGen_sis7  ;   //!
   TBranch        *b_etaJetGen_sis7;   //!
   TBranch        *b_phiJetGen_sis7;   //!
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
   TBranch        *b_hltCount;   //!
   TBranch        *b_hltNamesLen;   //!
   TBranch        *b_HLTNames;   //!
   TBranch        *b_HLTResults;   //!

   TH1D*    allptphot;
   TH1D*    allptjet;
   TH1D*    alldeltaphi;
   TH1D*    alldeltar;
   TH1D*    alldeltae;
   TH2D*    alldeltardeltae;

   TH1D*    mcptphot;
   TH1D*    mcetaphot;
   TH1D*    mcidphot;
   TH1D*    mcdeltaphi;
   TH1D*    mcdeltapt;
   TH1D*    mcptjet;
   TH1D*    mcetajet;
   TH1D*    mcidjet;

   TH1D*    gendeltaphi;
   TH1D*    gendeltapt;
   TH1D*    genmcdeltaphi;
   TH1D*    genmcdeltapt;
   TH1D*    genptjet;
   TH1D*    genetajet;

   TH1D*    leadptphot;
   TH1D*    leadetaphot;
   TH1D*    leaddeltaphi;
   TH1D*    leadptjet;
   TH1D*    leadetajet;

   TH1D*    secptphot;
   TH1D*    secetaphot;
   TH1D*    secdeltaphiphot;
   TH1D*    secptjet;
   TH1D*    secetajet;
   TH1D*    secdeltaphijet;

   TH1D*    matchdeltar;
   TH1D*    matchdeltae;

   TH1D *allevents; 

   TH1D *pt_true; 
   TH1D *pt_true_pt; 
   TH1D *pt_true_eta; 
   TH1D *pt_true_ntrk; 
   TH1D *pt_true_ptiso; 
   TH1D *pt_true_emf; 
   TH1D *pt_true_smaj; 
   TH1D *pt_true_smin; 
   TH1D *pt_true_ecaliso; 
   TH1D *pt_true_etajet; 
   TH1D *pt_true_deltaphi; 
   TH1D *pt_true_2ndjet; 
   TH1D *pt_true_deltaeta; 
   
   TH1D *pt_true_eta_int; 
   TH1D *pt_true_ntrk_int; 
   TH1D *pt_true_ptiso_int; 
   TH1D *pt_true_emf_int; 
   TH1D *pt_true_smaj_int; 
   TH1D *pt_true_smin_int; 
   TH1D *pt_true_ecaliso_int; 
   TH1D *pt_true_etajet_int; 
   TH1D *pt_true_deltaphi_int; 
   TH1D *pt_true_2ndjet_int; 
   TH1D *pt_true_deltaeta_int;   
   
   TH1D *eta_true; 
   TH1D *eta_true_pt; 
   TH1D *eta_true_eta; 
   TH1D *eta_true_ntrk; 
   TH1D *eta_true_ptiso; 
   TH1D *eta_true_emf; 
   TH1D *eta_true_smaj; 
   TH1D *eta_true_smin; 
   TH1D *eta_true_ecaliso; 
   TH1D *eta_true_etajet; 
   TH1D *eta_true_deltaphi; 
   TH1D *eta_true_2ndjet; 
   TH1D *eta_true_deltaeta; 
   
   TH1D *eta_true_eta_int; 
   TH1D *eta_true_ntrk_int; 
   TH1D *eta_true_ptiso_int; 
   TH1D *eta_true_emf_int; 
   TH1D *eta_true_smaj_int; 
   TH1D *eta_true_smin_int; 
   TH1D *eta_true_ecaliso_int; 
   TH1D *eta_true_etajet_int; 
   TH1D *eta_true_deltaphi_int; 
   TH1D *eta_true_2ndjet_int; 
   TH1D *eta_true_deltaeta_int;   
   
   photoncandidate phot;
   jetcandidate jet;

   // Configuration settings
   std::string _version;
   std::string _algo;
   bool _readall;
   //
   float _rcone;
   float _backtoback;
   //
   float _photetacut;
   float _photptcut;
   float _jetetacut;
   float _jet2_minpt;
   float _jet2_maxfrac;
   float _deltaphi;
   float _deltaeta;

   photonidcuts _looseid;
   photonidcuts _mediumid;
   photonidcuts _tightid;

   int irecphot;
   int irecphoteff;
   int isubjet; // substitute photon (reco jet)
   int isubphot; // substitute photon (parton jet)
   int i2ndphot;
   int igenphot;
   int imcphot;

   int irecjet;
   int i2ndjet;
   int igenjet;
   int imcjet;

   Bool_t matched;
   Float_t pt2ndjet;
   Float_t pt2ndphot;

   Int_t isgammas;
   Int_t ntrkisos;
   Float_t pts;
   Float_t ptisos;
   Float_t ptisoatecals;
   Float_t hcalovecals;
   Float_t sMajMajs;
   Float_t sMinMins;
   Float_t ecalisos;
   Float_t weights;

   Int_t ntrkisob;
   Float_t ptb;
   Float_t ptisob;
   Float_t ptisoatecalb;
   Float_t hcalovecalb;
   Float_t sMajMajb;
   Float_t sMinMinb;
   Float_t ecalisob;
   Float_t weightb;

   Int_t photonid;
   Int_t isgamma;
   Int_t ismatched;
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
   Float_t pt2sum;
   Float_t pt2vecsum;
   Float_t pt2phot;
   Float_t ptphottrue;
   Float_t ptjettrue;
   Float_t ptquarktrue;
   Float_t phiphottrue;
   Float_t phijettrue;
   Float_t phiquarktrue;


   TFile* hOutputFile ;
   TTree * S_tree ;
   TTree * ana_tree ;

   int npb;

   gjetefficiency(TTree *tree=0, char * outputname = "test.root");
   virtual ~gjetefficiency();
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop(double cross = 1., double genpt_start = 0.,
			 double genpt_limit = 14000.,
			 bool isgamma = true, int NEVT = 10000000);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
   
   // Load configuration from the file determined by 'CAFE_CONFIG'
   // Set the environment variable 'CAFE_CONFIG' like this:
   //   gSystem->Setenv("CAFE_CONFIG", "gjetefficiency.config");
   // Input parameter "tree" gives the name of the configuration.
   // Set the parameters like this:
   //   tree.DeltaPhi: 3.0
   //   tree.DeltaEta: 1.0
   // Remember to load the Config class before gjetefficiency.C:
   //   gROOT->ProcessLine(".L Config.cpp++");
   void Configure(const char* cfg = "tree");
   void DumpConfig();
   void BookHistos();

   //  calculate phi1-phi2 keeping value between 0 and pi
   inline float delta_phi(float phi1, float phi2);
   inline float delta_eta(float eta1, float eta2);
   //bool looseID(int i);
   //bool mediumID(int i);
   //bool tightID(int i);
   bool cutID(int i, photonidcuts const& pid);
   bool NNID(int i, std::vector<IClassifierReader*> const& classreaders);

};

#endif

#ifdef gjetefficiency_cxx
gjetefficiency::gjetefficiency(TTree *tree, char * outputname)
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

   Configure();
   DumpConfig();
}

gjetefficiency::~gjetefficiency()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
   S_tree->Write();
   ana_tree->Write();
   hOutputFile->Write() ;
   hOutputFile->Close() ;
   hOutputFile->Delete();
}

Int_t gjetefficiency::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}

Long64_t gjetefficiency::LoadTree(Long64_t entry)
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

void gjetefficiency::Init(TTree *tree)
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
   fChain->SetBranchAddress("genpt", &genpt, &b_genpt);
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
   fChain->SetBranchAddress("escPhot  ", escPhot  , &b_escPhot  );
   fChain->SetBranchAddress("etaPhot", etaPhot, &b_etaPhot);
   fChain->SetBranchAddress("phiPhot", phiPhot, &b_phiPhot);
   fChain->SetBranchAddress("nconvPhot", &nconvPhot, &b_nconvPhot);
   fChain->SetBranchAddress("chi2convPhot", &chi2convPhot, &b_chi2convPhot);
   fChain->SetBranchAddress("ndofconvPhot", &ndofconvPhot, &b_ndofconvPhot);
   fChain->SetBranchAddress("xconvPhot", &xconvPhot, &b_xconvPhot);
   fChain->SetBranchAddress("yconvPhot", &yconvPhot, &b_yconvPhot);
   fChain->SetBranchAddress("zconvPhot", &zconvPhot, &b_zconvPhot);
   fChain->SetBranchAddress("ntrkconvPhot", &ntrkconvPhot, &b_ntrkconvPhot);
   fChain->SetBranchAddress("eovpconvPhot", &eovpconvPhot, &b_eovpconvPhot);
   fChain->SetBranchAddress("etaecalconvPhot", &etaecalconvPhot, &b_etaecalconvPhot);
   fChain->SetBranchAddress("phiecalconvPhot", &phiecalconvPhot, &b_phiecalconvPhot);
   fChain->SetBranchAddress("eecalconvPhot", &eecalconvPhot, &b_eecalconvPhot);
   fChain->SetBranchAddress("ptiso0015Phot", ptiso0015Phot, &b_ptiso0015Phot);
   fChain->SetBranchAddress("ntrkiso0015Phot", ntrkiso0015Phot, &b_ntrkiso0015Phot);
   fChain->SetBranchAddress("ptiso035Phot", ptiso035Phot, &b_ptiso035Phot);
   fChain->SetBranchAddress("ntrkiso035Phot", ntrkiso035Phot, &b_ntrkiso035Phot);
   fChain->SetBranchAddress("ptiso05Phot", ptiso05Phot, &b_ptiso05Phot);
   fChain->SetBranchAddress("ntrkiso05Phot", ntrkiso05Phot, &b_ntrkiso05Phot);
   fChain->SetBranchAddress("ptiso07Phot", ptiso07Phot, &b_ptiso07Phot);
   fChain->SetBranchAddress("ntrkiso07Phot", ntrkiso07Phot, &b_ntrkiso07Phot);
   fChain->SetBranchAddress("ptiso1Phot", ptiso1Phot, &b_ptiso1Phot);
   fChain->SetBranchAddress("ntrkiso1Phot", ntrkiso1Phot, &b_ntrkiso1Phot);
   fChain->SetBranchAddress("hcalovecal01Phot", hcalovecal01Phot, &b_hcalovecal01Phot);
   fChain->SetBranchAddress("hcalovecal015Phot", hcalovecal015Phot, &b_hcalovecal015Phot);
   fChain->SetBranchAddress("hcalovecal04Phot", hcalovecal04Phot, &b_hcalovecal04Phot);
   fChain->SetBranchAddress("hcalovecal05Phot", hcalovecal05Phot, &b_hcalovecal05Phot);
   fChain->SetBranchAddress("hcalovecal07Phot", hcalovecal07Phot, &b_hcalovecal07Phot);
   fChain->SetBranchAddress("hcalovecal1Phot", hcalovecal1Phot, &b_hcalovecal1Phot);
   fChain->SetBranchAddress("ecaliso01Phot", ecaliso01Phot, &b_ecaliso01Phot);
   fChain->SetBranchAddress("ecaliso015Phot", ecaliso015Phot, &b_ecaliso015Phot);
   fChain->SetBranchAddress("ecaliso04Phot", ecaliso04Phot, &b_ecaliso04Phot);
   fChain->SetBranchAddress("ecaliso05Phot", ecaliso05Phot, &b_ecaliso05Phot);
   fChain->SetBranchAddress("ecaliso07Phot", ecaliso07Phot, &b_ecaliso07Phot);
   fChain->SetBranchAddress("ecaliso1Phot", ecaliso1Phot, &b_ecaliso1Phot);
   fChain->SetBranchAddress("ecaliso04oldPhot", ecaliso04oldPhot, &b_ecaliso04oldPhot);
   fChain->SetBranchAddress("LATPhot", LATPhot, &b_LATPhot);
   fChain->SetBranchAddress("sMajMajPhot", sMajMajPhot, &b_sMajMajPhot);
   fChain->SetBranchAddress("sMinMinPhot", sMinMinPhot, &b_sMinMinPhot);
   fChain->SetBranchAddress("alphaPhot", alphaPhot, &b_alphaPhot);
   fChain->SetBranchAddress("sEtaEtaPhot", sEtaEtaPhot, &b_sEtaEtaPhot);
   fChain->SetBranchAddress("sPhiPhiPhot", sPhiPhiPhot, &b_sPhiPhiPhot);
   fChain->SetBranchAddress("E1Phot", E1Phot, &b_E1Phot);
   fChain->SetBranchAddress("E9Phot", E9Phot, &b_E9Phot);
   fChain->SetBranchAddress("E25Phot", E25Phot, &b_E25Phot);
   fChain->SetBranchAddress("FisherPhot", FisherPhot, &b_FisherPhot);
   fChain->SetBranchAddress("sMajMajoldPhot", sMajMajoldPhot, &b_sMajMajoldPhot);
   fChain->SetBranchAddress("sMinMinoldPhot", sMinMinoldPhot, &b_sMinMinoldPhot);
   fChain->SetBranchAddress("nJet_ite", &nJet_ite, &b_nJet_ite);
   fChain->SetBranchAddress("pxJet_ite ", pxJet_ite , &b_pxJet_ite );
   fChain->SetBranchAddress("pyJet_ite ", pyJet_ite , &b_pyJet_ite );
   fChain->SetBranchAddress("pzJet_ite ", pzJet_ite , &b_pzJet_ite );
   fChain->SetBranchAddress("eJet_ite  ", eJet_ite  , &b_eJet_ite  );
   fChain->SetBranchAddress("etaJet_ite", etaJet_ite, &b_etaJet_ite);
   fChain->SetBranchAddress("phiJet_ite", phiJet_ite, &b_phiJet_ite);
   fChain->SetBranchAddress("nJet_kt4", &nJet_kt4, &b_nJet_kt4);
   fChain->SetBranchAddress("pxJet_kt4 ", pxJet_kt4 , &b_pxJet_kt4 );
   fChain->SetBranchAddress("pyJet_kt4 ", pyJet_kt4 , &b_pyJet_kt4 );
   fChain->SetBranchAddress("pzJet_kt4 ", pzJet_kt4 , &b_pzJet_kt4 );
   fChain->SetBranchAddress("eJet_kt4  ", eJet_kt4  , &b_eJet_kt4  );
   fChain->SetBranchAddress("etaJet_kt4", etaJet_kt4, &b_etaJet_kt4);
   fChain->SetBranchAddress("phiJet_kt4", phiJet_kt4, &b_phiJet_kt4);
   fChain->SetBranchAddress("nJet_kt6", &nJet_kt6, &b_nJet_kt6);
   fChain->SetBranchAddress("pxJet_kt6 ", pxJet_kt6 , &b_pxJet_kt6 );
   fChain->SetBranchAddress("pyJet_kt6 ", pyJet_kt6 , &b_pyJet_kt6 );
   fChain->SetBranchAddress("pzJet_kt6 ", pzJet_kt6 , &b_pzJet_kt6 );
   fChain->SetBranchAddress("eJet_kt6  ", eJet_kt6  , &b_eJet_kt6  );
   fChain->SetBranchAddress("etaJet_kt6", etaJet_kt6, &b_etaJet_kt6);
   fChain->SetBranchAddress("phiJet_kt6", phiJet_kt6, &b_phiJet_kt6);
   fChain->SetBranchAddress("nJet_sis5", &nJet_sis5, &b_nJet_sis5);
   fChain->SetBranchAddress("pxJet_sis5 ", pxJet_sis5 , &b_pxJet_sis5 );
   fChain->SetBranchAddress("pyJet_sis5 ", pyJet_sis5 , &b_pyJet_sis5 );
   fChain->SetBranchAddress("pzJet_sis5 ", pzJet_sis5 , &b_pzJet_sis5 );
   fChain->SetBranchAddress("eJet_sis5  ", eJet_sis5  , &b_eJet_sis5  );
   fChain->SetBranchAddress("etaJet_sis5", etaJet_sis5, &b_etaJet_sis5);
   fChain->SetBranchAddress("phiJet_sis5", phiJet_sis5, &b_phiJet_sis5);
   fChain->SetBranchAddress("nJet_sis7", &nJet_sis7, &b_nJet_sis7);
   fChain->SetBranchAddress("pxJet_sis7 ", pxJet_sis7 , &b_pxJet_sis7 );
   fChain->SetBranchAddress("pyJet_sis7 ", pyJet_sis7 , &b_pyJet_sis7 );
   fChain->SetBranchAddress("pzJet_sis7 ", pzJet_sis7 , &b_pzJet_sis7 );
   fChain->SetBranchAddress("eJet_sis7  ", eJet_sis7  , &b_eJet_sis7  );
   fChain->SetBranchAddress("etaJet_sis7", etaJet_sis7, &b_etaJet_sis7);
   fChain->SetBranchAddress("phiJet_sis7", phiJet_sis7, &b_phiJet_sis7);
   fChain->SetBranchAddress("nJet_pfite", &nJet_pfite, &b_nJet_pfite);
   fChain->SetBranchAddress("pxJet_pfite ", pxJet_pfite , &b_pxJet_pfite );
   fChain->SetBranchAddress("pyJet_pfite ", pyJet_pfite , &b_pyJet_pfite );
   fChain->SetBranchAddress("pzJet_pfite ", pzJet_pfite , &b_pzJet_pfite );
   fChain->SetBranchAddress("eJet_pfite  ", eJet_pfite  , &b_eJet_pfite  );
   fChain->SetBranchAddress("etaJet_pfite", etaJet_pfite, &b_etaJet_pfite);
   fChain->SetBranchAddress("phiJet_pfite", phiJet_pfite, &b_phiJet_pfite);
   fChain->SetBranchAddress("nJetGen_ite", &nJetGen_ite, &b_nJetGen_ite);
   fChain->SetBranchAddress("pxJetGen_ite ", pxJetGen_ite , &b_pxJetGen_ite );
   fChain->SetBranchAddress("pyJetGen_ite ", pyJetGen_ite , &b_pyJetGen_ite );
   fChain->SetBranchAddress("pzJetGen_ite ", pzJetGen_ite , &b_pzJetGen_ite );
   fChain->SetBranchAddress("eJetGen_ite  ", eJetGen_ite  , &b_eJetGen_ite  );
   fChain->SetBranchAddress("etaJetGen_ite", etaJetGen_ite, &b_etaJetGen_ite);
   fChain->SetBranchAddress("phiJetGen_ite", phiJetGen_ite, &b_phiJetGen_ite);
   fChain->SetBranchAddress("nJetGen_kt4", &nJetGen_kt4, &b_nJetGen_kt4);
   fChain->SetBranchAddress("pxJetGen_kt4 ", pxJetGen_kt4 , &b_pxJetGen_kt4 );
   fChain->SetBranchAddress("pyJetGen_kt4 ", pyJetGen_kt4 , &b_pyJetGen_kt4 );
   fChain->SetBranchAddress("pzJetGen_kt4 ", pzJetGen_kt4 , &b_pzJetGen_kt4 );
   fChain->SetBranchAddress("eJetGen_kt4  ", eJetGen_kt4  , &b_eJetGen_kt4  );
   fChain->SetBranchAddress("etaJetGen_kt4", etaJetGen_kt4, &b_etaJetGen_kt4);
   fChain->SetBranchAddress("phiJetGen_kt4", phiJetGen_kt4, &b_phiJetGen_kt4);
   fChain->SetBranchAddress("nJetGen_kt6", &nJetGen_kt6, &b_nJetGen_kt6);
   fChain->SetBranchAddress("pxJetGen_kt6 ", pxJetGen_kt6 , &b_pxJetGen_kt6 );
   fChain->SetBranchAddress("pyJetGen_kt6 ", pyJetGen_kt6 , &b_pyJetGen_kt6 );
   fChain->SetBranchAddress("pzJetGen_kt6 ", pzJetGen_kt6 , &b_pzJetGen_kt6 );
   fChain->SetBranchAddress("eJetGen_kt6  ", eJetGen_kt6  , &b_eJetGen_kt6  );
   fChain->SetBranchAddress("etaJetGen_kt6", etaJetGen_kt6, &b_etaJetGen_kt6);
   fChain->SetBranchAddress("phiJetGen_kt6", phiJetGen_kt6, &b_phiJetGen_kt6);
   fChain->SetBranchAddress("nJetGen_sis5", &nJetGen_sis5, &b_nJetGen_sis5);
   fChain->SetBranchAddress("pxJetGen_sis5 ", pxJetGen_sis5 , &b_pxJetGen_sis5 );
   fChain->SetBranchAddress("pyJetGen_sis5 ", pyJetGen_sis5 , &b_pyJetGen_sis5 );
   fChain->SetBranchAddress("pzJetGen_sis5 ", pzJetGen_sis5 , &b_pzJetGen_sis5 );
   fChain->SetBranchAddress("eJetGen_sis5  ", eJetGen_sis5  , &b_eJetGen_sis5  );
   fChain->SetBranchAddress("etaJetGen_sis5", etaJetGen_sis5, &b_etaJetGen_sis5);
   fChain->SetBranchAddress("phiJetGen_sis5", phiJetGen_sis5, &b_phiJetGen_sis5);
   fChain->SetBranchAddress("nJetGen_sis7", &nJetGen_sis7, &b_nJetGen_sis7);
   fChain->SetBranchAddress("pxJetGen_sis7 ", pxJetGen_sis7 , &b_pxJetGen_sis7 );
   fChain->SetBranchAddress("pyJetGen_sis7 ", pyJetGen_sis7 , &b_pyJetGen_sis7 );
   fChain->SetBranchAddress("pzJetGen_sis7 ", pzJetGen_sis7 , &b_pzJetGen_sis7 );
   fChain->SetBranchAddress("eJetGen_sis7  ", eJetGen_sis7  , &b_eJetGen_sis7  );
   fChain->SetBranchAddress("etaJetGen_sis7", etaJetGen_sis7, &b_etaJetGen_sis7);
   fChain->SetBranchAddress("phiJetGen_sis7", phiJetGen_sis7, &b_phiJetGen_sis7);
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
   fChain->SetBranchAddress("hltCount", &hltCount, &b_hltCount);
   fChain->SetBranchAddress("hltNamesLen", &hltNamesLen, &b_hltNamesLen);
   fChain->SetBranchAddress("HLTNames", HLTNames, &b_HLTNames);
   fChain->SetBranchAddress("HLTResults", HLTResults, &b_HLTResults);
   Notify();

   npb = 1000;

}

Bool_t gjetefficiency::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normaly not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void gjetefficiency::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}

#endif // #ifdef gjetefficiency_cxx
