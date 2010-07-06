#define ana_cxx
#include "ana.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TProfile.h>
#include <iostream>
#include <TLorentzVector.h>
#include <TVector3.h>
#include <TGraph.h>
#include <TGraphAsymmErrors.h>
#include <TFrame.h>

void ana::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L ana.C
//      Root > ana t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   FILE *Table = fopen("Input_table", "r");
   char filename[200];
   char tmp[100];
   int flag = 1;
   int nfile(0);
   double weight[20];
   double nevt[20];
   double luminosity = 100.;
   while (flag != -1){
      flag=fscanf(Table, "%s", filename);
      if (flag == -1) continue;
      flag=fscanf(Table, "%s", tmp);
      double cross=atof(tmp);
      flag=fscanf(Table, "%s", tmp);
      double evt=atof(tmp);
      flag=fscanf(Table, "%s", tmp);
      double skim_evt=atof(tmp);

      weight[nfile] = 0;
      weight[nfile] = cross*luminosity/evt;
      if (nfile == 0) nevt[nfile] = skim_evt;
      else nevt[nfile] = skim_evt + nevt[nfile-1];
      cout<<nfile<<"     "<<filename<<"     "<<cross<<"     "<<evt<<"     "<<weight[nfile]<<"     "<<skim_evt<<"     "<<nevt[nfile]<<endl;

      nfile += 1;
   }

   int file_Index(-1);
   double total_evt[20];
   double HLT_evt[20];
   double HLT_Ele_evt[20];
   double HLT_EleID_evt[20];
   double HLT_EleID_W_evt[20];
   double HLT_EleID_W_Pho_evt[20];
   double HLT_EleID_W_PhoID_evt[20];
   double HLT_EleID_W_PhoID_dR_evt[20];
   double FSR[20], ISR[20], Fake[20], Unknown[20], EleFake[20];
   for (int a=0; a<20; a++) {
     total_evt[a] = 0;
     HLT_evt[a] = 0;
     HLT_Ele_evt[a] = 0;
     HLT_EleID_evt[a] = 0;
     HLT_EleID_W_evt[a] = 0;
     HLT_EleID_W_Pho_evt[a] = 0;
     HLT_EleID_W_PhoID_evt[a] = 0;
     HLT_EleID_W_PhoID_dR_evt[a] = 0;

     FSR[a] = 0;
     ISR[a] = 0;
     Fake[a] = 0;
     EleFake[a] = 0;
     Unknown[a] = 0;
   }

   int   ele_num(0);
   float ele_Pt(10);
   int   pho_num(0);
   float pho_Pt(10);
   int   MCele_Index(-1), ele_Index(-1), pho_Index(-1);
   float dPhi(0), dEta(0);
   int   W_Index(-1);
   double W_Mt(0), dPhi_MET(0);

   TH1F *h2[20];
   TH1F *h4[20][3];
   TH1F *h5[20][3];
   TH2F *h6[20][3];
   TH2F *h7[20][3];
   TH1F *h8[20][3];
   TH2F *h9[20][3];
   for (int a=0; a<20; a++) {
     h2[a] = new TH1F(Form("deltaR_%d", a), "", 70, 0, 7);

     h4[a][0] = new TH1F(Form("calo_MET_%d", a), "", 200, 0, 200);
     h4[a][1] = new TH1F(Form("tc_MET_%d", a), "", 200, 0, 200);
     h4[a][2] = new TH1F(Form("pf_MET_%d", a), "", 200, 0, 200);

     h5[a][0] = new TH1F(Form("MtMET_%d", a), "", 200, 0, 200);
     h5[a][1] = new TH1F(Form("MttcMET_%d", a), "", 200, 0, 200);
     h5[a][2] = new TH1F(Form("MtpfMET_%d", a), "", 200, 0, 200);

     h6[a][0] = new TH2F(Form("Mt_caloMET_%d", a), "", 200, 0, 200, 200, 0, 200);
     h6[a][1] = new TH2F(Form("Mt_tcMET_%d", a), "", 200, 0, 200, 200, 0, 200);
     h6[a][2] = new TH2F(Form("Mt_pfMET_%d", a), "", 200, 0, 200, 200, 0, 200);

     h7[a][0] = new TH2F(Form("Mt_M_caloMET_%d", a), "", 200, 0, 200, 200, 0, 200);
     h7[a][1] = new TH2F(Form("Mt_M_tcMET_%d", a), "", 200, 0, 200, 200, 0, 200);
     h7[a][2] = new TH2F(Form("Mt_M_pfMET_%d", a), "", 200, 0, 200, 200, 0, 200);

     h8[a][0] = new TH1F(Form("PhoPt_EB_%d", a), "", 200, 0, 200);
     h8[a][1] = new TH1F(Form("PhoPt_EE_%d", a), "", 200, 0, 200);
     h8[a][2] = new TH1F(Form("PhoPt_All_%d", a), "", 200, 0, 200);

     h9[a][0] = new TH2F(Form("MT2_MT3_caloMET_%d", a), "", 200, 0, 200, 200, 0, 200);
     h9[a][1] = new TH2F(Form("MT2_MT3_tcMET_%d", a), "", 200, 0, 200, 200, 0, 200);
     h9[a][2] = new TH2F(Form("MT2_MT3_pfMET_%d", a), "", 200, 0, 200, 200, 0, 200);
   }

   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
 
      file_Index = 0;
      for (int a=0; a<nfile; a++) {
        if (jentry >= nevt[a]) file_Index = a+1;
      }

      int wg_enug(0);
      if (file_Index == 0) {
        for (int a=0; a<nMC; a++) {
          if (mcDecayType[a] == 2) wg_enug += 1;
        }
      }
      if (file_Index == 0 && wg_enug == 0) continue;
      total_evt[file_Index] += weight[file_Index];

      // HLT selection
      if (HLT[65] == 0) continue;
      HLT_evt[file_Index] += weight[file_Index];

      // Electron selection
      ele_Index = -1;
      ele_Pt = 20;
      ele_num = 0;
      for (int a=0; a<nEle; a++) {
        if (elePt[a] < ele_Pt) continue;
        if (fabs(eleSCEta[a]) > 2.5) continue;
        if (fabs(eleSCEta[a]) > 1.4442 && fabs(eleSCEta[a]) < 1.56) continue;
        ele_num += 1;

        if (eleID[a][8] != 7) continue;

        ele_Pt = elePt[a];
        ele_Index = a;
      }
      if (ele_num == 0) continue;
      HLT_Ele_evt[file_Index] += weight[file_Index];
      if (ele_Index == -1) continue;
      if (ele_num > 1) continue;
      HLT_EleID_evt[file_Index] += weight[file_Index];
	
      // Make the plots of MET and W transverse mass
      h4[file_Index][0]->Fill(MET, weight[file_Index]);
      h4[file_Index][1]->Fill(tcMET, weight[file_Index]);
      h4[file_Index][2]->Fill(pfMET, weight[file_Index]);
      W_Index = -1;
      for (int a=0; a<nWenu; a++) {
	if (WenuEleIndex[a] != ele_Index) continue;

        h5[file_Index][0]->Fill(WenuMassTCaloMET[a], weight[file_Index]);
        h6[file_Index][0]->Fill(WenuMassTCaloMET[a], MET, weight[file_Index]);

        h5[file_Index][1]->Fill(WenuMassTTcMET[a], weight[file_Index]);
        h6[file_Index][1]->Fill(WenuMassTTcMET[a], tcMET, weight[file_Index]);

        h5[file_Index][2]->Fill(WenuMassTPfMET[a], weight[file_Index]);
        h6[file_Index][2]->Fill(WenuMassTPfMET[a], pfMET, weight[file_Index]);
	W_Index = a;
      }
      
      // MET selection
      if (pfMET < 25) continue;
      HLT_EleID_W_evt[file_Index] += weight[file_Index];

      // Photon selection
      pho_Pt = 10;
      pho_Index = -1;
      pho_num = 0;
      for (int a=0; a<nPho; a++) {
        if (phoOverlap[a] == 1) continue;
        if (phohasPixelSeed[a] == 1) continue;
        if (phoEt[a] < pho_Pt) continue;
        if (fabs(phoSCEta[a]) > 2.5) continue;
        if (fabs(phoSCEta[a]) > 1.4442 && fabs(phoSCEta[a]) < 1.56) continue;
        if (file_Index == 5 && fabs(phoGenMomPID[a]) <= 22) continue;
        if (file_Index == 6 && fabs(phoGenMomPID[a]) <= 22) continue;
        pho_num += 1;

        if (fabs(phoSCEta[a]) < 1.4442 && (phoEcalIsoDR04[a]-0.004*phoEt[a]) > 2.6) continue;
        if (fabs(phoSCEta[a]) < 1.4442 && (phoHcalIsoDR04[a]-0.001*phoEt[a]) > 50) continue;
        if (fabs(phoSCEta[a]) < 1.4442 && (phoTrkIsoHollowDR04[a]-0.001*phoEt[a]) > 1.6) continue;
        if (fabs(phoSCEta[a]) < 1.4442 && phoHoverE[a] > 0.04) continue;
        if (fabs(phoSCEta[a]) < 1.4442 && phoSigmaIEtaIEta[a] > 0.012) continue;

        if (fabs(phoSCEta[a]) > 1.56 && (phoEcalIsoDR04[a]-0.004*phoEt[a]) > 50) continue;
        if (fabs(phoSCEta[a]) > 1.56 && (phoHcalIsoDR04[a]-0.001*phoEt[a]) > 1.4) continue;
        if (fabs(phoSCEta[a]) > 1.56 && (phoTrkIsoHollowDR04[a]-0.001*phoEt[a]) > 1.2) continue;
        if (fabs(phoSCEta[a]) > 1.56 && phoHoverE[a] > 0.045) continue;
        if (fabs(phoSCEta[a]) > 1.56 && phoSigmaIEtaIEta[a] > 0.038) continue;

        pho_Pt = phoEt[a];
        pho_Index = a;
      }
      if (pho_num == 0) continue;
      HLT_EleID_W_Pho_evt[file_Index] += weight[file_Index];
      if (pho_Index == -1) continue;
      HLT_EleID_W_PhoID_evt[file_Index] += weight[file_Index];

      // deltaR between electron and photon
      dEta = phoEta[pho_Index] - eleEta[ele_Index];
      dPhi = phoPhi[pho_Index] - elePhi[ele_Index];
      if (dPhi >  3.1415927) dPhi -= 2*3.1415927;
      if (dPhi < -3.1415927) dPhi += 2*3.1415927;
      h2[file_Index]->Fill(sqrt(pow(dEta,2)+pow(dPhi,2)), weight[file_Index]);
      if (sqrt(pow(dEta,2)+pow(dPhi,2)) < 0.7) continue;
      HLT_EleID_W_PhoID_dR_evt[file_Index] += weight[file_Index];

      // Make the plot of W transverse mass versus cluster transverse mass
      TLorentzVector LEle,LPho;
      TLorentzVector LZep;
      LPho.SetPxPyPzE(phoEt[pho_Index]*cos(phoPhi[pho_Index]),phoEt[pho_Index]*sin(phoPhi[pho_Index]),phoPz[pho_Index],phoE[pho_Index]);
      LEle.SetPxPyPzE(elePt[ele_Index]*cos(elePhi[ele_Index]),elePt[ele_Index]*sin(elePhi[ele_Index]),elePz[ele_Index],eleEn[ele_Index]);
      LZep = LPho + LEle;
      float M3(0);
      float first(0);
      float second(0);
      first = pow((sqrt(pow(LZep.M(), 2) + pow(phoEt[pho_Index]*cos(phoPhi[pho_Index]) + elePt[ele_Index]*cos(elePhi[ele_Index]) ,2) + pow(phoEt[pho_Index]*sin(phoPhi[pho_Index]) + elePt[ele_Index]*sin(elePhi[ele_Index]), 2)) + MET), 2);
      second = pow(phoEt[pho_Index]*cos(phoPhi[pho_Index]) + elePt[ele_Index]*cos(elePhi[ele_Index]) + METx, 2) + pow(phoEt[pho_Index]*sin(phoPhi[pho_Index]) + elePt[ele_Index]*sin(elePhi[ele_Index]) + METy, 2);
      M3 = sqrt(first - second);
      h9[file_Index][0]->Fill(WenuMassTCaloMET[W_Index], M3, weight[file_Index]);

      first = 0;
      second = 0;
      first = pow((sqrt(pow(LZep.M(), 2) + pow(phoEt[pho_Index]*cos(phoPhi[pho_Index]) + elePt[ele_Index]*cos(elePhi[ele_Index]) ,2) + pow(phoEt[pho_Index]*sin(phoPhi[pho_Index]) + elePt[ele_Index]*sin(elePhi[ele_Index]), 2)) + tcMET), 2);
      second = pow(phoEt[pho_Index]*cos(phoPhi[pho_Index]) + elePt[ele_Index]*cos(elePhi[ele_Index]) + tcMETx, 2) + pow(phoEt[pho_Index]*sin(phoPhi[pho_Index]) + elePt[ele_Index]*sin(elePhi[ele_Index]) + tcMETy, 2);
      M3 = sqrt(first - second);
      h9[file_Index][1]->Fill(WenuMassTTcMET[W_Index], M3, weight[file_Index]);

      first = 0;
      second = 0;
      first = pow((sqrt(pow(LZep.M(), 2) + pow(phoEt[pho_Index]*cos(phoPhi[pho_Index]) + elePt[ele_Index]*cos(elePhi[ele_Index]) ,2) + pow(phoEt[pho_Index]*sin(phoPhi[pho_Index]) + elePt[ele_Index]*sin(elePhi[ele_Index]), 2)) + pfMET), 2);
      second = pow(phoEt[pho_Index]*cos(phoPhi[pho_Index]) + elePt[ele_Index]*cos(elePhi[ele_Index]) + pfMETx, 2) + pow(phoEt[pho_Index]*sin(phoPhi[pho_Index]) + elePt[ele_Index]*sin(elePhi[ele_Index]) + pfMETy, 2);
      M3 = sqrt(first - second);
      h9[file_Index][2]->Fill(WenuMassTPfMET[W_Index], M3, weight[file_Index]);

      // Make the plost of photon pt spectrum
      if (fabs(phoSCEta[pho_Index]) < 1.442) h8[file_Index][0]->Fill(phoEt[pho_Index], weight[file_Index]);
      else h8[file_Index][1]->Fill(phoEt[pho_Index], weight[file_Index]);
      h8[file_Index][2]->Fill(phoEt[pho_Index], weight[file_Index]);

      // Count ISR, FSR, Fake photons and photon from electron misidentification
      MCele_Index = -1;
      for (int a=0; a<nMC; a++) {
        dEta = phoEta[pho_Index] - mcEta[a];
        dPhi = phoPhi[pho_Index] - mcPhi[a];
        if (dPhi >  3.1415927) dPhi -= 2*3.1415927;
        if (dPhi < -3.1415927) dPhi += 2*3.1415927;
        if (sqrt(pow(dEta,2)+pow(dPhi,2)) < 0.2 && fabs(mcPID[a]) == 11) MCele_Index = a;
      }

      if (fabs(phoGenMomPID[pho_Index]) == 11 || fabs(phoGenMomPID[pho_Index]) == 13 || fabs(phoGenMomPID[pho_Index]) == 15) FSR[file_Index] += weight[file_Index];
      else if (fabs(phoGenMomPID[pho_Index]) < 7 || fabs(phoGenMomPID[pho_Index]) == 22 || fabs(phoGenMomPID[pho_Index]) == 21) ISR[file_Index] += weight[file_Index];
      else if (fabs(phoGenMomPID[pho_Index]) != 999) Fake[file_Index] += weight[file_Index];
      else if (fabs(phoGenMomPID[pho_Index]) == 999 && MCele_Index != -1) EleFake[file_Index] += weight[file_Index];
      else if (fabs(phoGenMomPID[pho_Index]) == 999) Unknown[file_Index] += weight[file_Index];
   }

   double total_FSR(0), total_ISR(0), total_Fake(0), total_Unknown(0), total_bg(0), total_EleFake(0);
   for (int a=0; a<nfile; a++) {
     cout<<" Entry = "<<a<<endl;
     cout<<" Total              = "<<total_evt[a]<<endl;
     cout<<"     -> HLT         = "<<HLT_evt[a]<<endl;
     cout<<"     -> Ele Pt      = "<<HLT_Ele_evt[a]<<endl;
     cout<<"     -> Ele ID      = "<<HLT_EleID_evt[a]<<endl;
     cout<<"     -> W Selection = "<<HLT_EleID_W_evt[a]<<endl;
     cout<<"     -> Pho Pt      = "<<HLT_EleID_W_Pho_evt[a]<<endl;
     cout<<"     -> Pho ID      = "<<HLT_EleID_W_PhoID_evt[a]<<endl;
     cout<<"     -> dR          = "<<HLT_EleID_W_PhoID_dR_evt[a]<<endl;
     cout<<" FSR = "<<FSR[a]<<endl;
     cout<<" ISR = "<<ISR[a]<<endl;
     cout<<" Fake = "<<Fake[a]<<endl;
     cout<<" EleFake = "<<EleFake[a]<<endl;
     cout<<" Unknown = "<<Unknown[a]<<endl;
     cout<<" ===================================== "<<endl;
     if (a == 0) continue;
     total_bg += HLT_EleID_W_PhoID_dR_evt[a];
     total_FSR += FSR[a];
     total_ISR += ISR[a];
     total_Fake += Fake[a];
     total_EleFake += EleFake[a];
     total_Unknown += Unknown[a];
   }
   cout<<"        "<<endl;
   cout<<" Total BG = "<<total_bg<<endl;
   cout<<" Total FSR = "<<total_FSR<<endl;
   cout<<" Total ISR = "<<total_ISR<<endl;
   cout<<" Total Fake = "<<total_Fake<<endl;
   cout<<" Total EleFake = "<<total_EleFake<<endl;
   cout<<" Total Unknown = "<<total_Unknown<<endl;

   TFile *file = new TFile("Distribution.root", "UPDATE");
   for (int a=0; a<nfile; a++) {
     h2[a]->Write();
     for (int b=0; b<3; b++) {
       h4[a][b]->Write();
       h5[a][b]->Write();
       h6[a][b]->Write();
       h7[a][b]->Write();
       h8[a][b]->Write();
       h9[a][b]->Write();
     }
   }
   file->Close();
}
