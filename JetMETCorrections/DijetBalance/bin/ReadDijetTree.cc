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
  string         JetAlgo        = c1.getValue<string> ("JetAlgo");
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
  double NEVENTS(1.0);
  TString alg(JetAlgo);
  if (IsMCarlo)
    {
      NEVENTS = ((TH1F*)inf->Get(alg+"/ptHat"))->GetEntries();
      cout<<"Number of events processed: "<<NEVENTS<<endl;
    } 
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
  char name[1024];
  int i,j,k,ipt,ieta;
  int passLooseIdBarrel,passLooseIdProbe,passLooseIdJet3;
  bool cut_dphi,cut_ratio,cut_jet3,cut_id;
  float dphi,ratio,ptBarrel,ptProbe,ptJet3,etaProbe,eta,etaJet3,dijetPt,balance,pthat,wt;
  int HLTBits[6];
  double veta[MAX_NETA]; 
  for(i=0;i<=NETA;i++)
    veta[i] = EtaBnd[i];
        
  ///////////////////////////////////////////////////////////////  
  TTree *tr = (TTree*)inf->Get(alg+"/DiJetTree");
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
  TBranch *brIdBarrel = (TBranch*)tr->GetBranch("passLooseIdBarrel");
  TBranch *brIdProbe = (TBranch*)tr->GetBranch("passLooseIdProbe");
  TBranch *brIdJet3 = (TBranch*)tr->GetBranch("passLooseIdJet3");
  brIdBarrel->SetAddress(&passLooseIdBarrel);
  brIdProbe->SetAddress(&passLooseIdProbe);
  brIdJet3->SetAddress(&passLooseIdJet3); 
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

          for(j=0;j<NETA;j++)
            {  
              sprintf(name,"PtProbe_DijetPt%d_Eta%d_HLT%d",i,j,k);
              hPtProbe[i][j][k] = new TH1F(name,name,nbins,0,max);

              sprintf(name,"B_DijetPt%d_Eta%d_HLT%d",i,j,k);
              hBalance[i][j][k] = new TH1F(name,name,200,-4,4);
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
      cut_id    = (passLooseIdBarrel == 1 && passLooseIdProbe == 1);
      if (passLooseIdJet3 == 0)//jet3 coming probabaly from noise. Accept the event.
        cut_jet3 = true;
      else 
        cut_jet3 = (cut_ratio || (ptJet3<PTJET3_CUT));
      ipt       = getBin(dijetPt,DijetPtBnd);
      ieta      = getBin(eta,EtaBnd); 
      if (ipt<0 || ieta<0 || !cut_id) continue;
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
            }
        }
    }
  cout<<"Writing histograms in: "<<HistoFilename<<endl;
  outf->Write();
  outf->Close();
}
