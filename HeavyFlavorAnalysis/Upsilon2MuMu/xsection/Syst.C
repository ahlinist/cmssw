#include "TMath.h"
#include <math.h>
#include "TF1.h"
#include "TH1.h"

void XSNoMSC_1S(){
  
  //TFile *f = new TFile("Yield_MSC.root");
  TFile *f = new TFile("Yield_MSC_v3.root");
  TH2D *S1;
  S1 = (TH2D*)gFile->Get("fS1Yield");
  //TFile *f = new TFile("Yield_NoMSC.root");
  TFile *f = new TFile("Yield_NoMSC_v3.root");
  TH2D *S2;
  S2 = (TH2D*)gFile->Get("fS1Yield");
  //TFile *f = new TFile("XSection1Srho_finalversion.root");
  TFile *f = new TFile("XSection1Sfinal.root");
  TH1D *S3;
  S3 = (TH1D*)gFile->Get("S1YieldPt");  
  TH1D *S11 = (TH1D*)S3->Clone();
  TH1D *S22 = (TH1D*)S3->Clone();
  TH1D *S12 = (TH1D*)S3->Clone();
  TH1D *S13 = (TH1D*)S3->Clone();
  
  double s1(0.), s2(0.);
  double e1(0.), e2(0.);
  
  for (int j = 1; j <= S1->GetNbinsY(); ++j) {
    for (int i = 1; i <= S1->GetNbinsX(); ++i) {
      cout << "i = " << i << " j = " << j << endl;
      cout << "S1->GetBinContent(i,j) = " <<S1->GetBinContent(i,j) << " S2->GetBinContent(i,j) = " <<S2->GetBinContent(i,j) << endl; 
      s1 += S1->GetBinContent(i,j);
      s2 += S2->GetBinContent(i,j);
    }
    cout << "s1 = " <<s1<< " s2 = " <<s2 << endl;
    S11->SetBinContent(j,s1);
    S22->SetBinContent(j,s2);
    S12->SetBinContent(j,((s1-s2)/s1)*S3->GetBinContent(j));
    S13->SetBinContent(j,S12->GetBinContent(j)/S3->GetBinContent(j));
    s1=0.; s2=0.;
  }
  
  S13->Draw();
  TFile *f = new TFile("NoMSC_10ptbins_1Srho_v3.root", "RECREATE");
  S12->Write();
  
}

