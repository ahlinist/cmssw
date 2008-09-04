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
  
  string TreeFilename              = c1.getValue<string>("TreeFilename");
  string HistoFilename             = c1.getValue<string>("HistoFilename");
  double DPHI_CUT                  = c1.getValue<double>("DPHI_CUT");
  double RATIOPTJET3_CUT           = c1.getValue<double>("RATIOPTJET3_CUT"); 
  vector<double> DijetPtBoundaries = c1.getVector<double>("DijetPtBoundaries");
  vector<double> EtaBoundaries     = c1.getVector<double>("EtaBoundaries");
  if (!c1.check()) return 0;
  c1.print();
  const int MAX_NETA = 83;
  const int MAX_NPtBins = 21;
  int NETA = EtaBoundaries.size()-1;
  int NPtBins = DijetPtBoundaries.size()-1;
  if (NETA>MAX_NETA)
    {
      cout<<"WARNING: too many eta bins!!!! Setting default value 82"<<endl;
      NETA = 83;
    } 
  if (NPtBins>MAX_NPtBins)
    {
      cout<<"WARNING: too many dijetPt bins!!!! Setting default value 20"<<endl;
      NPtBins = 21;
    }
  TFile *inf;      
  inf = TFile::Open(TreeFilename.c_str(),"R");
  TFile *outf = new TFile(HistoFilename.c_str(),"RECREATE");         
  TH1F *hPtProbe[MAX_NPtBins][MAX_NETA];
  TH1F *hBalance[MAX_NPtBins][MAX_NETA];          
  TH1F *hDphi[MAX_NPtBins];
  TH1F *hDphiCuts[MAX_NPtBins];
  TH1F *hRatioPtJet3[MAX_NPtBins];
  TH1F *hRatioPtJet3Cuts[MAX_NPtBins];
  TH1F *hPtJet3[MAX_NPtBins];
  TH1F *hPtJet3Cuts[MAX_NPtBins];
  TH1F *hDijetPt;
  TH1F *hDijetPtCuts;
  TH2F *h2DPt;
  TH2F *h2DPtCuts;
  TH2F *h2DDPhiRatio;
  TH2F *h2DDPhiRatioCuts;
  char name[100];
  int i,j,ipt,ieta;
  bool cut_dphi,cut_ratio;
  float dphi,ratioPtJet3,ptBarrel,ptProbe,ptJet3,etaProbe,dijetPt,balance;
  /////////////////////////////////////////////////////////////// 
  TTree *tr = (TTree*)inf->Get("DijetTree");
  TBranch *brEtaProbe = (TBranch*)tr->GetBranch("etaProbe");
  brEtaProbe->SetAddress(&etaProbe);
  TBranch *brDphi = (TBranch*)tr->GetBranch("dphi");
  brDphi->SetAddress(&dphi);
  TBranch *brPtProbe = (TBranch*)tr->GetBranch("ptProbe");
  brPtProbe->SetAddress(&ptProbe);
  TBranch *brPtBarrel = (TBranch*)tr->GetBranch("ptBarrel");
  brPtBarrel->SetAddress(&ptBarrel);
  TBranch *brPtJet3 = (TBranch*)tr->GetBranch("ptJet3");
  brPtJet3->SetAddress(&ptJet3);
  ///////////////////////////////////////////////////////////////
  cout<<"Creating histograms"<<endl;
  sprintf(name,"DijetPt");
  hDijetPt = new TH1F(name,name,(int)DijetPtBoundaries[NPtBins],0,DijetPtBoundaries[NPtBins]);
  sprintf(name,"DijetPtCuts");
  hDijetPtCuts = new TH1F(name,name,(int)DijetPtBoundaries[NPtBins],0,DijetPtBoundaries[NPtBins]);
  sprintf(name,"PtProbe_vs_PtBarrel");
  h2DPt = new TH2F(name,name,(int)DijetPtBoundaries[NPtBins],0,DijetPtBoundaries[NPtBins],(int)DijetPtBoundaries[NPtBins],0,DijetPtBoundaries[NPtBins]);
  sprintf(name,"PtProbe_vs_PtBarrel_Cuts");
  h2DPtCuts = new TH2F(name,name,(int)DijetPtBoundaries[NPtBins],0,DijetPtBoundaries[NPtBins],(int)DijetPtBoundaries[NPtBins],0,DijetPtBoundaries[NPtBins]);
  sprintf(name,"DPhi_vs_RatioPtJet3");
  h2DDPhiRatio = new TH2F(name,name,100,0,1,630,0,6.3);
  sprintf(name,"DPhi_vs_RatioPtJet3_Cuts");
  h2DDPhiRatioCuts = new TH2F(name,name,100,0,1,630,0,6.3);
  for(i=0;i<NPtBins;i++)
    {
      sprintf(name,"dPhiJet12_DijetPtBin%d",i);
      hDphi[i] = new TH1F(name,name,630,0,6.3);
      sprintf(name,"dPhiJet12Cuts_DijetPtBin%d",i);
      hDphiCuts[i] = new TH1F(name,name,630,0,6.3);
      sprintf(name,"RatioJet3_DijetPtBin%d",i);
      hRatioPtJet3[i] = new TH1F(name,name,100,0,1);
      sprintf(name,"RatioJet3Cuts_DijetPtBin%d",i);
      hRatioPtJet3Cuts[i] = new TH1F(name,name,100,0,1);
      sprintf(name,"PtJet3_DijetPtBin%d",i);
      hPtJet3[i] = new TH1F(name,name,(int)DijetPtBoundaries[NPtBins],0,DijetPtBoundaries[NPtBins]);
      sprintf(name,"PtJet3Cuts_DijetPtBin%d",i);
      hPtJet3Cuts[i] = new TH1F(name,name,(int)DijetPtBoundaries[NPtBins],0,DijetPtBoundaries[NPtBins]);
      for(j=0;j<NETA;j++)
        {  
          sprintf(name,"PtProbeJet_DijetPtBin%d_EtaBin%d",i,j);
          hPtProbe[i][j] = new TH1F(name,name,(int)DijetPtBoundaries[NPtBins],0,DijetPtBoundaries[NPtBins]);
          sprintf(name,"B_DijetPtBin%d_EtaBin%d",i,j);
          hBalance[i][j] = new TH1F(name,name,100,-2,2);
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
      dijetPt = (ptBarrel+ptProbe)/2.;
      balance = (ptProbe-ptBarrel)/dijetPt;
      ratioPtJet3 = ptJet3/dijetPt;
      cut_dphi = (fabs(dphi)>DPHI_CUT);
      cut_ratio = (ratioPtJet3<RATIOPTJET3_CUT);
      ipt = getBin(dijetPt,DijetPtBoundaries);
      ieta = getBin(etaProbe,EtaBoundaries); 
      if (ipt>=0 && ieta>=0)
        {  
          hDphi[ipt]->Fill(Phi_0_2pi(dphi));
          hRatioPtJet3[ipt]->Fill(ratioPtJet3); 
          hPtJet3[ipt]->Fill(ptJet3);
          hDijetPt->Fill(dijetPt);
          h2DPt->Fill(ptBarrel,ptProbe);  
          h2DDPhiRatio->Fill(ratioPtJet3,Phi_0_2pi(dphi)); 
          if (cut_dphi && cut_ratio)
            {
              hPtProbe[ipt][ieta]->Fill(ptProbe);
              hBalance[ipt][ieta]->Fill(balance);
              hDphiCuts[ipt]->Fill(Phi_0_2pi(dphi));
              hRatioPtJet3Cuts[ipt]->Fill(ratioPtJet3);
              hPtJet3Cuts[ipt]->Fill(ptJet3);
              hDijetPtCuts->Fill(dijetPt);
              h2DPtCuts->Fill(ptBarrel,ptProbe);
              h2DDPhiRatioCuts->Fill(ratioPtJet3,Phi_0_2pi(dphi));
            }
        }
    }
  cout<<"Writing histograms in: "<<HistoFilename<<endl;
  outf->Write();
  outf->Close();
}
