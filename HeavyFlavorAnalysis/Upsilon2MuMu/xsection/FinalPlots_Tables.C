#include "TMath.h"
#include <math.h>
#include "TF1.h"
#include "TH1.h"

void XSSystTot_1S(){

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
  TFile *f = new TFile("MuIDPlus_10ptbins_1Srho_.root");
  TH1D *S6;
  S6 = (TH1D*)gFile->Get("S1YieldPt");
  TFile *f = new TFile("TrigPlus_10ptbins_1Srho_.root");
  TH1D *S7;
  S7 = (TH1D*)gFile->Get("S1YieldPt"); 
  TFile *f = new TFile("3SRho_10ptbins_1Srho_.root");
  TH1D *S8;
  S8 = (TH1D*)gFile->Get("S1YieldPt");
  
  double s1(0.), s5(0.), s2(0.), e5(0.), err5(0.);
  double s1_(0.), s2_(0.), e5_(0.);
  double Stat[10]; double Errh[10]; double Errl[10]; double y[10];
  
  double s0(0.), s0_(0.);
  double Errh_ratio[10]; double Errl_ratio[10]; double y_ratio[10];
  for (int i = 1; i <= S1->GetNbinsX(); ++i) {
    
    s1 += S1->GetBinContent(i)*S1->GetBinContent(i); ///Muid
    s1 += S2->GetBinContent(i)*S2->GetBinContent(i); //Trig
    s1 += S4->GetBinContent(i)*S4->GetBinContent(i); //bg
    s1_ += S3->GetBinContent(i)*S3->GetBinContent(i); //sig
    s1_ += S4->GetBinContent(i)*S4->GetBinContent(i); //bg
    s1_ += S6->GetBinContent(i)*S6->GetBinContent(i); //Muid
    s1_ += S7->GetBinContent(i)*S7->GetBinContent(i); //Trig
    s1_ += S8->GetBinContent(i)*S8->GetBinContent(i); //rho
    cout << " dSigma(Y(1S))/dp_{T} = "  << S5->GetBinContent(i) << " + " << TMath::Sqrt(s1) << " - " << TMath::Sqrt(s1_) << endl;
    e5 += TMath::Sqrt((s1)+(S5->GetBinError(i)*S5->GetBinError(i)))*S5->GetBinWidth(i);
    e5_ += TMath::Sqrt((s1_)+(S5->GetBinError(i)*S5->GetBinError(i)))*S5->GetBinWidth(i);
    Stat[i-1]=S5->GetBinError(i);
    s5 += S5->GetBinContent(i)*S5->GetBinWidth(i);
    s2 += TMath::Sqrt(s1)*S5->GetBinWidth(i);
    s2_ += TMath::Sqrt(s1_)*S5->GetBinWidth(i);
    err5 = TMath::Sqrt((s1)+(S5->GetBinError(i)*S5->GetBinError(i)));
    S5->SetBinError(i,err5); // only one side -- needs to be edited
    Errh[i-1] = TMath::Sqrt((s1)+(S5->GetBinError(i)*S5->GetBinError(i)));
    Errl[i-1] = TMath::Sqrt((s1_)+(S5->GetBinError(i)*S5->GetBinError(i)));
    y[i-1] = S5->GetBinContent(i);
    s1=0; s1_=0;
    
    /// For Ratio Calculation
    s0 += S4->GetBinContent(i)*S4->GetBinContent(i); //bg
    s0_ += S3->GetBinContent(i)*S3->GetBinContent(i); //sig
    s0_ += S4->GetBinContent(i)*S4->GetBinContent(i); //bg
    s0_ += S8->GetBinContent(i)*S8->GetBinContent(i); //rho
    Errh_ratio[i-1] = TMath::Sqrt((s1)+(S5->GetBinError(i)*S5->GetBinError(i)));
    Errl_ratio[i-1] = TMath::Sqrt((s1_)+(S5->GetBinError(i)*S5->GetBinError(i)));
    y_ratio[i-1] = S5->GetBinContent(i);
    
  }
  
  double xbin[10] = {1., 3., 6., 9., 11.5, 14.5, 18., 22.5, 27.5, 40.};
  double xh[10] = {1., 1., 2., 1., 1.5, 1.5, 2., 2.5, 2.5, 10.};
  double xl[10] = {1., 1., 2., 1., 1.5, 1.5, 2., 2.5, 2.5, 10.};
  
  gr = new TGraphAsymmErrors(10,xbin,y,xl,xh,Errl,Errh);
  gr->SetName("Ups1S");
  gr->SetMarkerColor(1);
  gr->SetMarkerStyle(21);
  gr->Draw("AP");
  
  gr_ratio = new TGraphAsymmErrors(10,xbin,y_ratio,xl,xh,Errl_ratio,Errh_ratio);
  gr_ratio->SetName("Ups1S_ratio");
  gr_ratio->SetMarkerColor(1);
  gr_ratio->SetMarkerStyle(21);
  gr_ratio->Draw("AP");  
  
  cout << " Y(1S) 1Srho Xsection = "  << s5 << " + " << s2 << " ("  << e5  << ")" << " - " << s2_ << " ("  << e5_  << ")"  <<  endl;
  TFile *f = new TFile("Final1S.root", "RECREATE");
  gr->Write();
  gr_ratio->Write();
  
  // Table for 1S Xsection and Errors
  double muid_h(0.), trig_h(0.), sig(0.), muid_l(0.), trig_l(0.), tot_h(0.), tot_l(0.), stat(0.), rho_l(0.), bg(0.); 
  ofstream OUT("1S.tex");
  OUT << "% ----------------------------------------------------------------------" << endl;
  OUT << "% -- 1S" << endl;
  for ( int x = 1; x <= S5->GetNbinsX(); ++x ){
    stat = Stat[x-1]/S5->GetBinContent(x);
    muid_h = TMath::Sqrt(S1->GetBinContent(x)*S1->GetBinContent(x))/S5->GetBinContent(x); 
    trig_h = TMath::Sqrt(S2->GetBinContent(x)*S2->GetBinContent(x))/S5->GetBinContent(x);
    sig = TMath::Sqrt(S3->GetBinContent(x)*S3->GetBinContent(x))/S5->GetBinContent(x);
    bg = TMath::Sqrt(S4->GetBinContent(x)*S4->GetBinContent(x))/S5->GetBinContent(x);
    muid_l = TMath::Sqrt(S6->GetBinContent(x)*S6->GetBinContent(x))/S5->GetBinContent(x);
    trig_l = TMath::Sqrt(S7->GetBinContent(x)*S7->GetBinContent(x))/S5->GetBinContent(x);  
    rho_l = TMath::Sqrt(S8->GetBinContent(x)*S8->GetBinContent(x))/S5->GetBinContent(x); 
    tot_h = TMath::Sqrt(muid_h*muid_h + trig_h*trig_h + bg*bg);
    tot_l = TMath::Sqrt(muid_l*muid_l + trig_l*trig_l + sig*sig + bg*bg + rho_l*rho_l);
    OUT << Form("\\vdef{%iS_sigma%i} {\\ensuremath{ {%.3f } } }",1, x, S5->GetBinContent(x)*S5->GetBinWidth(x) ) << endl;
    OUT << Form("\\vdef{%iS_toterror%i} {\\ensuremath{ {%.3f } } }",1, x, S5->GetBinError(x)*S5->GetBinWidth(x) ) << endl;
    OUT << Form("\\vdef{%iS_statoversigma%i} {\\ensuremath{ {%.3f } } }",1, x, stat ) << endl;
    OUT << Form("\\vdef{%iS_systHighoversigma%i} {\\ensuremath{ {%.3f } } }",1, x, tot_h ) << endl;
    OUT << Form("\\vdef{%iS_systLowoversigma%i} {\\ensuremath{ {%.3f } } }",1, x, tot_l ) << endl;
    OUT << Form("\\vdef{%iS_trigHighoversigma%i} {\\ensuremath{ {%.3f } } }",1, x, trig_h ) << endl;
    OUT << Form("\\vdef{%iS_trigLowoversigma%i} {\\ensuremath{ {%.3f } } }",1, x, trig_l ) << endl;
    OUT << Form("\\vdef{%iS_muidHighoversigma%i} {\\ensuremath{ {%.3f } } }",1, x, muid_h ) << endl;
    OUT << Form("\\vdef{%iS_muidLowoversigma%i} {\\ensuremath{ {%.3f } } }",1, x, muid_l ) << endl;    
    OUT << Form("\\vdef{%iS_sigoversigma%i} {\\ensuremath{ {%.3f } } }",1, x, sig ) << endl;
    OUT << Form("\\vdef{%iS_bgoversigma%i} {\\ensuremath{ {%.3f } } }",1, x, bg ) << endl;
    OUT << Form("\\vdef{%iS_rhoLowoversigma%i} {\\ensuremath{ {%.3f } } }",1, x, rho_l ) << endl;
  }
  
  OUT.close();
  
}

