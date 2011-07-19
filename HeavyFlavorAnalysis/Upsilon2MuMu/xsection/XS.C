#include "TMath.h"
#include <math.h>
#include "TF1.h"
#include "TH1.h"

void XS_AvsB(){

  TFile *f = new TFile("XSection_rA.root");
  TH1D *Pt_rA;
  Pt_rA = (TH1D*)gFile->Get("S1YieldPt");
  TH1D *Eta_rA;
  Eta_rA = (TH1D*)gFile->Get("S1YieldEta");
  Pt_rA->SetMarkerColor(4); Eta_rA->SetMarkerColor(4);
  Pt_rA->SetLineColor(4); Eta_rA->SetLineColor(4);
  
  
  TFile *f1 = new TFile("XSection_rB.root");
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
  legge = legg->AddEntry(Pt_rA,  "Run2010A Results ","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(Pt_rB,  "Run2010B Results ","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  c1->cd(2);
  Eta_rA->Draw("p");
  Eta_rB->Draw("psame"); 
  legg1 = new TLegend(0.3,0.65,0.7,0.9);
  //legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.08); legg->SetTextFont(132); 
  legg1->SetHeader("XSection Comparison For Ups(1S)");
  legge1 = legg1->AddEntry(Eta_rA,  "Run2010A Results ","p"); legge1->SetTextColor(kBlack);
  legge1 = legg1->AddEntry(Eta_rB,  "Run2010B Results ","p"); legge1->SetTextColor(kBlack);
  legg1->Draw();  
  
  
}

void XS_B(){

  TFile *f = new TFile("XSection_rB_s.root");
  TH1D *Pt_rA;
  Pt_rA = (TH1D*)gFile->Get("S1YieldPt");
  TH1D *Eta_rA;
  Eta_rA = (TH1D*)gFile->Get("S1YieldEta");
  Pt_rA->SetMarkerColor(4); Eta_rA->SetMarkerColor(4);
  Pt_rA->SetLineColor(4); Eta_rA->SetLineColor(4);
  
  
  TFile *f1 = new TFile("XSection_rB.root");
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
  legg->SetHeader("XSection Comparison For Ups(1S) in Run2010B");
  legge = legg->AddEntry(Pt_rA,  "Results for 146240-147116","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(Pt_rB,  "Results for 147196-149442","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  c1->cd(2);
  Eta_rA->Draw("p");
  Eta_rB->Draw("psame"); 
  legg1 = new TLegend(0.3,0.65,0.7,0.9);
  //legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.08); legg->SetTextFont(132); 
  legg1->SetHeader("XSection Comparison For Ups(1S) in Run2010B");
  legge1 = legg1->AddEntry(Eta_rA,  "Results for 146240-147116","p"); legge1->SetTextColor(kBlack);
  legge1 = legg1->AddEntry(Eta_rB,  "Results 147196-149442","p"); legge1->SetTextColor(kBlack);
  legg1->Draw();  
  
  
}


void XS_Ratio(){

  TFile *f = new TFile("XSection_50mbin_douRap_3Sbin.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S1YieldPt");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S2YieldPt");
  TH1D *S3;
  S3 = (TH1D*)gFile->Get("S3YieldPt");
  TH1D *Ratio_12 = (TH1D*)S1->Clone(); 
  Ratio_12->SetName("Ratio_12");
  Ratio_12->Reset();
  TH1D *Ratio_13 = (TH1D*)S1->Clone(); 
  Ratio_13->SetName("Ratio_13");
  Ratio_13->Reset();  
  S1->SetMarkerColor(3); S2->SetMarkerColor(4); S3->SetMarkerColor(5); 
  S1->SetLineColor(3); S2->SetLineColor(4); S3->SetLineColor(5);
  
  double s1(-99), s2(-99), s3(-99);
  double s1_(0.), s2_(0.), s3_(0.);
  double ratio12(-99), ratio13(-99);
  for (int i = 1; i <= S1->GetNbinsX(); ++i) {
    
    s1 = S1->GetBinContent(i); s1_ += S1->GetBinContent(i); 
    s2 = S2->GetBinContent(i); s2_ += S2->GetBinContent(i);
    s3 = S3->GetBinContent(i); s3_ += S3->GetBinContent(i);
    
    ratio12 = s2 / s1;
    ratio13 = s3 / s1;
    cout << "ratio12 = " << ratio12 << endl;
    cout << "ratio13 = " << ratio13 << endl;
    Ratio_12->SetBinContent(i,ratio12);
    Ratio_13->SetBinContent(i,ratio13);
    s1=0; s2=0; s3=0; ratio12=0; ratio13=0;
  
  }
  
  cout << "Y(2S)/Y(1S) = "  << s2_/s1_  << " Y(3S)/Y(1S) = " << s3_/s1_ << endl;
  
  Ratio_12->SetMarkerColor(4); Ratio_13->SetMarkerColor(5);
  Ratio_12->SetLineColor(4); Ratio_13->SetLineColor(5);
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,600); 
  Ratio_12->Draw("p");
  Ratio_12->SetMinimum(0.);
  Ratio_13->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("XSection Ratios For Ups(nS)");
  legge = legg->AddEntry(Ratio_12, "Y(2S)/Y(1S)","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(Ratio_13, "Y(3S)/Y(1S)","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
}


void XS_Total(){

  TFile *f = new TFile("XSection.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S1YieldEta");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S2YieldEta");
  TH1D *S3;
  S3 = (TH1D*)gFile->Get("S3YieldEta");
    
  double s1(0.), s2(0.), s3(0.);
  for (int i = 1; i <= S1->GetNbinsX(); ++i) {
    
    s1 += S1->GetBinContent(i);
    s2 += S2->GetBinContent(i);
    s3 += S3->GetBinContent(i);
    
  }
  
  cout << " Y(1S) Xsection = "  << s1 << endl;
  cout << " Y(2S) Xsection = "  << s2 << endl;
  cout << " Y(3S) Xsection = "  << s3 << endl;
  
  S1->SetMarkerColor(3); S2->SetMarkerColor(4); S3->SetMarkerColor(5); 
  S1->SetLineColor(3); S2->SetLineColor(4); S3->SetLineColor(5);
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,600); 
  S1->SetMinimum(0.);
  S1->Draw("p");
  S2->Draw("psame");
  S3->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("XSection Vs Rapidity  For Ups(nS)");
  legge = legg->AddEntry(S1, "Y(1S)","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "Y(2S)","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S3, "Y(3S)","p"); legge->SetTextColor(kBlack);
  legg->Draw();

  
  
}

void XS_Totalv2(){

  TFile *f = new TFile("XSection.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S1YieldPt");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S2YieldPt");
  TH1D *S3;
  S3 = (TH1D*)gFile->Get("S3YieldPt");
    
  double s1(0.), s2(0.), s3(0.);
  for (int i = 1; i <= S1->GetNbinsX(); ++i) {
    
    s1 += S1->GetBinContent(i)*S1->GetBinWidth(i);
    s2 += S2->GetBinContent(i)*S2->GetBinWidth(i);
    s3 += S3->GetBinContent(i)*S2->GetBinWidth(i);
    
  }
  
  cout << " Y(1S) Xsection = "  << s1 << endl;
  cout << " Y(2S) Xsection = "  << s2 << endl;
  cout << " Y(3S) Xsection = "  << s3 << endl;
  
  S1->SetMarkerColor(3); S2->SetMarkerColor(4); S3->SetMarkerColor(5); 
  S1->SetLineColor(3); S2->SetLineColor(4); S3->SetLineColor(5);
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,600); 
  S1->SetMinimum(0.);
  S1->Draw("p");
  S2->Draw("psame");
  S3->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("XSection Vs p_{T}  For Ups(nS)");
  legge = legg->AddEntry(S1, "Y(1S)","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "Y(2S)","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S3, "Y(3S)","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  
}

void XS_pt(){

  TFile *f = new TFile("XSection_1Sbin.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S1YieldPt");
  TFile *f1 = new TFile("XSection_2Sbin.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S2YieldPt");
  TFile *f2 = new TFile("XSection_3Sbin.root");
  TH1D *S3;
  S3 = (TH1D*)gFile->Get("S3YieldPt");
    
  double s1(0.), s2(0.), s3(0.), e1(0.), e2(0.), e3(0.);
  for (int i = 1; i <= S1->GetNbinsX(); ++i) {
    s1 += S1->GetBinContent(i)*S1->GetBinWidth(i);
    e1 += (S1->GetBinError(i)*S1->GetBinWidth(i))*(S1->GetBinError(i)*S1->GetBinWidth(i));
  }
  for (int i = 1; i <= S2->GetNbinsX(); ++i) {
    s2 += S2->GetBinContent(i)*S2->GetBinWidth(i);
    e2 += (S2->GetBinError(i)*S2->GetBinWidth(i))*(S2->GetBinError(i)*S2->GetBinWidth(i));
  }
  for (int i = 1; i <= S3->GetNbinsX(); ++i) {
    s3 += S3->GetBinContent(i)*S3->GetBinWidth(i);
    e3 += (S3->GetBinError(i)*S3->GetBinWidth(i))*(S3->GetBinError(i)*S3->GetBinWidth(i));
  }
  
  cout << " Y(1S) Xsection = "  << s1 << "+/-" << TMath::Sqrt(e1) << endl;
  cout << " Y(2S) Xsection = "  << s2 << "+/-" << TMath::Sqrt(e2) << endl;
  cout << " Y(3S) Xsection = "  << s3 << "+/-" << TMath::Sqrt(e3) << endl;
  
  S1->SetMarkerColor(3); S2->SetMarkerColor(4); S3->SetMarkerColor(5); 
  S1->SetLineColor(3); S2->SetLineColor(4); S3->SetLineColor(5);
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,600); 
  //S1->SetMinimum(0.);
  S1->Draw("pE");
  S2->Draw("psameE");
  S3->Draw("psameE");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("XSection Vs Pt  For Ups(nS)");
  legge = legg->AddEntry(S1, "Y(1S)","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "Y(2S)","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S3, "Y(3S)","p"); legge->SetTextColor(kBlack);
  legg->Draw();

  
}
