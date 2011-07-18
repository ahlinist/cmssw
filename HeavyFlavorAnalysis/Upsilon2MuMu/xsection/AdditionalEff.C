#include "TMath.h"
#include <math.h>
#include "TF1.h"
#include "TH1.h"
#include "../../../AnalysisDataFormats/HeavyFlavorObjects/rootio/PidTable.hh"


void Eff(){
  gStyle->SetOptStat(00000000000);
  TFile *f = new TFile("AnaEff.root");
  TH2D *fAnaEff;
  fAnaEff = (TH2D*)gFile->Get("fAnaEff");
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,600); 
  fAnaEff->SetTitle("");
  fAnaEff->GetYaxis()->SetTitle("p_{T}^{#Upsilon} [GeV/c]");
  fAnaEff->GetXaxis()->SetTitle("|y^{#Upsilon}|");
  fAnaEff->Draw("colz");
  c1->SaveAs("AnaEff.pdf");
  
  TFile *f1 = new TFile("preSelEff.root");
  TH2D *fPreSelEff;
  fPreSelEff = (TH2D*)gFile->Get("fPreSelEff");
  
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  fPreSelEff->SetTitle("");
  fPreSelEff->GetYaxis()->SetTitle("p_{T}^{#Upsilon} [GeV/c]");
  fPreSelEff->GetXaxis()->SetTitle("|y^{#Upsilon}|");
  fPreSelEff->Draw("colz");
  c2->SaveAs("PreSelEff.pdf");  
  

}

