void CSCRecHitPlotter(int segtype){

  /* Macro to plot histograms produced by CSCRecHitReader.cc
   * You may need to update the TFile name, and will need to
   * input the segtype as shown below.
   *
   * Author:  Dominique Fortin - UCR
   */


  nsigmas = 1.25;  // Number of sigmas around mean to fit gaussian.  It uses 2 iterations 
                   // i.e. range is = [mu - nsigmas * sigma, mu + nsigmas * sigma]

TFile *file = TFile::Open("cscrechitplots.root");

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

// Files for histogram output --> set suffixps to desired file type:  e.g. .eps, .jpg, ...

 TString plot1a = "xrec_"+segment+suffixps;
 TString plot1b = "xsim_"+segment+suffixps;
 TString plot1c = "xresol_"+segment+suffixps;
 TString plot1d = "xresol_vs_x_"+segment+suffixps;
 TString plot1e = "yrec_vs_xrec_"+segment+suffixps;
 TString plot1f = "ysim_vs_xsim_"+segment+suffixps;

 TString plot2a = "yrec_"+segment+suffixps; 
 TString plot2b = "ysim_"+segment+suffixps; 
 TString plot2c = "yresol_"+segment+suffixps; 
 TString plot2d = "yresol_vs_y_"+segment+suffixps;
 TString plot2e = "yrec_vs_ysim_"+segment+suffixps;

 TString plot3a = "wiregrp_rec_"+segment+suffixps;
 TString plot3b = "wiregrp_digi_"+segment+suffixps;
 TString plot3c = "wiregrp_resol_"+segment+suffixps;
 TString plot3d = "x_vs_wiregrp_"+segment+suffixps;
 TString plot3e = "y_vs_wiregrp_"+segment+suffixps;
 TString plot3f = "yresol_vs_wiregrp_"+segment+suffixps;
 TString plot3g = "wiregrp_alldigi_"+segment+suffixps;
 TString plot3h = "wiregrp_sim_"+segment+suffixps;

 TString plot4a = "stripnum_rec_"+segment+suffixps;
 TString plot4b = "stripnum_digi_"+segment+suffixps;
 TString plot4c = "stripnum_resol_"+segment+suffixps;
 TString plot4d = "x_vs_stripnum_"+segment+suffixps;
 TString plot4e = "stripnum_alldigi_"+segment+suffixps; 
 TString plot4f = "stripnum_sim_"+segment+suffixps;

 TString plot5a = "recphi_"+segment+suffixps;
 TString plot5b = "simphi_"+segment+suffixps;
 TString plot5c = "resphi_vs_simphi_"+segment+suffixps;
 TString plot5d = "rdphiresol_"+segment+suffixps; 

 TString plot6a = "receta_"+segment+suffixps; 
 TString plot6b = "simetaresol_"+segment+suffixps; 
 TString plot6c = "etaresol_"+segment+suffixps;
 TString plot6d = "etaresol_vs_eta_"+segment+suffixps;

// ********************************************************************
// Pointers to histograms
// ********************************************************************

// 1) X
 hRecPositionX     = (TH1F *) file->Get(segment+"_hRecPositionX");
 hSimPositionX     = (TH1F *) file->Get(segment+"_hSimPositionX");
 hResPositionX     = (TH1F *) file->Get(segment+"_hResPositionX");
 hResPositionXvsX  = (TH2F *) file->Get(segment+"_ResPositionXvsX");
 hRecYvsX          = (TH2F *) file->Get(segment+"_hRecPositionYvsX");
 hSimYvsX          = (TH2F *) file->Get(segment+"_hSimPositionYvsX");

// 2) Y
 hRecPositionY     = (TH1F *) file->Get(segment+"_hRecPositionY");
 hSimPositionY     = (TH1F *) file->Get(segment+"_hSimPositionY");
 hResPositionY     = (TH1F *) file->Get(segment+"_hResPositionY");
 hResPositionYvsY  = (TH2F *) file->Get(segment+"_hResPositionYvsY");
 hRecYvsSimY       = (TH2F *) file->Get(segment+"_hRecYvsSimY");

// 3) Wire group
 hWireGrp          = (TH1F *) file->Get(segment+"_hWireGrp");
 hWireGrpDigi      = (TH1F *) file->Get(segment+"_hWireGrpDigi");
 hResWireGrp       = (TH1F *) file->Get(segment+"_hResWireGrp");
 hWireGrpvsX       = (TH2F *) file->Get(segment+"_hWireGrpvsX");
 hWireGrpvsY       = (TH2F *) file->Get(segment+"_hWireGrpvsY");
 hWireGrpvsYres    = (TH2F *) file->Get(segment+"_hWireGrpvsYres");
 hAllWireGrpDigi   = (TH1F *) file->Get(segment+"_hAllWireGrpDigi");
 hSimWireGrp       = (TH1F *) file->Get(segment+"_hSimWireGrp");

// 4) Strip #
 hStripNu          = (TH1F *) file->Get(segment+"_hStripNu");
 hStripNuDigi      = (TH1F *) file->Get(segment+"_hStripNuDigi");   
 hResStripNu       = (TH1F *) file->Get(segment+"_hResStripNu");
 hStripNuvsX       = (TH2F *) file->Get(segment+"_hStripNuvsX");
 hAllStripNuDigi   = (TH1F *) file->Get(segment+"_hAllStripNuDigi");
 hSimStripNu       = (TH1F *) file->Get(segment+"_hSimStripNu");

// 5) Phi
 hRecphi           = (TH1F *) file->Get(segment+"_hRecphi");
 hSimphi           = (TH1F *) file->Get(segment+"_hSimphi");
 hResphivsSimphi   = (TH2F *) file->Get(segment+"_hResphivsSimphi");
 hrDphi            = (TH1F *) file->Get(segment+"_hrDphi");

// 6) Eta
 hReceta           = (TH1F *) file->Get(segment+"_hReceta");
 hSimeta           = (TH1F *) file->Get(segment+"_hSimeta");
 hDeta             = (TH1F *) file->Get(segment+"_hDeta");
 hDetavseta        = (TH2F *) file->Get(segment+"_hDetavseta");


// *****************************************************************
// 1) Local X position
// *****************************************************************

// 1c) X res
 gStyle->SetOptStat(kFALSE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hResPositionX->SetTitle(segment);
 hResPositionX->Draw();
 hResPositionX->GetXaxis()->SetTitle("#Delta x_{local} (cm)");
 hResPositionX->GetYaxis()->SetTitle(" ");
 hResPositionX->Fit("gaus");
 TF1 *myfunc = hResPositionX->GetFunction("gaus");
 float par0 = gaus->GetParameter(0); 
 float par1 = gaus->GetParameter(1); 
 float par2 = gaus->GetParameter(2); 
 cout << "Parameters are: " << "P0: " << par0  
     <<  " P1: " << par1 << " P2: " <<par2 << endl;
 float low = par1 -nsigmas * par2;
 float hi = par1 + nsigmas * par2;
 hResPositionX->Fit("gaus","R","",low,hi);
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
 hResPositionX->Fit("gaus","R","",low,hi);
 c1->Print(plot1c);

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
// 2a) Rec Y
 gStyle->SetOptStat(kFALSE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hRecPositionY->SetTitle(segment);
 hRecPositionY->Draw();
 hRecPositionY->GetXaxis()->SetTitle(" yrec_{local} (cm)");
 hRecPositionY->GetYaxis()->SetTitle(" ");
 c1->Print(plot2a);

// 2b) Sim Y
 gStyle->SetOptStat(kFALSE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hSimPositionY->SetTitle(segment);
 hSimPositionY->Draw();
 hSimPositionY->GetXaxis()->SetTitle(" ysim_{local} (cm)");
 hSimPositionY->GetYaxis()->SetTitle(" ");
 c1->Print(plot2b);

// 2c) Delta y
 gStyle->SetOptStat(kFALSE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hResPositionY->SetTitle(segment);
 hResPositionY->Draw();
 hResPositionY->GetXaxis()->SetTitle("#Delta y_{local} (cm)");
 hResPositionY->GetYaxis()->SetTitle(" ");
 hResPositionY->Fit("gaus");
 TF1 *myfunc = hResPositionY->GetFunction("gaus");
 float par0 = gaus->GetParameter(0); 
 float par1 = gaus->GetParameter(1); 
 float par2 = gaus->GetParameter(2); 
 cout << "Parameters are: " << "P0: " << par0  
     <<  " P1: " << par1 << " P2: " <<par2 << endl;
 float low = par1 -nsigmas * par2;
 float hi = par1 + nsigmas * par2;
 hResPositionY->Fit("gaus","R","",low,hi);
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
 hResPositionY->Fit("gaus","R","",low,hi);
 c1->Print(plot2c);

// 2d) Delta Y vs Y
 gStyle->SetOptStat(kFALSE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hResPositionYvsY->SetTitle(segment);
 hResPositionYvsY->Draw();
 hResPositionYvsY->GetXaxis()->SetTitle("y_{local} (cm)");
 hResPositionYvsY->GetYaxis()->SetTitle("#Delta y_{local} (cm) ");
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
// 3) Wire group
// *****************************************************************

// 3a) Reco wg
 gStyle->SetOptStat(kFALSE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hWireGrp->SetTitle(segment);   
 hWireGrp->Draw();   
 hWireGrp->GetXaxis()->SetTitle(" Wire Group ");
 hWireGrp->GetYaxis()->SetTitle(" ");
 c1->Print(plot3a);

// 3b) Digi wg
 gStyle->SetOptStat(kFALSE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hWireGrpDigi->SetTitle(segment);
 hWireGrpDigi->Draw();
 hWireGrpDigi->GetXaxis()->SetTitle(" Wire Group ");
 hWireGrpDigi->GetYaxis()->SetTitle(" ");
 c1->Print(plot3b);

// 3c) Delta wg
 gStyle->SetOptStat(kTRUE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hResWireGrp->SetTitle(segment);
 hResWireGrp->Draw();
 hResWireGrp->GetXaxis()->SetTitle("#Delta Wire Group ");
 hWireGrpDigi->GetYaxis()->SetTitle(" ");
 c1->Print(plot3c);

// 3d) X vs wg
 gStyle->SetOptStat(kFALSE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hWireGrpvsX->SetTitle(segment);
 hWireGrpvsX->Draw("BOX");
 hWireGrpvsX->GetYaxis()->SetTitle("x_{local} (cm)");
 hWireGrpvsX->GetXaxis()->SetTitle("wire group ");
 c1->Print(plot3d);

// 3e) Y vs wg
 gStyle->SetOptStat(kFALSE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hWireGrpvsY->SetTitle(segment);
 hWireGrpvsY->Draw("BOX");
 hWireGrpvsY->GetYaxis()->SetTitle("y_{local} (cm)");
 hWireGrpvsY->GetXaxis()->SetTitle("wire group ");
 c1->Print(plot3e);

// 3f) Delta Y vs wg
 gStyle->SetOptStat(kFALSE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hWireGrpvsYres->SetTitle(segment);
 hWireGrpvsYres->Draw("BOX");
 hWireGrpvsYres->GetYaxis()->SetTitle("y_{Resol} (cm)");
 hWireGrpvsYres->GetXaxis()->SetTitle("wire group ");
 c1->Print(plot3f);

// 3g) Digi wg
 gStyle->SetOptStat(kFALSE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hAllWireGrpDigi->SetTitle(segment);   
 hAllWireGrpDigi->Draw();
 hAllWireGrpDigi->GetXaxis()->SetTitle(" Wire Group ");
 hAllWireGrpDigi->GetYaxis()->SetTitle(" ");
 c1->Print(plot3g);

// 3h) Reco wg
 gStyle->SetOptStat(kFALSE);  
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hSimWireGrp->SetTitle(segment);
 hSimWireGrp->Draw();
 hSimWireGrp->GetXaxis()->SetTitle(" Wire Group ");
 hSimWireGrp->GetYaxis()->SetTitle(" ");
 c1->Print(plot3h);

// *****************************************************************
// 4) Strip #
// *****************************************************************
 
// 4a) Rec s#
 gStyle->SetOptStat(kTRUE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hStripNu->SetTitle(segment);
 hStripNu->Draw();
 hStripNu->GetXaxis()->SetTitle("Strip Number ");
 hStripNu->GetYaxis()->SetTitle(" ");
 c1->Print(plot4a);

// 4b) Digi s#
 gStyle->SetOptStat(kTRUE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hStripNuDigi->SetTitle(segment);
 hStripNuDigi->Draw();
 hStripNuDigi->GetXaxis()->SetTitle("Strip Number digi");
 hStripNuDigi->GetYaxis()->SetTitle(" ");
 c1->Print(plot4b);

// 4c) Delta s#
 gStyle->SetOptStat(kTRUE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hResStripNu->SetTitle(segment);
 hResStripNu->Draw();
 hResStripNu->GetXaxis()->SetTitle("#Delta Strip Number ");
 hResStripNu->GetYaxis()->SetTitle(" ");
 c1->Print(plot4c);

// 4d) X vs s#
 gStyle->SetOptStat(kFALSE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hStripNuvsX->SetTitle(segment);
 hStripNuvsX->Draw("Box");
 hStripNuvsX->GetYaxis()->SetTitle("x_{local} (cm)");
 hStripNuvsX->GetXaxis()->SetTitle("Strip Number ");
 c1->Print(plot4d);

// 4e) All Digi s#
 gStyle->SetOptStat(kTRUE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hAllStripNuDigi->SetTitle(segment);
 hAllStripNuDigi->Draw();
 hAllStripNuDigi->GetXaxis()->SetTitle("Strip Number digi");
 hAllStripNuDigi->GetYaxis()->SetTitle(" ");
 c1->Print(plot4e);

// 4f) Rec s#
 gStyle->SetOptStat(kTRUE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hSimStripNu->SetTitle(segment);
 hSimStripNu->Draw();
 hSimStripNu->GetXaxis()->SetTitle("Strip Number ");
 hSimStripNu->GetYaxis()->SetTitle(" ");
 c1->Print(plot4f);

// *****************************************************************
// 5) Phi
// *****************************************************************

// 5a) Rec phi

// 5b) Sim phi

// 5c) Delta phi
 
// 5d) R * Delta Phi
 gStyle->SetOptStat(kFALSE);
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
 c1->Print(plot5d);


// *****************************************************************
// 6) Eta
// *****************************************************************

// 6a) Rec Eta
 gStyle->SetOptStat(kFALSE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hReceta->SetTitle(segment);
 hReceta->Draw();
 hReceta->GetXaxis()->SetTitle("#eta_{reco} ");
 hReceta->GetYaxis()->SetTitle("");
 c1->Print(plot6a);

// 6b) Sim Eta
 gStyle->SetOptStat(kFALSE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hSimeta->SetTitle(segment);
 hSimeta->Draw();
 hSimeta->GetXaxis()->SetTitle("#eta_{sim}  ");
 hSimeta->GetYaxis()->SetTitle(" ");
 c1->Print(plot6b);


// 6c) Delta eta
 gStyle->SetOptStat(kFALSE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hDeta->SetTitle(segment);
 hDeta->Draw();
 hDeta->GetXaxis()->SetTitle("#Delta #eta ");
 hDeta->GetYaxis()->SetTitle(" ");
 hDeta->Fit("gaus");
 TF1 *myfunc = hDeta->GetFunction("gaus");
 float par0 = gaus->GetParameter(0); 
 float par1 = gaus->GetParameter(1); 
 float par2 = gaus->GetParameter(2); 
 cout << "Parameters are: " << "P0: " << par0  
     <<  " P1: " << par1 << " P2: " <<par2 << endl;
 float low = par1 -nsigmas * par2;
 float hi = par1 + nsigmas * par2;
 hDeta->Fit("gaus","R","",low,hi);
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
 hDeta->Fit("gaus","R","",low,hi);
 c1->Print(plot6c);

// 6d) Delta Eta vs Eta
 gStyle->SetOptStat(kFALSE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hDetavseta->SetTitle(segment);
 hDetavseta->Draw("BOX");
 hDetavseta->GetXaxis()->SetTitle("#eta  ");
 hDetavseta->GetYaxis()->SetTitle("#Delta #eta ");
 c1->Print(plot6d);




 gROOT->ProcessLine(".q");

}
