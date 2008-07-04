//*************************************************************************************************************************
//This is a root macro which take analysed histogram files made by DiMuonAnalyzer and then mix Upsilon signal and dimuon 
//Bkgaccording to their cross sections for 0.5 (nb)-1 intgrated luminosity. this macro fitt a polinomial of degree one to 
//dimuon bkg then take the parameter of this polinomial and then fitt this polinomial of degree one + three gaussian to 
//upsilon signal+dimuon Bkg.also it fit three gaussian to a bkg subtracted upsilon signal.
 
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
void Normalization()
{

//load the analysed histo file for dimuon BKG
  TFile *fil1 = new TFile("/afs/cern.ch/user/k/kumarv/scratch0/CMSSW_1_8_0/src/Ana/DiMuonAnalyzerTest/Run/DiMuonAnalysisBkg.root"); 
  TH1D *fHist2muMassGenb = (TH1D*)gDirectory->Get("fHistDiMuonGen_InvM"); 
  TH1D *fHist2muMassRecb = (TH1D*)gDirectory->Get("fHistDiMuonRec_InvM");
  fHist2muMassRecb-> Sumw2();//
  float BkgCrossSection =0.0006*1;  //cross section *BR (barn)
  float Luminosity      =0.5e9;     //barn-1  
  float NBkgEvents      = (BkgCrossSection*Luminosity);
  float NBkgFileEvents  =200000;
  float scaleBkg        =(NBkgEvents/NBkgFileEvents);
  fHist2muMassGenb->Scale(scaleBkg);
  fHist2muMassRecb->Scale(scaleBkg);
  //first: background approximation with poly1
  fHist2muMassRecb->Fit("pol1","R","",6.0,12.0);
  fHist2muMassRecb->Draw("E");
  //get this polinomial parameters
  TF1* fitPoly = (TF1*) fHist2muMassRecb->GetFunction("pol1");
  Double_t * param = fitPoly->GetParameters();

//load the analysed histo file for upsilon signal 
 TFile *fil2 = new TFile("/afs/cern.ch/user/k/kumarv/scratch0/CMSSW_1_8_0/src/Ana/DiMuonAnalyzerTest/Run/DiMuonAnalysisSig.root"); 
 
 float UpsCrossSection =0.078e-3*0.0248;  //cross section *BR (barn)
 float NUpsEvents      = (UpsCrossSection*Luminosity);
 float NSigFileEvents  =50000;
 float scaleSig        =(NUpsEvents/NSigFileEvents);
 //get the analysed histograms for upsilon signal 
 TH1D *fHist2muMassGen = (TH1D*)gDirectory->Get("fHistDiMuonGen_InvM"); 
 TH1D *fHist2muMassRec = (TH1D*)gDirectory->Get("fHistDiMuonRec_InvM");
 fHist2muMassRec-> Sumw2();
 //scaled according to signal cross sections for L=0.5(nb)-1
 fHist2muMassGen->Scale(scaleSig);
 fHist2muMassRec->Scale(scaleSig);
 
//add signal+Bkg
 TH1D* hurec_minv = new TH1D("totalinvmass","rec_U minv (GeV)",150,7.0,12.0);
 hurec_minv->SetLineColor(kBlue);
 hurec_minv->Add(fHist2muMassRecb,fHist2muMassRec);
 hurec_minv->Sumw2(); 
 // hurec_minv-> SetDefaultSumw2(Bool_t sumw2=kTRUE);
 new TCanvas;
 hurec_minv->Draw("E"); 
 //clone histo for background subtraction 
 TH1F* signal =(TH1F*)  hurec_minv->Clone();
 signal->SetName("SignalForBkgSub");
 

 //sig+Bkg fitting with three gaussian and polinomial of degree one
 TF1* signal_background = new TF1("signal_background","gaus(0)+gaus(3)+gaus(6)+[9]+[10]*x " ,7.0,12.0);
 signal_background->SetParameters(2e7,9.5,0.1,param[0],param[1]);
 
 //Gaussian parameters
 signal_background->SetParLimits(0,0,200);
 signal_background->SetParLimits(1,9.4,9.55);
 signal_background->SetParLimits(2,0.05,0.15);
 
 
 signal_background->SetParLimits(3,0,200);
 signal_background->SetParLimits(4,9.9,10.05);
 signal_background->SetParLimits(5,0.05,0.15);
  
 signal_background->SetParLimits(6,0,200);
 signal_background->SetParLimits(7,10.2,10.5);
 signal_background->SetParLimits(8,0.05,0.2);
 
 //Poly1 parameters
 signal_background->SetParLimits(9,param[0]-5e7,param[0]+5e7);//cst
 signal_background->SetParLimits(10,param[1]-5e7,param[1]+5e7);//x
 hurec_minv->Fit("signal_background","RI","",7.00,12);
 hurec_minv->Draw("E"); 
 
 TF1* background = new TF1("background","pol1",7,12);
 Double_t * paramBack = signal_background->GetParameters();
 background->SetParameters(&paramBack[9]);
 
 // background substraction
 // SignalForBkgSub->Add(background,-1,"I");
 signal->Add(background,-1,"I");
 
 
 TF1* signal_background_sub = new TF1("signal_background_sub","gaus(0)+gaus(3)+gaus(6)",7.0,12.0);
 
//Gaussian parameters
 signal_background_sub->SetParLimits(0,0,200);
 signal_background_sub->SetParLimits(1,9.0,9.8);
 signal_background_sub->SetParLimits(2,0.05,0.15);
 
 signal_background_sub->SetParLimits(3,0,200);
 signal_background_sub->SetParLimits(4,9.9,10.05);
 // signal_background_sub->SetParLimits(4,10.0,10.05);
 signal_background_sub->SetParLimits(5,0.05,0.15);
 signal_background_sub->SetParLimits(6,0,200);
 signal_background_sub->SetParLimits(7,10.2,10.5);
 signal_background_sub->SetParLimits(8,0.05,0.2);
 
 new TCanvas;
 signal->Fit("signal_background_sub","RI","",6.0,12.0);
 signal->Draw("E");  

}
