#include <iostream>
#include <string.h>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TMath.h>
#include <TTree.h>
#include "JetMETAnalysis/JetUtilities/interface/CommandLine.h"
#include "JetMETCorrections/DijetBalance/interface/JetUtil.h"
#include "JetMETCorrections/DijetBalance/interface/Utilities.h"
using namespace std;

int main(int argc, char**argv)
{
  CommandLine c1;
  c1.parse(argc,argv);
  //----------------------------------------------------------------------------
  string         TreeFilename   = c1.getValue<string> ("TreeFilename");
  string         HistoFilename  = c1.getValue<string> ("HistoFilename");
  string         JetType        = c1.getValue<string> ("JetType");
  bool           IsMCarlo       = c1.getValue<bool>   ("IsMCarlo",false);
  bool           ApplyWeights   = c1.getValue<bool>   ("ApplyWeights",false);  
  bool           ETA_SYMMETRY   = c1.getValue<bool>   ("ETA_SYMMETRY",false);
  int            NHLT           = c1.getValue<int>    ("NHLT");
  double         DPHI_CUT       = c1.getValue<double> ("DPHI_CUT");
  double         RATIO_CUT      = c1.getValue<double> ("RATIO_CUT");
  double         PTJET3_CUT     = c1.getValue<double> ("PTJET3_CUT");  
  double         Xsec           = c1.getValue<double> ("Xsec",1.);
  double         PTHAT_LOW      = c1.getValue<double> ("PTHAT_LOW",0);
  double         PTHAT_HIGH     = c1.getValue<double> ("PTHAT_HIGH",5000);
  vector<double> DijetPtBnd     = c1.getVector<double>("DijetPtBoundaries");
  vector<double> EtaBnd         = c1.getVector<double>("EtaBoundaries");
  //----------------------------------------------------------------------------
  if (!c1.check()) return 0;
  c1.print();

  const int MAX_NHLT = 10;
  const int MAX_NETA = 83;
  const int MAX_NPT  = 21;
  int NETA = EtaBnd.size()-1;
  int NPT  = DijetPtBnd.size()-1;
  int jet;  
  if      (((int)JetType.find("Calo")>-1) || ((int)JetType.find("CALO")>-1) || ((int)JetType.find("calo")>-1))
    jet = 0;
  else if (((int)JetType.find("Pf")>-1)   || ((int)JetType.find("PF")>-1)   || ((int)JetType.find("pf")>-1))
    jet = 1;
  else if (((int)JetType.find("Gen")>-1)  || ((int)JetType.find("GEN")>-1)  || ((int)JetType.find("gen")>-1))
    jet = 2;
  else
    {
      cout<<"Unknown jet type...."<<endl;
      jet = -1;
    } 
  if (NETA>MAX_NETA)
    {
      cout<<"WARNING: too many eta bins!!!! Setting default value 82"<<endl;
      NETA = 83;
    } 
  if (NPT>MAX_NPT)
    {
      cout<<"WARNING: too many dijetPt bins!!!! Setting default value 20"<<endl;
      NPT = 21;
    }
  if (NHLT>MAX_NHLT)
    {
      cout<<"WARNING: too many HLT bits!!!! Setting default value 10"<<endl;
      NHLT = 10;
    }
  //----------------------------------------------------------------------------
  TFile *inf;      
  inf = TFile::Open(TreeFilename.c_str(),"R");
  TFile *outf = new TFile(HistoFilename.c_str(),"RECREATE");   
  double NEVENTS = ((TH1F*)inf->Get("ptHat"))->GetEntries();
  cout<<"Number of events processed: "<<NEVENTS<<endl;
     
  TH1F *hPtProbe[MAX_NPT][MAX_NETA][MAX_NHLT];
  TH1F *hBalance[MAX_NPT][MAX_NETA][MAX_NHLT];          
  TH1F *hDPhi[MAX_NPT][MAX_NHLT];
  TH1F *hDPhiCuts[MAX_NPT][MAX_NHLT];
  TH1F *hRatio[MAX_NPT][MAX_NHLT];
  TH1F *hRatioCuts[MAX_NPT][MAX_NHLT];
  TH1F *hPtJet3[MAX_NPT][MAX_NHLT];
  TH1F *hPtJet3Cuts[MAX_NPT][MAX_NHLT];
  TH1F *hDijetPt[MAX_NHLT];
  TH1F *hDijetPtCuts[MAX_NHLT];
  TH1F *hEvents[MAX_NPT][MAX_NHLT];
  TH1F *hEventsCuts[MAX_NPT][MAX_NHLT];
  TH2F *h2DPt[MAX_NHLT];
  TH2F *h2DPtCuts[MAX_NHLT];
  TH2F *h2DDPhiRatio[MAX_NHLT];
  TH2F *h2DDPhiRatioCuts[MAX_NHLT];
  TH2F *h2DIdJet3Pt[MAX_NHLT];
  TH2F *h2DIdJet3Ratio[MAX_NHLT];
  TH1F *hIdJet3[MAX_NPT][3][MAX_NHLT];
  TH1F *hIdJet3Cuts[MAX_NPT][3][MAX_NHLT];
  TH1F *hIdProbe[MAX_NPT][3][MAX_NHLT];
  TH1F *hIdProbeCuts[MAX_NPT][3][MAX_NHLT];
  TH1F *hIdBarrel[MAX_NPT][MAX_NHLT];
  TH1F *hIdBarrelCuts[MAX_NPT][MAX_NHLT];
  char name[1024];
  int i,j,k,ipt,ieta;
  bool cut_dphi,cut_ratio,cut_jet3;
  float dphi,ratio,ptBarrel,ptProbe,ptJet3,etaProbe,eta,etaJet3,dijetPt,balance,pthat,wt,idProbe,idBarrel,idJet3;
  int HLTBits[6];
  double veta[MAX_NETA]; 
  for(i=0;i<=NETA;i++)
    veta[i] = EtaBnd[i];
        
  /////////////////////////////////////////////////////////////// 
  TTree *tr = (TTree*)inf->Get("DijetTree");
  TBranch *brEtaProbe = (TBranch*)tr->GetBranch("etaProbe");
  brEtaProbe->SetAddress(&etaProbe);
  TBranch *brEtaJet3 = (TBranch*)tr->GetBranch("etaJet3");
  brEtaJet3->SetAddress(&etaJet3);
  TBranch *brDphi = (TBranch*)tr->GetBranch("dphi");
  brDphi->SetAddress(&dphi);
  TBranch *brPtProbe = (TBranch*)tr->GetBranch("ptProbe");
  brPtProbe->SetAddress(&ptProbe);
  TBranch *brPtBarrel = (TBranch*)tr->GetBranch("ptBarrel");
  brPtBarrel->SetAddress(&ptBarrel);
  TBranch *brPtJet3 = (TBranch*)tr->GetBranch("ptJet3");
  brPtJet3->SetAddress(&ptJet3);
  TBranch *brHLTBits = (TBranch*)tr->GetBranch("HLTBits");
  brHLTBits->SetAddress(&HLTBits);
  if (jet==0)
    {
      TBranch *brIdBarrel = (TBranch*)tr->GetBranch("emfBarrel");
      TBranch *brIdProbe = (TBranch*)tr->GetBranch("emfProbe");
      TBranch *brIdJet3 = (TBranch*)tr->GetBranch("emfJet3");
      brIdBarrel->SetAddress(&idBarrel);
      brIdProbe->SetAddress(&idProbe);
      brIdJet3->SetAddress(&idJet3); 
    }
  if (jet==1)
    {
      TBranch *brIdBarrel = (TBranch*)tr->GetBranch("nhfBarrel");
      TBranch *brIdProbe = (TBranch*)tr->GetBranch("nhfProbe");
      TBranch *brIdJet3 = (TBranch*)tr->GetBranch("nhfJet3");
      brIdBarrel->SetAddress(&idBarrel);
      brIdProbe->SetAddress(&idProbe);
      brIdJet3->SetAddress(&idJet3); 
    }
  if (IsMCarlo)
    {
      TBranch *brPtHat = (TBranch*)tr->GetBranch("pthat");
      brPtHat->SetAddress(&pthat);
    }
  ///////////////////////////////////////////////////////////////
  cout<<"Booking histograms"<<endl;
  for(k=0;k<NHLT;k++)
    {
      int nbins = (int)DijetPtBnd[NPT];
      double max = DijetPtBnd[NPT];

      sprintf(name,"DijetPt_HLT%d",k);
      hDijetPt[k] = new TH1F(name,name,nbins,0,max);
  
      sprintf(name,"DijetPtCuts_HLT%d",k);
      hDijetPtCuts[k] = new TH1F(name,name,nbins,0,max);
 
      sprintf(name,"PtProbe_vs_PtBarrel_HLT%d",k);
      h2DPt[k] = new TH2F(name,name,nbins,0,max,nbins,0,max);

      sprintf(name,"PtProbe_vs_PtBarrel_Cuts_HLT%d",k);
      h2DPtCuts[k] = new TH2F(name,name,nbins,0,max,nbins,0,max);

      sprintf(name,"DPhi_vs_Ratio_HLT%d",k);
      h2DDPhiRatio[k] = new TH2F(name,name,100,0,1,630,0,6.3);

      sprintf(name,"DPhi_vs_Ratio_Cuts_HLT%d",k);
      h2DDPhiRatioCuts[k] = new TH2F(name,name,100,0,1,630,0,6.3);

      if (jet==0 || jet==1)
        {
          sprintf(name,"IdJet3_vs_Pt_HLT%d",k);
          h2DIdJet3Pt[k] = new TH2F(name,name,nbins,0,max,210,-1,1.1);
          sprintf(name,"IdJet3_vs_Ratio_HLT%d",k);
          h2DIdJet3Ratio[k] = new TH2F(name,name,100,0,1,210,-1,1.1);
        }
      for(i=0;i<NPT;i++)
        {
          sprintf(name,"DPhi_DijetPt%d_HLT%d",i,k);
          hDPhi[i][k] = new TH1F(name,name,630,0,6.3);

          sprintf(name,"DPhiCuts_DijetPt%d_HLT%d",i,k);
          hDPhiCuts[i][k] = new TH1F(name,name,630,0,6.3);

          sprintf(name,"Ratio_DijetPt%d_HLT%d",i,k);
          hRatio[i][k] = new TH1F(name,name,100,0,1);

          sprintf(name,"RatioCuts_DijetPt%d_HLT%d",i,k);
          hRatioCuts[i][k] = new TH1F(name,name,100,0,1);

          sprintf(name,"PtJet3_DijetPt%d_HLT%d",i,k);
          hPtJet3[i][k] = new TH1F(name,name,nbins,0,max);

          sprintf(name,"PtJet3Cuts_DijetPt%d_HLT%d",i,k);
          hPtJet3Cuts[i][k] = new TH1F(name,name,nbins,0,max);

          sprintf(name,"Events_DijetPt%d_HLT%d",i,k);
          hEvents[i][k] = new TH1F(name,name,NETA,veta);

          sprintf(name,"EventsCuts_DijetPt%d_HLT%d",i,k);
          hEventsCuts[i][k] = new TH1F(name,name,NETA,veta);

          if (jet==0 || jet==1)
            {
              sprintf(name,"IdJet3_DijetPt%d_HLT%d_Barrel",i,k);
              hIdJet3[i][0][k] = new TH1F(name,name,210,-1,1.1);

              sprintf(name,"IdJet3Cuts_DijetPt%d_HLT%d_Barrel",i,k);
              hIdJet3Cuts[i][0][k] = new TH1F(name,name,210,-1,1.1);

              sprintf(name,"IdJet3_DijetPt%d_HLT%d_Endcap",i,k);
              hIdJet3[i][1][k] = new TH1F(name,name,210,-1,1.1);

              sprintf(name,"IdJet3Cuts_DijetPt%d_HLT%d_Endcap",i,k);
              hIdJet3Cuts[i][1][k] = new TH1F(name,name,210,-1,1.1);

              sprintf(name,"IdJet3_DijetPt%d_HLT%d_Fwd",i,k);
              hIdJet3[i][2][k] = new TH1F(name,name,210,-1,1.1);

              sprintf(name,"IdJet3Cuts_DijetPt%d_HLT%d_Fwd",i,k);
              hIdJet3Cuts[i][2][k] = new TH1F(name,name,210,-1,1.1);

              sprintf(name,"IdBarrel_DijetPt%d_HLT%d",i,k);
              hIdBarrel[i][k] = new TH1F(name,name,210,-1,1.1);

              sprintf(name,"IdBarrelCuts_DijetPt%d_HLT%d",i,k);
              hIdBarrelCuts[i][k] = new TH1F(name,name,210,-1,1.1);

              sprintf(name,"IdProbe_DijetPt%d_HLT%d_Barrel",i,k);
              hIdProbe[i][0][k] = new TH1F(name,name,210,-1,1.1);

              sprintf(name,"IdProbeCuts_DijetPt%d_HLT%d_Barrel",i,k);
              hIdProbeCuts[i][0][k] = new TH1F(name,name,210,-1,1.1); 

              sprintf(name,"IdProbe_DijetPt%d_HLT%d_Endcap",i,k);
              hIdProbe[i][1][k] = new TH1F(name,name,210,-1,1.1);

              sprintf(name,"IdProbeCuts_DijetPt%d_HLT%d_Endcap",i,k);
              hIdProbeCuts[i][1][k] = new TH1F(name,name,210,-1,1.1);

              sprintf(name,"IdProbe_DijetPt%d_HLT%d_Fwd",i,k);
              hIdProbe[i][2][k] = new TH1F(name,name,210,-1,1.1);

              sprintf(name,"IdProbeCuts_DijetPt%d_HLT%d_Fwd",i,k);
              hIdProbeCuts[i][2][k] = new TH1F(name,name,210,-1,1.1);
            } 
          for(j=0;j<NETA;j++)
            {  
              sprintf(name,"PtProbe_DijetPt%d_Eta%d_HLT%d",i,j,k);
              hPtProbe[i][j][k] = new TH1F(name,name,nbins,0,max);

              sprintf(name,"B_DijetPt%d_Eta%d_HLT%d",i,j,k);
              hBalance[i][j][k] = new TH1F(name,name,100,-2,2);
            } 
        }
    } 
  ///////////////////////////////////////////////////////////////
  cout<<"Found "<<tr->GetEntries()<<" entries in: "<<TreeFilename<<endl;
  cout<<"Filling histograms"<<endl;
  for(i=0;i<tr->GetEntries();i++)
    {
      if (i % 500000 == 0)
        cout<<i<<endl;
      tr->GetEntry(i);
      wt = 1.;
      if (IsMCarlo)
        {
          if (pthat<PTHAT_LOW || pthat>PTHAT_HIGH) continue;
          if (ApplyWeights) 
            wt = Xsec/NEVENTS; 
        }
      if (ETA_SYMMETRY)
        eta = fabs(etaProbe);
      else
        eta = etaProbe;
      dijetPt   = (ptBarrel+ptProbe)/2.;
      balance   = (ptProbe-ptBarrel)/dijetPt;
      ratio     = ptJet3/dijetPt;
      cut_dphi  = (fabs(dphi)>DPHI_CUT);
      cut_ratio = (ratio<RATIO_CUT);
      if (jet==0)
        {
          if (fabs(idJet3)<0.001)//jet3 coming probabaly from noise. Accept the event.
            cut_jet3 = true;
          else 
            cut_jet3 = (cut_ratio || (ptJet3<PTJET3_CUT));
        }
      else if (jet==1)
        {
          if (idJet3>0.99)//jet3 coming probabaly from noise. Accept the event.
            cut_jet3 = true;
          else 
            cut_jet3 = (cut_ratio || (ptJet3<PTJET3_CUT));
        }
      else
        cut_jet3 = (cut_ratio || (ptJet3<PTJET3_CUT));

      ipt       = getBin(dijetPt,DijetPtBnd);
      ieta      = getBin(eta,EtaBnd); 
      if (ipt<0 || ieta<0) continue;
      for(k=0;k<NHLT;k++)
        {
          if (HLTBits[k]==0) continue;  
          hDPhi[ipt][k]      ->Fill(Phi_0_2pi(dphi),wt);
          hRatio[ipt][k]     ->Fill(ratio,wt); 
          hPtJet3[ipt][k]    ->Fill(ptJet3,wt);
          hEvents[ipt][k]    ->Fill(eta,wt); 
          hDijetPt[k]        ->Fill(dijetPt,wt);
          h2DPt[k]           ->Fill(ptBarrel,ptProbe,wt);  
          h2DDPhiRatio[k]    ->Fill(ratio,Phi_0_2pi(dphi),wt); 
          if (jet==0 || jet==1)
            {
              hIdBarrel[ipt][k]     ->Fill(idBarrel,wt);
              if (fabs(etaProbe)<1.3) 
                hIdProbe[ipt][0][k]      ->Fill(idProbe,wt);
              if ((fabs(etaProbe)<3) && (fabs(etaProbe)>=1.3))
                hIdProbe[ipt][1][k]      ->Fill(idProbe,wt);
              if ((fabs(etaProbe)<5) && (fabs(etaProbe)>=3.0))
                hIdProbe[ipt][2][k]      ->Fill(idProbe,wt);
              if (fabs(etaJet3)<1.3) 
                hIdJet3[ipt][0][k]      ->Fill(idJet3,wt);
              if ((fabs(etaJet3)<3) && (fabs(etaJet3)>=1.3))
                hIdJet3[ipt][1][k]      ->Fill(idJet3,wt);
              if ((fabs(etaJet3)<5) && (fabs(etaJet3)>=3.0))
                hIdJet3[ipt][2][k]      ->Fill(idJet3,wt); 
              
              h2DIdJet3Pt[k]        ->Fill(dijetPt,idJet3,wt);
              h2DIdJet3Ratio[k]     ->Fill(ratio,idJet3,wt); 
            }
          if (cut_dphi && cut_jet3)
            {
              hPtProbe[ipt][ieta][k]->Fill(ptProbe,wt);
              hBalance[ipt][ieta][k]->Fill(balance,wt);
              hDPhiCuts[ipt][k]     ->Fill(Phi_0_2pi(dphi),wt);
              hRatioCuts[ipt][k]    ->Fill(ratio,wt);
              hPtJet3Cuts[ipt][k]   ->Fill(ptJet3,wt);
              hEventsCuts[ipt][k]   ->Fill(eta,wt);
              hDijetPtCuts[k]       ->Fill(dijetPt,wt);
              h2DPtCuts[k]          ->Fill(ptBarrel,ptProbe,wt);
              h2DDPhiRatioCuts[k]   ->Fill(ratio,Phi_0_2pi(dphi),wt);
              if (jet==0 || jet==1)
                {
                  hIdBarrelCuts[ipt][k]     ->Fill(idBarrel,wt);
                  if (fabs(etaProbe)<1.3) 
                    hIdProbeCuts[ipt][0][k]      ->Fill(idProbe,wt);
                  if ((fabs(etaProbe)<3) && (fabs(etaProbe)>=1.3))
                    hIdProbeCuts[ipt][1][k]      ->Fill(idProbe,wt);
                  if ((fabs(etaProbe)<5) && (fabs(etaProbe)>=3.0))
                    hIdProbeCuts[ipt][2][k]      ->Fill(idProbe,wt);
                  if (fabs(etaJet3)<1.3) 
                    hIdJet3Cuts[ipt][0][k]      ->Fill(idJet3,wt);
                  if ((fabs(etaJet3)<3) && (fabs(etaJet3)>=1.3))
                    hIdJet3Cuts[ipt][1][k]      ->Fill(idJet3,wt);
                  if ((fabs(etaJet3)<5) && (fabs(etaJet3)>=3.0))
                    hIdJet3Cuts[ipt][2][k]      ->Fill(idJet3,wt); 
                }
            }
        }
    }
  cout<<"Writing histograms in: "<<HistoFilename<<endl;
  outf->Write();
  outf->Close();
}
