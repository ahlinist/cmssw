#define analysisClass_cxx
#include "analysisClass.h"
#include <TH2.h>
#include <TH1D.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLorentzVector.h>
#include <TVector2.h>
#include <TVector3.h>
#include <TProfile.h>
#include <TF1.h>

float N_ch_TOT_inHF = 1728;

analysisClass::analysisClass(string * inputList, string * cutFile, string * treeName, string * outputFileName, string * cutEfficFile)
  :baseClass(inputList, cutFile, treeName, outputFileName, cutEfficFile)
{
  std::cout << "analysisClass::analysisClass(): begins " << std::endl;

  std::cout << "analysisClass::analysisClass(): ends " << std::endl;
}

analysisClass::~analysisClass()
{
  std::cout << "analysisClass::~analysisClass(): begins " << std::endl;

  std::cout << "analysisClass::~analysisClass(): ends " << std::endl;
}

void analysisClass::Loop()
{
   std::cout << "analysisClass::Loop() begins" <<std::endl;   
    
   if (fChain == 0) return;

   
   //////////book histos here

   //binning
   int Nbins_Nspikes=10;
   float Max_Nspikes=10;
   int   Nbins_E = 320;
   float Min_E = -10.;
   float Max_E = 950.;
   int   Nbins_ET = 240;
   float Min_ET = -10.;
   float Max_ET = 230.;

   //1D histograms
   TH1D *h_N_HFspikesPerEv_L    = new TH1D ("h_N_HFspikesPerEv_L","h_N_HFspikesPerEv_L",Nbins_Nspikes,0,Max_Nspikes);
   TH1D *h_N_HFspikesPerEv_S    = new TH1D ("h_N_HFspikesPerEv_S","h_N_HFspikesPerEv_S",Nbins_Nspikes,0,Max_Nspikes);
   TH1D *h_N_HFspikesPerEv_TOT    = new TH1D ("h_N_HFspikesPerEv_TOT","h_N_HFspikesPerEv_TOT",Nbins_Nspikes,0,Max_Nspikes);
   TH1D *h_N_EBspikesPerEv_TOT    = new TH1D ("h_N_EBspikesPerEv_TOT","h_N_EBspikesPerEv_TOT",Nbins_Nspikes,0,Max_Nspikes);

   h_N_HFspikesPerEv_L->Sumw2();
   h_N_HFspikesPerEv_S->Sumw2();
   h_N_HFspikesPerEv_TOT->Sumw2();
   h_N_EBspikesPerEv_TOT->Sumw2();

   TH1D *h_HFRecHitE_L = new TH1D("h_HFRecHitE_L","h_HFRecHitE_L;E [GeV]",Nbins_E,Min_E,Max_E);
   TH1D *h_HFRecHitE_L_Flagged = new TH1D("h_HFRecHitE_L_Flagged","h_HFRecHitE_L_Flagged;E [GeV]",Nbins_E,Min_E,Max_E);
   TH1D *h_HFRecHitE_S = new TH1D("h_HFRecHitE_S","h_HFRecHitE_S;E [GeV]",Nbins_E,Min_E,Max_E);
   TH1D *h_HFRecHitE_S_Flagged = new TH1D("h_HFRecHitE_S_Flagged","h_HFRecHitE_S_Flagged;E [GeV]",Nbins_E,Min_E,Max_E);
   
   h_HFRecHitE_L->Sumw2();
   h_HFRecHitE_L_Flagged->Sumw2();
   h_HFRecHitE_S->Sumw2();
   h_HFRecHitE_S_Flagged->Sumw2();
   
   map<UInt_t,TH1D*> h_HFRecHitE_L_ieta;
   map<UInt_t,TH1D*> h_HFRecHitE_L_Flagged_ieta;
   map<UInt_t,TH1D*> h_HFRecHitE_S_ieta;
   map<UInt_t,TH1D*> h_HFRecHitE_S_Flagged_ieta;
   
   TH1D *h_HFRecHitET_L = new TH1D("h_HFRecHitET_L","h_HFRecHitET_L;E_{T} [GeV]",Nbins_ET,Min_ET,Max_ET);
   TH1D *h_HFRecHitET_S = new TH1D("h_HFRecHitET_S","h_HFRecHitET_S;E_{T} [GeV]",Nbins_ET,Min_ET,Max_ET);
   TH1D *h_HFRecHitET_L_Flagged = new TH1D("h_HFRecHitET_L_Flagged","h_HFRecHitET_L_Flagged;E_{T} [GeV]",Nbins_ET,Min_ET,Max_ET);
   TH1D *h_HFRecHitET_S_Flagged = new TH1D("h_HFRecHitET_S_Flagged","h_HFRecHitET_S_Flagged;E_{T} [GeV]",Nbins_ET,Min_ET,Max_ET);
   
   h_HFRecHitET_L->Sumw2();
   h_HFRecHitET_S->Sumw2();
   h_HFRecHitET_L_Flagged->Sumw2();
   h_HFRecHitET_S_Flagged->Sumw2();
   
   map<UInt_t,TH1D*> h_HFRecHitET_L_ieta;
   map<UInt_t,TH1D*> h_HFRecHitET_S_ieta;
   map<UInt_t,TH1D*> h_HFRecHitET_L_Flagged_ieta;
   map<UInt_t,TH1D*> h_HFRecHitET_S_Flagged_ieta;
   
   for( uint i=29; i<=41; i++ ) {
     h_HFRecHitE_L_ieta[i] = new TH1D(Form("h_HFRecHitE_L_ieta_%u",i),Form("h_HFRecHitE_L_ieta_%u;E [GeV]",i),Nbins_E,Min_E,Max_E);
     h_HFRecHitE_L_ieta[i]->Sumw2();
     h_HFRecHitE_S_ieta[i] = new TH1D(Form("h_HFRecHitE_S_ieta_%u",i),Form("h_HFRecHitE_S_ieta_%u;E [GeV]",i),Nbins_E,Min_E,Max_E);
     h_HFRecHitE_S_ieta[i]->Sumw2();
     h_HFRecHitE_L_Flagged_ieta[i] = new TH1D(Form("h_HFRecHitE_L_Flagged_ieta_%u",i),Form("h_HFRecHitE_L_Flagged_ieta_%u;E [GeV]",i),Nbins_E,Min_E,Max_E);
     h_HFRecHitE_L_Flagged_ieta[i]->Sumw2();
     h_HFRecHitE_S_Flagged_ieta[i] = new TH1D(Form("h_HFRecHitE_S_Flagged_ieta_%u",i),Form("h_HFRecHitE_S_Flagged_ieta_%u;E [GeV]",i),Nbins_E,Min_E,Max_E);
     h_HFRecHitE_S_Flagged_ieta[i]->Sumw2();
     
     h_HFRecHitET_L_ieta[i] = new TH1D(Form("h_HFRecHitET_L_ieta_%u",i),Form("h_HFRecHitET_L_ieta_%u;E_{T} [GeV]",i),Nbins_ET,Min_ET,Max_ET);
     h_HFRecHitET_L_ieta[i]->Sumw2();
     h_HFRecHitET_S_ieta[i] = new TH1D(Form("h_HFRecHitET_S_ieta_%u",i),Form("h_HFRecHitET_S_ieta_%u;E_{T} [GeV]",i),Nbins_ET,Min_ET,Max_ET);
     h_HFRecHitET_S_ieta[i]->Sumw2();
     h_HFRecHitET_L_Flagged_ieta[i] = new TH1D(Form("h_HFRecHitET_L_Flagged_ieta_%u",i),Form("h_HFRecHitET_L_Flagged_ieta_%u;E_{T} [GeV]",i),Nbins_ET,Min_ET,Max_ET);
     h_HFRecHitET_L_Flagged_ieta[i]->Sumw2();
     h_HFRecHitET_S_Flagged_ieta[i] = new TH1D(Form("h_HFRecHitET_S_Flagged_ieta_%u",i),Form("h_HFRecHitET_S_Flagged_ieta_%u;E_{T} [GeV]",i),Nbins_ET,Min_ET,Max_ET);
     h_HFRecHitET_S_Flagged_ieta[i]->Sumw2();
   }
   
   //MET histograms
   int   Nbins_METSumET = 100;
   float Max_METSumET = 100;
   int   Nbins_Phi = 50;
   float Max_Phi = 3.15;
  
   TH1D *h_CaloMET   = new TH1D ("h_CaloMET","h_CaloMET;#slash{E}_{T} [GeV]",Nbins_METSumET,0,Max_METSumET);
   TH1D *h_CaloMETPhi  = new TH1D ("h_CaloMETPhi","h_CaloMETPhi;#phi",Nbins_Phi,-Max_Phi,Max_Phi);
   TH1D *h_CaloMEx   = new TH1D ("h_CaloMEx","h_CaloMEx;#slash{E}_{x} [GeV]",Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
   TH1D *h_CaloMEy   = new TH1D ("h_CaloMEy","h_CaloMEy;#slash{E}_{y} [GeV]",Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
   TH1D *h_CaloSumET   = new TH1D ("h_CaloSumET","h_CaloSumET;#SigmaE_{T}",Nbins_METSumET,0,Max_METSumET);
   
   h_CaloMET->Sumw2();
   h_CaloMETPhi->Sumw2();
   h_CaloMEx->Sumw2();
   h_CaloMEy->Sumw2();
   h_CaloSumET->Sumw2();
   
   TH1D *h_CaloMEThcalNoise   = new TH1D ("h_CaloMEThcalNoise","h_CaloMEThcalNoise;#slash{E}_{T} [GeV]",Nbins_METSumET,0,Max_METSumET);
   
   h_CaloMEThcalNoise->Sumw2();
      
//    TH1D *h_CaloMET_cleanECAL = new TH1D ("h_CaloMET_cleanECAL","h_CaloMET_cleanECAL;#slash{E}_{T} [GeV]",Nbins_METSumET,0,Max_METSumET);
//    TH1D *h_CaloMETPhi_cleanECAL  = new TH1D ("h_CaloMETPhi_cleanECAL","h_CaloMETPhi_cleanECAL;#phi",Nbins_Phi,-Max_Phi,Max_Phi);
//    TH1D *h_CaloMEx_cleanECAL   = new TH1D ("h_CaloMEx_cleanECAL","h_CaloMEx_cleanECAL;#slash{E}_{x} [GeV]",Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
//    TH1D *h_CaloMEy_cleanECAL   = new TH1D ("h_CaloMEy_cleanECAL","h_CaloMEy_cleanECAL;#slash{E}_{y} [GeV]",Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
//    TH1D *h_CaloSumET_cleanECAL   = new TH1D ("h_CaloSumET_cleanECAL","h_CaloSumET_cleanECAL;#SigmaE_{T}",Nbins_METSumET,0,Max_METSumET);
// 
//    h_CaloMET_cleanECAL->Sumw2();
//    h_CaloMETPhi_cleanECAL->Sumw2();
//    h_CaloMEx_cleanECAL->Sumw2();
//    h_CaloMEy_cleanECAL->Sumw2();
//    h_CaloSumET_cleanECAL->Sumw2();

   TH1D *h_CaloMET_clean = new TH1D ("h_CaloMET_clean","h_CaloMET_clean;#slash{E}_{T} [GeV]",Nbins_METSumET,0,Max_METSumET);
   TH1D *h_CaloMETPhi_clean  = new TH1D ("h_CaloMETPhi_clean","h_CaloMETPhi_clean;#phi",Nbins_Phi,-Max_Phi,Max_Phi);
   TH1D *h_CaloMEx_clean   = new TH1D ("h_CaloMEx_clean","h_CaloMEx_clean;#slash{E}_{x} [GeV]",Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
   TH1D *h_CaloMEy_clean   = new TH1D ("h_CaloMEy_clean","h_CaloMEy_clean;#slash{E}_{y} [GeV]",Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
   TH1D *h_CaloSumET_clean   = new TH1D ("h_CaloSumET_clean","h_CaloSumET_clean;#SigmaE_{T}",Nbins_METSumET,0,Max_METSumET);
   
   h_CaloMET_clean->Sumw2();
   h_CaloMETPhi_clean->Sumw2();
   h_CaloMEx_clean->Sumw2();
   h_CaloMEy_clean->Sumw2();
   h_CaloSumET_clean->Sumw2();
   
   TH1D *h_tcMET   = new TH1D ("h_tcMET","h_tcMET;#slash{E}_{T} [GeV]",Nbins_METSumET,0,Max_METSumET);
   TH1D *h_tcMETPhi  = new TH1D ("h_tcMETPhi","h_tcMETPhi;#phi",Nbins_Phi,-Max_Phi,Max_Phi);
   TH1D *h_tcMEx   = new TH1D ("h_tcMEx","h_tcMEx;#slash{E}_{x} [GeV]",Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
   TH1D *h_tcMEy   = new TH1D ("h_tcMEy","h_tcMEy;#slash{E}_{y} [GeV]",Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
   TH1D *h_tcSumET   = new TH1D ("h_tcSumET","h_tcSumET;#SigmaE_{T}",Nbins_METSumET,0,Max_METSumET);
   
   h_tcMET->Sumw2();
   h_tcMETPhi->Sumw2();
   h_tcMEx->Sumw2();
   h_tcMEy->Sumw2();
   h_tcSumET->Sumw2();
   
   TH1D *h_tcMET_clean = new TH1D ("h_tcMET_clean","h_tcMET_clean;#slash{E}_{T} [GeV]",Nbins_METSumET,0,Max_METSumET);
   TH1D *h_tcMETPhi_clean  = new TH1D ("h_tcMETPhi_clean","h_tcMETPhi_clean;#phi",Nbins_Phi,-Max_Phi,Max_Phi);
   TH1D *h_tcMEx_clean   = new TH1D ("h_tcMEx_clean","h_tcMEx_clean;#slash{E}_{x} [GeV]",Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
   TH1D *h_tcMEy_clean   = new TH1D ("h_tcMEy_clean","h_tcMEy_clean;#slash{E}_{y} [GeV]",Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
   TH1D *h_tcSumET_clean   = new TH1D ("h_tcSumET_clean","h_tcSumET_clean;#SigmaE_{T}",Nbins_METSumET,0,Max_METSumET);
   
   h_tcMET_clean->Sumw2();
   h_tcMETPhi_clean->Sumw2();
   h_tcMEx_clean->Sumw2();
   h_tcMEy_clean->Sumw2();
   h_tcSumET_clean->Sumw2();

   TH1D *h_pfMET   = new TH1D ("h_pfMET","h_pfMET;#slash{E}_{T} [GeV]",Nbins_METSumET,0,Max_METSumET);
   TH1D *h_pfMETPhi  = new TH1D ("h_pfMETPhi","h_pfMETPhi;#phi",Nbins_Phi,-Max_Phi,Max_Phi);
   TH1D *h_pfMEx   = new TH1D ("h_pfMEx","h_pfMEx;#slash{E}_{x} [GeV]",Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
   TH1D *h_pfMEy   = new TH1D ("h_pfMEy","h_pfMEy;#slash{E}_{y} [GeV]",Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
   TH1D *h_pfSumET   = new TH1D ("h_pfSumET","h_pfSumET;#SigmaE_{T}",Nbins_METSumET,0,Max_METSumET);
   
   h_pfMET->Sumw2();
   h_pfMETPhi->Sumw2();
   h_pfMEx->Sumw2();
   h_pfMEy->Sumw2();
   h_pfSumET->Sumw2();
   
   TH1D *h_CaloMETHF   = new TH1D ("h_CaloMETHF","h_CaloMETHF;#slash{E}_{T} [GeV]",Nbins_METSumET,0,Max_METSumET);
   TH1D *h_CaloMETPhiHF  = new TH1D ("h_CaloMETPhiHF","h_CaloMETPhiHF;#phi",Nbins_Phi,-Max_Phi,Max_Phi);
   TH1D *h_CaloMExHF   = new TH1D ("h_CaloMExHF","h_CaloMExHF;#slash{E}_{x} [GeV]",Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
   TH1D *h_CaloMEyHF   = new TH1D ("h_CaloMEyHF","h_CaloMEyHF;#slash{E}_{y} [GeV]",Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
   TH1D *h_CaloSumETHF   = new TH1D ("h_CaloSumETHF","h_CaloSumETHF;#SigmaE_{T}",Nbins_METSumET,0,Max_METSumET);

   h_CaloMETHF->Sumw2();
   h_CaloMETPhiHF->Sumw2();
   h_CaloMExHF->Sumw2();
   h_CaloMEyHF->Sumw2();
   h_CaloSumETHF->Sumw2();
   
   TH1D *h_CaloMETHF_clean   = new TH1D ("h_CaloMETHF_clean","h_CaloMETHF_clean;#slash{E}_{T} [GeV]",Nbins_METSumET,0,Max_METSumET);
   TH1D *h_CaloMETPhiHF_clean  = new TH1D ("h_CaloMETPhiHF_clean","h_CaloMETPhiHF_clean;#phi",Nbins_Phi,-Max_Phi,Max_Phi);
   TH1D *h_CaloMExHF_clean   = new TH1D ("h_CaloMExHF_clean","h_CaloMExHF_clean;#slash{E}_{x} [GeV]",Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
   TH1D *h_CaloMEyHF_clean   = new TH1D ("h_CaloMEyHF_clean","h_CaloMEyHF_clean;#slash{E}_{y} [GeV]",Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
   TH1D *h_CaloSumETHF_clean   = new TH1D ("h_CaloSumETHF_clean","h_CaloSumETHF_clean;#SigmaE_{T}",Nbins_METSumET,0,Max_METSumET);
   
   h_CaloMETHF_clean->Sumw2();
   h_CaloMETPhiHF_clean->Sumw2();
   h_CaloMExHF_clean->Sumw2();
   h_CaloMEyHF_clean->Sumw2();
   h_CaloSumETHF_clean->Sumw2();

   //Sum E histograms
   int Nbins_SumE = 200;
   float Max_SumE = 5000;
   TH1 *h_SumEinHF   = new TH1D ("h_SumEinHF","h_SumEinHF;#SigmaE [GeV]",Nbins_SumE,0,Max_SumE);
   
   //2D histograms
   TH2D *h2_N_HFspikes_L_ieta_iphi  = new TH2D ("h2_N_HFspikes_L_ieta_iphi","h2_N_HFspikes_L_ieta_iphi;i#eta;i#phi",13,28.5,41.5,72,0.5,72.5);
   TH2D *h2_N_HFspikes_S_ieta_iphi  = new TH2D ("h2_N_HFspikes_S_ieta_iphi","h2_N_HFspikes_S_ieta_iphi;i#eta;i#phi",13,28.5,41.5,72,0.5,72.5);
   
   int   Nbins_R = 220;
   float Min_R = -1.1;
   float Max_R = 1.1;
   int   Nbins_S9oS1 = 240;
   float Min_S9oS1 = -0.4;
   float Max_S9oS1 = 2.0;
   
   TH2D *h2_HFRecHitE_L_vs_S = new TH2D ("h2_HFRecHitE_L_vs_S","h2_HFRecHitE_L_vs_S;E [GeV];E [GeV]",Nbins_E,Min_E,Max_E,Nbins_E,Min_E,Max_E);
   TH2D *h2_HFRecHitET_L_vs_S = new TH2D ("h2_HFRecHitET_L_vs_S","h2_HFRecHitET_L_vs_S;E_{T} [GeV];E_{T} [GeV]",Nbins_ET,Min_ET,Max_ET,Nbins_ET,Min_ET,Max_ET);
   TH2D *h2_HFRecHitE_vs_R_L = new TH2D ("h2_HFRecHitE_vs_R_L","h2_HFRecHitE_vs_R_L;R;E [GeV]",Nbins_R,Min_R,Max_R,Nbins_E,Min_E,Max_E);
   TH2D *h2_HFRecHitE_vs_R_S = new TH2D ("h2_HFRecHitE_vs_R_S","h2_HFRecHitE_vs_R_S;R;E [GeV]",Nbins_R,Min_R,Max_R,Nbins_E,Min_E,Max_E);
   TH2D *h2_HFRecHitET_vs_R_L = new TH2D ("h2_HFRecHitET_vs_R_L","h2_HFRecHitET_vs_R_L;R;E_{T} [GeV]",Nbins_R,Min_R,Max_R,Nbins_ET,Min_ET,Max_ET);
   TH2D *h2_HFRecHitET_vs_R_S = new TH2D ("h2_HFRecHitET_vs_R_S","h2_HFRecHitET_vs_R_S;R;E_{T} [GeV]",Nbins_R,Min_R,Max_R,Nbins_ET,Min_ET,Max_ET);
   
   map<UInt_t,TH2D*> h2_HFRecHitE_vs_R_L_ieta;
   map<UInt_t,TH2D*> h2_HFRecHitE_vs_R_S_ieta;
   map<UInt_t,TH2D*> h2_HFRecHitET_vs_R_L_ieta;
   map<UInt_t,TH2D*> h2_HFRecHitET_vs_R_S_ieta;
   
   TH2D *h2_R_vs_iphi_L  = new TH2D ("h2_R_vs_iphi_L","h2_R_vs_iphi_L;i#phi;R",72,0.5,72.5,Nbins_R,Min_R,Max_R);
   TH2D *h2_R_vs_ieta_L  = new TH2D ("h2_R_vs_ieta_L","h2_R_vs_ieta_L;i#eta;R",13,28.5,41.5,Nbins_R,Min_R,Max_R);
   TH2D *h2_R_vs_iphi_S  = new TH2D ("h2_R_vs_iphi_S","h2_R_vs_iphi_S;i#phi;R",72,0.5,72.5,Nbins_R,Min_R,Max_R);
   TH2D *h2_R_vs_ieta_S  = new TH2D ("h2_R_vs_ieta_S","h2_R_vs_ieta_S;i#eta;R",13,28.5,41.5,Nbins_R,Min_R,Max_R);

   TH2D *h2_S9oS1_vs_E_L  = new TH2D ("h2_S9oS1_vs_E_L","h2_S9oS1_vs_E_L;E [GeV];S9/S1",Nbins_E,Min_E,Max_E,Nbins_S9oS1,Min_S9oS1,Max_S9oS1);
   TH2D *h2_S9oS1_vs_E_S  = new TH2D ("h2_S9oS1_vs_E_S","h2_S9oS1_vs_E_S;E [GeV];S9/S1",Nbins_E,Min_E,Max_E,Nbins_S9oS1,Min_S9oS1,Max_S9oS1);
   TH2D *h2_S8oS1_vs_E_L  = new TH2D ("h2_S8oS1_vs_E_L","h2_S8oS1_vs_E_L;E [GeV];S8/S1",Nbins_E,Min_E,Max_E,Nbins_S9oS1,Min_S9oS1,Max_S9oS1);
   TH2D *h2_S8oS1_vs_E_S  = new TH2D ("h2_S8oS1_vs_E_S","h2_S8oS1_vs_E_S;E [GeV];S8/S1",Nbins_E,Min_E,Max_E,Nbins_S9oS1,Min_S9oS1,Max_S9oS1);
   TH2D *h2_S9oS1_vs_E_L_Flagged  = new TH2D ("h2_S9oS1_vs_E_L_Flagged","h2_S9oS1_vs_E_L_Flagged;E [GeV];S9/S1",Nbins_E,Min_E,Max_E,Nbins_S9oS1,Min_S9oS1,Max_S9oS1);
   TH2D *h2_S9oS1_vs_E_S_Flagged  = new TH2D ("h2_S9oS1_vs_E_S_Flagged","h2_S9oS1_vs_E_S_Flagged;E [GeV];S9/S1",Nbins_E,Min_E,Max_E,Nbins_S9oS1,Min_S9oS1,Max_S9oS1);
   TH2D *h2_S8oS1_vs_E_L_Flagged  = new TH2D ("h2_S8oS1_vs_E_L_Flagged","h2_S8oS1_vs_E_L_Flagged;E [GeV];S8/S1",Nbins_E,Min_E,Max_E,Nbins_S9oS1,Min_S9oS1,Max_S9oS1);
   TH2D *h2_S8oS1_vs_E_S_Flagged  = new TH2D ("h2_S8oS1_vs_E_S_Flagged","h2_S8oS1_vs_E_S_Flagged;E [GeV];S8/S1",Nbins_E,Min_E,Max_E,Nbins_S9oS1,Min_S9oS1,Max_S9oS1);

   map<UInt_t,TH2D*> h2_S9oS1_vs_E_L_ieta;
   map<UInt_t,TH2D*> h2_S9oS1_vs_E_S_ieta;
   map<UInt_t,TH2D*> h2_S8oS1_vs_E_L_ieta;
   map<UInt_t,TH2D*> h2_S8oS1_vs_E_S_ieta;
   map<UInt_t,TH2D*> h2_S9oS1_vs_E_L_Flagged_ieta;
   map<UInt_t,TH2D*> h2_S9oS1_vs_E_S_Flagged_ieta;
   map<UInt_t,TH2D*> h2_S8oS1_vs_E_L_Flagged_ieta;
   map<UInt_t,TH2D*> h2_S8oS1_vs_E_S_Flagged_ieta;
   
   for( uint i=29; i<=41; i++ ) {
     h2_HFRecHitE_vs_R_L_ieta[i] = new TH2D(Form("h2_HFRecHitE_vs_R_L_ieta_%u",i),Form("h2_HFRecHitE_vs_R_L_ieta_%u;R;E [GeV]",i),Nbins_R,Min_R,Max_R,Nbins_E,Min_E,Max_E);
     h2_HFRecHitE_vs_R_S_ieta[i] = new TH2D(Form("h2_HFRecHitE_vs_R_S_ieta_%u",i),Form("h2_HFRecHitE_vs_R_S_ieta_%u;R;E [GeV]",i),Nbins_R,Min_R,Max_R,Nbins_E,Min_E,Max_E);
     h2_HFRecHitET_vs_R_L_ieta[i] = new TH2D(Form("h2_HFRecHitET_vs_R_L_ieta_%u",i),Form("h2_HFRecHitET_vs_R_L_ieta_%u;R;E_{T} [GeV]",i),Nbins_R,Min_R,Max_R,Nbins_ET,Min_ET,Max_ET);
     h2_HFRecHitET_vs_R_S_ieta[i] = new TH2D(Form("h2_HFRecHitET_vs_R_S_ieta_%u",i),Form("h2_HFRecHitET_vs_R_S_ieta_%u;R;E_{T} [GeV]",i),Nbins_R,Min_R,Max_R,Nbins_ET,Min_ET,Max_ET);
    
     h2_S9oS1_vs_E_L_ieta[i] = new TH2D(Form("h2_S9oS1_vs_E_L_ieta_%u",i),Form("h2_S9oS1_vs_E_L_ieta_%u;E [GeV];S9/S1",i),Nbins_E,Min_E,Max_E,Nbins_S9oS1,Min_S9oS1,Max_S9oS1);
     h2_S9oS1_vs_E_S_ieta[i] = new TH2D(Form("h2_S9oS1_vs_E_S_ieta_%u",i),Form("h2_S9oS1_vs_E_S_ieta_%u;E [GeV];S9/S1",i),Nbins_E,Min_E,Max_E,Nbins_S9oS1,Min_S9oS1,Max_S9oS1);
     h2_S8oS1_vs_E_L_ieta[i] = new TH2D(Form("h2_S8oS1_vs_E_L_ieta_%u",i),Form("h2_S8oS1_vs_E_L_ieta_%u;E [GeV];S8/S1",i),Nbins_E,Min_E,Max_E,Nbins_S9oS1,Min_S9oS1,Max_S9oS1);
     h2_S8oS1_vs_E_S_ieta[i] = new TH2D(Form("h2_S8oS1_vs_E_S_ieta_%u",i),Form("h2_S8oS1_vs_E_S_ieta_%u;E [GeV];S8/S1",i),Nbins_E,Min_E,Max_E,Nbins_S9oS1,Min_S9oS1,Max_S9oS1);
     h2_S9oS1_vs_E_L_Flagged_ieta[i] = new TH2D(Form("h2_S9oS1_vs_E_L_Flagged_ieta_%u",i),Form("h2_S9oS1_vs_E_L_Flagged_ieta_%u;E [GeV];S9/S1",i),Nbins_E,Min_E,Max_E,Nbins_S9oS1,Min_S9oS1,Max_S9oS1);
     h2_S9oS1_vs_E_S_Flagged_ieta[i] = new TH2D(Form("h2_S9oS1_vs_E_S_Flagged_ieta_%u",i),Form("h2_S9oS1_vs_E_S_Flagged_ieta_%u;E [GeV];S9/S1",i),Nbins_E,Min_E,Max_E,Nbins_S9oS1,Min_S9oS1,Max_S9oS1);
     h2_S8oS1_vs_E_L_Flagged_ieta[i] = new TH2D(Form("h2_S8oS1_vs_E_L_Flagged_ieta_%u",i),Form("h2_S8oS1_vs_E_L_Flagged_ieta_%u;E [GeV];S8/S1",i),Nbins_E,Min_E,Max_E,Nbins_S9oS1,Min_S9oS1,Max_S9oS1);
     h2_S8oS1_vs_E_S_Flagged_ieta[i] = new TH2D(Form("h2_S8oS1_vs_E_S_Flagged_ieta_%u",i),Form("h2_S8oS1_vs_E_S_Flagged_ieta_%u;E [GeV];S8/S1",i),Nbins_E,Min_E,Max_E,Nbins_S9oS1,Min_S9oS1,Max_S9oS1);
   }
   
   TProfile * p_NspikesInHF_vs_SumEinHF    = new TProfile ("p_NspikesInHF_vs_SumEinHF","p_NspikesInHF_vs_SumEinHF",100,0,5000.);
   TProfile * p_NspikesInHF_L_vs_SumEinHF  = new TProfile ("p_NspikesInHF_L_vs_SumEinHF","p_NspikesInHF_L_vs_SumEinHF",100,0,5000.);
   TProfile * p_NspikesInHF_S_vs_SumEinHF  = new TProfile ("p_NspikesInHF_S_vs_SumEinHF","p_NspikesInHF_S_vs_SumEinHF",100,0,5000.);   

   map<UInt_t,TProfile*> p_NspikesInHF_vs_SumEinHF_ieta;
   for( uint i=1; i<=5; i++ ) {
     p_NspikesInHF_vs_SumEinHF_ieta[i] = new TProfile(Form("p_NspikesInHF_vs_SumEinHF_ieta_%u",i),Form("p_NspikesInHF_vs_SumEinHF_ieta_%u;SumE HF [GeV]; N. spikes per ev.",i)
						      ,100,0,5000.);
   }
   
   TProfile * p_OccupancyHF_vs_SumEinHF = new TProfile ("p_OccupancyHF_vs_SumEinHF","p_OccupancyHF_vs_SumEinHF",100,0,5000.);

   // #### Validation plots ####
   //Jet variables
   int useJetMETVariables = (int)getPreCutValue1("useJetMETVariables");
   float JetPtCut = getPreCutValue1("JetPtCut");

//    int Nbins_pt = 300;
//    float Min_pt = -0.5;
//    float Max_pt = 299.5;
//    int Nbins_default = 100; 
//    float Min_default = -0.5; 
//    float Max_default = 99.5; 
// 
//    
//    TH1D *h_JetInHF_pT   = new TH1D ("h_JetInHF_pT","h_JetInHF_pT;jet p_{T} in HF [GeV]",Nbins_pt,Min_pt,Max_pt);
//    TH1D *h_JetInHF_eta   = new TH1D ("h_JetInHF_eta","h_JetInHF_eta;jet #eta in HF",Nbins_default,-6,6);
//    TH1D *h_JetInHF_phi   = new TH1D ("h_JetInHF_phi","h_JetInHF_phi;jet #phi in HF",Nbins_default,-4,4);
//    TH1D *h_JetInHF_Njet   = new TH1D ("h_JetInHF_Njet","h_JetInHF_Njet;Number of jets in HF",Nbins_default,Min_default,Max_default);
//    TH1D *h_JetInHF_Njet_ptcut   = new TH1D ("h_JetInHF_Njet_ptcut","h_JetInHF_Njet_ptcut;Number of jets in HF (p_{T} cut)",Nbins_default,Min_default,Max_default);
//    TH1D *h_JetInHF_Nconst   = new TH1D ("h_JetInHF_Nconst","h_JetInHF_Nconst;Number of constituents of jets in HF",Nbins_default,Min_default,Max_default);
//    TH1D *h_JetInHF_N60   = new TH1D ("h_JetInHF_N60","h_JetInHF_N60;N60 variable of jets in HF",Nbins_default,Min_default,Max_default);
//    TH1D *h_JetInHF_N90   = new TH1D ("h_JetInHF_N90","h_JetInHF_N90;N90 variable of jets in HF",Nbins_default,Min_default,Max_default);
//    
//    TH1D *h_JetInHF_pT_PET   = new TH1D ("h_JetInHF_pT_PET","h_JetInHF_pT_PET;jet p_{T} in HF [GeV]",Nbins_pt,Min_pt,Max_pt);
//    TH1D *h_JetInHF_eta_PET   = new TH1D ("h_JetInHF_eta_PET","h_JetInHF_eta_PET;jet #eta in HF",Nbins_default,-6,6);
//    TH1D *h_JetInHF_phi_PET   = new TH1D ("h_JetInHF_phi_PET","h_JetInHF_phi_PET;jet #phi in HF",Nbins_default,-4,4);
//    TH1D *h_JetInHF_Njet_PET   = new TH1D ("h_JetInHF_Njet_PET","h_JetInHF_Njet_PET;Number of jets in HF",Nbins_default,Min_default,Max_default);
//    TH1D *h_JetInHF_Njet_ptcut_PET   = new TH1D ("h_JetInHF_Njet_ptcut_PET","h_JetInHF_Njet_ptcut_PET;Number of jets in HF (p_{T} cut)",Nbins_default,Min_default,Max_default);
//    TH1D *h_JetInHF_Nconst_PET   = new TH1D ("h_JetInHF_Nconst_PET","h_JetInHF_Nconst_PET;Number of constituents of jets in HF",Nbins_default,Min_default,Max_default);
//    TH1D *h_JetInHF_N60_PET   = new TH1D ("h_JetInHF_N60_PET","h_JetInHF_N60_PET;N60 variable of jets in HF",Nbins_default,Min_default,Max_default);
//    TH1D *h_JetInHF_N90_PET   = new TH1D ("h_JetInHF_N90_PET","h_JetInHF_N90_PET;N90 variable of jets in HF",Nbins_default,Min_default,Max_default);
//    
//    TH1D *h_JetInHF_pT_S9S1   = new TH1D ("h_JetInHF_pT_S9S1","h_JetInHF_pT_S9S1;jet p_{T} in HF [GeV]",Nbins_pt,Min_pt,Max_pt);
//    TH1D *h_JetInHF_eta_S9S1   = new TH1D ("h_JetInHF_eta_S9S1","h_JetInHF_eta_S9S1;jet #eta in HF",Nbins_default,-6,6);
//    TH1D *h_JetInHF_phi_S9S1   = new TH1D ("h_JetInHF_phi_S9S1","h_JetInHF_phi_S9S1;jet #phi in HF",Nbins_default,-4,4);
//    TH1D *h_JetInHF_Njet_S9S1   = new TH1D ("h_JetInHF_Njet_S9S1","h_JetInHF_Njet_S9S1;Number of jets in HF",Nbins_default,Min_default,Max_default);
//    TH1D *h_JetInHF_Njet_ptcut_S9S1   = new TH1D ("h_JetInHF_Njet_ptcut_S9S1","h_JetInHF_Njet_ptcut_S9S1;Number of jets in HF (p_{T} cut)",Nbins_default,Min_default,Max_default);
//    TH1D *h_JetInHF_Nconst_S9S1   = new TH1D ("h_JetInHF_Nconst_S9S1","h_JetInHF_Nconst_S9S1;Number of constituents of jets in HF",Nbins_default,Min_default,Max_default);
//    TH1D *h_JetInHF_N60_S9S1   = new TH1D ("h_JetInHF_N60_S9S1","h_JetInHF_N60_S9S1;N60 variable of jets in HF",Nbins_default,Min_default,Max_default);
//    TH1D *h_JetInHF_N90_S9S1   = new TH1D ("h_JetInHF_N90_S9S1","h_JetInHF_N90_S9S1;N90 variable of jets in HF",Nbins_default,Min_default,Max_default);
// 
//    h_JetInHF_pT->Sumw2();
//    h_JetInHF_eta->Sumw2();
//    h_JetInHF_phi->Sumw2();
//    h_JetInHF_Njet->Sumw2();
//    h_JetInHF_Njet_ptcut->Sumw2();
//    h_JetInHF_Nconst->Sumw2();  
//    h_JetInHF_N60->Sumw2();  
//    h_JetInHF_N90->Sumw2(); 
//    
//    h_JetInHF_pT_PET->Sumw2();
//    h_JetInHF_eta_PET->Sumw2();
//    h_JetInHF_phi_PET->Sumw2();
//    h_JetInHF_Njet_PET->Sumw2();
//    h_JetInHF_Njet_ptcut_PET->Sumw2();
//    h_JetInHF_Nconst_PET->Sumw2();
//    h_JetInHF_N60_PET->Sumw2();
//    h_JetInHF_N90_PET->Sumw2();
//    
//    h_JetInHF_pT_S9S1->Sumw2();
//    h_JetInHF_eta_S9S1->Sumw2();
//    h_JetInHF_phi_S9S1->Sumw2();
//    h_JetInHF_Njet_S9S1->Sumw2();
//    h_JetInHF_Njet_ptcut_S9S1->Sumw2();
//    h_JetInHF_Nconst_S9S1->Sumw2();
//    h_JetInHF_N60_S9S1->Sumw2();
//    h_JetInHF_N90_S9S1->Sumw2();
//    
//    //MET variables
//    TH1D *h_CaloMETinHF   = new TH1D ("h_CaloMETinHF","h_CaloMETinHF;#slash{E}_{T} [GeV]",Nbins_METSumET,0,Max_METSumET);
//    TH1D *h_CaloMETPhiinHF  = new TH1D ("h_CaloMETPhiinHF","h_CaloMETPhiinHF;#phi",Nbins_Phi,-Max_Phi,Max_Phi);
//    TH1D *h_CaloMExinHF   = new TH1D ("h_CaloMExinHF","h_CaloMExinHF;#slash{E}_{x} [GeV]",Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
//    TH1D *h_CaloMEyinHF   = new TH1D ("h_CaloMEyinHF","h_CaloMEyinHF;#slash{E}_{y} [GeV]",Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
//    TH1D *h_CaloSumETinHF   = new TH1D ("h_CaloSumETinHF","h_CaloSumETinHF;#SigmaE_{T}",Nbins_METSumET,0,Max_METSumET);
//    
//    TH1D *h_CaloMETinHF_PET   = new TH1D ("h_CaloMETinHF_PET","h_CaloMETinHF_PET;#slash{E}_{T} [GeV]",Nbins_METSumET,0,Max_METSumET);
//    TH1D *h_CaloMETPhiinHF_PET  = new TH1D ("h_CaloMETPhiinHF_PET","h_CaloMETPhiinHF_PET;#phi",Nbins_Phi,-Max_Phi,Max_Phi);
//    TH1D *h_CaloMExinHF_PET   = new TH1D ("h_CaloMExinHF_PET","h_CaloMExinHF_PET;#slash{E}_{x} [GeV]",Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
//    TH1D *h_CaloMEyinHF_PET   = new TH1D ("h_CaloMEyinHF_PET","h_CaloMEyinHF_PET;#slash{E}_{y} [GeV]",Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
//    TH1D *h_CaloSumETinHF_PET   = new TH1D ("h_CaloSumETinHF_PET","h_CaloSumETinHF_PET;#SigmaE_{T}",Nbins_METSumET,0,Max_METSumET);
// 
//    TH1D *h_CaloMETinHF_S9S1   = new TH1D ("h_CaloMETinHF_S9S1","h_CaloMETinHF_S9S1;#slash{E}_{T} [GeV]",Nbins_METSumET,0,Max_METSumET);
//    TH1D *h_CaloMETPhiinHF_S9S1  = new TH1D ("h_CaloMETPhiinHF_S9S1","h_CaloMETPhiinHF_S9S1;#phi",Nbins_Phi,-Max_Phi,Max_Phi);
//    TH1D *h_CaloMExinHF_S9S1   = new TH1D ("h_CaloMExinHF_S9S1","h_CaloMExinHF_S9S1;#slash{E}_{x} [GeV]",Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
//    TH1D *h_CaloMEyinHF_S9S1   = new TH1D ("h_CaloMEyinHF_S9S1","h_CaloMEyinHF_S9S1;#slash{E}_{y} [GeV]",Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
//    TH1D *h_CaloSumETinHF_S9S1   = new TH1D ("h_CaloSumETinHF_S9S1","h_CaloSumETinHF_S9S1;#SigmaE_{T}",Nbins_METSumET,0,Max_METSumET);
//    
//    h_CaloMETinHF->Sumw2();
//    h_CaloMETPhiinHF->Sumw2();
//    h_CaloMExinHF->Sumw2();
//    h_CaloMEyinHF->Sumw2();
//    h_CaloSumETinHF->Sumw2();
// 
//    h_CaloMETinHF_PET->Sumw2();
//    h_CaloMETPhiinHF_PET->Sumw2();
//    h_CaloMExinHF_PET->Sumw2();
//    h_CaloMEyinHF_PET->Sumw2();
//    h_CaloSumETinHF_PET->Sumw2();
//    
//    h_CaloMETinHF_S9S1->Sumw2();
//    h_CaloMETPhiinHF_S9S1->Sumw2();
//    h_CaloMExinHF_S9S1->Sumw2();
//    h_CaloMEyinHF_S9S1->Sumw2();
//    h_CaloSumETinHF_S9S1->Sumw2();
   
   /////////initialize variables

   int printout  = (int) getPreCutValue1("printout");

   //////////////////////////////
   ///// Goood Run List  ////////
   //////////////////////////////
//    int goodruns[] = {123596, 123615, 123732, 123815, 123818,
// 		     123908, 124008, 124009, 124020, 124022,
// 		     124023, 124024, 124025, 124027, 124030,
// 		     124120};
//                      //124120 at 2360 GeV
//    
//    int goodLSmin[] = {2, 70, 62, 8, 2,
//                       2, 1, 1, 12, 66,
//                       38, 2, 5, 24, 2,
//                       1};
//    
//    int goodLSmax[] = {9999, 9999, 109, 9999, 42,
//                       12, 1, 68, 94, 179,
//                       9999, 83, 13, 9999, 9999,
//                       9999};

   // 2010 runs at 7 TeV
   int goodruns[] = {132440, 132440, 132473, 132599, 132601,
                     132602, 132605, 132605, 132605, 132605,
                     132606};

   
   int goodLSmin[] = {85, 141, 1, 1, 261,
                      1, 1, 446, 624, 831,
                      1};
   
   int goodLSmax[] = {138, 401, 29, 74, 1131,
                      83, 444, 622, 829, 968,
                      37};
   
   // For S9/S1 flagging
   const double slopes[] = {0.0164905,0.0238698,0.0321383,0.041296,0.0513428,0.0622789,0.0741041,0.0868186,0.100422,0.135313,0.136289,0.0589927};
   
   int useS9oS1Algo = (int)getPreCutValue1("useS9oS1Algo");
   float metPrintoutCut = (float)getPreCutValue1("metPrintoutCut");

   Long64_t nentries = fChain->GetEntriesFast();
   std::cout << "analysisClass::Loop(): nentries = " << nentries << std::endl;   



   ////// The following ~7 lines have been taken from rootNtupleClass->Loop() /////
   ////// If the root version is updated and rootNtupleClass regenerated,     /////
   ////// these lines may need to be updated.                                 /////    
   Long64_t nb = 0;

   for (Long64_t jentry=0; jentry<nentries;jentry++) 
     {
       Long64_t ientry = LoadTree(jentry);
       if (ientry < 0) break;
       nb = fChain->GetEntry(jentry);   
       
       if(jentry < 10 || jentry%1000 == 0) std::cout << "analysisClass::Loop(): jentry = " << jentry << std::endl;   
       ////////////////////// User's code starts here ///////////////////////

       //## Event selection --> see below under fillVariables 

       //## Check if the run is in the list of good runs
       int pass_GoodRunList = 0;
       if(isData==1)
	 {
	   for (int i = 0; i < sizeof(goodruns)/sizeof(int) ; i++) {
	     if (goodruns[i] == run && ls >= goodLSmin[i] && ls <= goodLSmax[i]) {
	       pass_GoodRunList = 1;
	       break;
	     }
	   }
	 }
       else if(isData == 0)
	 {
	   pass_GoodRunList = 1;
	 }


       //## Definition of ECAL spikes

       //the ntuple maker stores already only ecal crystals with ET > 5 GeV
       vector<float> EBspikes_ET;
       vector<float> EBspikes_Px;
       vector<float> EBspikes_Py;
       
       for (int xtal=0; xtal<PMTnoiseEcalEnergy->size(); xtal++)
	 {
	   //calculate S4/S1 of crystal
	   float S4oS1 = PMTnoiseEcalS4->at(xtal) / PMTnoiseEcalEnergy->at(xtal);

	   //calculate ET of crystal
	   float etaSize_ = 0.0174;
	   float ieta_ = PMTnoiseEcalIeta->at(xtal);
	   float eta_ = etaSize_ * ( fabs(ieta_) - 0.5 );
	   if(ieta_ < 0) 
	     eta_ = eta_ * -1;
	   float ET_ = PMTnoiseEcalEnergy->at(xtal)/cosh(eta_);
           float iphi_ = PMTnoiseEcalIphi->at(xtal);
           float phi_ = etaSize_ * ( iphi_ - 0.5 );
           float px_ = ET_ * cos( phi_ );
           float py_ = ET_ * sin( phi_ );
	   // 	   cout << "ET: "     << ET_  
	   // 		<< " S4/S1: " << S4oS1 << endl;
	   if(S4oS1 < 0.05)
	     EBspikes_ET.push_back(ET_);
             EBspikes_Px.push_back(px_);
             EBspikes_Py.push_back(py_);
	 }       

       //## Check if the event passes the hcal noise filter
       int passHcalNoise = 0;
       if( HcalNoiseFilterLoose ) passHcalNoise = 1;
//        if( HcalNoiseFilterTight ) passHcalNoise = 1;
       //#################################################
       
       // Set the evaluation of the cuts to false and clear the variable values and filled status
       resetCuts();
       
       // Set the value of the variableNames listed in the cutFile to their current value

       //## Fill variables for Event selection
       if(isData==1)
	 {
	   fillVariableWithValue("passPhysDecl", PMTnoiseisPhysDeclared);
	   fillVariableWithValue("passBSCMinBias", PMTnoiseisBSCminBias);
	   fillVariableWithValue("passVetoBSChalo", !PMTnoiseisBSChalo);
	   fillVariableWithValue("passBPTX0", PMTnoiseisBPTX0);
	   fillVariableWithValue("passBeamScrap", PMTnoisePassesBeamScraping);
	   fillVariableWithValue("passGoodPV", PMTnoiseisPrimaryVertex);
	   fillVariableWithValue("passGoodRunList", pass_GoodRunList);
           fillVariableWithValue("passHcalNoise", passHcalNoise);
	 }

       if(isData==0)
	 {
	   fillVariableWithValue("passPhysDecl", 1);
	   fillVariableWithValue("passBSCMinBias", PMTnoiseisBSCminBias);
	   fillVariableWithValue("passVetoBSChalo", 1);
	   fillVariableWithValue("passBPTX0", 1);
	   fillVariableWithValue("passBeamScrap", PMTnoisePassesBeamScraping);
	   fillVariableWithValue("passGoodPV", PMTnoiseisPrimaryVertex);
	   fillVariableWithValue("passGoodRunList", pass_GoodRunList);
           fillVariableWithValue("passHcalNoise", passHcalNoise);
	 }

       //## Fill other variables
       //fillVariableWithValue("passPhysDecl", PMTnoiseisPhysDeclared);

       // Evaluate cuts (but do not apply them)
       evaluateCuts();
       
       // Fill histograms and do analysis based on cut evaluation

       if( passedCut("0") )
	 {

           //## Loop over HF rechits             

           vector<float> HFspikes_ET;
           vector<float> HFspikes_Px;
           vector<float> HFspikes_Py;
           vector<float> HFspikes_E;
           vector<int> HFspikes_ieta;

           int N_HFspikes_L=0;
           int N_HFspikes_S=0;
           int N_HFspikes_TOT=0;

	   float SumEinHF=0.;
	   // 	   float SumEinHFP=0.;
	   // 	   float SumEinHFM=0.;

	   map<UInt_t,float> SumEinHF_ieta;
	   map<UInt_t,float> N_HFspikes_L_ieta;
	   map<UInt_t,float> N_HFspikes_S_ieta;
	   map<UInt_t,float> N_HFspikes_TOT_ieta;
	   for( uint i=1; i<=5; i++ ) {
	     SumEinHF_ieta[i]=0;
	     N_HFspikes_L_ieta[i]=0;
	     N_HFspikes_S_ieta[i]=0;
	     N_HFspikes_TOT_ieta[i]=0;
	   }

	   float N_rechit_above_thr_inHF = 0;

           for (int i = 0; i<int(PMTnoiseRecHitET->size()); i++)
             {

               bool isPMThit = false;
               double energy = PMTnoiseRecHitEnergy->at(i);
               double ET = PMTnoiseRecHitET->at(i);
               double partenergy = PMTnoiseRecHitPartEnergy->at(i);
               double sum4Long = PMTnoiseRecHitSum4Long->at(i);
               double sum4Short = PMTnoiseRecHitSum4Short->at(i);
               int ieta = PMTnoiseRecHitIeta->at(i);
               int iphi = PMTnoiseRecHitIphi->at(i);
               double phi = ((2*3.14159)/72) * iphi;
               if(abs(ieta)>39) phi = ((2*3.14159)/72) * (iphi+1);
               int depth = PMTnoiseRecHitDepth->at(i);

	       //eta regions
	       int index_eta = 0;
	       if(abs(ieta)==29) index_eta = 1;
	       if(abs(ieta)>=30 && abs(ieta)<=33) index_eta = 2;
	       if(abs(ieta)>=34 && abs(ieta)<=37) index_eta = 3;
	       if(abs(ieta)>=38 && abs(ieta)<=40) index_eta = 4;
	       if(abs(ieta)==41) index_eta = 5;

	       //calculate sum of energy in HF (including spikes at this stage)
	       SumEinHF = SumEinHF + energy;
	       SumEinHF_ieta[index_eta] = SumEinHF_ieta[index_eta] + energy; 

// 	       if(ieta>0)
// 		 float SumEinHFP = SumEinHFP + energy;
// 	       if(ieta<0)
// 		 float SumEinHFM = SumEinHFM + energy;

	       //calculate occupancy in HF
	       float OccThreshold = 1;
	       if(energy > OccThreshold)
		 N_rechit_above_thr_inHF++;
 
               //skip the RecHit if it's just a pedestal noise
               if( (depth==1 && energy<1.2) || (depth==2 && energy<1.8) ) continue;
               
               //masked towers
               // HF(29,67,1): STATUS = 0x40
               // HF(29,67,2): STATUS = 0x40
               // only masked in 2009 runs
               // HF(37,67,1): STATUS = 0x8040
               // HF(35,67,1): STATUS = 0x8040
               // HF(30,67,2): STATUS = 0x8040
               // HF(32,67,2): STATUS = 0x8040
               // HF(36,67,2): STATUS = 0x8040
               // HF(38,67,2): STATUS = 0x8040

               //tower masked
               int isLongMasked=0;
               int isShortMasked=0;
//                if( (ieta==37 || ieta==29 || ieta==35) && iphi==67)
//                  isLongMasked = 1;
// 
//                if( (ieta==29 || ieta==30 || ieta==32 || ieta==36 || ieta==38) && iphi==67)
//                  isShortMasked = 1;
               // for 2010 runs
               if( ieta==29 && iphi==67) {
                 isLongMasked = 1;
                 isShortMasked = 1;
               }

               //skip the RecHit if it's in the tower with crystals mounted
               if( isLongMasked==1 || isShortMasked==1 ) continue;
               
               //R = L-S/L+S
               double R = PMTnoiseRecHitRValue->at(i);
               
               //S9/S1
               double S9oS1 = ( partenergy + sum4Long + sum4Short ) / energy;
               
               //S8/S1
               double S8oS1 = ( sum4Long + sum4Short ) / energy;

               // For S9/S1 flagging
               double slope = 0;
               if(abs(ieta)>29) slope = slopes[abs(ieta)-30];
               double intercept = -slope*log((162.4-10.19*abs(ieta)+0.21*ieta*ieta));
                
               //## identify HF spikes
               if( depth==1 ) {
                 if( useS9oS1Algo==0 ) {
                    if( energy>(162.4-10.19*abs(ieta)+0.21*ieta*ieta) && R>0.98 ) isPMThit = true;
                 }
                 else if( useS9oS1Algo==1 ) {
                    if( abs(ieta)==29 && ( energy>(162.4-10.19*abs(ieta)+0.21*ieta*ieta) && R>0.98 ) ) isPMThit = true;
                    else if( abs(ieta)>29 && ( energy>(162.4-10.19*abs(ieta)+0.21*ieta*ieta) && S9oS1<(intercept+slope*log(energy)) ) ) isPMThit = true;
                 }
               }
               else if( depth==2 && energy>(129.9-6.61*abs(ieta)+0.1153*ieta*ieta) && R<-0.9 ) isPMThit = true;
               
               if( depth==1 && isPMThit )
                 {
                   N_HFspikes_L++;
		   N_HFspikes_L_ieta[index_eta] = N_HFspikes_L_ieta[index_eta] + 1;

                   HFspikes_ET.push_back(ET);
                   HFspikes_Px.push_back(ET*cos(phi));
                   HFspikes_Py.push_back(ET*sin(phi));
                   HFspikes_E.push_back(energy);
		   HFspikes_ieta.push_back(ieta);

                   h2_N_HFspikes_L_ieta_iphi->Fill(abs(ieta),iphi);
                   h_HFRecHitE_L_Flagged->Fill( energy );
                   h_HFRecHitE_L_Flagged_ieta[abs(ieta)]->Fill( energy );
                   h_HFRecHitET_L_Flagged->Fill( ET );
                   h_HFRecHitET_L_Flagged_ieta[abs(ieta)]->Fill( ET );
                   h2_S9oS1_vs_E_L_Flagged->Fill( energy, S9oS1 );
                   h2_S8oS1_vs_E_L_Flagged->Fill( energy, S8oS1 );
                   h2_S9oS1_vs_E_L_Flagged_ieta[abs(ieta)]->Fill( energy, S9oS1 );
                   h2_S8oS1_vs_E_L_Flagged_ieta[abs(ieta)]->Fill( energy, S8oS1 );
                 }
               else if( depth==2 && isPMThit )
                 {
                   N_HFspikes_S++;
		   N_HFspikes_S_ieta[index_eta] = N_HFspikes_S_ieta[index_eta] + 1;
                 
                   HFspikes_ET.push_back(ET);
                   HFspikes_Px.push_back(ET*cos(phi));
                   HFspikes_Py.push_back(ET*sin(phi));
                   HFspikes_E.push_back(energy);
		   HFspikes_ieta.push_back(ieta);

                   h2_N_HFspikes_S_ieta_iphi->Fill(abs(ieta),iphi);
                   h_HFRecHitE_S_Flagged->Fill( energy );
                   h_HFRecHitE_S_Flagged_ieta[abs(ieta)]->Fill( energy );
                   h_HFRecHitET_S_Flagged->Fill( ET );
                   h_HFRecHitET_S_Flagged_ieta[abs(ieta)]->Fill( ET );
                   h2_S9oS1_vs_E_S_Flagged->Fill( energy, S9oS1 );
                   h2_S8oS1_vs_E_S_Flagged->Fill( energy, S8oS1 );
                   h2_S9oS1_vs_E_S_Flagged_ieta[abs(ieta)]->Fill( energy, S9oS1 );
                   h2_S8oS1_vs_E_S_Flagged_ieta[abs(ieta)]->Fill( energy, S8oS1 );
                 }//## end of identify HF spikes

               if( depth==1 ) {
                 h2_HFRecHitE_vs_R_L->Fill( R, energy );
                 h2_HFRecHitET_vs_R_L->Fill( R, ET );
                 h2_HFRecHitE_vs_R_L_ieta[abs(ieta)]->Fill( R, energy );
                 h2_HFRecHitET_vs_R_L_ieta[abs(ieta)]->Fill( R, ET );
                 h2_HFRecHitE_L_vs_S->Fill( partenergy, energy );
                 h2_HFRecHitET_L_vs_S->Fill( partenergy*(ET/energy), ET );
                 h_HFRecHitE_L->Fill( energy );
                 h_HFRecHitE_L_ieta[abs(ieta)]->Fill( energy );
                 h_HFRecHitET_L->Fill( ET );
                 h_HFRecHitET_L_ieta[abs(ieta)]->Fill( ET );
                 h2_R_vs_iphi_L->Fill( iphi, R );
                 h2_R_vs_ieta_L->Fill( abs(ieta), R );
                 h2_S9oS1_vs_E_L->Fill( energy, S9oS1 );
                 h2_S8oS1_vs_E_L->Fill( energy, S8oS1 );
                 h2_S9oS1_vs_E_L_ieta[abs(ieta)]->Fill( energy, S9oS1 );
                 h2_S8oS1_vs_E_L_ieta[abs(ieta)]->Fill( energy, S8oS1 );
               }

               if( depth==2 ) {
                 h2_HFRecHitE_vs_R_S->Fill( R, energy );
                 h2_HFRecHitET_vs_R_S->Fill( R, ET );
                 h2_HFRecHitE_vs_R_S_ieta[abs(ieta)]->Fill( R, energy );
                 h2_HFRecHitET_vs_R_S_ieta[abs(ieta)]->Fill( R, ET );
                 if(partenergy<1.2) h2_HFRecHitE_L_vs_S->Fill( energy, partenergy ); // if(...) necessary to avoid double-counting
                 if(partenergy<1.2) h2_HFRecHitET_L_vs_S->Fill( ET, partenergy*(ET/energy) ); // if(...) necessary to avoid double-counting
                 h_HFRecHitE_S->Fill( energy );
                 h_HFRecHitE_S_ieta[abs(ieta)]->Fill( energy );
                 h_HFRecHitET_S->Fill( ET );
                 h_HFRecHitET_S_ieta[abs(ieta)]->Fill( ET );
                 h2_R_vs_iphi_S->Fill( iphi, R );
                 h2_R_vs_ieta_S->Fill( abs(ieta), R );
                 h2_S9oS1_vs_E_S->Fill( energy, S9oS1 );
                 h2_S8oS1_vs_E_S->Fill( energy, S8oS1 );
                 h2_S9oS1_vs_E_S_ieta[abs(ieta)]->Fill( energy, S9oS1 );
                 h2_S8oS1_vs_E_S_ieta[abs(ieta)]->Fill( energy, S8oS1 );
               }
               
               //## identify spikes a la caloMET
               
//                if( PMTnoiseRecHitDepth->at(i)==1 
//                    && (PMTnoiseRecHitET->at(i) + ETpart) > 5 && R > 0.99)
//                  N_HFspikes_L++;
//                
//                if( PMTnoiseRecHitDepth->at(i)==2  
//                    && (PMTnoiseRecHitET->at(i) + ETpart) > 5 && R < -0.8)
//                  N_HFspikes_S++;

             }//end loop over hf rechits

           N_HFspikes_TOT = N_HFspikes_L + N_HFspikes_S;
	   for( uint i=1; i<=5; i++ ) {
	     N_HFspikes_TOT_ieta[i] = N_HFspikes_L_ieta[i] + N_HFspikes_S_ieta[i];                             
	   }

	   h_N_HFspikesPerEv_L->Fill(N_HFspikes_L);
	   h_N_HFspikesPerEv_S->Fill(N_HFspikes_S);
	   h_N_HFspikesPerEv_TOT->Fill(N_HFspikes_TOT);

	   //N. HF spikes vs SumE in HF
	   for(int spike=0; spike<HFspikes_E.size(); spike++)
	     {
	       SumEinHF = SumEinHF - HFspikes_E[spike];	      
	       //eta regions
	       int index_eta = 0;
	       if(abs(HFspikes_ieta[spike])==29) index_eta = 1;
	       if(abs(HFspikes_ieta[spike])>=30 && abs(HFspikes_ieta[spike])<=33) index_eta = 2;
	       if(abs(HFspikes_ieta[spike])>=34 && abs(HFspikes_ieta[spike])<=37) index_eta = 3;
	       if(abs(HFspikes_ieta[spike])>=38 && abs(HFspikes_ieta[spike])<=40) index_eta = 4;
	       if(abs(HFspikes_ieta[spike])==41) index_eta = 5;	       
	       SumEinHF_ieta[index_eta] = SumEinHF_ieta[index_eta] - HFspikes_E[spike];	       
	     }

	   h_SumEinHF->Fill(SumEinHF);
	   p_NspikesInHF_vs_SumEinHF->Fill(SumEinHF,N_HFspikes_TOT);
	   p_NspikesInHF_L_vs_SumEinHF->Fill(SumEinHF,N_HFspikes_L);
	   p_NspikesInHF_S_vs_SumEinHF->Fill(SumEinHF,N_HFspikes_S);
	   for( uint i=1; i<=5; i++ ) {
	     p_NspikesInHF_vs_SumEinHF_ieta[i]->Fill(SumEinHF_ieta[i],N_HFspikes_TOT_ieta[i]);
	   }

	   //HF occupancy (above thr.) vs SumE in HF
	   float occupancyInHF = float( ( N_rechit_above_thr_inHF - HFspikes_E.size() ) / N_ch_TOT_inHF);
	   p_OccupancyHF_vs_SumEinHF->Fill(SumEinHF,occupancyInHF);

	   //ECAL spikes
	   h_N_EBspikesPerEv_TOT->Fill(EBspikes_ET.size());
           
           // met before cleaning
           double met_clean = PMTnoiseMET->at(0);
           double metphi_clean = PMTnoiseMETphi->at(0);
           double mex_clean = met_clean*cos(metphi_clean);
           double mey_clean = met_clean*sin(metphi_clean);
           double sumet_clean = PMTnoiseSUMET->at(0);

           if( passedCut("all") ) {
             h_CaloMET->Fill( met_clean );
             h_CaloMETPhi->Fill( metphi_clean );
             h_CaloMEx->Fill( mex_clean );
             h_CaloMEy->Fill( mey_clean );
             h_CaloSumET->Fill( sumet_clean );
           }
           // tcmet before cleaning
           double tcmet_clean = tcmetPt->at(0);
           double tcmetphi_clean = tcmetPhi->at(0);
           double tcmex_clean = tcmetPx->at(0);
           double tcmey_clean = tcmetPy->at(0);
           double tcsumet_clean = tcmetSumEt->at(0);

           if( passedCut("all") ) {
             h_tcMET->Fill( tcmet_clean );
             h_tcMETPhi->Fill( tcmetphi_clean );
             h_tcMEx->Fill( tcmex_clean );
             h_tcMEy->Fill( tcmey_clean );
             h_tcSumET->Fill( tcsumet_clean );
           }
           // pfmet
           if( passedCut("all") ) {
             h_pfMET->Fill( pfmetPt->at(0) );
             h_pfMETPhi->Fill( pfmetPhi->at(0) );
             h_pfMEx->Fill( pfmetPx->at(0) );
             h_pfMEy->Fill( pfmetPy->at(0) );
             h_pfSumET->Fill( pfmetSumEt->at(0) );
           }
           
           //## clean ECAL spikes (in CMSSW_3_5_X>=3_5_6 this is done in standard reco)
//            for (int i=0; i<EBspikes_ET.size(); i++)
//              {
//                mex_clean += EBspikes_Px[i];
//                mey_clean += EBspikes_Py[i];
//                sumet_clean -= EBspikes_ET[i]; 
//              }
//            
//            //## re-calculate met pt and phi after ECAL spike cleaning
//            met_clean = sqrt( mex_clean * mex_clean + mey_clean * mey_clean );
//            metphi_clean = atan2( mey_clean, mex_clean );
//            
//            h_CaloMET_cleanECAL->Fill( met_clean );
//            h_CaloMETPhi_cleanECAL->Fill( metphi_clean );
//            h_CaloMEx_cleanECAL->Fill( mex_clean );
//            h_CaloMEy_cleanECAL->Fill( mey_clean );
//            h_CaloSumET_cleanECAL->Fill( sumet_clean );

           double mexHF_clean = PMTnoiseMETHFM->at(0)*cos( PMTnoiseMETphiHFM->at(0) ) + PMTnoiseMETHFP->at(0)*cos( PMTnoiseMETphiHFP->at(0) );
           double meyHF_clean = PMTnoiseMETHFM->at(0)*sin( PMTnoiseMETphiHFM->at(0) ) + PMTnoiseMETHFP->at(0)*sin( PMTnoiseMETphiHFP->at(0) );
           double metHF_clean = sqrt( mexHF_clean * mexHF_clean + meyHF_clean * meyHF_clean );
           double metphiHF_clean = atan2( meyHF_clean, mexHF_clean );
           double sumetHF_clean = PMTnoiseSUMETHFM->at(0) + PMTnoiseSUMETHFP->at(0);
           
           if( passedCut("all") ) {
             h_CaloMETHF->Fill( metHF_clean );
             h_CaloMETPhiHF->Fill( metphiHF_clean );
             h_CaloMExHF->Fill( mexHF_clean );
             h_CaloMEyHF->Fill( meyHF_clean );
             h_CaloSumETHF->Fill( sumetHF_clean );
           }
           //## clean HF spikes
           for (int i=0; i<HFspikes_ET.size(); i++)
             {
               mex_clean += HFspikes_Px[i];
               mey_clean += HFspikes_Py[i];
               sumet_clean -= HFspikes_ET[i];
               
               tcmex_clean += HFspikes_Px[i];
               tcmey_clean += HFspikes_Py[i];
               tcsumet_clean -= HFspikes_ET[i];
               
               mexHF_clean += HFspikes_Px[i];
               meyHF_clean += HFspikes_Py[i];
               sumetHF_clean -= HFspikes_ET[i];
             }
           
           //## re-calculate met pt and phi after HF spike cleaning
           met_clean = sqrt( mex_clean * mex_clean + mey_clean * mey_clean );
           metphi_clean = atan2( mey_clean, mex_clean );
           
           tcmet_clean = sqrt( tcmex_clean * tcmex_clean + tcmey_clean * tcmey_clean );
           tcmetphi_clean = atan2( tcmey_clean, tcmex_clean );
           
           metHF_clean = sqrt( mexHF_clean * mexHF_clean + meyHF_clean * meyHF_clean );
           metphiHF_clean = atan2( meyHF_clean, mexHF_clean );
           
           if( passedCut("all") ) {
             h_CaloMET_clean->Fill( met_clean );
             h_CaloMETPhi_clean->Fill( metphi_clean );
             h_CaloMEx_clean->Fill( mex_clean );
             h_CaloMEy_clean->Fill( mey_clean );
             h_CaloSumET_clean->Fill( sumet_clean );
             
             h_tcMET_clean->Fill( tcmet_clean );
             h_tcMETPhi_clean->Fill( tcmetphi_clean );
             h_tcMEx_clean->Fill( tcmex_clean );
             h_tcMEy_clean->Fill( tcmey_clean );
             h_tcSumET_clean->Fill( tcsumet_clean );
             
             h_CaloMETHF_clean->Fill( metHF_clean );
             h_CaloMETPhiHF_clean->Fill( metphiHF_clean );
             h_CaloMExHF_clean->Fill( mexHF_clean );
             h_CaloMEyHF_clean->Fill( meyHF_clean );
             h_CaloSumETHF_clean->Fill( sumetHF_clean );
           }
           
           // CaloMET for events rejected by HcalNoiseSummary object
           if( printout && !passedCut("passHcalNoise") ) 
             {
               h_CaloMEThcalNoise->Fill( met_clean );
               cout << "event: " << event << " " 
                    << "ls: " << ls << " "
                    << "run: " << run << "  "
                    << "--  CaloMEThcalNoise : " <<  met_clean << " "
                    << endl;
             }
            
	   if(printout)
	     {
	       if( met_clean > metPrintoutCut )
		 {
		   cout << "event: " << event << " " 
			<< "ls: " << ls << " "
			<< "run: " << run << "  "
			<< "--  CaloMET_clean : " <<  met_clean << " "
			<< endl;
		 }
	     }
           
	   //## JetMET variables
	   if(useJetMETVariables)
	     {

// 	       int NjetInHF=0;
// 	       int NjetInHF_PET=0;
// 	       int NjetInHF_S9S1=0;
// 	       int NjetInHF_ptCut=0;
// 	       int NjetInHF_ptCut_PET=0;
// 	       int NjetInHF_ptCut_S9S1=0;
// 
// 	       //-- no cleaning --
// 	       for(int jet=0; jet<ak5JetpT->size(); jet++)
// 		 {		   
// 		   //only jets in HF
// 		   if(fabs(ak5JetEta->at(jet))<3.0)
// 		     continue;
// 		  
// 		   NjetInHF++;
// 		   if( ak5JetpT->at(jet) > JetPtCut )
// 		     NjetInHF_ptCut++;
// 
// 		   h_JetInHF_pT->Fill( ak5JetpT->at(jet) );
// 		   h_JetInHF_eta->Fill( ak5JetEta->at(jet) );
// 		   h_JetInHF_phi->Fill( ak5JetPhi->at(jet) );
// 		   h_JetInHF_Nconst->Fill( ak5JetNConstituents->at(jet) );
// 		   h_JetInHF_N60->Fill( ak5JetN60->at(jet) );
// 		   h_JetInHF_N90->Fill( ak5JetN90->at(jet) );		   
// 		 }
// 	       h_JetInHF_Njet->Fill(NjetInHF);
// 	       h_JetInHF_Njet_ptcut->Fill(NjetInHF_ptCut);
// 
//                double mexinHF = calometMETInmHF->at(0)*cos( calometMETPhiInmHF->at(0) ) + calometMETInpHF->at(0)*cos( calometMETPhiInpHF->at(0) );
//                double meyinHF = calometMETInmHF->at(0)*sin( calometMETPhiInmHF->at(0) ) + calometMETInpHF->at(0)*sin( calometMETPhiInpHF->at(0) );
//                double metinHF = sqrt( mexinHF * mexinHF + meyinHF * meyinHF );
//                double metphiinHF = atan2( meyinHF, mexinHF );
//                double sumetinHF = calometSETInmHF->at(0) + calometSETInpHF->at(0);
//                
//                h_CaloMETinHF->Fill( metinHF );
//                h_CaloMETPhiinHF->Fill( metphiinHF );
//                h_CaloMExinHF->Fill( mexinHF );
//                h_CaloMEyinHF->Fill( meyinHF );
//                h_CaloSumETinHF->Fill( sumetinHF );
//                
// 	       //-- PET cleaning --
// 	       for(int jet=0; jet<ak5JetpTPET->size(); jet++)
// 		 {		   
// 		   //only jets in HF
// 		   if(fabs(ak5JetEtaPET->at(jet))<3.0)
// 		     continue;
// 		  
// 		   NjetInHF_PET++;
// 		   if( ak5JetpTPET->at(jet) > JetPtCut )
// 		     NjetInHF_ptCut_PET++;
// 
// 		   h_JetInHF_pT_PET->Fill( ak5JetpTPET->at(jet) );
// 		   h_JetInHF_eta_PET->Fill( ak5JetEtaPET->at(jet) );
// 		   h_JetInHF_phi_PET->Fill( ak5JetPhiPET->at(jet) );
// 		   h_JetInHF_Nconst_PET->Fill( ak5JetNConstituentsPET->at(jet) );
// 		   h_JetInHF_N60_PET->Fill( ak5JetN60PET->at(jet) );
// 		   h_JetInHF_N90_PET->Fill( ak5JetN90PET->at(jet) );		   
// 		 }
// 	       h_JetInHF_Njet_PET->Fill(NjetInHF_PET);
// 	       h_JetInHF_Njet_ptcut_PET->Fill(NjetInHF_ptCut_PET);
// 
//                mexinHF = calometMETInmHFPET->at(0)*cos( calometMETPhiInmHFPET->at(0) ) + calometMETInpHFPET->at(0)*cos( calometMETPhiInpHFPET->at(0) );
//                meyinHF = calometMETInmHFPET->at(0)*sin( calometMETPhiInmHFPET->at(0) ) + calometMETInpHFPET->at(0)*sin( calometMETPhiInpHFPET->at(0) );
//                metinHF = sqrt( mexinHF * mexinHF + meyinHF * meyinHF );
//                metphiinHF = atan2( meyinHF, mexinHF );
//                sumetinHF = calometSETInmHFPET->at(0) + calometSETInpHFPET->at(0);
//                
//                h_CaloMETinHF_PET->Fill( metinHF );
//                h_CaloMETPhiinHF_PET->Fill( metphiinHF );
//                h_CaloMExinHF_PET->Fill( mexinHF );
//                h_CaloMEyinHF_PET->Fill( meyinHF );
//                h_CaloSumETinHF_PET->Fill( sumetinHF );
//                
// 	       //-- S9S1 cleaning --
// 	       for(int jet=0; jet<ak5JetpTS9S1->size(); jet++)
// 		 {		   
// 		   //only jets in HF
// 		   if(fabs(ak5JetEtaS9S1->at(jet))<3.0)
// 		     continue;
// 		  
// 		   NjetInHF_S9S1++;
// 		   if( ak5JetpTS9S1->at(jet) > JetPtCut )
// 		     NjetInHF_ptCut_S9S1++;
// 
// 		   h_JetInHF_pT_S9S1->Fill( ak5JetpTS9S1->at(jet) );
// 		   h_JetInHF_eta_S9S1->Fill( ak5JetEtaS9S1->at(jet) );
// 		   h_JetInHF_phi_S9S1->Fill( ak5JetPhiS9S1->at(jet) );
// 		   h_JetInHF_Nconst_S9S1->Fill( ak5JetNConstituentsS9S1->at(jet) );
// 		   h_JetInHF_N60_S9S1->Fill( ak5JetN60S9S1->at(jet) );
// 		   h_JetInHF_N90_S9S1->Fill( ak5JetN90S9S1->at(jet) );		   
// 		 }
// 	       h_JetInHF_Njet_S9S1->Fill(NjetInHF_S9S1);
// 	       h_JetInHF_Njet_ptcut_S9S1->Fill(NjetInHF_ptCut_S9S1);
//                
//                mexinHF = calometMETInmHFS9S1->at(0)*cos( calometMETPhiInmHFS9S1->at(0) ) + calometMETInpHFS9S1->at(0)*cos( calometMETPhiInpHFS9S1->at(0) );
//                meyinHF = calometMETInmHFS9S1->at(0)*sin( calometMETPhiInmHFS9S1->at(0) ) + calometMETInpHFS9S1->at(0)*sin( calometMETPhiInpHFS9S1->at(0) );
//                metinHF = sqrt( mexinHF * mexinHF + meyinHF * meyinHF );
//                metphiinHF = atan2( meyinHF, mexinHF );
//                sumetinHF = calometSETInmHFS9S1->at(0) + calometSETInpHFS9S1->at(0);
//                
//                h_CaloMETinHF_S9S1->Fill( metinHF );
//                h_CaloMETPhiinHF_S9S1->Fill( metphiinHF );
//                h_CaloMExinHF_S9S1->Fill( mexinHF );
//                h_CaloMEyinHF_S9S1->Fill( meyinHF );
//                h_CaloSumETinHF_S9S1->Fill( sumetinHF );
	      
	     }//end of JetMET variables

	 }//end pass all cut level 0

       // retrieve value of previously filled variables (after making sure that they were filled)
       //double diff;
       //if ( variableIsFilled("my_calometPt") && variableIsFilled("calometPt") ) 
       //  diff = getVariableValue("my_calometPt") - getVariableValue("calometPt");
       
       // reject events that did not pass level 0 cuts
       //if( !passedCut("0") ) continue;
       // ......     
       // reject events that did not pass level 1 cuts
       //if( !passedCut("1") ) continue;
       // ......
       // reject events that did not pass the full cut list
       //if( !passedCut("all") ) continue;
       // ......
       // reject events that did not pass the cut in parenthesis
       //if( !passedCut("my_calometPt") )  continue;      
       
       
       ////////////////////// User's code ends here ///////////////////////
       
     } // End loop over events


//    p_NspikesInHF_L_vs_SumEinHF->Fill(SumEinHF,N_HFspikes_L);
//    p_NspikesInHF_S_vs_SumEinHF->Fill(SumEinHF,N_HFspikes_S);
//    for( uint i=1; i<=5; i++ ) {
//      p_NspikesInHF_vs_SumEinHF_ieta[i]->Fill(SumEinHF_ieta[i],N_HFspikes_TOT_ieta[i]);
//    }
   
//    //HF occupancy (above thr.) vs SumE in HF
//    float occupancyInHF = float( ( N_rechit_above_thr_inHF - HFspikes_E.size() ) / N_ch_TOT_inHF);
//    p_OccupancyHF_vs_SumEinHF->Fill(SumEinHF,occupancyInHF);

   
   //////////write histos 
   
   //TH1D * h_example = new TH1D ("h_example","", getHistoNBins("my_calometPt"), getHistoMin("my_calometPt"), getHistoMax("my_calometPt"));
   //h_example->Add( & getHisto_noCuts_or_skim("my_calometPt") ); // all histos can be retrieved, see other getHisto_xxxx methods in baseClass.h

   h_N_HFspikesPerEv_L->Write();
   h_N_HFspikesPerEv_S->Write();
   h_N_HFspikesPerEv_TOT->Write();
   h_N_EBspikesPerEv_TOT->Write();

   h_HFRecHitE_L->Write();
   h_HFRecHitE_L_Flagged->Write();
   h_HFRecHitE_S->Write();
   h_HFRecHitE_S_Flagged->Write();
   
   for(map<UInt_t,TH1D*>::const_iterator it = h_HFRecHitE_L_ieta.begin(); it != h_HFRecHitE_L_ieta.end(); it++) it->second->Write();
   for(map<UInt_t,TH1D*>::const_iterator it = h_HFRecHitE_L_Flagged_ieta.begin(); it != h_HFRecHitE_L_Flagged_ieta.end(); it++) it->second->Write();
   for(map<UInt_t,TH1D*>::const_iterator it = h_HFRecHitE_S_ieta.begin(); it != h_HFRecHitE_S_ieta.end(); it++) it->second->Write();
   for(map<UInt_t,TH1D*>::const_iterator it = h_HFRecHitE_S_Flagged_ieta.begin(); it != h_HFRecHitE_S_Flagged_ieta.end(); it++) it->second->Write();
   
   h_HFRecHitET_L->Write();
   h_HFRecHitET_S->Write();
   h_HFRecHitET_L_Flagged->Write();
   h_HFRecHitET_S_Flagged->Write();
   
   for(map<UInt_t,TH1D*>::const_iterator it = h_HFRecHitET_L_ieta.begin(); it != h_HFRecHitET_L_ieta.end(); it++) it->second->Write();
   for(map<UInt_t,TH1D*>::const_iterator it = h_HFRecHitET_S_ieta.begin(); it != h_HFRecHitET_S_ieta.end(); it++) it->second->Write();
   for(map<UInt_t,TH1D*>::const_iterator it = h_HFRecHitET_L_Flagged_ieta.begin(); it != h_HFRecHitET_L_Flagged_ieta.end(); it++) it->second->Write();
   for(map<UInt_t,TH1D*>::const_iterator it = h_HFRecHitET_S_Flagged_ieta.begin(); it != h_HFRecHitET_S_Flagged_ieta.end(); it++) it->second->Write();
   
   h_CaloMET->Write();
   h_CaloMETPhi->Write();
   h_CaloMEx->Write();
   h_CaloMEy->Write();
   h_CaloSumET->Write();
   
   h_CaloMEThcalNoise->Write();
   
//    h_CaloMET_cleanECAL->Write();
//    h_CaloMETPhi_cleanECAL->Write();
//    h_CaloMEx_cleanECAL->Write();
//    h_CaloMEy_cleanECAL->Write();
//    h_CaloSumET_cleanECAL->Write();
   
   h_CaloMET_clean->Write();
   h_CaloMETPhi_clean->Write();
   h_CaloMEx_clean->Write();
   h_CaloMEy_clean->Write();
   h_CaloSumET_clean->Write();
   
   h_tcMET->Write();
   h_tcMETPhi->Write();
   h_tcMEx->Write();
   h_tcMEy->Write();
   h_tcSumET->Write();
   
   h_tcMET_clean->Write();
   h_tcMETPhi_clean->Write();
   h_tcMEx_clean->Write();
   h_tcMEy_clean->Write();
   h_tcSumET_clean->Write();
   
   h_pfMET->Write();
   h_pfMETPhi->Write();
   h_pfMEx->Write();
   h_pfMEy->Write();
   h_pfSumET->Write();
   
   h_CaloMETHF->Write();
   h_CaloMETPhiHF->Write();
   h_CaloMExHF->Write();
   h_CaloMEyHF->Write();
   h_CaloSumETHF->Write();
   
   h_CaloMETHF_clean->Write();
   h_CaloMETPhiHF_clean->Write();
   h_CaloMExHF_clean->Write();
   h_CaloMEyHF_clean->Write();
   h_CaloSumETHF_clean->Write();
   
   h2_N_HFspikes_L_ieta_iphi->Write();
   h2_N_HFspikes_S_ieta_iphi->Write();
   
   h2_HFRecHitE_L_vs_S->Write();
   h2_HFRecHitET_L_vs_S->Write();
   h2_HFRecHitE_vs_R_L->Write();
   h2_HFRecHitE_vs_R_S->Write();
   h2_HFRecHitET_vs_R_L->Write();
   h2_HFRecHitET_vs_R_S->Write();
   
   for(map<UInt_t,TH2D*>::const_iterator it = h2_HFRecHitE_vs_R_L_ieta.begin(); it != h2_HFRecHitE_vs_R_L_ieta.end(); it++) it->second->Write();
   for(map<UInt_t,TH2D*>::const_iterator it = h2_HFRecHitE_vs_R_S_ieta.begin(); it != h2_HFRecHitE_vs_R_S_ieta.end(); it++) it->second->Write();
   for(map<UInt_t,TH2D*>::const_iterator it = h2_HFRecHitET_vs_R_L_ieta.begin(); it != h2_HFRecHitET_vs_R_L_ieta.end(); it++) it->second->Write();
   for(map<UInt_t,TH2D*>::const_iterator it = h2_HFRecHitET_vs_R_S_ieta.begin(); it != h2_HFRecHitET_vs_R_S_ieta.end(); it++) it->second->Write();
   
   h2_R_vs_iphi_L->Write();
   h2_R_vs_ieta_L->Write();
   h2_R_vs_iphi_S->Write();
   h2_R_vs_ieta_S->Write();

   h2_S9oS1_vs_E_L->Write();
   h2_S9oS1_vs_E_S->Write();
   h2_S8oS1_vs_E_L->Write();
   h2_S8oS1_vs_E_S->Write();
   h2_S9oS1_vs_E_L_Flagged->Write();
   h2_S9oS1_vs_E_S_Flagged->Write();
   h2_S8oS1_vs_E_L_Flagged->Write();
   h2_S8oS1_vs_E_S_Flagged->Write();
   
   for(map<UInt_t,TH2D*>::const_iterator it = h2_S9oS1_vs_E_L_ieta.begin(); it != h2_S9oS1_vs_E_L_ieta.end(); it++) it->second->Write();
   for(map<UInt_t,TH2D*>::const_iterator it = h2_S9oS1_vs_E_S_ieta.begin(); it != h2_S9oS1_vs_E_S_ieta.end(); it++) it->second->Write();
   for(map<UInt_t,TH2D*>::const_iterator it = h2_S8oS1_vs_E_L_ieta.begin(); it != h2_S8oS1_vs_E_L_ieta.end(); it++) it->second->Write();
   for(map<UInt_t,TH2D*>::const_iterator it = h2_S8oS1_vs_E_S_ieta.begin(); it != h2_S8oS1_vs_E_S_ieta.end(); it++) it->second->Write();
   for(map<UInt_t,TH2D*>::const_iterator it = h2_S9oS1_vs_E_L_Flagged_ieta.begin(); it != h2_S9oS1_vs_E_L_Flagged_ieta.end(); it++) it->second->Write();
   for(map<UInt_t,TH2D*>::const_iterator it = h2_S9oS1_vs_E_S_Flagged_ieta.begin(); it != h2_S9oS1_vs_E_S_Flagged_ieta.end(); it++) it->second->Write();
   for(map<UInt_t,TH2D*>::const_iterator it = h2_S8oS1_vs_E_L_Flagged_ieta.begin(); it != h2_S8oS1_vs_E_L_Flagged_ieta.end(); it++) it->second->Write();
   for(map<UInt_t,TH2D*>::const_iterator it = h2_S8oS1_vs_E_S_Flagged_ieta.begin(); it != h2_S8oS1_vs_E_S_Flagged_ieta.end(); it++) it->second->Write();

   h_SumEinHF->Write();
   p_NspikesInHF_vs_SumEinHF->Write(); 
   p_NspikesInHF_L_vs_SumEinHF->Write();
   p_NspikesInHF_S_vs_SumEinHF->Write();

   for( uint i=1; i<=5; i++ ) {
     p_NspikesInHF_vs_SumEinHF_ieta[i]->Write();
   }

   p_OccupancyHF_vs_SumEinHF->Write();


//    h_JetInHF_pT->Write();
//    h_JetInHF_eta->Write();
//    h_JetInHF_phi->Write();
//    h_JetInHF_Njet->Write();
//    h_JetInHF_Njet_ptcut->Write();
//    h_JetInHF_Nconst->Write();  
//    h_JetInHF_N60->Write();  
//    h_JetInHF_N90->Write(); 
//    
//    h_JetInHF_pT_PET->Write();
//    h_JetInHF_eta_PET->Write();
//    h_JetInHF_phi_PET->Write();
//    h_JetInHF_Njet_PET->Write();
//    h_JetInHF_Njet_ptcut_PET->Write();
//    h_JetInHF_Nconst_PET->Write();
//    h_JetInHF_N60_PET->Write();
//    h_JetInHF_N90_PET->Write();
//    
//    h_JetInHF_pT_S9S1->Write();
//    h_JetInHF_eta_S9S1->Write();
//    h_JetInHF_phi_S9S1->Write();
//    h_JetInHF_Njet_S9S1->Write();
//    h_JetInHF_Njet_ptcut_S9S1->Write();
//    h_JetInHF_Nconst_S9S1->Write();
//    h_JetInHF_N60_S9S1->Write();
//    h_JetInHF_N90_S9S1->Write();
//    
//    h_CaloMETinHF->Write();
//    h_CaloMETPhiinHF->Write();
//    h_CaloMExinHF->Write();
//    h_CaloMEyinHF->Write();
//    h_CaloSumETinHF->Write();
// 
//    h_CaloMETinHF_PET->Write();
//    h_CaloMETPhiinHF_PET->Write();
//    h_CaloMExinHF_PET->Write();
//    h_CaloMEyinHF_PET->Write();
//    h_CaloSumETinHF_PET->Write();
//    
//    h_CaloMETinHF_S9S1->Write();
//    h_CaloMETPhiinHF_S9S1->Write();
//    h_CaloMExinHF_S9S1->Write();
//    h_CaloMEyinHF_S9S1->Write();
//    h_CaloSumETinHF_S9S1->Write();
   
   
   std::cout << "analysisClass::Loop() ends" <<std::endl;   
}
