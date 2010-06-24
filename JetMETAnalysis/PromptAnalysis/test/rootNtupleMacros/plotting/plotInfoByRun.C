#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include"TFile.h"
#include"TMath.h"
#include"TH1.h"
#include"TH2.h"
#include"TTree.h"
#include"TCanvas.h"
#include"TString.h"
#include "TPaveStats.h"
#include "TLegend.h"
#include "TStyle.h"
#include "tdrStyle.C"
#include "TROOT.h"
//#include ".h"
//#include ".h"


void plotInfoByRun(){
  setTDRStyle();
  gROOT->ForceStyle();
  TFile *data = TFile::Open("test.root"); 
  TTree *tree=(TTree*) data->Get("tree");

  int nrunsmax=100;
  int nruns;
  int runnumberTMP[nrunsmax];
  double lumiTMP[nrunsmax];
  int NumOfJetsTMP[nrunsmax];
  double SumJetPtTMP[nrunsmax];
  int SelectedEvtsTMP[nrunsmax];
  int NumOfJets_dijetsTMP[nrunsmax];
  double SumJetPt_dijetsTMP[nrunsmax];
  int SelectedEvts_dijetsTMP[nrunsmax];


  tree->SetBranchAddress("nruns",&nruns);
  tree->SetBranchAddress("runnumber",&runnumberTMP);
  tree->SetBranchAddress("lumi",&lumiTMP);
  tree->SetBranchAddress("SumJetPt",&SumJetPtTMP);
  tree->SetBranchAddress("NumOfJets",&NumOfJetsTMP);
  tree->SetBranchAddress("SelectedEvts",&SelectedEvtsTMP);
  tree->SetBranchAddress("SumJetPt_dijets",&SumJetPt_dijetsTMP);
  tree->SetBranchAddress("NumOfJets_dijets",&NumOfJets_dijetsTMP);
  tree->SetBranchAddress("SelectedEvts_dijets",&SelectedEvts_dijetsTMP);
  tree->GetEntry(0);

  int runnum[nruns];
  double lumi[nruns];
  int selEv[nruns];
  int Njets[nruns];
  double JetSumPt[nruns];
  int selEv_dj[nruns];
  int Njets_dj[nruns];
  double JetSumPt_dj[nruns];

  //reorder the runnnumbers
  int NewI=0;
  while(NewI<nruns){

  int minRN=3*runnumberTMP[0];
  int MinI=999;
  for(int i=0; i<nruns; i++) {
    if(runnumberTMP[i]<minRN && runnumberTMP[i]!=-1){
      MinI=i;
    }
  }
  runnum[NewI]=runnumberTMP[MinI];
  lumi[NewI]=lumiTMP[MinI];
  selEv[NewI]=SelectedEvtsTMP[MinI];
  Njets[NewI]=NumOfJetsTMP[MinI];
  JetSumPt[NewI]=SumJetPtTMP[MinI];
  selEv_dj[NewI]=SelectedEvts_dijetsTMP[MinI];
  Njets_dj[NewI]=NumOfJets_dijetsTMP[MinI];
  JetSumPt_dj[NewI]=SumJetPt_dijetsTMP[MinI];
  NewI++;
  runnumberTMP[MinI]=-1;
}

  TString names[nrunsmax];
  char arg[100];

  for(int n=0; n<nruns; n++){
    sprintf(arg,"%d",runnum[n]);
    names[n]=(TString)arg;
  }
  

  //define histos with right number of bins

  int nbins=nruns;

  TH1D *SelectedEvts = new TH1D("SelectedEvts","SelectedEvts",nbins,0.,nbins-1);
  TH1D *AvJetPt = new TH1D("AvJetPt","AvJetPt",nbins,0.,nbins-1);
  TH1D *SelectedEvts_dijets = new TH1D("SelectedEvts_dijets","SelectedEvts_dijets",nbins,0.,nbins-1);
  TH1D *AvJetPt_dijets = new TH1D("AvJetPt_dijets","AvJetPt_dijets",nbins,0.,nbins-1);

  for(int h=0; h<nbins;h++){
    if(lumi[h]<=0.) continue;
    SelectedEvts->GetXaxis()->SetBinLabel(h+1,names[h]);
    SelectedEvts->SetBinContent(h+1,selEv[h]/lumi[h]);
    //    if(selEv[h]>0)    SelectedEvts->SetBinError(h+1,sqrt(selEv[h])/lumi[h]);
    //
    SelectedEvts_dijets->GetXaxis()->SetBinLabel(h+1,names[h]);
    SelectedEvts_dijets->SetBinContent(h+1,selEv_dj[h]/lumi[h]);
    //
    AvJetPt->GetXaxis()->SetBinLabel(h+1,names[h]);
    if(Njets[h]>0) AvJetPt->SetBinContent(h+1,JetSumPt[h]/Njets[h]);
    //
    AvJetPt_dijets->GetXaxis()->SetBinLabel(h+1,names[h]);
    if(Njets_dj[h]>0) AvJetPt_dijets->SetBinContent(h+1,JetSumPt_dj[h]/Njets_dj[h]);
  }
  //style
    SelectedEvts->SetYTitle("Events/lumi [nb]");
    SelectedEvts->LabelsOption("v","X");
    SelectedEvts->SetMaximum(1.5*SelectedEvts->GetMaximum());
    //
    SelectedEvts_dijets->SetYTitle("Events/lumi [nb]");
    SelectedEvts_dijets->LabelsOption("v","X");
    SelectedEvts_dijets->SetMaximum(1.5*SelectedEvts_dijets->GetMaximum());
    //
    AvJetPt->SetYTitle("Average Jet p_{T} [GeV]");
    AvJetPt->LabelsOption("v","X");
    AvJetPt->SetMaximum(1.5*AvJetPt->GetMaximum());
    //
    AvJetPt_dijets->SetYTitle("Average Jet p_{T}[GeV]");
    AvJetPt_dijets->LabelsOption("v","X");
    AvJetPt_dijets->SetMaximum(1.5*AvJetPt_dijets->GetMaximum());



  TCanvas *c = new TCanvas("c","",600,600);
  TPaveText *title = new TPaveText(0.5,0.75,0.9,0.9,"blNDC");
  title->SetFillStyle(4000);
  title->SetFillColor(kWhite);
  title->SetBorderSize(0.1);
  title->SetTextFont(42);
  title->AddText("JPT jets");
  title->AddText("p_{T}>25 GeV , |#eta|<2.6");
  //
  TPaveText *titledj = new TPaveText(0.5,0.75,0.9,0.9,"blNDC");
  titledj->SetFillStyle(4000);
  titledj->SetFillColor(kWhite);
  titledj->SetBorderSize(0.1);
  titledj->SetTextFont(42);
  titledj->AddText("JPT dijets");
  titledj->AddText("p_{T}>25 GeV , |#eta|<3 , #Delta#phi>2.1");
  //


  SelectedEvts->Draw();
  title->Draw();
  c->SaveAs("SelectedEvts.gif");
  AvJetPt->Draw();
  title->Draw();
  c->SaveAs("AvJetPt.gif");
  title->Draw();
  SelectedEvts_dijets->Draw();
  titledj->Draw();
  c->SaveAs("SelectedEvts_dijets.gif");
  AvJetPt_dijets->Draw();
  titledj->Draw();
  c->SaveAs("AvJetPt_dijets.gif");
  c->Close();

}
