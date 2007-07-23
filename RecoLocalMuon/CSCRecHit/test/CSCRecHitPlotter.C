void CSCRecHitPlotter(int segtype){

  /* Macro to plot histograms produced by CSCRecHitReader.cc
   * You may need to update the TFile name, and will need to
   * input the segtype as shown below.
   *
   * Author:  Dominique Fortin - UCR
   */

  float nsigmas = 1.5;    // Number of sigmas around mean to fit gaussian.  It uses 2 iterations 
                          // i.e. range is = [mu - nsigmas * sigma, mu + nsigmas * sigma]

TFile *file = TFile::Open("cscrechitplots.root");

// Files for histogram output --> set suffixps to desired file type:  e.g. .eps, .jpg, ...

TString suffixps = ".gif";

TString segment = "ME_All";

 if (segtype == 10) TString segment = "ME_1_a";
 if (segtype == 11) TString segment = "ME_1_b";
 if (segtype == 12) TString segment = "ME_1_2";
 if (segtype == 13) TString segment = "ME_1_3";
 if (segtype == 21) TString segment = "ME_2_1";
 if (segtype == 22) TString segment = "ME_2_2";
 if (segtype == 31) TString segment = "ME_3_1";
 if (segtype == 32) TString segment = "ME_3_2";
 if (segtype == 40) TString segment = "ME_4_1";


 TString plot00 = "rechit_eff_"+suffixps;
 TString plot0 = "matched_pair_"+segment+suffixps;

 TString plot1a = "xrec_"+segment+suffixps;
 TString plot1b = "xsim_"+segment+suffixps;
 TString plot1c = "dx_"+segment+suffixps;
 TString plot1d = "x_pulls_"+segment+suffixps;
 TString plot1e = "yrec_vs_xrec_"+segment+suffixps;
 TString plot1f = "ysim_vs_xsim_"+segment+suffixps;
 TString plot1g = "unmatched_hit_ysim_vs_xsim_"+segment+suffixps;

 TString plot2a = "yrec_"+segment+suffixps; 
 TString plot2b = "ysim_"+segment+suffixps; 
 TString plot2c = "dy_"+segment+suffixps; 
 TString plot2d = "y_pulls_"+segment+suffixps;

 TString plot5a = "dphi_"+segment+suffixps;
 TString plot5b = "dphi_pulls_"+segment+suffixps;
 TString plot5c = "rdphi_"+segment+suffixps;
 TString plot5d = "rdphi_pulls_"+segment+suffixps;
 TString plot5e = "rdphisw_"+segment+suffixps;


// ********************************************************************
// Pointers to histograms
// ********************************************************************

// 0) Matched pair
 hRhiteff        = (TH1F *) file->Get("h0");

// 0) Matched pair
 hHaveMatch        = (TH1F *) file->Get(segment+"_hHaveMatch");

// 1) X
 hRecPositionX     = (TH1F *) file->Get(segment+"_hRecPositionX");
 hSimPositionX     = (TH1F *) file->Get(segment+"_hSimPositionX");
 hResPositionX     = (TH1F *) file->Get(segment+"_hResPositionX");
 hPullX            = (TH1F *) file->Get(segment+"_hPullX");
 hRecYvsX          = (TH2F *) file->Get(segment+"_hRecPositionYvsX");
 hSimYvsX          = (TH2F *) file->Get(segment+"_hSimPositionYvsX");
 hMisYvsX          = (TH2F *) file->Get(segment+"_hMisPositionYvsX");

// 2) Y
 hRecPositionY     = (TH1F *) file->Get(segment+"_hRecPositionY");
 hSimPositionY     = (TH1F *) file->Get(segment+"_hSimPositionY");
 hResPositionY     = (TH1F *) file->Get(segment+"_hResPositionY");
 hPullY            = (TH1F *) file->Get(segment+"_hPullY");

// 5) Phi
 hResphi           = (TH1F *) file->Get(segment+"_hResphi");
 hPullDphi        = (TH1F *) file->Get(segment+"_hPullDphi");
 hrDphi            = (TH1F *) file->Get(segment+"_hrDphi");
 hPullrDphi        = (TH1F *) file->Get(segment+"_hPullrDphi");
 hrDphiSW          = (TH1F *) file->Get(segment+"_hrDphiSW");

// ***************************************************************** 
// Have match
// *****************************************************************

 gStyle->SetOptStat(kTRUE);
 gStyle->SetOptStat(kFALSE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hRhiteff->SetTitle(segment);
 hRhiteff->Draw();
 hRhiteff->GetXaxis()->SetTitle("Chamber type");
 hRhiteff->GetYaxis()->SetTitle("efficiency");
 c1->Print(plot00);


 gStyle->SetOptStat(kTRUE);
 gStyle->SetOptStat(kFALSE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hHaveMatch->SetTitle(segment);
 hHaveMatch->Draw();
 hHaveMatch->GetXaxis()->SetTitle("Sim hits without (0) or with (1) matching rechit");
 c1->Print(plot0);
 
// *****************************************************************
// 1) Local X position
// *****************************************************************

// 1c) delta X
 gStyle->SetOptStat(kTRUE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);   
 c1->SetFillColor(10);
 hResPositionX->SetTitle(segment);
 hResPositionX->Draw();
 hResPositionX->GetXaxis()->SetTitle("#Delta x_{local} (cm)");
 hResPositionX->GetYaxis()->SetTitle(" ");
 c1->Print(plot1c);
 
// 1d) X residual
 gStyle->SetOptStat(kTRUE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hPullX->SetTitle(segment);
 hPullX->Draw();
 hPullX->GetXaxis()->SetTitle("#Delta x_{local}/#sigma x_{reco}");
 hPullX->GetYaxis()->SetTitle(" ");   
 c1->Print(plot1d);
 
// 1e) Yrec vs Xrec
 gStyle->SetOptStat(kFALSE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hRecYvsX->SetTitle(segment);
 hRecYvsX->Draw();
 hRecYvsX->GetXaxis()->SetTitle("x_{reco} (cm) ");
 hRecYvsX->GetYaxis()->SetTitle("y_{reco} (cm) ");
 c1->Print(plot1e);

// 1f) Ysim vs Xsim
 gStyle->SetOptStat(kFALSE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hSimYvsX->SetTitle(segment);
 hSimYvsX->Draw();
 hSimYvsX->GetXaxis()->SetTitle("x_{sim} (cm) ");
 hSimYvsX->GetYaxis()->SetTitle("y_{sim} (cm) ");
 c1->Print(plot1f);

// 1f) Simhits without matching rechits Ysim vs Xsim
 gStyle->SetOptStat(kFALSE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hMisYvsX->SetTitle(segment);
 hMisYvsX->Draw("BOX");
 hMisYvsX->GetXaxis()->SetTitle("x_{sim} (cm) ");
 hMisYvsX->GetYaxis()->SetTitle("y_{sim} (cm) ");
 c1->Print(plot1g);


// *****************************************************************
// 2) Local Y position  
// *****************************************************************

// 2c) Delta y
 gStyle->SetOptStat(kTRUE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);   
 c1->SetFillColor(10);
 hResPositionY->SetTitle(segment);
 hResPositionY->Draw();
 hResPositionY->GetXaxis()->SetTitle("#Delta y_{local} (cm)");
 hResPositionY->GetYaxis()->SetTitle(" ");
 c1->Print(plot2c);

// 2d) Y residual
 gStyle->SetOptStat(kTRUE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);   
 c1->SetFillColor(10);
 hPullY->SetTitle(segment);
 hPullY->Draw();
 hPullY->GetXaxis()->SetTitle("#Delta y_{local}/#sigma y_{reco}");
 hPullY->GetYaxis()->SetTitle(" ");
 c1->Print(plot2d);

// *****************************************************************
// 5) Phi
// *****************************************************************

// 5a) Res phi
 gStyle->SetOptStat(kFALSE);  
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hResphi->SetTitle(segment);
// hResphi->Draw();
 hResphi->GetXaxis()->SetTitle("#Delta #phi  (mrads)");
 hResphi->GetYaxis()->SetTitle(" ");
 hResphi->Fit("gaus");
 TF1 *myfunc = hrDphi->GetFunction("gaus");
 float par0 = gaus->GetParameter(0);
 float par1 = gaus->GetParameter(1);
 float par2 = gaus->GetParameter(2);
 cout << "Parameters are: " << "P0: " << par0
     <<  " P1: " << par1 << " P2: " <<par2 << endl;
 float low = par1 -nsigmas * par2;
 float hi = par1 + nsigmas * par2;
 hResphi->Fit("gaus","R","",low,hi);
 par0 = gaus->GetParameter(0);
 par1 = gaus->GetParameter(1);
 par2 = gaus->GetParameter(2);
 cout << "********* Second fit *********" << endl;
 cout << "Parameters are: " << "P0: " << par0
      <<  " P1: " << par1 << " P2: " << par2 << endl;
 gStyle->SetOptStat(kTRUE);
 gStyle->SetOptFit(0111);
 low = par1 -nsigmas * par2;
 hi = par1 + nsigmas * par2;
 hResphi->Fit("gaus","R","",low,hi);
// hResPhi->Draw();
 c1->Print(plot5a);


// 5b) Pull Delta Phi
 gStyle->SetOptStat(kTRUE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hPullDphi->SetTitle(segment);
 hPullDphi->Draw();
 hPullDphi->GetXaxis()->SetTitle("#Delta #phi/#sigma");
 hPullDphi->GetYaxis()->SetTitle("");
 c1->Print(plot5b);


// 5c) R * Delta Phi
 gStyle->SetOptStat(kFALSE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hrDphi->SetTitle(segment);
 hrDphi->Draw();
 hrDphi->GetXaxis()->SetTitle("r #Delta #phi  (cm)");
 hrDphi->GetYaxis()->SetTitle(" ");
 hrDphi->Fit("gaus");
 TF1 *myfunc = hrDphi->GetFunction("gaus");
 float par0 = gaus->GetParameter(0); 
 float par1 = gaus->GetParameter(1); 
 float par2 = gaus->GetParameter(2); 
 cout << "Parameters are: " << "P0: " << par0  
     <<  " P1: " << par1 << " P2: " <<par2 << endl;
 float low = par1 -nsigmas * par2;
 float hi = par1 + nsigmas * par2;
 hrDphi->Fit("gaus","R","",low,hi);
 par0 = gaus->GetParameter(0); 
 par1 = gaus->GetParameter(1); 
 par2 = gaus->GetParameter(2); 
 cout << "********* Second fit *********" << endl;
 cout << "Parameters are: " << "P0: " << par0  
      <<  " P1: " << par1 << " P2: " << par2 << endl;
 gStyle->SetOptStat(kTRUE);
 gStyle->SetOptFit(0111);
 low = par1 -nsigmas * par2;
 hi = par1 + nsigmas * par2;
 hrDphi->Fit("gaus","R","",low,hi);
 c1->Print(plot5c);


// 5d) Pull R * Delta Phi
 gStyle->SetOptStat(kTRUE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hPullrDphi->SetTitle(segment);
 hPullrDphi->Draw();
 hPullrDphi->GetXaxis()->SetTitle("R #Delta #phi/#sigma");
 hPullrDphi->GetYaxis()->SetTitle("");
 c1->Print(plot5d);


// 5e) R * Delta Phi (SW)
 gStyle->SetOptStat(kFALSE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hrDphiSW->SetTitle(segment);
 hrDphiSW->Draw();
 hrDphiSW->GetXaxis()->SetTitle("r #Delta #phi/strip width");
 hrDphiSW->GetYaxis()->SetTitle(" ");
 hrDphiSW->Fit("gaus");
 TF1 *myfunc = hrDphiSW->GetFunction("gaus");
 float par0 = gaus->GetParameter(0); 
 float par1 = gaus->GetParameter(1); 
 float par2 = gaus->GetParameter(2); 
 cout << "Parameters are: " << "P0: " << par0  
      <<  " P1: " << par1 << " P2: " <<par2 << endl;
 float low = par1 -nsigmas * par2;
 float hi = par1 + nsigmas * par2;
 hrDphiSW->Fit("gaus","R","",low,hi);
 par0 = gaus->GetParameter(0); 
 par1 = gaus->GetParameter(1); 
 par2 = gaus->GetParameter(2); 
 cout << "********* Second fit *********" << endl;
 cout << "Parameters are: " << "P0: " << par0  
      <<  " P1: " << par1 << " P2: " << par2 << endl;
 gStyle->SetOptStat(kTRUE);
 gStyle->SetOptFit(0111);
 low = par1 -nsigmas * par2;
 hi = par1 + nsigmas * par2;
 hrDphiSW->Fit("gaus","R","",low,hi);
 c1->Print(plot5e);



 gROOT->ProcessLine(".q");

}
