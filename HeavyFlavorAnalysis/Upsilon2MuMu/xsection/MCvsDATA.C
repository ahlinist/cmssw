#include "TMath.h"
#include <math.h>
#include "TF1.h"
#include "TH1.h"


void All(){
  gStyle->SetOptStat(00000000000);
  TFile *f = new TFile("DataForMCvsDATA.root");
  TH1D *CandPt_data;
  CandPt_data = (TH1D*)gFile->Get("CandPt");
  TH1D *CandEta_data;
  CandEta_data = (TH1D*)gFile->Get("CandEta");
  TH1D *CandRapidity_data;
  CandRapidity_data = (TH1D*)gFile->Get("CandRapidity");
  TH1D *SigMuEta_data;
  SigMuEta_data = (TH1D*)gFile->Get("SigMuEta");
  TH1D *SigMuPt_data;
  SigMuPt_data = (TH1D*)gFile->Get("SigMuPt");  
  
  TFile *f = new TFile("MCForMCvsDATA.root");
  TH1D *CandPt_mc;
  CandPt_mc = (TH1D*)gFile->Get("CandPt");
  TH1D *CandEta_mc;
  CandEta_mc = (TH1D*)gFile->Get("CandEta");
  TH1D *CandRapidity_mc;
  CandRapidity_mc = (TH1D*)gFile->Get("CandRapidity");
  TH1D *SigMuEta_mc;
  SigMuEta_mc = (TH1D*)gFile->Get("SigMuEta");
  TH1D *SigMuPt_mc;
  SigMuPt_mc = (TH1D*)gFile->Get("SigMuPt");   
  
  double scale_cand(-1);
  scale_cand = CandPt_data->GetEntries()/CandPt_mc->GetEntries();
  
  CandPt_data->Rebin(2);
  CandPt_mc->Rebin(2);
  
  TCanvas *c1 = new TCanvas("c1","c1",800,600);
  CandPt_data->SetTitle("#Upsilon(1S) p_{T}");
  CandPt_data->GetXaxis()->SetTitle("p_{T}^{#Upsilon} [GeV/c]");
  CandPt_data->SetLineColor(2);
  CandPt_mc->SetMarkerStyle(22);
  //CandPt_mc->Scale(scale_cand);
  CandPt_data->Draw();
  CandPt_mc->Draw("same");
  //c1->SaveAs("CandPt.pdf");
  
  CandRapidity_data->Rebin(2);
  CandRapidity_mc->Rebin(2);
  
  TCanvas *c2 = new TCanvas("c2","c2",800,600);
  CandRapidity_data->SetTitle("#Upsilon(1S) y");
  CandRapidity_data->GetXaxis()->SetTitle("|y^{#Upsilon}|");
  CandRapidity_data->SetLineColor(2);
  CandRapidity_mc->SetMarkerStyle(22);
  //CandRapidity_mc->Scale(scale_cand);
  CandRapidity_data->Draw();
  CandRapidity_mc->Draw("same");
  //c2->SaveAs("CandRapidity.pdf");  
  
  TCanvas *c3 = new TCanvas("c3","c3",800,600);
  CandEta_data->SetTitle("#Upsilon(1S) #eta");
  CandEta_data->GetXaxis()->SetTitle("#eta^{#Upsilon}");
  CandEta_data->SetLineColor(2);
  CandEta_mc->SetMarkerStyle(22);
  //CandEta_mc->Scale(scale_cand);
  CandEta_data->Draw();
  CandEta_mc->Draw("same");
  //c3->SaveAs("CandEta.pdf");  
  
  TCanvas *c4 = new TCanvas("c4","c4",800,600);
  SigMuEta_data->SetTitle("#Upsilon(1S) muon #eta");
  SigMuEta_data->GetXaxis()->SetTitle("#eta^{#mu}");
  SigMuEta_data->SetLineColor(2);
  SigMuEta_mc->SetMarkerStyle(22);
  //SigMuEta_mc->Scale(scale_cand);
  SigMuEta_data->Draw();
  SigMuEta_mc->Draw("same");
  //c4->SaveAs("MuonEta.pdf");   
  
  SigMuPt_data->Rebin(2);
  SigMuPt_mc->Rebin(2);
  
  TCanvas *c5 = new TCanvas("c5","c5",800,600);
  SigMuPt_data->SetTitle("#Upsilon(1S) muon p_{T}");
  SigMuPt_data->GetXaxis()->SetTitle("P_{T}^{#mu}");
  SigMuPt_data->SetLineColor(2);
  SigMuPt_mc->SetMarkerStyle(22);
  //SigMuPt_mc->Scale(scale_cand);
  SigMuPt_data->Draw();
  SigMuPt_mc->Draw("same");
  //c5->SaveAs("MuonPt.pdf");  
  
}