void XSSystTot_2S(){

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
  TFile *f = new TFile("MuIDPlus_10ptbins_2Srho_.root");
  TH1D *S6;
  S6 = (TH1D*)gFile->Get("S2YieldPt");
  TFile *f = new TFile("TrigPlus_10ptbins_2Srho_.root");
  TH1D *S7;
  S7 = (TH1D*)gFile->Get("S2YieldPt"); 
  TFile *f = new TFile("3SRho_10ptbins_2Srho_.root");
  TH1D *S8;
  S8 = (TH1D*)gFile->Get("S2YieldPt");
  TFile *f = new TFile("1SRho_10ptbins_2Srho_.root");
  TH1D *S9;
  S9 = (TH1D*)gFile->Get("S2YieldPt");  
  
  double s1(0.), s5(0.), s2(0.), e5(0.), err5(0.);
  double s1_(0.), s2_(0.), e5_(0.);
  double Stat[10]; double Errh[10]; double Errl[10]; double y[10]; 
  
  double s0(0.), s0_(0.);
  double Errh_ratio[10]; double Errl_ratio[10]; double y_ratio[10]; 
  for (int i = 1; i <= S1->GetNbinsX(); ++i) {
    
    s1 += S1->GetBinContent(i)*S1->GetBinContent(i); //Muid
    s1 += S2->GetBinContent(i)*S2->GetBinContent(i); //Trig
    s1 += S4->GetBinContent(i)*S4->GetBinContent(i); //bg
    s1 += S9->GetBinContent(i)*S9->GetBinContent(i); //rho
    s1_ += S3->GetBinContent(i)*S3->GetBinContent(i); //Sig
    s1_ += S4->GetBinContent(i)*S4->GetBinContent(i); //bg
    s1_ += S6->GetBinContent(i)*S6->GetBinContent(i); //Muid
    s1_ += S7->GetBinContent(i)*S7->GetBinContent(i); //Trig    
    s1_ += S8->GetBinContent(i)*S8->GetBinContent(i); //rho  
    cout << " dSigma(Y(2S))/dp_{T} = "  << S5->GetBinContent(i) << " + " << TMath::Sqrt(s1) <<  " - " << TMath::Sqrt(s1_) << endl;
    e5 += TMath::Sqrt((s1)+(S5->GetBinError(i)*S5->GetBinError(i)))*S5->GetBinWidth(i);
    e5_ += TMath::Sqrt((s1_)+(S5->GetBinError(i)*S5->GetBinError(i)))*S5->GetBinWidth(i);
    Stat[i-1]=S5->GetBinError(i);
    s5 += S5->GetBinContent(i)*S5->GetBinWidth(i);
    s2 += TMath::Sqrt(s1)*S5->GetBinWidth(i);
    s2_ += TMath::Sqrt(s1_)*S5->GetBinWidth(i);
    err5 = TMath::Sqrt((s1)+(S5->GetBinError(i)*S5->GetBinError(i)));
    S5->SetBinError(i,err5);
    Errh[i-1] = TMath::Sqrt((s1)+(S5->GetBinError(i)*S5->GetBinError(i)));
    Errl[i-1] = TMath::Sqrt((s1_)+(S5->GetBinError(i)*S5->GetBinError(i)));
    y[i-1] = S5->GetBinContent(i);
    s1=0;s1_=0;	
    
    /// For Ratio Calculation
    s0 += S4->GetBinContent(i)*S4->GetBinContent(i); //bg
    s0_ += S3->GetBinContent(i)*S3->GetBinContent(i); //sig
    s0_ += S4->GetBinContent(i)*S4->GetBinContent(i); //bg
    s0_ += S8->GetBinContent(i)*S8->GetBinContent(i); //rho
    Errh_ratio[i-1] = TMath::Sqrt((s1)+(S5->GetBinError(i)*S5->GetBinError(i)));
    Errl_ratio[i-1] = TMath::Sqrt((s1_)+(S5->GetBinError(i)*S5->GetBinError(i)));
    y_ratio[i-1] = S5->GetBinContent(i);
    
  }
 
  double xbin[10] = {1., 3., 6., 9., 11.5, 14.5, 18., 22.5, 27.5, 40.};
  double xh[10] = {1., 1., 2., 1., 1.5, 1.5, 2., 2.5, 2.5, 10.};
  double xl[10] = {1., 1., 2., 1., 1.5, 1.5, 2., 2.5, 2.5, 10.};
  
  gr = new TGraphAsymmErrors(10,xbin,y,xl,xh,Errl,Errh);
  gr->SetName("Ups2S");
  gr->SetMarkerColor(4);
  gr->SetMarkerStyle(21);
  gr->Draw("AP");
  
  gr_ratio = new TGraphAsymmErrors(10,xbin,y_ratio,xl,xh,Errl_ratio,Errh_ratio);
  gr_ratio->SetName("Ups2S_ratio");
  gr_ratio->SetMarkerColor(4);
  gr_ratio->SetMarkerStyle(21);
  gr_ratio->Draw("AP");
  
  cout << " Y(2S) 2Srho Xsection = "  << s5 << " + " << s2 << " ("  << e5  << ")" << " - " << s2_ << " ("  << e5_  << ")" <<  endl;
  TFile *f = new TFile("Final2S.root", "RECREATE");
  gr->Write();
  gr_ratio->Write();
  
  // Table for 2S Xsection and Errors
  double muid_h(0.), trig_h(0.), sig(0.), muid_l(0.), trig_l(0.), tot_h(0.), tot_l(0.), stat(0.), rho_l(0.), bg(0.), rho_h(0.); 
  ofstream OUT("2S.tex");
  OUT << "% ----------------------------------------------------------------------" << endl;
  OUT << "% -- 2S" << endl;
  for ( int x = 1; x <= S5->GetNbinsX(); ++x ){
    stat = Stat[x-1]/S5->GetBinContent(x);
    muid_h = TMath::Sqrt(S1->GetBinContent(x)*S1->GetBinContent(x))/S5->GetBinContent(x); 
    trig_h = TMath::Sqrt(S2->GetBinContent(x)*S2->GetBinContent(x))/S5->GetBinContent(x);
    sig = TMath::Sqrt(S3->GetBinContent(x)*S3->GetBinContent(x))/S5->GetBinContent(x);
    bg = TMath::Sqrt(S4->GetBinContent(x)*S4->GetBinContent(x))/S5->GetBinContent(x);
    muid_l = TMath::Sqrt(S6->GetBinContent(x)*S6->GetBinContent(x))/S5->GetBinContent(x);
    trig_l = TMath::Sqrt(S7->GetBinContent(x)*S7->GetBinContent(x))/S5->GetBinContent(x);  
    rho_l = TMath::Sqrt(S8->GetBinContent(x)*S8->GetBinContent(x))/S5->GetBinContent(x);
    rho_h = TMath::Sqrt(S9->GetBinContent(x)*S9->GetBinContent(x))/S5->GetBinContent(x);
    tot_h = TMath::Sqrt(((muid_h*muid_h) + (trig_h*trig_h) + (bg*bg) + (rho_h*rho_h)));
    tot_l = TMath::Sqrt((muid_l*muid_l) + (trig_l*trig_l) + (sig*sig) + (bg*bg) + (rho_l*rho_l));
    OUT << Form("\\vdef{%iS_sigma%i} {\\ensuremath{ {%.3f } } }",2, x, S5->GetBinContent(x)*S5->GetBinWidth(x) ) << endl;
    OUT << Form("\\vdef{%iS_toterror%i} {\\ensuremath{ {%.3f } } }",2, x, S5->GetBinError(x)*S5->GetBinWidth(x) ) << endl;
    OUT << Form("\\vdef{%iS_statoversigma%i} {\\ensuremath{ {%.3f } } }",2, x, stat ) << endl;
    OUT << Form("\\vdef{%iS_systHighoversigma%i} {\\ensuremath{ {%.3f } } }",2, x, tot_h ) << endl;
    OUT << Form("\\vdef{%iS_systLowoversigma%i} {\\ensuremath{ {%.3f } } }",2, x, tot_l ) << endl;
    OUT << Form("\\vdef{%iS_trigHighoversigma%i} {\\ensuremath{ {%.3f } } }",2, x, trig_h ) << endl;
    OUT << Form("\\vdef{%iS_trigLowoversigma%i} {\\ensuremath{ {%.3f } } }",2, x, trig_l ) << endl;
    OUT << Form("\\vdef{%iS_muidHighoversigma%i} {\\ensuremath{ {%.3f } } }",2, x, muid_h ) << endl;
    OUT << Form("\\vdef{%iS_muidLowoversigma%i} {\\ensuremath{ {%.3f } } }",2, x, muid_l ) << endl;    
    OUT << Form("\\vdef{%iS_sigoversigma%i} {\\ensuremath{ {%.3f } } }",2, x, sig ) << endl;
    OUT << Form("\\vdef{%iS_bgoversigma%i} {\\ensuremath{ {%.3f } } }",2, x, bg ) << endl;
    OUT << Form("\\vdef{%iS_rhoLowoversigma%i} {\\ensuremath{ {%.3f } } }",2, x, rho_l ) << endl;
    OUT << Form("\\vdef{%iS_rhoHighoversigma%i} {\\ensuremath{ {%.3f } } }",2, x, rho_h ) << endl;
  }
  
  OUT.close();
  
}

