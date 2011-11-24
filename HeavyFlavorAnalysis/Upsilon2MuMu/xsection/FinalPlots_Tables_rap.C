#include "TMath.h"
#include <math.h>
#include "TF1.h"
#include "TH1.h"

void XSSystTot_1S(){

  TFile *f = new TFile("MuIDMinus_10ptbins_1Srho_rap_v2.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S1YieldEta");
  TFile *f = new TFile("TrigMinus_10ptbins_1Srho_rap_v2.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S1YieldEta");
  TFile *f = new TFile("SigPDF_10ptbins_1Srho_rap.root");
  TH1D *S3;
  S3 = (TH1D*)gFile->Get("S1YieldEta");
  TFile *f = new TFile("BgPDF_10ptbins_1Srho_rap.root");
  TH1D *S4;
  S4 = (TH1D*)gFile->Get("S1YieldEta");  
  TFile *f = new TFile("XSection1Srho_finalversion.root");
  //TFile *f = new TFile("XSection_lhcb.root");
  TH1D *S5;
  S5 = (TH1D*)gFile->Get("S1YieldEta");    
  TFile *f = new TFile("MuIDPlus_10ptbins_1Srho_rap_v2.root");
  TH1D *S6;
  S6 = (TH1D*)gFile->Get("S1YieldEta");
  TFile *f = new TFile("TrigPlus_10ptbins_1Srho_rap_v2.root");
  TH1D *S7;
  S7 = (TH1D*)gFile->Get("S1YieldEta"); 
  TFile *f = new TFile("3SRho_10ptbins_1Srho_rap.root");
  TH1D *S8;
  S8 = (TH1D*)gFile->Get("S1YieldEta");
  TFile *f = new TFile("NoMSC_10ptbins_1Srho_rap.root");
  TH1D *S9;
  S9 = (TH1D*)gFile->Get("S1YieldEta");  
  TH1D *S100 = (TH1D*)S1->Clone(); 
  TH1D *S200 = (TH1D*)S1->Clone();
  S100->SetName("Ups1S_ratioh");
  S200->SetName("Ups1S_ratiol");
  
  double s1(0.), s5(0.), s2(0.), e5(0.), err5(0.);
  double s1_(0.), s2_(0.), e5_(0.);
  double Stat[6]; double Errh[6]; double Errl[6]; double y[6];
  
  double s0(0.), s0_(0.);
  double Errh_ratio[6]; double Errl_ratio[6]; double y_ratio[6];
  double Errh_lhcb[6]; double Errl_lhcb[6]; double y_lhcb[6];
  for (int i = 1; i <= S1->GetNbinsX(); ++i) {
    
    s1 += S1->GetBinContent(i)*S1->GetBinContent(i); ///Muid
    s1 += S2->GetBinContent(i)*S2->GetBinContent(i); //Trig
    s1 += S4->GetBinContent(i)*S4->GetBinContent(i); //bg
    s1 += S3->GetBinContent(i)*S3->GetBinContent(i); //sig
    s1 += S9->GetBinContent(i)*S9->GetBinContent(i); //msc
    s1_ += S3->GetBinContent(i)*S3->GetBinContent(i); //sig
    s1_ += S4->GetBinContent(i)*S4->GetBinContent(i); //bg
    s1_ += S6->GetBinContent(i)*S6->GetBinContent(i); //Muid
    s1_ += S7->GetBinContent(i)*S7->GetBinContent(i); //Trig
    s1_ += S8->GetBinContent(i)*S8->GetBinContent(i); //rho
    s1_ += S9->GetBinContent(i)*S9->GetBinContent(i); //msc
    cout << " dSigma(Y(1S))/d|y| = "  << S5->GetBinContent(i) << " + " << TMath::Sqrt(s1) << " - " << TMath::Sqrt(s1_) << endl;
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
    s0 += S3->GetBinContent(i)*S3->GetBinContent(i); //sig
    s0 += S9->GetBinContent(i)*S9->GetBinContent(i); //msc
    s0_ += S3->GetBinContent(i)*S3->GetBinContent(i); //sig
    s0_ += S4->GetBinContent(i)*S4->GetBinContent(i); //bg
    s0_ += S8->GetBinContent(i)*S8->GetBinContent(i); //rho
    s0_ += S9->GetBinContent(i)*S9->GetBinContent(i); //msc
    Errh_ratio[i-1] = TMath::Sqrt((s0)+(S5->GetBinError(i)*S5->GetBinError(i)));
    Errl_ratio[i-1] = TMath::Sqrt((s0_)+(S5->GetBinError(i)*S5->GetBinError(i)));
    y_ratio[i-1] = S5->GetBinContent(i);
    
    S100->SetBinContent(i, S5->GetBinContent(i));
    S200->SetBinContent(i, S5->GetBinContent(i));
    S100->SetBinError(i, TMath::Sqrt((s0)+(S5->GetBinError(i)*S5->GetBinError(i))));
    S200->SetBinError(i, TMath::Sqrt((s0_)+(S5->GetBinError(i)*S5->GetBinError(i))));    
    s0=0;s0_=0;
  }
  
  double xbin[6] = {0.208, 0.611, 1.004, 1.395, 1.786, 2.126};
  double xh[6] = {0.192, 0.189, 0.196, 0.205, 0.214, 0.274};
  double xl[6] = {0.208, 0.211, 0.204, 0.195, 0.186, 0.126};
  
  double br_scale(40.3);
  for (int i = 1; i <= S1->GetNbinsX(); ++i) {
    Errh_lhcb[i-1] = Errh[i-1]*br_scale/2;
    Errl_lhcb[i-1] = Errl[i-1]*br_scale/2;
    y_lhcb[i-1] = y[i-1]*br_scale/2;
  }
  
  gr = new TGraphAsymmErrors(6,xbin,y,xl,xh,Errl,Errh);
  gr->SetName("Ups1S");
  gr->SetMarkerColor(1);
  gr->SetMarkerStyle(21);
  gr->Draw("AP");
  
  gr_ratio = new TGraphAsymmErrors(6,xbin,y_ratio,xl,xh,Errl_ratio,Errh_ratio);
  gr_ratio->SetName("Ups1S_ratio");
  gr_ratio->SetMarkerColor(1);
  gr_ratio->SetMarkerStyle(21);
  gr_ratio->Draw("AP");  
  
  gr_lhcb = new TGraphAsymmErrors(6,xbin,y_lhcb,xl,xh,Errl_lhcb,Errh_lhcb);
  gr_lhcb->SetName("Ups1S_LHCb");
  gr_lhcb->SetMarkerColor(1);
  gr_lhcb->SetMarkerStyle(21);
  gr_lhcb->Draw("AP");  
  
  cout << " Y(1S) 1Srho Xsection = "  << s5 << " + " << s2 << " ("  << e5  << ")" << " - " << s2_ << " ("  << e5_  << ")"  <<  endl;
  //TFile *f = new TFile("Final1S_rap_lhcb.root", "RECREATE");
  TFile *f = new TFile("Final1S_rap.root", "RECREATE");
  gr->Write();
  gr_ratio->Write();
  gr_lhcb->Write();
  S100->Write();
  S200->Write();
  
  // Table for 1S Xsection and Errors
  double muid_h(0.), trig_h(0.), sig(0.), muid_l(0.), trig_l(0.), tot_h(0.), tot_l(0.), stat(0.), rho_l(0.), bg(0.), msc(0.); 
  ofstream OUT("1S_rap.tex");
  OUT << "% ----------------------------------------------------------------------" << endl;
  OUT << "% -- 1S_rap" << endl;
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
    tot_h = TMath::Sqrt(muid_h*muid_h + trig_h*trig_h + bg*bg + sig*sig + msc*msc);
    tot_l = TMath::Sqrt(muid_l*muid_l + trig_l*trig_l + sig*sig + bg*bg + rho_l*rho_l + msc*msc);
    OUT << Form("\\vdef{%iS_sigma%i_rap} {\\ensuremath{ {%.3f } } }",1, x, S5->GetBinContent(x)*S5->GetBinWidth(x) ) << endl;
    OUT << Form("\\vdef{%iS_toterror%i_rap} {\\ensuremath{ {%.3f } } }",1, x, S5->GetBinError(x)*S5->GetBinWidth(x) ) << endl;
    OUT << Form("\\vdef{%iS_statoversigma%i_rap} {\\ensuremath{ {%.3f } } }",1, x, stat ) << endl;
    OUT << Form("\\vdef{%iS_systHighoversigma%i_rap} {\\ensuremath{ {%.3f } } }",1, x, tot_h ) << endl;
    OUT << Form("\\vdef{%iS_systLowoversigma%i_rap} {\\ensuremath{ {%.3f } } }",1, x, tot_l ) << endl;
    OUT << Form("\\vdef{%iS_trigHighoversigma%i_rap} {\\ensuremath{ {%.3f } } }",1, x, trig_h ) << endl;
    OUT << Form("\\vdef{%iS_trigLowoversigma%i_rap} {\\ensuremath{ {%.3f } } }",1, x, trig_l ) << endl;
    OUT << Form("\\vdef{%iS_muidHighoversigma%i_rap} {\\ensuremath{ {%.3f } } }",1, x, muid_h ) << endl;
    OUT << Form("\\vdef{%iS_muidLowoversigma%i_rap} {\\ensuremath{ {%.3f } } }",1, x, muid_l ) << endl;    
    OUT << Form("\\vdef{%iS_sigoversigma%i_rap} {\\ensuremath{ {%.3f } } }",1, x, sig ) << endl;
    OUT << Form("\\vdef{%iS_bgoversigma%i_rap} {\\ensuremath{ {%.3f } } }",1, x, bg ) << endl;
    OUT << Form("\\vdef{%iS_rhoLowoversigma%i_rap} {\\ensuremath{ {%.3f } } }",1, x, rho_l ) << endl;
    OUT << Form("\\vdef{%iS_mscoversigma%i_rap} {\\ensuremath{ {%.3f } } }",1, x, msc ) << endl;
  }
  
  OUT.close();
  
}

