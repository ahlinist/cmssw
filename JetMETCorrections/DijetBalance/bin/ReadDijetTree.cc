#include <iostream>
#include <string.h>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TKey.h>
#include <TROOT.h>
#include <TMath.h>
#include <TTree.h>
#include "JetMETAnalysis/JetUtilities/interface/CommandLine.h"
#include "JetMETCorrections/DijetBalance/interface/JetUtil.h"
#include "JetMETCorrections/DijetBalance/interface/Utilities.h"
using namespace std;

typedef struct 
{
  int prescale;
  int fired;
} TrigStruct;
int main(int argc, char**argv)
{
  CommandLine c1;
  c1.parse(argc,argv);
  //----------------------------------------------------------------------------
  string         TreeFilename   = c1.getValue<string> ("TreeFilename");
  string         HistoFilename  = c1.getValue<string> ("HistoFilename");
  bool           IsMCarlo       = c1.getValue<bool>   ("IsMCarlo",false);
  bool           ApplyWeights   = c1.getValue<bool>   ("ApplyWeights",false);  
  bool           ETA_SYMMETRY   = c1.getValue<bool>   ("ETA_SYMMETRY",false);  
  double         DPHI_CUT       = c1.getValue<double> ("DPHI_CUT");
  double         RATIO_CUT      = c1.getValue<double> ("RATIO_CUT");
  double         PTJET3_CUT     = c1.getValue<double> ("PTJET3_CUT");  
  vector<string> HLTbit         = c1.getVector<string>("HLTbit","");
  vector<string> JetAlgos       = c1.getVector<string>("JetAlgos","");
  vector<double> PtHatBnd       = c1.getVector<double>("PtHatBoundaries","0,1");
  vector<double> PtHatXsec      = c1.getVector<double>("PtHatXsec","0,0");
  vector<double> PtHatNevt      = c1.getVector<double>("PtHatEvents","0,0");
  vector<double> DijetPtBnd     = c1.getVector<double>("DijetPtBoundaries");
  vector<double> EtaBnd         = c1.getVector<double>("EtaBoundaries");
  //----------------------------------------------------------------------------
  if (!c1.check()) return 0;
  c1.print();
  unsigned int NHLT = HLTbit.size();
  const unsigned int MAX_NHLT = 10;
  const int MAX_NETA = 83;
  const int MAX_NPT  = 21;
  if (!ETA_SYMMETRY) {
    cout<<"ETA_SYMMETRY is FALSE: unfolding eta bins...."<<endl;
    vector<double> newEtaBnd;
    for(unsigned i=EtaBnd.size()-1;i>0;i--)
      newEtaBnd.push_back(-1*EtaBnd[i]);
    for(unsigned i=0;i<EtaBnd.size();i++)
      newEtaBnd.push_back(EtaBnd[i]);
    EtaBnd.clear();
    for(unsigned i=0;i<newEtaBnd.size();i++)
      EtaBnd.push_back(newEtaBnd[i]);   
  } 
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
  char name[1024];
  int i,j,ipt,ieta,runNo;
  int passLooseIdBarrel,passLooseIdProbe,passLooseIdJet3;
  bool cut_dphi,cut_ratio,cut_jet3,cut_id;
  float dphi,ratio,ptBarrel,ptProbe,ptJet3,etaProbe,eta,etaJet3,dijetPt,balance,pthat,wt;
  TrigStruct HLTTrigResults[5];
  double veta[MAX_NETA];
  double vpt[MAX_NPT]; 
  for(i=0;i<=NETA;i++)
    veta[i] = EtaBnd[i];
  for(i=0;i<=NPT;i++)
    vpt[i] = DijetPtBnd[i];
        
  ///////////////////////////////////////////////////////////////  
  TIter next(inf->GetListOfKeys());
  TKey* key(0);
while ((key=(TKey*)next())) {
  if (strcmp(key->GetClassName(),"TDirectoryFile")!=0) continue;
    
  TDirectoryFile* idir = (TDirectoryFile*)key->ReadObj();
  string alg(idir->GetName());
  if (JetAlgos.size()>0&&!contains(JetAlgos,alg)) continue;
  cout<<alg<<" ... "<<endl;

  TTree *tr = (TTree*)idir->Get("DiJetTree");
  TBranch *brRunNo = (TBranch*)tr->GetBranch("runNo");
  brRunNo->SetAddress(&runNo);
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
  
  TBranch *brIdBarrel = (TBranch*)tr->GetBranch("passLooseIdBarrel");
  TBranch *brIdProbe = (TBranch*)tr->GetBranch("passLooseIdProbe");
  TBranch *brIdJet3 = (TBranch*)tr->GetBranch("passLooseIdJet3");
  brIdBarrel->SetAddress(&passLooseIdBarrel);
  brIdProbe->SetAddress(&passLooseIdProbe);
  brIdJet3->SetAddress(&passLooseIdJet3); 
  TBranch *b_hltResults[5]; 
  for(unsigned int k=0;k<NHLT;k++) 
    {
      b_hltResults[k] = (TBranch*)tr->GetBranch(HLTbit[k].c_str());
      b_hltResults[k]->SetAddress(&HLTTrigResults[k]);
    }
  if (IsMCarlo)
    {
      TBranch *brPtHat = (TBranch*)tr->GetBranch("pthat");
      brPtHat->SetAddress(&pthat);
    }
  ///////////////////////////////////////////////////////////////
  cout<<"Booking histograms"<<endl;
  TDirectoryFile* odir = (TDirectoryFile*)outf->mkdir(alg.c_str());
  odir->cd();
  TH1F *hPtProbe[MAX_NPT][MAX_NETA][MAX_NHLT];
  TH1F *hBalance[MAX_NPT][MAX_NETA][MAX_NHLT];
  TH1F *hDPhi[MAX_NPT][MAX_NHLT];
  TH1F *hDPhiCuts[MAX_NPT][MAX_NHLT];
  TH1F *hRatio[MAX_NPT][MAX_NHLT];
  TH1F *hRatioCuts[MAX_NPT][MAX_NHLT];
  TH1F *hPtJet3[MAX_NPT][MAX_NHLT];
  TH1F *hPtJet3Cuts[MAX_NPT][MAX_NHLT];
  TH1F *hDijetPt[MAX_NHLT];
  TH1F *hDijetPtCoarse[MAX_NHLT];
  TH1F *hDijetPtCuts[MAX_NHLT];
  TH1F *hDijetPtCoarseCuts[MAX_NHLT];
  TH1F *hDijetPtEff[MAX_NHLT];
  TH1F *hDijetPtEffBase[MAX_NHLT];
  TH1F *hEvents[MAX_NPT][MAX_NHLT];
  TH1F *hEventsCuts[MAX_NPT][MAX_NHLT];
  TH2F *h2DPt[MAX_NHLT];
  TH2F *h2DPtCuts[MAX_NHLT];
  TH2F *h2DDPhiRatio[MAX_NHLT];
  TH2F *h2DDPhiRatioCuts[MAX_NHLT];
  for(unsigned int k=0;k<NHLT;k++)
    {
      int nbins = (int)DijetPtBnd[NPT];
      double max = DijetPtBnd[NPT];

      sprintf(name,"DijetPt_HLT%d",k);
      hDijetPt[k] = new TH1F(name,name,nbins,0,max);
  
      sprintf(name,"DijetPtCoarse_HLT%d",k);
      hDijetPtCoarse[k] = new TH1F(name,name,NPT,vpt);

      sprintf(name,"DijetPtCoarseCuts_HLT%d",k);
      hDijetPtCoarseCuts[k] = new TH1F(name,name,NPT,vpt);

      sprintf(name,"DijetPtCuts_HLT%d",k);
      hDijetPtCuts[k] = new TH1F(name,name,nbins,0,max);
 
      sprintf(name,"DijetPtEff_HLT%d",k);
      hDijetPtEff[k] = new TH1F(name,name,nbins,0,max);

      sprintf(name,"DijetPtEffBase_HLT%d",k);
      hDijetPtEffBase[k] = new TH1F(name,name,nbins,0,max);

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
          int ipthat = getBin(pthat,PtHatBnd);
          double xsec = PtHatXsec[ipthat];
          double nevt = PtHatNevt[ipthat];
          if (ApplyWeights) 
            wt = xsec/nevt; 
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
      // need to investigate the PFJet ID
      //cut_id    = (passLooseIdBarrel == 1 && passLooseIdProbe == 1);
      cut_id = true;
 
      //if (passLooseIdJet3 == 0)//jet3 coming probabaly from noise. Accept the event.
        //cut_jet3 = true;
      //else 
      cut_jet3  = (cut_ratio || (ptJet3<PTJET3_CUT));
      ipt       = getBin(dijetPt,DijetPtBnd);
      ieta      = getBin(eta,EtaBnd); 
      if (ipt<0 || ieta<0 || !cut_id) continue;
      for(unsigned int k=0;k<NHLT-1;k++)
        {
          if (cut_dphi && cut_jet3)
            {
              if (HLTTrigResults[k].fired == 1)
                hDijetPtEffBase[k+1]->Fill(dijetPt,wt);  
              if (HLTTrigResults[k].fired == 1 && HLTTrigResults[k+1].fired == 1)
                hDijetPtEff[k+1]->Fill(dijetPt,wt);
            }
        }
      for(unsigned int k=0;k<NHLT;k++)
        { 
          if (HLTTrigResults[k].fired == 1) {  
            hDPhi[ipt][k]      ->Fill(Phi_0_2pi(dphi),wt);
            hRatio[ipt][k]     ->Fill(ratio,wt); 
            hPtJet3[ipt][k]    ->Fill(ptJet3,wt);
            hEvents[ipt][k]    ->Fill(eta,wt); 
            hDijetPt[k]        ->Fill(dijetPt,wt);
            hDijetPtCoarse[k]  ->Fill(dijetPt,wt);
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
                hDijetPtCoarseCuts[k] ->Fill(dijetPt,wt);
                h2DPtCuts[k]          ->Fill(ptBarrel,ptProbe,wt);
                h2DDPhiRatioCuts[k]   ->Fill(ratio,Phi_0_2pi(dphi),wt);
              }
           }
        }
    }// tree loop
    cout<<" DONE."<<endl;
  }// algo loop
  outf->Write();
  gROOT->GetListOfFiles()->Remove(outf);
  outf->Close();
  delete outf;
  inf->Close();
  delete inf;
  return 0;
}