void XSSystTot_3S(){

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
  TFile *f = new TFile("MuIDPlus_10ptbins_3Srho_.root");
  TH1D *S6;
  S6 = (TH1D*)gFile->Get("S3YieldPt");
  TFile *f = new TFile("TrigPlus_10ptbins_3Srho_.root");
  TH1D *S7;
  S7 = (TH1D*)gFile->Get("S3YieldPt"); 
  TFile *f = new TFile("1SRho_10ptbins_3Srho_.root");
  TH1D *S8;
  S8 = (TH1D*)gFile->Get("S3YieldPt");
  
  double s1(0.), s5(0.), s2(0.), e5(0.), err5(0.);
  double s1_(0.), s2_(0.), e5_(0.);
  double Stat[10]; double Errh[10]; double Errl[10]; double y[10];

  double s0(0.), s0_(0.);
  double Errh_ratio[10]; double Errl_ratio[10]; double y_ratio[10]; 
  for (int i = 1; i <= S1->GetNbinsX(); ++i) {
    
    s1 += S1->GetBinContent(i)*S1->GetBinContent(i); //Muid
    s1 += S2->GetBinContent(i)*S2->GetBinContent(i); //Trig
    s1 += S4->GetBinContent(i)*S4->GetBinContent(i); //bg
    s1 += S8->GetBinContent(i)*S8->GetBinContent(i); //rho
    s1_ += S3->GetBinContent(i)*S3->GetBinContent(i); //sig
    s1_ += S4->GetBinContent(i)*S4->GetBinContent(i); //bg
    s1_ += S6->GetBinContent(i)*S6->GetBinContent(i); //Muid
    s1_ += S7->GetBinContent(i)*S7->GetBinContent(i); //Trig
    s1_ += S8->GetBinContent(i)*S8->GetBinContent(i); //rho    
    cout << " dSigma(Y(1S))/dp_{T} = "  << S5->GetBinContent(i) << " + " << TMath::Sqrt(s1) << " - " << TMath::Sqrt(s1_) << endl;
    e5 += TMath::Sqrt((s1)+(S5->GetBinError(i)*S5->GetBinError(i)))*S5->GetBinWidth(i);
    e5_ += TMath::Sqrt((s1)+(S5->GetBinError(i)*S5->GetBinError(i)))*S5->GetBinWidth(i);
    Stat[i-1]=S5->GetBinError(i);
    s5 += S5->GetBinContent(i)*S5->GetBinWidth(i);
    s2 += TMath::Sqrt(s1)*S5->GetBinWidth(i);
    s2_ += TMath::Sqrt(s1_)*S5->GetBinWidth(i);
    err5 = TMath::Sqrt((s1)+(S5->GetBinError(i)*S5->GetBinError(i)));
    S5->SetBinError(i,err5);
    Errh[i-1] = TMath::Sqrt((s1)+(S5->GetBinError(i)*S5->GetBinError(i)));
    Errl[i-1] = TMath::Sqrt((s1_)+(S5->GetBinError(i)*S5->GetBinError(i)));
    y[i-1] = S5->GetBinContent(i);
    s1=0;s1_=0;	
    
    /// For Ratio Calculation
    s0 += S4->GetBinContent(i)*S4->GetBinContent(i); //bg
    s0_ += S3->GetBinContent(i)*S3->GetBinContent(i); //sig
    s0_ += S4->GetBinContent(i)*S4->GetBinContent(i); //bg
    s0_ += S8->GetBinContent(i)*S8->GetBinContent(i); //rho
    Errh_ratio[i-1] = TMath::Sqrt((s1)+(S5->GetBinError(i)*S5->GetBinError(i)));
    Errl_ratio[i-1] = TMath::Sqrt((s1_)+(S5->GetBinError(i)*S5->GetBinError(i)));
    y_ratio[i-1] = S5->GetBinContent(i);
  }
    
  double xbin[10] = {1., 3., 6., 9., 11.5, 14.5, 18., 22.5, 27.5, 40.};
  double xh[10] = {1., 1., 2., 1., 1.5, 1.5, 2., 2.5, 2.5, 10.};
  double xl[10] = {1., 1., 2., 1., 1.5, 1.5, 2., 2.5, 2.5, 10.};
  
  gr = new TGraphAsymmErrors(10,xbin,y,xl,xh,Errl,Errh);
  gr->SetName("Ups3S");
  gr->SetMarkerColor(2);
  gr->SetMarkerStyle(21);
  gr->Draw("AP");
  
  gr_ratio = new TGraphAsymmErrors(10,xbin,y_ratio,xl,xh,Errl_ratio,Errh_ratio);
  gr_ratio->SetName("Ups3S_ratio");
  gr_ratio->SetMarkerColor(2);
  gr_ratio->SetMarkerStyle(21);
  gr_ratio->Draw("AP");
  
  cout << " Y(3S) 1Srho Xsection = "  << s5 << " + " << s2 << " ("  << e5  << ")" << " - " << s2_ << " ("  << e5_ << ")" <<  endl;
  TFile *f = new TFile("Final3S.root", "RECREATE");
  gr->Write();
  gr_ratio->Write();
  
  // Table for 3S Xsection and Errors
  double muid_h(0.), trig_h(0.), sig(0.), muid_l(0.), trig_l(0.), tot_h(0.), tot_l(0.), stat(0.), rho_h(0.), bg(0.); 
  ofstream OUT("3S.tex");
  OUT << "% ----------------------------------------------------------------------" << endl;
  OUT << "% -- 3S" << endl;
  for ( int x = 1; x <= S5->GetNbinsX(); ++x ){
    stat = Stat[x-1]/S5->GetBinContent(x);
    muid_h = TMath::Sqrt(S1->GetBinContent(x)*S1->GetBinContent(x))/S5->GetBinContent(x); 
    trig_h = TMath::Sqrt(S2->GetBinContent(x)*S2->GetBinContent(x))/S5->GetBinContent(x);
    sig = TMath::Sqrt(S3->GetBinContent(x)*S3->GetBinContent(x))/S5->GetBinContent(x);
    bg = TMath::Sqrt(S4->GetBinContent(x)*S4->GetBinContent(x))/S5->GetBinContent(x);
    muid_l = TMath::Sqrt(S6->GetBinContent(x)*S6->GetBinContent(x))/S5->GetBinContent(x);
    trig_l = TMath::Sqrt(S7->GetBinContent(x)*S7->GetBinContent(x))/S5->GetBinContent(x);  
    rho_h = TMath::Sqrt(S8->GetBinContent(x)*S8->GetBinContent(x))/S5->GetBinContent(x); 
    tot_h = TMath::Sqrt(muid_h*muid_h + trig_h*trig_h + bg*bg + rho_h*rho_h);
    tot_l = TMath::Sqrt(muid_l*muid_l + trig_l*trig_l + sig*sig + bg*bg);
    OUT << Form("\\vdef{%iS_sigma%i} {\\ensuremath{ {%.3f } } }",3, x, S5->GetBinContent(x)*S5->GetBinWidth(x) ) << endl;
    OUT << Form("\\vdef{%iS_toterror%i} {\\ensuremath{ {%.3f } } }",3, x, S5->GetBinError(x)*S5->GetBinWidth(x) ) << endl;
    OUT << Form("\\vdef{%iS_statoversigma%i} {\\ensuremath{ {%.3f } } }",3, x, stat ) << endl;
    OUT << Form("\\vdef{%iS_systHighoversigma%i} {\\ensuremath{ {%.3f } } }",3, x, tot_h ) << endl;
    OUT << Form("\\vdef{%iS_systLowoversigma%i} {\\ensuremath{ {%.3f } } }",3, x, tot_l ) << endl;
    OUT << Form("\\vdef{%iS_trigHighoversigma%i} {\\ensuremath{ {%.3f } } }",3, x, trig_h ) << endl;
    OUT << Form("\\vdef{%iS_trigLowoversigma%i} {\\ensuremath{ {%.3f } } }",3, x, trig_l ) << endl;
    OUT << Form("\\vdef{%iS_muidHighoversigma%i} {\\ensuremath{ {%.3f } } }",3, x, muid_h ) << endl;
    OUT << Form("\\vdef{%iS_muidLowoversigma%i} {\\ensuremath{ {%.3f } } }",3, x, muid_l ) << endl;    
    OUT << Form("\\vdef{%iS_sigoversigma%i} {\\ensuremath{ {%.3f } } }",3, x, sig ) << endl;
    OUT << Form("\\vdef{%iS_bgoversigma%i} {\\ensuremath{ {%.3f } } }",3, x, bg ) << endl;
    OUT << Form("\\vdef{%iS_rhoHighoversigma%i} {\\ensuremath{ {%.3f } } }",3, x, rho_h ) << endl;
  }
  
  OUT.close();
  
}

