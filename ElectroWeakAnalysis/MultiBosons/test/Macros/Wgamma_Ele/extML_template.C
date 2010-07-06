// Written by R.S Lu (National Taiwan University)

#include <TFractionFitter.h>
#include <iostream>
#include <fstream>
#include <TH1.h>
#include <TRandom3.h>
#include <TCanvas.h>
#include <TH2.h>
#include <TStyle.h>
#include <TMinuit.h>
#include <vector>
#include <TMath.h>
#include <TVirtualFitter.h>
#include <TFile.h>

using namespace std;

#define NPARBIN 2

vector<Double_t> dataCollBin;
vector<Double_t> sigCollBin;
vector<Double_t> bkgCollBin;

vector<Double_t> infoBin;
vector<Double_t> infoBin_err;

void fcnBin(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag)
{

  Double_t Lsum=0.;
  Double_t Nevt=0.;
  Double_t fs = par[0];
  Double_t fb = par[1];

  for (unsigned int i=0; i<dataCollBin.size(); i++ ) {
    Nevt += dataCollBin[i];
    //PDF for signal and background
    Double_t Ls = sigCollBin[i];
    Double_t Lb = bkgCollBin[i];	
    for (int data=0; data<dataCollBin[i]; data++) {	
      //Get Log Likelihood
      Lsum += TMath::Log( (fs*Ls + fb*Lb) / (fs+fb) );
    }
  }
  f=2*( -1*Lsum + (fs+fb)  - Nevt*TMath::Log(fs+fb) );
  
}


