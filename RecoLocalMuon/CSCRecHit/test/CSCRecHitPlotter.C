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


 TString plot1 = "xresol_"+segment+suffixps;
 TString plot2 = "yresol_"+segment+suffixps; 
 TString plot3 = "yresol_vs_y_"+segment+suffixps;
 TString plot4 = "rdphiresol_"+segment+suffixps; 
 TString plot5 = "etaresol_"+segment+suffixps; 
 TString plot6 = "etaresol_vs_eta_"+segment+suffixps; 
 TString plot7 = "yrec_vs_ysim_"+segment+suffixps;
 TString plot8 = "ysim_vs_xsim_"+segment+suffixps;
 TString plot8b = "yrec_vs_xrec_"+segment+suffixps;
 TString plot9 = "recphi_"+segment+suffixps; 
 TString plot10 = "simphi_"+segment+suffixps;
 TString plot11 = "recphi_vs_simphi_"+segment+suffixps;
 TString plot12 = "xrec_"+segment+suffixps;
 TString plot13 = "xsim_"+segment+suffixps;


 hRecPositionX     = (TH1F *) file->Get(segment+"_hRecPositionX");
 hRecPositionY     = (TH1F *) file->Get(segment+"_hRecPositionY");
 hRecYvsX          = (TH2F *) file->Get(segment+"_hRecPositionYvsX");
 hSimPositionX     = (TH1F *) file->Get(segment+"_hSimPositionX");
 hSimPositionY     = (TH1F *) file->Get(segment+"_hSimPositionY");
 hSimYvsX          = (TH2F *) file->Get(segment+"_hSimPositionYvsX");
 hResPositionX     = (TH1F *) file->Get(segment+"_hResPositionX");
 hResPositionY     = (TH1F *) file->Get(segment+"_hResPositionY");
 hResPositionYvsY  = (TH2F *) file->Get(segment+"_hResPositionYvsY");
 
 hphi              = (TH1F *) file->Get(segment+"_hphi");
 hrDphi            = (TH1F *) file->Get(segment+"_hrDphi");
 heta              = (TH1F *) file->Get(segment+"_heta");
 hdeta             = (TH1F *) file->Get(segment+"_hdeta");
 hdetavseta        = (TH2F *) file->Get(segment+"_hdetavseta");
 hRecYvsSimY       = (TH2F *) file->Get(segment+"_hRecYvsSimY");

 hRecphi           = (TH1F *) file->Get(segment+"_hRecphi");
 hSimphi           = (TH1F *) file->Get(segment+"_hSimphi");
 hRecphivsSimphi   = (TH2F *) file->Get(segment+"_hRecphivsSimphi");

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

 c1->Print(plot1);


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
 c1->Print(plot2);

 gStyle->SetOptStat(kTRUE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hResPositionYvsY->SetTitle(segment);
 hResPositionYvsY->Draw("BOX");
 hResPositionYvsY->GetXaxis()->SetTitle("y_{local} (cm)");
 hResPositionYvsY->GetYaxis()->SetTitle("#Delta y_{local} (cm) ");
 c1->Print(plot3);

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

 c1->Print(plot4);

 gStyle->SetOptStat(kFALSE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hdeta->SetTitle(segment);
 hdeta->Draw();
 hdeta->GetXaxis()->SetTitle("#Delta #eta ");
 hdeta->GetYaxis()->SetTitle(" ");
 hdeta->Fit("gaus");
 TF1 *myfunc = hdeta->GetFunction("gaus");
 float par0 = gaus->GetParameter(0); 
 float par1 = gaus->GetParameter(1); 
 float par2 = gaus->GetParameter(2); 
 cout << "Parameters are: " << "P0: " << par0  
     <<  " P1: " << par1 << " P2: " <<par2 << endl;
 float low = par1 -nsigmas * par2;
 float hi = par1 + nsigmas * par2;
 hdeta->Fit("gaus","R","",low,hi);
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
 hdeta->Fit("gaus","R","",low,hi);
 c1->Print(plot5);

 gStyle->SetOptStat(kTRUE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hdetavseta->SetTitle(segment);
 hdetavseta->Draw("BOX");
 hdetavseta->GetXaxis()->SetTitle("#eta  ");
 hdetavseta->GetYaxis()->SetTitle("#Delta #eta ");
 c1->Print(plot6);


 gStyle->SetOptStat(kTRUE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hRecYvsSimY->SetTitle(segment);
 hRecYvsSimY->Draw("BOX");
 hRecYvsSimY->GetXaxis()->SetTitle("y_{sim} (cm) ");
 hRecYvsSimY->GetYaxis()->SetTitle("y_{reco} (cm) ");
 c1->Print(plot7);

 gStyle->SetOptStat(kFALSE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hSimYvsX->SetTitle(segment);
 hSimYvsX->Draw();
 hSimYvsX->GetXaxis()->SetTitle("x_{sim} (cm) ");
 hSimYvsX->GetYaxis()->SetTitle("y_{sim} (cm) ");
 c1->Print(plot8);

 gStyle->SetOptStat(kFALSE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hRecYvsX->SetTitle(segment);
 hRecYvsX->Draw();
 hRecYvsX->GetXaxis()->SetTitle("x_{reco} (cm) ");
 hRecYvsX->GetYaxis()->SetTitle("y_{reco} (cm) ");
 c1->Print(plot8b);






 gROOT->ProcessLine(".q");

}