void XSSystPol_1S(){

  TFile *f = new TFile("XSection_1Srho_CSMi.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S1YieldPt");
  TFile *f = new TFile("XSection_1Srho_CSPl.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S1YieldPt");
  TFile *f = new TFile("XSection_1Srho_HelMi.root");
  TH1D *S3;
  S3 = (TH1D*)gFile->Get("S1YieldPt");
  TFile *f = new TFile("XSection_1Srho_HelPl.root");
  TH1D *S4;
  S4 = (TH1D*)gFile->Get("S1YieldPt");  
  TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_1Srho.root");
  TH1D *S5;
  S5 = (TH1D*)gFile->Get("S1YieldPt");    
    
  // Table for Systmetical Uncertainty from Polarization
  double csmi(0.), cspl(0.), helmi(0.), helpl(0.);
  ofstream OUT("Pol1S.tex");
  OUT << "% ----------------------------------------------------------------------" << endl;
  OUT << "% -- Polarization" << endl;
  for ( int x = 1; x <= S5->GetNbinsX(); ++x ){
    csmi = (S1->GetBinContent(x) - S5->GetBinContent(x))/S5->GetBinContent(x);
    cspl = (S2->GetBinContent(x) - S5->GetBinContent(x))/S5->GetBinContent(x);
    helmi = (S3->GetBinContent(x) - S5->GetBinContent(x))/S5->GetBinContent(x);
    helpl = (S4->GetBinContent(x) - S5->GetBinContent(x))/S5->GetBinContent(x);    
    OUT << Form("\\vdef{%iS_csmi%i} {\\ensuremath{ {%.3f } } }",1, x, csmi ) << endl;
    OUT << Form("\\vdef{%iS_cspl%i} {\\ensuremath{ {%.3f } } }",1, x, cspl ) << endl;
    OUT << Form("\\vdef{%iS_helmi%i} {\\ensuremath{ {%.3f } } }",1, x, helmi ) << endl;
    OUT << Form("\\vdef{%iS_helpl%i} {\\ensuremath{ {%.3f } } }",1, x, helpl ) << endl;
  }
  
  OUT.close();
  
}

