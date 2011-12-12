#include "TMath.h"
#include <math.h>
#include "TF1.h"
#include "TH1.h"

void XSSystTot_1S(){

  TFile *f = new TFile("MuIDMinus_10ptbins_1Srho_v2.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S1YieldPt");
  TFile *f = new TFile("TrigMinus_10ptbins_1Srho_v2.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S1YieldPt");
  TFile *f = new TFile("SigPDF_10ptbins_1Srho_v2.root");
  TH1D *S3;
  S3 = (TH1D*)gFile->Get("S1YieldPt");
  TFile *f = new TFile("BgPDF_10ptbins_1Srho_v2.root");
  TH1D *S4;
  S4 = (TH1D*)gFile->Get("S1YieldPt");  
  TFile *f = new TFile("XSection1Sfinal.root");
  TH1D *S5;
  S5 = (TH1D*)gFile->Get("S1YieldPt");    
  //TFile *f = new TFile("XSection_1Srho_0_2_0_50.root");
  //TFile *f = new TFile("XSection0_2_0_30.root");
  //TH1D *S55;
  //S55 = (TH1D*)gFile->Get("S1YieldPt");  
  TFile *f = new TFile("MuIDPlus_10ptbins_1Srho_v2.root");
  TH1D *S6;
  S6 = (TH1D*)gFile->Get("S1YieldPt");
  TFile *f = new TFile("TrigPlus_10ptbins_1Srho_v2.root");
  TH1D *S7;
  S7 = (TH1D*)gFile->Get("S1YieldPt"); 
  TFile *f = new TFile("3SRho_10ptbins_1Srho_.root");
  TH1D *S8;
  S8 = (TH1D*)gFile->Get("S1YieldPt");
  TFile *f = new TFile("NoMSC_10ptbins_1Srho_v3.root");
  TH1D *S9;
  S9 = (TH1D*)gFile->Get("S1YieldPt");  
  TFile *f = new TFile("BinMigrationPt_1S.root");
  TH1D *S10;
  S10 = (TH1D*)gFile->Get("Eff_Cand");    
  TH1D *S100 = (TH1D*)S1->Clone(); 
  TH1D *S200 = (TH1D*)S1->Clone();
  S100->SetName("Ups1S_ratioh");
  S200->SetName("Ups1S_ratiol");
  
  double s1(0.), s5(0.), s2(0.), e5(0.), err5(0.);
  double s1_(0.), s2_(0.), e5_(0.);
  double Stat[10]; double Errh[10]; double Errl[10]; double y[10];
  //double Stat[9]; double Errh[9]; double Errl[9]; double y[9];
  
  double s0(0.), s0_(0.);
  double Errh_ratio[10]; double Errl_ratio[10]; double y_ratio[10];
  //double Errh_ratio[9]; double Errl_ratio[9]; double y_ratio[9];
  
  for (int i = 1; i <= S5->GetNbinsX(); ++i) {
    
    s1 += S1->GetBinContent(i)*S1->GetBinContent(i); ///Muid
    s1 += S2->GetBinContent(i)*S2->GetBinContent(i); //Trig
    s1 += S4->GetBinContent(i)*S4->GetBinContent(i); //bg
    s1 += S9->GetBinContent(i)*S9->GetBinContent(i); //msc
    s1 += S3->GetBinContent(i)*S3->GetBinContent(i); //sig
    s1 += S10->GetBinContent(i)*S10->GetBinContent(i); //binmig
    s1_ += S3->GetBinContent(i)*S3->GetBinContent(i); //sig
    s1_ += S4->GetBinContent(i)*S4->GetBinContent(i); //bg
    s1_ += S6->GetBinContent(i)*S6->GetBinContent(i); //Muid
    s1_ += S7->GetBinContent(i)*S7->GetBinContent(i); //Trig
    s1_ += S8->GetBinContent(i)*S8->GetBinContent(i); //rho
    s1_ += S9->GetBinContent(i)*S9->GetBinContent(i); //msc
    s1_ += S10->GetBinContent(i)*S10->GetBinContent(i); //binmig
    cout << " dSigma(Y(1S))/dp_{T} = "  << S5->GetBinContent(i) << " + " << TMath::Sqrt(s1) << " - " << TMath::Sqrt(s1_) << endl;
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
    //y[i-1] = S55->GetBinContent(i); //for csm/com comparison
    s1=0; s1_=0;
    
    /// For Ratio Calculation
    s0 += S4->GetBinContent(i)*S4->GetBinContent(i); //bg
    s0 += S9->GetBinContent(i)*S9->GetBinContent(i); //msc
    s0 += S3->GetBinContent(i)*S3->GetBinContent(i); //sig
    s0 += S10->GetBinContent(i)*S10->GetBinContent(i); //binmig
    s0_ += S3->GetBinContent(i)*S3->GetBinContent(i); //sig
    s0_ += S4->GetBinContent(i)*S4->GetBinContent(i); //bg
    s0_ += S8->GetBinContent(i)*S8->GetBinContent(i); //rho
    s0_ += S9->GetBinContent(i)*S9->GetBinContent(i); //msc
    s0_ += S10->GetBinContent(i)*S10->GetBinContent(i); //binmig
    Errh_ratio[i-1] = TMath::Sqrt((s0)+(S5->GetBinError(i)*S5->GetBinError(i)));
    Errl_ratio[i-1] = TMath::Sqrt((s0_)+(S5->GetBinError(i)*S5->GetBinError(i)));
    y_ratio[i-1] = S5->GetBinContent(i);
    
    S100->SetBinContent(i, S5->GetBinContent(i));
    S200->SetBinContent(i, S5->GetBinContent(i));
    S100->SetBinError(i, TMath::Sqrt((s0)+(S5->GetBinError(i)*S5->GetBinError(i))));
    S200->SetBinError(i, TMath::Sqrt((s0_)+(S5->GetBinError(i)*S5->GetBinError(i))));    
    s0=0;s0_=0;
  }
  
  double xbin[10] = {1.27, 2.95, 5.72, 8.92, 11.30, 14.32, 17.73, 22.06, 27.16, 36.25};
  double xl[10] = {1.27, 0.95, 1.72, 0.92, 1.30, 1.32, 1.73, 2.06, 2.16, 6.25};
  double xh[10] = {0.73, 1.05, 2.28, 1.08, 1.70, 1.68, 2.27, 2.94, 2.84, 13.75};
  
  //double xbin[9] = {1.27, 2.95, 5.72, 8.92, 11.30, 14.32, 17.73, 22.06, 27.16};
  //double xl[9] = {1.27, 0.95, 1.72, 0.92, 1.30, 1.32, 1.73, 2.06, 2.16};
  //double xh[9] = {0.73, 1.05, 2.28, 1.08, 1.70, 1.68, 2.27, 2.94, 2.84};  
  
  double br_scale(40.3);
  
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
  //TFile *f = new TFile("Final1S_0_2_0_30.root", "RECREATE");
  gr->Write();
  gr_ratio->Write();
  S100->Write();
  S200->Write();
  
  // Table for 1S Xsection and Errors
  double muid_h(0.), trig_h(0.), sig(0.), muid_l(0.), trig_l(0.), tot_h(0.), tot_l(0.), stat(0.), rho_l(0.), bg(0.), msc(0.), binmig(0.); 
  ofstream OUT("1S.tex");
  OUT << "% ----------------------------------------------------------------------" << endl;
  OUT << "% -- 1S" << endl;
  for ( int x = 1; x <= S5->GetNbinsX(); ++x ){
    stat = Stat[x-1]/S5->GetBinContent(x);
    muid_h = TMath::Sqrt(S1->GetBinContent(x)*S1->GetBinContent(x))/S5->GetBinContent(x); 
    trig_h = TMath::Sqrt(S2->GetBinContent(x)*S2->GetBinContent(x))/S5->GetBinContent(x);
    sig = TMath::Sqrt(S3->GetBinContent(x)*S3->GetBinContent(x))/S5->GetBinContent(x);
    bg = TMath::Sqrt(S4->GetBinContent(x)*S4->GetBinContent(x))/S5->GetBinContent(x);
    msc = TMath::Sqrt(S9->GetBinContent(x)*S9->GetBinContent(x))/S5->GetBinContent(x);
    muid_l = TMath::Sqrt(S6->GetBinContent(x)*S6->GetBinContent(x))/S5->GetBinContent(x);
    trig_l = TMath::Sqrt(S7->GetBinContent(x)*S7->GetBinContent(x))/S5->GetBinContent(x);  
    rho_l = TMath::Sqrt(S8->GetBinContent(x)*S8->GetBinContent(x))/S5->GetBinContent(x); 
    binmig = TMath::Sqrt(S10->GetBinContent(x)*S10->GetBinContent(x))/S5->GetBinContent(x);
    tot_h = TMath::Sqrt(muid_h*muid_h + trig_h*trig_h + bg*bg + sig*sig + msc*msc + binmig*binmig);
    tot_l = TMath::Sqrt(muid_l*muid_l + trig_l*trig_l + sig*sig + bg*bg + rho_l*rho_l + msc*msc + binmig*binmig);
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
    OUT << Form("\\vdef{%iS_mscoversigma%i} {\\ensuremath{ {%.3f } } }",1, x, msc ) << endl;
    OUT << Form("\\vdef{%iS_binmigoversigma%i} {\\ensuremath{ {%.3f } } }",1, x, binmig ) << endl;
  }
  
  OUT.close();
  
}