void XSSystTot_2S(){

  TFile *f = new TFile("MuIDMinus_10ptbins_2Srho_rap_v2.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S2YieldEta");
  TFile *f = new TFile("TrigMinus_10ptbins_2Srho_rap_v2.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S2YieldEta");
  TFile *f = new TFile("SigPDF_10ptbins_2Srho_rap.root");
  TH1D *S3;
  S3 = (TH1D*)gFile->Get("S2YieldEta");
  TFile *f = new TFile("BgPDF_10ptbins_2Srho_rap.root");
  TH1D *S4;
  S4 = (TH1D*)gFile->Get("S2YieldEta");  
  TFile *f = new TFile("XSection2Srho_finalversion.root");
  TH1D *S5;
  S5 = (TH1D*)gFile->Get("S2YieldEta");    
  TFile *f = new TFile("MuIDPlus_10ptbins_2Srho_rap_v2.root");
  TH1D *S6;
  S6 = (TH1D*)gFile->Get("S2YieldEta");
  TFile *f = new TFile("TrigPlus_10ptbins_2Srho_rap_v2.root");
  TH1D *S7;
  S7 = (TH1D*)gFile->Get("S2YieldEta"); 
  TFile *f = new TFile("3SRho_10ptbins_2Srho_rap.root");
  TH1D *S8;
  S8 = (TH1D*)gFile->Get("S2YieldEta");
  TFile *f = new TFile("1SRho_10ptbins_2Srho_rap.root");
  TH1D *S9;
  S9 = (TH1D*)gFile->Get("S2YieldEta");  
  TFile *f = new TFile("NoMSC_10ptbins_2Srho_rap.root");
  TH1D *S10;
  S10 = (TH1D*)gFile->Get("S2YieldEta");    
  TH1D *S100 = (TH1D*)S1->Clone(); 
  TH1D *S200 = (TH1D*)S1->Clone();
  S100->SetName("Ups2S_ratioh");
  S200->SetName("Ups2S_ratiol");
  
  double s1(0.), s5(0.), s2(0.), e5(0.), err5(0.);
  double s1_(0.), s2_(0.), e5_(0.);
  double Stat[6]; double Errh[6]; double Errl[6]; double y[6]; 
  
  double s0(0.), s0_(0.);
  double Errh_ratio[6]; double Errl_ratio[6]; double y_ratio[6]; 
  for (int i = 1; i <= S1->GetNbinsX(); ++i) {
    
    s1 += S1->GetBinContent(i)*S1->GetBinContent(i); //Muid
    s1 += S2->GetBinContent(i)*S2->GetBinContent(i); //Trig
    s1 += S4->GetBinContent(i)*S4->GetBinContent(i); //bg
    s1 += S9->GetBinContent(i)*S9->GetBinContent(i); //rho
    s1 += S3->GetBinContent(i)*S3->GetBinContent(i); //Sig
    s1 += S10->GetBinContent(i)*S10->GetBinContent(i); //msc
    s1_ += S3->GetBinContent(i)*S3->GetBinContent(i); //Sig
    s1_ += S4->GetBinContent(i)*S4->GetBinContent(i); //bg
    s1_ += S6->GetBinContent(i)*S6->GetBinContent(i); //Muid
    s1_ += S7->GetBinContent(i)*S7->GetBinContent(i); //Trig    
    s1_ += S8->GetBinContent(i)*S8->GetBinContent(i); //rho
    s1_ += S10->GetBinContent(i)*S10->GetBinContent(i); //msc
    cout << " dSigma(Y(2S))/d|y| = "  << S5->GetBinContent(i) << " + " << TMath::Sqrt(s1) <<  " - " << TMath::Sqrt(s1_) << endl;
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
    s0 += S9->GetBinContent(i)*S9->GetBinContent(i); //rho
    s0 += S3->GetBinContent(i)*S3->GetBinContent(i); //Sig
    s0 += S10->GetBinContent(i)*S10->GetBinContent(i); //msc
    s0_ += S3->GetBinContent(i)*S3->GetBinContent(i); //Sig
    s0_ += S4->GetBinContent(i)*S4->GetBinContent(i); //bg
    s0_ += S8->GetBinContent(i)*S8->GetBinContent(i); //rho
    s0_ += S10->GetBinContent(i)*S10->GetBinContent(i); //msc
    Errh_ratio[i-1] = TMath::Sqrt((s0)+(S5->GetBinError(i)*S5->GetBinError(i)));
    Errl_ratio[i-1] = TMath::Sqrt((s0_)+(S5->GetBinError(i)*S5->GetBinError(i)));
    y_ratio[i-1] = S5->GetBinContent(i);
    
    S100->SetBinContent(i, S5->GetBinContent(i));
    S200->SetBinContent(i, S5->GetBinContent(i));
    S100->SetBinError(i, TMath::Sqrt((s0)+(S5->GetBinError(i)*S5->GetBinError(i))));
    S200->SetBinError(i, TMath::Sqrt((s0_)+(S5->GetBinError(i)*S5->GetBinError(i))));    
    s0=0;s0_=0;
  }
 
  double xbin[6] = {0.208, 0.611, 1.004, 1.395, 1.786, 2.126};
  double xh[6] = {0.192, 0.189, 0.196, 0.205, 0.214, 0.274};
  double xl[6] = {0.208, 0.211, 0.204, 0.195, 0.186, 0.126};
 
  gr = new TGraphAsymmErrors(6,xbin,y,xl,xh,Errl,Errh);
  gr->SetName("Ups2S");
  gr->SetMarkerColor(4);
  gr->SetMarkerStyle(21);
  gr->Draw("AP");
  
  gr_ratio = new TGraphAsymmErrors(6,xbin,y_ratio,xl,xh,Errl_ratio,Errh_ratio);
  gr_ratio->SetName("Ups2S_ratio");
  gr_ratio->SetMarkerColor(4);
  gr_ratio->SetMarkerStyle(21);
  gr_ratio->Draw("AP");
  
  cout << " Y(2S) 2Srho Xsection = "  << s5 << " + " << s2 << " ("  << e5  << ")" << " - " << s2_ << " ("  << e5_  << ")" <<  endl;
  TFile *f = new TFile("Final2S_rap.root", "RECREATE");
  gr->Write();
  gr_ratio->Write();
  S100->Write();
  S200->Write();

  // Table for 2S Xsection and Errors
  double muid_h(0.), trig_h(0.), sig(0.), muid_l(0.), trig_l(0.), tot_h(0.), tot_l(0.), stat(0.), rho_l(0.), bg(0.), rho_h(0.), msc(0.); 
  ofstream OUT("2S_rap.tex");
  OUT << "% ----------------------------------------------------------------------" << endl;
  OUT << "% -- 2S_rap" << endl;
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
    tot_h = TMath::Sqrt(((muid_h*muid_h) + (trig_h*trig_h) + (bg*bg) + (rho_h*rho_h) + (sig*sig) + (msc*msc)));
    tot_l = TMath::Sqrt((muid_l*muid_l) + (trig_l*trig_l) + (sig*sig) + (bg*bg) + (rho_l*rho_l) + (msc*msc));
    OUT << Form("\\vdef{%iS_sigma%i_rap} {\\ensuremath{ {%.3f } } }",2, x, S5->GetBinContent(x)*S5->GetBinWidth(x) ) << endl;
    OUT << Form("\\vdef{%iS_toterror%i_rap} {\\ensuremath{ {%.3f } } }",2, x, S5->GetBinError(x)*S5->GetBinWidth(x) ) << endl;
    OUT << Form("\\vdef{%iS_statoversigma%i_rap} {\\ensuremath{ {%.3f } } }",2, x, stat ) << endl;
    OUT << Form("\\vdef{%iS_systHighoversigma%i_rap} {\\ensuremath{ {%.3f } } }",2, x, tot_h ) << endl;
    OUT << Form("\\vdef{%iS_systLowoversigma%i_rap} {\\ensuremath{ {%.3f } } }",2, x, tot_l ) << endl;
    OUT << Form("\\vdef{%iS_trigHighoversigma%i_rap} {\\ensuremath{ {%.3f } } }",2, x, trig_h ) << endl;
    OUT << Form("\\vdef{%iS_trigLowoversigma%i_rap} {\\ensuremath{ {%.3f } } }",2, x, trig_l ) << endl;
    OUT << Form("\\vdef{%iS_muidHighoversigma%i_rap} {\\ensuremath{ {%.3f } } }",2, x, muid_h ) << endl;
    OUT << Form("\\vdef{%iS_muidLowoversigma%i_rap} {\\ensuremath{ {%.3f } } }",2, x, muid_l ) << endl;    
    OUT << Form("\\vdef{%iS_sigoversigma%i_rap} {\\ensuremath{ {%.3f } } }",2, x, sig ) << endl;
    OUT << Form("\\vdef{%iS_bgoversigma%i_rap} {\\ensuremath{ {%.3f } } }",2, x, bg ) << endl;
    OUT << Form("\\vdef{%iS_rhoLowoversigma%i_rap} {\\ensuremath{ {%.3f } } }",2, x, rho_l ) << endl;
    OUT << Form("\\vdef{%iS_rhoHighoversigma%i_rap} {\\ensuremath{ {%.3f } } }",2, x, rho_h ) << endl;
  }
  
  OUT.close();
  
}

