#include "TMath.h"
#include <math.h>
#include "TF1.h"
#include "TH1.h"

void XSrho(){

  TFile *f = new TFile("Syst/24ptbins/XSection_24ptbins.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S1YieldPt");
  TFile *f = new TFile("Syst/24ptbins/Rho/XSection_24ptbins_2SrhoMuID.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S1YieldPt");
  TFile *f = new TFile("Syst/24ptbins/Rho/XSection_24ptbins_3SrhoMuID.root");
  TH1D *S3;
  S3 = (TH1D*)gFile->Get("S1YieldPt");
  TH1D *S12 = (TH1D*)S3->Clone();
  TH1D *S13 = (TH1D*)S3->Clone();
  
  double s1(0.), s2(0.), s3(0.);
  double e1(0.), e2(0.), e3(0.);
  for (int i = 1; i <= S1->GetNbinsX(); ++i) {
    
    s1 += S1->GetBinContent(i)*S1->GetBinWidth(i);
    e1 += (S1->GetBinError(i)*S1->GetBinWidth(i))*(S1->GetBinError(i)*S1->GetBinWidth(i));
    s2 += S2->GetBinContent(i)*S2->GetBinWidth(i);
    e2 += (S2->GetBinError(i)*S2->GetBinWidth(i))*(S2->GetBinError(i)*S2->GetBinWidth(i));
    s3 += S3->GetBinContent(i)*S3->GetBinWidth(i);
    e3 += (S3->GetBinError(i)*S3->GetBinWidth(i))*(S3->GetBinError(i)*S3->GetBinWidth(i));
    S12->SetBinContent(i,(S1->GetBinContent(i)*S1->GetBinWidth(i))-(S2->GetBinContent(i)*S2->GetBinWidth(i)));
    S13->SetBinContent(i,(S1->GetBinContent(i)*S1->GetBinWidth(i))-(S3->GetBinContent(i)*S3->GetBinWidth(i)));
    S12->SetBinError(i,0.); S13->SetBinError(i,0.); 
  }
  
  cout << " Y(1S) 1SrhoMuID Xsection = "  << s1 << "+/-" << TMath::Sqrt(e1) << endl;
  cout << " Y(1S) 2SrhoMuID Xsection = "  << s2 << "+/-" << TMath::Sqrt(e2) << endl;
  cout << " Y(2S) 3SrhoMuID Xsection = "  << s3 << "+/-" << TMath::Sqrt(e3) << endl;
  
  S1->SetMarkerColor(3); S2->SetMarkerColor(4); S3->SetMarkerColor(5); 
  S1->SetLineColor(3); S2->SetLineColor(4); S3->SetLineColor(5);
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,600); 
  S1->SetMaximum(1.4);
  S1->Draw("p");
  S2->Draw("psame");
  S3->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("XSection Vs P_{T}  For Ups(1S)");
  legge = legg->AddEntry(S1, "Y(1S) 1SrhoMuID","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "Y(1S) 2SrhoMuID","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S3, "Y(1S) 3SrhoMuID","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S12->SetLineColor(3); S12->SetMarkerColor(3); 
  S13->SetLineColor(4); S13->SetMarkerColor(4);
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  S12->SetMinimum(-0.01);
  S12->SetMaximum(0.01);
  S12->Draw("p");
  S13->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("#Delta XSection Vs P_{T}  For Ups(1S)");
  legge = legg->AddEntry(S12, "Y(1S) 1SrhoMuID - 2SrhoMuID","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S13, "Y(1S) 1SrhoMuID - 3SrhoMuID","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  
}

