
#include <TObjArray.h>
#include <TTree.h>
#include "stdio.h"
#else
#endif

void InvariantMassUPCDimuons()
{

 TFile *fil1 = new TFile("/afs/cern.ch/user/k/kumarv/scratch0/CMSSW_1_8_0/src/UPC/UpcAnalyzer/Run/UpcAnalysisBkgWoc.root"); 

 TH1D *fHist2muMassGenb = (TH1D*)gDirectory->Get("Gen Up inv mass"); 
 TH1D *fHist2muMassRecb = (TH1D*)gDirectory->Get("rec Up inv mass");

 //backgd mass
 fHist2muMassRecb-> Sumw2();
 //fHist2muMassRecb-> SetDefaultSumw2(Bool_t sumw2=kTRUE);
 fHist2muMassRecb->SetTitle("rec Bkg ");
 fHist2muMassRecb->SetName("fHist2muMassRecb"); //rec dimuon mass");
 fHist2muMassRecb->Fit("pol5","R","",6.0,12.0);

 // first: background approximation with poly0 or poly1
 fHist2muMassRecb->Draw("E");
 //get this polinomial parameters

 TF1* fitPoly = (TF1*) fHist2muMassRecb->GetFunction("pol0");
 Double_t * param = fitPoly->GetParameters();

 //get signal 
 TFile *fil2 = new TFile("/afs/cern.ch/user/k/kumarv/scratch0/CMSSW_1_8_0/src/UPC/UpcAnalyzer/Run/UpcAnalysisSigWOU.root"); 
 TH1D *fHist2muMassGen = (TH1D*)gDirectory->Get("Gen Up inv mass"); 
 TH1D *fHist2muMassRec = (TH1D*)gDirectory->Get("rec Up inv mass");
 fHist2muMassRec-> Sumw2();

 double ContOverUps_dimuon = 0.0175; // ratio of continuum/Upsilon cross-sections according to Starlight

 fHist2muMassRec->Scale(ContOverUps_dimuon);
 TCanvas *c1 = new TCanvas(); 
 fHist2muMassRec->Draw();


 //fHist2muMassRec-> SetDefaultSumw2(Bool_t sumw2=kTRUE);
 //add signal+Bkg
 TH1D* hurec_minv = new TH1D("total inv mass ","rec_U minv (GeV)",150,7.0,12.0);
 
 hurec_minv->Add(fHist2muMassRecb,fHist2muMassRec,1,1);
 hurec_minv->Sumw2(); 
 // hurec_minv-> SetDefaultSumw2(Bool_t sumw2=kTRUE);

 TCanvas *c2 = new TCanvas();
 hurec_minv->Draw("E");   

 TH1F* signal =(TH1F*)  hurec_minv->Clone();
  signal->SetName("signal");
  
  TF1* signal_background = new TF1("signal_background","gaus(0)+gaus(3)+gaus(6)+[9]+[10]*x+[11]*x**2+[12]*x**3+[13]*x**4+[14]*x**5",7.0,12.0);
  
  signal_background->SetParameters(2e7,9.5,0.1,param[0],param[1],param[2],param[3],param[4],param[5]); 
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
  
  //Poly5 parameters
  signal_background->SetParLimits(9,param[0]-5e7,param[0]+5e7);//cst
  signal_background->SetParLimits(10,param[1]-5e7,param[1]+5e7);//x
  signal_background->SetParLimits(11,param[2]-5e7,param[2]+5e7);//x2
  signal_background->SetParLimits(12,param[3]-1e7,param[3]+1e7);//x3
  signal_background->SetParLimits(13,param[4]-5e5,param[4]+5e5);//x4
  signal_background->SetParLimits(14,param[5]-1e4,param[5]+1e4);
  
  hurec_minv->Fit("signal_background","RI","",7.00,12);
  hurec_minv->Draw("E"); 

  TF1* background = new TF1("background","pol5",7,12);
  Double_t * paramBack = signal_background->GetParameters();
  background->SetParameters(&paramBack[9]);
  
  // background substraction
  signal->Add(background,-1,"I");
  
  TF1* signal_background_sub = new TF1("signal_background_sub","gaus(0)+gaus(3)+gaus(6)",7.0,12.0); 
 //TF1* signal_background_sub = new TF1("signal_background_sub","gaus(0)",7.0,12.0);

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

   TCanvas *c3 = new TCanvas;
  signal->Fit("signal_background_sub","RI","",6.0,12.0);
  
  signal->Draw("E");  
  
}