void XSSystTot_3S(){

  TFile *f = new TFile("MuIDMinus_10ptbins_3Srho_rap_v2.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S3YieldEta");
  TFile *f = new TFile("TrigMinus_10ptbins_3Srho_rap_v2.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S3YieldEta");
  TFile *f = new TFile("SigPDF_10ptbins_3Srho_rap.root");
  TH1D *S3;
  S3 = (TH1D*)gFile->Get("S3YieldEta");
  TFile *f = new TFile("BgPDF_10ptbins_3Srho_rap.root");
  TH1D *S4;
  S4 = (TH1D*)gFile->Get("S3YieldEta");  
  TFile *f = new TFile("XSection3Srho_finalversion.root");
  TH1D *S5;
  S5 = (TH1D*)gFile->Get("S3YieldEta");    
  TFile *f = new TFile("MuIDPlus_10ptbins_3Srho_rap_v2.root");
  TH1D *S6;
  S6 = (TH1D*)gFile->Get("S3YieldEta");
  TFile *f = new TFile("TrigPlus_10ptbins_3Srho_rap_v2.root");
  TH1D *S7;
  S7 = (TH1D*)gFile->Get("S3YieldEta"); 
  TFile *f = new TFile("1SRho_10ptbins_3Srho_rap.root");
  TH1D *S8;
  S8 = (TH1D*)gFile->Get("S3YieldEta");
  TFile *f = new TFile("NoMSC_10ptbins_3Srho_rap.root");
  TH1D *S9;
  S9 = (TH1D*)gFile->Get("S3YieldEta");  
  TH1D *S100 = (TH1D*)S1->Clone(); 
  TH1D *S200 = (TH1D*)S1->Clone();
  S100->SetName("Ups3S_ratioh");
  S200->SetName("Ups3S_ratiol");
  
  double s1(0.), s5(0.), s2(0.), e5(0.), err5(0.);
  double s1_(0.), s2_(0.), e5_(0.);
  double Stat[6]; double Errh[6]; double Errl[6]; double y[6];

  double s0(0.), s0_(0.);
  double Errh_ratio[6]; double Errl_ratio[6]; double y_ratio[6]; 
  for (int i = 1; i <= S1->GetNbinsX(); ++i) {
    
    s1 += S1->GetBinContent(i)*S1->GetBinContent(i); //Muid
    s1 += S2->GetBinContent(i)*S2->GetBinContent(i); //Trig
    s1 += S4->GetBinContent(i)*S4->GetBinContent(i); //bg
    s1 += S8->GetBinContent(i)*S8->GetBinContent(i); //rho
    s1 += S9->GetBinContent(i)*S9->GetBinContent(i); //msc
    s1  += S3->GetBinContent(i)*S3->GetBinContent(i); //Sig
    s1_ += S3->GetBinContent(i)*S3->GetBinContent(i); //Sig
    s1_ += S4->GetBinContent(i)*S4->GetBinContent(i); //bg
    s1_ += S6->GetBinContent(i)*S6->GetBinContent(i); //Muid
    s1_ += S7->GetBinContent(i)*S7->GetBinContent(i); //Trig
    s1_ += S9->GetBinContent(i)*S9->GetBinContent(i); //msc
    cout << " dSigma(Y(3S))/d|y| = "  << S5->GetBinContent(i) << " + " << TMath::Sqrt(s1) << " - " << TMath::Sqrt(s1_) << endl;
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
    s0 += S8->GetBinContent(i)*S8->GetBinContent(i); //rho
    s0 += S3->GetBinContent(i)*S3->GetBinContent(i); //Sig
    s0 += S9->GetBinContent(i)*S9->GetBinContent(i); //msc
    s0_ += S3->GetBinContent(i)*S3->GetBinContent(i); //Sig    
    s0_ += S4->GetBinContent(i)*S4->GetBinContent(i); //bg
    s0_ += S9->GetBinContent(i)*S9->GetBinContent(i); //msc
    Errh_ratio[i-1] = TMath::Sqrt((s0)+(S5->GetBinError(i)*S5->GetBinError(i)));
    Errl_ratio[i-1] = TMath::Sqrt((s0_)+(S5->GetBinError(i)*S5->GetBinError(i)));
    y_ratio[i-1] = S5->GetBinContent(i);
    
    S100->SetBinContent(i, S5->GetBinContent(i));
    S200->SetBinContent(i, S5->GetBinContent(i));
    S100->SetBinError(i, TMath::Sqrt((s0)+(S5->GetBinError(i)*S5->GetBinError(i))));
    S200->SetBinError(i, TMath::Sqrt((s0_)+(S5->GetBinError(i)*S5->GetBinError(i))));    
    s0=0;s0_=0;	
    
  }
    
  double xbin[6] = {0.208, 0.611, 1.004, 1.395, 1.786, 2.126};
  double xh[6] = {0.192, 0.189, 0.196, 0.205, 0.214, 0.274};
  double xl[6] = {0.208, 0.211, 0.204, 0.195, 0.186, 0.126};

  gr = new TGraphAsymmErrors(6,xbin,y,xl,xh,Errl,Errh);
  gr->SetName("Ups3S");
  gr->SetMarkerColor(2);
  gr->SetMarkerStyle(21);
  gr->Draw("AP");
  
  gr_ratio = new TGraphAsymmErrors(6,xbin,y_ratio,xl,xh,Errl_ratio,Errh_ratio);
  gr_ratio->SetName("Ups3S_ratio");
  gr_ratio->SetMarkerColor(2);
  gr_ratio->SetMarkerStyle(21);
  gr_ratio->Draw("AP");
  
  cout << " Y(3S) 3Srho Xsection = "  << s5 << " + " << s2 << " ("  << e5  << ")" << " - " << s2_ << " ("  << e5_ << ")" <<  endl;
  TFile *f = new TFile("Final3S_rap.root", "RECREATE");
  gr->Write();
  gr_ratio->Write();
  S100->Write();
  S200->Write();
  
  // Table for 3S Xsection and Errors
  double muid_h(0.), trig_h(0.), sig(0.), muid_l(0.), trig_l(0.), tot_h(0.), tot_l(0.), stat(0.), rho_h(0.), bg(0.), msc(0.); 
  ofstream OUT("3S_rap.tex");
  OUT << "% ----------------------------------------------------------------------" << endl;
  OUT << "% -- 3S_rap" << endl;
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
    tot_h = TMath::Sqrt(muid_h*muid_h + trig_h*trig_h + bg*bg + rho_h*rho_h + sig*sig + msc*msc);
    tot_l = TMath::Sqrt(muid_l*muid_l + trig_l*trig_l + sig*sig + bg*bg + msc*msc);
    OUT << Form("\\vdef{%iS_sigma%i_rap} {\\ensuremath{ {%.3f } } }",3, x, S5->GetBinContent(x)*S5->GetBinWidth(x) ) << endl;
    OUT << Form("\\vdef{%iS_toterror%i_rap} {\\ensuremath{ {%.3f } } }",3, x, S5->GetBinError(x)*S5->GetBinWidth(x) ) << endl;
    OUT << Form("\\vdef{%iS_statoversigma%i_rap} {\\ensuremath{ {%.3f } } }",3, x, stat ) << endl;
    OUT << Form("\\vdef{%iS_systHighoversigma%i_rap} {\\ensuremath{ {%.3f } } }",3, x, tot_h ) << endl;
    OUT << Form("\\vdef{%iS_systLowoversigma%i_rap} {\\ensuremath{ {%.3f } } }",3, x, tot_l ) << endl;
    OUT << Form("\\vdef{%iS_trigHighoversigma%i_rap} {\\ensuremath{ {%.3f } } }",3, x, trig_h ) << endl;
    OUT << Form("\\vdef{%iS_trigLowoversigma%i_rap} {\\ensuremath{ {%.3f } } }",3, x, trig_l ) << endl;
    OUT << Form("\\vdef{%iS_muidHighoversigma%i_rap} {\\ensuremath{ {%.3f } } }",3, x, muid_h ) << endl;
    OUT << Form("\\vdef{%iS_muidLowoversigma%i_rap} {\\ensuremath{ {%.3f } } }",3, x, muid_l ) << endl;    
    OUT << Form("\\vdef{%iS_sigoversigma%i_rap} {\\ensuremath{ {%.3f } } }",3, x, sig ) << endl;
    OUT << Form("\\vdef{%iS_bgoversigma%i_rap} {\\ensuremath{ {%.3f } } }",3, x, bg ) << endl;
    OUT << Form("\\vdef{%iS_rhoHighoversigma%i_rap} {\\ensuremath{ {%.3f } } }",3, x, rho_h ) << endl;
    OUT << Form("\\vdef{%iS_mscoversigma%i_rap} {\\ensuremath{ {%.3f } } }",3, x, msc ) << endl;
  }
  
  OUT.close();
  
}

