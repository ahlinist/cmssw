//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Feb 13 15:40:52 2009 by ROOT version 5.18/00a
// from TChain pippo/pippo
// using 'chain.MakeClass("test")'
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

#include <TMVAPhotons_MLP.class.C>
#include <TMVAPhotons_1_MLP.class.C>
#include <TMVAPhotons_2_MLP.class.C>
#include <TMVAPhotons_3_MLP.class.C>
#include <TMVAPhotons_4_MLP.class.C>

#include "Config.hpp"

#include <map>
#include <string>
#include <fstream>

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
  double emf;
};

// Container for PhotonID parameters
struct photonidcuts {
  int tracknb;
  float trackiso_rel;
  float trackiso_abs;
  float trackiso_log;
  float trackiso5;
  float trackiso7;
  float ecaliso_rel;
  float ecaliso_abs;
  float ecaliso_log;
  float hcaliso_rel;
  float hcaliso_abs;
  float hcaliso_log;
  float sminmin;
  float smajmaj;
  float hovere;
  float setaeta;
  float emf_rel; // 22X
  float emf_abs; // 22X
};

// Histograms for MC factorization of response (mk_mcfactors.C)
// and for plotting distributions (mk_plots.C)
class gjettree_histos {
  public :
    
  TH1D *pthat;

  // CODING: L=leading/S=second; R=reco/G=gen/P=parton; P=photon/J=jet
  TH1D *lrp_pt;
  TH1D *lrp_eta;
  TH1D *lrp_phi;

  TH1D *lrj_pt;
  TH1D *lrj_eta;
  TH1D *lrj_phi;

  TH1D *lgj_pt;
  TH1D *lgj_eta;
  TH1D *lgj_phi;

  TH1D *srp_pt;
  TH1D *srp_eta;
  TH1D *srp_phi;

  TH1D *srj_pt;
  TH1D *srj_eta;
  TH1D *srj_phi;

  // Jet flavor studies
  TH1D *lrj_flavor;
  TH2D *lrj_dptvg2;
  TH2D *lrj_dptvg2a;
  TH2D *lrj_dptvg2g;
  TH2D *lrj_dptvg2lq;
  TH2D *lrj_dptvg2cq;
  TH2D *lrj_dptvg2bq;
  TH2D *lrj_dptvg2tq;

  // Photon flavor studies
  TH1D *lrp_flavor;
  TH2D *lrp_dptvg2;
  TH2D *lrp_dptvg2a;
  TH2D *lrp_dptvg2g;
  TH2D *lrp_dptvg2lq;
  TH2D *lrp_dptvg2cq;
  TH2D *lrp_dptvg2bq;
  TH2D *lrp_dptvg2tq;
  TH1D *lrp_pid;

  // Studies on additional radiation
  TH1D *srj_ptsum;
  TH1D *srj_ptvecsum;
  TH1D *srj_ptsumop;
  TH1D *srj_ptvecsumop;

  // pthat=PtOverPtHat
  TH1D *lrp_pthat;
  TH1D *lgg_pthat;
  TH1D *lgp_pthat;
  TH1D *lgn_pthat;
  TH1D *lrj_pthat;
  TH1D *lgj_pthat;
  TH1D *lgq_pthat;
  TH1D *srp_pthat;
  TH1D *srj_pthat;

  // ptop=PtOverPtPhoton
  TH1D *lrj_ptop;
  TH1D *lgj_ptop;
  TH1D *lgq_ptop;
  TH1D *srp_ptop;
  TH1D *srj_ptop;
  // ptogg=PtOverPt(Gen)Gamma
  TH1D *lrj_ptogg;
  TH1D *lgj_ptogg;
  // dphivp=DeltaPhiVsPhoton
  TH1D *lrj_dphivp;
  TH1D *lgj_dphivp;
  TH1D *srp_dphivp;
  TH1D *srj_dphivp;
  // dphivp=DeltaRVsPhoton
  TH1D *srj_drvp;
  // dphivgg=DeltaPhiVs(Gen)Gamma
  TH1D *lrj_dphivgg;
  TH1D *lgj_dphivgg;
  // detap=DeltaEtaVsPhoton
  TH1D *lrj_detavp;
  TH1D *lgj_detavp;
  TH1D *srp_detavp;
  TH1D *srj_detavp;
  // detap=DeltaEtaVsGenGamma
  TH1D *lrj_detavgg;
  TH1D *lgj_detavgg;

  // dr=DeltaR vg=VsGenObject (GenJet, parton photon) vgg=GenGamma
  TH1D *lrp_devg;
  TH1D *lrp_dptvg;
  TH1D *lrp_drvg;
  TH1D *lrp_detavg;
  TH1D *lrp_dphivg;

  TH1D *lrp_devgg;
  TH1D *lrp_dptvgg;
  TH1D *lrp_drvgg;
  TH1D *lrp_detavgg;
  TH1D *lrp_dphivgg;

  TH1D *lrp_devn;
  TH1D *lrp_dptvn;
  TH1D *lrp_drvn;
  TH1D *lrp_detavn;
  TH1D *lrp_dphivn;

  TH1D *lgg_devg;
  TH1D *lgg_dptvg;
  TH1D *lgg_drvg;
  TH1D *lgg_detavg;
  TH1D *lgg_dphivg;

  TH1D *lgn_devg;
  TH1D *lgn_dptvg;
  TH1D *lgn_drvg;
  TH1D *lgn_detavg;
  TH1D *lgn_dphivg;


  TH1D *lrj_devg;
  TH1D *lrj_dptvg;
  TH1D *lrj_drvg;
  TH1D *lrj_detavg;
  TH1D *lrj_dphivg;

  TH1D *lrj_devq;
  TH1D *lrj_dptvq;
  TH1D *lrj_drvq;
  TH1D *lrj_detavq;
  TH1D *lrj_dphivq;

  TH1D *lgj_devq;
  TH1D *lgj_dptvq;
  TH1D *lgj_drvq;
  TH1D *lgj_detavq;
  TH1D *lgj_dphivq;

  // PhotonID histograms
  TH1D *lrp_ntrkiso;
  TH1D *lrp_ptiso;
  TH1D *lrp_ptiso5;
  TH1D *lrp_ptiso7;
  TH1D *lrp_ecaliso;
  TH1D *lrp_hcaliso;
  TH1D *lrp_sMajMaj;
  TH1D *lrp_sMinMin;
  TH1D *lrp_hovere;
  TH1D *lrp_setaeta;

  TH1D *srp_ntrkiso;
  TH1D *srp_ptiso;
  TH1D *srp_ptiso5;
  TH1D *srp_ptiso7;
  TH1D *srp_ecaliso;
  TH1D *srp_hcaliso;
  TH1D *srp_sMajMaj;
  TH1D *srp_sMinMin;
  TH1D *srp_hovere;
  TH1D *srp_setaeta;

  gjettree_histos();
  ~gjettree_histos();
  void Write();
};

// Histograms copied over from gjetefficiency.h
// Used for making factorized efficiency/rejection plots
class gjetefficiency_histos {

 public:
  TH2D *id_beforetopo;
  TH2D *id_aftertopo;
  TH2D *topo_beforeid;
  TH2D *topo_afterid;

  TH2D *ecid_beforetopo;
  TH2D *ecid_aftertopo;
  TH2D *ectopo_beforeid;
  TH2D *ectopo_afterid;

  gjetefficiency_histos();
  ~gjetefficiency_histos();
  void Write();
};