void XSSystTot_2S(){

  TFile *f = new TFile("MuIDMinus_10ptbins_2Srho_v2.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S2YieldPt");
  TFile *f = new TFile("TrigMinus_10ptbins_2Srho_v2.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S2YieldPt");
  TFile *f = new TFile("SigPDF_10ptbins_2Srho_v2.root");
  TH1D *S3;
  S3 = (TH1D*)gFile->Get("S2YieldPt");
  TFile *f = new TFile("BgPDF_10ptbins_2Srho_v2.root");
  TH1D *S4;
  S4 = (TH1D*)gFile->Get("S2YieldPt");  
  TFile *f = new TFile("XSection2Sfinal.root");
  //TFile *f = new TFile("XSection0_2_0_30.root");
  TH1D *S5;
  S5 = (TH1D*)gFile->Get("S2YieldPt");    
  TFile *f = new TFile("MuIDPlus_10ptbins_2Srho_v2.root");
  TH1D *S6;
  S6 = (TH1D*)gFile->Get("S2YieldPt");
  TFile *f = new TFile("TrigPlus_10ptbins_2Srho_v2.root");
  TH1D *S7;
  S7 = (TH1D*)gFile->Get("S2YieldPt"); 
  TFile *f = new TFile("3SRho_10ptbins_2Srho_.root");
  TH1D *S8;
  S8 = (TH1D*)gFile->Get("S2YieldPt");
  TFile *f = new TFile("1SRho_10ptbins_2Srho_.root");
  TH1D *S9;
  S9 = (TH1D*)gFile->Get("S2YieldPt");  
  TFile *f = new TFile("NoMSC_10ptbins_2Srho_v3.root");
  TH1D *S10;
  S10 = (TH1D*)gFile->Get("S2YieldPt");    
  TFile *f = new TFile("BinMigrationPt_2S.root");
  TH1D *S11;
  S11 = (TH1D*)gFile->Get("Eff_Cand");      
  TH1D *S100 = (TH1D*)S1->Clone(); 
  TH1D *S200 = (TH1D*)S1->Clone();
  S100->SetName("Ups2S_ratioh");
  S200->SetName("Ups2S_ratiol");
  
  double s1(0.), s5(0.), s2(0.), e5(0.), err5(0.);
  double s1_(0.), s2_(0.), e5_(0.);
  double Stat[10]; double Errh[10]; double Errl[10]; double y[10]; 
  //double Stat[9]; double Errh[9]; double Errl[9]; double y[9]; 
  
  double s0(0.), s0_(0.);
  double Errh_ratio[10]; double Errl_ratio[10]; double y_ratio[10]; 
  //double Errh_ratio[9]; double Errl_ratio[9]; double y_ratio[9]; 
  
  for (int i = 1; i <= S1->GetNbinsX(); ++i) {
    
    s1 += S1->GetBinContent(i)*S1->GetBinContent(i); //Muid
    s1 += S2->GetBinContent(i)*S2->GetBinContent(i); //Trig
    s1 += S4->GetBinContent(i)*S4->GetBinContent(i); //bg
    s1 += S9->GetBinContent(i)*S9->GetBinContent(i); //rho
    s1 += S3->GetBinContent(i)*S3->GetBinContent(i); //Sig
    s1 += S10->GetBinContent(i)*S10->GetBinContent(i); //msc
    s1 += S11->GetBinContent(i)*S11->GetBinContent(i); //binmigration
    s1_ += S3->GetBinContent(i)*S3->GetBinContent(i); //Sig
    s1_ += S4->GetBinContent(i)*S4->GetBinContent(i); //bg
    s1_ += S6->GetBinContent(i)*S6->GetBinContent(i); //Muid
    s1_ += S7->GetBinContent(i)*S7->GetBinContent(i); //Trig    
    s1_ += S8->GetBinContent(i)*S8->GetBinContent(i); //rho  
    s1_ += S10->GetBinContent(i)*S10->GetBinContent(i); //msc
    s1_ += S11->GetBinContent(i)*S11->GetBinContent(i); //binmigration
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
    s0 += S3->GetBinContent(i)*S3->GetBinContent(i); //sig
    s0 += S10->GetBinContent(i)*S10->GetBinContent(i); //msc
    s0 += S11->GetBinContent(i)*S11->GetBinContent(i); //binmigration
    s0_ += S3->GetBinContent(i)*S3->GetBinContent(i); //sig
    s0_ += S4->GetBinContent(i)*S4->GetBinContent(i); //bg
    s0_ += S8->GetBinContent(i)*S8->GetBinContent(i); //rho
    s0_ += S10->GetBinContent(i)*S10->GetBinContent(i); //msc
    s0_ += S11->GetBinContent(i)*S11->GetBinContent(i); //binmigration
    Errh_ratio[i-1] = TMath::Sqrt((s0)+(S5->GetBinError(i)*S5->GetBinError(i)));
    Errl_ratio[i-1] = TMath::Sqrt((s0_)+(S5->GetBinError(i)*S5->GetBinError(i)));
    y_ratio[i-1] = S5->GetBinContent(i);
    
    S100->SetBinContent(i, S5->GetBinContent(i));
    S200->SetBinContent(i, S5->GetBinContent(i));
    S100->SetBinError(i, TMath::Sqrt((s0)+(S5->GetBinError(i)*S5->GetBinError(i))));
    S200->SetBinError(i, TMath::Sqrt((s0_)+(S5->GetBinError(i)*S5->GetBinError(i))));    
    s0=0;s0_=0;
  }
 
  double xbin[10] = {1.27, 2.95, 5.72, 8.92, 11.30, 14.32, 17.73, 22.06, 27.16, 36.25};
  double xl[10] = {1.27, 0.95, 1.72, 0.92, 1.30, 1.32, 1.73, 2.06, 2.16, 6.25};
  double xh[10] = {0.73, 1.05, 2.28, 1.08, 1.70, 1.68, 2.27, 2.94, 2.84, 13.75};
  
  //double xbin[9] = {1.27, 2.95, 5.72, 8.92, 11.30, 14.32, 17.73, 22.06, 27.16};
  //double xl[9] = {1.27, 0.95, 1.72, 0.92, 1.30, 1.32, 1.73, 2.06, 2.16};
  //double xh[9] = {0.73, 1.05, 2.28, 1.08, 1.70, 1.68, 2.27, 2.94, 2.84};  
  
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
  //TFile *f = new TFile("Final2S_0_2_0_30.root", "RECREATE");
  gr->Write();
  gr_ratio->Write();
  S100->Write();
  S200->Write();

  // Table for 2S Xsection and Errors
  double muid_h(0.), trig_h(0.), sig(0.), muid_l(0.), trig_l(0.), tot_h(0.), tot_l(0.), stat(0.), rho_l(0.), bg(0.), rho_h(0.), msc(0.), binmig(0.); 
  ofstream OUT("2S.tex");
  OUT << "% ----------------------------------------------------------------------" << endl;
  OUT << "% -- 2S" << endl;
  for ( int x = 1; x <= S5->GetNbinsX(); ++x ){
    stat = Stat[x-1]/S5->GetBinContent(x);
    muid_h = TMath::Sqrt(S1->GetBinContent(x)*S1->GetBinContent(x))/S5->GetBinContent(x); 
    trig_h = TMath::Sqrt(S2->GetBinContent(x)*S2->GetBinContent(x))/S5->GetBinContent(x);
    sig = TMath::Sqrt(S3->GetBinContent(x)*S3->GetBinContent(x))/S5->GetBinContent(x);
    bg = TMath::Sqrt(S4->GetBinContent(x)*S4->GetBinContent(x))/S5->GetBinContent(x);
    msc = TMath::Sqrt(S10->GetBinContent(x)*S10->GetBinContent(x))/S5->GetBinContent(x);
    muid_l = TMath::Sqrt(S6->GetBinContent(x)*S6->GetBinContent(x))/S5->GetBinContent(x);
    trig_l = TMath::Sqrt(S7->GetBinContent(x)*S7->GetBinContent(x))/S5->GetBinContent(x);  
    rho_l = TMath::Sqrt(S8->GetBinContent(x)*S8->GetBinContent(x))/S5->GetBinContent(x);
    rho_h = TMath::Sqrt(S9->GetBinContent(x)*S9->GetBinContent(x))/S5->GetBinContent(x);
    binmig = TMath::Sqrt(S11->GetBinContent(x)*S11->GetBinContent(x))/S5->GetBinContent(x);
    tot_h = TMath::Sqrt(((muid_h*muid_h) + (trig_h*trig_h) + (bg*bg) + (sig*sig) + (rho_h*rho_h) + (msc*msc) + (binmig*binmig)));
    tot_l = TMath::Sqrt((muid_l*muid_l) + (trig_l*trig_l) + (sig*sig) + (bg*bg) + (rho_l*rho_l) + (msc*msc) + (binmig*binmig));
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
    OUT << Form("\\vdef{%iS_mscoversigma%i} {\\ensuremath{ {%.3f } } }",2, x, msc ) << endl;
    OUT << Form("\\vdef{%iS_binmigoversigma%i} {\\ensuremath{ {%.3f } } }",2, x, binmig ) << endl; 
  }
  
  OUT.close();
  
}