void XSSystPol_1S(){

  TFile *f = new TFile("XSection_1Srho_CSMi.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S1YieldEta");
  TFile *f = new TFile("XSection_1Srho_CSPl.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S1YieldEta");
  TFile *f = new TFile("XSection_1Srho_HelMi.root");
  TH1D *S3;
  S3 = (TH1D*)gFile->Get("S1YieldEta");
  TFile *f = new TFile("XSection_1Srho_HelPl.root");
  TH1D *S4;
  S4 = (TH1D*)gFile->Get("S1YieldEta");  
  TFile *f = new TFile("XSection1Srho_finalversion.root");
  TH1D *S5;
  S5 = (TH1D*)gFile->Get("S1YieldEta");    
    
  // Table for Systmetical Uncertainty from Polarization
  double csmi(0.), cspl(0.), helmi(0.), helpl(0.);
  ofstream OUT("Pol1S_rap.tex");
  OUT << "% ----------------------------------------------------------------------" << endl;
  OUT << "% -- Polarization Rapidity" << endl;
  for ( int x = 1; x <= S5->GetNbinsX(); ++x ){
    csmi = (S1->GetBinContent(x) - S5->GetBinContent(x))/S5->GetBinContent(x);
    cspl = (S2->GetBinContent(x) - S5->GetBinContent(x))/S5->GetBinContent(x);
    helmi = (S3->GetBinContent(x) - S5->GetBinContent(x))/S5->GetBinContent(x);
    helpl = (S4->GetBinContent(x) - S5->GetBinContent(x))/S5->GetBinContent(x);    
    OUT << Form("\\vdef{%iS_csmi%i_rap} {\\ensuremath{ {%.3f } } }",1, x, csmi ) << endl;
    OUT << Form("\\vdef{%iS_cspl%i_rap} {\\ensuremath{ {%.3f } } }",1, x, cspl ) << endl;
    OUT << Form("\\vdef{%iS_helmi%i_rap} {\\ensuremath{ {%.3f } } }",1, x, helmi ) << endl;
    OUT << Form("\\vdef{%iS_helpl%i_rap} {\\ensuremath{ {%.3f } } }",1, x, helpl ) << endl;
  }
  
  OUT.close();
  
}

