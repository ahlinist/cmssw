#include <iostream>
#include <fstream>
#include <string.h>
#include <cmath>
#include <iomanip>
#include <TFile.h>
#include <TH1F.h>
#include <TF1.h>
#include <TMath.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include "UserDefinedData.h"
#include "Functions.h"
#include "Functions.cc"
using namespace std;

void L3Correction_my()
{
 gROOT->SetStyle("Plain");
  
 gStyle->SetPalette(1);
 gStyle->SetOptStat(1111111);  // Show overflow, underflow + SumOfWeights 
 //  gStyle->SetStatStyle(0); // for a completely transparent stat box
 // gStyle->SetOptFit(111110); 
 gStyle->SetOptFile(1); 
  
 gStyle->SetMarkerStyle(20);
 gStyle->SetMarkerSize(.3);
 gStyle->SetMarkerColor(1);
  
 gStyle->SetTitleBorderSize(0);  // no border around histogram title (font size can't be calculated anyways ...)
  
 gROOT->ForceStyle();
 gStyle->SetOptStat(0);

 const int NPtBins = 25; 
 char name[100];
 int i,auxi;
 TFile *inf2;
 inf2 = new TFile("/afs/cern.ch/user/d/delre/scratch0/CMSSW/CMSSW_2_0_7_new/src/csa08_finalplots_02_06_kt.root");
 TH1F *h2;
 sprintf(name,"biassum_sig");
 h2 = (TH1F*)inf2->Get(name);
 TFile *inf;
 inf = new TFile("/afs/cern.ch/user/d/delre/scratch0/CMSSW/CMSSW_2_0_7_new/src/csa08_finalplots_02_06_kt.root");
 TH1F *h;
 sprintf(name,"biassum_sig");
 h = (TH1F*)inf->Get(name);
 const int myNGenPtBins = 11;
 double Response[myNGenPtBins],Correction[NPtBins],ResponseError[myNGenPtBins];
 double xGenPt[myNGenPtBins],exGenPt[myNGenPtBins],gen_pt,calo_pt;
 double MinCaloPt,MaxCaloPt,MinGenPt,MaxGenPt;
 double xCaloPt[NPtBins] = {4,6,8,10,15,20,30,40,50,75,100,150,200,300,400,500,750,1000,1500,2000,2500,3000,3500,4000,5000};
 double xCaloPt_aux[NPtBins]; 
 TFile *outf;
 TF1 *RespFit;
 TF1 *CorFit;

 TGraphErrors *g_Resp;
 TGraph *g_Cor;

 ofstream L3ResponseFile;
 ofstream L3CorrectionFile;
 L3ResponseFile.open(L3ResponseFilename);
 L3CorrectionFile.open(L3CorrectionFilename);

//  ///////////// GenPt /////////////////////////////////////
//  sprintf(name,"MeanGenPt_Barrel");
//  h = (TH1F*)inf->Get(name);
//  for(i=0;i<myNGenPtBins;i++)
//    xGenPt[i] = h->GetBinContent(i+1);
 ///////////////////////// Gen Response //////////////////////////////////////////////
 for(i=0;i<myNGenPtBins;i++)
   {
     Response[i] = h2->GetBinContent(i+1);
     ResponseError[i] = h2->GetBinError(i+1);       
     if(ResponseError[i]<0.00001) ResponseError[i] = Response[i]/10.;
     xGenPt[i] = h2->GetBinCenter(i+1);
     exGenPt[i] = 0.;
     cout << i << "   " << Response[i] <<  "  " << ResponseError[i] << "   " << xGenPt[i] << endl;
   }
 
 TCanvas *c0 = new TCanvas("c0","--c0--",472,0,800,800);
 g_Resp = new TGraphErrors(myNGenPtBins,xGenPt,Response,exGenPt,ResponseError);
 sprintf(name,"RespFit");
 RespFit = new TF1(name,"[0]-[1]/(pow(log10(x),[2])+[3])+[4]/x",40.,h2->GetBinLowEdge(h2->GetNbinsX()+1)); 
 // RespFit = new TF1(name,"[0]-[1]/(pow(log10(x),[2])+[3])+[4]/x",40.,500.); 
 RespFit->SetParameter(0,1.);
 RespFit->SetParameter(1,5.);
 RespFit->SetParameter(2,5.);
 RespFit->SetParameter(3,2.);
 RespFit->SetParameter(4,1.);
 RespFit->SetParameter(5,1.);
 RespFit->SetParLimits(2,0.,10000.);
 RespFit->SetParLimits(3,0.,10000.);
 g_Resp->Fit(RespFit,"MR");
 c0->Clear();
 g_Resp->SetTitle("");
 g_Resp->SetMinimum(0.3);
 g_Resp->SetMaximum(1.);
 g_Resp->SetMarkerStyle(8);
 g_Resp->SetMarkerSize(1.);
 g_Resp->SetMarkerStyle(24);
 g_Resp->SetMarkerColor(kRed);
 g_Resp->SetLineColor(kRed);
 g_Resp->GetXaxis()->SetTitle("p_{T}[GeV/c]");
 g_Resp->GetYaxis()->SetTitle("#frac{p_{T}(jet)}{p_{T}(#gamma)}");
 g_Resp->GetYaxis()->SetTitleOffset(.8);
 g_Resp->GetXaxis()->SetRange(2.,2000.);
 g_Resp->Draw("AP");
 // RespFit->Draw("same");
 c0->SaveAs("corr_lin.eps");
 c0->SetLogx();
 g_Resp->Draw("AP");
 // RespFit->Draw("same");
 c0->SaveAs("corr_log.eps");
 /////////////////////////////////////////////////////////
 
 ///////////////////////////////////////////////////////// 
 MinGenPt = xGenPt[0];
 MaxGenPt = xGenPt[myNGenPtBins-1];
 MinCaloPt = xCaloPt[0];
 MaxCaloPt = xCaloPt[NPtBins-1]; 
 auxi = 0;
 ///////Calculating the L3 Correction vs CaloPt from Response vs GenPt ///////////////
 for(i=0;i<NPtBins;i++)
   { 
     calo_pt = xCaloPt[i]; 
     Invert(RespFit,MinGenPt,MaxGenPt,calo_pt,gen_pt);
     if (gen_pt>0)//If gen_pt=0 (pathological case) the point is not taken into account.
       {
         Correction[auxi] = gen_pt/calo_pt;
         xCaloPt_aux[auxi] = calo_pt;
	 cout << auxi << "   " << xCaloPt_aux[auxi] <<  "  " << Correction[auxi] << endl;
         auxi++;
       }
   }
 g_Cor = new TGraph(auxi,xCaloPt_aux,Correction);
 sprintf(name,"CorFit");
 CorFit = new TF1(name,"[0]+[1]/(pow(log10(x),[2])+[3])",MinCaloPt,MaxCaloPt);
 CorFit->SetParameter(0,1.);
 CorFit->SetParameter(1,1.);
 CorFit->SetParameter(2,1.);
 CorFit->SetParameter(3,1.);
 CorFit->SetParameter(4,1.);
 c0->Clear();
 g_Cor->Fit(CorFit,""); 
 g_Cor->Draw("AP");
 c0->SaveAs("pippo4.eps");
 
 //////////////////////////// Writing ///////////////////////////////////////////// 
 L3ResponseFile.setf(ios::left);
 L3ResponseFile <<setw(12)<<RespFit->GetParameter(0)
                   <<setw(12)<<RespFit->GetParameter(1)
                   <<setw(12)<<RespFit->GetParameter(2)
                   <<setw(12)<<RespFit->GetParameter(3)
                   <<setw(12)<<RespFit->GetParameter(4);
 L3ResponseFile.close();

 L3CorrectionFile.setf(ios::left);
 L3CorrectionFile <<setw(12)<<-5.191
                     <<setw(12)<<5.191
                     <<setw(12)<<(int)6 
                     <<setw(12)<<MinCaloPt
                     <<setw(12)<<MaxCaloPt
                     <<setw(12)<<CorFit->GetParameter(0)
                     <<setw(12)<<CorFit->GetParameter(1)
                     <<setw(12)<<CorFit->GetParameter(2)
                     <<setw(12)<<CorFit->GetParameter(3);
 L3CorrectionFile.close(); 

 outf = new TFile(L3OutputROOTFilename,"RECREATE");  
 sprintf(name,"Response_vs_GenPt");    
 g_Resp->Write(name);
 sprintf(name,"Correction_vs_CaloPt");    
 g_Cor->Write(name);
 outf->Close();
}