void XSSystPol_2S(){

  TFile *f = new TFile("XSection_2Srho_CSMi.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S2YieldPt");
  TFile *f = new TFile("XSection_2Srho_CSPl.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S2YieldPt");
  TFile *f = new TFile("XSection_2Srho_HelMi.root");
  TH1D *S3;
  S3 = (TH1D*)gFile->Get("S2YieldPt");
  TFile *f = new TFile("XSection_2Srho_HelPl.root");
  TH1D *S4;
  S4 = (TH1D*)gFile->Get("S2YieldPt");  
  TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_2Srho.root");
  TH1D *S5;
  S5 = (TH1D*)gFile->Get("S2YieldPt");    
    
  // Table for Systmetical Uncertainty from Polarization
  double csmi(0.), cspl(0.), helmi(0.), helpl(0.);
  ofstream OUT("Pol2S.tex");
  OUT << "% ----------------------------------------------------------------------" << endl;
  OUT << "% -- Polarization" << endl;
  for ( int x = 1; x <= S5->GetNbinsX(); ++x ){
    csmi = (S1->GetBinContent(x) - S5->GetBinContent(x))/S5->GetBinContent(x);
    cspl = (S2->GetBinContent(x) - S5->GetBinContent(x))/S5->GetBinContent(x);
    helmi = (S3->GetBinContent(x) - S5->GetBinContent(x))/S5->GetBinContent(x);
    helpl = (S4->GetBinContent(x) - S5->GetBinContent(x))/S5->GetBinContent(x);    
    OUT << Form("\\vdef{%iS_csmi%i} {\\ensuremath{ {%.3f } } }",2, x, csmi ) << endl;
    OUT << Form("\\vdef{%iS_cspl%i} {\\ensuremath{ {%.3f } } }",2, x, cspl ) << endl;
    OUT << Form("\\vdef{%iS_helmi%i} {\\ensuremath{ {%.3f } } }",2, x, helmi ) << endl;
    OUT << Form("\\vdef{%iS_helpl%i} {\\ensuremath{ {%.3f } } }",2, x, helpl ) << endl;
  }
  
  OUT.close();
  
}