class gjettree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Float_t         genpt;
   Bool_t          isMC;
   Int_t           store;
   Int_t           lbn;
   Int_t           bx;
   Int_t           orbit;
   Int_t           run;
   Int_t           event;
   Int_t           nMC;
   Int_t           pdgIdMC[99];   //[nMC]
   Int_t           statusMC[99];   //[nMC]
   Float_t         ptMC [99];   //[nMC]
   Float_t         eMC  [99];   //[nMC]
   Float_t         etaMC[99];   //[nMC]
   Float_t         phiMC[99];   //[nMC]
   Int_t           nSIM;
   Int_t           pdgIdSIM[125];   //[nSIM]
   Int_t           statusSIM[125];   //[nSIM]
   Float_t         ptSIM [125];   //[nSIM]
   Float_t         eSIM  [125];   //[nSIM]
   Float_t         etaSIM[125];   //[nSIM]
   Float_t         phiSIM[125];   //[nSIM]
   Float_t         rSIM[125];   //[nSIM]
   Float_t         zSIM[125];   //[nSIM]
   Int_t           nPF;
   Int_t           pdgIdPF[55];   //[nPF]
   Float_t         ptPF [55];   //[nPF]
   Float_t         ePF  [55];   //[nPF]
   Float_t         etaPF[55];   //[nPF]
   Float_t         phiPF[55];   //[nPF]
   Int_t           nPhot;
   Float_t         ptPhot [5];   //[nPhot]
   Float_t         ePhot  [5];   //[nPhot]
   Float_t         escPhot  [5];   //[nPhot]
   Float_t         etaPhot[5];   //[nPhot]
   Float_t         phiPhot[5];   //[nPhot]
   Int_t           nconvPhot;
   Float_t         chi2convPhot[10];   //[nconvPhot]
   Float_t         ndofconvPhot[10];   //[nconvPhot]
   Float_t         rconvPhot[10];   //[nconvPhot]
   Float_t         phiconvPhot[10];   //[nconvPhot]
   Float_t         zconvPhot[10];   //[nconvPhot]
   Int_t           ntrkconvPhot[10];   //[nconvPhot]
   Float_t         eovpconvPhot[10];   //[nconvPhot]
   Float_t         etaecalconvPhot[10];   //[nconvPhot]
   Float_t         phiecalconvPhot[10];   //[nconvPhot]
   Float_t         eecalconvPhot[10];   //[nconvPhot]
   Int_t           algoconvPhot[10];   //[nconvPhot]
   Float_t         d0convPhot[10];   //[nconvPhot]
   Float_t         detaecalconvPhot[10];   //[nconvPhot]
   Float_t         dphiecalconvPhot[10];   //[nconvPhot]
   Float_t         dphivtxconvPhot[10];   //[nconvPhot]
   Float_t         pairsepconvPhot[10];   //[nconvPhot]
   Float_t         pairmassconvPhot[10];   //[nconvPhot]
   Float_t         trchi21convPhot[10];   //[nconvPhot]
   Float_t         trndof1convPhot[10];   //[nconvPhot]
   Int_t           trqual1convPhot[10];   //[nconvPhot]
   Float_t         trpt1convPhot[10];   //[nconvPhot]
   Float_t         trerr1convPhot[10];   //[nconvPhot]
   Float_t         trchi22convPhot[10];   //[nconvPhot]
   Float_t         trndof2convPhot[10];   //[nconvPhot]
   Int_t           trqual2convPhot[10];   //[nconvPhot]
   Float_t         trpt2convPhot[10];   //[nconvPhot]
   Float_t         trerr2convPhot[10];   //[nconvPhot]
   Bool_t          pid_isEM[5];   //[nPhot]
   Bool_t          pid_isLoose[5];   //[nPhot]
   Bool_t          pid_isTight[5];   //[nPhot]
   Float_t         pid_jurECAL[5];   //[nPhot]
   Float_t         pid_twrHCAL[5];   //[nPhot]
   Float_t         pid_HoverE[5];   //[nPhot]
   Float_t         pid_hlwTrack[5];   //[nPhot]
   Float_t         pid_etawid[5];   //[nPhot]
   Float_t         ptiso0015Phot[5];   //[nPhot]
   Int_t           ntrkiso0015Phot[5];   //[nPhot]
   Float_t         ptiso035Phot[5];   //[nPhot]
   Int_t           ntrkiso035Phot[5];   //[nPhot]
   Float_t         ptiso04Phot[5];   //[nPhot]
   Int_t           ntrkiso04Phot[5];   //[nPhot]
   Float_t         ptiso05Phot[5];   //[nPhot]
   Int_t           ntrkiso05Phot[5];   //[nPhot]
   Float_t         ptiso07Phot[5];   //[nPhot]
   Int_t           ntrkiso07Phot[5];   //[nPhot]
   Float_t         ptiso1Phot[5];   //[nPhot]
   Int_t           ntrkiso1Phot[5];   //[nPhot]
   Float_t         hcalovecal01Phot[5];   //[nPhot]
   Float_t         hcalovecal015Phot[5];   //[nPhot]
   Float_t         hcalovecal04Phot[5];   //[nPhot]
   Float_t         hcalovecal05Phot[5];   //[nPhot]
   Float_t         hcalovecal07Phot[5];   //[nPhot]
   Float_t         hcalovecal1Phot[5];   //[nPhot]
   Float_t         ecaliso01Phot[5];   //[nPhot]
   Float_t         ecaliso015Phot[5];   //[nPhot]
   Float_t         ecaliso04Phot[5];   //[nPhot]
   Float_t         ecaliso05Phot[5];   //[nPhot]
   Float_t         ecaliso07Phot[5];   //[nPhot]
   Float_t         ecaliso1Phot[5];   //[nPhot]
   Float_t         LATPhot[5];   //[nPhot]
   Float_t         sMajMajPhot[5];   //[nPhot]
   Float_t         sMinMinPhot[5];   //[nPhot]
   Float_t         alphaPhot[5];   //[nPhot]
   Float_t         sEtaEtaPhot[5];   //[nPhot]
   Float_t         sPhiPhiPhot[5];   //[nPhot]
   Float_t         E1Phot[5];   //[nPhot]
   Float_t         E9Phot[5];   //[nPhot]
   Float_t         E25Phot[5];   //[nPhot]
   Float_t         FisherPhot[5];   //[nPhot]
   Int_t           nJet_ite;
   Float_t         ptJet_ite [18];   //[nJet_ite]
   Float_t         eJet_ite  [18];   //[nJet_ite]
   Float_t         etaJet_ite[18];   //[nJet_ite]
   Float_t         phiJet_ite[18];   //[nJet_ite]
   Float_t         emfJet_ite[18];   //[nJet_ite]
   Int_t           nJet_kt4;
   Float_t         ptJet_kt4 [19];   //[nJet_kt4]
   Float_t         eJet_kt4  [19];   //[nJet_kt4]
   Float_t         etaJet_kt4[19];   //[nJet_kt4]
   Float_t         phiJet_kt4[19];   //[nJet_kt4]
   Float_t         emfJet_kt4[19];   //[nJet_kt4]
   Int_t           nJet_kt6;
   Float_t         ptJet_kt6 [20];   //[nJet_kt6]
   Float_t         eJet_kt6  [20];   //[nJet_kt6]
   Float_t         etaJet_kt6[20];   //[nJet_kt6]
   Float_t         phiJet_kt6[20];   //[nJet_kt6]
   Float_t         emfJet_kt6[20];   //[nJet_kt6]
   Int_t           nJet_akt5;
   Float_t         ptJet_akt5 [19];   //[nJet_akt5]
   Float_t         eJet_akt5  [19];   //[nJet_akt5]
   Float_t         etaJet_akt5[19];   //[nJet_akt5]
   Float_t         phiJet_akt5[19];   //[nJet_akt5]
   Float_t         emfJet_akt5[19];   //[nJet_akt5]
   Int_t           nJet_sis5;
   Float_t         ptJet_sis5 [20];   //[nJet_sis5]
   Float_t         eJet_sis5  [20];   //[nJet_sis5]
   Float_t         etaJet_sis5[20];   //[nJet_sis5]
   Float_t         phiJet_sis5[20];   //[nJet_sis5]
   Float_t         emfJet_sis5[20];   //[nJet_sis5]
   Int_t           nJet_sis7;
   Float_t         ptJet_sis7 [20];   //[nJet_sis7]
   Float_t         eJet_sis7  [20];   //[nJet_sis7]
   Float_t         etaJet_sis7[20];   //[nJet_sis7]
   Float_t         phiJet_sis7[20];   //[nJet_sis7]
   Float_t         emfJet_sis7[20];   //[nJet_sis7]
   Int_t           nJet_pfite;
   Float_t         ptJet_pfite [23];   //[nJet_pfite]
   Float_t         eJet_pfite  [23];   //[nJet_pfite]
   Float_t         etaJet_pfite[23];   //[nJet_pfite]
   Float_t         phiJet_pfite[23];   //[nJet_pfite]
   Int_t           nJet_pfkt4;
   Float_t         ptJet_pfkt4 [24];   //[nJet_pfkt4]
   Float_t         eJet_pfkt4  [24];   //[nJet_pfkt4]
   Float_t         etaJet_pfkt4[24];   //[nJet_pfkt4]
   Float_t         phiJet_pfkt4[24];   //[nJet_pfkt4]
   Int_t           nJet_pfakt5;
   Float_t         ptJet_pfakt5 [24];   //[nJet_pfakt5]
   Float_t         eJet_pfakt5  [24];   //[nJet_pfakt5]
   Float_t         etaJet_pfakt5[24];   //[nJet_pfakt5]
   Float_t         phiJet_pfakt5[24];   //[nJet_pfakt5]
   Int_t           nChargedHadrons_pfakt5[24];   //[nJet_pfakt5]
   Int_t           nPhotons_pfakt5[24];   //[nJet_pfakt5]
   Int_t           nMuons_pfakt5[24];   //[nJet_pfakt5]
   Int_t           nElectrons_pfakt5[24];   //[nJet_pfakt5]
   Int_t           nNeutralHadrons_pfakt5[24];   //[nJet_pfakt5]
   Int_t           nHFHadrons_pfakt5[24];   //[nJet_pfakt5]
   Int_t           nHFEM_pfakt5[24];   //[nJet_pfakt5]
   Float_t         eChargedHadrons_pfakt5[24];   //[nJet_pfakt5]
   Float_t         ePhotons_pfakt5[24];   //[nJet_pfakt5]
   Float_t         eMuons_pfakt5[24];   //[nJet_pfakt5]
   Float_t         eElectrons_pfakt5[24];   //[nJet_pfakt5]
   Float_t         eNeutralHadrons_pfakt5[24];   //[nJet_pfakt5]
   Float_t         eHFHadrons_pfakt5[24];   //[nJet_pfakt5]
   Float_t         eHFEM_pfakt5[24];   //[nJet_pfakt5]
   Float_t         ptChargedHadrons_pfakt5[24];   //[nJet_pfakt5]
   Float_t         ptPhotons_pfakt5[24];   //[nJet_pfakt5]
   Float_t         ptMuons_pfakt5[24];   //[nJet_pfakt5]
   Float_t         ptElectrons_pfakt5[24];   //[nJet_pfakt5]
   Float_t         ptNeutralHadrons_pfakt5[24];   //[nJet_pfakt5]
   Float_t         ptHFHadrons_pfakt5[24];   //[nJet_pfakt5]
   Float_t         ptHFEM_pfakt5[24];   //[nJet_pfakt5]
   Float_t         phiChargedHadrons_pfakt5[24];   //[nJet_pfakt5]
   Float_t         phiPhotons_pfakt5[24];   //[nJet_pfakt5]
   Float_t         phiMuons_pfakt5[24];   //[nJet_pfakt5]
   Float_t         phiElectrons_pfakt5[24];   //[nJet_pfakt5]
   Float_t         phiNeutralHadrons_pfakt5[24];   //[nJet_pfakt5]
   Float_t         phiHFHadrons_pfakt5[24];   //[nJet_pfakt5]
   Float_t         phiHFEM_pfakt5[24];   //[nJet_pfakt5]
   Float_t         etaChargedHadrons_pfakt5[24];   //[nJet_pfakt5]
   Float_t         etaPhotons_pfakt5[24];   //[nJet_pfakt5]
   Float_t         etaMuons_pfakt5[24];   //[nJet_pfakt5]
   Float_t         etaElectrons_pfakt5[24];   //[nJet_pfakt5]
   Float_t         etaNeutralHadrons_pfakt5[24];   //[nJet_pfakt5]
   Float_t         etaHFHadrons_pfakt5[24];   //[nJet_pfakt5]
   Float_t         etaHFEM_pfakt5[24];   //[nJet_pfakt5]
   Int_t           nJet_pfsis5;
   Float_t         ptJet_pfsis5 [22];   //[nJet_pfsis5]
   Float_t         eJet_pfsis5  [22];   //[nJet_pfsis5]
   Float_t         etaJet_pfsis5[22];   //[nJet_pfsis5]
   Float_t         phiJet_pfsis5[22];   //[nJet_pfsis5]
   Int_t           nJet_pfkt6;
   Float_t         ptJet_pfkt6 [26];   //[nJet_pfkt6]
   Float_t         eJet_pfkt6  [26];   //[nJet_pfkt6]
   Float_t         etaJet_pfkt6[26];   //[nJet_pfkt6]
   Float_t         phiJet_pfkt6[26];   //[nJet_pfkt6]
   Int_t           nJet_pfsis7;
   Float_t         ptJet_pfsis7 [25];   //[nJet_pfsis7]
   Float_t         eJet_pfsis7  [25];   //[nJet_pfsis7]
   Float_t         etaJet_pfsis7[25];   //[nJet_pfsis7]
   Float_t         phiJet_pfsis7[25];   //[nJet_pfsis7]
   Int_t           nJetGen_ite;
   Float_t         ptJetGen_ite [27];   //[nJetGen_ite]
   Float_t         eJetGen_ite  [27];   //[nJetGen_ite]
   Float_t         etaJetGen_ite[27];   //[nJetGen_ite]
   Float_t         phiJetGen_ite[27];   //[nJetGen_ite]
   Int_t           nJetGen_akt5;
   Float_t         ptJetGen_akt5 [25];   //[nJetGen_akt5]
   Float_t         eJetGen_akt5  [25];   //[nJetGen_akt5]
   Float_t         etaJetGen_akt5[25];   //[nJetGen_akt5]
   Float_t         phiJetGen_akt5[25];   //[nJetGen_akt5]
   Int_t           nMuonsGen_akt5[25];   //[nJetGen_akt5]
   Int_t           nElectronsGen_akt5[25];   //[nJetGen_akt5]
   Int_t           nPhotonsGen_akt5[25];   //[nJetGen_akt5]
   Int_t           nTracksGen_akt5[25];   //[nJetGen_akt5]
   Int_t           nNeutralHadronsGen_akt5[25];   //[nJetGen_akt5]
   Float_t         eMuonsGen_akt5[25];   //[nJetGen_akt5]
   Float_t         eElectronsGen_akt5[25];   //[nJetGen_akt5]
   Float_t         ePhotonsGen_akt5[25];   //[nJetGen_akt5]
   Float_t         eTracksGen_akt5[25];   //[nJetGen_akt5]
   Float_t         eNeutralHadronsGen_akt5[25];   //[nJetGen_akt5]
   Float_t         ptMuonsGen_akt5[25];   //[nJetGen_akt5]
   Float_t         ptElectronsGen_akt5[25];   //[nJetGen_akt5]
   Float_t         ptPhotonsGen_akt5[25];   //[nJetGen_akt5]
   Float_t         ptTracksGen_akt5[25];   //[nJetGen_akt5]
   Float_t         ptNeutralHadronsGen_akt5[25];   //[nJetGen_akt5]
   Float_t         phiMuonsGen_akt5[25];   //[nJetGen_akt5]
   Float_t         phiElectronsGen_akt5[25];   //[nJetGen_akt5]
   Float_t         phiPhotonsGen_akt5[25];   //[nJetGen_akt5]
   Float_t         phiTracksGen_akt5[25];   //[nJetGen_akt5]
   Float_t         phiNeutralHadronsGen_akt5[25];   //[nJetGen_akt5]
   Float_t         etaMuonsGen_akt5[25];   //[nJetGen_akt5]
   Float_t         etaElectronsGen_akt5[25];   //[nJetGen_akt5]
   Float_t         etaPhotonsGen_akt5[25];   //[nJetGen_akt5]
   Float_t         etaTracksGen_akt5[25];   //[nJetGen_akt5]
   Float_t         etaNeutralHadronsGen_akt5[25];   //[nJetGen_akt5]
   Int_t           nJetGen_kt4;
   Float_t         ptJetGen_kt4 [25];   //[nJetGen_kt4]
   Float_t         eJetGen_kt4  [25];   //[nJetGen_kt4]
   Float_t         etaJetGen_kt4[25];   //[nJetGen_kt4]
   Float_t         phiJetGen_kt4[25];   //[nJetGen_kt4]
   Int_t           nJetGen_kt6;
   Float_t         ptJetGen_kt6 [30];   //[nJetGen_kt6]
   Float_t         eJetGen_kt6  [30];   //[nJetGen_kt6]
   Float_t         etaJetGen_kt6[30];   //[nJetGen_kt6]
   Float_t         phiJetGen_kt6[30];   //[nJetGen_kt6]
   Int_t           nJetGen_sis5;
   Float_t         ptJetGen_sis5[28];   //[nJetGen_sis5]
   Float_t         eJetGen_sis5  [28];   //[nJetGen_sis5]
   Float_t         etaJetGen_sis5[28];   //[nJetGen_sis5]
   Float_t         phiJetGen_sis5[28];   //[nJetGen_sis5]
   Int_t           nJetGen_sis7;
   Float_t         ptJetGen_sis7 [27];   //[nJetGen_sis7]
   Float_t         eJetGen_sis7  [27];   //[nJetGen_sis7]
   Float_t         etaJetGen_sis7[27];   //[nJetGen_sis7]
   Float_t         phiJetGen_sis7[27];   //[nJetGen_sis7]
   Float_t         sMet  ;
   Float_t         eMet  ;
   Float_t         phiMet;
   Float_t         stcMet  ;
   Float_t         etcMet  ;
   Float_t         phitcMet;
   Float_t         spfMet  ;
   Float_t         epfMet  ;
   Float_t         phipfMet;
   Float_t         sMetGen  ;
   Float_t         eMetGen  ;
   Float_t         phiMetGen;
   Float_t         sMetGen2  ;
   Float_t         eMetGen2  ;
   Float_t         phiMetGen2;
   Bool_t          hltPass;
   Int_t           nHLT;
   Int_t           hltNamesLen;
   Char_t          HLTNames[1865];   //[hltNamesLen]
   Bool_t          HLTResults[8];   //[nHLT]

   // Switchable pointers to branch variables
   Int_t nJet;
   Float_t *ptJet;
   Float_t *eJet;
   Float_t *etaJet;
   Float_t *phiJet;

   Int_t nJetGen;
   Float_t *ptJetGen;
   Float_t *eJetGen;
   Float_t *etaJetGen;
   Float_t *phiJetGen;

   // List of branches
   TBranch        *b_genpt;   //!
   TBranch        *b_isMC;   //!
   TBranch        *b_store;   //!
   TBranch        *b_lbn;   //!
   TBranch        *b_bx;   //!
   TBranch        *b_orbit;   //!
   TBranch        *b_run;   //!
   TBranch        *b_event;   //!
   TBranch        *b_nMC;   //!
   TBranch        *b_pdgIdMC;   //!
   TBranch        *b_statusMC;   //!
   TBranch        *b_ptMC ;   //!
   TBranch        *b_eMC  ;   //!
   TBranch        *b_etaMC;   //!
   TBranch        *b_phiMC;   //!
   TBranch        *b_nSIM;   //!
   TBranch        *b_pdgIdSIM;   //!
   TBranch        *b_statusSIM;   //!
   TBranch        *b_ptSIM ;   //!
   TBranch        *b_eSIM  ;   //!
   TBranch        *b_etaSIM;   //!
   TBranch        *b_phiSIM;   //!
   TBranch        *b_rSIM;   //!
   TBranch        *b_zSIM;   //!
   TBranch        *b_nPF;   //!
   TBranch        *b_pdgIdPF;   //!
   TBranch        *b_ptPF ;   //!
   TBranch        *b_ePF  ;   //!
   TBranch        *b_etaPF;   //!
   TBranch        *b_phiPF;   //!
   TBranch        *b_nPhot;   //!
   TBranch        *b_ptPhot ;   //!
   TBranch        *b_ePhot  ;   //!
   TBranch        *b_escPhot  ;   //!
   TBranch        *b_etaPhot;   //!
   TBranch        *b_phiPhot;   //!
   TBranch        *b_nconvPhot;   //!
   TBranch        *b_chi2convPhot;   //!
   TBranch        *b_ndofconvPhot;   //!
   TBranch        *b_rconvPhot;   //!
   TBranch        *b_phiconvPhot;   //!
   TBranch        *b_zconvPhot;   //!
   TBranch        *b_ntrkconvPhot;   //!
   TBranch        *b_eovpconvPhot;   //!
   TBranch        *b_etaecalconvPhot;   //!
   TBranch        *b_phiecalconvPhot;   //!
   TBranch        *b_eecalconvPhot;   //!
   TBranch        *b_algoconvPhot;   //!
   TBranch        *b_d0convPhot;   //!
   TBranch        *b_detaecalconvPhot;   //!
   TBranch        *b_dphiecalconvPhot;   //!
   TBranch        *b_dphivtxconvPhot;   //!
   TBranch        *b_pairsepconvPhot;   //!
   TBranch        *b_pairmassconvPhot;   //!
   TBranch        *b_trchi21convPhot;   //!
   TBranch        *b_trndof1convPhot;   //!
   TBranch        *b_trqual1convPhot;   //!
   TBranch        *b_trpt1convPhot;   //!
   TBranch        *b_trerr1convPhot;   //!
   TBranch        *b_trchi22convPhot;   //!
   TBranch        *b_trndof2convPhot;   //!
   TBranch        *b_trqual2convPhot;   //!
   TBranch        *b_trpt2convPhot;   //!
   TBranch        *b_trerr2convPhot;   //!
   TBranch        *b_pid_isEM;   //!
   TBranch        *b_pid_isLoose;   //!
   TBranch        *b_pid_isTight;   //!
   TBranch        *b_pid_jurECAL;   //!
   TBranch        *b_pid_twrHCAL;   //!
   TBranch        *b_pid_HoverE;   //!
   TBranch        *b_pid_hlwTrack;   //!
   TBranch        *b_pid_etawid;   //!
   TBranch        *b_ptiso0015Phot;   //!
   TBranch        *b_ntrkiso0015Phot;   //!
   TBranch        *b_ptiso035Phot;   //!
   TBranch        *b_ntrkiso035Phot;   //!
   TBranch        *b_ptiso04Phot;   //!
   TBranch        *b_ntrkiso04Phot;   //!
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
   TBranch        *b_nJet_ite;   //!
   TBranch        *b_ptJet_ite ;   //!
   TBranch        *b_eJet_ite  ;   //!
   TBranch        *b_etaJet_ite;   //!
   TBranch        *b_phiJet_ite;   //!
   TBranch        *b_emfJet_ite;   //!
   TBranch        *b_nJet_kt4;   //!
   TBranch        *b_ptJet_kt4 ;   //!
   TBranch        *b_eJet_kt4  ;   //!
   TBranch        *b_etaJet_kt4;   //!
   TBranch        *b_phiJet_kt4;   //!
   TBranch        *b_emfJet_kt4;   //!
   TBranch        *b_nJet_kt6;   //!
   TBranch        *b_ptJet_kt6 ;   //!
   TBranch        *b_eJet_kt6  ;   //!
   TBranch        *b_etaJet_kt6;   //!
   TBranch        *b_phiJet_kt6;   //!
   TBranch        *b_emfJet_kt6;   //!
   TBranch        *b_nJet_akt5;   //!
   TBranch        *b_ptJet_akt5 ;   //!
   TBranch        *b_eJet_akt5  ;   //!
   TBranch        *b_etaJet_akt5;   //!
   TBranch        *b_phiJet_akt5;   //!
   TBranch        *b_emfJet_akt5;   //!
   TBranch        *b_nJet_sis5;   //!
   TBranch        *b_ptJet_sis5 ;   //!
   TBranch        *b_eJet_sis5  ;   //!
   TBranch        *b_etaJet_sis5;   //!
   TBranch        *b_phiJet_sis5;   //!
   TBranch        *b_emfJet_sis5;   //!
   TBranch        *b_nJet_sis7;   //!
   TBranch        *b_ptJet_sis7 ;   //!
   TBranch        *b_eJet_sis7  ;   //!
   TBranch        *b_etaJet_sis7;   //!
   TBranch        *b_phiJet_sis7;   //!
   TBranch        *b_emfJet_sis7;   //!
   TBranch        *b_nJet_pfite;   //!
   TBranch        *b_ptJet_pfite ;   //!
   TBranch        *b_eJet_pfite  ;   //!
   TBranch        *b_etaJet_pfite;   //!
   TBranch        *b_phiJet_pfite;   //!
   TBranch        *b_nJet_pfkt4;   //!
   TBranch        *b_ptJet_pfkt4 ;   //!
   TBranch        *b_eJet_pfkt4  ;   //!
   TBranch        *b_etaJet_pfkt4;   //!
   TBranch        *b_phiJet_pfkt4;   //!
   TBranch        *b_nJet_pfakt5;   //!
   TBranch        *b_ptJet_pfakt5 ;   //!
   TBranch        *b_eJet_pfakt5  ;   //!
   TBranch        *b_etaJet_pfakt5;   //!
   TBranch        *b_phiJet_pfakt5;   //!
   TBranch        *b_nChargedHadrons_pfakt5;   //!
   TBranch        *b_nPhotons_pfakt5;   //!
   TBranch        *b_nMuons_pfakt5;   //!
   TBranch        *b_nElectrons_pfakt5;   //!
   TBranch        *b_nNeutralHadrons_pfakt5;   //!
   TBranch        *b_nHFHadrons_pfakt5;   //!
   TBranch        *b_nHFEM_pfakt5;   //!
   TBranch        *b_eChargedHadrons_pfakt5;   //!
   TBranch        *b_ePhotons_pfakt5;   //!
   TBranch        *b_eMuons_pfakt5;   //!
   TBranch        *b_eElectrons_pfakt5;   //!
   TBranch        *b_eNeutralHadrons_pfakt5;   //!
   TBranch        *b_eHFHadrons_pfakt5;   //!
   TBranch        *b_eHFEM_pfakt5;   //!
   TBranch        *b_ptChargedHadrons_pfakt5;   //!
   TBranch        *b_ptPhotons_pfakt5;   //!
   TBranch        *b_ptMuons_pfakt5;   //!
   TBranch        *b_ptElectrons_pfakt5;   //!
   TBranch        *b_ptNeutralHadrons_pfakt5;   //!
   TBranch        *b_ptHFHadrons_pfakt5;   //!
   TBranch        *b_ptHFEM_pfakt5;   //!
   TBranch        *b_phiChargedHadrons_pfakt5;   //!
   TBranch        *b_phiPhotons_pfakt5;   //!
   TBranch        *b_phiMuons_pfakt5;   //!
   TBranch        *b_phiElectrons_pfakt5;   //!
   TBranch        *b_phiNeutralHadrons_pfakt5;   //!
   TBranch        *b_phiHFHadrons_pfakt5;   //!
   TBranch        *b_phiHFEM_pfakt5;   //!
   TBranch        *b_etaChargedHadrons_pfakt5;   //!
   TBranch        *b_etaPhotons_pfakt5;   //!
   TBranch        *b_etaMuons_pfakt5;   //!
   TBranch        *b_etaElectrons_pfakt5;   //!
   TBranch        *b_etaNeutralHadrons_pfakt5;   //!
   TBranch        *b_etaHFHadrons_pfakt5;   //!
   TBranch        *b_etaHFEM_pfakt5;   //!
   TBranch        *b_nJet_pfsis5;   //!
   TBranch        *b_ptJet_pfsis5 ;   //!
   TBranch        *b_eJet_pfsis5  ;   //!
   TBranch        *b_etaJet_pfsis5;   //!
   TBranch        *b_phiJet_pfsis5;   //!
   TBranch        *b_nJet_pfkt6;   //!
   TBranch        *b_ptJet_pfkt6 ;   //!
   TBranch        *b_eJet_pfkt6  ;   //!
   TBranch        *b_etaJet_pfkt6;   //!
   TBranch        *b_phiJet_pfkt6;   //!
   TBranch        *b_nJet_pfsis7;   //!
   TBranch        *b_ptJet_pfsis7 ;   //!
   TBranch        *b_eJet_pfsis7  ;   //!
   TBranch        *b_etaJet_pfsis7;   //!
   TBranch        *b_phiJet_pfsis7;   //!
   TBranch        *b_nJetGen_ite;   //!
   TBranch        *b_ptJetGen_ite ;   //!
   TBranch        *b_eJetGen_ite  ;   //!
   TBranch        *b_etaJetGen_ite;   //!
   TBranch        *b_phiJetGen_ite;   //!
   TBranch        *b_nJetGen_akt5;   //!
   TBranch        *b_ptJetGen_akt5 ;   //!
   TBranch        *b_eJetGen_akt5  ;   //!
   TBranch        *b_etaJetGen_akt5;   //!
   TBranch        *b_phiJetGen_akt5;   //!
   TBranch        *b_nMuonsGen_akt5;   //!
   TBranch        *b_nElectronsGen_akt5;   //!
   TBranch        *b_nPhotonsGen_akt5;   //!
   TBranch        *b_nTracksGen_akt5;   //!
   TBranch        *b_nNeutralHadronsGen_akt5;   //!
   TBranch        *b_eMuonsGen_akt5;   //!
   TBranch        *b_eElectronsGen_akt5;   //!
   TBranch        *b_ePhotonsGen_akt5;   //!
   TBranch        *b_eTracksGen_akt5;   //!
   TBranch        *b_eNeutralHadronsGen_akt5;   //!
   TBranch        *b_ptMuonsGen_akt5;   //!
   TBranch        *b_ptElectronsGen_akt5;   //!
   TBranch        *b_ptPhotonsGen_akt5;   //!
   TBranch        *b_ptTracksGen_akt5;   //!
   TBranch        *b_ptNeutralHadronsGen_akt5;   //!
   TBranch        *b_phiMuonsGen_akt5;   //!
   TBranch        *b_phiElectronsGen_akt5;   //!
   TBranch        *b_phiPhotonsGen_akt5;   //!
   TBranch        *b_phiTracksGen_akt5;   //!
   TBranch        *b_phiNeutralHadronsGen_akt5;   //!
   TBranch        *b_etaMuonsGen_akt5;   //!
   TBranch        *b_etaElectronsGen_akt5;   //!
   TBranch        *b_etaPhotonsGen_akt5;   //!
   TBranch        *b_etaTracksGen_akt5;   //!
   TBranch        *b_etaNeutralHadronsGen_akt5;   //!
   TBranch        *b_nJetGen_kt4;   //!
   TBranch        *b_ptJetGen_kt4 ;   //!
   TBranch        *b_eJetGen_kt4  ;   //!
   TBranch        *b_etaJetGen_kt4;   //!
   TBranch        *b_phiJetGen_kt4;   //!
   TBranch        *b_nJetGen_kt6;   //!
   TBranch        *b_ptJetGen_kt6 ;   //!
   TBranch        *b_eJetGen_kt6  ;   //!
   TBranch        *b_etaJetGen_kt6;   //!
   TBranch        *b_phiJetGen_kt6;   //!
   TBranch        *b_nJetGen_sis5;   //!
   TBranch        *b_ptJetGen_sis5;   //!
   TBranch        *b_eJetGen_sis5  ;   //!
   TBranch        *b_etaJetGen_sis5;   //!
   TBranch        *b_phiJetGen_sis5;   //!
   TBranch        *b_nJetGen_sis7;   //!
   TBranch        *b_ptJetGen_sis7 ;   //!
   TBranch        *b_eJetGen_sis7  ;   //!
   TBranch        *b_etaJetGen_sis7;   //!
   TBranch        *b_phiJetGen_sis7;   //!
   TBranch        *b_sMet;   //!
   TBranch        *b_eMet;   //!
   TBranch        *b_phiMet;   //!
   TBranch        *b_stcMet;   //!
   TBranch        *b_etcMet;   //!
   TBranch        *b_phitcMet;   //!
   TBranch        *b_spfMet;   //!
   TBranch        *b_epfMet;   //!
   TBranch        *b_phipfMet;   //!
   TBranch        *b_sMetGen;   //!
   TBranch        *b_eMetGen;   //!
   TBranch        *b_phiMetGen;   //!
   TBranch        *b_sMetGen2;   //!
   TBranch        *b_eMetGen2;   //!
   TBranch        *b_phiMetGen2;   //!
   TBranch        *b_hltPass;   //!
   TBranch        *b_nHLT;   //!
   TBranch        *b_hltNamesLen;   //!
   TBranch        *b_HLTNames;   //!
   TBranch        *b_HLTResults;   //!

   photoncandidate phot;
   jetcandidate jet;

   std::ofstream *fout;
   std::ofstream *ferr;

   // Configuration settings
   //std::string _version;
   std::string _algo;
   std::string _logfile;
   std::string _errfile;
   bool _readall;
   //bool _summer08;
   //
   float _rcone;
   float _backtoback;
   //
   float _photetacut;
   float _photptcut;
   float _rmatch;
   float _jetetacut;
   float _lowptreco;
   float _jet2_minpt;
   float _jet2_maxfrac;
   float _deltaphi;
   float _deltaeta;

   // Storage for photonID cut parameters
   photonidcuts _looseid;
   photonidcuts _mediumid;
   photonidcuts _tightid;

   // Indeces of photons used in analysis
   int irecphot; // leading reconstructed photon
   int isubphot; // substitute for reco photon (parton)
   int i2ndphot; // second reconstructed photon
   int igenphot; // generator jet matching reco photon
   int imcphot;  // leading MC parton photon (or matching quark for QCD)
   int imcptcl;  // MC particle matching reco photon

   // Indeces of jets used in analysis
   int irecjet;  // leading reconstructed jet
   int i2ndjet;  // second reconstructed jet
   int igenjet;  // generator jet matching reco jet
   int imcjet;   // MC parton quark/gluon matching reco jet

   Bool_t matched;
   Float_t pt2ndjet;
   Float_t pt2ndphot;
   Bool_t isdiphoton;

   // Photon ID efficiency tree (EffTree)
   Int_t isgammas;
   Int_t ntrkisos;
   Float_t pts;
   Float_t ptisos;
   Float_t ptiso5s;
   Float_t ptiso7s;
   Float_t ecalisos;
   Float_t hcalisos;
   Float_t sMajMajs;
   Float_t sMinMins;
   Float_t hoveres;
   Float_t setaetas;
   Float_t r9s;
   Float_t weights;

   // Analysis tree (AnaTree)
   Int_t photonid;
   Int_t isgamma;
   Int_t ismatched;
   Int_t pflavor; // added Aug 24, 2009
   Int_t ppid; // added Aug 26, 2009
   Int_t flavor;
   Float_t pthat;
   Float_t weight;
   Float_t ptph;
   Float_t ptj;
   Float_t etaph;
   Float_t etaj;
   Float_t phiph;
   Float_t phij;
   Float_t emfj; // added May 5, 2009
   Float_t pt2jet;
   Float_t pt2sum;
   Float_t pt2vecsum;
   Float_t pt2phot;
   Float_t ptphottrue;
   Float_t ptphotgen; // added Aug 24, 2009
   Float_t ptjettrue;
   Float_t ptquarktrue;
   Float_t phiphottrue;
   Float_t phijettrue;
   Float_t phiquarktrue;
   Float_t etaphottrue;
   Float_t etajettrue;
   Float_t etaquarktrue;
   Float_t mpf;
   Float_t mpftrue;


   TFile* hOutputFile ;
   TTree * eff_tree ;
   TTree * ana_tree ;

   int npb;

   gjettree(TTree *tree=0, char * outputname = "test.root");
   virtual ~gjettree();
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop(double cross = 1., double genpt_start = 0.,
			 double genpt_limit = 14000.,
			 bool isgamma = true, int NEVT = 10000000);
   virtual void     BookHistos();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
   
   // Load configuration from the file determined by 'CAFE_CONFIG'
   // Set the environment variable 'CAFE_CONFIG' like this:
   //   gSystem->Setenv("CAFE_CONFIG", "gjettree.config");
   // Input parameter "tree" gives the name of the configuration.
   // Set the parameters like this:
   //   tree.DeltaPhi: 3.0
   //   tree.DeltaEta: 1.0
   // Remember to load the Config class before gjettree.C:
   //   gROOT->ProcessLine(".L Config.cpp++");
   void Configure(const char* cfg = "tree");
   void DumpConfig();

   //  calculate phi1-phi2 keeping value between 0 and pi
   inline double delta_phi(double phi1, double phi2);
   inline double delta_eta(double eta1, double eta2);
   inline double oplus(double a, double b);
   bool cutID(int i, photonidcuts const& pid, std::vector<bool> *vpass = 0);
   //bool NNID(int i, std::vector<IClassifierReader*> const& classreaders);

   std::map<std::string, gjettree_histos*> histos;
   std::map<std::string, gjetefficiency_histos*> effs;
   void FillHistos(std::string dirname);
   void FillEffHistos(std::string dirname, photonidcuts const& pid, double w);
   void WriteHistos();
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
      if (tree == 0)
	tree = (TTree*)gDirectory->Get("myanalysis/pippo");
   }
   Init(tree);
   hOutputFile   = new TFile(outputname , "RECREATE" ) ;

   Configure();
   DumpConfig();
}