void XSSystPol_2S(){

  TFile *f = new TFile("XSection_2Srho_CSMi.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S2YieldEta");
  TFile *f = new TFile("XSection_2Srho_CSPl.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S2YieldEta");
  TFile *f = new TFile("XSection_2Srho_HelMi.root");
  TH1D *S3;
  S3 = (TH1D*)gFile->Get("S2YieldEta");
  TFile *f = new TFile("XSection_2Srho_HelPl.root");
  TH1D *S4;
  S4 = (TH1D*)gFile->Get("S2YieldEta");  
  TFile *f = new TFile("XSection2Srho_finalversion.root");
  TH1D *S5;
  S5 = (TH1D*)gFile->Get("S2YieldEta");    
    
  // Table for Systmetical Uncertainty from Polarization
  double csmi(0.), cspl(0.), helmi(0.), helpl(0.);
  ofstream OUT("Pol2S_rap.tex");
  OUT << "% ----------------------------------------------------------------------" << endl;
  OUT << "% -- Polarization Rapidity" << endl;
  for ( int x = 1; x <= S5->GetNbinsX(); ++x ){
    csmi = (S1->GetBinContent(x) - S5->GetBinContent(x))/S5->GetBinContent(x);
    cspl = (S2->GetBinContent(x) - S5->GetBinContent(x))/S5->GetBinContent(x);
    helmi = (S3->GetBinContent(x) - S5->GetBinContent(x))/S5->GetBinContent(x);
    helpl = (S4->GetBinContent(x) - S5->GetBinContent(x))/S5->GetBinContent(x);    
    OUT << Form("\\vdef{%iS_csmi%i_rap} {\\ensuremath{ {%.3f } } }",2, x, csmi ) << endl;
    OUT << Form("\\vdef{%iS_cspl%i_rap} {\\ensuremath{ {%.3f } } }",2, x, cspl ) << endl;
    OUT << Form("\\vdef{%iS_helmi%i_rap} {\\ensuremath{ {%.3f } } }",2, x, helmi ) << endl;
    OUT << Form("\\vdef{%iS_helpl%i_rap} {\\ensuremath{ {%.3f } } }",2, x, helpl ) << endl;
  }
  
  OUT.close();
  
}

