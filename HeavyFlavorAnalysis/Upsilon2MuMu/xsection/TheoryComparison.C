#include "TMath.h"
#include <math.h>
#include "TF1.h"
#include "TH1.h"

void CSM(){
  
  gStyle->SetOptStat(00000000000);
  TFile *f = new TFile("Final1S.root");
  TGraphAsymmErrors *S1;
  S1 = (TGraphAsymmErrors*)gFile->Get("Ups1S");
  
  TString ytitle = TString::Format("d^{2}#sigma/dp_{T}d|y| #times BR(#mu#mu)  (nb/(GeV/c))");
  
  float scale=1.0/0.5; //the feed down fraction factor 

  float Nlo_pt[8]={3,5,10,15,20,25,30,35};
  float Nlo_pt_err[8]={0,0,0,0,0,0,0,0};
  float Nlo_high[8]={0.34,0.205, 0.024,0.0033,0.00066,0.000135,0.000045,0.000014};
  float Nlo_low[8] ={0.135,0.083,0.011,0.00175, 0.00033, 0.000075,0.000025,0.0000085};
  float Nlo_mean[8];
  float Nlo_error[8];
  for (int i=0; i<8; i++){
    Nlo_mean[i]=(Nlo_high[i]+Nlo_low[i])/2.0;
    Nlo_error[i]=(Nlo_high[i]-Nlo_low[i])/2.0;
    Nlo_mean[i]=Nlo_mean[i]*scale;
    Nlo_error[i]=Nlo_error[i]*scale;
  }
  TGraphErrors* NLO = new TGraphErrors(8,Nlo_pt,Nlo_mean,Nlo_pt_err,Nlo_error);
  
  float NNlo_pt[7]={5,10,15,20,25,30,35};
  float NNlo_pt_err[7]={0,0,0,0,0,0,0};
  float NNlo_high[7]={0.51, 0.104, 0.0189, 0.0048, 0.00128,  0.00039, 0.000138};
  float NNlo_low[7] ={0.087,0.0132,0.0027,0.00062,0.000163,0.000061,0.0000217};
  float NNlo_mean[7];
  float NNlo_error[7];
  for (int i=0; i<7; i++){
    NNlo_mean[i]=(NNlo_high[i]+NNlo_low[i])/2.0;
    NNlo_error[i]=(NNlo_high[i]-NNlo_low[i])/2.0;
    NNlo_mean[i]=NNlo_mean[i]*scale;
    NNlo_error[i]=NNlo_error[i]*scale;
  }
  TGraphErrors* NNLO = new TGraphErrors(7,NNlo_pt,NNlo_mean,NNlo_pt_err,NNlo_error);
  
  TCanvas *c2 = new TCanvas();//"c1","title",800,600);
  TH1F *frame = gPad->DrawFrame(0,0.000005,35,2);
  frame->SetStats(0);
  frame->GetXaxis()->SetTitle("p_{T}^{Y} (GeV/c)");
  frame->GetYaxis()->SetTitle("d#sigma/dp_{T} * BR (nb/(GeV/c))");
  frame->GetXaxis()->SetTitle("p_{T}^{Y} (GeV/c)");
  frame->GetYaxis()->SetTitle(ytitle);
  frame->GetYaxis()->SetTitleOffset(1.1);
  frame->Draw();
  
  c2->cd();
  c2->SetLeftMargin(0.15);
  c2->SetLogy();
  frame->Draw();

  S1->Draw("P");
  NLO->SetFillColor(2);
  NLO->SetLineColor(2);
  NLO->Draw("3");

  NNLO->SetFillColor(4);
  NNLO->SetFillStyle(3244);
  NNLO->SetLineColor(4);
  NNLO->Draw("same 3");
  
  TLegend *leg = new TLegend(0.65,0.65,0.89,0.89);
  leg->SetBorderSize(0);
  leg->SetFillColor(10);

  leg->AddEntry(NLO, "NLO prediction", "FL");
  leg->AddEntry(NNLO,"NNLO* prediction", "FL");
  leg->Draw();

  
  c1->SaveAs("CSM_NLO_0_2.pdf");
}

void NRQCD(){
  
  gStyle->SetOptStat(00000000000);
  TFile *f = new TFile("Final1S.root");
  TGraphAsymmErrors *S1;
  S1 = (TGraphAsymmErrors*)gFile->Get("Ups1S");
  
  TString ytitle = TString::Format("d^{2}#sigma/dp_{T}d|y| #times BR(#mu#mu)  (nb/(GeV/c))");
  
  float Nrqcd_pt[7]={8., 10., 14., 18., 22., 26., 30.};
  float Nrqcd_pt_err[7]={0,0,0,0,0,0,0};
  float Nrqcd_high[7]={0.5814, 0.3467, 0.0991, 0.0326, 0.0131, 0.0064, 0.0035};
  float Nrqcd_low[7] ={0.2704, 0.1824, 0.0515, 0.0127, 0.0040, 0.0015, 0.0007};
  float Nrqcd_mean[7];
  float Nrqcd_error[7];
  for (int i=0; i<7; i++){
    Nrqcd_mean[i]=(Nrqcd_high[i]+Nrqcd_low[i])/2.0;
    Nrqcd_error[i]=(Nrqcd_high[i]-Nrqcd_low[i])/2.0;
  }
  TGraphErrors* Nrqcd = new TGraphErrors(7,Nrqcd_pt,Nrqcd_mean,Nrqcd_pt_err,Nrqcd_error);
  
  TCanvas *c2 = new TCanvas();//"c1","title",800,600);
  TH1F *frame = gPad->DrawFrame(0,0.000005,35,2);
  frame->SetStats(0);
  frame->GetXaxis()->SetTitle("p_{T}^{Y} (GeV/c)");
  frame->GetYaxis()->SetTitle("d#sigma/dp_{T} * BR (nb/(GeV/c))");
  frame->GetXaxis()->SetTitle("p_{T}^{Y} (GeV/c)");
  frame->GetYaxis()->SetTitle(ytitle);
  frame->GetYaxis()->SetTitleOffset(1.1);
  frame->Draw();
  
  c2->cd();
  c2->SetLeftMargin(0.15);
  c2->SetLogy();
  frame->Draw();

  Nrqcd->SetFillColor(3);
  Nrqcd->SetLineColor(3);
  Nrqcd->Draw("3");
  S1->Draw("P");
  TLegend *leg = new TLegend(0.65,0.65,0.89,0.89);
  leg->SetBorderSize(0);
  leg->SetFillColor(10);

  leg->AddEntry(Nrqcd, "NRQCD prediction", "FL");
  leg->Draw();

  
  c1->SaveAs("COM_NRQCD_0_2.pdf");
}