gjettree::~gjettree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
   eff_tree->Write();
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

   fChain->SetBranchAddress("genpt", &genpt, &b_genpt);
   fChain->SetBranchAddress("isMC", &isMC, &b_isMC);
   fChain->SetBranchAddress("store", &store, &b_store);
   fChain->SetBranchAddress("lbn", &lbn, &b_lbn);
   fChain->SetBranchAddress("bx", &bx, &b_bx);
   fChain->SetBranchAddress("orbit", &orbit, &b_orbit);
   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("event", &event, &b_event);
   fChain->SetBranchAddress("nMC", &nMC, &b_nMC);
   fChain->SetBranchAddress("pdgIdMC", pdgIdMC, &b_pdgIdMC);
   fChain->SetBranchAddress("statusMC", statusMC, &b_statusMC);
   fChain->SetBranchAddress("ptMC ", ptMC , &b_ptMC );
   fChain->SetBranchAddress("eMC  ", eMC  , &b_eMC  );
   fChain->SetBranchAddress("etaMC", etaMC, &b_etaMC);
   fChain->SetBranchAddress("phiMC", phiMC, &b_phiMC);
   fChain->SetBranchAddress("nSIM", &nSIM, &b_nSIM);
   fChain->SetBranchAddress("pdgIdSIM", pdgIdSIM, &b_pdgIdSIM);
   fChain->SetBranchAddress("statusSIM", statusSIM, &b_statusSIM);
   fChain->SetBranchAddress("ptSIM ", ptSIM , &b_ptSIM );
   fChain->SetBranchAddress("eSIM  ", eSIM  , &b_eSIM  );
   fChain->SetBranchAddress("etaSIM", etaSIM, &b_etaSIM);
   fChain->SetBranchAddress("phiSIM", phiSIM, &b_phiSIM);
   fChain->SetBranchAddress("rSIM", rSIM, &b_rSIM);
   fChain->SetBranchAddress("zSIM", zSIM, &b_zSIM);
   fChain->SetBranchAddress("nPF", &nPF, &b_nPF);
   fChain->SetBranchAddress("pdgIdPF", pdgIdPF, &b_pdgIdPF);
   fChain->SetBranchAddress("ptPF ", ptPF , &b_ptPF );
   fChain->SetBranchAddress("ePF  ", ePF  , &b_ePF  );
   fChain->SetBranchAddress("etaPF", etaPF, &b_etaPF);
   fChain->SetBranchAddress("phiPF", phiPF, &b_phiPF);
   fChain->SetBranchAddress("nPhot", &nPhot, &b_nPhot);
   fChain->SetBranchAddress("ptPhot ", ptPhot , &b_ptPhot );
   fChain->SetBranchAddress("ePhot  ", ePhot  , &b_ePhot  );
   fChain->SetBranchAddress("escPhot  ", escPhot  , &b_escPhot  );
   fChain->SetBranchAddress("etaPhot", etaPhot, &b_etaPhot);
   fChain->SetBranchAddress("phiPhot", phiPhot, &b_phiPhot);
   fChain->SetBranchAddress("nconvPhot", &nconvPhot, &b_nconvPhot);
   fChain->SetBranchAddress("chi2convPhot", chi2convPhot, &b_chi2convPhot);
   fChain->SetBranchAddress("ndofconvPhot", ndofconvPhot, &b_ndofconvPhot);
   fChain->SetBranchAddress("rconvPhot", rconvPhot, &b_rconvPhot);
   fChain->SetBranchAddress("phiconvPhot", phiconvPhot, &b_phiconvPhot);
   fChain->SetBranchAddress("zconvPhot", zconvPhot, &b_zconvPhot);
   fChain->SetBranchAddress("ntrkconvPhot", ntrkconvPhot, &b_ntrkconvPhot);
   fChain->SetBranchAddress("eovpconvPhot", eovpconvPhot, &b_eovpconvPhot);
   fChain->SetBranchAddress("etaecalconvPhot", etaecalconvPhot, &b_etaecalconvPhot);
   fChain->SetBranchAddress("phiecalconvPhot", phiecalconvPhot, &b_phiecalconvPhot);
   fChain->SetBranchAddress("eecalconvPhot", eecalconvPhot, &b_eecalconvPhot);
   fChain->SetBranchAddress("algoconvPhot", algoconvPhot, &b_algoconvPhot);
   fChain->SetBranchAddress("d0convPhot", d0convPhot, &b_d0convPhot);
   fChain->SetBranchAddress("detaecalconvPhot", detaecalconvPhot, &b_detaecalconvPhot);
   fChain->SetBranchAddress("dphiecalconvPhot", dphiecalconvPhot, &b_dphiecalconvPhot);
   fChain->SetBranchAddress("dphivtxconvPhot", dphivtxconvPhot, &b_dphivtxconvPhot);
   fChain->SetBranchAddress("pairsepconvPhot", pairsepconvPhot, &b_pairsepconvPhot);
   fChain->SetBranchAddress("pairmassconvPhot", pairmassconvPhot, &b_pairmassconvPhot);
   fChain->SetBranchAddress("trchi21convPhot", trchi21convPhot, &b_trchi21convPhot);
   fChain->SetBranchAddress("trndof1convPhot", trndof1convPhot, &b_trndof1convPhot);
   fChain->SetBranchAddress("trqual1convPhot", trqual1convPhot, &b_trqual1convPhot);
   fChain->SetBranchAddress("trpt1convPhot", trpt1convPhot, &b_trpt1convPhot);
   fChain->SetBranchAddress("trerr1convPhot", trerr1convPhot, &b_trerr1convPhot);
   fChain->SetBranchAddress("trchi22convPhot", trchi22convPhot, &b_trchi22convPhot);
   fChain->SetBranchAddress("trndof2convPhot", trndof2convPhot, &b_trndof2convPhot);
   fChain->SetBranchAddress("trqual2convPhot", trqual2convPhot, &b_trqual2convPhot);
   fChain->SetBranchAddress("trpt2convPhot", trpt2convPhot, &b_trpt2convPhot);
   fChain->SetBranchAddress("trerr2convPhot", trerr2convPhot, &b_trerr2convPhot);
   fChain->SetBranchAddress("pid_isEM", pid_isEM, &b_pid_isEM);
   fChain->SetBranchAddress("pid_isLoose", pid_isLoose, &b_pid_isLoose);
   fChain->SetBranchAddress("pid_isTight", pid_isTight, &b_pid_isTight);
   fChain->SetBranchAddress("pid_jurECAL", pid_jurECAL, &b_pid_jurECAL);
   fChain->SetBranchAddress("pid_twrHCAL", pid_twrHCAL, &b_pid_twrHCAL);
   fChain->SetBranchAddress("pid_HoverE", pid_HoverE, &b_pid_HoverE);
   fChain->SetBranchAddress("pid_hlwTrack", pid_hlwTrack, &b_pid_hlwTrack);
   fChain->SetBranchAddress("pid_etawid", pid_etawid, &b_pid_etawid);
   fChain->SetBranchAddress("ptiso0015Phot", ptiso0015Phot, &b_ptiso0015Phot);
   fChain->SetBranchAddress("ntrkiso0015Phot", ntrkiso0015Phot, &b_ntrkiso0015Phot);
   fChain->SetBranchAddress("ptiso035Phot", ptiso035Phot, &b_ptiso035Phot);
   fChain->SetBranchAddress("ntrkiso035Phot", ntrkiso035Phot, &b_ntrkiso035Phot);
   fChain->SetBranchAddress("ptiso04Phot", ptiso04Phot, &b_ptiso04Phot);
   fChain->SetBranchAddress("ntrkiso04Phot", ntrkiso04Phot, &b_ntrkiso04Phot);
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
   fChain->SetBranchAddress("nJet_ite", &nJet_ite, &b_nJet_ite);
   fChain->SetBranchAddress("ptJet_ite ", ptJet_ite , &b_ptJet_ite );
   fChain->SetBranchAddress("eJet_ite  ", eJet_ite  , &b_eJet_ite  );
   fChain->SetBranchAddress("etaJet_ite", etaJet_ite, &b_etaJet_ite);
   fChain->SetBranchAddress("phiJet_ite", phiJet_ite, &b_phiJet_ite);
   fChain->SetBranchAddress("emfJet_ite", emfJet_ite, &b_emfJet_ite);
   fChain->SetBranchAddress("nJet_kt4", &nJet_kt4, &b_nJet_kt4);
   fChain->SetBranchAddress("ptJet_kt4 ", ptJet_kt4 , &b_ptJet_kt4 );
   fChain->SetBranchAddress("eJet_kt4  ", eJet_kt4  , &b_eJet_kt4  );
   fChain->SetBranchAddress("etaJet_kt4", etaJet_kt4, &b_etaJet_kt4);
   fChain->SetBranchAddress("phiJet_kt4", phiJet_kt4, &b_phiJet_kt4);
   fChain->SetBranchAddress("emfJet_kt4", emfJet_kt4, &b_emfJet_kt4);
   fChain->SetBranchAddress("nJet_kt6", &nJet_kt6, &b_nJet_kt6);
   fChain->SetBranchAddress("ptJet_kt6 ", ptJet_kt6 , &b_ptJet_kt6 );
   fChain->SetBranchAddress("eJet_kt6  ", eJet_kt6  , &b_eJet_kt6  );
   fChain->SetBranchAddress("etaJet_kt6", etaJet_kt6, &b_etaJet_kt6);
   fChain->SetBranchAddress("phiJet_kt6", phiJet_kt6, &b_phiJet_kt6);
   fChain->SetBranchAddress("emfJet_kt6", emfJet_kt6, &b_emfJet_kt6);
   fChain->SetBranchAddress("nJet_akt5", &nJet_akt5, &b_nJet_akt5);
   fChain->SetBranchAddress("ptJet_akt5 ", ptJet_akt5 , &b_ptJet_akt5 );
   fChain->SetBranchAddress("eJet_akt5  ", eJet_akt5  , &b_eJet_akt5  );
   fChain->SetBranchAddress("etaJet_akt5", etaJet_akt5, &b_etaJet_akt5);
   fChain->SetBranchAddress("phiJet_akt5", phiJet_akt5, &b_phiJet_akt5);
   fChain->SetBranchAddress("emfJet_akt5", emfJet_akt5, &b_emfJet_akt5);
   fChain->SetBranchAddress("nJet_sis5", &nJet_sis5, &b_nJet_sis5);
   fChain->SetBranchAddress("ptJet_sis5 ", ptJet_sis5 , &b_ptJet_sis5 );
   fChain->SetBranchAddress("eJet_sis5  ", eJet_sis5  , &b_eJet_sis5  );
   fChain->SetBranchAddress("etaJet_sis5", etaJet_sis5, &b_etaJet_sis5);
   fChain->SetBranchAddress("phiJet_sis5", phiJet_sis5, &b_phiJet_sis5);
   fChain->SetBranchAddress("emfJet_sis5", emfJet_sis5, &b_emfJet_sis5);
   fChain->SetBranchAddress("nJet_sis7", &nJet_sis7, &b_nJet_sis7);
   fChain->SetBranchAddress("ptJet_sis7 ", ptJet_sis7 , &b_ptJet_sis7 );
   fChain->SetBranchAddress("eJet_sis7  ", eJet_sis7  , &b_eJet_sis7  );
   fChain->SetBranchAddress("etaJet_sis7", etaJet_sis7, &b_etaJet_sis7);
   fChain->SetBranchAddress("phiJet_sis7", phiJet_sis7, &b_phiJet_sis7);
   fChain->SetBranchAddress("emfJet_sis7", emfJet_sis7, &b_emfJet_sis7);
   fChain->SetBranchAddress("nJet_pfite", &nJet_pfite, &b_nJet_pfite);
   fChain->SetBranchAddress("ptJet_pfite ", ptJet_pfite , &b_ptJet_pfite );
   fChain->SetBranchAddress("eJet_pfite  ", eJet_pfite  , &b_eJet_pfite  );
   fChain->SetBranchAddress("etaJet_pfite", etaJet_pfite, &b_etaJet_pfite);
   fChain->SetBranchAddress("phiJet_pfite", phiJet_pfite, &b_phiJet_pfite);
   fChain->SetBranchAddress("nJet_pfkt4", &nJet_pfkt4, &b_nJet_pfkt4);
   fChain->SetBranchAddress("ptJet_pfkt4 ", ptJet_pfkt4 , &b_ptJet_pfkt4 );
   fChain->SetBranchAddress("eJet_pfkt4  ", eJet_pfkt4  , &b_eJet_pfkt4  );
   fChain->SetBranchAddress("etaJet_pfkt4", etaJet_pfkt4, &b_etaJet_pfkt4);
   fChain->SetBranchAddress("phiJet_pfkt4", phiJet_pfkt4, &b_phiJet_pfkt4);
   fChain->SetBranchAddress("nJet_pfakt5", &nJet_pfakt5, &b_nJet_pfakt5);
   fChain->SetBranchAddress("ptJet_pfakt5 ", ptJet_pfakt5 , &b_ptJet_pfakt5 );
   fChain->SetBranchAddress("eJet_pfakt5  ", eJet_pfakt5  , &b_eJet_pfakt5  );
   fChain->SetBranchAddress("etaJet_pfakt5", etaJet_pfakt5, &b_etaJet_pfakt5);
   fChain->SetBranchAddress("phiJet_pfakt5", phiJet_pfakt5, &b_phiJet_pfakt5);
   fChain->SetBranchAddress("nChargedHadrons_pfakt5", nChargedHadrons_pfakt5, &b_nChargedHadrons_pfakt5);
   fChain->SetBranchAddress("nPhotons_pfakt5", nPhotons_pfakt5, &b_nPhotons_pfakt5);
   fChain->SetBranchAddress("nMuons_pfakt5", nMuons_pfakt5, &b_nMuons_pfakt5);
   fChain->SetBranchAddress("nElectrons_pfakt5", nElectrons_pfakt5, &b_nElectrons_pfakt5);
   fChain->SetBranchAddress("nNeutralHadrons_pfakt5", nNeutralHadrons_pfakt5, &b_nNeutralHadrons_pfakt5);
   fChain->SetBranchAddress("nHFHadrons_pfakt5", nHFHadrons_pfakt5, &b_nHFHadrons_pfakt5);
   fChain->SetBranchAddress("nHFEM_pfakt5", nHFEM_pfakt5, &b_nHFEM_pfakt5);
   fChain->SetBranchAddress("eChargedHadrons_pfakt5", eChargedHadrons_pfakt5, &b_eChargedHadrons_pfakt5);
   fChain->SetBranchAddress("ePhotons_pfakt5", ePhotons_pfakt5, &b_ePhotons_pfakt5);
   fChain->SetBranchAddress("eMuons_pfakt5", eMuons_pfakt5, &b_eMuons_pfakt5);
   fChain->SetBranchAddress("eElectrons_pfakt5", eElectrons_pfakt5, &b_eElectrons_pfakt5);
   fChain->SetBranchAddress("eNeutralHadrons_pfakt5", eNeutralHadrons_pfakt5, &b_eNeutralHadrons_pfakt5);
   fChain->SetBranchAddress("eHFHadrons_pfakt5", eHFHadrons_pfakt5, &b_eHFHadrons_pfakt5);
   fChain->SetBranchAddress("eHFEM_pfakt5", eHFEM_pfakt5, &b_eHFEM_pfakt5);
   fChain->SetBranchAddress("ptChargedHadrons_pfakt5", ptChargedHadrons_pfakt5, &b_ptChargedHadrons_pfakt5);
   fChain->SetBranchAddress("ptPhotons_pfakt5", ptPhotons_pfakt5, &b_ptPhotons_pfakt5);
   fChain->SetBranchAddress("ptMuons_pfakt5", ptMuons_pfakt5, &b_ptMuons_pfakt5);
   fChain->SetBranchAddress("ptElectrons_pfakt5", ptElectrons_pfakt5, &b_ptElectrons_pfakt5);
   fChain->SetBranchAddress("ptNeutralHadrons_pfakt5", ptNeutralHadrons_pfakt5, &b_ptNeutralHadrons_pfakt5);
   fChain->SetBranchAddress("ptHFHadrons_pfakt5", ptHFHadrons_pfakt5, &b_ptHFHadrons_pfakt5);
   fChain->SetBranchAddress("ptHFEM_pfakt5", ptHFEM_pfakt5, &b_ptHFEM_pfakt5);
   fChain->SetBranchAddress("phiChargedHadrons_pfakt5", phiChargedHadrons_pfakt5, &b_phiChargedHadrons_pfakt5);
   fChain->SetBranchAddress("phiPhotons_pfakt5", phiPhotons_pfakt5, &b_phiPhotons_pfakt5);
   fChain->SetBranchAddress("phiMuons_pfakt5", phiMuons_pfakt5, &b_phiMuons_pfakt5);
   fChain->SetBranchAddress("phiElectrons_pfakt5", phiElectrons_pfakt5, &b_phiElectrons_pfakt5);
   fChain->SetBranchAddress("phiNeutralHadrons_pfakt5", phiNeutralHadrons_pfakt5, &b_phiNeutralHadrons_pfakt5);
   fChain->SetBranchAddress("phiHFHadrons_pfakt5", phiHFHadrons_pfakt5, &b_phiHFHadrons_pfakt5);
   fChain->SetBranchAddress("phiHFEM_pfakt5", phiHFEM_pfakt5, &b_phiHFEM_pfakt5);
   fChain->SetBranchAddress("etaChargedHadrons_pfakt5", etaChargedHadrons_pfakt5, &b_etaChargedHadrons_pfakt5);
   fChain->SetBranchAddress("etaPhotons_pfakt5", etaPhotons_pfakt5, &b_etaPhotons_pfakt5);
   fChain->SetBranchAddress("etaMuons_pfakt5", etaMuons_pfakt5, &b_etaMuons_pfakt5);
   fChain->SetBranchAddress("etaElectrons_pfakt5", etaElectrons_pfakt5, &b_etaElectrons_pfakt5);
   fChain->SetBranchAddress("etaNeutralHadrons_pfakt5", etaNeutralHadrons_pfakt5, &b_etaNeutralHadrons_pfakt5);
   fChain->SetBranchAddress("etaHFHadrons_pfakt5", etaHFHadrons_pfakt5, &b_etaHFHadrons_pfakt5);
   fChain->SetBranchAddress("etaHFEM_pfakt5", etaHFEM_pfakt5, &b_etaHFEM_pfakt5);
   fChain->SetBranchAddress("nJet_pfsis5", &nJet_pfsis5, &b_nJet_pfsis5);
   fChain->SetBranchAddress("ptJet_pfsis5 ", ptJet_pfsis5 , &b_ptJet_pfsis5 );
   fChain->SetBranchAddress("eJet_pfsis5  ", eJet_pfsis5  , &b_eJet_pfsis5  );
   fChain->SetBranchAddress("etaJet_pfsis5", etaJet_pfsis5, &b_etaJet_pfsis5);
   fChain->SetBranchAddress("phiJet_pfsis5", phiJet_pfsis5, &b_phiJet_pfsis5);
   fChain->SetBranchAddress("nJet_pfkt6", &nJet_pfkt6, &b_nJet_pfkt6);
   fChain->SetBranchAddress("ptJet_pfkt6 ", ptJet_pfkt6 , &b_ptJet_pfkt6 );
   fChain->SetBranchAddress("eJet_pfkt6  ", eJet_pfkt6  , &b_eJet_pfkt6  );
   fChain->SetBranchAddress("etaJet_pfkt6", etaJet_pfkt6, &b_etaJet_pfkt6);
   fChain->SetBranchAddress("phiJet_pfkt6", phiJet_pfkt6, &b_phiJet_pfkt6);
   fChain->SetBranchAddress("nJet_pfsis7", &nJet_pfsis7, &b_nJet_pfsis7);
   fChain->SetBranchAddress("ptJet_pfsis7 ", ptJet_pfsis7 , &b_ptJet_pfsis7 );
   fChain->SetBranchAddress("eJet_pfsis7  ", eJet_pfsis7  , &b_eJet_pfsis7  );
   fChain->SetBranchAddress("etaJet_pfsis7", etaJet_pfsis7, &b_etaJet_pfsis7);
   fChain->SetBranchAddress("phiJet_pfsis7", phiJet_pfsis7, &b_phiJet_pfsis7);
   fChain->SetBranchAddress("nJetGen_ite", &nJetGen_ite, &b_nJetGen_ite);
   fChain->SetBranchAddress("ptJetGen_ite ", ptJetGen_ite , &b_ptJetGen_ite );
   fChain->SetBranchAddress("eJetGen_ite  ", eJetGen_ite  , &b_eJetGen_ite  );
   fChain->SetBranchAddress("etaJetGen_ite", etaJetGen_ite, &b_etaJetGen_ite);
   fChain->SetBranchAddress("phiJetGen_ite", phiJetGen_ite, &b_phiJetGen_ite);
   fChain->SetBranchAddress("nJetGen_akt5", &nJetGen_akt5, &b_nJetGen_akt5);
   fChain->SetBranchAddress("ptJetGen_akt5 ", ptJetGen_akt5 , &b_ptJetGen_akt5 );
   fChain->SetBranchAddress("eJetGen_akt5  ", eJetGen_akt5  , &b_eJetGen_akt5  );
   fChain->SetBranchAddress("etaJetGen_akt5", etaJetGen_akt5, &b_etaJetGen_akt5);
   fChain->SetBranchAddress("phiJetGen_akt5", phiJetGen_akt5, &b_phiJetGen_akt5);
   fChain->SetBranchAddress("nMuonsGen_akt5", nMuonsGen_akt5, &b_nMuonsGen_akt5);
   fChain->SetBranchAddress("nElectronsGen_akt5", nElectronsGen_akt5, &b_nElectronsGen_akt5);
   fChain->SetBranchAddress("nPhotonsGen_akt5", nPhotonsGen_akt5, &b_nPhotonsGen_akt5);
   fChain->SetBranchAddress("nTracksGen_akt5", nTracksGen_akt5, &b_nTracksGen_akt5);
   fChain->SetBranchAddress("nNeutralHadronsGen_akt5", nNeutralHadronsGen_akt5, &b_nNeutralHadronsGen_akt5);
   fChain->SetBranchAddress("eMuonsGen_akt5", eMuonsGen_akt5, &b_eMuonsGen_akt5);
   fChain->SetBranchAddress("eElectronsGen_akt5", eElectronsGen_akt5, &b_eElectronsGen_akt5);
   fChain->SetBranchAddress("ePhotonsGen_akt5", ePhotonsGen_akt5, &b_ePhotonsGen_akt5);
   fChain->SetBranchAddress("eTracksGen_akt5", eTracksGen_akt5, &b_eTracksGen_akt5);
   fChain->SetBranchAddress("eNeutralHadronsGen_akt5", eNeutralHadronsGen_akt5, &b_eNeutralHadronsGen_akt5);
   fChain->SetBranchAddress("ptMuonsGen_akt5", ptMuonsGen_akt5, &b_ptMuonsGen_akt5);
   fChain->SetBranchAddress("ptElectronsGen_akt5", ptElectronsGen_akt5, &b_ptElectronsGen_akt5);
   fChain->SetBranchAddress("ptPhotonsGen_akt5", ptPhotonsGen_akt5, &b_ptPhotonsGen_akt5);
   fChain->SetBranchAddress("ptTracksGen_akt5", ptTracksGen_akt5, &b_ptTracksGen_akt5);
   fChain->SetBranchAddress("ptNeutralHadronsGen_akt5", ptNeutralHadronsGen_akt5, &b_ptNeutralHadronsGen_akt5);
   fChain->SetBranchAddress("phiMuonsGen_akt5", phiMuonsGen_akt5, &b_phiMuonsGen_akt5);
   fChain->SetBranchAddress("phiElectronsGen_akt5", phiElectronsGen_akt5, &b_phiElectronsGen_akt5);
   fChain->SetBranchAddress("phiPhotonsGen_akt5", phiPhotonsGen_akt5, &b_phiPhotonsGen_akt5);
   fChain->SetBranchAddress("phiTracksGen_akt5", phiTracksGen_akt5, &b_phiTracksGen_akt5);
   fChain->SetBranchAddress("phiNeutralHadronsGen_akt5", phiNeutralHadronsGen_akt5, &b_phiNeutralHadronsGen_akt5);
   fChain->SetBranchAddress("etaMuonsGen_akt5", etaMuonsGen_akt5, &b_etaMuonsGen_akt5);
   fChain->SetBranchAddress("etaElectronsGen_akt5", etaElectronsGen_akt5, &b_etaElectronsGen_akt5);
   fChain->SetBranchAddress("etaPhotonsGen_akt5", etaPhotonsGen_akt5, &b_etaPhotonsGen_akt5);
   fChain->SetBranchAddress("etaTracksGen_akt5", etaTracksGen_akt5, &b_etaTracksGen_akt5);
   fChain->SetBranchAddress("etaNeutralHadronsGen_akt5", etaNeutralHadronsGen_akt5, &b_etaNeutralHadronsGen_akt5);
   fChain->SetBranchAddress("nJetGen_kt4", &nJetGen_kt4, &b_nJetGen_kt4);
   fChain->SetBranchAddress("ptJetGen_kt4 ", ptJetGen_kt4 , &b_ptJetGen_kt4 );
   fChain->SetBranchAddress("eJetGen_kt4  ", eJetGen_kt4  , &b_eJetGen_kt4  );
   fChain->SetBranchAddress("etaJetGen_kt4", etaJetGen_kt4, &b_etaJetGen_kt4);
   fChain->SetBranchAddress("phiJetGen_kt4", phiJetGen_kt4, &b_phiJetGen_kt4);
   fChain->SetBranchAddress("nJetGen_kt6", &nJetGen_kt6, &b_nJetGen_kt6);
   fChain->SetBranchAddress("ptJetGen_kt6 ", ptJetGen_kt6 , &b_ptJetGen_kt6 );
   fChain->SetBranchAddress("eJetGen_kt6  ", eJetGen_kt6  , &b_eJetGen_kt6  );
   fChain->SetBranchAddress("etaJetGen_kt6", etaJetGen_kt6, &b_etaJetGen_kt6);
   fChain->SetBranchAddress("phiJetGen_kt6", phiJetGen_kt6, &b_phiJetGen_kt6);
   fChain->SetBranchAddress("nJetGen_sis5", &nJetGen_sis5, &b_nJetGen_sis5);
   fChain->SetBranchAddress("ptJetGen_sis5", ptJetGen_sis5, &b_ptJetGen_sis5);
   fChain->SetBranchAddress("eJetGen_sis5  ", eJetGen_sis5  , &b_eJetGen_sis5  );
   fChain->SetBranchAddress("etaJetGen_sis5", etaJetGen_sis5, &b_etaJetGen_sis5);
   fChain->SetBranchAddress("phiJetGen_sis5", phiJetGen_sis5, &b_phiJetGen_sis5);
   fChain->SetBranchAddress("nJetGen_sis7", &nJetGen_sis7, &b_nJetGen_sis7);
   fChain->SetBranchAddress("ptJetGen_sis7 ", ptJetGen_sis7 , &b_ptJetGen_sis7 );
   fChain->SetBranchAddress("eJetGen_sis7  ", eJetGen_sis7  , &b_eJetGen_sis7  );
   fChain->SetBranchAddress("etaJetGen_sis7", etaJetGen_sis7, &b_etaJetGen_sis7);
   fChain->SetBranchAddress("phiJetGen_sis7", phiJetGen_sis7, &b_phiJetGen_sis7);
   fChain->SetBranchAddress("sMet  ", &sMet  , &b_sMet);
   fChain->SetBranchAddress("eMet  ", &eMet  , &b_eMet);
   fChain->SetBranchAddress("phiMet", &phiMet, &b_phiMet);
   fChain->SetBranchAddress("stcMet  ", &stcMet  , &b_stcMet);
   fChain->SetBranchAddress("etcMet  ", &etcMet  , &b_etcMet);
   fChain->SetBranchAddress("phitcMet", &phitcMet, &b_phitcMet);
   fChain->SetBranchAddress("spfMet  ", &spfMet  , &b_spfMet);
   fChain->SetBranchAddress("epfMet  ", &epfMet  , &b_epfMet);
   fChain->SetBranchAddress("phipfMet", &phipfMet, &b_phipfMet);
   fChain->SetBranchAddress("sMetGen  ", &sMetGen  , &b_sMetGen);
   fChain->SetBranchAddress("eMetGen  ", &eMetGen  , &b_eMetGen);
   fChain->SetBranchAddress("phiMetGen", &phiMetGen, &b_phiMetGen);
   fChain->SetBranchAddress("sMetGen2  ", &sMetGen2  , &b_sMetGen2);
   fChain->SetBranchAddress("eMetGen2  ", &eMetGen2  , &b_eMetGen2);
   fChain->SetBranchAddress("phiMetGen2", &phiMetGen2, &b_phiMetGen2);
   fChain->SetBranchAddress("hltPass", &hltPass, &b_hltPass);
   fChain->SetBranchAddress("nHLT", &nHLT, &b_nHLT);
   fChain->SetBranchAddress("hltNamesLen", &hltNamesLen, &b_hltNamesLen);
   fChain->SetBranchAddress("HLTNames", HLTNames, &b_HLTNames);
   fChain->SetBranchAddress("HLTResults", HLTResults, &b_HLTResults);
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