void XSSystPol_3S(){

  TFile *f = new TFile("XSection_3Srho_CSMi.root");
  TH1D *S1;
  S1 = (TH1D*)gFile->Get("S3YieldEta");
  TFile *f = new TFile("XSection_3Srho_CSPl.root");
  TH1D *S2;
  S2 = (TH1D*)gFile->Get("S3YieldEta");
  TFile *f = new TFile("XSection_3Srho_HelMi.root");
  TH1D *S3;
  S3 = (TH1D*)gFile->Get("S3YieldEta");
  TFile *f = new TFile("XSection_3Srho_HelPl.root");
  TH1D *S4;
  S4 = (TH1D*)gFile->Get("S3YieldEta");  
  TFile *f = new TFile("XSection3Srho_finalversion.root");
  TH1D *S5;
  S5 = (TH1D*)gFile->Get("S3YieldEta");    
    
  // Table for Systmetical Uncertainty from Polarization
  double csmi(0.), cspl(0.), helmi(0.), helpl(0.);
  ofstream OUT("Pol3S_rap.tex");
  OUT << "% ----------------------------------------------------------------------" << endl;
  OUT << "% -- Polarization Rapidity" << endl;
  for ( int x = 1; x <= S5->GetNbinsX(); ++x ){
    csmi = (S1->GetBinContent(x) - S5->GetBinContent(x))/S5->GetBinContent(x);
    cspl = (S2->GetBinContent(x) - S5->GetBinContent(x))/S5->GetBinContent(x);
    helmi = (S3->GetBinContent(x) - S5->GetBinContent(x))/S5->GetBinContent(x);
    helpl = (S4->GetBinContent(x) - S5->GetBinContent(x))/S5->GetBinContent(x);    
    OUT << Form("\\vdef{%iS_csmi%i_rap} {\\ensuremath{ {%.3f } } }",3, x, csmi ) << endl;
    OUT << Form("\\vdef{%iS_cspl%i_rap} {\\ensuremath{ {%.3f } } }",3, x, cspl ) << endl;
    OUT << Form("\\vdef{%iS_helmi%i_rap} {\\ensuremath{ {%.3f } } }",3, x, helmi ) << endl;
    OUT << Form("\\vdef{%iS_helpl%i_rap} {\\ensuremath{ {%.3f } } }",3, x, helpl ) << endl;
  }
  
  OUT.close();
  
}