void XSSystTot_3S(){

  TFile *f = new TFile("MuIDMinus_10ptbins_3Srho_v2.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S3YieldPt");
  TFile *f = new TFile("TrigMinus_10ptbins_3Srho_v2.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S3YieldPt");
  TFile *f = new TFile("SigPDF_10ptbins_3Srho_v2.root");
  TH1D *S3;
  S3 = (TH1D*)gFile->Get("S3YieldPt");
  TFile *f = new TFile("BgPDF_10ptbins_3Srho_v2.root");
  TH1D *S4;
  S4 = (TH1D*)gFile->Get("S3YieldPt");  
  TFile *f = new TFile("XSection3Sfinal.root");
  TH1D *S5;
  S5 = (TH1D*)gFile->Get("S3YieldPt");    
  //TFile *f = new TFile("XSection_3Srho_0_2_0_50.root");
  //TFile *f = new TFile("XSection0_2_0_30.root");
  //TH1D *S55;
  //S55 = (TH1D*)gFile->Get("S3YieldPt");    
  TFile *f = new TFile("MuIDPlus_10ptbins_3Srho_v2.root");
  TH1D *S6;
  S6 = (TH1D*)gFile->Get("S3YieldPt");
  TFile *f = new TFile("TrigPlus_10ptbins_3Srho_v2.root");
  TH1D *S7;
  S7 = (TH1D*)gFile->Get("S3YieldPt"); 
  TFile *f = new TFile("1SRho_10ptbins_3Srho_.root");
  TH1D *S8;
  S8 = (TH1D*)gFile->Get("S3YieldPt");
  TFile *f = new TFile("NoMSC_10ptbins_3Srho_v3.root");
  TH1D *S9;
  S9 = (TH1D*)gFile->Get("S3YieldPt");  
  TFile *f = new TFile("BinMigrationPt_3S.root");
  TH1D *S10;
  S10 = (TH1D*)gFile->Get("Eff_Cand"); 
  TH1D *S100 = (TH1D*)S1->Clone(); 
  TH1D *S200 = (TH1D*)S1->Clone();
  S100->SetName("Ups3S_ratioh");
  S200->SetName("Ups3S_ratiol");
  
  double s1(0.), s5(0.), s2(0.), e5(0.), err5(0.);
  double s1_(0.), s2_(0.), e5_(0.);
  double Stat[10]; double Errh[10]; double Errl[10]; double y[10];
  //double Stat[9]; double Errh[9]; double Errl[9]; double y[9];

  double s0(0.), s0_(0.);
  double Errh_ratio[10]; double Errl_ratio[10]; double y_ratio[10]; 
  //double Errh_ratio[9]; double Errl_ratio[9]; double y_ratio[9]; 
  for (int i = 1; i <= S1->GetNbinsX(); ++i) {
    
    s1 += S1->GetBinContent(i)*S1->GetBinContent(i); //Muid
    s1 += S2->GetBinContent(i)*S2->GetBinContent(i); //Trig
    s1 += S4->GetBinContent(i)*S4->GetBinContent(i); //bg
    s1 += S8->GetBinContent(i)*S8->GetBinContent(i); //rho
    s1 += S9->GetBinContent(i)*S9->GetBinContent(i); //msc
    s1 += S3->GetBinContent(i)*S3->GetBinContent(i); //sig
    s1 += S10->GetBinContent(i)*S10->GetBinContent(i); //binmig
    s1_ += S3->GetBinContent(i)*S3->GetBinContent(i); //sig
    s1_ += S4->GetBinContent(i)*S4->GetBinContent(i); //bg
    s1_ += S6->GetBinContent(i)*S6->GetBinContent(i); //Muid
    s1_ += S7->GetBinContent(i)*S7->GetBinContent(i); //Trig
    s1_ += S8->GetBinContent(i)*S8->GetBinContent(i); //rho
    s1_ += S9->GetBinContent(i)*S9->GetBinContent(i); //msc
    s1_ += S10->GetBinContent(i)*S10->GetBinContent(i); //binmig
    cout << " dSigma(Y(3S))/dp_{T} = "  << S5->GetBinContent(i) << " + " << TMath::Sqrt(s1) << " - " << TMath::Sqrt(s1_) << endl;
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
    //y[i-1] = S55->GetBinContent(i);
    s1=0;s1_=0;	
    
    /// For Ratio Calculation
    s0 += S4->GetBinContent(i)*S4->GetBinContent(i); //bg
    s0 += S3->GetBinContent(i)*S3->GetBinContent(i); //sig
    s0 += S9->GetBinContent(i)*S9->GetBinContent(i); //msc
    s0 += S10->GetBinContent(i)*S10->GetBinContent(i); //binmig
    s0_ += S3->GetBinContent(i)*S3->GetBinContent(i); //sig
    s0_ += S4->GetBinContent(i)*S4->GetBinContent(i); //bg
    s0_ += S8->GetBinContent(i)*S8->GetBinContent(i); //rho
    s0_ += S9->GetBinContent(i)*S9->GetBinContent(i); //msc
    s0_ += S10->GetBinContent(i)*S10->GetBinContent(i); //binmig
    Errh_ratio[i-1] = TMath::Sqrt((s0)+(S5->GetBinError(i)*S5->GetBinError(i)));
    Errl_ratio[i-1] = TMath::Sqrt((s0_)+(S5->GetBinError(i)*S5->GetBinError(i)));
    y_ratio[i-1] = S5->GetBinContent(i);
    
    S100->SetBinContent(i, S5->GetBinContent(i));
    S200->SetBinContent(i, S5->GetBinContent(i));
    S100->SetBinError(i, TMath::Sqrt((s0)+(S5->GetBinError(i)*S5->GetBinError(i))));
    S200->SetBinError(i, TMath::Sqrt((s0_)+(S5->GetBinError(i)*S5->GetBinError(i))));    
    s0=0;s0_=0;	
    
  }
    
  double xbin[10] = {1.27, 2.95, 5.72, 8.92, 11.30, 14.32, 17.73, 22.06, 27.16, 36.25};
  double xl[10] = {1.27, 0.95, 1.72, 0.92, 1.30, 1.32, 1.73, 2.06, 2.16, 6.25};
  double xh[10] = {0.73, 1.05, 2.28, 1.08, 1.70, 1.68, 2.27, 2.94, 2.84, 13.75};
  
  //double xbin[9] = {1.27, 2.95, 5.72, 8.92, 11.30, 14.32, 17.73, 22.06, 27.16};
  //double xl[9] = {1.27, 0.95, 1.72, 0.92, 1.30, 1.32, 1.73, 2.06, 2.16};
  //double xh[9] = {0.73, 1.05, 2.28, 1.08, 1.70, 1.68, 2.27, 2.94, 2.84};  
  
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
  //TFile *f = new TFile("Final3S_0_2_0_30.root", "RECREATE");
  gr->Write();
  gr_ratio->Write();
  S100->Write();
  S200->Write();
  
  // Table for 3S Xsection and Errors
  double muid_h(0.), trig_h(0.), sig(0.), muid_l(0.), trig_l(0.), tot_h(0.), tot_l(0.), stat(0.), rho_h(0.), bg(0.), msc(0.), binmig(0.); 
  ofstream OUT("3S.tex");
  OUT << "% ----------------------------------------------------------------------" << endl;
  OUT << "% -- 3S" << endl;
  for ( int x = 1; x <= S5->GetNbinsX(); ++x ){
    stat = Stat[x-1]/S5->GetBinContent(x);
    muid_h = TMath::Sqrt(S1->GetBinContent(x)*S1->GetBinContent(x))/S5->GetBinContent(x); 
    trig_h = TMath::Sqrt(S2->GetBinContent(x)*S2->GetBinContent(x))/S5->GetBinContent(x);
    sig = TMath::Sqrt(S3->GetBinContent(x)*S3->GetBinContent(x))/S5->GetBinContent(x);
    bg = TMath::Sqrt(S4->GetBinContent(x)*S4->GetBinContent(x))/S5->GetBinContent(x);
    msc = TMath::Sqrt(S9->GetBinContent(x)*S9->GetBinContent(x))/S5->GetBinContent(x);
    muid_l = TMath::Sqrt(S6->GetBinContent(x)*S6->GetBinContent(x))/S5->GetBinContent(x);
    trig_l = TMath::Sqrt(S7->GetBinContent(x)*S7->GetBinContent(x))/S5->GetBinContent(x);  
    rho_h = TMath::Sqrt(S8->GetBinContent(x)*S8->GetBinContent(x))/S5->GetBinContent(x); 
    binmig = TMath::Sqrt(S10->GetBinContent(x)*S10->GetBinContent(x))/S5->GetBinContent(x);
    tot_h = TMath::Sqrt(muid_h*muid_h + trig_h*trig_h + bg*bg + rho_h*rho_h + sig*sig + msc*msc + binmig*binmig);
    tot_l = TMath::Sqrt(muid_l*muid_l + trig_l*trig_l + sig*sig + bg*bg + msc*msc + binmig*binmig);
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
    OUT << Form("\\vdef{%iS_mscoversigma%i} {\\ensuremath{ {%.3f } } }",3, x, msc ) << endl;
    OUT << Form("\\vdef{%iS_binmigoversigma%i} {\\ensuremath{ {%.3f } } }",3, x, binmig ) << endl;
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
  S1->SetMarkerStyle(20); S2->SetMarkerStyle(21); S3->SetMarkerStyle(22);
  S1->GetXaxis()->SetTitle("p_{T}^{#Upsilon}(GeV/c)");
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

double f_fit(double *x, double *par) {
  
  Double_t fitval =  TMath::Power(x[0], par[0]);
  return fitval;
    
}


void Overlay_Fit(){
  
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
    
  f0 = new TF1("f0", f_fit, 10., 20., 1);
  f0->ReleaseParameter(0); 
  f0->SetParLimits(0, -3, -1);
  //f0->FixParameter(0, -1.7);
  S1->Fit(f0);
    
  TCanvas *c1 = new TCanvas("c1", "c1", 800,600);
  c1->SetLogy();
  S1->SetLineColor(1); S1->SetMarkerColor(1); S2->SetLineColor(2); S2->SetMarkerColor(2); S3->SetLineColor(4); S3->SetMarkerColor(4);
  S1->SetMarkerStyle(20); S2->SetMarkerStyle(21); S3->SetMarkerStyle(22);
  S1->GetXaxis()->SetTitle("p_{T}^{#Upsilon}(GeV/c)");
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
  c1->SaveAs("Overlay_log_fit.pdf");
}



void Ratio_unpol(){
  
  gStyle->SetOptStat(00000000000);
  TFile *f = new TFile("Final1S.root");
  TH1D *S1h; TH1D *S1l;
  S1h = (TH1D*)gFile->Get("Ups1S_ratioh");
  S1l = (TH1D*)gFile->Get("Ups1S_ratiol");
  TFile *f = new TFile("Final2S.root");
  TH1D *S2h; TH1D *S2l;
  S2h = (TH1D*)gFile->Get("Ups2S_ratioh");
  S2l = (TH1D*)gFile->Get("Ups2S_ratiol");  
  TFile *f = new TFile("Final3S.root");
  TH1D *S3h; TH1D *S3l;
  S3h = (TH1D*)gFile->Get("Ups3S_ratioh");
  S3l = (TH1D*)gFile->Get("Ups3S_ratiol");   
  
  double s21[10]; double s31[10]; double e21_h[10]; double e31_h[10]; double e21_l[10]; double e31_l[10];
  double term1_h(0.), term2_h(0.),term3_h(0.), term21_h(0.), term31_h(0.);
  double term1_l(0.), term2_l(0.),term3_l(0.), term21_l(0.), term31_l(0.);
  for (int i = 1; i <= S1h->GetNbinsX(); ++i) {
    
   s21[i-1] = S2h->GetBinContent(i)/S1h->GetBinContent(i);
   s31[i-1] = S3h->GetBinContent(i)/S1h->GetBinContent(i);
      
   term1_h = (S1h->GetBinError(i)/S1h->GetBinContent(i))*(S1h->GetBinError(i)/S1h->GetBinContent(i));
   term2_h = (S2h->GetBinError(i)/S2h->GetBinContent(i))*(S2h->GetBinError(i)/S2h->GetBinContent(i));
   term3_h = (S3h->GetBinError(i)/S3h->GetBinContent(i))*(S3h->GetBinError(i)/S3h->GetBinContent(i));
   term21_h = term1_h+term2_h;
   term31_h = term1_h+term3_h;
   e21_h[i-1] = s21[i-1]*TMath::Sqrt(term21_h);
   e31_h[i-1] = s31[i-1]*TMath::Sqrt(term31_h);
      
   term1_l = (S1l->GetBinError(i)/S1l->GetBinContent(i))*(S1l->GetBinError(i)/S1l->GetBinContent(i));
   term2_l = (S2l->GetBinError(i)/S2l->GetBinContent(i))*(S2l->GetBinError(i)/S2l->GetBinContent(i));
   term3_l = (S3l->GetBinError(i)/S3l->GetBinContent(i))*(S3l->GetBinError(i)/S3l->GetBinContent(i));
   term21_l = term1_l+term2_l;
   term31_l = term1_l+term3_l;
   e21_l[i-1] = s21[i-1]*TMath::Sqrt(term21_l);
   e31_l[i-1] = s31[i-1]*TMath::Sqrt(term31_l);   
   
   cout << e21_h[i-1] << endl; cout << e21_l[i-1] << endl;
   cout << e31_h[i-1] << endl; cout << e31_l[i-1] << endl;
   cout << endl;
  }  
  
  double xbin[10] = {1.27, 2.95, 5.72, 8.92, 11.30, 14.32, 17.73, 22.06, 27.16, 36.25};
  double xl[10] = {1.27, 0.95, 1.72, 0.92, 1.30, 1.32, 1.73, 2.06, 2.16, 6.25};
  double xh[10] = {0.73, 1.05, 2.28, 1.08, 1.70, 1.68, 2.27, 2.94, 2.84, 13.75};

  S21 = new TGraphAsymmErrors(10,xbin,s21,xl,xh,e21_l,e21_h);
  S21->SetName("Ups1S_2S");
  S21->SetMarkerColor(2);
  S21->SetMarkerStyle(21);
    
  S31 = new TGraphAsymmErrors(10,xbin,s31,xl,xh,e31_l,e31_h);
  S31->SetName("Ups1S_3S");
  S31->SetMarkerColor(4);
  S31->SetMarkerStyle(22);
    
  S21->GetXaxis()->SetTitle("p_{T}^{#Upsilon}(GeV/c)");
  S21->GetYaxis()->SetTitle("Ratio");
  S21->SetTitle("");
  S21->SetMinimum(0.);
  S21->SetMaximum(1.);
  S21->Draw("AP");
  S31->Draw("P");
  legg = new TLegend(0.1,0.6,0.5,0.8);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("");
  legge = legg->AddEntry(S21, "#Upsilon(2S)/#Upsilon(1S)" ,"p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(S31, "#Upsilon(3S)/#Upsilon(1S)","p"); legge->SetTextColor(kBlue);
  legg->Draw();
  c1->SaveAs("Ratio_unpol.pdf");
  
  
  // Table for Ratios
  ofstream OUT("Ratios_unpol.tex");
  OUT << "% ----------------------------------------------------------------------" << endl;
  OUT << "% -- Ratios_unpol" << endl;
  for ( int x = 1; x <= S1h->GetNbinsX(); ++x ){
    OUT << Form("\\vdef{%iSover%iS__bin%iContent_unpol} {\\ensuremath{ {%.2f } } }",2 ,1, x, s21[x-1] ) << endl;
    OUT << Form("\\vdef{%iSover%iS_bin%iErrorHigh_unpol} {\\ensuremath{ {%.2f } } }",2 ,1, x, S21->GetErrorYhigh(x-1) ) << endl;
    OUT << Form("\\vdef{%iSover%iS_bin%iErrorLow_unpol} {\\ensuremath{ {%.2f } } }",2 ,1, x, S21->GetErrorYlow(x-1) ) << endl;
    OUT << Form("\\vdef{%iSover%iS__bin%iContent_unpol} {\\ensuremath{ {%.2f } } }",3 ,1, x, s31[x-1] ) << endl;
    OUT << Form("\\vdef{%iSover%iS_bin%iErrorHigh_unpol} {\\ensuremath{ {%.2f } } }",3 ,1, x, S31->GetErrorYhigh(x-1) ) << endl; 
    OUT << Form("\\vdef{%iSover%iS_bin%iErrorLow_unpol} {\\ensuremath{ {%.2f } } }",3 ,1, x, S31->GetErrorYlow(x-1) ) << endl;
  }
  
  OUT.close();
  
}


void Ratio_pol(){
  
  gStyle->SetOptStat(00000000000);
  TFile *f = new TFile("Final1S.root");
  TH1D *S1h; TH1D *S1l;
  S1h = (TH1D*)gFile->Get("Ups1S_ratioh");
  S1l = (TH1D*)gFile->Get("Ups1S_ratiol");
  TFile *f = new TFile("Final2S.root");
  TH1D *S2h; TH1D *S2l;
  S2h = (TH1D*)gFile->Get("Ups2S_ratioh");
  S2l = (TH1D*)gFile->Get("Ups2S_ratiol");  
  TFile *f = new TFile("Final3S.root");
  TH1D *S3h; TH1D *S3l;
  S3h = (TH1D*)gFile->Get("Ups3S_ratioh");
  S3l = (TH1D*)gFile->Get("Ups3S_ratiol");   
  
  TFile *f = new TFile("XSection_1Srho_HelMi.root");
  TH1D *S1HelMi; S1HelMi = (TH1D*)gFile->Get("S1YieldPt");  
  TFile *f = new TFile("XSection_1Srho_HelPl.root");
  TH1D *S1HelPl; S1HelPl = (TH1D*)gFile->Get("S1YieldPt");   
  TFile *f = new TFile("XSection_2Srho_HelMi.root");
  TH1D *S2HelMi; S2HelMi = (TH1D*)gFile->Get("S2YieldPt");  
  TFile *f = new TFile("XSection_2Srho_HelPl.root");
  TH1D *S2HelPl; S2HelPl = (TH1D*)gFile->Get("S2YieldPt");   
  TFile *f = new TFile("XSection_3Srho_HelMi.root");
  TH1D *S3HelMi; S3HelMi = (TH1D*)gFile->Get("S3YieldPt");  
  TFile *f = new TFile("XSection_3Srho_HelPl.root");
  TH1D *S3HelPl; S3HelPl = (TH1D*)gFile->Get("S3YieldPt"); 
    
  
  double s21[10]; double s31[10]; double e21_h[10]; double e31_h[10]; double e21_l[10]; double e31_l[10];
  double term1_h(0.), term2_h(0.),term3_h(0.), term21_h(0.), term31_h(0.);
  double term1_l(0.), term2_l(0.),term3_l(0.), term21_l(0.), term31_l(0.);
  double polterm1_h(0.), polterm2_h(0.), polterm3_h(0.), polterm1_l(0.), polterm2_l(0.), polterm3_l(0.);
  for (int i = 1; i <= S1h->GetNbinsX(); ++i) {
    
   s21[i-1] = S2h->GetBinContent(i)/S1h->GetBinContent(i);
   s31[i-1] = S3h->GetBinContent(i)/S1h->GetBinContent(i);
   
   polterm1_h = ((S1h->GetBinContent(i)-S1HelPl->GetBinContent(i))/S1h->GetBinContent(i)) * ((S1h->GetBinContent(i)-S1HelPl->GetBinContent(i))/S1h->GetBinContent(i));
   polterm2_h = ((S2h->GetBinContent(i)-S2HelPl->GetBinContent(i))/S2h->GetBinContent(i)) * ((S2h->GetBinContent(i)-S2HelPl->GetBinContent(i))/S2h->GetBinContent(i));
   polterm3_h = ((S3h->GetBinContent(i)-S3HelPl->GetBinContent(i))/S3h->GetBinContent(i)) * ((S3h->GetBinContent(i)-S3HelPl->GetBinContent(i))/S3h->GetBinContent(i));
   
   term1_h = (S1h->GetBinError(i)/S1h->GetBinContent(i))*(S1h->GetBinError(i)/S1h->GetBinContent(i));
   term2_h = (S2h->GetBinError(i)/S2h->GetBinContent(i))*(S2h->GetBinError(i)/S2h->GetBinContent(i));
   term3_h = (S3h->GetBinError(i)/S3h->GetBinContent(i))*(S3h->GetBinError(i)/S3h->GetBinContent(i));
   term21_h = term1_h+term2_h+polterm1_h+polterm2_h;
   term31_h = term1_h+term3_h+polterm1_h+polterm3_h;
   e21_h[i-1] = s21[i-1]*TMath::Sqrt(term21_h);
   e31_h[i-1] = s31[i-1]*TMath::Sqrt(term31_h);
   
   polterm1_l = ((S1h->GetBinContent(i)-S1HelMi->GetBinContent(i))/S1h->GetBinContent(i)) * ((S1h->GetBinContent(i)-S1HelMi->GetBinContent(i))/S1h->GetBinContent(i));
   polterm2_l = ((S2h->GetBinContent(i)-S2HelMi->GetBinContent(i))/S2h->GetBinContent(i)) * ((S2h->GetBinContent(i)-S2HelMi->GetBinContent(i))/S2h->GetBinContent(i));
   polterm3_l = ((S3h->GetBinContent(i)-S3HelMi->GetBinContent(i))/S3h->GetBinContent(i)) * ((S3h->GetBinContent(i)-S3HelMi->GetBinContent(i))/S3h->GetBinContent(i));
   
   term1_l = (S1l->GetBinError(i)/S1l->GetBinContent(i))*(S1l->GetBinError(i)/S1l->GetBinContent(i));
   term2_l = (S2l->GetBinError(i)/S2l->GetBinContent(i))*(S2l->GetBinError(i)/S2l->GetBinContent(i));
   term3_l = (S3l->GetBinError(i)/S3l->GetBinContent(i))*(S3l->GetBinError(i)/S3l->GetBinContent(i));
   term21_l = term1_l+term2_l+polterm1_l+polterm2_l;
   term31_l = term1_l+term3_l+polterm1_l+polterm3_l;
   e21_l[i-1] = s21[i-1]*TMath::Sqrt(term21_l);
   e31_l[i-1] = s31[i-1]*TMath::Sqrt(term31_l);   
   
   cout << e21_h[i-1] << endl; cout << e21_l[i-1] << endl;
   cout << e31_h[i-1] << endl; cout << e31_l[i-1] << endl;
   cout << endl;
  }  
  
  double xbin[10] = {1.27, 2.95, 5.72, 8.92, 11.30, 14.32, 17.73, 22.06, 27.16, 36.25};
  double xl[10] = {1.27, 0.95, 1.72, 0.92, 1.30, 1.32, 1.73, 2.06, 2.16, 6.25};
  double xh[10] = {0.73, 1.05, 2.28, 1.08, 1.70, 1.68, 2.27, 2.94, 2.84, 13.75};
  
  S21 = new TGraphAsymmErrors(10,xbin,s21,xl,xh,e21_l,e21_h);
  S21->SetName("Ups1S_2S");
  S21->SetMarkerColor(2);
  S21->SetMarkerStyle(21);
    
  S31 = new TGraphAsymmErrors(10,xbin,s31,xl,xh,e31_l,e31_h);
  S31->SetName("Ups1S_3S");
  S31->SetMarkerColor(4);
  S31->SetMarkerStyle(22);
    
  S21->GetXaxis()->SetTitle("p_{T}^{#Upsilon}(GeV/c)");
  S21->GetYaxis()->SetTitle("Ratio");
  S21->SetTitle("");
  S21->SetMinimum(0.);
  S21->SetMaximum(1.);
  S21->Draw("AP");
  S31->Draw("P");
  legg = new TLegend(0.1,0.6,0.5,0.8);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("");
  legge = legg->AddEntry(S21, "#Upsilon(2S)/#Upsilon(1S)" ,"p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(S31, "#Upsilon(3S)/#Upsilon(1S)","p"); legge->SetTextColor(kBlue);
  legg->Draw();
  c1->SaveAs("Ratio_pol.pdf");
  
  
  // Table for Ratios
  ofstream OUT("Ratios_pol.tex");
  OUT << "% ----------------------------------------------------------------------" << endl;
  OUT << "% -- Ratios_pol" << endl;
  for ( int x = 1; x <= S1h->GetNbinsX(); ++x ){
    OUT << Form("\\vdef{%iSover%iS__bin%iContent_pol} {\\ensuremath{ {%.2f } } }",2 ,1, x, s21[x-1] ) << endl;
    OUT << Form("\\vdef{%iSover%iS_bin%iErrorHigh_pol} {\\ensuremath{ {%.2f } } }",2 ,1, x, S21->GetErrorYhigh(x-1) ) << endl;
    OUT << Form("\\vdef{%iSover%iS_bin%iErrorLow_pol} {\\ensuremath{ {%.2f } } }",2 ,1, x, S21->GetErrorYlow(x-1) ) << endl;
    OUT << Form("\\vdef{%iSover%iS__bin%iContent_pol} {\\ensuremath{ {%.2f } } }",3 ,1, x, s31[x-1] ) << endl;
    OUT << Form("\\vdef{%iSover%iS_bin%iErrorHigh_pol} {\\ensuremath{ {%.2f } } }",3 ,1, x, S31->GetErrorYhigh(x-1) ) << endl; 
    OUT << Form("\\vdef{%iSover%iS_bin%iErrorLow_pol} {\\ensuremath{ {%.2f } } }",3 ,1, x, S31->GetErrorYlow(x-1) ) << endl;
  }
  
  OUT.close();
  
}