//___________________________________________________________________________
//Double_t* IfitBin(TH1F* dataInput, TH1F* sigTemplate, TH1F* bkgTemplate)
Double_t* IfitBin(TH1D* dataInput, TH1D* sigTemplate, TH1D* bkgTemplate)
{

  TCanvas *c1 = new TCanvas("HF1", "Histos1", 0, 0, 600, 600);
  dataCollBin.clear();
  sigCollBin.clear();
  bkgCollBin.clear();

  Double_t* fitted = new Double_t[8];
  fitted[0] = fitted[1] = fitted[2] = fitted[3] = fitted[4] = fitted[5] = fitted[6] = fitted[7] = 0.0;

  float ntemplate = 1.;
  float sigfrac = 0.1;

  TH1F *hdata;
  TH1F *hsig  = (TH1F*)sigTemplate->Clone();
  hsig->SetName("hsig");

  TH1F *hbkg  = (TH1F*)bkgTemplate->Clone();
  hbkg->SetName("hbkg");

  float ndata=0;
  hdata = (TH1F*)dataInput->Clone();
  hdata->SetName("hdata");
  ndata = hdata->Integral();

  if(ndata==0) {
    printf(" ---  no events in the fit \n");
    return fitted;
  }
    
  printf(" --------- before the fit ------------- \n");
  printf("Nsig %2.3f, Nbg %2.3f, Ntemplate %3.3f \n", hsig->Integral(), hbkg->Integral(), ntemplate);
  printf(" -------------------------------------- \n");

  int nbins = hdata->GetNbinsX();

  hsig->Scale(1./hsig->Integral());
  hbkg->Scale(1./hbkg->Integral());  
  for (int ibin=1; ibin<=nbins; ibin++) {
    dataCollBin.push_back(hdata->GetBinContent(ibin));
    sigCollBin.push_back(hsig->GetBinContent(ibin));
    bkgCollBin.push_back(hbkg->GetBinContent(ibin));    
  }
  printf( " -----  Got %d, %d, %d events for fit ----- \n ", dataCollBin.size(), sigCollBin.size(), bkgCollBin.size() );  
  if ( dataCollBin.size() != sigCollBin.size() || sigCollBin.size()!=bkgCollBin.size() ) {
    printf(" error ...  inconsistent hit collection size \n");
    return fitted;
  }

  //--------------------------------------------------
  //init parameters for fit
  Double_t vstart[10] = {1., 1.};
  vstart[0] = sigfrac*ndata;
  vstart[1] = (1-sigfrac)*ndata;
 
  TMinuit *gMinuit = new TMinuit(NPARBIN);  
  gMinuit->Command("SET STR 1");
  gMinuit->SetFCN(fcnBin);
  Double_t arglist[10];
  Int_t ierflg = 0;
  
  arglist[0] = 1;
  gMinuit->mnexcm("SET ERR", arglist ,1, ierflg);
  arglist[0] = 1;
  gMinuit->mnexcm("SET PRINT", arglist ,1, ierflg);

  Double_t step[] = { 0.1, 0.1,};

  gMinuit->mnparm(0,  "Signal yield"  , vstart[0],  step[0], 0., ndata*2.  , ierflg);
  gMinuit->mnparm(1,  "background yield"  , vstart[1],  step[1], 0., ndata*2. , ierflg);
  
  printf(" --------------------------------------------------------- \n");
  printf(" Now ready for minimization step \n --------------------------------------------------------- \n");
  
  arglist[0] = 2000; // number of iteration
  arglist[1] = 1.;
  gMinuit->mnexcm("MIGRAD", arglist ,2, ierflg);
  printf (" -------------------------------------------- \n");
  printf("Finished.  ierr = %d \n", ierflg);

  infoBin.clear();
  infoBin_err.clear();

  double para[NPARBIN+1], errpara[NPARBIN+1];
  if ( ierflg == 0 ) 
    {
      for(int j=0; j<=NPARBIN-1;j++) {
        gMinuit->GetParameter(j, para[j],errpara[j]);
        para[NPARBIN] = dataCollBin.size();
        infoBin.push_back(para[j]);
        infoBin_err.push_back(errpara[j]);
        printf("Parameter (yeild) %d = %f +- %f\n",j,para[j],errpara[j]);
	
      }
      printf(" fitted yield %2.3f \n", (para[0]+para[1])/ndata );
      infoBin.push_back(sigCollBin.size());

    }
  else {
    printf(" *********** Fit failed! ************\n");
    gMinuit->GetParameter(0, para[0],errpara[0]);
    gMinuit->GetParameter(1, para[1],errpara[1]);
    para[0]=0.; errpara[0]=0.;
  }

  
  // Print results
  Double_t amin,edm,errdef;
  Int_t nvpar,nparx,icstat;
  gMinuit->mnstat(amin,edm,errdef,nvpar,nparx,icstat);
  gMinuit->mnprin(1,amin);  
  gMinuit->mnmatu(1);
  printf(" ========= happy ending !? =========================== \n");
  
  printf("FCN =  %3.3f \n", amin);

  double yerr[20];
  for(int i=0;i<20;i++){
    yerr[i] = 0.;
  }

  hsig->Scale(para[0]);
  hbkg->Scale(para[1]);
  TH1F *hfit = (TH1F*)hsig->Clone();
  hfit->Add(hbkg);

  hsig->SetLineColor(1);
  hsig->SetFillColor(5);
  hsig->SetFillStyle(3001);

  hbkg->SetLineWidth(2);
  // plot
  c1->Draw();  
  //gPad->SetLogy();
  hdata->SetLineColor(1);
  hdata->SetNdivisions(505,"XY");
  hdata->SetXTitle("#sigma_{i#etai#eta}");
  hdata->SetYTitle("Number of photons");
  hdata->SetTitle("");
  hdata->SetMarkerStyle(8);
  hdata->SetMinimum(0.);
  hdata->SetMaximum(hdata->GetMaximum()*1.4);
  hdata->Draw("PE1");
  hsig->Draw("hist same");
  hbkg->SetMarkerStyle(0);
  hbkg->SetFillColor(8);
  hbkg->SetLineWidth(1);
  hbkg->SetFillStyle(3013);
  hbkg->SetError(yerr);
  hbkg->Draw("hist same");
  hfit->SetMarkerStyle(0);
  hfit->SetLineColor(1);
  hfit->SetLineWidth(2);
  hfit->SetError(yerr);
  hfit->Draw("hist same");

  TLegend *tleg = new TLegend(0.4, 0.65, 0.95, 0.9);
  char text[50];
  tleg->SetHeader(dataInput->GetTitle());
  tleg->SetFillColor(0);
  tleg->SetShadowColor(0);
  tleg->SetBorderSize(0);
  sprintf(text,"Data %5.1f events",hdata->Integral());
  tleg->AddEntry(hdata,text,"pl");
  sprintf(text,"Fitted %5.1f events",hfit->Integral());
  tleg->AddEntry(hfit,text,"l");
  sprintf(text,"SIG %5.1f #pm %5.1f events",para[0], errpara[0]);
  tleg->AddEntry(hsig,text,"f");
  sprintf(text,"BKG %5.1f #pm %5.1f events",para[1], errpara[1]);
  tleg->AddEntry(hbkg,text,"f");
  tleg->Draw();

  gPad->RedrawAxis();

  cout << dataInput->GetName() << endl;
  char fname[300];
  sprintf(fname,"plots/Ifit_%s.pdf",dataInput->GetName());
  c1->SaveAs(fname);

  printf("----- fit results with signal projection   ----------- \n");

  //   ftemplate->Close();
  
  int purityMaxBin = hsig->FindBin(5.0)-1;
  Double_t scale_signal = hsig->Integral(1,purityMaxBin)/hsig->Integral();
  Double_t scale_background = hbkg->Integral(1,purityMaxBin)/hbkg->Integral();
  
  fitted[0] = para[0];
  fitted[1] = errpara[0];
  fitted[2] = para[1];
  fitted[3] = errpara[1];

  // for integral up to 5 GeV
  fitted[4] = para[0]*scale_signal;
  fitted[5] = errpara[0]*scale_signal;
  fitted[6] = para[1]*scale_background;
  fitted[7] = errpara[1]*scale_background;

  return fitted;
}

