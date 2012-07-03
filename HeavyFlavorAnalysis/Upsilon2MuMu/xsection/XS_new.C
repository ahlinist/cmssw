#include "TMath.h"
#include <math.h>
#include "TF1.h"
#include "TH1.h"

void XS_7vs9(){

  TFile *f = new TFile("XSection_v4_t1.root");
  TH1D *Pt_rA;
  Pt_rA = (TH1D*)gFile->Get("S1YieldPt");
  TH1D *Eta_rA;
  Eta_rA = (TH1D*)gFile->Get("S1YieldEta");
  Pt_rA->SetMarkerColor(4); Eta_rA->SetMarkerColor(4);
  Pt_rA->SetLineColor(4); Eta_rA->SetLineColor(4);
  
  
  TFile *f1 = new TFile("XSection_v5_t1.root");
  TH1D *Pt_rB;
  Pt_rB = (TH1D*)gFile->Get("S1YieldPt");
  TH1D *Eta_rB;
  Eta_rB = (TH1D*)gFile->Get("S1YieldEta");
  
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,600); 
  c1->Divide(2,1); 
  c1->cd(1);
  Pt_rA->Draw("p");
  Pt_rB->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  //legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.08); legg->SetTextFont(132); 
  legg->SetHeader("XSection Comparison For Ups(1S)");
  legge = legg->AddEntry(Pt_rA,  "Dimuon7_Upsilon_Barrel_v1,4","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(Pt_rB,  "Dimuon9_Upsilon_Barrel_v1,4 ","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  c1->cd(2);
  Eta_rA->Draw("p");
  Eta_rB->Draw("psame"); 
  legg1 = new TLegend(0.3,0.65,0.7,0.9);
  //legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.08); legg->SetTextFont(132); 
  legg1->SetHeader("XSection Comparison For Ups(1S)");
  legge1 = legg1->AddEntry(Eta_rA,  "Dimuon7_Upsilon_Barrel_v1,4 ","p"); legge1->SetTextColor(kBlack);
  legge1 = legg1->AddEntry(Eta_rB,  "Dimuon9_Upsilon_Barrel_v1,4 ","p"); legge1->SetTextColor(kBlack);
  legg1->Draw();  
  
  
}

void XS_0vs5(){

  TFile *f = new TFile("XSection_v2_t1.root");
  TH1D *Pt_rA;
  Pt_rA = (TH1D*)gFile->Get("S1YieldPt");
  TH1D *Eta_rA;
  Eta_rA = (TH1D*)gFile->Get("S1YieldEta");
  Pt_rA->SetMarkerColor(4); Eta_rA->SetMarkerColor(4);
  Pt_rA->SetLineColor(4); Eta_rA->SetLineColor(4);
  
  
  TFile *f1 = new TFile("XSection_v3_t1.root");
  TH1D *Pt_rB;
  Pt_rB = (TH1D*)gFile->Get("S1YieldPt");
  TH1D *Eta_rB;
  Eta_rB = (TH1D*)gFile->Get("S1YieldEta");
  
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,600); 
  c1->Divide(2,1); 
  c1->cd(1);
  Pt_rA->Draw("p");
  Pt_rB->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  //legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.08); legg->SetTextFont(132); 
  legg->SetHeader("XSection Comparison For Ups(1S)");
  legge = legg->AddEntry(Pt_rA,  "Dimuon0_Barrel_Upsilon_v1","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(Pt_rB,  "Dimuon5_Upsilon_Barrel_v*","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  c1->cd(2);
  Eta_rA->Draw("p");
  Eta_rB->Draw("psame"); 
  legg1 = new TLegend(0.3,0.65,0.7,0.9);
  //legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.08); legg->SetTextFont(132); 
  legg1->SetHeader("XSection Comparison For Ups(1S)");
  legge1 = legg1->AddEntry(Eta_rA,  "Dimuon0_Barrel_Upsilon_v1","p"); legge1->SetTextColor(kBlack);
  legge1 = legg1->AddEntry(Eta_rB,  "Dimuon5_Upsilon_Barrel_v*","p"); legge1->SetTextColor(kBlack);
  legg1->Draw();  
  
  
}

void XS_5vs7(){

  TFile *f = new TFile("XSection_v3_t1.root");
  TH1D *Pt_rA;
  Pt_rA = (TH1D*)gFile->Get("S1YieldPt");
  TH1D *Eta_rA;
  Eta_rA = (TH1D*)gFile->Get("S1YieldEta");
  Pt_rA->SetMarkerColor(4); Eta_rA->SetMarkerColor(4);
  Pt_rA->SetLineColor(4); Eta_rA->SetLineColor(4);
  
  
  TFile *f1 = new TFile("XSection_v4_t1.root");
  TH1D *Pt_rB;
  Pt_rB = (TH1D*)gFile->Get("S1YieldPt");
  TH1D *Eta_rB;
  Eta_rB = (TH1D*)gFile->Get("S1YieldEta");
  
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,600); 
  c1->Divide(2,1); 
  c1->cd(1);
  Pt_rA->Draw("p");
  Pt_rB->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  //legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.08); legg->SetTextFont(132); 
  legg->SetHeader("XSection Comparison For Ups(1S)");
  legge = legg->AddEntry(Pt_rA,  "Dimuon5_Upsilon_Barrel_v*","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(Pt_rB,  "Dimuon7_Upsilon_Barrel_v*","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  c1->cd(2);
  Eta_rA->Draw("p");
  Eta_rB->Draw("psame"); 
  legg1 = new TLegend(0.3,0.65,0.7,0.9);
  //legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.08); legg->SetTextFont(132); 
  legg1->SetHeader("XSection Comparison For Ups(1S)");
  legge1 = legg1->AddEntry(Eta_rA,  "Dimuon5_Upsilon_Barrel_v*","p"); legge1->SetTextColor(kBlack);
  legge1 = legg1->AddEntry(Eta_rB,  "Dimuon7_Upsilon_Barrel_v*","p"); legge1->SetTextColor(kBlack);
  legg1->Draw();  
  
  
}




