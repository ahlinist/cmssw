#include "TMath.h"
#include <math.h>
#include "TF1.h"
#include "TH1.h"

void XS2SRho_1S(){

  TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_1Srho.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S1YieldEta");
  TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_2Srho.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S1YieldEta");
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
  S1->SetMaximum(5.);
  S1->Draw("p");
  S2->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("XSection Vs |y|  For Ups(1S)");
  legge = legg->AddEntry(S1, "Y(1S) 1Srho","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "Y(1S) 2Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S12->SetLineColor(3); S12->SetMarkerColor(3); 
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  S12->SetMinimum(-0.2);
  S12->SetMaximum(0.2);
  S12->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("#Delta XSection Vs |y|  For Ups(1S)");
  legge = legg->AddEntry(S12, "Y(1S) 1Srho - 2Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S13->SetLineColor(3); S13->SetMarkerColor(3); 
  TCanvas *c3 = new TCanvas("c3","c3",1200,600); 
  S13->SetMinimum(-0.2);
  S13->SetMaximum(0.2);
  S13->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("Percentage #Delta XSection Vs |y|  For Ups(1S)");
  legge = legg->AddEntry(S13, "Y(1S) 1Srho - 2Srho / Y(1S) 1Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  TFile *f = new TFile("2SRho_10ptbins_1Srho_rap.root", "RECREATE");
  S12->Write();
  
}



void XS3SRho_1S(){

  TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_1Srho.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S1YieldEta");
  TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_3Srho.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S1YieldEta");
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
  S1->SetMaximum(5.);
  S1->Draw("p");
  S2->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("XSection Vs |y| For Ups(1S)");
  legge = legg->AddEntry(S1, "Y(1S) 1Srho","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "Y(1S) 3Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S12->SetLineColor(3); S12->SetMarkerColor(3); 
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  S12->SetMinimum(-0.02);
  S12->SetMaximum(0.02);
  S12->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("#Delta XSection Vs |y| For Ups(1S)");
  legge = legg->AddEntry(S12, "Y(1S) 1Srho - 3Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S13->SetLineColor(3); S13->SetMarkerColor(3); 
  TCanvas *c3 = new TCanvas("c3","c3",1200,600); 
  S13->SetMinimum(-0.02);
  S13->SetMaximum(0.02);
  S13->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("Percentage #Delta XSection Vs |y| For Ups(1S)");
  legge = legg->AddEntry(S13, "Y(1S) 1Srho - 3Srho / Y(1S) 1Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  TFile *f = new TFile("3SRho_10ptbins_1Srho_rap.root", "RECREATE");
  S12->Write();
  
}



void XSMuIDPlus_1S(){

  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_1Srho.root");
  TFile *f = new TFile("XSection1Srho_finalversion.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S1YieldEta");
  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_1Srho_MuIDPlus.root");
  TFile *f = new TFile("MuIDPlus_10ptbins_1Srho_fv.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S1YieldEta");
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
  S1->SetMaximum(5.);
  S1->Draw("p");
  S2->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("XSection Vs |y| For Ups(1S)");
  legge = legg->AddEntry(S1, "Y(1S) 1Srho","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "Y(1S) 1Srho_MuIDPlus","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S12->SetLineColor(3); S12->SetMarkerColor(3); 
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  S12->SetMinimum(0.);
  S12->SetMaximum(0.2);
  S12->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("#Delta XSection Vs |y| For Ups(1S)");
  legge = legg->AddEntry(S12, "Y(1S) 1Srho - 1Srho_MuIDPlus","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S13->SetLineColor(3); S13->SetMarkerColor(3); 
  TCanvas *c3 = new TCanvas("c3","c3",1200,600); 
  S13->SetMinimum(0.);
  S13->SetMaximum(0.05);
  S13->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("Percentage #Delta XSection Vs |y|  For Ups(1S)");
  legge = legg->AddEntry(S13, "Y(1S) 1Srho - 1Srho_MuIDPlus / Y(1S) 1Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  //TFile *f = new TFile("MuIDPlus_10ptbins_1Srho_rap.root", "RECREATE");
  TFile *f = new TFile("MuIDPlus_10ptbins_1Srho_rap_v2.root", "RECREATE");
  S12->Write();
  
}

void XSMuIDMinus_1S(){

  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_1Srho.root");
  TFile *f = new TFile("XSection1Srho_finalversion.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S1YieldEta");
  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_1Srho_MuIDMinus.root");
  TFile *f = new TFile("MuIDMinus_10ptbins_1Srho_fv.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S1YieldEta");
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
  S1->SetMaximum(5.);
  S1->Draw("p");
  S2->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("XSection Vs |y| For Ups(1S)");
  legge = legg->AddEntry(S1, "Y(1S) 1Srho","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "Y(1S) 1Srho_MuIDMinus","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S12->SetLineColor(3); S12->SetMarkerColor(3); 
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  S12->SetMinimum(-0.2);
  S12->SetMaximum(0.);
  S12->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("#Delta XSection Vs |y| For Ups(1S)");
  legge = legg->AddEntry(S12, "Y(1S) 1Srho - 1Srho_MuIDMinus","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S13->SetLineColor(3); S13->SetMarkerColor(3); 
  TCanvas *c3 = new TCanvas("c3","c3",1200,600); 
  S13->SetMinimum(-0.1);
  S13->SetMaximum(0.);
  S13->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("Percentage #Delta XSection Vs |y| For Ups(1S)");
  legge = legg->AddEntry(S13, "Y(1S) 1Srho - 1Srho_MuIDMinus / Y(1S) 1Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  //TFile *f = new TFile("MuIDMinus_10ptbins_1Srho_rap.root", "RECREATE");
  TFile *f = new TFile("MuIDMinus_10ptbins_1Srho_rap_v2.root", "RECREATE");
  S12->Write();
  
}


void XSTrigMinus_1S(){

  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_1Srho.root");
  TFile *f = new TFile("XSection1Srho_finalversion.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S1YieldEta");
  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_1Srho_TrigMinus.root");
  TFile *f = new TFile("TrigMinus_10ptbins_1Srho_fv.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S1YieldEta");
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
  S1->SetMaximum(5.);
  S1->Draw("p");
  S2->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("XSection Vs |y| For Ups(1S)");
  legge = legg->AddEntry(S1, "Y(1S) 1Srho","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "Y(1S) 1Srho_TrigMinus","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S12->SetLineColor(3); S12->SetMarkerColor(3); 
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  S12->SetMinimum(-0.5);
  S12->SetMaximum(0.);
  S12->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("#Delta XSection Vs |y| For Ups(1S)");
  legge = legg->AddEntry(S12, "Y(1S) 1Srho - 1Srho_TrigMinus","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S13->SetLineColor(3); S13->SetMarkerColor(3); 
  TCanvas *c3 = new TCanvas("c3","c3",1200,600); 
  S13->SetMinimum(-0.2);
  S13->SetMaximum(0.);
  S13->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("Percentage #Delta XSection Vs |y| For Ups(1S)");
  legge = legg->AddEntry(S13, "Y(1S) 1Srho - 1Srho_TrigMinus / Y(1S) 1Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  //TFile *f = new TFile("TrigMinus_10ptbins_1Srho_rap.root", "RECREATE");
  TFile *f = new TFile("TrigMinus_10ptbins_1Srho_rap_v2.root", "RECREATE");
  S12->Write();
  
}

void XSTrigPlus_1S(){

  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_1Srho.root");
  TFile *f = new TFile("XSection1Srho_finalversion.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S1YieldEta");
  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_1Srho_TrigPlus.root");
  TFile *f = new TFile("TrigPlus_10ptbins_1Srho_fv.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S1YieldEta");
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
  S1->SetMaximum(5.);
  S1->Draw("p");
  S2->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("XSection Vs |y|  For Ups(1S)");
  legge = legg->AddEntry(S1, "Y(1S) 1Srho","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "Y(1S) 1Srho_TrigPlus","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S12->SetLineColor(3); S12->SetMarkerColor(3); 
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  S12->SetMinimum(0.);
  S12->SetMaximum(0.4);
  S12->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("#Delta XSection Vs |y| For Ups(1S)");
  legge = legg->AddEntry(S12, "Y(1S) 1Srho - 1Srho_TrigPlus","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S13->SetLineColor(3); S13->SetMarkerColor(3); 
  TCanvas *c3 = new TCanvas("c3","c3",1200,600); 
  S13->SetMinimum(0.);
  S13->SetMaximum(0.1);
  S13->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("Percentage #Delta XSection Vs |y| For Ups(1S)");
  legge = legg->AddEntry(S13, "Y(1S) 1Srho - 1Srho_TrigPlus / Y(1S) 1Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  //TFile *f = new TFile("TrigPlus_10ptbins_1Srho_rap.root", "RECREATE");
  TFile *f = new TFile("TrigPlus_10ptbins_1Srho_rap_v2.root", "RECREATE");
  S12->Write();
  
}



void XSSigPDF_1S(){

  TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_1Srho.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S1YieldEta");
  TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_1Srho_SigPDF.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S1YieldEta");
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
  S1->SetMaximum(5.);
  S1->Draw("p");
  S2->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("XSection Vs |y|  For Ups(1S)");
  legge = legg->AddEntry(S1, "Y(1S) 1Srho","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "Y(1S) 1Srho_SigPDF","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S12->SetLineColor(3); S12->SetMarkerColor(3); 
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  S12->SetMinimum(0.);
  S12->SetMaximum(0.2);
  S12->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("#Delta XSection Vs |y| For Ups(1S)");
  legge = legg->AddEntry(S12, "Y(1S) 1Srho - 1Srho_SigPDF","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S13->SetLineColor(3); S13->SetMarkerColor(3); 
  TCanvas *c3 = new TCanvas("c3","c3",1200,600); 
  S13->SetMinimum(0.);
  S13->SetMaximum(0.1);
  S13->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("Percentage #Delta XSection Vs |y| For Ups(1S)");
  legge = legg->AddEntry(S13, "Y(1S) 1Srho - 1Srho_SigPDF / Y(1S) 1Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  TFile *f = new TFile("SigPDF_10ptbins_1Srho_rap.root", "RECREATE");
  S12->Write();
  
}



void XSBgPDF_1S(){

  TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_1Srho_BgPDF_ref.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S1YieldEta");
  TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_1Srho_BgPDF.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S1YieldEta");
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
  S1->SetMaximum(5.);
  S1->Draw("p");
  S2->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("XSection Vs |y| For Ups(1S)");
  legge = legg->AddEntry(S1, "Y(1S) 1Srho","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "Y(1S) 1Srho_BgPDF","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S12->SetLineColor(3); S12->SetMarkerColor(3); 
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  S12->SetMinimum(-0.02);
  S12->SetMaximum(0.02);
  S12->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("#Delta XSection Vs |y| For Ups(1S)");
  legge = legg->AddEntry(S12, "Y(1S) 1Srho - 1Srho_BgPDF","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S13->SetLineColor(3); S13->SetMarkerColor(3); 
  TCanvas *c3 = new TCanvas("c3","c3",1200,600); 
  S13->SetMinimum(-0.02);
  S13->SetMaximum(0.02);
  S13->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("Percentage #Delta XSection Vs |y| For Ups(1S)");
  legge = legg->AddEntry(S13, "Y(1S) 1Srho - 1Srho_BgPDF / Y(1S) 1Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  TFile *f = new TFile("BgPDF_10ptbins_1Srho_rap.root", "RECREATE");
  S12->Write();
  
}

void XS1SRho_2S(){

  TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_2Srho.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S2YieldEta");
  TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_1Srho.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S2YieldEta");
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
  S1->SetMaximum(1.5);
  S1->Draw("p");
  S2->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("XSection Vs |y| For Ups(2S)");
  legge = legg->AddEntry(S1, "Y(2S) 2Srho","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "Y(2S) 1Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S12->SetLineColor(3); S12->SetMarkerColor(3); 
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  S12->SetMinimum(-0.02);
  S12->SetMaximum(0.02);
  S12->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("#Delta XSection Vs |y| For Ups(2S)");
  legge = legg->AddEntry(S12, "Y(2S) 2Srho - 1Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S13->SetLineColor(3); S13->SetMarkerColor(3); 
  TCanvas *c3 = new TCanvas("c3","c3",1200,600); 
  S13->SetMinimum(-0.02);
  S13->SetMaximum(0.02);
  S13->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("Percentage #Delta XSection Vs |y| For Ups(2S)");
  legge = legg->AddEntry(S13, "Y(2S) 2Srho - 1Srho / Y(2S) 2Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  TFile *f = new TFile("1SRho_10ptbins_2Srho_rap.root", "RECREATE");
  S12->Write();
  
}

void XS3SRho_2S(){

  TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_2Srho.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S2YieldEta");
  TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_3Srho.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S2YieldEta");
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
  S1->SetMaximum(1.5);
  S1->Draw("p");
  S2->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("XSection Vs |y| For Ups(2S)");
  legge = legg->AddEntry(S1, "Y(2S) 2Srho","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "Y(2S) 3Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S12->SetLineColor(3); S12->SetMarkerColor(3); 
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  S12->SetMinimum(-0.02);
  S12->SetMaximum(0.02);
  S12->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("#Delta XSection Vs |y| For Ups(2S)");
  legge = legg->AddEntry(S12, "Y(2S) 2Srho - 3Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S13->SetLineColor(3); S13->SetMarkerColor(3); 
  TCanvas *c3 = new TCanvas("c3","c3",1200,600); 
  S13->SetMinimum(-0.02);
  S13->SetMaximum(0.02);
  S13->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("Percentage #Delta XSection Vs |y| For Ups(2S)");
  legge = legg->AddEntry(S13, "Y(2S) 2Srho - 3Srho / Y(2S) 2Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  TFile *f = new TFile("3SRho_10ptbins_2Srho_rap.root", "RECREATE");
  S12->Write();
  
}

void XSMuIDPlus_2S(){

  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_2Srho.root");
  TFile *f = new TFile("XSection2Srho_finalversion.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S2YieldEta");
  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_2Srho_MuIDPlus.root");
  TFile *f = new TFile("MuIDPlus_10ptbins_2Srho_fv.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S2YieldEta");
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
  S1->SetMaximum(1.5);
  S1->Draw("p");
  S2->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("XSection Vs P_{T}  For Ups(2S)");
  legge = legg->AddEntry(S1, "Y(2S) 2Srho","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "Y(2S) 2Srho_MuIDPlus","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S12->SetLineColor(3); S12->SetMarkerColor(3); 
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  S12->SetMinimum(0.);
  S12->SetMaximum(0.05);
  S12->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("#Delta XSection Vs |y| For Ups(2S)");
  legge = legg->AddEntry(S12, "Y(2S) 2Srho - 2Srho_MuIDPlus","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S13->SetLineColor(3); S13->SetMarkerColor(3); 
  TCanvas *c3 = new TCanvas("c3","c3",1200,600); 
  S13->SetMinimum(0.);
  S13->SetMaximum(0.05);
  S13->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("Percentage #Delta XSection Vs |y| For Ups(2S)");
  legge = legg->AddEntry(S13, "Y(2S) 2Srho - 2Srho_MuIDPlus / Y(2S) 2Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  //TFile *f = new TFile("MuIDPlus_10ptbins_2Srho_rap.root", "RECREATE");
  TFile *f = new TFile("MuIDPlus_10ptbins_2Srho_rap_v2.root", "RECREATE");
  S12->Write();
  
}

void XSMuIDMinus_2S(){

  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_2Srho.root");
  TFile *f = new TFile("XSection2Srho_finalversion.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S2YieldEta");
  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_2Srho_MuIDMinus.root");
  TFile *f = new TFile("MuIDMinus_10ptbins_2Srho_fv.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S2YieldEta");
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
  S1->SetMaximum(1.5);
  S1->Draw("p");
  S2->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("XSection Vs |y| For Ups(2S)");
  legge = legg->AddEntry(S1, "Y(2S) 2Srho","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "Y(2S) 2Srho_MuIDMinus","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S12->SetLineColor(3); S12->SetMarkerColor(3); 
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  S12->SetMinimum(-0.2);
  S12->SetMaximum(0.2);
  S12->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("#Delta XSection Vs |y| For Ups(2S)");
  legge = legg->AddEntry(S12, "Y(2S) 2Srho - 2Srho_MuIDMinus","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S13->SetLineColor(3); S13->SetMarkerColor(3); 
  TCanvas *c3 = new TCanvas("c3","c3",1200,600); 
  S13->SetMinimum(-0.2);
  S13->SetMaximum(0.2);
  S13->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("Percentage #Delta XSection Vs |y| For Ups(2S)");
  legge = legg->AddEntry(S13, "Y(2S) 2Srho - 2Srho_MuIDMinus / Y(2S) 2Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  //TFile *f = new TFile("MuIDMinus_10ptbins_2Srho_rap.root", "RECREATE");
  TFile *f = new TFile("MuIDMinus_10ptbins_2Srho_rap_v2.root", "RECREATE");
  S12->Write();
  
}

void XSTrigMinus_2S(){

  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_2Srho.root");
  TFile *f = new TFile("XSection2Srho_finalversion.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S2YieldEta");
  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_2Srho_TrigMinus.root");
  TFile *f = new TFile("TrigMinus_10ptbins_2Srho_fv.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S2YieldEta");
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
  S1->SetMaximum(1.5);
  S1->Draw("p");
  S2->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("XSection Vs |y| For Ups(2S)");
  legge = legg->AddEntry(S1, "Y(2S) 2Srho","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "Y(2S) 2Srho_TrigMinus","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S12->SetLineColor(3); S12->SetMarkerColor(3); 
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  S12->SetMinimum(-0.2);
  S12->SetMaximum(0.2);
  S12->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("#Delta XSection Vs |y| For Ups(2S)");
  legge = legg->AddEntry(S12, "Y(2S) 2Srho - 2Srho_TrigMinus","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S13->SetLineColor(3); S13->SetMarkerColor(3); 
  TCanvas *c3 = new TCanvas("c3","c3",1200,600); 
  S13->SetMinimum(-0.15);
  S13->SetMaximum(0.15);
  S13->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("Percentage #Delta XSection Vs |y| For Ups(2S)");
  legge = legg->AddEntry(S13, "Y(2S) 2Srho - 2Srho_TrigMinus / Y(2S) 2Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  //TFile *f = new TFile("TrigMinus_10ptbins_2Srho_rap.root", "RECREATE");
  TFile *f = new TFile("TrigMinus_10ptbins_2Srho_rap_v2.root", "RECREATE");
  S12->Write();
  
}

void XSTrigPlus_2S(){

  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_2Srho.root");
  TFile *f = new TFile("XSection2Srho_finalversion.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S2YieldEta");
  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_2Srho_TrigPlus.root");
  TFile *f = new TFile("TrigPlus_10ptbins_2Srho_fv.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S2YieldEta");
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
  legg->SetHeader("XSection Vs |y| For Ups(2S)");
  legge = legg->AddEntry(S1, "Y(2S) 2Srho","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "Y(2S) 2Srho_TrigPlus","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S12->SetLineColor(3); S12->SetMarkerColor(3); 
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  S12->SetMinimum(-0.2);
  S12->SetMaximum(0.2);
  S12->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("#Delta XSection Vs |y| For Ups(2S)");
  legge = legg->AddEntry(S12, "Y(2S) 2Srho - 2Srho_TrigPlus","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S13->SetLineColor(3); S13->SetMarkerColor(3); 
  TCanvas *c3 = new TCanvas("c3","c3",1200,600); 
  S13->SetMinimum(-0.2);
  S13->SetMaximum(0.2);
  S13->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("Percentage #Delta XSection Vs |y| For Ups(2S)");
  legge = legg->AddEntry(S13, "Y(2S) 2Srho - 2Srho_TrigPlus / Y(2S) 2Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  //TFile *f = new TFile("TrigPlus_10ptbins_2Srho_rap.root", "RECREATE");
  TFile *f = new TFile("TrigPlus_10ptbins_2Srho_rap_v2.root", "RECREATE");
  S12->Write();
  
}

void XSSigPDF_2S(){

  TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_2Srho.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S2YieldEta");
  TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_2Srho_SigPDF.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S2YieldEta");
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
  S1->SetMaximum(1.5);
  S1->Draw("p");
  S2->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("XSection Vs |y| For Ups(2S)");
  legge = legg->AddEntry(S1, "Y(2S) 2Srho","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "Y(2S) 2Srho_SigPDF","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S12->SetLineColor(3); S12->SetMarkerColor(3); 
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  S12->SetMinimum(-0.2);
  S12->SetMaximum(0.1);
  S12->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("#Delta XSection Vs |y| For Ups(2S)");
  legge = legg->AddEntry(S12, "Y(2S) 2Srho - 2Srho_SigPDF","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S13->SetLineColor(3); S13->SetMarkerColor(3); 
  TCanvas *c3 = new TCanvas("c3","c3",1200,600); 
  S13->SetMinimum(-0.4);
  S13->SetMaximum(0.1);
  S13->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("Percentage #Delta XSection Vs |y| For Ups(2S)");
  legge = legg->AddEntry(S13, "Y(2S) 2Srho - 2Srho_SigPDF / Y(2S) 2Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  TFile *f = new TFile("SigPDF_10ptbins_2Srho_rap.root", "RECREATE");
  S12->Write();
  
}

void XSBgPDF_2S(){

  TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_2Srho_BgPDF_ref.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S2YieldEta");
  TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_2Srho_BgPDF.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S2YieldEta");
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
  S1->SetMaximum(1.5);
  S1->Draw("p");
  S2->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("XSection Vs |y| For Ups(2S)");
  legge = legg->AddEntry(S1, "Y(2S) 2Srho","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "Y(2S) 2Srho_BgPDF","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S12->SetLineColor(3); S12->SetMarkerColor(3); 
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  S12->SetMinimum(-0.2);
  S12->SetMaximum(0.2);
  S12->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("#Delta XSection Vs |y| For Ups(2S)");
  legge = legg->AddEntry(S12, "Y(2S) 2Srho - 2Srho_BgPDF","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S13->SetLineColor(3); S13->SetMarkerColor(3); 
  TCanvas *c3 = new TCanvas("c3","c3",1200,600); 
  S13->SetMinimum(-0.2);
  S13->SetMaximum(0.2);
  S13->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("Percentage #Delta XSection Vs |y| For Ups(2S)");
  legge = legg->AddEntry(S13, "Y(2S) 2Srho - 2Srho_BgPDF / Y(2S) 2Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  TFile *f = new TFile("BgPDF_10ptbins_2Srho_rap.root", "RECREATE");
  S12->Write();
  
}

void XS1SRho_3S(){

  TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_3Srho.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S3YieldEta");
  TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_1Srho.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S3YieldEta");
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
  S1->SetMaximum(1.);
  S1->Draw("p");
  S2->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("XSection Vs |y| For Ups(3S)");
  legge = legg->AddEntry(S1, "Y(3S) 3Srho","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "Y(3S) 1Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S12->SetLineColor(3); S12->SetMarkerColor(3); 
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  S12->SetMinimum(-0.2);
  S12->SetMaximum(0.2);
  S12->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("#Delta XSection Vs |y| For Ups(3S)");
  legge = legg->AddEntry(S12, "Y(3S) 3Srho - 1Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S13->SetLineColor(3); S13->SetMarkerColor(3); 
  TCanvas *c3 = new TCanvas("c3","c3",1200,600); 
  S13->SetMinimum(-0.2);
  S13->SetMaximum(0.2);
  S13->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("Percentage #Delta XSection Vs |y| For Ups(3S)");
  legge = legg->AddEntry(S13, "Y(3S) 3Srho - 1Srho / Y(3S) 3Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  TFile *f = new TFile("1SRho_10ptbins_3Srho_rap.root", "RECREATE");
  S12->Write();
  
}

void XS2SRho_3S(){

  TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_3Srho.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S3YieldEta");
  TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_2Srho.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S3YieldEta");
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
  S1->SetMaximum(1.);
  S1->Draw("p");
  S2->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("XSection Vs |y| For Ups(3S)");
  legge = legg->AddEntry(S1, "Y(3S) 3Srho","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "Y(3S) 2Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S12->SetLineColor(3); S12->SetMarkerColor(3); 
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  S12->SetMinimum(-0.2);
  S12->SetMaximum(0.2);
  S12->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("#Delta XSection Vs |y| For Ups(3S)");
  legge = legg->AddEntry(S12, "Y(3S) 3Srho - 2Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S13->SetLineColor(3); S13->SetMarkerColor(3); 
  TCanvas *c3 = new TCanvas("c3","c3",1200,600); 
  S13->SetMinimum(-0.2);
  S13->SetMaximum(0.2);
  S13->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("Percentage #Delta XSection Vs |y| For Ups(3S)");
  legge = legg->AddEntry(S13, "Y(3S) 3Srho - 2Srho / Y(3S) 3Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  TFile *f = new TFile("2SRho_10ptbins_3Srho_rap.root", "RECREATE");
  S12->Write();
  
}

void XSMuIDPlus_3S(){

  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_3Srho.root");
  TFile *f = new TFile("XSection3Srho_finalversion.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S3YieldEta");
  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_3Srho_MuIDPlus.root");
  TFile *f = new TFile("MuIDPlus_10ptbins_3Srho_fv.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S3YieldEta");
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
  S1->SetMaximum(1.);
  S1->Draw("p");
  S2->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("XSection Vs |y| For Ups(3S)");
  legge = legg->AddEntry(S1, "Y(3S) 3Srho","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "Y(3S) 3Srho_MuIDPlus","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S12->SetLineColor(3); S12->SetMarkerColor(3); 
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  S12->SetMinimum(-0.2);
  S12->SetMaximum(0.2);
  S12->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("#Delta XSection Vs |y| For Ups(3S)");
  legge = legg->AddEntry(S12, "Y(3S) 3Srho - 3Srho_MuIDPlus","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S13->SetLineColor(3); S13->SetMarkerColor(3); 
  TCanvas *c3 = new TCanvas("c3","c3",1200,600); 
  S13->SetMinimum(-0.2);
  S13->SetMaximum(0.2);
  S13->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("Percentage #Delta XSection Vs |y| For Ups(3S)");
  legge = legg->AddEntry(S13, "Y(3S) 3Srho - 3Srho_MuIDPlus / Y(3S) 3Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  //TFile *f = new TFile("MuIDPlus_10ptbins_3Srho_rap.root", "RECREATE");
  TFile *f = new TFile("MuIDPlus_10ptbins_3Srho_rap_v2.root", "RECREATE");
  S12->Write();
  
}

void XSMuIDMinus_3S(){

  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_3Srho.root");
  TFile *f = new TFile("XSection3Srho_finalversion.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S3YieldEta");
  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_3Srho_MuIDMinus.root");
  TFile *f = new TFile("MuIDMinus_10ptbins_3Srho_fv.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S3YieldEta");
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
  S1->SetMaximum(1.);
  S1->Draw("p");
  S2->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("XSection Vs |y| For Ups(3S)");
  legge = legg->AddEntry(S1, "Y(3S) 3Srho","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "Y(3S) 3Srho_MuIDMinus","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S12->SetLineColor(3); S12->SetMarkerColor(3); 
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  S12->SetMinimum(-0.2);
  S12->SetMaximum(0.2);
  S12->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("#Delta XSection Vs |y| For Ups(3S)");
  legge = legg->AddEntry(S12, "Y(3S) 3Srho - 3Srho_MuIDMinus","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S13->SetLineColor(3); S13->SetMarkerColor(3); 
  TCanvas *c3 = new TCanvas("c3","c3",1200,600); 
  S13->SetMinimum(-0.2);
  S13->SetMaximum(0.2);
  S13->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("Percentage #Delta XSection Vs |y| For Ups(3S)");
  legge = legg->AddEntry(S13, "Y(3S) 3Srho - 3Srho_MuIDMinus / Y(3S) 3Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  //TFile *f = new TFile("MuIDMinus_10ptbins_3Srho_rap.root", "RECREATE");
  TFile *f = new TFile("MuIDMinus_10ptbins_3Srho_rap_v2.root", "RECREATE");
  S12->Write();
  
}

void XSTrigMinus_3S(){

  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_3Srho.root");
  TFile *f = new TFile("XSection3Srho_finalversion.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S3YieldEta");
  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_3Srho_TrigMinus.root");
  TFile *f = new TFile("TrigMinus_10ptbins_3Srho_fv.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S3YieldEta");
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
  S1->SetMaximum(1.);
  S1->Draw("p");
  S2->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("XSection Vs |y| For Ups(3S)");
  legge = legg->AddEntry(S1, "Y(3S) 3Srho","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "Y(3S) 3Srho_TrigMinus","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S12->SetLineColor(3); S12->SetMarkerColor(3); 
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  S12->SetMinimum(-0.2);
  S12->SetMaximum(0.2);
  S12->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("#Delta XSection Vs |y| For Ups(3S)");
  legge = legg->AddEntry(S12, "Y(3S) 3Srho - 3Srho_TrigMinus","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S13->SetLineColor(3); S13->SetMarkerColor(3); 
  TCanvas *c3 = new TCanvas("c3","c3",1200,600); 
  S13->SetMinimum(-0.2);
  S13->SetMaximum(0.2);
  S13->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("Percentage #Delta XSection Vs |y| For Ups(3S)");
  legge = legg->AddEntry(S13, "Y(3S) 3Srho - 3Srho_TrigMinus / Y(3S) 3Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  //TFile *f = new TFile("TrigMinus_10ptbins_3Srho_rap.root", "RECREATE");
  TFile *f = new TFile("TrigMinus_10ptbins_3Srho_rap_v2.root", "RECREATE");
  S12->Write();
  
}

void XSTrigPlus_3S(){

  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_3Srho.root");
  TFile *f = new TFile("XSection3Srho_finalversion.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S3YieldEta");
  //TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_3Srho_TrigPlus.root");
  TFile *f = new TFile("TrigPlus_10ptbins_3Srho_fv.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S3YieldEta");
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
  S1->SetMaximum(1.);
  S1->Draw("p");
  S2->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("XSection Vs |y| For Ups(3S)");
  legge = legg->AddEntry(S1, "Y(3S) 3Srho","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "Y(3S) 3Srho_TrigPlus","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S12->SetLineColor(3); S12->SetMarkerColor(3); 
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  S12->SetMinimum(-0.2);
  S12->SetMaximum(0.2);
  S12->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("#Delta XSection Vs |y| For Ups(3S)");
  legge = legg->AddEntry(S12, "Y(3S) 3Srho - 3Srho_TrigPlus","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S13->SetLineColor(3); S13->SetMarkerColor(3); 
  TCanvas *c3 = new TCanvas("c3","c3",1200,600); 
  S13->SetMinimum(-0.2);
  S13->SetMaximum(0.2);
  S13->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("Percentage #Delta XSection Vs |y| For Ups(3S)");
  legge = legg->AddEntry(S13, "Y(3S) 3Srho - 3Srho_TrigPlus / Y(3S) 3Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  //TFile *f = new TFile("TrigPlus_10ptbins_3Srho_rap.root", "RECREATE");
  TFile *f = new TFile("TrigPlus_10ptbins_3Srho_rap_v2.root", "RECREATE");
  S12->Write();
  
}


void XSSigPDF_3S(){

  TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_3Srho.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S3YieldEta");
  TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_3Srho_SigPDF.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S3YieldEta");
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
  S1->SetMaximum(1.);
  S1->Draw("p");
  S2->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("XSection Vs |y| For Ups(3S)");
  legge = legg->AddEntry(S1, "Y(3S) 3Srho","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "Y(3S) 3Srho_SigPDF","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S12->SetLineColor(3); S12->SetMarkerColor(3); 
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  S12->SetMinimum(-0.2);
  S12->SetMaximum(0.2);
  S12->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("#Delta XSection Vs |y| For Ups(3S)");
  legge = legg->AddEntry(S12, "Y(3S) 3Srho - 3Srho_SigPDF","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S13->SetLineColor(3); S13->SetMarkerColor(3); 
  TCanvas *c3 = new TCanvas("c3","c3",1200,600); 
  S13->SetMinimum(-0.2);
  S13->SetMaximum(0.2);
  S13->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("Percentage #Delta XSection Vs |y| For Ups(3S)");
  legge = legg->AddEntry(S13, "Y(3S) 3Srho - 3Srho_SigPDF / Y(3S) 3Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  TFile *f = new TFile("SigPDF_10ptbins_3Srho_rap.root", "RECREATE");
  S12->Write();
  
}

void XSBgPDF_3S(){

  TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_3Srho_BgPDF_ref.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S3YieldEta");
  TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_3Srho_BgPDF.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S3YieldEta");
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
  S1->SetMaximum(1.);
  S1->Draw("p");
  S2->Draw("psame");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("XSection Vs |y| For Ups(3S)");
  legge = legg->AddEntry(S1, "Y(3S) 3Srho","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "Y(3S) 3Srho_BgPDF","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S12->SetLineColor(3); S12->SetMarkerColor(3); 
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  S12->SetMinimum(-0.2);
  S12->SetMaximum(0.2);
  S12->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("#Delta XSection Vs |y| For Ups(3S)");
  legge = legg->AddEntry(S12, "Y(3S) 3Srho - 3Srho_BgPDF","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  S13->SetLineColor(3); S13->SetMarkerColor(3); 
  TCanvas *c3 = new TCanvas("c3","c3",1200,600); 
  S13->SetMinimum(-0.2);
  S13->SetMaximum(0.2);
  S13->Draw("p");
  legg = new TLegend(0.3,0.6,0.7,0.9);
  legg->SetHeader("Percentage #Delta XSection Vs |y| For Ups(3S)");
  legge = legg->AddEntry(S13, "Y(3S) 3Srho - 3Srho_BgPDF / Y(3S) 3Srho","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  TFile *f = new TFile("BgPDF_10ptbins_3Srho_rap.root", "RECREATE");
  S12->Write();
  
}