void XSSystPol_3S(){

  TFile *f = new TFile("XSection_3Srho_CSMi.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S3YieldPt");
  TFile *f = new TFile("XSection_3Srho_CSPl.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S3YieldPt");
  TFile *f = new TFile("XSection_3Srho_HelMi.root");
  TH1D *S3;
  S3 = (TH1D*)gFile->Get("S3YieldPt");
  TFile *f = new TFile("XSection_3Srho_HelPl.root");
  TH1D *S4;
  S4 = (TH1D*)gFile->Get("S3YieldPt");  
  TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_3Srho.root");
  TH1D *S5;
  S5 = (TH1D*)gFile->Get("S3YieldPt");    
    
  // Table for Systmetical Uncertainty from Polarization
  double csmi(0.), cspl(0.), helmi(0.), helpl(0.);
  ofstream OUT("Pol3S.tex");
  OUT << "% ----------------------------------------------------------------------" << endl;
  OUT << "% -- Polarization" << endl;
  for ( int x = 1; x <= S5->GetNbinsX(); ++x ){
    csmi = (S1->GetBinContent(x) - S5->GetBinContent(x))/S5->GetBinContent(x);
    cspl = (S2->GetBinContent(x) - S5->GetBinContent(x))/S5->GetBinContent(x);
    helmi = (S3->GetBinContent(x) - S5->GetBinContent(x))/S5->GetBinContent(x);
    helpl = (S4->GetBinContent(x) - S5->GetBinContent(x))/S5->GetBinContent(x);    
    OUT << Form("\\vdef{%iS_csmi%i} {\\ensuremath{ {%.3f } } }",3, x, csmi ) << endl;
    OUT << Form("\\vdef{%iS_cspl%i} {\\ensuremath{ {%.3f } } }",3, x, cspl ) << endl;
    OUT << Form("\\vdef{%iS_helmi%i} {\\ensuremath{ {%.3f } } }",3, x, helmi ) << endl;
    OUT << Form("\\vdef{%iS_helpl%i} {\\ensuremath{ {%.3f } } }",3, x, helpl ) << endl;
  }
  
  OUT.close();
  
}