void XS2SRho_1S(){

  TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_1Srho.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S1YieldPt");
  TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_2Srho.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S1YieldPt");
  TH1D *S12 = (TH1D*)S2->Clone();
  TH1D *S13 = (TH1D*)S2->Clone();
  
  double s1(0.), s2(0.);
  double e1(0.), e2(0.);
  for (int i = 1; i <= S1->GetNbinsX(); ++i) {
    
    s1 += S1->GetBinContent(i)*S1->GetBinWidth(i);
    e1 += (S1->GetBinError(i)*S1->GetBinWidth(i))*(S1->GetBinError(i)*S1->GetBinWidth(i));
    s2 += S2->GetBinContent(i)*S2->GetBinWidth(i);
    e2 += (S2->GetBinError(i)*S2->GetBinWidth(i))*(S2->GetBinError(i)*S2->GetBinWidth(i));
    S12->SetBinContent(i, S1->GetBinContent(i) - S2->GetBinContent(i) );
    S12->SetBinError(i,0.); 
    S13->SetBinContent(i, S12->GetBinContent(i) / S1->GetBinContent(i) );
    S13->SetBinError(i,0.); 
		       
  }
  
  cout << " Y(1S) 1Srho Xsection = "  << s1 << "+/-" << TMath::Sqrt(e1) << endl;
  cout << " Y(1S) 2Srho Xsection = "  << s2 << "+/-" << TMath::Sqrt(e2) << endl;
  
  S1->SetMarkerColor(3); S2->SetMarkerColor(4); 
  S1->SetLineColor(3); S2->SetLineColor(4); 
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,600); 
  S1->SetMaximum(1.4);
  S1->Draw("p");
  S2->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("XSection Vs P_{T}  For Ups(1S)");
  legge = legg->AddEntry(S1, "Y(1S) 1Srho","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "Y(1S) 2Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S12->SetLineColor(3); S12->SetMarkerColor(3); 
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  //S12->SetMinimum(0.);
  //S12->SetMaximum(0.02);
  S12->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("#Delta XSection Vs P_{T}  For Ups(1S)");
  legge = legg->AddEntry(S12, "Y(1S) 1Srho - 2Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S13->SetLineColor(3); S13->SetMarkerColor(3); 
  TCanvas *c3 = new TCanvas("c3","c3",1200,600); 
  //S12->SetMinimum(0.);
  //S12->SetMaximum(0.02);
  S13->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("Percentage #Delta XSection Vs P_{T}  For Ups(1S)");
  legge = legg->AddEntry(S13, "Y(1S) 1Srho - 2Srho / Y(1S) 1Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  TFile *f = new TFile("2SRho_10ptbins_1Srho_.root", "RECREATE");
  S12->Write();
  
}

void XS3SRho_1S(){

  TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_1Srho.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S1YieldPt");
  TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_3Srho.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S1YieldPt");
  TH1D *S12 = (TH1D*)S2->Clone();
  TH1D *S13 = (TH1D*)S2->Clone();
  
  double s1(0.), s2(0.);
  double e1(0.), e2(0.);
  for (int i = 1; i <= S1->GetNbinsX(); ++i) {
    
    s1 += S1->GetBinContent(i)*S1->GetBinWidth(i);
    e1 += (S1->GetBinError(i)*S1->GetBinWidth(i))*(S1->GetBinError(i)*S1->GetBinWidth(i));
    s2 += S2->GetBinContent(i)*S2->GetBinWidth(i);
    e2 += (S2->GetBinError(i)*S2->GetBinWidth(i))*(S2->GetBinError(i)*S2->GetBinWidth(i));
    S12->SetBinContent(i, S1->GetBinContent(i) - S2->GetBinContent(i) );
    S12->SetBinError(i,0.); 
    S13->SetBinContent(i, S12->GetBinContent(i) / S1->GetBinContent(i) );
    S13->SetBinError(i,0.); 
		       
  }
  
  cout << " Y(1S) 1Srho Xsection = "  << s1 << "+/-" << TMath::Sqrt(e1) << endl;
  cout << " Y(1S) 3Srho Xsection = "  << s2 << "+/-" << TMath::Sqrt(e2) << endl;
  
  S1->SetMarkerColor(3); S2->SetMarkerColor(4); 
  S1->SetLineColor(3); S2->SetLineColor(4); 
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,600); 
  S1->SetMaximum(1.4);
  S1->Draw("p");
  S2->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("XSection Vs P_{T}  For Ups(1S)");
  legge = legg->AddEntry(S1, "Y(1S) 1Srho","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "Y(1S) 3Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S12->SetLineColor(3); S12->SetMarkerColor(3); 
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  //S12->SetMinimum(0.);
  //S12->SetMaximum(0.02);
  S12->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("#Delta XSection Vs P_{T}  For Ups(1S)");
  legge = legg->AddEntry(S12, "Y(1S) 1Srho - 3Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S13->SetLineColor(3); S13->SetMarkerColor(3); 
  TCanvas *c3 = new TCanvas("c3","c3",1200,600); 
  //S12->SetMinimum(0.);
  //S12->SetMaximum(0.02);
  S13->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("Percentage #Delta XSection Vs P_{T}  For Ups(1S)");
  legge = legg->AddEntry(S13, "Y(1S) 1Srho - 3Srho / Y(1S) 1Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  TFile *f = new TFile("3SRho_10ptbins_1Srho_.root", "RECREATE");
  S12->Write();
  
}


void XSMuIDPlus_1S(){

  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_1Srho.root");
  TFile *f = new TFile("XSection1Srho_finalversion.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S1YieldPt");
  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_1Srho_MuIDPlus.root");
  TFile *f = new TFile("MuIDPlus_10ptbins_1Srho_fv.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S1YieldPt");
  TH1D *S12 = (TH1D*)S2->Clone();
  TH1D *S13 = (TH1D*)S2->Clone();
  
  double s1(0.), s2(0.);
  double e1(0.), e2(0.);
  for (int i = 1; i <= S1->GetNbinsX(); ++i) {
    
    s1 += S1->GetBinContent(i)*S1->GetBinWidth(i);
    e1 += (S1->GetBinError(i)*S1->GetBinWidth(i))*(S1->GetBinError(i)*S1->GetBinWidth(i));
    s2 += S2->GetBinContent(i)*S2->GetBinWidth(i);
    e2 += (S2->GetBinError(i)*S2->GetBinWidth(i))*(S2->GetBinError(i)*S2->GetBinWidth(i));
    S12->SetBinContent(i, S1->GetBinContent(i) - S2->GetBinContent(i) );
    S12->SetBinError(i,0.); 
    S13->SetBinContent(i, S12->GetBinContent(i) / S1->GetBinContent(i) );
    S13->SetBinError(i,0.); 
		       
  }
  
  cout << " Y(1S) 1Srho Xsection = "  << s1 << "+/-" << TMath::Sqrt(e1) << endl;
  cout << " Y(1S) 1Srho_MuIDPlus Xsection = "  << s2 << "+/-" << TMath::Sqrt(e2) << endl;
  
  S1->SetMarkerColor(3); S2->SetMarkerColor(4); 
  S1->SetLineColor(3); S2->SetLineColor(4); 
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,600); 
  S1->SetMaximum(1.4);
  S1->Draw("p");
  S2->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("XSection Vs P_{T}  For Ups(1S)");
  legge = legg->AddEntry(S1, "Y(1S) 1Srho","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "Y(1S) 1Srho_MuIDPlus","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S12->SetLineColor(3); S12->SetMarkerColor(3); 
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  //S12->SetMinimum(0.);
  //S12->SetMaximum(0.02);
  S12->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("#Delta XSection Vs P_{T}  For Ups(1S)");
  legge = legg->AddEntry(S12, "Y(1S) 1Srho - 1Srho_MuIDPlus","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S13->SetLineColor(3); S13->SetMarkerColor(3); 
  TCanvas *c3 = new TCanvas("c3","c3",1200,600); 
  //S12->SetMinimum(0.);
  //S12->SetMaximum(0.02);
  S13->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("Percentage #Delta XSection Vs P_{T}  For Ups(1S)");
  legge = legg->AddEntry(S13, "Y(1S) 1Srho - 1Srho_MuIDPlus / Y(1S) 1Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  //TFile *f = new TFile("MuIDPlus_10ptbins_1Srho_.root", "RECREATE");
  TFile *f = new TFile("MuIDPlus_10ptbins_1Srho_v2.root", "RECREATE");
  S12->Write();
  
}

void XSMuIDMinus_1S(){

  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_1Srho.root");
  TFile *f = new TFile("XSection1Srho_finalversion.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S1YieldPt");
  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_1Srho_MuIDMinus.root");
  TFile *f = new TFile("MuIDMinus_10ptbins_1Srho_fv.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S1YieldPt");
  TH1D *S12 = (TH1D*)S2->Clone();
  TH1D *S13 = (TH1D*)S2->Clone();
  
  double s1(0.), s2(0.);
  double e1(0.), e2(0.);
  for (int i = 1; i <= S1->GetNbinsX(); ++i) {
    
    s1 += S1->GetBinContent(i)*S1->GetBinWidth(i);
    e1 += (S1->GetBinError(i)*S1->GetBinWidth(i))*(S1->GetBinError(i)*S1->GetBinWidth(i));
    s2 += S2->GetBinContent(i)*S2->GetBinWidth(i);
    e2 += (S2->GetBinError(i)*S2->GetBinWidth(i))*(S2->GetBinError(i)*S2->GetBinWidth(i));
    S12->SetBinContent(i, S1->GetBinContent(i) - S2->GetBinContent(i) );
    S12->SetBinError(i,0.); 
    S13->SetBinContent(i, S12->GetBinContent(i) / S1->GetBinContent(i) );
    S13->SetBinError(i,0.); 
		       
  }
  
  cout << " Y(1S) 1Srho Xsection = "  << s1 << "+/-" << TMath::Sqrt(e1) << endl;
  cout << " Y(1S) 1Srho_MuIDMinus Xsection = "  << s2 << "+/-" << TMath::Sqrt(e2) << endl;
  
  S1->SetMarkerColor(3); S2->SetMarkerColor(4); 
  S1->SetLineColor(3); S2->SetLineColor(4); 
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,600); 
  S1->SetMaximum(1.4);
  S1->Draw("p");
  S2->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("XSection Vs P_{T}  For Ups(1S)");
  legge = legg->AddEntry(S1, "Y(1S) 1Srho","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "Y(1S) 1Srho_MuIDMinus","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S12->SetLineColor(3); S12->SetMarkerColor(3); 
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  //S12->SetMinimum(0.);
  //S12->SetMaximum(0.02);
  S12->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("#Delta XSection Vs P_{T}  For Ups(1S)");
  legge = legg->AddEntry(S12, "Y(1S) 1Srho - 1Srho_MuIDMinus","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S13->SetLineColor(3); S13->SetMarkerColor(3); 
  TCanvas *c3 = new TCanvas("c3","c3",1200,600); 
  //S12->SetMinimum(0.);
  //S12->SetMaximum(0.02);
  S13->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("Percentage #Delta XSection Vs P_{T}  For Ups(1S)");
  legge = legg->AddEntry(S13, "Y(1S) 1Srho - 1Srho_MuIDMinus / Y(1S) 1Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  //TFile *f = new TFile("MuIDMinus_10ptbins_1Srho_.root", "RECREATE");
  TFile *f = new TFile("MuIDMinus_10ptbins_1Srho_v2.root", "RECREATE");
  S12->Write();
  
}

void XSTrigMinus_1S(){

  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_1Srho.root");
  TFile *f = new TFile("XSection1Srho_finalversion.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S1YieldPt");
  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_1Srho_TrigMinus.root");
  TFile *f = new TFile("TrigMinus_10ptbins_1Srho_fv.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S1YieldPt");
  TH1D *S12 = (TH1D*)S2->Clone();
  TH1D *S13 = (TH1D*)S2->Clone();
  
  double s1(0.), s2(0.);
  double e1(0.), e2(0.);
  for (int i = 1; i <= S1->GetNbinsX(); ++i) {
    
    s1 += S1->GetBinContent(i)*S1->GetBinWidth(i);
    e1 += (S1->GetBinError(i)*S1->GetBinWidth(i))*(S1->GetBinError(i)*S1->GetBinWidth(i));
    s2 += S2->GetBinContent(i)*S2->GetBinWidth(i);
    e2 += (S2->GetBinError(i)*S2->GetBinWidth(i))*(S2->GetBinError(i)*S2->GetBinWidth(i));
    S12->SetBinContent(i, S1->GetBinContent(i) - S2->GetBinContent(i) );
    S12->SetBinError(i,0.); 
    S13->SetBinContent(i, S12->GetBinContent(i) / S1->GetBinContent(i) );
    S13->SetBinError(i,0.); 
		       
  }
  
  cout << " Y(1S) 1Srho Xsection = "  << s1 << "+/-" << TMath::Sqrt(e1) << endl;
  cout << " Y(1S) 1Srho_TrigMinus Xsection = "  << s2 << "+/-" << TMath::Sqrt(e2) << endl;
  
  S1->SetMarkerColor(3); S2->SetMarkerColor(4); 
  S1->SetLineColor(3); S2->SetLineColor(4); 
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,600); 
  S1->SetMaximum(1.4);
  S1->Draw("p");
  S2->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("XSection Vs P_{T}  For Ups(1S)");
  legge = legg->AddEntry(S1, "Y(1S) 1Srho","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "Y(1S) 1Srho_TrigMinus","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S12->SetLineColor(3); S12->SetMarkerColor(3); 
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  //S12->SetMinimum(0.);
  //S12->SetMaximum(0.02);
  S12->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("#Delta XSection Vs P_{T}  For Ups(1S)");
  legge = legg->AddEntry(S12, "Y(1S) 1Srho - 1Srho_TrigMinus","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S13->SetLineColor(3); S13->SetMarkerColor(3); 
  TCanvas *c3 = new TCanvas("c3","c3",1200,600); 
  //S12->SetMinimum(0.);
  //S12->SetMaximum(0.02);
  S13->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("Percentage #Delta XSection Vs P_{T}  For Ups(1S)");
  legge = legg->AddEntry(S13, "Y(1S) 1Srho - 1Srho_TrigMinus / Y(1S) 1Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  //TFile *f = new TFile("TrigMinus_10ptbins_1Srho_.root", "RECREATE");
  TFile *f = new TFile("TrigMinus_10ptbins_1Srho_v2.root", "RECREATE");
  S12->Write();
  
}

void XSTrigPlus_1S(){

  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_1Srho.root");
  TFile *f = new TFile("XSection1Srho_finalversion.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S1YieldPt");
  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_1Srho_TrigPlus.root");
  TFile *f = new TFile("TrigPlus_10ptbins_1Srho_fv.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S1YieldPt");
  TH1D *S12 = (TH1D*)S2->Clone();
  TH1D *S13 = (TH1D*)S2->Clone();
  
  double s1(0.), s2(0.);
  double e1(0.), e2(0.);
  for (int i = 1; i <= S1->GetNbinsX(); ++i) {
    
    s1 += S1->GetBinContent(i)*S1->GetBinWidth(i);
    e1 += (S1->GetBinError(i)*S1->GetBinWidth(i))*(S1->GetBinError(i)*S1->GetBinWidth(i));
    s2 += S2->GetBinContent(i)*S2->GetBinWidth(i);
    e2 += (S2->GetBinError(i)*S2->GetBinWidth(i))*(S2->GetBinError(i)*S2->GetBinWidth(i));
    S12->SetBinContent(i, S1->GetBinContent(i) - S2->GetBinContent(i) );
    S12->SetBinError(i,0.); 
    S13->SetBinContent(i, S12->GetBinContent(i) / S1->GetBinContent(i) );
    S13->SetBinError(i,0.); 
		       
  }
  
  cout << " Y(1S) 1Srho Xsection = "  << s1 << "+/-" << TMath::Sqrt(e1) << endl;
  cout << " Y(1S) 1Srho_TrigMinus Xsection = "  << s2 << "+/-" << TMath::Sqrt(e2) << endl;
  
  S1->SetMarkerColor(3); S2->SetMarkerColor(4); 
  S1->SetLineColor(3); S2->SetLineColor(4); 
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,600); 
  S1->SetMaximum(1.4);
  S1->Draw("p");
  S2->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("XSection Vs P_{T}  For Ups(1S)");
  legge = legg->AddEntry(S1, "Y(1S) 1Srho","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "Y(1S) 1Srho_TrigPlus","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S12->SetLineColor(3); S12->SetMarkerColor(3); 
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  //S12->SetMinimum(0.);
  //S12->SetMaximum(0.02);
  S12->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("#Delta XSection Vs P_{T}  For Ups(1S)");
  legge = legg->AddEntry(S12, "Y(1S) 1Srho - 1Srho_TrigPlus","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S13->SetLineColor(3); S13->SetMarkerColor(3); 
  TCanvas *c3 = new TCanvas("c3","c3",1200,600); 
  //S12->SetMinimum(0.);
  //S12->SetMaximum(0.02);
  S13->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("Percentage #Delta XSection Vs P_{T}  For Ups(1S)");
  legge = legg->AddEntry(S13, "Y(1S) 1Srho - 1Srho_TrigPlus / Y(1S) 1Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  //TFile *f = new TFile("TrigPlus_10ptbins_1Srho_.root", "RECREATE");
  TFile *f = new TFile("TrigPlus_10ptbins_1Srho_v2.root", "RECREATE");
  S12->Write();
  
}


void XSSigPDF_1S(){

  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_1Srho.root");
  TFile *f = new TFile("XSection1Sfinal.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S1YieldPt");
  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_1Srho_SigPDF.root");
  TFile *f = new TFile("XSection1Sfinal_sigPDF.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S1YieldPt");
  TH1D *S12 = (TH1D*)S2->Clone();
  TH1D *S13 = (TH1D*)S2->Clone();
  
  double s1(0.), s2(0.);
  double e1(0.), e2(0.);
  for (int i = 1; i <= S1->GetNbinsX(); ++i) {
    
    s1 += S1->GetBinContent(i)*S1->GetBinWidth(i);
    e1 += (S1->GetBinError(i)*S1->GetBinWidth(i))*(S1->GetBinError(i)*S1->GetBinWidth(i));
    s2 += S2->GetBinContent(i)*S2->GetBinWidth(i);
    e2 += (S2->GetBinError(i)*S2->GetBinWidth(i))*(S2->GetBinError(i)*S2->GetBinWidth(i));
    S12->SetBinContent(i, S1->GetBinContent(i) - S2->GetBinContent(i) );
    S12->SetBinError(i,0.); 
    S13->SetBinContent(i, S12->GetBinContent(i) / S1->GetBinContent(i) );
    S13->SetBinError(i,0.); 
		       
  }
  
  cout << " Y(1S) 1Srho Xsection = "  << s1 << "+/-" << TMath::Sqrt(e1) << endl;
  cout << " Y(1S) 1Srho_SigPDF Xsection = "  << s2 << "+/-" << TMath::Sqrt(e2) << endl;
  
  S1->SetMarkerColor(3); S2->SetMarkerColor(4); 
  S1->SetLineColor(3); S2->SetLineColor(4); 
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,600); 
  S1->SetMaximum(1.4);
  S1->Draw("p");
  S2->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("XSection Vs P_{T}  For Ups(1S)");
  legge = legg->AddEntry(S1, "Y(1S) 1Srho","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "Y(1S) 1Srho_SigPDF","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S12->SetLineColor(3); S12->SetMarkerColor(3); 
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  //S12->SetMinimum(0.);
  //S12->SetMaximum(0.02);
  S12->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("#Delta XSection Vs P_{T}  For Ups(1S)");
  legge = legg->AddEntry(S12, "Y(1S) 1Srho - 1Srho_SigPDF","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S13->SetLineColor(3); S13->SetMarkerColor(3); 
  TCanvas *c3 = new TCanvas("c3","c3",1200,600); 
  //S12->SetMinimum(0.);
  //S12->SetMaximum(0.02);
  S13->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("Percentage #Delta XSection Vs P_{T}  For Ups(1S)");
  legge = legg->AddEntry(S13, "Y(1S) 1Srho - 1Srho_SigPDF / Y(1S) 1Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  TFile *f = new TFile("SigPDF_10ptbins_1Srho_v2.root", "RECREATE");
  S12->Write();
  
}

void XSBgPDF_1S(){

  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_1Srho_BgPDF_ref.root");
  TFile *f = new TFile("XSection1Sfinal.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S1YieldPt");
  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_1Srho_BgPDF.root");
  TFile *f = new TFile("XSection1Sfinal_bgPDF.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S1YieldPt");
  TH1D *S12 = (TH1D*)S2->Clone();
  TH1D *S13 = (TH1D*)S2->Clone();
  
  double s1(0.), s2(0.);
  double e1(0.), e2(0.);
  for (int i = 1; i <= S1->GetNbinsX(); ++i) {
    
    s1 += S1->GetBinContent(i)*S1->GetBinWidth(i);
    e1 += (S1->GetBinError(i)*S1->GetBinWidth(i))*(S1->GetBinError(i)*S1->GetBinWidth(i));
    s2 += S2->GetBinContent(i)*S2->GetBinWidth(i);
    e2 += (S2->GetBinError(i)*S2->GetBinWidth(i))*(S2->GetBinError(i)*S2->GetBinWidth(i));
    S12->SetBinContent(i, S1->GetBinContent(i) - S2->GetBinContent(i) );
    S12->SetBinError(i,0.); 
    S13->SetBinContent(i, S12->GetBinContent(i) / S1->GetBinContent(i) );
    S13->SetBinError(i,0.); 
		       
  }
  
  cout << " Y(1S) 1Srho Xsection = "  << s1 << "+/-" << TMath::Sqrt(e1) << endl;
  cout << " Y(1S) 1Srho_BgPDF Xsection = "  << s2 << "+/-" << TMath::Sqrt(e2) << endl;
  
  S1->SetMarkerColor(3); S2->SetMarkerColor(4); 
  S1->SetLineColor(3); S2->SetLineColor(4); 
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,600); 
  S1->SetMaximum(1.4);
  S1->Draw("p");
  S2->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("XSection Vs P_{T}  For Ups(1S)");
  legge = legg->AddEntry(S1, "Y(1S) 1Srho","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "Y(1S) 1Srho_BgPDF","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S12->SetLineColor(3); S12->SetMarkerColor(3); 
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  //S12->SetMinimum(0.);
  //S12->SetMaximum(0.02);
  S12->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("#Delta XSection Vs P_{T}  For Ups(1S)");
  legge = legg->AddEntry(S12, "Y(1S) 1Srho - 1Srho_BgPDF","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S13->SetLineColor(3); S13->SetMarkerColor(3); 
  TCanvas *c3 = new TCanvas("c3","c3",1200,600); 
  //S12->SetMinimum(0.);
  //S12->SetMaximum(0.02);
  S13->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("Percentage #Delta XSection Vs P_{T}  For Ups(1S)");
  legge = legg->AddEntry(S13, "Y(1S) 1Srho - 1Srho_BgPDF / Y(1S) 1Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  TFile *f = new TFile("BgPDF_10ptbins_1Srho_v2.root", "RECREATE");
  S12->Write();
  
}

void XSSystTotNeg_1S(){

  TFile *f = new TFile("MuIDPlus_10ptbins_1Srho_.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S1YieldPt");
  TFile *f = new TFile("TrigPlus_10ptbins_1Srho_.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S1YieldPt");
  TFile *f = new TFile("SigPDF_10ptbins_1Srho_.root");
  TH1D *S3;
  S3 = (TH1D*)gFile->Get("S1YieldPt");
  TFile *f = new TFile("BgPDF_10ptbins_1Srho_.root");
  TH1D *S4;
  S4 = (TH1D*)gFile->Get("S1YieldPt");  
  TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_1Srho.root");
  TH1D *S5;
  S5 = (TH1D*)gFile->Get("S1YieldPt");    
  
  
  double s1(0.), s5(0.), s2(0.);
  for (int i = 1; i <= S1->GetNbinsX(); ++i) {
    
    s1 += S1->GetBinContent(i)*S1->GetBinContent(i);
    s1 += S2->GetBinContent(i)*S2->GetBinContent(i);
    s1 += S3->GetBinContent(i)*S3->GetBinContent(i);
    //s1 += S4->GetBinContent(i)*S4->GetBinContent(i);
    s1 += (S5->GetBinContent(i)/100)*(S5->GetBinContent(i)/100); // bg funct
    s1 += (S5->GetBinContent(i)/50)*(S5->GetBinContent(i)/50); // rho factor
    cout << " dSigma(Y(1S))/dp_{T} = "  << S5->GetBinContent(i) << " - " << TMath::Sqrt(s1) << endl;
    s5 += S5->GetBinContent(i)*S5->GetBinWidth(i);
    s2 += TMath::Sqrt(s1)*S5->GetBinWidth(i);
    s1=0;
    
  }
    
  cout << " Y(1S) 1Srho Xsection = "  << s5 << " - " << s2 << endl;
   
}

void XSSystTotPos_1S(){

  TFile *f = new TFile("MuIDMinus_10ptbins_1Srho_.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S1YieldPt");
  TFile *f = new TFile("TrigMinus_10ptbins_1Srho_.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S1YieldPt");
  TFile *f = new TFile("SigPDF_10ptbins_1Srho_.root");
  TH1D *S3;
  S3 = (TH1D*)gFile->Get("S1YieldPt");
  TFile *f = new TFile("BgPDF_10ptbins_1Srho_.root");
  TH1D *S4;
  S4 = (TH1D*)gFile->Get("S1YieldPt");  
  TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_1Srho.root");
  TH1D *S5;
  S5 = (TH1D*)gFile->Get("S1YieldPt");    
  
  
  double s1(0.), s5(0.), s2(0.);
  for (int i = 1; i <= S1->GetNbinsX(); ++i) {
    
    s1 += S1->GetBinContent(i)*S1->GetBinContent(i);
    s1 += S2->GetBinContent(i)*S2->GetBinContent(i);
    //s1 += S3->GetBinContent(i)*S3->GetBinContent(i);
    //s1 += S4->GetBinContent(i)*S4->GetBinContent(i);
    s1 += (S5->GetBinContent(i)/100)*(S5->GetBinContent(i)/100); // bg funct
    s1 += (S5->GetBinContent(i)/50)*(S5->GetBinContent(i)/50); // rho factor
    cout << " dSigma(Y(1S))/dp_{T} = "  << S5->GetBinContent(i) << " + " << TMath::Sqrt(s1) << endl;
    s5 += S5->GetBinContent(i)*S5->GetBinWidth(i);
    s2 += TMath::Sqrt(s1)*S5->GetBinWidth(i);
    s1=0;
    
  }
    
  cout << " Y(1S) 1Srho Xsection = "  << s5 << " + " << s2 << endl;
   
}

void XSNoMSC_2S(){
  
  //TFile *f = new TFile("Yield_MSC.root");
  TFile *f = new TFile("Yield_MSC_v3.root");
  TH2D *S1;
  S1 = (TH2D*)gFile->Get("fS2Yield");
  //TFile *f = new TFile("Yield_NoMSC.root");
  TFile *f = new TFile("Yield_NoMSC_v3.root");
  TH2D *S2;
  S2 = (TH2D*)gFile->Get("fS2Yield");
  //TFile *f = new TFile("XSection1Srho_finalversion.root");
  TFile *f = new TFile("XSection2Sfinal.root");
  TH1D *S3;
  S3 = (TH1D*)gFile->Get("S2YieldPt");  
  TH1D *S11 = (TH1D*)S3->Clone();
  TH1D *S22 = (TH1D*)S3->Clone();
  TH1D *S12 = (TH1D*)S3->Clone();
  TH1D *S13 = (TH1D*)S3->Clone();
  
  double s1(0.), s2(0.);
  double e1(0.), e2(0.);
  
  for (int j = 1; j <= S1->GetNbinsY(); ++j) {
    for (int i = 1; i <= S1->GetNbinsX(); ++i) {
      cout << "i = " << i << " j = " << j << endl;
      cout << "S1->GetBinContent(i,j) = " <<S1->GetBinContent(i,j) << " S2->GetBinContent(i,j) = " <<S2->GetBinContent(i,j) << endl; 
      s1 += S1->GetBinContent(i,j); 
      s2 += S2->GetBinContent(i,j);
    }
    cout << "s1 = " << s1 << " s2 = " << s2 << endl;
    S11->SetBinContent(j,s1);
    S22->SetBinContent(j,s2);
    S12->SetBinContent(j,((s1-s2)/s1)*S3->GetBinContent(j));
    S13->SetBinContent(j,S12->GetBinContent(j)/S3->GetBinContent(j));
    s1=0.; s2=0.;
  }
  
  S13->Draw();
  TFile *f = new TFile("NoMSC_10ptbins_2Srho_v3.root", "RECREATE");
  S12->Write();
  
}

void XS1SRho_2S(){

  TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_2Srho.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S2YieldPt");
  TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_1Srho.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S2YieldPt");
  TH1D *S12 = (TH1D*)S2->Clone();
  TH1D *S13 = (TH1D*)S2->Clone();
  
  double s1(0.), s2(0.);
  double e1(0.), e2(0.);
  for (int i = 1; i <= S1->GetNbinsX(); ++i) {
    
    s1 += S1->GetBinContent(i)*S1->GetBinWidth(i);
    e1 += (S1->GetBinError(i)*S1->GetBinWidth(i))*(S1->GetBinError(i)*S1->GetBinWidth(i));
    s2 += S2->GetBinContent(i)*S2->GetBinWidth(i);
    e2 += (S2->GetBinError(i)*S2->GetBinWidth(i))*(S2->GetBinError(i)*S2->GetBinWidth(i));
    S12->SetBinContent(i, S1->GetBinContent(i) - S2->GetBinContent(i) );
    S12->SetBinError(i,0.); 
    S13->SetBinContent(i, S12->GetBinContent(i) / S1->GetBinContent(i) );
    S13->SetBinError(i,0.); 
		       
  }
  
  cout << " Y(2S) 2Srho Xsection = "  << s1 << "+/-" << TMath::Sqrt(e1) << endl;
  cout << " Y(2S) 1Srho Xsection = "  << s2 << "+/-" << TMath::Sqrt(e2) << endl;
  
  S1->SetMarkerColor(3); S2->SetMarkerColor(4); 
  S1->SetLineColor(3); S2->SetLineColor(4); 
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,600); 
  S1->SetMaximum(1.4);
  S1->Draw("p");
  S2->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("XSection Vs P_{T}  For Ups(2S)");
  legge = legg->AddEntry(S1, "Y(2S) 2Srho","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "Y(2S) 1Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S12->SetLineColor(3); S12->SetMarkerColor(3); 
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  //S12->SetMinimum(0.);
  //S12->SetMaximum(0.02);
  S12->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("#Delta XSection Vs P_{T}  For Ups(2S)");
  legge = legg->AddEntry(S12, "Y(2S) 2Srho - 1Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S13->SetLineColor(3); S13->SetMarkerColor(3); 
  TCanvas *c3 = new TCanvas("c3","c3",1200,600); 
  //S12->SetMinimum(0.);
  //S12->SetMaximum(0.02);
  S13->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("Percentage #Delta XSection Vs P_{T}  For Ups(2S)");
  legge = legg->AddEntry(S13, "Y(2S) 2Srho - 1Srho / Y(2S) 2Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  TFile *f = new TFile("1SRho_10ptbins_2Srho_.root", "RECREATE");
  S12->Write();
  
}

void XS3SRho_2S(){

  TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_2Srho.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S2YieldPt");
  TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_3Srho.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S2YieldPt");
  TH1D *S12 = (TH1D*)S2->Clone();
  TH1D *S13 = (TH1D*)S2->Clone();
  
  double s1(0.), s2(0.);
  double e1(0.), e2(0.);
  for (int i = 1; i <= S1->GetNbinsX(); ++i) {
    
    s1 += S1->GetBinContent(i)*S1->GetBinWidth(i);
    e1 += (S1->GetBinError(i)*S1->GetBinWidth(i))*(S1->GetBinError(i)*S1->GetBinWidth(i));
    s2 += S2->GetBinContent(i)*S2->GetBinWidth(i);
    e2 += (S2->GetBinError(i)*S2->GetBinWidth(i))*(S2->GetBinError(i)*S2->GetBinWidth(i));
    S12->SetBinContent(i, S1->GetBinContent(i) - S2->GetBinContent(i) );
    S12->SetBinError(i,0.); 
    S13->SetBinContent(i, S12->GetBinContent(i) / S1->GetBinContent(i) );
    S13->SetBinError(i,0.); 
		       
  }
  
  cout << " Y(2S) 2Srho Xsection = "  << s1 << "+/-" << TMath::Sqrt(e1) << endl;
  cout << " Y(2S) 3Srho Xsection = "  << s2 << "+/-" << TMath::Sqrt(e2) << endl;
  
  S1->SetMarkerColor(3); S2->SetMarkerColor(4); 
  S1->SetLineColor(3); S2->SetLineColor(4); 
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,600); 
  S1->SetMaximum(1.4);
  S1->Draw("p");
  S2->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("XSection Vs P_{T}  For Ups(2S)");
  legge = legg->AddEntry(S1, "Y(2S) 2Srho","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "Y(2S) 3Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S12->SetLineColor(3); S12->SetMarkerColor(3); 
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  //S12->SetMinimum(0.);
  //S12->SetMaximum(0.02);
  S12->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("#Delta XSection Vs P_{T}  For Ups(2S)");
  legge = legg->AddEntry(S12, "Y(2S) 2Srho - 3Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S13->SetLineColor(3); S13->SetMarkerColor(3); 
  TCanvas *c3 = new TCanvas("c3","c3",1200,600); 
  //S12->SetMinimum(0.);
  //S12->SetMaximum(0.02);
  S13->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("Percentage #Delta XSection Vs P_{T}  For Ups(2S)");
  legge = legg->AddEntry(S13, "Y(2S) 2Srho - 3Srho / Y(2S) 2Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  TFile *f = new TFile("3SRho_10ptbins_2Srho_.root", "RECREATE");
  S12->Write();
  
}

void XSMuIDPlus_2S(){

  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_2Srho.root");
  TFile *f = new TFile("XSection2Srho_finalversion.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S2YieldPt");
  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_2Srho_MuIDPlus.root");
  TFile *f = new TFile("MuIDPlus_10ptbins_2Srho_fv.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S2YieldPt");
  TH1D *S12 = (TH1D*)S2->Clone();
  TH1D *S13 = (TH1D*)S2->Clone();
  
  double s1(0.), s2(0.);
  double e1(0.), e2(0.);
  for (int i = 1; i <= S1->GetNbinsX(); ++i) {
    
    s1 += S1->GetBinContent(i)*S1->GetBinWidth(i);
    e1 += (S1->GetBinError(i)*S1->GetBinWidth(i))*(S1->GetBinError(i)*S1->GetBinWidth(i));
    s2 += S2->GetBinContent(i)*S2->GetBinWidth(i);
    e2 += (S2->GetBinError(i)*S2->GetBinWidth(i))*(S2->GetBinError(i)*S2->GetBinWidth(i));
    S12->SetBinContent(i, S1->GetBinContent(i) - S2->GetBinContent(i) );
    S12->SetBinError(i,0.); 
    S13->SetBinContent(i, S12->GetBinContent(i) / S1->GetBinContent(i) );
    S13->SetBinError(i,0.); 
		       
  }
  
  cout << " Y(2S) 2Srho Xsection = "  << s1 << "+/-" << TMath::Sqrt(e1) << endl;
  cout << " Y(2S) 2Srho_MuIDPlus Xsection = "  << s2 << "+/-" << TMath::Sqrt(e2) << endl;
  
  S1->SetMarkerColor(3); S2->SetMarkerColor(4); 
  S1->SetLineColor(3); S2->SetLineColor(4); 
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,600); 
  S1->SetMaximum(1.4);
  S1->Draw("p");
  S2->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("XSection Vs P_{T}  For Ups(2S)");
  legge = legg->AddEntry(S1, "Y(2S) 2Srho","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "Y(2S) 2Srho_MuIDPlus","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S12->SetLineColor(3); S12->SetMarkerColor(3); 
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  //S12->SetMinimum(0.);
  //S12->SetMaximum(0.02);
  S12->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("#Delta XSection Vs P_{T}  For Ups(2S)");
  legge = legg->AddEntry(S12, "Y(2S) 2Srho - 2Srho_MuIDPlus","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S13->SetLineColor(3); S13->SetMarkerColor(3); 
  TCanvas *c3 = new TCanvas("c3","c3",1200,600); 
  //S12->SetMinimum(0.);
  //S12->SetMaximum(0.02);
  S13->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("Percentage #Delta XSection Vs P_{T}  For Ups(2S)");
  legge = legg->AddEntry(S13, "Y(2S) 2Srho - 2Srho_MuIDPlus / Y(2S) 2Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  //TFile *f = new TFile("MuIDPlus_10ptbins_2Srho_.root", "RECREATE");
  TFile *f = new TFile("MuIDPlus_10ptbins_2Srho_v2.root", "RECREATE");
  S12->Write();
  
}

void XSMuIDMinus_2S(){

  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_2Srho.root");
  TFile *f = new TFile("XSection2Srho_finalversion.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S2YieldPt");
  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_2Srho_MuIDMinus.root");
  TFile *f = new TFile("MuIDMinus_10ptbins_2Srho_fv.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S2YieldPt");
  TH1D *S12 = (TH1D*)S2->Clone();
  TH1D *S13 = (TH1D*)S2->Clone();
  
  double s1(0.), s2(0.);
  double e1(0.), e2(0.);
  for (int i = 1; i <= S1->GetNbinsX(); ++i) {
    
    s1 += S1->GetBinContent(i)*S1->GetBinWidth(i);
    e1 += (S1->GetBinError(i)*S1->GetBinWidth(i))*(S1->GetBinError(i)*S1->GetBinWidth(i));
    s2 += S2->GetBinContent(i)*S2->GetBinWidth(i);
    e2 += (S2->GetBinError(i)*S2->GetBinWidth(i))*(S2->GetBinError(i)*S2->GetBinWidth(i));
    S12->SetBinContent(i, S1->GetBinContent(i) - S2->GetBinContent(i) );
    S12->SetBinError(i,0.); 
    S13->SetBinContent(i, S12->GetBinContent(i) / S1->GetBinContent(i) );
    S13->SetBinError(i,0.); 
		       
  }
  
  cout << " Y(2S) 2Srho Xsection = "  << s1 << "+/-" << TMath::Sqrt(e1) << endl;
  cout << " Y(2S) 2Srho_MuIDMinus Xsection = "  << s2 << "+/-" << TMath::Sqrt(e2) << endl;
  
  S1->SetMarkerColor(3); S2->SetMarkerColor(4); 
  S1->SetLineColor(3); S2->SetLineColor(4); 
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,600); 
  S1->SetMaximum(1.4);
  S1->Draw("p");
  S2->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("XSection Vs P_{T}  For Ups(2S)");
  legge = legg->AddEntry(S1, "Y(2S) 2Srho","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "Y(2S) 2Srho_MuIDMinus","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S12->SetLineColor(3); S12->SetMarkerColor(3); 
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  //S12->SetMinimum(0.);
  //S12->SetMaximum(0.02);
  S12->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("#Delta XSection Vs P_{T}  For Ups(2S)");
  legge = legg->AddEntry(S12, "Y(2S) 2Srho - 2Srho_MuIDMinus","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S13->SetLineColor(3); S13->SetMarkerColor(3); 
  TCanvas *c3 = new TCanvas("c3","c3",1200,600); 
  //S12->SetMinimum(0.);
  //S12->SetMaximum(0.02);
  S13->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("Percentage #Delta XSection Vs P_{T}  For Ups(2S)");
  legge = legg->AddEntry(S13, "Y(2S) 2Srho - 2Srho_MuIDMinus / Y(2S) 2Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  //TFile *f = new TFile("MuIDMinus_10ptbins_2Srho_.root", "RECREATE");
  TFile *f = new TFile("MuIDMinus_10ptbins_2Srho_v2.root", "RECREATE");
  S12->Write();
  
}

void XSTrigMinus_2S(){

  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_2Srho.root");
  TFile *f = new TFile("XSection2Srho_finalversion.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S2YieldPt");
  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_2Srho_TrigMinus.root");
  TFile *f = new TFile("TrigMinus_10ptbins_2Srho_fv.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S2YieldPt");
  TH1D *S12 = (TH1D*)S2->Clone();
  TH1D *S13 = (TH1D*)S2->Clone();
  
  double s1(0.), s2(0.);
  double e1(0.), e2(0.);
  for (int i = 1; i <= S1->GetNbinsX(); ++i) {
    
    s1 += S1->GetBinContent(i)*S1->GetBinWidth(i);
    e1 += (S1->GetBinError(i)*S1->GetBinWidth(i))*(S1->GetBinError(i)*S1->GetBinWidth(i));
    s2 += S2->GetBinContent(i)*S2->GetBinWidth(i);
    e2 += (S2->GetBinError(i)*S2->GetBinWidth(i))*(S2->GetBinError(i)*S2->GetBinWidth(i));
    S12->SetBinContent(i, S1->GetBinContent(i) - S2->GetBinContent(i) );
    S12->SetBinError(i,0.); 
    S13->SetBinContent(i, S12->GetBinContent(i) / S1->GetBinContent(i) );
    S13->SetBinError(i,0.); 
		       
  }
  
  cout << " Y(2S) 2Srho Xsection = "  << s1 << "+/-" << TMath::Sqrt(e1) << endl;
  cout << " Y(2S) 2Srho_TrigMinus Xsection = "  << s2 << "+/-" << TMath::Sqrt(e2) << endl;
  
  S1->SetMarkerColor(3); S2->SetMarkerColor(4); 
  S1->SetLineColor(3); S2->SetLineColor(4); 
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,600); 
  S1->SetMaximum(1.4);
  S1->Draw("p");
  S2->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("XSection Vs P_{T}  For Ups(2S)");
  legge = legg->AddEntry(S1, "Y(2S) 2Srho","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "Y(2S) 2Srho_TrigMinus","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S12->SetLineColor(3); S12->SetMarkerColor(3); 
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  //S12->SetMinimum(0.);
  //S12->SetMaximum(0.02);
  S12->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("#Delta XSection Vs P_{T}  For Ups(2S)");
  legge = legg->AddEntry(S12, "Y(2S) 2Srho - 2Srho_TrigMinus","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S13->SetLineColor(3); S13->SetMarkerColor(3); 
  TCanvas *c3 = new TCanvas("c3","c3",1200,600); 
  //S12->SetMinimum(0.);
  //S12->SetMaximum(0.02);
  S13->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("Percentage #Delta XSection Vs P_{T}  For Ups(2S)");
  legge = legg->AddEntry(S13, "Y(2S) 2Srho - 2Srho_TrigMinus / Y(2S) 2Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  //TFile *f = new TFile("TrigMinus_10ptbins_2Srho_.root", "RECREATE");
  TFile *f = new TFile("TrigMinus_10ptbins_2Srho_v2.root", "RECREATE");
  S12->Write();
  
}

void XSTrigPlus_2S(){

  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_2Srho.root");
  TFile *f = new TFile("XSection2Srho_finalversion.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S2YieldPt");
  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_2Srho_TrigPlus.root");
  TFile *f = new TFile("TrigPlus_10ptbins_2Srho_fv.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S2YieldPt");
  TH1D *S12 = (TH1D*)S2->Clone();
  TH1D *S13 = (TH1D*)S2->Clone();
  
  double s1(0.), s2(0.);
  double e1(0.), e2(0.);
  for (int i = 1; i <= S1->GetNbinsX(); ++i) {
    
    s1 += S1->GetBinContent(i)*S1->GetBinWidth(i);
    e1 += (S1->GetBinError(i)*S1->GetBinWidth(i))*(S1->GetBinError(i)*S1->GetBinWidth(i));
    s2 += S2->GetBinContent(i)*S2->GetBinWidth(i);
    e2 += (S2->GetBinError(i)*S2->GetBinWidth(i))*(S2->GetBinError(i)*S2->GetBinWidth(i));
    S12->SetBinContent(i, S1->GetBinContent(i) - S2->GetBinContent(i) );
    S12->SetBinError(i,0.); 
    S13->SetBinContent(i, S12->GetBinContent(i) / S1->GetBinContent(i) );
    S13->SetBinError(i,0.); 
		       
  }
  
  cout << " Y(2S) 2Srho Xsection = "  << s1 << "+/-" << TMath::Sqrt(e1) << endl;
  cout << " Y(2S) 2Srho_TrigMinus Xsection = "  << s2 << "+/-" << TMath::Sqrt(e2) << endl;
  
  S1->SetMarkerColor(3); S2->SetMarkerColor(4); 
  S1->SetLineColor(3); S2->SetLineColor(4); 
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,600); 
  S1->SetMaximum(1.4);
  S1->Draw("p");
  S2->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("XSection Vs P_{T}  For Ups(2S)");
  legge = legg->AddEntry(S1, "Y(2S) 2Srho","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "Y(2S) 2Srho_TrigPlus","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S12->SetLineColor(3); S12->SetMarkerColor(3); 
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  //S12->SetMinimum(0.);
  //S12->SetMaximum(0.02);
  S12->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("#Delta XSection Vs P_{T}  For Ups(2S)");
  legge = legg->AddEntry(S12, "Y(2S) 2Srho - 2Srho_TrigPlus","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S13->SetLineColor(3); S13->SetMarkerColor(3); 
  TCanvas *c3 = new TCanvas("c3","c3",1200,600); 
  //S12->SetMinimum(0.);
  //S12->SetMaximum(0.02);
  S13->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("Percentage #Delta XSection Vs P_{T}  For Ups(2S)");
  legge = legg->AddEntry(S13, "Y(2S) 2Srho - 2Srho_TrigPlus / Y(2S) 2Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  //TFile *f = new TFile("TrigPlus_10ptbins_2Srho_.root", "RECREATE");
  TFile *f = new TFile("TrigPlus_10ptbins_2Srho_v2.root", "RECREATE");
  S12->Write();
  
}


void XSSigPDF_2S(){

  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_2Srho.root");
  TFile *f = new TFile("XSection2Sfinal.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S2YieldPt");
  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_2Srho_SigPDF.root");
  TFile *f = new TFile("XSection2Sfinal_sigPDF.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S2YieldPt");
  TH1D *S12 = (TH1D*)S2->Clone();
  TH1D *S13 = (TH1D*)S2->Clone();
  
  double s1(0.), s2(0.);
  double e1(0.), e2(0.);
  for (int i = 1; i <= S1->GetNbinsX(); ++i) {
    
    s1 += S1->GetBinContent(i)*S1->GetBinWidth(i);
    e1 += (S1->GetBinError(i)*S1->GetBinWidth(i))*(S1->GetBinError(i)*S1->GetBinWidth(i));
    s2 += S2->GetBinContent(i)*S2->GetBinWidth(i);
    e2 += (S2->GetBinError(i)*S2->GetBinWidth(i))*(S2->GetBinError(i)*S2->GetBinWidth(i));
    S12->SetBinContent(i, S1->GetBinContent(i) - S2->GetBinContent(i) );
    S12->SetBinError(i,0.); 
    S13->SetBinContent(i, S12->GetBinContent(i) / S1->GetBinContent(i) );
    S13->SetBinError(i,0.); 
		       
  }
  
  cout << " Y(2S) 2Srho Xsection = "  << s1 << "+/-" << TMath::Sqrt(e1) << endl;
  cout << " Y(2S) 2Srho_SigPDF Xsection = "  << s2 << "+/-" << TMath::Sqrt(e2) << endl;
  
  S1->SetMarkerColor(3); S2->SetMarkerColor(4); 
  S1->SetLineColor(3); S2->SetLineColor(4); 
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,600); 
  S1->SetMaximum(1.4);
  S1->Draw("p");
  S2->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("XSection Vs P_{T}  For Ups(2S)");
  legge = legg->AddEntry(S1, "Y(2S) 2Srho","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "Y(2S) 2Srho_SigPDF","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S12->SetLineColor(3); S12->SetMarkerColor(3); 
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  //S12->SetMinimum(0.);
  //S12->SetMaximum(0.02);
  S12->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("#Delta XSection Vs P_{T}  For Ups(2S)");
  legge = legg->AddEntry(S12, "Y(2S) 2Srho - 2Srho_SigPDF","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S13->SetLineColor(3); S13->SetMarkerColor(3); 
  TCanvas *c3 = new TCanvas("c3","c3",1200,600); 
  //S12->SetMinimum(0.);
  //S12->SetMaximum(0.02);
  S13->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("Percentage #Delta XSection Vs P_{T}  For Ups(2S)");
  legge = legg->AddEntry(S13, "Y(2S) 2Srho - 2Srho_SigPDF / Y(2S) 2Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  TFile *f = new TFile("SigPDF_10ptbins_2Srho_v2.root", "RECREATE");
  S12->Write();
  
}

void XSBgPDF_2S(){

  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_2Srho_BgPDF_ref.root");
  TFile *f = new TFile("XSection2Sfinal.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S2YieldPt");
  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_2Srho_BgPDF.root");
  TFile *f = new TFile("XSection2Sfinal_bgPDF.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S2YieldPt");
  TH1D *S12 = (TH1D*)S2->Clone();
  TH1D *S13 = (TH1D*)S2->Clone();
  
  double s1(0.), s2(0.);
  double e1(0.), e2(0.);
  for (int i = 1; i <= S1->GetNbinsX(); ++i) {
    
    s1 += S1->GetBinContent(i)*S1->GetBinWidth(i);
    e1 += (S1->GetBinError(i)*S1->GetBinWidth(i))*(S1->GetBinError(i)*S1->GetBinWidth(i));
    s2 += S2->GetBinContent(i)*S2->GetBinWidth(i);
    e2 += (S2->GetBinError(i)*S2->GetBinWidth(i))*(S2->GetBinError(i)*S2->GetBinWidth(i));
    S12->SetBinContent(i, S1->GetBinContent(i) - S2->GetBinContent(i) );
    S12->SetBinError(i,0.); 
    S13->SetBinContent(i, S12->GetBinContent(i) / S1->GetBinContent(i) );
    S13->SetBinError(i,0.); 
		       
  }
  
  cout << " Y(2S) 2Srho Xsection = "  << s1 << "+/-" << TMath::Sqrt(e1) << endl;
  cout << " Y(2S) 2Srho_BgPDF Xsection = "  << s2 << "+/-" << TMath::Sqrt(e2) << endl;
  
  S1->SetMarkerColor(3); S2->SetMarkerColor(4); 
  S1->SetLineColor(3); S2->SetLineColor(4); 
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,600); 
  S1->SetMaximum(1.4);
  S1->Draw("p");
  S2->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("XSection Vs P_{T}  For Ups(2S)");
  legge = legg->AddEntry(S1, "Y(2S) 2Srho","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "Y(2S) 2Srho_BgPDF","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S12->SetLineColor(3); S12->SetMarkerColor(3); 
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  //S12->SetMinimum(0.);
  //S12->SetMaximum(0.02);
  S12->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("#Delta XSection Vs P_{T}  For Ups(2S)");
  legge = legg->AddEntry(S12, "Y(2S) 2Srho - 2Srho_BgPDF","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S13->SetLineColor(3); S13->SetMarkerColor(3); 
  TCanvas *c3 = new TCanvas("c3","c3",1200,600); 
  //S12->SetMinimum(0.);
  //S12->SetMaximum(0.02);
  S13->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("Percentage #Delta XSection Vs P_{T}  For Ups(2S)");
  legge = legg->AddEntry(S13, "Y(2S) 2Srho - 2Srho_BgPDF / Y(2S) 2Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  TFile *f = new TFile("BgPDF_10ptbins_2Srho_v2.root", "RECREATE");
  S12->Write();
  
}

void XSSystTotNeg_2S(){

  TFile *f = new TFile("MuIDPlus_10ptbins_2Srho_.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S2YieldPt");
  TFile *f = new TFile("TrigPlus_10ptbins_2Srho_.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S2YieldPt");
  TFile *f = new TFile("SigPDF_10ptbins_2Srho_.root");
  TH1D *S3;
  S3 = (TH1D*)gFile->Get("S2YieldPt");
  TFile *f = new TFile("BgPDF_10ptbins_2Srho_.root");
  TH1D *S4;
  S4 = (TH1D*)gFile->Get("S2YieldPt");  
  TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_2Srho.root");
  TH1D *S5;
  S5 = (TH1D*)gFile->Get("S2YieldPt");    
  
  
  double s1(0.), s5(0.), s2(0.);
  for (int i = 1; i <= S1->GetNbinsX(); ++i) {
    
    s1 += S1->GetBinContent(i)*S1->GetBinContent(i);
    s1 += S2->GetBinContent(i)*S2->GetBinContent(i);
    s1 += S3->GetBinContent(i)*S3->GetBinContent(i);
    //s1 += S4->GetBinContent(i)*S4->GetBinContent(i);
    s1 += (S5->GetBinContent(i)/100)*(S5->GetBinContent(i)/100); // bg funct
    s1 += (S5->GetBinContent(i)/50)*(S5->GetBinContent(i)/50); // rho factor
    cout << " dSigma(Y(2S))/dp_{T} = "  << S5->GetBinContent(i) << " - " << TMath::Sqrt(s1) << endl;
    s5 += S5->GetBinContent(i)*S5->GetBinWidth(i);
    s2 += TMath::Sqrt(s1)*S5->GetBinWidth(i);
    s1=0;
    
  }
    
  cout << " Y(2S) 2Srho Xsection = "  << s5 << " - " << s2 << endl;
   
}

void XSSystTotPos_2S(){

  TFile *f = new TFile("MuIDMinus_10ptbins_2Srho_.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S2YieldPt");
  TFile *f = new TFile("TrigMinus_10ptbins_2Srho_.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S2YieldPt");
  TFile *f = new TFile("SigPDF_10ptbins_2Srho_.root");
  TH1D *S3;
  S3 = (TH1D*)gFile->Get("S2YieldPt");
  TFile *f = new TFile("BgPDF_10ptbins_2Srho_.root");
  TH1D *S4;
  S4 = (TH1D*)gFile->Get("S2YieldPt");  
  TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_2Srho.root");
  TH1D *S5;
  S5 = (TH1D*)gFile->Get("S2YieldPt");    
  
  
  double s1(0.), s5(0.), s2(0.);
  for (int i = 1; i <= S1->GetNbinsX(); ++i) {
    
    s1 += S1->GetBinContent(i)*S1->GetBinContent(i);
    s1 += S2->GetBinContent(i)*S2->GetBinContent(i);
    //s1 += S3->GetBinContent(i)*S3->GetBinContent(i);
    //s1 += S4->GetBinContent(i)*S4->GetBinContent(i);
    s1 += (S5->GetBinContent(i)/100)*(S5->GetBinContent(i)/100);// bg funct
    s1 += (S5->GetBinContent(i)/50)*(S5->GetBinContent(i)/50); // rho factor
    cout << " dSigma(Y(2S))/dp_{T} = "  << S5->GetBinContent(i) << " + " << TMath::Sqrt(s1) << endl;
    s5 += S5->GetBinContent(i)*S5->GetBinWidth(i);
    s2 += TMath::Sqrt(s1)*S5->GetBinWidth(i);
    s1=0;
    
  }
    
  cout << " Y(2S) 2Srho Xsection = "  << s5 << " + " << s2 << endl;
   
}

void XSNoMSC_3S(){
  
  //TFile *f = new TFile("Yield_MSC.root");
  TFile *f = new TFile("Yield_MSC_v3.root");
  TH2D *S1;
  S1 = (TH2D*)gFile->Get("fS3Yield");
  //TFile *f = new TFile("Yield_NoMSC.root");
  TFile *f = new TFile("Yield_NoMSC_v3.root");
  TH2D *S2;
  S2 = (TH2D*)gFile->Get("fS3Yield");
  //TFile *f = new TFile("XSection1Srho_finalversion.root");
  TFile *f = new TFile("XSection2Sfinal.root");
  TH1D *S3;
  S3 = (TH1D*)gFile->Get("S3YieldPt");  
  TH1D *S11 = (TH1D*)S3->Clone();
  TH1D *S22 = (TH1D*)S3->Clone();
  TH1D *S12 = (TH1D*)S3->Clone();
  TH1D *S13 = (TH1D*)S3->Clone();
  
  double s1(0.), s2(0.);
  double e1(0.), e2(0.);
  
  for (int j = 1; j <= S1->GetNbinsY(); ++j) {
    for (int i = 1; i <= S1->GetNbinsX(); ++i) {
      cout << "i = " << i << " j = " << j << endl;
      cout << "S1->GetBinContent(i,j) = " <<S1->GetBinContent(i,j) << " S2->GetBinContent(i,j) = " <<S2->GetBinContent(i,j) << endl; 
      s1 += S1->GetBinContent(i,j);
      s2 += S2->GetBinContent(i,j);
    }
    cout << "s1 = " << s1 << " s2 = " << s2 << endl;
    S11->SetBinContent(j,s1);
    S22->SetBinContent(j,s2);
    S12->SetBinContent(j,((s1-s2)/s1)*S3->GetBinContent(j));
    S13->SetBinContent(j,S12->GetBinContent(j)/S3->GetBinContent(j));
    s1=0.; s2=0.;
  }
  
  S13->Draw();
  TFile *f = new TFile("NoMSC_10ptbins_3Srho_v3.root", "RECREATE");
  S12->Write();
  
}

void XS1SRho_3S(){

  TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_3Srho.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S3YieldPt");
  TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_1Srho.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S3YieldPt");
  TH1D *S12 = (TH1D*)S2->Clone();
  TH1D *S13 = (TH1D*)S2->Clone();
  
  double s1(0.), s2(0.);
  double e1(0.), e2(0.);
  for (int i = 1; i <= S1->GetNbinsX(); ++i) {
    
    s1 += S1->GetBinContent(i)*S1->GetBinWidth(i);
    e1 += (S1->GetBinError(i)*S1->GetBinWidth(i))*(S1->GetBinError(i)*S1->GetBinWidth(i));
    s2 += S2->GetBinContent(i)*S2->GetBinWidth(i);
    e2 += (S2->GetBinError(i)*S2->GetBinWidth(i))*(S2->GetBinError(i)*S2->GetBinWidth(i));
    S12->SetBinContent(i, S1->GetBinContent(i) - S2->GetBinContent(i) );
    S12->SetBinError(i,0.); 
    S13->SetBinContent(i, S12->GetBinContent(i) / S1->GetBinContent(i) );
    S13->SetBinError(i,0.); 
		       
  }
  
  cout << " Y(3S) 3Srho Xsection = "  << s1 << "+/-" << TMath::Sqrt(e1) << endl;
  cout << " Y(3S) 1Srho Xsection = "  << s2 << "+/-" << TMath::Sqrt(e2) << endl;
  
  S1->SetMarkerColor(3); S2->SetMarkerColor(4); 
  S1->SetLineColor(3); S2->SetLineColor(4); 
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,600); 
  S1->SetMaximum(1.4);
  S1->Draw("p");
  S2->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("XSection Vs P_{T}  For Ups(3S)");
  legge = legg->AddEntry(S1, "Y(3S) 3Srho","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "Y(3S) 1Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S12->SetLineColor(3); S12->SetMarkerColor(3); 
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  //S12->SetMinimum(0.);
  //S12->SetMaximum(0.02);
  S12->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("#Delta XSection Vs P_{T}  For Ups(3S)");
  legge = legg->AddEntry(S12, "Y(3S) 3Srho - 1Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S13->SetLineColor(3); S13->SetMarkerColor(3); 
  TCanvas *c3 = new TCanvas("c3","c3",1200,600); 
  //S12->SetMinimum(0.);
  //S12->SetMaximum(0.02);
  S13->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("Percentage #Delta XSection Vs P_{T}  For Ups(3S)");
  legge = legg->AddEntry(S13, "Y(3S) 3Srho - 1Srho / Y(3S) 3Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  TFile *f = new TFile("1SRho_10ptbins_3Srho_.root", "RECREATE");
  S12->Write();
  
}

void XS2SRho_3S(){

  TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_3Srho.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S3YieldPt");
  TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_2Srho.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S3YieldPt");
  TH1D *S12 = (TH1D*)S2->Clone();
  TH1D *S13 = (TH1D*)S2->Clone();
  
  double s1(0.), s2(0.);
  double e1(0.), e2(0.);
  for (int i = 1; i <= S1->GetNbinsX(); ++i) {
    
    s1 += S1->GetBinContent(i)*S1->GetBinWidth(i);
    e1 += (S1->GetBinError(i)*S1->GetBinWidth(i))*(S1->GetBinError(i)*S1->GetBinWidth(i));
    s2 += S2->GetBinContent(i)*S2->GetBinWidth(i);
    e2 += (S2->GetBinError(i)*S2->GetBinWidth(i))*(S2->GetBinError(i)*S2->GetBinWidth(i));
    S12->SetBinContent(i, S1->GetBinContent(i) - S2->GetBinContent(i) );
    S12->SetBinError(i,0.); 
    S13->SetBinContent(i, S12->GetBinContent(i) / S1->GetBinContent(i) );
    S13->SetBinError(i,0.); 
		       
  }
  
  cout << " Y(3S) 3Srho Xsection = "  << s1 << "+/-" << TMath::Sqrt(e1) << endl;
  cout << " Y(3S) 2Srho Xsection = "  << s2 << "+/-" << TMath::Sqrt(e2) << endl;
  
  S1->SetMarkerColor(3); S2->SetMarkerColor(4); 
  S1->SetLineColor(3); S2->SetLineColor(4); 
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,600); 
  S1->SetMaximum(1.4);
  S1->Draw("p");
  S2->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("XSection Vs P_{T}  For Ups(3S)");
  legge = legg->AddEntry(S1, "Y(3S) 3Srho","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "Y(3S) 2Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S12->SetLineColor(3); S12->SetMarkerColor(3); 
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  //S12->SetMinimum(0.);
  //S12->SetMaximum(0.02);
  S12->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("#Delta XSection Vs P_{T}  For Ups(3S)");
  legge = legg->AddEntry(S12, "Y(3S) 3Srho - 2Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S13->SetLineColor(3); S13->SetMarkerColor(3); 
  TCanvas *c3 = new TCanvas("c3","c3",1200,600); 
  //S12->SetMinimum(0.);
  //S12->SetMaximum(0.02);
  S13->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("Percentage #Delta XSection Vs P_{T}  For Ups(3S)");
  legge = legg->AddEntry(S13, "Y(3S) 3Srho - 2Srho / Y(3S) 3Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  TFile *f = new TFile("2SRho_10ptbins_3Srho_.root", "RECREATE");
  S12->Write();
  
}


void XSMuIDPlus_3S(){

  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_3Srho.root");
  TFile *f = new TFile("XSection3Srho_finalversion.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S3YieldPt");
  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_3Srho_MuIDPlus.root");
  TFile *f = new TFile("MuIDPlus_10ptbins_3Srho_fv.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S3YieldPt");
  TH1D *S12 = (TH1D*)S2->Clone();
  TH1D *S13 = (TH1D*)S2->Clone();
  
  double s1(0.), s2(0.);
  double e1(0.), e2(0.);
  for (int i = 1; i <= S1->GetNbinsX(); ++i) {
    
    s1 += S1->GetBinContent(i)*S1->GetBinWidth(i);
    e1 += (S1->GetBinError(i)*S1->GetBinWidth(i))*(S1->GetBinError(i)*S1->GetBinWidth(i));
    s2 += S2->GetBinContent(i)*S2->GetBinWidth(i);
    e2 += (S2->GetBinError(i)*S2->GetBinWidth(i))*(S2->GetBinError(i)*S2->GetBinWidth(i));
    S12->SetBinContent(i, S1->GetBinContent(i) - S2->GetBinContent(i) );
    S12->SetBinError(i,0.); 
    S13->SetBinContent(i, S12->GetBinContent(i) / S1->GetBinContent(i) );
    S13->SetBinError(i,0.); 
		       
  }
  
  cout << " Y(3S) 3Srho Xsection = "  << s1 << "+/-" << TMath::Sqrt(e1) << endl;
  cout << " Y(3S) 3Srho_MuIDPlus Xsection = "  << s2 << "+/-" << TMath::Sqrt(e2) << endl;
  
  S1->SetMarkerColor(3); S2->SetMarkerColor(4); 
  S1->SetLineColor(3); S2->SetLineColor(4); 
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,600); 
  S1->SetMaximum(1.4);
  S1->Draw("p");
  S2->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("XSection Vs P_{T}  For Ups(3S)");
  legge = legg->AddEntry(S1, "Y(3S) 3Srho","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "Y(3S) 3Srho_MuIDPlus","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S12->SetLineColor(3); S12->SetMarkerColor(3); 
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  //S12->SetMinimum(0.);
  //S12->SetMaximum(0.02);
  S12->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("#Delta XSection Vs P_{T}  For Ups(3S)");
  legge = legg->AddEntry(S12, "Y(3S) 3Srho - 3Srho_MuIDPlus","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S13->SetLineColor(3); S13->SetMarkerColor(3); 
  TCanvas *c3 = new TCanvas("c3","c3",1200,600); 
  //S12->SetMinimum(0.);
  //S12->SetMaximum(0.02);
  S13->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("Percentage #Delta XSection Vs P_{T}  For Ups(3S)");
  legge = legg->AddEntry(S13, "Y(3S) 3Srho - 3Srho_MuIDPlus / Y(3S) 3Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  //TFile *f = new TFile("MuIDPlus_10ptbins_3Srho_.root", "RECREATE");
  TFile *f = new TFile("MuIDPlus_10ptbins_3Srho_v2.root", "RECREATE");
  S12->Write();
  
}

void XSMuIDMinus_3S(){

  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_3Srho.root");
  TFile *f = new TFile("XSection3Srho_finalversion.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S3YieldPt");
  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_3Srho_MuIDMinus.root");
  TFile *f = new TFile("MuIDMinus_10ptbins_3Srho_fv.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S3YieldPt");
  TH1D *S12 = (TH1D*)S2->Clone();
  TH1D *S13 = (TH1D*)S2->Clone();
  
  double s1(0.), s2(0.);
  double e1(0.), e2(0.);
  for (int i = 1; i <= S1->GetNbinsX(); ++i) {
    
    s1 += S1->GetBinContent(i)*S1->GetBinWidth(i);
    e1 += (S1->GetBinError(i)*S1->GetBinWidth(i))*(S1->GetBinError(i)*S1->GetBinWidth(i));
    s2 += S2->GetBinContent(i)*S2->GetBinWidth(i);
    e2 += (S2->GetBinError(i)*S2->GetBinWidth(i))*(S2->GetBinError(i)*S2->GetBinWidth(i));
    S12->SetBinContent(i, S1->GetBinContent(i) - S2->GetBinContent(i) );
    S12->SetBinError(i,0.); 
    S13->SetBinContent(i, S12->GetBinContent(i) / S1->GetBinContent(i) );
    S13->SetBinError(i,0.); 
		       
  }
  
  cout << " Y(3S) 3Srho Xsection = "  << s1 << "+/-" << TMath::Sqrt(e1) << endl;
  cout << " Y(3S) 3Srho_MuIDMinus Xsection = "  << s2 << "+/-" << TMath::Sqrt(e2) << endl;
  
  S1->SetMarkerColor(3); S2->SetMarkerColor(4); 
  S1->SetLineColor(3); S2->SetLineColor(4); 
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,600); 
  S1->SetMaximum(1.4);
  S1->Draw("p");
  S2->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("XSection Vs P_{T}  For Ups(3S)");
  legge = legg->AddEntry(S1, "Y(3S) 3Srho","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "Y(3S) 3Srho_MuIDMinus","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S12->SetLineColor(3); S12->SetMarkerColor(3); 
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  //S12->SetMinimum(0.);
  //S12->SetMaximum(0.02);
  S12->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("#Delta XSection Vs P_{T}  For Ups(3S)");
  legge = legg->AddEntry(S12, "Y(3S) 3Srho - 3Srho_MuIDMinus","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S13->SetLineColor(3); S13->SetMarkerColor(3); 
  TCanvas *c3 = new TCanvas("c3","c3",1200,600); 
  //S12->SetMinimum(0.);
  //S12->SetMaximum(0.02);
  S13->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("Percentage #Delta XSection Vs P_{T}  For Ups(3S)");
  legge = legg->AddEntry(S13, "Y(3S) 3Srho - 3Srho_MuIDMinus / Y(3S) 3Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  //TFile *f = new TFile("MuIDMinus_10ptbins_3Srho_.root", "RECREATE");
  TFile *f = new TFile("MuIDMinus_10ptbins_3Srho_v2.root", "RECREATE");
  S12->Write();
  
}

void XSTrigMinus_3S(){

  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_3Srho.root");
  TFile *f = new TFile("XSection3Srho_finalversion.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S3YieldPt");
  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_3Srho_TrigMinus.root");
  TFile *f = new TFile("TrigMinus_10ptbins_3Srho_fv.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S3YieldPt");
  TH1D *S12 = (TH1D*)S2->Clone();
  TH1D *S13 = (TH1D*)S2->Clone();
  
  double s1(0.), s2(0.);
  double e1(0.), e2(0.);
  for (int i = 1; i <= S1->GetNbinsX(); ++i) {
    
    s1 += S1->GetBinContent(i)*S1->GetBinWidth(i);
    e1 += (S1->GetBinError(i)*S1->GetBinWidth(i))*(S1->GetBinError(i)*S1->GetBinWidth(i));
    s2 += S2->GetBinContent(i)*S2->GetBinWidth(i);
    e2 += (S2->GetBinError(i)*S2->GetBinWidth(i))*(S2->GetBinError(i)*S2->GetBinWidth(i));
    S12->SetBinContent(i, S1->GetBinContent(i) - S2->GetBinContent(i) );
    S12->SetBinError(i,0.); 
    S13->SetBinContent(i, S12->GetBinContent(i) / S1->GetBinContent(i) );
    S13->SetBinError(i,0.); 
		       
  }
  
  cout << " Y(3S) 3Srho Xsection = "  << s1 << "+/-" << TMath::Sqrt(e1) << endl;
  cout << " Y(3S) 3Srho_TrigMinus Xsection = "  << s2 << "+/-" << TMath::Sqrt(e2) << endl;
  
  S1->SetMarkerColor(3); S2->SetMarkerColor(4); 
  S1->SetLineColor(3); S2->SetLineColor(4); 
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,600); 
  S1->SetMaximum(1.4);
  S1->Draw("p");
  S2->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("XSection Vs P_{T}  For Ups(3S)");
  legge = legg->AddEntry(S1, "Y(3S) 3Srho","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "Y(3S) 3Srho_TrigMinus","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S12->SetLineColor(3); S12->SetMarkerColor(3); 
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  //S12->SetMinimum(0.);
  //S12->SetMaximum(0.02);
  S12->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("#Delta XSection Vs P_{T}  For Ups(3S)");
  legge = legg->AddEntry(S12, "Y(3S) 3Srho - 3Srho_TrigMinus","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S13->SetLineColor(3); S13->SetMarkerColor(3); 
  TCanvas *c3 = new TCanvas("c3","c3",1200,600); 
  //S12->SetMinimum(0.);
  //S12->SetMaximum(0.02);
  S13->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("Percentage #Delta XSection Vs P_{T}  For Ups(3S)");
  legge = legg->AddEntry(S13, "Y(3S) 3Srho - 3Srho_TrigMinus / Y(3S) 3Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  //TFile *f = new TFile("TrigMinus_10ptbins_3Srho_.root", "RECREATE");
  TFile *f = new TFile("TrigMinus_10ptbins_3Srho_v2.root", "RECREATE");
  S12->Write();
  
}

void XSTrigPlus_3S(){

  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_3Srho.root");
  TFile *f = new TFile("XSection3Srho_finalversion.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S3YieldPt");
  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_3Srho_TrigPlus.root");
  TFile *f = new TFile("TrigPlus_10ptbins_3Srho_fv.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S3YieldPt");
  TH1D *S12 = (TH1D*)S2->Clone();
  TH1D *S13 = (TH1D*)S2->Clone();
  
  double s1(0.), s2(0.);
  double e1(0.), e2(0.);
  for (int i = 1; i <= S1->GetNbinsX(); ++i) {
    
    s1 += S1->GetBinContent(i)*S1->GetBinWidth(i);
    e1 += (S1->GetBinError(i)*S1->GetBinWidth(i))*(S1->GetBinError(i)*S1->GetBinWidth(i));
    s2 += S2->GetBinContent(i)*S2->GetBinWidth(i);
    e2 += (S2->GetBinError(i)*S2->GetBinWidth(i))*(S2->GetBinError(i)*S2->GetBinWidth(i));
    S12->SetBinContent(i, S1->GetBinContent(i) - S2->GetBinContent(i) );
    S12->SetBinError(i,0.); 
    S13->SetBinContent(i, S12->GetBinContent(i) / S1->GetBinContent(i) );
    S13->SetBinError(i,0.); 
		       
  }
  
  cout << " Y(S) 3Srho Xsection = "  << s1 << "+/-" << TMath::Sqrt(e1) << endl;
  cout << " Y(3S) 3Srho_TrigMinus Xsection = "  << s2 << "+/-" << TMath::Sqrt(e2) << endl;
  
  S1->SetMarkerColor(3); S2->SetMarkerColor(4); 
  S1->SetLineColor(3); S2->SetLineColor(4); 
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,600); 
  S1->SetMaximum(1.4);
  S1->Draw("p");
  S2->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("XSection Vs P_{T}  For Ups(3S)");
  legge = legg->AddEntry(S1, "Y(3S) 3Srho","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "Y(3S) 3Srho_TrigPlus","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S12->SetLineColor(3); S12->SetMarkerColor(3); 
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  //S12->SetMinimum(0.);
  //S12->SetMaximum(0.02);
  S12->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("#Delta XSection Vs P_{T}  For Ups(3S)");
  legge = legg->AddEntry(S12, "Y(3S) 3Srho - 3Srho_TrigPlus","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S13->SetLineColor(3); S13->SetMarkerColor(3); 
  TCanvas *c3 = new TCanvas("c3","c3",1200,600); 
  //S12->SetMinimum(0.);
  //S12->SetMaximum(0.02);
  S13->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("Percentage #Delta XSection Vs P_{T}  For Ups(3S)");
  legge = legg->AddEntry(S13, "Y(3S) 3Srho - 3Srho_TrigPlus / Y(3S) 3Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  //TFile *f = new TFile("TrigPlus_10ptbins_3Srho_.root", "RECREATE");
  TFile *f = new TFile("TrigPlus_10ptbins_3Srho_v2.root", "RECREATE");
  S12->Write();
  
}


void XSSigPDF_3S(){

  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_3Srho.root");
  TFile *f = new TFile("XSection3Sfinal.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S3YieldPt");
  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_3Srho_SigPDF.root");
  TFile *f = new TFile("XSection3Sfinal_sigPDF.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S3YieldPt");
  TH1D *S12 = (TH1D*)S2->Clone();
  TH1D *S13 = (TH1D*)S2->Clone();
  
  double s1(0.), s2(0.);
  double e1(0.), e2(0.);
  for (int i = 1; i <= S1->GetNbinsX(); ++i) {
    
    s1 += S1->GetBinContent(i)*S1->GetBinWidth(i);
    e1 += (S1->GetBinError(i)*S1->GetBinWidth(i))*(S1->GetBinError(i)*S1->GetBinWidth(i));
    s2 += S2->GetBinContent(i)*S2->GetBinWidth(i);
    e2 += (S2->GetBinError(i)*S2->GetBinWidth(i))*(S2->GetBinError(i)*S2->GetBinWidth(i));
    S12->SetBinContent(i, S1->GetBinContent(i) - S2->GetBinContent(i) );
    S12->SetBinError(i,0.); 
    S13->SetBinContent(i, S12->GetBinContent(i) / S1->GetBinContent(i) );
    S13->SetBinError(i,0.); 
		       
  }
  
  cout << " Y(3S) 3Srho Xsection = "  << s1 << "+/-" << TMath::Sqrt(e1) << endl;
  cout << " Y(3S) 3Srho_SigPDF Xsection = "  << s2 << "+/-" << TMath::Sqrt(e2) << endl;
  
  S1->SetMarkerColor(3); S2->SetMarkerColor(4); 
  S1->SetLineColor(3); S2->SetLineColor(4); 
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,600); 
  S1->SetMaximum(1.4);
  S1->Draw("p");
  S2->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("XSection Vs P_{T}  For Ups(3S)");
  legge = legg->AddEntry(S1, "Y(3S) 3Srho","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "Y(3S) 3Srho_SigPDF","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S12->SetLineColor(3); S12->SetMarkerColor(3); 
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  //S12->SetMinimum(0.);
  //S12->SetMaximum(0.02);
  S12->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("#Delta XSection Vs P_{T}  For Ups(3S)");
  legge = legg->AddEntry(S12, "Y(3S) 3Srho - 3Srho_SigPDF","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S13->SetLineColor(3); S13->SetMarkerColor(3); 
  TCanvas *c3 = new TCanvas("c3","c3",1200,600); 
  //S12->SetMinimum(0.);
  //S12->SetMaximum(0.02);
  S13->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("Percentage #Delta XSection Vs P_{T}  For Ups(3S)");
  legge = legg->AddEntry(S13, "Y(3S) 3Srho - 3Srho_SigPDF / Y(3S) 3Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  TFile *f = new TFile("SigPDF_10ptbins_3Srho_v2.root", "RECREATE");
  S12->Write();
  
}

void XSBgPDF_3S(){

  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_3Srho_BgPDF_ref.root");
  TFile *f = new TFile("XSection3Sfinal.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S3YieldPt");
  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_3Srho_BgPDF.root");
  TFile *f = new TFile("XSection3Sfinal_bgPDF.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S3YieldPt");
  TH1D *S12 = (TH1D*)S2->Clone();
  TH1D *S13 = (TH1D*)S2->Clone();
  
  double s1(0.), s2(0.);
  double e1(0.), e2(0.);
  for (int i = 1; i <= S1->GetNbinsX(); ++i) {
    
    s1 += S1->GetBinContent(i)*S1->GetBinWidth(i);
    e1 += (S1->GetBinError(i)*S1->GetBinWidth(i))*(S1->GetBinError(i)*S1->GetBinWidth(i));
    s2 += S2->GetBinContent(i)*S2->GetBinWidth(i);
    e2 += (S2->GetBinError(i)*S2->GetBinWidth(i))*(S2->GetBinError(i)*S2->GetBinWidth(i));
    S12->SetBinContent(i, S1->GetBinContent(i) - S2->GetBinContent(i) );
    S12->SetBinError(i,0.); 
    S13->SetBinContent(i, S12->GetBinContent(i) / S1->GetBinContent(i) );
    S13->SetBinError(i,0.); 
		       
  }
  
  cout << " Y(3S) 3Srho Xsection = "  << s1 << "+/-" << TMath::Sqrt(e1) << endl;
  cout << " Y(3S) 3Srho_BgPDF Xsection = "  << s2 << "+/-" << TMath::Sqrt(e2) << endl;
  
  S1->SetMarkerColor(3); S2->SetMarkerColor(4); 
  S1->SetLineColor(3); S2->SetLineColor(4); 
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,600); 
  S1->SetMaximum(1.4);
  S1->Draw("p");
  S2->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("XSection Vs P_{T}  For Ups(3S)");
  legge = legg->AddEntry(S1, "Y(3S) 3Srho","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "Y(3S) 3Srho_BgPDF","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S12->SetLineColor(3); S12->SetMarkerColor(3); 
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  //S12->SetMinimum(0.);
  //S12->SetMaximum(0.02);
  S12->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("#Delta XSection Vs P_{T}  For Ups(3S)");
  legge = legg->AddEntry(S12, "Y(3S) 3Srho - 3Srho_BgPDF","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S13->SetLineColor(3); S13->SetMarkerColor(3); 
  TCanvas *c3 = new TCanvas("c3","c3",1200,600); 
  //S12->SetMinimum(0.);
  //S12->SetMaximum(0.02);
  S13->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("Percentage #Delta XSection Vs P_{T}  For Ups(3S)");
  legge = legg->AddEntry(S13, "Y(3S) 3Srho - 3Srho_BgPDF / Y(3S) 3Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  TFile *f = new TFile("BgPDF_10ptbins_3Srho_v2.root", "RECREATE");
  S12->Write();
  
}

void XSSystTotNeg_3S(){

  TFile *f = new TFile("MuIDPlus_10ptbins_3Srho_.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S3YieldPt");
  TFile *f = new TFile("TrigPlus_10ptbins_3Srho_.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S3YieldPt");
  TFile *f = new TFile("SigPDF_10ptbins_3Srho_.root");
  TH1D *S3;
  S3 = (TH1D*)gFile->Get("S3YieldPt");
  TFile *f = new TFile("BgPDF_10ptbins_3Srho_.root");
  TH1D *S4;
  S4 = (TH1D*)gFile->Get("S3YieldPt");  
  TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_3Srho.root");
  TH1D *S5;
  S5 = (TH1D*)gFile->Get("S3YieldPt");    
  
  
  double s1(0.), s5(0.), s2(0.);
  for (int i = 1; i <= S1->GetNbinsX(); ++i) {
    
    s1 += S1->GetBinContent(i)*S1->GetBinContent(i);
    s1 += S2->GetBinContent(i)*S2->GetBinContent(i);
    s1 += S3->GetBinContent(i)*S3->GetBinContent(i);
    //s1 += S4->GetBinContent(i)*S4->GetBinContent(i);
    s1 += (S5->GetBinContent(i)/100)*(S5->GetBinContent(i)/100); // bg funct
    s1 += (S5->GetBinContent(i)/50)*(S5->GetBinContent(i)/50); // rho factor
    cout << " dSigma(Y(3S))/dp_{T} = "  << S5->GetBinContent(i) << " - " << TMath::Sqrt(s1) << endl;
    s5 += S5->GetBinContent(i)*S5->GetBinWidth(i);
    s2 += TMath::Sqrt(s1)*S5->GetBinWidth(i);
    s1=0;
    
  }
    
  cout << " Y(3S) 3Srho Xsection = "  << s5 << " - " << s2 << endl;
   
}

void XSSystTotPos_3S(){

  TFile *f = new TFile("MuIDMinus_10ptbins_3Srho_.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S3YieldPt");
  TFile *f = new TFile("TrigMinus_10ptbins_3Srho_.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S3YieldPt");
  TFile *f = new TFile("SigPDF_10ptbins_3Srho_.root");
  TH1D *S3;
  S3 = (TH1D*)gFile->Get("S3YieldPt");
  TFile *f = new TFile("BgPDF_10ptbins_3Srho_.root");
  TH1D *S4;
  S4 = (TH1D*)gFile->Get("S3YieldPt");  
  TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_3Srho.root");
  TH1D *S5;
  S5 = (TH1D*)gFile->Get("S3YieldPt");    
  
  
  double s1(0.), s5(0.), s2(0.);
  for (int i = 1; i <= S1->GetNbinsX(); ++i) {
    
    s1 += S1->GetBinContent(i)*S1->GetBinContent(i);
    s1 += S2->GetBinContent(i)*S2->GetBinContent(i);
    //s1 += S3->GetBinContent(i)*S3->GetBinContent(i);
    //s1 += S4->GetBinContent(i)*S4->GetBinContent(i);
    s1 += (S5->GetBinContent(i)/100)*(S5->GetBinContent(i)/100);// bg funct
    s1 += (S5->GetBinContent(i)/50)*(S5->GetBinContent(i)/50); // rho factor
    cout << " dSigma(Y(3S))/dp_{T} = "  << S5->GetBinContent(i) << " + " << TMath::Sqrt(s1) << endl;
    s5 += S5->GetBinContent(i)*S5->GetBinWidth(i);
    s2 += TMath::Sqrt(s1)*S5->GetBinWidth(i);
    s1=0;
    
  }
    
  cout << " Y(3S) 3Srho Xsection = "  << s5 << " + " << s2 << endl;
   
}


void ClosureTest1S(){

  TFile *f = new TFile("101201.fl10.mm.ups1s.xsReader_1S.10ptbins_ClosureTest.root");
  TH2D *S1;
  S1 = (TH2D*)gFile->Get("UpsilonYield");
  TFile *f = new TFile("ClosureTest_10ptbins.root");
  TH2D *S2;
  S2 = (TH2D*)gFile->Get("fS1Yield");
  TH2D *S3 = (TH2D*)S2->Clone();
  TH2D *S4 = (TH2D*)S2->Clone();
  TH2D *S5 = (TH2D*)S2->Clone();
  TH1D *S6;
  S6 = new TH1D("s","s", 50, -0.2,0.2);
  
  double s1(0.), s2(0.), s3(0.);
  double e2(0.), e3(0.); 
  for (int i = 1; i <= S1->GetNbinsX(); ++i) {
    for (int j = 1; j <= S1->GetNbinsY(); ++j) {
    
      s1 = S1->GetCellContent(i,j);
      s2 = S2->GetCellContent(i,j);
      s3 = (s2 - s1)/s1;
      S3->SetCellContent(i,j,s3);
      e2 = S2->GetCellError(i,j);
      e3 = e2/s1;
      S4->SetCellContent(i,j,e3);
      S5->SetCellContent(i,j,s3/e3);
      S6->Fill(s3);
       
    }
  }
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,600); 
  c1->Divide(3,1);
  c1->cd(1);
  S1->Draw("colz");
  c1->cd(2);
  S2->Draw("colz");
  c1->cd(3);
  //S3->SetMinimum(-0.03);
  //S3->SetMaximum(0.3);
  S3->Draw("colz");
  TCanvas *c2 = new TCanvas("c2","c2",1200,600);
  //c2->Divide(3,1);
  //c2->cd(1);
  //S3->SetMinimum(-0.10);
  //S3->Draw("colz");
  //c2->cd(2);
  //S4->SetMaximum(0.07);
  //S4->Draw("colz");
  //c2->cd(3);
  //S5->SetMaximum(4.); 
  //S5->SetMinimum(-4.);
  //S5->Draw("colz");
  S6->Draw(); 
}

void ClosureTest2S(){

  TFile *f = new TFile("101201.fl10.mm.ups2s.xsReader_2S.10ptbins_ClosureTest.root");
  TH2D *S1;
  S1 = (TH2D*)gFile->Get("UpsilonYield");
  TFile *f = new TFile("ClosureTest_10ptbins.root");
  TH2D *S2;
  S2 = (TH2D*)gFile->Get("fS2Yield");
  TH2D *S3 = (TH2D*)S2->Clone();
  TH2D *S4 = (TH2D*)S2->Clone();
  TH2D *S5 = (TH2D*)S2->Clone();
  TH1D *S6;
  S6 = new TH1D("s","s", 50, -0.2,0.2);
  
  double s1(0.), s2(0.), s3(0.);
  double e2(0.), e3(0.); 
  for (int i = 1; i <= S1->GetNbinsX(); ++i) {
    for (int j = 1; j <= S1->GetNbinsY(); ++j) {
    
      s1 = S1->GetCellContent(i,j);
      s2 = S2->GetCellContent(i,j);
      s3 = ((s2*1.17) - s1)/s1;
      S3->SetCellContent(i,j,s3);
      e2 = S2->GetCellError(i,j);
      e3 = e2/s1;
      S4->SetCellContent(i,j,e3);
      S5->SetCellContent(i,j,s3/e3);
      S6->Fill(s3);
      
    }
  }
  
  TCanvas *c3 = new TCanvas("c3","c3",1200,600); 
  c3->Divide(3,1);
  c3->cd(1);
  S1->Draw("colz");
  c3->cd(2);
  S2->Draw("colz");
  c3->cd(3);
  //S3->SetMinimum(-0.03);
  S3->SetMaximum(0.10);
  S3->Draw("colz");
  TCanvas *c4 = new TCanvas("c4","c4",1200,600);
  //c4->Divide(3,1);
  //c4->cd(1);
  //S3->Draw("colz");
  //c4->cd(2);
  //S4->Draw("colz");
  //c4->cd(3);
  //S5->SetMaximum(4.); 
  //S5->SetMinimum(-4.); 
  //S5->Draw("colz"); 
  S6->Draw(); 
  
  
}
  
