//*************************************************************************************************************************
//This is a root macro which take analysed histogram files made by DiMuonAnalyzer and then mix Upsilon signal and dielec
//Bkgaccording to their cross sections for 0.5 (nb)-1 intgrated luminosity. this macro fitt a polinomial of degree one to 
//dielecbkg then take the parameter of this polinomial and then fitt this polinomial of degree one + three gaussian to 
//upsilon signal+dielecBkg.also it fit three gaussian to a bkg subtracted upsilon signal.
 
//ToDo--> change the no of signal and background events according to file you have.
//*************************************************************************************************************************
#ifndef __CINT__
#include <iostream>
#include "TROOT.h"
#include "TStyle.h"
#include "TFile.h"
#include "TH1F.h"
#include "TTree.h"
#include "TMath.h"
#include "TLorentzVector.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TLine.h"
#include <TObjArray.h>
#include <TTree.h>
#include "stdio.h"
#endif
void Normalization(char *filein1, char *filein2)
{

//load the analysed histo file for dielec BKG
  TFile *fil1 = new TFile(filein1); 
  TH1D *fHist2ElecMassGenb = (TH1D*)gDirectory->Get("fHistGenDiElecMinv"); 
  TH1D *fHist2ElecMassRecb = (TH1D*)gDirectory->Get("fHistRecDiElecMinv");
  fHist2ElecMassRecb-> Sumw2();//
  float BkgCrossSection =0.0014*1;  //cross section *BR (barn)
  float Luminosity      =0.5e9;     //barn-1  
  float NBkgEvents      = (BkgCrossSection*Luminosity);
  float NBkgFileEvents  =900000;
  float scaleBkg        =(NBkgEvents/NBkgFileEvents);
  fHist2ElecMassGenb->Scale(scaleBkg);
  fHist2ElecMassRecb->Scale(scaleBkg);
  //first: background approximation with poly1
  fHist2ElecMassRecb->Fit("pol4","R","",8.0,20.0);
  fHist2ElecMassRecb->Draw("E");
  //get this polinomial parameters
  TF1* fitPoly = (TF1*) fHist2ElecMassRecb->GetFunction("pol4");
  Double_t * param = fitPoly->GetParameters();

//load the analysed histo file for upsilon signal 
 TFile *fil2 = new TFile(filein2); 
 
 float UpsCrossSection =0.078e-3*0.0248;  //cross section *BR (barn)
 float NUpsEvents      =(UpsCrossSection*Luminosity);
 float NSigFileEvents  =50000;
 float scaleSig        =(NUpsEvents/NSigFileEvents);
 //get the analysed histograms for upsilon signal 
 TH1D *fHist2ElecMassGen = (TH1D*)gDirectory->Get("fHistGenDiElecMinv"); 
 TH1D *fHist2ElecMassRec = (TH1D*)gDirectory->Get("fHistRecDiElecMinv");
 fHist2ElecMassRec-> Sumw2();
 //scaled according to signal cross sections for L=0.5(nb)-1
 fHist2ElecMassGen->Scale(scaleSig);
 fHist2ElecMassRec->Scale(scaleSig);
 
//add signal+Bkg
 TH1D* hurec_minv = new TH1D("totalinvmass","rec_U minv (GeV)",200,0.0,20.0);
 hurec_minv->SetLineColor(kBlue);
 hurec_minv->Add(fHist2ElecMassRecb,fHist2ElecMassRec);
 hurec_minv->Sumw2(); 
 // hurec_minv-> SetDefaultSumw2(Bool_t sumw2=kTRUE);
 new TCanvas;
 hurec_minv->Draw("E"); 
 //clone histo for background subtraction 
 TH1F* signal =(TH1F*)  hurec_minv->Clone();
 signal->SetName("SignalForBkgSub");
 

 //sig+Bkg fitting with three gaussian and polinomial of degree one
 //TF1* signal_background = new TF1("signal_background","gaus(0)+gaus(3)+gaus(6)+[9]+[10]*x " ,0.0,20.0);
 
 //*** TF1* signal_background = new TF1("signal_background","gaus(0)+[3]+[4]*x+[5]*x*x+[6]*x*x*x+[7]*x*x*x*x " ,0.0,20.0);
 TF1* signal_background = new TF1("signal_background","gaus(0)+[3]+[4]*x+[5]*x*x+[6]*x*x*x+[7]*x*x*x*x " ,8.0,20.0); 



 signal_background->SetParameters(2e7,9.5,0.1,param[0],param[1]);
 
 //Gaussian parameters
 signal_background->SetParLimits(0,0,200);
 signal_background->SetParLimits(1,9.2,9.6);
 signal_background->SetParLimits(2,0.05,0.15);
 
 /* 
 signal_background->SetParLimits(3,0,200);
 signal_background->SetParLimits(4,9.9,10.05);
 signal_background->SetParLimits(5,0.05,0.15);
  
 signal_background->SetParLimits(6,0,200);
 signal_background->SetParLimits(7,10.2,10.5);
 signal_background->SetParLimits(8,0.05,0.2);
 */ 
 
//Poly1 parameters
 signal_background->SetParLimits(3,param[0]-5e7,param[0]+5e7);//cst
 signal_background->SetParLimits(4,param[1]-5e7,param[1]+5e7);//x
 signal_background->SetParLimits(5,param[0]-5e7,param[2]+5e7);//x2
 signal_background->SetParLimits(6,param[1]-5e7,param[3]+5e7);//x3
 signal_background->SetParLimits(7,param[1]-5e7,param[4]+5e7);//x3

 hurec_minv->Fit("signal_background","RI","",8.0,20.0);
 hurec_minv->Draw("E"); 
 
 TF1* background = new TF1("background","pol4",8.0,20.0);
 Double_t * paramBack = signal_background->GetParameters();
 background->SetParameters(&paramBack[3]);
 
 // background substraction
 // SignalForBkgSub->Add(background,-1,"I");
 signal->Add(background,-1,"I");
 
 
 // TF1* signal_background_sub = new TF1("signal_background_sub","gaus(0)+gaus(3)+gaus(6)",0.0,20.0);

 TF1* signal_background_sub = new TF1("signal_background_sub","gaus(0)",8.0,20.0); 
//Gaussian parameters
 signal_background_sub->SetParLimits(0,0,200);
 signal_background_sub->SetParLimits(1,9.2,9.6);
 signal_background_sub->SetParLimits(2,0.05,0.15);
 
 
 /*
 signal_background_sub->SetParLimits(3,0,200);
 signal_background_sub->SetParLimits(4,9.9,10.05);
 // signal_background_sub->SetParLimits(4,10.0,10.05);
 signal_background_sub->SetParLimits(5,0.05,0.15);
 signal_background_sub->SetParLimits(6,0,200);
 signal_background_sub->SetParLimits(7,10.2,10.5);
 signal_background_sub->SetParLimits(8,0.05,0.2);
 */
 new TCanvas;
 signal->Fit("signal_background_sub","RI","",8.0,20.0);
 signal->Draw("E");  

}
 
