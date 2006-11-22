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

TString suffixps = ".jpg";

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

// Files for histogram output --> set suffixps to desired file type:  e.g. .eps, .jpg, ...

 TString plot0 = "matched_pair_"+segment+suffixps;

 TString plot1a = "xrec_"+segment+suffixps;
 TString plot1b = "xsim_"+segment+suffixps;
 TString plot1c = "dx_"+segment+suffixps;
 TString plot1d = "x_pulls_"+segment+suffixps;
 TString plot1e = "yrec_vs_xrec_"+segment+suffixps;
 TString plot1f = "ysim_vs_xsim_"+segment+suffixps;

 TString plot2a = "yrec_"+segment+suffixps; 
 TString plot2b = "ysim_"+segment+suffixps; 
 TString plot2c = "dy_"+segment+suffixps; 
 TString plot2d = "y_pulls_"+segment+suffixps;
 TString plot2e = "yrec_vs_ysim_"+segment+suffixps;

 TString plot5a = "dphi_"+segment+suffixps;
 TString plot5aa = "dphi_bottom_"+segment+suffixps;
 TString plot5aaa = "dphi_top_"+segment+suffixps;
 TString plot5b = "dphi_pulls_"+segment+suffixps;
 TString plot5c = "rdphi_"+segment+suffixps;
 TString plot5cc = "rdphi_bottom_"+segment+suffixps;
 TString plot5ccc = "rdphi_top_"+segment+suffixps;
 TString plot5d = "rdphi_pulls_"+segment+suffixps;

 TString plot6c = "deta_"+segment+suffixps;

// ********************************************************************
// Pointers to histograms
// ********************************************************************

// 0) Matched pair
 hHaveMatch        = (TH1F *) file->Get(segment+"_hHaveMatch");

// 1) X
 hRecPositionX     = (TH1F *) file->Get(segment+"_hRecPositionX");
 hSimPositionX     = (TH1F *) file->Get(segment+"_hSimPositionX");
 hResPositionX     = (TH1F *) file->Get(segment+"_hResPositionX");
 hPullX            = (TH1F *) file->Get(segment+"_hPullX");
 hResPositionXvsX  = (TH2F *) file->Get(segment+"_ResPositionXvsX");
 hRecYvsX          = (TH2F *) file->Get(segment+"_hRecPositionYvsX");
 hSimYvsX          = (TH2F *) file->Get(segment+"_hSimPositionYvsX");

// 2) Y
 hRecPositionY     = (TH1F *) file->Get(segment+"_hRecPositionY");
 hSimPositionY     = (TH1F *) file->Get(segment+"_hSimPositionY");
 hResPositionY     = (TH1F *) file->Get(segment+"_hResPositionY");
 hPullY            = (TH1F *) file->Get(segment+"_hPullY");
 hResPositionYvsY  = (TH2F *) file->Get(segment+"_hResPositionYvsY");
 hRecYvsSimY       = (TH2F *) file->Get(segment+"_hRecYvsSimY");

// 5) Phi
 hResphi           = (TH1F *) file->Get(segment+"_hResphi");
 hResphib          = (TH1F *) file->Get(segment+"_hResphib");
 hResphit          = (TH1F *) file->Get(segment+"_hResphit");
 hPullphi          = (TH1F *) file->Get(segment+"_hPullphi");
 hrDphi            = (TH1F *) file->Get(segment+"_hrDphi");
 hrDphib           = (TH1F *) file->Get(segment+"_hrDphib");
 hrDphit           = (TH1F *) file->Get(segment+"_hrDphit");
 hPullrDphi        = (TH1F *) file->Get(segment+"_hPullrDphi");

// 6) Eta
 hDeta             = (TH1F *) file->Get(segment+"_hDeta");

// ***************************************************************** 
// Have match
// *****************************************************************

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

// 2e) Y reco vs Y sim 
 gStyle->SetOptStat(kFALSE);  
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hRecYvsSimY->SetTitle(segment);
 hRecYvsSimY->Draw();
 hRecYvsSimY->GetXaxis()->SetTitle("y_{sim} (cm) ");
 hRecYvsSimY->GetYaxis()->SetTitle("y_{reco} (cm) ");
 c1->Print(plot2e);

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

