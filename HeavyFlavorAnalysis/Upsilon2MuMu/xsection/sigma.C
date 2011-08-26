#include "TMath.h"
#include <math.h>
#include "TF1.h"
#include "TH1.h"


void Sigma(){
  gStyle->SetOptStat(00000000000);
  TFile *f = new TFile("SigmaDATA_PtInt.root");
  TH1D *hSigma1S;
  hSigma1S = (TH1D*)gFile->Get("hSigma1S");
  TH1D *hSigma2S;
  hSigma2S = (TH1D*)gFile->Get("hSigma2S");
     
  TCanvas *c1 = new TCanvas("c1","c1",800,600);
  hSigma1S->SetTitle("");
  hSigma1S->GetYaxis()->SetTitleOffset(1.2);
  hSigma1S->GetYaxis()->SetTitle("Mass Resolution [MeV/c^{2}]");
  hSigma1S->GetXaxis()->SetTitle("|y^{#Upsilon}|");
  hSigma1S->SetMinimum(0.);
  hSigma1S->SetMarkerStyle(8);
  hSigma1S->Draw("e");
  c1->SaveAs("sigma1S.pdf");
  
  TCanvas *c2 = new TCanvas("c2","c2",800,600); 
  hSigma2S->SetTitle("");
  hSigma2S->GetYaxis()->SetTitleOffset(1.2);
  hSigma2S->GetYaxis()->SetTitle("Mass Resolution [MeV/c^{2}]");
  hSigma2S->GetXaxis()->SetTitle("|y^{#Upsilon}|");
  hSigma2S->SetMinimum(0.);
  hSigma2S->SetMarkerStyle(8);  
  hSigma2S->Draw("e");
  c2->SaveAs("sigma2S.pdf");
  
}

void SigmaDATAvsMC(){
  gStyle->SetOptStat(00000000000);
  TFile *f = new TFile("SigmaDATA_PtInt.root");
  TH1D *hSigma1S;
  hSigma1S = (TH1D*)gFile->Get("hSigma1S");
  
  TFile *f1 = new TFile("FitParametersMC_1D_PtInt.root");
  TH1D *hSigma1S_mc;
  hSigma1S_mc = (TH1D*)gFile->Get("hSigma1S"); 
  
  TCanvas *c5 = new TCanvas("c5","c5",800,600);
  hSigma1S->SetTitle("");
  hSigma1S->GetYaxis()->SetTitleOffset(1.2);
  hSigma1S->GetYaxis()->SetTitle("Mass Resolution [MeV/c^{2}]");
  hSigma1S->GetXaxis()->SetTitle("|y^{#Upsilon}|");
  hSigma1S->SetMinimum(0.);
  hSigma1S->SetMarkerStyle(8);
  hSigma1S_mc->SetMarkerColor(2);
  hSigma1S_mc->SetMarkerStyle(8);
  hSigma1S->Draw("e");
  hSigma1S_mc->Draw("samee");
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.08); legg->SetTextFont(132);
  legge = legg->AddEntry(hSigma1S,  "Data ","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(hSigma1S_mc,  "MC ","p"); legge->SetTextColor(kRed);
  legg->Draw();
  //c5->SaveAs("sigma1S_DATAvsMC.pdf");
  
}



void SigmaPt(){
  gStyle->SetOptStat(00000000000);
  TFile *f = new TFile("SigmaDATA_RapInt_1SBin.root");
  TH1D *hSigma1S;
  hSigma1S = (TH1D*)gFile->Get("hSigma1S");
  TH1D *hSigma2S;
  hSigma2S = (TH1D*)gFile->Get("hSigma2S");
     
  TCanvas *c3 = new TCanvas("c3","c3",800,600);
  hSigma1S->SetTitle("");
  hSigma1S->GetYaxis()->SetTitleOffset(1.2);
  hSigma1S->GetYaxis()->SetTitle("Mass Resolution [MeV/c^{2}]");
  hSigma1S->GetXaxis()->SetTitle("p_{T}^{#Upsilon}");
  hSigma1S->SetMinimum(0.);
  hSigma1S->SetMarkerStyle(8);
  hSigma1S->Draw("e");
  c3->SaveAs("sigma1S_pt.pdf");
  
  TCanvas *c4 = new TCanvas("c4","c4",800,600); 
  hSigma2S->SetTitle("");
  hSigma2S->GetYaxis()->SetTitleOffset(1.2);
  hSigma2S->GetYaxis()->SetTitle("Mass Resolution [MeV/c^{2}]");
  hSigma2S->GetXaxis()->SetTitle("p_{T}^{#Upsilon}");
  hSigma2S->SetMinimum(0.);
  hSigma2S->SetMarkerStyle(8);  
  hSigma2S->Draw("e");
  c4->SaveAs("sigma2S_pt.pdf");
  
}

void SigmaPtDATAvsMC(){
  gStyle->SetOptStat(00000000000);
  TFile *f = new TFile("SigmaDATA_RapInt_1SBin.root");
  TH1D *hSigma1S;
  hSigma1S = (TH1D*)gFile->Get("hSigma1S");
  
  TFile *f1 = new TFile("FitParametersMC_1D_RapInt.root");
  TH1D *hSigma1S_mc;
  hSigma1S_mc = (TH1D*)gFile->Get("hSigma1S"); 
  
  TCanvas *c6 = new TCanvas("c6","c6",800,600);
  hSigma1S->SetTitle("");
  hSigma1S->GetYaxis()->SetTitleOffset(1.2);
  hSigma1S->GetYaxis()->SetTitle("Mass Resolution [MeV/c^{2}]");
  hSigma1S->GetXaxis()->SetTitle("p_{T}^{#Upsilon}");
  hSigma1S->SetMinimum(0.);
  hSigma1S->SetMarkerStyle(8);
  hSigma1S_mc->SetMarkerColor(2);
  hSigma1S_mc->SetMarkerStyle(8);
  hSigma1S_mc->SetBinError(24,0.002);
  hSigma1S->Draw("e");
  hSigma1S_mc->Draw("samee");
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.08); legg->SetTextFont(132);
  legge = legg->AddEntry(hSigma1S,  "Data ","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(hSigma1S_mc,  "MC ","p"); legge->SetTextColor(kRed);
  legg->Draw();
  //c6->SaveAs("sigma1S_ptDATAvsMC.pdf");
  
}