void Overlay(){
  
  gStyle->SetOptStat(00000000000);
  TFile *f = new TFile("Final1S.root");
  TGraphAsymmErrors *S1;
  S1 = (TGraphAsymmErrors*)gFile->Get("Ups1S");
  TFile *f = new TFile("Final2S.root");
  TGraphAsymmErrors *S2;
  S2 = (TGraphAsymmErrors*)gFile->Get("Ups2S");
  TFile *f = new TFile("Final3S.root");
  TGraphAsymmErrors *S3;
  S3 = (TGraphAsymmErrors*)gFile->Get("Ups3S");  
  
  
  TCanvas *c1 = new TCanvas("c1", "c1", 800,600);
  c1->SetLogy();
  S1->SetLineColor(1); S1->SetMarkerColor(1); S2->SetLineColor(2); S2->SetMarkerColor(2); S3->SetLineColor(4); S3->SetMarkerColor(4);
  S1->GetXaxis()->SetTitle("p_{T}^{#Upsilon}");
  S1->GetYaxis()->SetTitle("d#sigma/dp_{T}#times Br(#mu#mu)");
  S1->SetTitle("");
  S1->SetMinimum(0.0001);
  S1->SetMaximum(1.5);
  S1->Draw("AP");
  S2->Draw("P");
  S3->Draw("P");
  legg = new TLegend(0.5,0.6,0.7,0.8);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("");
  legge = legg->AddEntry(S1, "#Upsilon(1S)" ,"p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "#Upsilon(2S)","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(S3, "#Upsilon(3S)","p"); legge->SetTextColor(kBlue);
  legg->Draw();
  c1->SaveAs("Overlay_log.pdf");
}

void Ratio_unpol(){
  
  gStyle->SetOptStat(00000000000);
  TFile *f = new TFile("Final1S.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S1YieldPt");
  TFile *f = new TFile("Final2S.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S2YieldPt");
  TFile *f = new TFile("Final3S.root");
  TH1D *S3;
  S3 = (TH1D*)gFile->Get("S3YieldPt");  
  TH1D *S21 = (TH1D*)S1->Clone(); 
  TH1D *S31 = (TH1D*)S1->Clone();
  
  double s21(0.), s31(0.), e21(0.), e31(0.);
  double term1(0.), term2(0.),term3(0.), term21(0.), term31(0.);
  for (int i = 1; i <= S1->GetNbinsX(); ++i) {
    
   s21 = S2->GetBinContent(i)/S1->GetBinContent(i);
   s31 = S3->GetBinContent(i)/S1->GetBinContent(i);
   S21->SetBinContent(i,s21); 
   S31->SetBinContent(i,s31); 
   term1 = (S1->GetBinError(i)/S1->GetBinContent(i))*(S1->GetBinError(i)/S1->GetBinContent(i));
   term2 = (S2->GetBinError(i)/S2->GetBinContent(i))*(S2->GetBinError(i)/S2->GetBinContent(i));
   term3 = (S3->GetBinError(i)/S3->GetBinContent(i))*(S3->GetBinError(i)/S3->GetBinContent(i));
   term21 = term1+term2;
   term31 = term1+term3;
   e21 = s21*TMath::Sqrt(term21);
   e31 = s31*TMath::Sqrt(term31);
   S21->SetBinError(i,e21); 
   S31->SetBinError(i,e31);    
   
  }  
  
  S21->SetLineColor(1); S21->SetMarkerColor(1); S31->SetLineColor(4); S31->SetMarkerColor(4);
  S21->GetXaxis()->SetTitle("p_{T}^{#Upsilon}");
  S21->GetYaxis()->SetTitle("#sigma#times Br(#mu#mu)");
  S21->SetTitle("");
  S21->SetMinimum(0.);
  S21->SetMaximum(1.);
  S21->Draw("p");
  S31->Draw("psame");
  legg = new TLegend(0.1,0.6,0.5,0.8);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("");
  legge = legg->AddEntry(S21, "#Upsilon(2S)/#Upsilon(1S)" ,"p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S31, "#Upsilon(3S)/#Upsilon(1S)","p"); legge->SetTextColor(kBlue);
  legg->Draw();
  
  // Table for Ratios
  ofstream OUT("Ratios.tex");
  OUT << "% ----------------------------------------------------------------------" << endl;
  OUT << "% -- Ratios" << endl;
  for ( int x = 1; x <= S21->GetNbinsX(); ++x ){
    OUT << Form("\\vdef{%iSover%iS__bin%iContent} {\\ensuremath{ {%.2f } } }",2 ,1, x, S21->GetBinContent(x) ) << endl;
    OUT << Form("\\vdef{%iSover%iS_bin%iError} {\\ensuremath{ {%.2f } } }",2 ,1, x, S21->GetBinError(x) ) << endl;
    OUT << Form("\\vdef{%iSover%iS__bin%iContent} {\\ensuremath{ {%.2f } } }",3 ,1, x, S31->GetBinContent(x) ) << endl;
    OUT << Form("\\vdef{%iSover%iS_bin%iError} {\\ensuremath{ {%.2f } } }",3 ,1, x, S31->GetBinError(x) ) << endl;   
  }
  OUT.close();
  
}