// 5aa) Res phi bottom of chamber
 gStyle->SetOptStat(kFALSE);  
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hResphib->SetTitle("20% bottom of "+segment);
 hResphib->GetXaxis()->SetTitle("#Delta #phi  (mrads)");
 hResphib->GetYaxis()->SetTitle(" ");
 hResphib->Fit("gaus");
 TF1 *myfunc = hrDphi->GetFunction("gaus");
 float par0 = gaus->GetParameter(0);
 float par1 = gaus->GetParameter(1);
 float par2 = gaus->GetParameter(2);
 cout << "Parameters are: " << "P0: " << par0
     <<  " P1: " << par1 << " P2: " <<par2 << endl;
 float low = par1 -nsigmas * par2;
 float hi = par1 + nsigmas * par2;
 hResphib->Fit("gaus","R","",low,hi);
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
 hResphib->Fit("gaus","R","",low,hi);
// hResphib->Draw();
 c1->Print(plot5aa);

// 5aaa) Res phi top of chamber
 gStyle->SetOptStat(kFALSE);  
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hResphit->SetTitle("20% top of "+segment);
// hResphit->Draw();
 hResphit->GetXaxis()->SetTitle("#Delta #phi  (mrads)");
 hResphit->GetYaxis()->SetTitle(" ");
 hResphit->Fit("gaus");
 TF1 *myfunc = hrDphi->GetFunction("gaus");
 float par0 = gaus->GetParameter(0);
 float par1 = gaus->GetParameter(1);
 float par2 = gaus->GetParameter(2);
 cout << "Parameters are: " << "P0: " << par0
     <<  " P1: " << par1 << " P2: " <<par2 << endl;
 float low = par1 -nsigmas * par2;
 float hi = par1 + nsigmas * par2;
 hResphit->Fit("gaus","R","",low,hi);
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
 hResphit->Fit("gaus","R","",low,hi);
 c1->Print(plot5aaa);

// 5b) Pull phi
 gStyle->SetOptStat(kTRUE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hPullphi->SetTitle(segment);
 hPullphi->Draw();
 hPullphi->GetXaxis()->SetTitle("#Delta #phi/#sigma #phi");
 hPullphi->GetYaxis()->SetTitle("");
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

// 5cc) R * Delta Phi
 gStyle->SetOptStat(kFALSE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hrDphib->SetTitle("20% bottom of "+segment);
 hrDphib->Draw();
 hrDphib->GetXaxis()->SetTitle("r #Delta #phi  (cm)");
 hrDphib->GetYaxis()->SetTitle(" ");
 hrDphib->Fit("gaus");
 TF1 *myfunc = hrDphi->GetFunction("gaus");
 float par0 = gaus->GetParameter(0); 
 float par1 = gaus->GetParameter(1); 
 float par2 = gaus->GetParameter(2); 
 cout << "Parameters are: " << "P0: " << par0  
     <<  " P1: " << par1 << " P2: " <<par2 << endl;
 float low = par1 -nsigmas * par2;
 float hi = par1 + nsigmas * par2;
 hrDphib->Fit("gaus","R","",low,hi);
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
 hrDphib->Fit("gaus","R","",low,hi);
 c1->Print(plot5cc);

// 5ccc) R * Delta Phi
 gStyle->SetOptStat(kFALSE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hrDphit->SetTitle("20% top of "+segment);
 hrDphit->Draw();
 hrDphit->GetXaxis()->SetTitle("r #Delta #phi  (cm)");
 hrDphit->GetYaxis()->SetTitle(" ");
 hrDphit->Fit("gaus");
 TF1 *myfunc = hrDphi->GetFunction("gaus");
 float par0 = gaus->GetParameter(0); 
 float par1 = gaus->GetParameter(1); 
 float par2 = gaus->GetParameter(2); 
 cout << "Parameters are: " << "P0: " << par0  
      <<  " P1: " << par1 << " P2: " <<par2 << endl;
 float low = par1 -nsigmas * par2;
 float hi = par1 + nsigmas * par2;
 hrDphit->Fit("gaus","R","",low,hi);
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
 hrDphit->Fit("gaus","R","",low,hi);
 c1->Print(plot5ccc);

// 5d) Pull R * Delta Phi
 gStyle->SetOptStat(kTRUE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hPullrDphi->SetTitle(segment);
 hPullrDphi->Draw();
 hPullrDphi->GetXaxis()->SetTitle("r #Delta #phi/#sigma (r #phi) ");
 hPullrDphi->GetYaxis()->SetTitle("");
 c1->Print(plot5d);



// *****************************************************************
// 6) Eta
// *****************************************************************

// 6c) Delta eta
 gStyle->SetOptStat(kFALSE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hDeta->SetTitle(segment);
 hDeta->Draw();
 hDeta->GetXaxis()->SetTitle("#Delta #eta ");
 hDeta->GetYaxis()->SetTitle(" ");
 c1->Print(plot6c);



 gROOT->ProcessLine(".q");

}
