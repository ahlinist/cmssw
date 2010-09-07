#define ZgSelectData_cxx
#include "ZgSelectData.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <TTree.h>      
#include <TLorentzVector.h>
#include <TH2D.h>
#include <TString.h>
#include <iomanip>

void ZgSelectData::Loop(Int_t eleID_index, Int_t phoID_index, Float_t DelRCut, Float_t ZMassCutL, Float_t ZMassCutU, TString OutputRootFile)
{
   Double_t PhoIDCutEB[8][5]={{50 ,50 ,3.9,0.5 ,0.012},{2.6,50 ,2.5,0.04,0.012},
                              {2.6,50 ,1.6,0.04,0.012},{2.6,1.2,1.3,0.04,0.011},
                              {2.6,1.2,0.9,0.04,0.011},{1.6,1.2,0.7,0.01,0.011},
                              {4.2,2.2,2.0,0.05,0.5  },{4.2,2.2,2.0,0.05,0.01}};

   Double_t PhoIDCutEE[8][5]={{50 ,50 ,2.6,0.045,0.1  },{50 ,50 ,1.5,0.045,0.038},
                              {50 ,1.4,1.2,0.045,0.038},{1.4,0.8,1.0,0.045,0.038},
                              {1.4,0.8,0.8,0.045,0.035},{1.4,0.6,0.8,0.025,0.03 },
                              {4.2,2.2,2.0,0.05 ,0.5  },{4.2,2.2,2.0,0.05 ,0.03 }};

   TString phoID_String[8]={"Poter's95","Poter's90","Poter's85","Poter's80","Poter's70","Poter's60","EG Loose","EG Tight"};
   TString eleID_String[12]={"ComIso60","ComIso70","ComIso80","ComIso685","ComIso90","ComIso95","RelIso60","RelIso70","RelIso80","RelIso85","RelIso90","RelIso95"};

   float ElePtCut(20),  PhoPtCut(10);

   Int_t     nWrite=0, passPhoID=0;   
   Int_t     InEE_EB=0, PhoInEE=0, Ele1InEE=0, Ele2InEE=0;
   Int_t     EleN_num=0,EleP_num=0;
   Int_t     PassEIDN[65], PassEIDP[65];
   Int_t     PassTagN[65], PassTagP[65];
   Int_t     ele1_index=0, ele2_index=0, pho_index=0;

   Float_t   ThisValue=0;
   Float_t   MZee=0,dMZee=0;
   Float_t   MC_dPhi=0, MC_dEta=0, DelR1=0, DelR2=0;

   Double_t  EvtWeight=1;
   Long64_t  nZgCandidates=0;

   TLorentzVector ele1,ele2,gamma,Zee,Mllg;

   TH1D *h1Et[3], *h2Pt[3];
   TH1D *heEta[3],*hpEta[3];
   TH1D *hpVar[3][6][2], *heVar[3][8][2];
   TH1D *hdR[3];              
   TH1D *hMee[3], *hMeeg[3];

   TH2D *hMeeMeeg = new TH2D("hMeeMeeg","M_{ee} vs. M_{ee#gamma}",40,0,200,40,0,200);  
 
   TString TName_Var[8]={"EIso","HIso","TIso","CIso","SIE","HoE","dEtaIn","dPhiIn"};
   TString TName_EE[2]={"EB","EE"};
   TString TName_Al;
   TString TName_Num[3]={"1","2","3"};
   
   Int_t   nBin[8]={70,70,70,70,100,15,100,100};
   Float_t uBin[8]={34,34,34,34,0.1,0.15,0.1,0.1};
   Float_t lBin[8]={-1,-1,-1,-1,0,0,0,0};

   for (Int_t ii=0;ii<3;ii++){
     TName_Al="hMee_"+TName_Num[ii];
     hMee[ii]  = new TH1D(TName_Al,TName_Al,40,0,200);
     TName_Al="hMeeg_"+TName_Num[ii];
     hMeeg[ii] = new TH1D(TName_Al,TName_Al,40,0,200);
   
     TName_Al="hpEt"+TName_Num[ii];
     h1Et[ii]  = new TH1D(TName_Al,TName_Al,40,0,200);

     TName_Al="heEt"+TName_Num[ii];
     h2Pt[ii]  = new TH1D(TName_Al,TName_Al,40,0,200);

     TName_Al="hdR"+TName_Num[ii];
     hdR[ii] = new TH1D(TName_Al,TName_Al,100,0,5);

     TName_Al="heEta"+TName_Num[ii];
     heEta[ii] = new TH1D(TName_Al,TName_Al,30,-3,3);
     TName_Al="hpEta"+TName_Num[ii];
     hpEta[ii] = new TH1D(TName_Al,TName_Al,30,-3,3);

     for (Int_t jj=0;jj<2;jj++){
       for (Int_t kk=0;kk<8;kk++){
          TName_Al="he_"+TName_Var[kk]+"_"+TName_EE[jj]+"_"+TName_Num[ii];
          heVar[ii][kk][jj] = new TH1D(TName_Al, TName_Al, nBin[kk], lBin[kk], uBin[kk]);
       }
       for (Int_t kk=0;kk<6;kk++){
          TName_Al="hp_"+TName_Var[kk]+"_"+TName_EE[jj]+"_"+TName_Num[ii];
          hpVar[ii][kk][jj] = new TH1D(TName_Al, TName_Al, nBin[kk], lBin[kk], uBin[kk]);
       }
     }
   }

   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t ThisRunN=0;

   Long64_t nbytes = 0, nb = 0;

   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      if (nVtx ==0 ) continue;
      if (IsVtxGood ==0 ) continue;
      if (IsTracksGood ==0 ) continue;

      //----Skim: 1 phoEt>10 && 1 lepton pt>15
      EleN_num=0;
      for (Int_t iPho=0;iPho<nPho;iPho++){
        if (phoEt[iPho] < 10) continue;
        if (phoOverlap[iPho] == 1) continue;
        EleN_num++;
      }  
      if (EleN_num==0) continue;

      EleN_num=0,EleP_num=0;
      for (Int_t iEle=0;iEle<nEle;iEle++){
        if (elePt[iEle] < 15) continue;
        EleN_num++;
      }
      for (Int_t iMu=0;iMu<nMu;iMu++){
        if (muPt[iMu] < 15) continue;
        EleP_num++;
      }
      if (EleN_num==0 && EleP_num==0) continue;
      //----

      //HLT, HLT_Ele15_SW_L1R:HLTIndex[9] HLT_Ele15_LW_L1R:HLTIndex[7] HLT_Ele15_EleId_L1R:HLTIndex[14] 
      if (HLT[HLTIndex[14]]!=1 && HLT[HLTIndex[7]] !=1 && HLT[HLTIndex[9]] !=1) continue;

      for (Int_t ii=0;ii<65;ii++) {
        PassEIDN[ii]=-1;
        PassEIDP[ii]=-1;
            
        PassTagN[ii]=-1;
        PassTagP[ii]=-1;
      } 

      EleN_num=0,EleP_num=0;//1
      Ele1InEE=-1, Ele2InEE=-1, PhoInEE=-1;


      for (Int_t iEle=0;iEle<nEle;iEle++){
         if (elePt[iEle]<ElePtCut) continue;
         if (fabs(eleSCEta[iEle])>2.5) continue;
         if ( fabs(eleSCEta[iEle])>1.4442 && fabs(eleSCEta[iEle])<1.566) continue;

         if (fabs(eleSCEta[iEle]) < 1.4442) {
           if (eleRecoFlag[iEle]==2) continue;
           if (eleSeverity[iEle]==3) continue;
           if (eleSeverity[iEle]==4) continue;
         }

         if ( fabs(eleSCEta[iEle]) < 1.4442 ) Ele1InEE = 0;
         else if (fabs(eleSCEta[iEle])<2.56 && fabs(eleSCEta[iEle])>1.566) Ele1InEE = 1;

         heEta[0]->Fill(eleEta[iEle],EvtWeight);
         h2Pt[0]->Fill(elePt[iEle],EvtWeight);

         heVar[0][0][Ele1InEE] -> Fill(eleIsoEcalDR03[iEle],EvtWeight);
         heVar[0][1][Ele1InEE] -> Fill(eleIsoHcalDR03[iEle],EvtWeight);
         heVar[0][2][Ele1InEE] -> Fill(eleIsoTrkDR03[iEle],EvtWeight);
         heVar[0][3][Ele1InEE] -> Fill((eleIsoTrkDR03[iEle]+eleIsoHcalDR03[iEle]+eleIsoEcalDR03[iEle]),EvtWeight);
         heVar[0][4][Ele1InEE] -> Fill(eleSigmaIEtaIEta[iEle],EvtWeight);
         heVar[0][5][Ele1InEE] -> Fill(eleHoverE[iEle],EvtWeight);
         heVar[0][6][Ele1InEE] -> Fill(fabs(eledEtaAtVtx[iEle]),EvtWeight);
         heVar[0][7][Ele1InEE] -> Fill(fabs(eledPhiAtVtx[iEle]),EvtWeight);

         if (eleID[iEle][eleID_index]!=7) continue;
         if (eleCharge[iEle]==1){
            PassEIDP[EleP_num]=iEle;
            EleP_num++;
         } else if (eleCharge[iEle]==-1){
            PassEIDN[EleN_num]=iEle;
            EleN_num++;
         }
      }

      MZee=0;
      Ele1InEE=-1, Ele2InEE=-1, PhoInEE=-1;
      if (EleN_num>=1 && EleP_num>=1) {
        //(3) pass the Z mass cut, no cut for now... 
        dMZee=999.0;
        ThisValue=0,MZee=0;

        for (Int_t i1=0;i1<EleP_num;i1++){
          for (Int_t i2=0;i2<EleN_num;i2++){
            ele1.SetPtEtaPhiE(elePt[PassEIDP[i1]],eleEta[PassEIDP[i1]],elePhi[PassEIDP[i1]],eleEn[PassEIDP[i1]]);
            ele2.SetPtEtaPhiE(elePt[PassEIDN[i2]],eleEta[PassEIDN[i2]],elePhi[PassEIDN[i2]],eleEn[PassEIDN[i2]]);
            Zee=ele1+ele2;
            ThisValue=Zee.M();

            if (fabs(ThisValue-91.18)<dMZee){
              dMZee=fabs(ThisValue-91.18);
              ele1_index=PassEIDP[i1], ele2_index=PassEIDN[i2];
              MZee=ThisValue;
            }
        } }
      }

      if (MZee > ZMassCutL && MZee < ZMassCutU) {

        if ( fabs(eleSCEta[ele1_index]) < 1.4442 ) Ele1InEE = 0;
        else if (fabs(eleSCEta[ele1_index])<2.56 && fabs(eleSCEta[ele1_index])>1.566) Ele1InEE = 1;
         
        if ( fabs(eleSCEta[ele2_index]) < 1.4442 ) Ele2InEE = 0;
        else if (fabs(eleSCEta[ele2_index])<2.56 && fabs(eleSCEta[ele2_index])>1.566) Ele2InEE = 1;

        h2Pt[1]->Fill(elePt[ele1_index],EvtWeight);
        h2Pt[1]->Fill(elePt[ele2_index],EvtWeight);
        heEta[1]->Fill(eleEta[ele1_index],EvtWeight);
        heEta[1]->Fill(eleEta[ele2_index],EvtWeight);

        heVar[1][0][Ele1InEE] -> Fill(eleIsoEcalDR03[ele1_index],EvtWeight);
        heVar[1][1][Ele1InEE] -> Fill(eleIsoHcalDR03[ele1_index],EvtWeight);
        heVar[1][2][Ele1InEE] -> Fill(eleIsoTrkDR03[ele1_index],EvtWeight);
        heVar[1][3][Ele1InEE] -> Fill((eleIsoTrkDR03[ele1_index]+eleIsoHcalDR03[ele1_index]+eleIsoEcalDR03[ele1_index]),EvtWeight);
        heVar[1][4][Ele1InEE] -> Fill(eleSigmaIEtaIEta[ele1_index],EvtWeight);
        heVar[1][5][Ele1InEE] -> Fill(eleHoverE[ele1_index],EvtWeight);
        heVar[1][6][Ele1InEE] -> Fill(fabs(eledEtaAtVtx[ele1_index]),EvtWeight);
        heVar[1][7][Ele1InEE] -> Fill(fabs(eledPhiAtVtx[ele1_index]),EvtWeight);

        heVar[1][0][Ele2InEE] -> Fill(eleIsoEcalDR03[ele2_index],EvtWeight);
        heVar[1][1][Ele2InEE] -> Fill(eleIsoHcalDR03[ele2_index],EvtWeight);
        heVar[1][2][Ele2InEE] -> Fill(eleIsoTrkDR03[ele2_index],EvtWeight);
        heVar[1][3][Ele2InEE] -> Fill((eleIsoTrkDR03[ele2_index]+eleIsoHcalDR03[ele2_index]+eleIsoEcalDR03[ele2_index]),EvtWeight);
        heVar[1][4][Ele2InEE] -> Fill(eleSigmaIEtaIEta[ele2_index],EvtWeight);
        heVar[1][5][Ele2InEE] -> Fill(eleHoverE[ele2_index],EvtWeight);
        heVar[1][6][Ele2InEE] -> Fill(fabs(eledEtaAtVtx[ele2_index]),EvtWeight);
        heVar[1][7][Ele2InEE] -> Fill(fabs(eledPhiAtVtx[ele2_index]),EvtWeight);

        hMee[0]->Fill(MZee,EvtWeight);

        pho_index=-1;
        ThisValue=0;

        for (Int_t iPho=0;iPho<nPho;iPho++){
           if (phoEt[iPho] < PhoPtCut) continue;
           if (fabs(phoSCEta[iPho]) > 2.5) continue;
           if (fabs(phoSCEta[iPho]) > 1.4442 && fabs(phoSCEta[iPho]) < 1.566 ) continue;
           if (phoOverlap[iPho] == 1) continue;
           if (phohasPixelSeed[iPho] == 1) continue;
           if (phoHoverE[iPho] > 0.5) continue;

           if (fabs(phoSCEta[iPho]) < 1.4442) {
             if (phoRecoFlag[iPho]==2) continue;
             if (phoSeverity[iPho]==3) continue;
             if (phoSeverity[iPho]==4) continue;
           }

           h1Et[0] -> Fill(phoEt[iPho],EvtWeight);
           hpEta[0]-> Fill(phoEta[iPho],EvtWeight);

           if ( fabs(phoSCEta[iPho]) < 1.4442 ) PhoInEE = 0;
           else if (fabs(phoSCEta[iPho])<2.56 && fabs(phoSCEta[iPho])>1.566) PhoInEE = 1;

           hpVar[0][0][PhoInEE] -> Fill(phoEcalIsoDR04[iPho],EvtWeight);
           hpVar[0][1][PhoInEE] -> Fill(phoHcalIsoDR04[iPho],EvtWeight);
           hpVar[0][2][PhoInEE] -> Fill(phoTrkIsoHollowDR04[iPho],EvtWeight);
           hpVar[0][3][PhoInEE] -> Fill((phoTrkIsoHollowDR04[iPho]+phoHcalIsoDR04[iPho]+phoEcalIsoDR04[iPho]),EvtWeight);
           hpVar[0][4][PhoInEE] -> Fill(phoSigmaIEtaIEta[iPho],EvtWeight);
           hpVar[0][5][PhoInEE] -> Fill(phoHoverE[iPho],EvtWeight);


           if ( fabs(phoSCEta[iPho]) < 1.4442 ) {
               if ((phoEcalIsoDR04[iPho]-0.004*phoEt[iPho]) > PhoIDCutEB[phoID_index][0]) continue;
               if ((phoHcalIsoDR04[iPho]-0.001*phoEt[iPho]) > PhoIDCutEB[phoID_index][1]) continue;
               if ((phoTrkIsoHollowDR04[iPho]-0.001*phoEt[iPho]) > PhoIDCutEB[phoID_index][2]) continue;
               if (phoHoverE[iPho] > PhoIDCutEB[phoID_index][3]) continue;
               if (phoSigmaIEtaIEta[iPho]> PhoIDCutEB[phoID_index][4]) continue;
           } else if (fabs(phoSCEta[iPho])<2.56 && fabs(phoSCEta[iPho])>1.566) { 
               if ((phoEcalIsoDR04[iPho]-0.004*phoEt[iPho]) > PhoIDCutEE[phoID_index][0]) continue;
               if ((phoHcalIsoDR04[iPho]-0.001*phoEt[iPho]) > PhoIDCutEE[phoID_index][1]) continue;
               if ((phoTrkIsoHollowDR04[iPho]-0.001*phoEt[iPho]) > PhoIDCutEE[phoID_index][2]) continue;
               if (phoHoverE[iPho] > PhoIDCutEE[phoID_index][3]) continue;
               if (phoSigmaIEtaIEta[iPho]> PhoIDCutEE[phoID_index][4]) continue;
           } else{
               continue;
           }

           if (ThisValue < phoEt[iPho]){
              ThisValue = phoEt[iPho];
              pho_index = iPho;
           }
        }

        if (pho_index!=-1) {
            if ( fabs(phoSCEta[pho_index]) < 1.4442 ) PhoInEE = 0;
            else if (fabs(phoSCEta[pho_index]) < 2.56 && fabs(phoSCEta[pho_index]) > 1.566) PhoInEE = 1;

            ele1.SetPtEtaPhiE(elePt[ele1_index],eleEta[ele1_index],elePhi[ele1_index],eleEn[ele1_index]);
            ele2.SetPtEtaPhiE(elePt[ele2_index],eleEta[ele2_index],elePhi[ele2_index],eleEn[ele2_index]);
            gamma.SetPtEtaPhiE(phoEt[pho_index],phoEta[pho_index],phoPhi[pho_index],phoE[pho_index]);

            Zee=ele1+ele2;
            Mllg=ele1+ele2+gamma;
            //Fill Pho2 & Meeg1
            h1Et[1]  -> Fill(phoEt[pho_index],EvtWeight);
            hpEta[1] -> Fill(phoEta[pho_index],EvtWeight);

            hpVar[1][0][PhoInEE] -> Fill(phoEcalIsoDR04[pho_index],EvtWeight);
            hpVar[1][1][PhoInEE] -> Fill(phoHcalIsoDR04[pho_index],EvtWeight);
            hpVar[1][2][PhoInEE] -> Fill(phoTrkIsoHollowDR04[pho_index],EvtWeight);
            hpVar[1][3][PhoInEE] -> Fill((phoTrkIsoHollowDR04[pho_index]+phoHcalIsoDR04[pho_index]+phoEcalIsoDR04[pho_index]),EvtWeight);
            hpVar[1][4][PhoInEE] -> Fill(phoSigmaIEtaIEta[pho_index],EvtWeight);
            hpVar[1][5][PhoInEE] -> Fill(phoHoverE[pho_index],EvtWeight);

            hMee[1]  -> Fill(Zee.M(),EvtWeight);
            hMeeg[0] -> Fill(Mllg.M(),EvtWeight);

            MC_dPhi=0.0;
            MC_dEta=0.0;
            DelR1=0.0;

            MC_dPhi = phoPhi[pho_index] - elePhi[ele1_index];
            if (MC_dPhi >  3.1415927) MC_dPhi -= 2*3.1415927;
            if (MC_dPhi < -3.1415927) MC_dPhi += 2*3.1415927;
            MC_dEta = phoEta[pho_index] - eleEta[ele1_index];
            DelR1=sqrt(MC_dEta*MC_dEta + MC_dPhi*MC_dPhi);

            MC_dPhi=0.0;
            MC_dEta=0.0;
            DelR2=0.0;

            MC_dPhi = phoPhi[pho_index] - elePhi[ele2_index];
            if (MC_dPhi >  3.1415927) MC_dPhi -= 2*3.1415927;
            if (MC_dPhi < -3.1415927) MC_dPhi += 2*3.1415927;
            MC_dEta = phoEta[pho_index] - eleEta[ele2_index];
            DelR2=sqrt(MC_dEta*MC_dEta + MC_dPhi*MC_dPhi);

            hdR[0]->Fill(DelR1,EvtWeight);
            hdR[0]->Fill(DelR2,EvtWeight);
            if (DelR1> DelRCut && DelR2>DelRCut){
               h2Pt[2] -> Fill(elePt[ele1_index],EvtWeight);
               heEta[2]-> Fill(eleEta[ele1_index],EvtWeight);
               h2Pt[2] -> Fill(elePt[ele2_index],EvtWeight);
               heEta[2]-> Fill(eleEta[ele2_index],EvtWeight);

               hMee[2] -> Fill(Zee.M(),EvtWeight);
               hMeeg[1]-> Fill(Mllg.M(),EvtWeight);
               hMeeMeeg -> Fill(Zee.M(),Mllg.M(),EvtWeight);

               h1Et[2] -> Fill(phoEt[pho_index],EvtWeight);
               hpEta[2]-> Fill(phoEta[pho_index],EvtWeight);

               hpVar[2][0][PhoInEE] -> Fill(phoEcalIsoDR04[pho_index],EvtWeight);
               hpVar[2][1][PhoInEE] -> Fill(phoHcalIsoDR04[pho_index],EvtWeight);
               hpVar[2][2][PhoInEE] -> Fill(phoTrkIsoHollowDR04[pho_index],EvtWeight);
               hpVar[2][3][PhoInEE] -> Fill((phoTrkIsoHollowDR04[pho_index]+phoHcalIsoDR04[pho_index]+phoEcalIsoDR04[pho_index]),EvtWeight);
               hpVar[2][4][PhoInEE] -> Fill(phoSigmaIEtaIEta[pho_index],EvtWeight);
               hpVar[2][5][PhoInEE] -> Fill(phoHoverE[pho_index],EvtWeight);

               heVar[2][0][Ele1InEE] -> Fill(eleIsoEcalDR03[ele1_index],EvtWeight);
               heVar[2][1][Ele1InEE] -> Fill(eleIsoHcalDR03[ele1_index],EvtWeight);
               heVar[2][2][Ele1InEE] -> Fill(eleIsoTrkDR03[ele1_index],EvtWeight);
               heVar[2][3][Ele1InEE] -> Fill((eleIsoTrkDR03[ele1_index]+eleIsoHcalDR03[ele1_index]+eleIsoEcalDR03[ele1_index]),EvtWeight);
               heVar[2][4][Ele1InEE] -> Fill(eleSigmaIEtaIEta[ele1_index],EvtWeight);
               heVar[2][5][Ele1InEE] -> Fill(eleHoverE[ele1_index],EvtWeight);
               heVar[2][6][Ele1InEE] -> Fill(fabs(eledEtaAtVtx[ele1_index]),EvtWeight);
               heVar[2][7][Ele1InEE] -> Fill(fabs(eledPhiAtVtx[ele1_index]),EvtWeight);

               heVar[2][0][Ele2InEE] -> Fill(eleIsoEcalDR03[ele2_index],EvtWeight);
               heVar[2][1][Ele2InEE] -> Fill(eleIsoHcalDR03[ele2_index],EvtWeight);
               heVar[2][2][Ele2InEE] -> Fill(eleIsoTrkDR03[ele2_index],EvtWeight);
               heVar[2][3][Ele2InEE] -> Fill((eleIsoTrkDR03[ele2_index]+eleIsoHcalDR03[ele2_index]+eleIsoEcalDR03[ele2_index]),EvtWeight);
               heVar[2][4][Ele2InEE] -> Fill(eleSigmaIEtaIEta[ele2_index],EvtWeight);
               heVar[2][5][Ele2InEE] -> Fill(eleHoverE[ele2_index],EvtWeight);
               heVar[2][6][Ele2InEE] -> Fill(fabs(eledEtaAtVtx[ele2_index]),EvtWeight);
               heVar[2][7][Ele2InEE] -> Fill(fabs(eledPhiAtVtx[ele2_index]),EvtWeight);

               nWrite++;
cout<<"#Zgamma:"<<setw(3)<<nWrite<<setw(8)<<"run:"<<setw(7)<<run<<setw(10)<<"event:"<<setw(12)<<event<<setw(12)<<"lumis:"<<setw(5)<<lumis<<endl;
            }

            //Check Zgamma candidates with dR>0.3
            if (DelR1> 0.3 && DelR2>0.3){
              cout<<"-----------------------------"<<endl;
              cout<<"Find Zee+gamma Candidate!!..."<<nZgCandidates<<endl;
              cout<<"run:"<<run<<setw(10)<<"     event:"<<event<<setw(12)<<"     lumis:"<<lumis<<endl;
              cout<<""<<endl;
              cout<<"Z(ee) mass:"<<MZee<<setw(6)<<"     Z(eegamma)mass:"<<Mllg.M()<<setw(6)<<endl;

              if ( (fabs(eleSCEta[ele1_index]) < 1.4442 ) || 
                   (fabs(eleSCEta[ele1_index]) < 2.5 && fabs(eleSCEta[ele1_index]) > 1.56)) InEE_EB=1; else InEE_EB=0;
              cout<<"Electron2:"<<endl;
              cout<<"Pt    :"<<setw(7)<<elePt[ele1_index]<<setw(7)<<"   Eta   :"<<setw(7)<<eleEta[ele1_index]<<setw(7)<<"   Phi      :"<<setw(7)<<elePhi[ele1_index]<<setw(7)<<"   SCEta :"<<setw(7)<<eleSCEta[ele1_index]<<setw(7)<<"   InECAL?:"<<InEE_EB<<endl;
              cout<<"EIso  :"<<setw(7)<<eleIsoEcalDR04[ele1_index]<<setw(7)<<"   HcalIso:"<<setw(7)<<eleIsoHcalDR04[ele1_index]<<setw(7)<<"   TrkIso   :"<<setw(7)<<eleIsoTrkDR04[ele1_index]<<endl;
              cout<<"dEatIn:"<<setw(7)<<eledEtaAtVtx[ele1_index]<<setw(7)<<  "   dPhiIn :"<<setw(7)<<eledPhiAtVtx[ele1_index]<<setw(7)<<  "   SigmaIEta:"<<setw(7)<<eleSigmaIEtaIEta[ele1_index]<<setw(7)<<"   HoverE:"<<setw(7)<<eleHoverE[ele1_index]<<endl;
              //eleID
              for (Int_t id1=11;id1>5;id1--){
                if (eleID[ele1_index][id1]==7) cout<<"PassID:"<<eleID_String[id1]<<"   ";
              }
              cout<<endl;


              if ( (fabs(eleSCEta[ele2_index]) < 1.4442 ) || 
                   (fabs(eleSCEta[ele2_index]) < 2.5 && fabs(eleSCEta[ele2_index]) > 1.56)) InEE_EB=1; else InEE_EB=0;
              cout<<"Electron2:"<<endl;
              cout<<"Pt    :"<<setw(7)<<elePt[ele2_index]<<setw(7)<<"   Eta   :"<<setw(7)<<eleEta[ele2_index]<<setw(7)<<"   Phi      :"<<setw(7)<<elePhi[ele2_index]<<setw(7)<<"   SCEta :"<<setw(7)<<eleSCEta[ele2_index]<<setw(7)<<"   InECAL?:"<<InEE_EB<<endl;
              cout<<"EIso  :"<<setw(7)<<eleIsoEcalDR04[ele2_index]<<setw(7)<<"   HcalIso:"<<setw(7)<<eleIsoHcalDR04[ele2_index]<<setw(7)<<"   TrkIso   :"<<setw(7)<<eleIsoTrkDR04[ele2_index]<<endl;
              cout<<"dEatIn:"<<setw(7)<<eledEtaAtVtx[ele2_index]<<setw(7)<<  "   dPhiIn :"<<setw(7)<<eledPhiAtVtx[ele2_index]<<setw(7)<<  "   SigmaIEta:"<<setw(7)<<eleSigmaIEtaIEta[ele2_index]<<setw(7)<<"   HoverE:"<<setw(7)<<eleHoverE[ele2_index]<<endl;
              //eleID
              for (Int_t id1=11;id1>5;id1--){
                if (eleID[ele2_index][id1]==7) cout<<"PassID:"<<eleID_String[id1]<<"   ";
              }
              cout<<endl;


              if ( (fabs(phoSCEta[pho_index]) < 1.4442 ) || 
                   (fabs(phoSCEta[pho_index]) < 2.5 && fabs(phoSCEta[pho_index]) > 1.56)) InEE_EB=1; else InEE_EB=0;
              cout<<"photon:"<<endl;
              cout<<"Pt:"<<phoEt[pho_index]<<setw(6)<< "   Eta:"<<phoEta[pho_index]<<setw(7)<<"   Phi:"<<phoPhi[pho_index]<<setw(7)<<"   SCEta:"<<phoSCEta[pho_index]<<setw(6)<<"   InECAL?:"<<InEE_EB<<setw(6)<<"   dR1:"<<DelR1<<setw(6)<<"   dR2:"<<DelR2<<endl;
              cout<<"EIso:"<<phoEcalIsoDR04[pho_index]<<setw(8)<<"   HcalIso:"<<phoHcalIsoDR04[pho_index]<<setw(8)<<"   TrkIso:"<<phoTrkIsoHollowDR04[pho_index]<<endl;
              cout<<"SigmaIEtaIEta:"<<phoSigmaIEtaIEta[pho_index]<<setw(8)<<"   HoverE:"<<phoHoverE[pho_index]<<setw(8)<<"   R9:"<<phoR9[pho_index]<<endl;
              //check phoID
              for (Int_t id1=0;id1<6;id1++){
                passPhoID=1;
                if ( fabs(phoSCEta[pho_index]) < 1.4442 ) {
                   if ((phoEcalIsoDR04[pho_index]-0.004*phoEt[pho_index]) > PhoIDCutEB[id1][0]) passPhoID=0;
                   if ((phoHcalIsoDR04[pho_index]-0.001*phoEt[pho_index]) > PhoIDCutEB[id1][1]) passPhoID=0;
                   if ((phoTrkIsoHollowDR04[pho_index]-0.001*phoEt[pho_index]) > PhoIDCutEB[id1][2]) passPhoID=0;
                   if (phoHoverE[pho_index] > PhoIDCutEB[id1][3]) passPhoID=0;
                   if (phoSigmaIEtaIEta[pho_index]> PhoIDCutEB[id1][4]) passPhoID=0;
                } else if (fabs(phoSCEta[pho_index])<2.56 && fabs(phoSCEta[pho_index])>1.56) {
                   if ((phoEcalIsoDR04[pho_index]-0.004*phoEt[pho_index]) > PhoIDCutEE[id1][0]) passPhoID=0;
                   if ((phoHcalIsoDR04[pho_index]-0.001*phoEt[pho_index]) > PhoIDCutEE[id1][1]) passPhoID=0;
                   if ((phoTrkIsoHollowDR04[pho_index]-0.001*phoEt[pho_index]) > PhoIDCutEE[id1][2]) passPhoID=0;
                   if (phoHoverE[pho_index] > PhoIDCutEE[id1][3]) passPhoID=0;
                   if (phoSigmaIEtaIEta[pho_index]> PhoIDCutEE[id1][4]) passPhoID=0;
                } else {
                   passPhoID=0;
                }
                if (passPhoID==1) cout<<"Pho passID: "<<phoID_String[id1]<<"   ";
              }
              cout<<endl;
              nZgCandidates++;
            }
        }
      }
   }

   TFile *f1 = new TFile(OutputRootFile+".root","RECREATE");

   f1->cd();
     for (Int_t ii=0;ii<3;ii++){
       hMee[ii]  -> Write();
       hMeeg[ii] -> Write();
       h1Et[ii]  -> Write();
       h2Pt[ii]  -> Write();
       hdR[ii]   -> Write();
       heEta[ii] -> Write();
       hpEta[ii] -> Write();
       for (Int_t jj=0;jj<2;jj++){
         for (Int_t kk=0;kk<8;kk++) heVar[ii][kk][jj] -> Write();
         for (Int_t kk=0;kk<6;kk++) hpVar[ii][kk][jj] -> Write();
       }
     }
     hMeeMeeg -> Write();
   f1->Close();
   cout<<nWrite<<endl;
}
