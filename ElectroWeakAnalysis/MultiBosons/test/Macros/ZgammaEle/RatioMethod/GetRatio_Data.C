#define GetRatio_Data_cxx
#include "GetRatio_Data.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TTree.h>
#include <TH2D.h>
#include <TString.h>
#include <TRandom.h>
#include <iostream>

void GetRatio_Data::Loop(Int_t AntiFlag, Float_t AntiUEE, Float_t AntiLEE, Float_t AntiUEB, Float_t AntiLEB, Int_t phoID_index, TString SaveFileName)
{

   //Photon ID table
   Double_t PhoIDCutEB[9][5]={{50 ,50 ,3.9,0.5 ,0.012},{2.6,50 ,2.5,0.04,0.012},
                              {2.6,50 ,1.6,0.04,0.012},{2.6,1.2,1.3,0.04,0.011},
                              {2.6,1.2,0.9,0.04,0.011},{1.6,1.2,0.7,0.01,0.011},
                              {4.2,2.2,3.5,0.05,0.5  },{4.2,2.2,2.0,0.05,0.013},  {4.2,2.2,2.0,0.05,0.5}};

   Double_t PhoIDCutEE[9][5]={{50 ,50 ,2.6,0.045,0.1  },{50 ,50 ,1.5,0.045,0.038},
                              {50 ,1.4,1.2,0.045,0.038},{1.4,0.8,1.0,0.045,0.038},
                              {1.4,0.8,0.8,0.045,0.035},{1.4,0.6,0.8,0.025,0.03 },
                              {4.2,2.2,3.5,0.05 ,0.5  },{4.2,2.2,2.0,0.05 ,0.03 },{4.2,2.2,2.0,0.05,0.5}};

   Double_t PhoIDScale[9][3]={{0.004,0.001 ,0.001},{0.004,0.001 ,0.001},{0.004,0.001,0.001},
                              {0.004,0.001 ,0.001},{0.004,0.001 ,0.001},{0.004,0.001,0.001},
                              {0.006,0.0025,0.001},{0.006,0.0025,0.001},{0.003,0.001,0.001}};

   //Some Cuts
   Float_t  HoverECut=0.5;
   Float_t  PhoEtCut =15., LJetPtCut=10;
   
   if (phoID_index>8 || phoID_index<0) phoID_index=8;//Use QCD photon ID as default
   //Some Vairables
   Int_t    PassPhoId=1, PassAntiPhoCut=1;
   Int_t    jet_index=0, IsEE=0, jet_Overlap = 0;
   Double_t dEta=0,dPhi=0;       
   Double_t LJetPt=0;           
   Float_t  EvtWeight=1.0;

   //Histogram                   
   TH1F *hIso[2][2];

   hIso[0][0]= new TH1F("IsoPho_Loose_EB", "", 500, 0, 500);
   hIso[0][1]= new TH1F("IsoPho_Loose_EE", "", 500, 0, 500);
   hIso[1][0]= new TH1F("NonIsoPho_Loose_EB", "", 500, 0, 500);
   hIso[1][1]= new TH1F("NonIsoPho_Loose_EE", "", 500, 0, 500);


   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;

   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      //data selection
      if (IsVtxGood ==0 ) continue;
      if (IsTracksGood ==0 ) continue;

      if (HLT[HLTIndex[1]]==0 && HLT[HLTIndex[0]]==0 && HLT[HLTIndex[2]]==0 && HLT[HLTIndex[3]]==0 && HLT[HLTIndex[4]]==0) continue;

      //Jet selection
      jet_index=-1;
      LJetPt = LJetPtCut;
      for (Int_t iJet=0;iJet<nJet;iJet++){

         if (fabs(jetEta[iJet]) < 2.6 && jetemEnergyFraction[iJet] <= 0.01) continue;
         if (jetfHPD[iJet] >= 0.98) continue;
         if (jetN90[iJet]  <= 1) continue;

         jet_Overlap = 0;
         for (Int_t iPho=0;iPho<nPho;iPho++){
            dEta = phoEta[iPho] - jetEta[iJet];
            dPhi = phoPhi[iPho] - jetPhi[iJet];
            if (dPhi >  3.1415927) dPhi -= 2*3.1415927;
            if (dPhi < -3.1415927) dPhi += 2*3.1415927;
            if (sqrt(pow(dEta,2)+pow(dPhi,2)) < 0.15) jet_Overlap = 1;
         }
         for (Int_t iEle=0;iEle<nEle;iEle++){
            dEta = eleEta[iEle] - jetEta[iJet];
            dPhi = elePhi[iEle] - jetPhi[iJet];
            if (dPhi >  3.1415927) dPhi -= 2*3.1415927;
            if (dPhi < -3.1415927) dPhi += 2*3.1415927;
            if (sqrt(pow(dEta,2)+pow(dPhi,2)) < 0.15) jet_Overlap = 1;
         }
         if (jet_Overlap==1) continue;
         if (jetTrg[iJet][0]!=1 && jetTrg[iJet][1]!=1 && jetTrg[iJet][2]!=1 && jetTrg[iJet][3]!=1 && jetTrg[iJet][4]!=1) continue;

         if (jetPt[iJet] > LJetPt) {
            LJetPt=jetPt[iJet];
            jet_index=iJet;
         }
      }

      if (jet_index==-1) continue;

      //Photon selection
      for (Int_t iPho=0;iPho<nPho;iPho++){
         //preselection
         PassPhoId=1, PassAntiPhoCut=1;
         if (phoEt[iPho] < PhoEtCut) continue;

         //Check HLT for different pt range
         if (phoEt[iPho] > PhoEtCut && phoEt[iPho]<40) {
           if (HLT[HLTIndex[0]]!=1)     continue;
           if (jetTrg[jet_index][0]!=1) continue;
         } else if (phoEt[iPho]>=40 && phoEt[iPho]<60) {
           if (HLT[HLTIndex[1]]!=1)     continue;
           if (jetTrg[jet_index][1]!=1) continue;
         } else if (phoEt[iPho]>=60 && phoEt[iPho]<80) {
           if (HLT[HLTIndex[2]]!=1)     continue;
           if (jetTrg[jet_index][2]!=1) continue;
         } else if (phoEt[iPho]>=80 && phoEt[iPho]<110) {
           if (HLT[HLTIndex[3]]!=1)     continue;
           if (jetTrg[jet_index][3]!=1) continue;
         } else if (phoEt[iPho]>=110){
           if (HLT[HLTIndex[4]]!=1)     continue;
           if (jetTrg[jet_index][4]!=1) continue;
         }

         if (phoOverlap[iPho] == 1) continue;
         if (phohasPixelSeed[iPho] == 1) continue;
         if (fabs(phoSCEta[iPho]) > 2.5) continue;
         if (fabs(phoSCEta[iPho]) > 1.4442 && fabs(phoSCEta[iPho]) < 1.566) continue;
         if (phoHoverE[iPho] > HoverECut) continue;

         if (fabs(phoSCEta[iPho]) < 1.4442 && phoSigmaIEtaIEta[iPho]<0.002) continue;

         //dR(Leading jet, pho) > 0.7
         dEta = phoEta[iPho] - jetEta[jet_index];
         dPhi = phoPhi[iPho] - jetPhi[jet_index];
         if (dPhi >  3.1415927) dPhi -= 2*3.1415927;
         if (dPhi < -3.1415927) dPhi += 2*3.1415927;
         if (sqrt(pow(dEta,2)+pow(dPhi,2)) < 0.7) continue;

         //Pass phoID
         //EB or EE
         if (fabs(phoSCEta[iPho]) < 1.47) IsEE=0; else IsEE=1;

         if (IsEE==0) {
           if ((phoEcalIsoDR04[iPho]-PhoIDScale[phoID_index][0]*phoEt[iPho]) > PhoIDCutEB[phoID_index][0]) PassPhoId=-1;
           if ((phoHcalIsoDR04[iPho]-PhoIDScale[phoID_index][1]*phoEt[iPho]) > PhoIDCutEB[phoID_index][1]) PassPhoId=-1;
           if ((phoTrkIsoHollowDR04[iPho]-PhoIDScale[phoID_index][2]*phoEt[iPho]) > PhoIDCutEB[phoID_index][2]) PassPhoId=-1;
           if (phoHoverE[iPho] > PhoIDCutEB[phoID_index][3]) PassPhoId=-1;
           if (phoSigmaIEtaIEta[iPho]> PhoIDCutEB[phoID_index][4]) PassPhoId=-1;
         } else if (IsEE==1) {
           if ((phoEcalIsoDR04[iPho]-PhoIDScale[phoID_index][0]*phoEt[iPho]) > PhoIDCutEE[phoID_index][0]) PassPhoId=-1;
           if ((phoHcalIsoDR04[iPho]-PhoIDScale[phoID_index][1]*phoEt[iPho]) > PhoIDCutEE[phoID_index][1]) PassPhoId=-1;
           if ((phoTrkIsoHollowDR04[iPho]-PhoIDScale[phoID_index][2]*phoEt[iPho]) > PhoIDCutEE[phoID_index][2]) PassPhoId=-1;
           if (phoHoverE[iPho] > PhoIDCutEE[phoID_index][3]) PassPhoId=-1;
           if (phoSigmaIEtaIEta[iPho]> PhoIDCutEE[phoID_index][4]) PassPhoId=-1;
         } 

         //Pass Anti-Selection 
         if (AntiFlag==0) {
            if (IsEE==0) {
              if (phoTrkIsoHollowDR04[iPho]-0.001*phoEt[iPho] < AntiLEB) PassAntiPhoCut=-1;
              if (phoTrkIsoHollowDR04[iPho]-0.001*phoEt[iPho] > AntiUEB) PassAntiPhoCut=-1;
            } else if (IsEE==1) {
              if (phoTrkIsoHollowDR04[iPho]-0.001*phoEt[iPho] < AntiLEE) PassAntiPhoCut=-1;
              if (phoTrkIsoHollowDR04[iPho]-0.001*phoEt[iPho] > AntiUEE) PassAntiPhoCut=-1;
            }
         } else {
            if (IsEE==0) {
               if (phoSigmaIEtaIEta[iPho] < AntiLEB) PassAntiPhoCut=-1;
               if (phoSigmaIEtaIEta[iPho] > AntiUEB) PassAntiPhoCut=-1;
            } else if (IsEE==1) {
               if (phoSigmaIEtaIEta[iPho] < AntiLEE) PassAntiPhoCut=-1;
               if (phoSigmaIEtaIEta[iPho] > AntiUEE) PassAntiPhoCut=-1;
            } 
         }
         //fill iso-photon
         if (PassPhoId==1) {
           hIso[0][IsEE]->Fill(phoEt[iPho],EvtWeight);
         }

         //fill non-iso-photon
         if (PassAntiPhoCut==1) {
           hIso[1][IsEE]->Fill(phoEt[iPho],EvtWeight);
         }
      }
   }

   TFile *f1 = new TFile(SaveFileName+".root","RECREATE");
   f1->cd();
     for (Int_t ii=0;ii<2;ii++){
       hIso[0][ii]   -> Write();
       hIso[1][ii]   -> Write();
     }
   f1->Close();
}