void Overlay(){
  
  gStyle->SetOptStat(00000000000);
  TFile *f = new TFile("Final1S_rap.root");
  TGraphAsymmErrors *S1;
  S1 = (TGraphAsymmErrors*)gFile->Get("Ups1S");
  TFile *f = new TFile("Final2S_rap.root");
  TGraphAsymmErrors *S2;
  S2 = (TGraphAsymmErrors*)gFile->Get("Ups2S");
  TFile *f = new TFile("Final3S_rap.root");
  TGraphAsymmErrors *S3;
  S3 = (TGraphAsymmErrors*)gFile->Get("Ups3S");  
    
  TCanvas *c1 = new TCanvas("c1", "c1", 800,600);
  //c1->SetLogy();
  S1->SetLineColor(1); S1->SetMarkerColor(1); S2->SetLineColor(2); S2->SetMarkerColor(2); S3->SetLineColor(4); S3->SetMarkerColor(4);
  S1->SetMarkerStyle(20); S2->SetMarkerStyle(21); S3->SetMarkerStyle(22); 
  S1->GetXaxis()->SetTitle("|y^{#Upsilon}|");
  S1->GetYaxis()->SetTitle("d#sigma/d|y|#times Br(#mu#mu)");
  S1->SetTitle("");
  S1->SetMinimum(0.1);
  S1->SetMaximum(5.);
  S1->Draw("AP");
  S2->Draw("P");
  S3->Draw("P");
  legg = new TLegend(0.7,0.7,0.9,0.9);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("");
  legge = legg->AddEntry(S1, "#Upsilon(1S)" ,"p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(S2, "#Upsilon(2S)","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(S3, "#Upsilon(3S)","p"); legge->SetTextColor(kBlue);
  legg->Draw();
  c1->SaveAs("Overlay_log_rap.pdf");
}

void Ratio_unpol(){
  
  gStyle->SetOptStat(00000000000);
  TFile *f = new TFile("Final1S_rap.root");
  TH1D *S1h; TH1D *S1l;
  S1h = (TH1D*)gFile->Get("Ups1S_ratioh");
  S1l = (TH1D*)gFile->Get("Ups1S_ratiol");
  TFile *f = new TFile("Final2S_rap.root");
  TH1D *S2h; TH1D *S2l;
  S2h = (TH1D*)gFile->Get("Ups2S_ratioh");
  S2l = (TH1D*)gFile->Get("Ups2S_ratiol");  
  TFile *f = new TFile("Final3S_rap.root");
  TH1D *S3h; TH1D *S3l;
  S3h = (TH1D*)gFile->Get("Ups3S_ratioh");
  S3l = (TH1D*)gFile->Get("Ups3S_ratiol");   
  
  double s21[6]; double s31[6]; double e21_h[6]; double e31_h[6]; double e21_l[6]; double e31_l[6];
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
  
  double xbin[6] = {0.208, 0.611, 1.004, 1.395, 1.786, 2.126};
  double xh[6] = {0.192, 0.189, 0.196, 0.205, 0.214, 0.274};
  double xl[6] = {0.208, 0.211, 0.204, 0.195, 0.186, 0.126};

  S21 = new TGraphAsymmErrors(6,xbin,s21,xl,xh,e21_l,e21_h);
  S21->SetName("Ups1S_2S");
  S21->SetMarkerColor(2);
  S21->SetMarkerStyle(21);
    
  S31 = new TGraphAsymmErrors(6,xbin,s31,xl,xh,e31_l,e31_h);
  S31->SetName("Ups1S_3S");
  S31->SetMarkerColor(4);
  S31->SetMarkerStyle(22);
    
  S21->GetXaxis()->SetTitle("|y^{#Upsilon}|");
  S21->GetYaxis()->SetTitle("Ratio");
  S21->SetTitle("");
  S21->SetMinimum(0.);
  S21->SetMaximum(0.6);
  S21->Draw("AP");
  S31->Draw("P");
  legg = new TLegend(0.1,0.6,0.5,0.8);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("");
  legge = legg->AddEntry(S21, "#Upsilon(2S)/#Upsilon(1S)" ,"p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(S31, "#Upsilon(3S)/#Upsilon(1S)","p"); legge->SetTextColor(kBlue);
  legg->Draw();
  c1->SaveAs("Ratio_unpol_rap.pdf");
  
  
  // Table for Ratios
  ofstream OUT("Ratios_unpol_rap.tex");
  OUT << "% ----------------------------------------------------------------------" << endl;
  OUT << "% -- Ratios_unpol" << endl;
  for ( int x = 1; x <= S1h->GetNbinsX(); ++x ){
    OUT << Form("\\vdef{%iSover%iS__bin%iContent_unpol_rap} {\\ensuremath{ {%.2f } } }",2 ,1, x, s21[x-1] ) << endl;
    OUT << Form("\\vdef{%iSover%iS_bin%iErrorHigh_unpol_rap} {\\ensuremath{ {%.2f } } }",2 ,1, x, S21->GetErrorYhigh(x-1) ) << endl;
    OUT << Form("\\vdef{%iSover%iS_bin%iErrorLow_unpol_rap} {\\ensuremath{ {%.2f } } }",2 ,1, x, S21->GetErrorYlow(x-1) ) << endl;
    OUT << Form("\\vdef{%iSover%iS__bin%iContent_unpol_rap} {\\ensuremath{ {%.2f } } }",3 ,1, x, s31[x-1] ) << endl;
    OUT << Form("\\vdef{%iSover%iS_bin%iErrorHigh_unpol_rap} {\\ensuremath{ {%.2f } } }",3 ,1, x, S31->GetErrorYhigh(x-1) ) << endl; 
    OUT << Form("\\vdef{%iSover%iS_bin%iErrorLow_unpol_rap} {\\ensuremath{ {%.2f } } }",3 ,1, x, S31->GetErrorYlow(x-1) ) << endl;
  }
  
  OUT.close();
  
}

void Ratio_pol(){
  
  gStyle->SetOptStat(00000000000);
  TFile *f = new TFile("Final1S_rap.root");
  TH1D *S1h; TH1D *S1l;
  S1h = (TH1D*)gFile->Get("Ups1S_ratioh");
  S1l = (TH1D*)gFile->Get("Ups1S_ratiol");
  TFile *f = new TFile("Final2S_rap.root");
  TH1D *S2h; TH1D *S2l;
  S2h = (TH1D*)gFile->Get("Ups2S_ratioh");
  S2l = (TH1D*)gFile->Get("Ups2S_ratiol");  
  TFile *f = new TFile("Final3S_rap.root");
  TH1D *S3h; TH1D *S3l;
  S3h = (TH1D*)gFile->Get("Ups3S_ratioh");
  S3l = (TH1D*)gFile->Get("Ups3S_ratiol");   
  
  TFile *f = new TFile("Syst/10ptbins/Rho/XSection_10ptbins_1Srho.root");
  TH1D *S5; S5 = (TH1D*)gFile->Get("S1YieldEta");
  TFile *f = new TFile("XSection_1Srho_HelMi.root");
  TH1D *S1HelMi; S1HelMi = (TH1D*)gFile->Get("S1YieldEta");  
  TFile *f = new TFile("XSection_1Srho_HelPl.root");
  TH1D *S1HelPl; S1HelPl = (TH1D*)gFile->Get("S1YieldEta");   
  TFile *f = new TFile("XSection_2Srho_HelMi.root");
  TH1D *S2HelMi; S2HelMi = (TH1D*)gFile->Get("S2YieldEta");  
  TFile *f = new TFile("XSection_2Srho_HelPl.root");
  TH1D *S2HelPl; S2HelPl = (TH1D*)gFile->Get("S2YieldEta");   
  TFile *f = new TFile("XSection_3Srho_HelMi.root");
  TH1D *S3HelMi; S3HelMi = (TH1D*)gFile->Get("S3YieldEta");  
  TFile *f = new TFile("XSection_3Srho_HelPl.root");
  TH1D *S3HelPl; S3HelPl = (TH1D*)gFile->Get("S3YieldEta"); 
    
  
  double s21[6]; double s31[6]; double e21_h[6]; double e31_h[6]; double e21_l[6]; double e31_l[6];
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
  
  double xbin[6] = {0.208, 0.611, 1.004, 1.395, 1.786, 2.126};
  double xh[6] = {0.192, 0.189, 0.196, 0.205, 0.214, 0.274};
  double xl[6] = {0.208, 0.211, 0.204, 0.195, 0.186, 0.126};

  S21 = new TGraphAsymmErrors(6,xbin,s21,xl,xh,e21_l,e21_h);
  S21->SetName("Ups1S_2S");
  S21->SetMarkerColor(2);
  S21->SetMarkerStyle(21);
    
  S31 = new TGraphAsymmErrors(6,xbin,s31,xl,xh,e31_l,e31_h);
  S31->SetName("Ups1S_3S");
  S31->SetMarkerColor(4);
  S31->SetMarkerStyle(22);
    
  S21->GetXaxis()->SetTitle("|y^{#Upsilon}|");
  S21->GetYaxis()->SetTitle("Ratio");
  S21->SetTitle("");
  S21->SetMinimum(0.);
  S21->SetMaximum(0.6);
  S21->Draw("AP");
  S31->Draw("P");
  legg = new TLegend(0.1,0.6,0.5,0.8);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("");
  legge = legg->AddEntry(S21, "#Upsilon(2S)/#Upsilon(1S)" ,"p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(S31, "#Upsilon(3S)/#Upsilon(1S)","p"); legge->SetTextColor(kBlue);
  legg->Draw();
  c1->SaveAs("Ratio_pol_rap.pdf");
  
  
  // Table for Ratios
  ofstream OUT("Ratios_pol_rap.tex");
  OUT << "% ----------------------------------------------------------------------" << endl;
  OUT << "% -- Ratios_pol_rap" << endl;
  for ( int x = 1; x <= S1h->GetNbinsX(); ++x ){
    OUT << Form("\\vdef{%iSover%iS__bin%iContent_pol_rap} {\\ensuremath{ {%.2f } } }",2 ,1, x, s21[x-1] ) << endl;
    OUT << Form("\\vdef{%iSover%iS_bin%iErrorHigh_pol_rap} {\\ensuremath{ {%.2f } } }",2 ,1, x, S21->GetErrorYhigh(x-1) ) << endl;
    OUT << Form("\\vdef{%iSover%iS_bin%iErrorLow_pol_rap} {\\ensuremath{ {%.2f } } }",2 ,1, x, S21->GetErrorYlow(x-1) ) << endl;
    OUT << Form("\\vdef{%iSover%iS__bin%iContent_pol_rap} {\\ensuremath{ {%.2f } } }",3 ,1, x, s31[x-1] ) << endl;
    OUT << Form("\\vdef{%iSover%iS_bin%iErrorHigh_pol_rap} {\\ensuremath{ {%.2f } } }",3 ,1, x, S31->GetErrorYhigh(x-1) ) << endl; 
    OUT << Form("\\vdef{%iSover%iS_bin%iErrorLow_pol_rap} {\\ensuremath{ {%.2f } } }",3 ,1, x, S31->GetErrorYlow(x-1) ) << endl;
  }
  
  OUT.close();
  
}


