void CSCRecHitPlotter(int segtype){

  /* Macro to plot histograms produced by CSCRecHitReader.cc
   * You may need to update the TFile name, and will need to
   * input the segtype as shown below.
   *
   * Author:  Dominique Fortin - UCR
   */


TFile *file = TFile::Open("cscrechitplots.root");

TString suffixps = ".ps";

TString segment = "ME_All";

 if (segtype == 10) TString segment = "ME_1_a";
 if (segtype == 11) TString segment = "ME_1_b";
 if (segtype == 12) TString segment = "ME_1_2";
 if (segtype == 13) TString segment = "ME_1_3";
 if (segtype == 21) TString segment = "ME_2_1";
 if (segtype == 22) TString segment = "ME_2_2";
 if (segtype == 31) TString segment = "ME_3_1";
 if (segtype == 32) TString segment = "ME_3_2";
 if (segtype == 40) TString segment = "ME_4";

 TString plot1 = "xresol_"+segment+suffixps;
 TString plot2 = "yresol_"+segment+suffixps; 
 TString plot3 = "yresol_vs_y_"+segment+suffixps;
 TString plot4 = "rdphiresol_"+segment+suffixps; 
 TString plot5 = "etaresol_"+segment+suffixps; 
 TString plot6 = "etaresol_vs_eta_"+segment+suffixps; 


 hRecPositionX     = (TH1F *) file->Get(segment+"_hRecPositionX");
 hRecPositionY     = (TH1F *) file->Get(segment+"_hRecPositionY");
 hSimPositionX     = (TH1F *) file->Get(segment+"_hSimPositionX");
 hSimPositionY     = (TH1F *) file->Get(segment+"_hSimPositionY");
 hResPositionX     = (TH1F *) file->Get(segment+"_hResPositionX");
 hResPositionY     = (TH1F *) file->Get(segment+"_hResPositionY");
 hResPositionYvsY  = (TH2F *) file->Get(segment+"_hResPositionYvsY");
 
 hphi              = (TH1F *) file->Get(segment+"_hphi");
 hrDphi            = (TH1F *) file->Get(segment+"_hrDphi");
 heta              = (TH1F *) file->Get(segment+"_heta");
 hdeta             = (TH1F *) file->Get(segment+"_hdeta");
 hdetavseta        = (TH2F *) file->Get(segment+"_hdetavseta");
 

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
 float low = par1 -1.5 * par2;
 float hi = par1 + 1.5 * par2;
 hResPositionX->Fit("gaus","R","",low,hi);
 par0 = gaus->GetParameter(0); 
 par1 = gaus->GetParameter(1); 
 par2 = gaus->GetParameter(2); 
 cout << "********* Second fit *********" << endl;
 cout << "Parameters are: " << "P0: " << par0  
      <<  " P1: " << par1 << " P2: " << par2 << endl;
 gStyle->SetOptStat(kTRUE);
 gStyle->SetOptFit(0111);
 low = par1 -1.5 * par2;
 hi = par1 + 1.5 * par2;
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
 float low = par1 -1.5 * par2;
 float hi = par1 + 1.5 * par2;
 hResPositionY->Fit("gaus","R","",low,hi);
 par0 = gaus->GetParameter(0); 
 par1 = gaus->GetParameter(1); 
 par2 = gaus->GetParameter(2); 
 cout << "********* Second fit *********" << endl;
 cout << "Parameters are: " << "P0: " << par0  
      <<  " P1: " << par1 << " P2: " << par2 << endl;
 gStyle->SetOptStat(kTRUE);
 gStyle->SetOptFit(0111);
 low = par1 -1.5 * par2;
 hi = par1 + 1.5 * par2;
 hResPositionY->Fit("gaus","R","",low,hi);
 c1->Print(plot2);

 gStyle->SetOptStat(kTRUE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hResPositionYvsY->SetTitle(segment);
 hResPositionYvsY->Draw();
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
 float low = par1 -1.5 * par2;
 float hi = par1 + 1.5 * par2;
 hrDphi->Fit("gaus","R","",low,hi);
 par0 = gaus->GetParameter(0); 
 par1 = gaus->GetParameter(1); 
 par2 = gaus->GetParameter(2); 
 cout << "********* Second fit *********" << endl;
 cout << "Parameters are: " << "P0: " << par0  
      <<  " P1: " << par1 << " P2: " << par2 << endl;
 gStyle->SetOptStat(kTRUE);
 gStyle->SetOptFit(0111);
 low = par1 -1.5 * par2;
 hi = par1 + 1.5 * par2;
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
 float low = par1 -1.5 * par2;
 float hi = par1 + 1.5 * par2;
 hdeta->Fit("gaus","R","",low,hi);
 par0 = gaus->GetParameter(0); 
 par1 = gaus->GetParameter(1); 
 par2 = gaus->GetParameter(2); 
 cout << "********* Second fit *********" << endl;
 cout << "Parameters are: " << "P0: " << par0  
      <<  " P1: " << par1 << " P2: " << par2 << endl;
 gStyle->SetOptStat(kTRUE);
 gStyle->SetOptFit(0111);
 low = par1 -1.5 * par2;
 hi = par1 + 1.5 * par2;
 hdeta->Fit("gaus","R","",low,hi);
 c1->Print(plot5);

 gStyle->SetOptStat(kTRUE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hdetavseta->SetTitle(segment);
 hdetavseta->Draw();
 hdetavseta->GetXaxis()->SetTitle("#eta  ");
 hdetavseta->GetYaxis()->SetTitle("#Delta #eta ");
 c1->Print(plot6);

 gROOT->ProcessLine(".q");

}
