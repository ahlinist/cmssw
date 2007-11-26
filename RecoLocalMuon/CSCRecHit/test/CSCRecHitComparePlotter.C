void CSCRecHitComparePlotter(int segtype){

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

TString suffixps = ".eps";

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

 TString plot6a = "chi2all_"+segment+suffixps;
 TString plot6b = "chi2Core_"+segment+suffixps;
 TString plot6c = "chi2Out_"+segment+suffixps;
 TString plot6d = "pull_vs_chi2_box_"+segment+suffixps;
 TString plot6e = "pull_vs_chi2_profile_"+segment+suffixps;

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
 hPullDphi         = (TH1F *) file->Get(segment+"_hPullDphi");
 hrDphi            = (TH1F *) file->Get(segment+"_hrDphi");
 hPullrDphi        = (TH1F *) file->Get(segment+"_hPullrDphi");
 hrDphiSW          = (TH1F *) file->Get(segment+"_hrDphiSW");

// 6) chi^2 study
 hChi2All          = (TH1F *) file->Get(segment+"_hChi2All");
 hChi2Core         = (TH1F *) file->Get(segment+"_hChi2Core");
 hChi2Out          = (TH1F *) file->Get(segment+"_hChi2Out");
 hPullvsChi2       = (TH2F *) file->Get(segment+"_hPullvsChi2");

// *****************************************************************
// 1) Local X position
// *****************************************************************

// 1c) delta X
 gStyle->SetOptStat(kTRUE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);   
 c1->SetFillColor(10);
 hResPositionX->SetTitle(segment);
 hResPositionX->SetFillColor(1);
 hResPositionX->SetFillStyle(3004);
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
 hPullX->SetFillColor(1);
 hPullX->SetFillStyle(3004);
 hPullX->Draw();
 hPullX->GetXaxis()->SetTitle("#Delta x_{local}/#sigma x_{reco}");
 hPullX->GetYaxis()->SetTitle(" ");   
 c1->Print(plot1d);

// *****************************************************************
// 2) Local Y position  
// *****************************************************************

// 2c) Delta y
 gStyle->SetOptStat(kTRUE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);   
 c1->SetFillColor(10);
 hResPositionY->SetTitle(segment);
 hResPositionY->SetFillColor(1);
 hResPositionY->SetFillStyle(3004);
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
 hPullY->SetFillColor(1);
 hPullY->SetFillStyle(3004);
 hPullY->Draw();
 hPullY->GetXaxis()->SetTitle("#Delta y_{local}/#sigma y_{reco}");
 hPullY->GetYaxis()->SetTitle(" ");
 c1->Print(plot2d);

// *****************************************************************
// 5) Phi
// *****************************************************************

// 5a) Res phi
 gStyle->SetOptStat(kTRUE);  
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hResphi->SetTitle(segment);
 hResphi->SetFillColor(1);
 hResphi->SetFillStyle(3004);
 hResphi->Draw();
 c1->Print(plot5a);


// 5b) Pull Delta Phi
 gStyle->SetOptStat(kTRUE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hPullDphi->SetTitle(segment);
 hPullDphi->SetFillColor(1);
 hPullDphi->SetFillStyle(3004);
 hPullDphi->Draw();
 hPullDphi->GetXaxis()->SetTitle("#Delta #phi/#sigma");
 hPullDphi->GetYaxis()->SetTitle("");
 c1->Print(plot5b);


// 5c) R * Delta Phi
 gStyle->SetOptStat(kTRUE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hrDphi->SetTitle(segment);
 hrDphi->SetFillColor(1);
 hrDphi->SetFillStyle(3004);
 hrDphi->Draw();
 hrDphi->GetXaxis()->SetTitle("r #Delta #phi  (cm)");
 hrDphi->GetYaxis()->SetTitle(" ");
 c1->Print(plot5c);


// 5d) Pull R * Delta Phi
 gStyle->SetOptStat(kTRUE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hPullrDphi->SetTitle(segment);
 hPullrDphi->SetFillColor(1);
 hPullrDphi->SetFillStyle(3004);
 hPullrDphi->Draw();
 hPullrDphi->GetXaxis()->SetTitle("R #Delta #phi/#sigma");
 hPullrDphi->GetYaxis()->SetTitle("");
 c1->Print(plot5d);


// 5e) R * Delta Phi (SW)
 gStyle->SetOptStat(kTRUE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hrDphiSW->SetTitle(segment);
 hrDphiSW->SetFillColor(1);
 hrDphiSW->SetFillStyle(3004);
 hrDphiSW->Draw();
 hrDphiSW->GetXaxis()->SetTitle("r #Delta #phi/strip width");
 hrDphiSW->GetYaxis()->SetTitle(" ");
 c1->Print(plot5e);



 gROOT->ProcessLine(".q");

}
