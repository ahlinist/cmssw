#define gjet_response_fit_csa08_cxx
#include "gjet_response_fit_csa08.h"
#include <TF1.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

void gjet_response_fit_csa08::Loop(bool usew)
{
//   In a ROOT session, you can do:
//      Root > .L gjet_response_fit_csa08.C
//      Root > gjet_response_fit_csa08 t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   double pi = 3.14159;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      
      if(!usew) weight = 1.;

      ptphot_nocut->Fill(ptphot,weight);
      response_nocut->Fill(ptjet/ptphot,weight);
      responsevspt_nocut->Fill(ptphot,ptjet/ptphot,weight);

      bool ptcut = ptphot > 28;
      //      bool pt2jetcut = pt2jet/ptphot < cut2jet;
      bool pt2jetcut = pt2jet/ptphot < 0.09;
      bool nnphotcut;
      if(ptphot<60) nnphotcut = nniso > 1.007;
      if(ptphot<100 && ptphot>60)  nnphotcut = nniso > .999;  
      if(ptphot<200 && ptphot>100)   nnphotcut = nniso > .98;   
      if(ptphot>200)     nnphotcut = nniso > .999;             
  //       else nnphotcut = nniso_int > cutnn;
//       nnphotcut = nniso > .999;
      bool etacut = TMath::Abs(etaphot) < 1.2; 
      bool etajetcut = TMath::Abs(etajet) < cutetajet; 
      bool deltaphicut = TMath::Abs(phijet - phiphot) - pi<.2 && TMath::Abs(phijet - phiphot) - pi>-.2;      
      bool allcuts = nnphotcut && ptcut && etacut && etajetcut && deltaphicut && pt2jetcut;
      
      if(issignal && isphoton){
	ptphot_nocut_sig->Fill(ptphot,weight);
	response_nocut_sig->Fill(ptjet/ptphot,weight);
	responsevspt_nocut_sig->Fill(ptphot,ptjet/ptphot,weight);	
	if(ptphot<60)                 response_nocut_sig_region1->Fill(ptjet/ptphot,weight);
	if(ptphot<100 && ptphot>60)   response_nocut_sig_region2->Fill(ptjet/ptphot,weight);
	if(ptphot<200 && ptphot>100)  response_nocut_sig_region3->Fill(ptjet/ptphot,weight);
	if(ptphot>200)                response_nocut_sig_region4->Fill(ptjet/ptphot,weight);
	if(ptcut && pt2jetcut &&  etacut && etajetcut && deltaphicut){
	  if(ptphot<60)                 nn_region1_sig->Fill(nniso,weight);
	  if(ptphot<100 && ptphot>60)   nn_region2_sig->Fill(nniso,weight);
	  if(ptphot<200 && ptphot>100)  nn_region3_sig->Fill(nniso,weight);
	  if(ptphot>200)                nn_region4_sig->Fill(nniso,weight);
	}
      } else {
	ptphot_nocut_bkg->Fill(ptphot,weight);
	response_nocut_bkg->Fill(ptjet/ptphot,weight);
	responsevspt_nocut_bkg->Fill(ptphot,ptjet/ptphot,weight);	
	if(ptcut && pt2jetcut &&  etacut && etajetcut && deltaphicut){
	  if(ptphot<60)                 nn_region1_bkg->Fill(nniso,weight);
	  if(ptphot<100 && ptphot>60)   nn_region2_bkg->Fill(nniso,weight);
	  if(ptphot<200 && ptphot>100)  nn_region3_bkg->Fill(nniso,weight);
	  if(ptphot>200)                nn_region4_bkg->Fill(nniso,weight);
	}
      }
      
      if(allcuts){
	ptphot_allcut->Fill(ptphot,weight);
	response_allcut->Fill(ptjet/ptphot,weight);
	responsevspt_allcut->Fill(ptphot,ptjet/ptphot,weight);
	if(ptphot<60)                 response_allcut_region1->Fill(ptjet/ptphot,weight);
	if(ptphot<100 && ptphot>60)   response_allcut_region2->Fill(ptjet/ptphot,weight);
	if(ptphot<200 && ptphot>100)  response_allcut_region3->Fill(ptjet/ptphot,weight);
	if(ptphot>200)                response_allcut_region4->Fill(ptjet/ptphot,weight);
	
	if(issignal && isphoton){
	  ptphot_allcut_sig->Fill(ptphot,weight);
	  response_allcut_sig->Fill(ptjet/ptphot,weight);
	  responsevspt_allcut_sig->Fill(ptphot,ptjet/ptphot,weight);	
	  if(ptphot<60)                 response_allcut_sig_region1->Fill(ptjet/ptphot,weight);
	  if(ptphot<100 && ptphot>60)   response_allcut_sig_region2->Fill(ptjet/ptphot,weight);
	  if(ptphot<200 && ptphot>100)  response_allcut_sig_region3->Fill(ptjet/ptphot,weight);
	  if(ptphot>200)                response_allcut_sig_region4->Fill(ptjet/ptphot,weight);
	} else {
	  ptphot_allcut_bkg->Fill(ptphot,weight);
	  response_allcut_bkg->Fill(ptjet/ptphot,weight);
	  responsevspt_allcut_bkg->Fill(ptphot,ptjet/ptphot,weight);	
	  if(ptphot<60)                 response_allcut_bkg_region1->Fill(ptjet/ptphot,weight);
	  if(ptphot<100 && ptphot>60)   response_allcut_bkg_region2->Fill(ptjet/ptphot,weight);
	  if(ptphot<200 && ptphot>100)  response_allcut_bkg_region3->Fill(ptjet/ptphot,weight);
	  if(ptphot>200)                response_allcut_bkg_region4->Fill(ptjet/ptphot,weight);
	}
      }
   }
}

void gjet_response_fit_csa08::BookHistos()
{
  double bins[12];
  for (int i=0;i<8;i++){
    bins[i] = 30.+ 10*i;
  }
  for (int i=1;i<3;i++){
    bins[7+i] = 100.+ 50*i;
  }
  for (int i=1;i<2;i++){
    bins[9+i] = 300.+ 100*i;
  }
  for (int i=1;i<2;i++){
    bins[10+i] = 400.+ 300*i;
  }
  
  ptphot_nocut = new TH1D("ptphot_nocut","ptphot_nocut",11, bins);  ptphot_nocut->Sumw2();
  ptphot_allcut = new TH1D("ptphot_allcut","ptphot_allcut",11, bins); ptphot_allcut->Sumw2();
  response_nocut = new TH1D("response_nocut","response_nocut",YBINS,0.,2.); response_nocut->Sumw2();
  response_allcut = new TH1D("response_allcut","response_allcut",YBINS,0.,2.); response_allcut->Sumw2();
  responsevspt_nocut = new TH2D("responsevspt_nocut","responsevspt_nocut",11, bins,YBINS,0.,2.); responsevspt_nocut->Sumw2();
  responsevspt_allcut = new TH2D("responsevspt_allcut","responsevspt_allcut",11, bins,YBINS,0.,2.); responsevspt_allcut->Sumw2();
  response_allcut_region1 = new TH1D("response_allcut_region1","response_allcut",YBINS,0.,2.); response_allcut_region1->Sumw2();
  response_allcut_region2 = new TH1D("response_allcut_region2","response_allcut",YBINS,0.,2.); response_allcut_region2->Sumw2();
  response_allcut_region3 = new TH1D("response_allcut_region3","response_allcut",YBINS,0.,2.); response_allcut_region3->Sumw2();
  response_allcut_region4 = new TH1D("response_allcut_region4","response_allcut",YBINS,0.,2.); response_allcut_region4->Sumw2();

  ptphot_nocut_sig = new TH1D("ptphot_nocut_sig","ptphot_nocut_sig",11, bins);  ptphot_nocut_sig->Sumw2();
  ptphot_allcut_sig = new TH1D("ptphot_allcut_sig","ptphot_allcut_sig",11, bins); ptphot_allcut_sig->Sumw2();
  response_nocut_sig = new TH1D("response_nocut_sig","response_nocut_sig",YBINS,0.,2.); response_nocut_sig->Sumw2();
  response_allcut_sig = new TH1D("response_allcut_sig","response_allcut_sig",YBINS,0.,2.); response_allcut_sig->Sumw2();
  responsevspt_nocut_sig = new TH2D("responsevspt_nocut_sig","responsevspt_nocut_sig",11, bins,YBINS,0.,2.); responsevspt_nocut_sig->Sumw2();
  responsevspt_allcut_sig = new TH2D("responsevspt_allcut_sig","responsevspt_allcut_sig",11, bins,YBINS,0.,2.); responsevspt_allcut_sig->Sumw2();
  response_nocut_sig_region1 = new TH1D("response_nocut_sig_region1","response_nocut",YBINS,0.,2.); response_nocut_sig_region1->Sumw2();
  response_nocut_sig_region2 = new TH1D("response_nocut_sig_region2","response_nocut",YBINS,0.,2.); response_nocut_sig_region2->Sumw2();
  response_nocut_sig_region3 = new TH1D("response_nocut_sig_region3","response_nocut",YBINS,0.,2.); response_nocut_sig_region3->Sumw2();
  response_nocut_sig_region4 = new TH1D("response_nocut_sig_region4","response_nocut",YBINS,0.,2.); response_nocut_sig_region4->Sumw2();
  response_allcut_sig_region1 = new TH1D("response_allcut_sig_region1","response_allcut",YBINS,0.,2.); response_allcut_sig_region1->Sumw2();
  response_allcut_sig_region2 = new TH1D("response_allcut_sig_region2","response_allcut",YBINS,0.,2.); response_allcut_sig_region2->Sumw2();
  response_allcut_sig_region3 = new TH1D("response_allcut_sig_region3","response_allcut",YBINS,0.,2.); response_allcut_sig_region3->Sumw2();
  response_allcut_sig_region4 = new TH1D("response_allcut_sig_region4","response_allcut",YBINS,0.,2.); response_allcut_sig_region4->Sumw2();

  ptphot_nocut_bkg = new TH1D("ptphot_nocut_bkg","ptphot_nocut_bkg",11, bins);  ptphot_nocut_bkg->Sumw2();
  ptphot_allcut_bkg = new TH1D("ptphot_allcut_bkg","ptphot_allcut_bkg",11, bins); ptphot_allcut_bkg->Sumw2();
  response_nocut_bkg = new TH1D("response_nocut_bkg","response_nocut_bkg",YBINS,0.,2.); response_nocut_bkg->Sumw2();
  response_allcut_bkg = new TH1D("response_allcut_bkg","response_allcut_bkg",YBINS,0.,2.); response_allcut_bkg->Sumw2();
  responsevspt_nocut_bkg = new TH2D("responsevspt_nocut_bkg","responsevspt_nocut_bkg",11, bins,YBINS,0.,2.); responsevspt_nocut_bkg->Sumw2();
  responsevspt_allcut_bkg = new TH2D("responsevspt_allcut_bkg","responsevspt_allcut_bkg",11, bins,YBINS,0.,2.); responsevspt_allcut_bkg->Sumw2();
  response_allcut_bkg_region1 = new TH1D("response_allcut_bkg_region1","response_allcut",YBINS,0.,2.); response_allcut_bkg_region1->Sumw2();
  response_allcut_bkg_region2 = new TH1D("response_allcut_bkg_region2","response_allcut",YBINS,0.,2.); response_allcut_bkg_region2->Sumw2();
  response_allcut_bkg_region3 = new TH1D("response_allcut_bkg_region3","response_allcut",YBINS,0.,2.); response_allcut_bkg_region3->Sumw2();
  response_allcut_bkg_region4 = new TH1D("response_allcut_bkg_region4","response_allcut",YBINS,0.,2.); response_allcut_bkg_region4->Sumw2();

  sob_vs_eff_1 = new TH2D("sob_vs_eff_1","sob_vs_eff_1",100,0,1.,100,0.,5.);
  sob_vs_eff_2 = new TH2D("sob_vs_eff_2","sob_vs_eff_2",100,0,1.,100,0.,5.);
  sob_vs_eff_3 = new TH2D("sob_vs_eff_3","sob_vs_eff_3",100,0,1.,100,0.,5.);
  sob_vs_eff_4 = new TH2D("sob_vs_eff_4","sob_vs_eff_4",100,0,1.,100,0.,5.);

  nn_region1_sig = new TH1D("nn_region1_sig","nn_region1_sig",400,-1.,1.1);
  nn_region2_sig = new TH1D("nn_region2_sig","nn_region2_sig",400,-1.,1.1);
  nn_region3_sig = new TH1D("nn_region3_sig","nn_region3_sig",400,-1.,1.1);
  nn_region4_sig = new TH1D("nn_region4_sig","nn_region4_sig",400,-1.,1.1);
  nn_region1_bkg = new TH1D("nn_region1_bkg","nn_region1_bkg",400,-1.,1.1);
  nn_region2_bkg = new TH1D("nn_region2_bkg","nn_region2_bkg",400,-1.,1.1);
  nn_region3_bkg = new TH1D("nn_region3_bkg","nn_region3_bkg",400,-1.,1.1);
  nn_region4_bkg = new TH1D("nn_region4_bkg","nn_region4_bkg",400,-1.,1.1);

}


void gjet_response_fit_csa08::ResetHistos()
{
  ptphot_nocut->Reset();
  ptphot_allcut->Reset();
  response_nocut->Reset();
  response_allcut->Reset();
  responsevspt_nocut->Reset();
  responsevspt_allcut->Reset();
  response_allcut_region1->Reset();
  response_allcut_region2->Reset();
  response_allcut_region3->Reset();
  response_allcut_region4->Reset();

  ptphot_nocut_sig->Reset();
  ptphot_allcut_sig->Reset();
  response_nocut_sig->Reset();
  response_allcut_sig->Reset();
  responsevspt_nocut_sig->Reset();
  responsevspt_allcut_sig->Reset();
  response_nocut_sig_region1->Reset();
  response_nocut_sig_region2->Reset();
  response_nocut_sig_region3->Reset();
  response_nocut_sig_region4->Reset();
  response_allcut_sig_region1->Reset();
  response_allcut_sig_region2->Reset();
  response_allcut_sig_region3->Reset();
  response_allcut_sig_region4->Reset();

  ptphot_nocut_bkg->Reset();
  ptphot_allcut_bkg->Reset();
  response_nocut_bkg->Reset();
  response_allcut_bkg->Reset();
  responsevspt_nocut_bkg->Reset();
  responsevspt_allcut_bkg->Reset();
  response_allcut_bkg_region1->Reset();
  response_allcut_bkg_region2->Reset();
  response_allcut_bkg_region3->Reset();
  response_allcut_bkg_region4->Reset();
}

void gjet_response_fit_csa08::Optimum()
{
  BookHistos();
  for(int j=0; j<11; j++){
    for(int i=0; i<21; i++){
      cut2jet = 0. + double(j)*3./100;    
      cutnn = 0.9 + double(i)/200;    
      ResetHistos();
      Loop(1);
      double S1_bef = response_nocut_sig_region1->Integral();
      double S2_bef = response_nocut_sig_region2->Integral();
      double S3_bef = response_nocut_sig_region3->Integral();
      double S4_bef = response_nocut_sig_region4->Integral();
      double S1 = response_allcut_sig_region1->Integral();
      double B1 = response_allcut_bkg_region1->Integral();
      double S2 = response_allcut_sig_region2->Integral();
      double B2 = response_allcut_bkg_region2->Integral();
      double S3 = response_allcut_sig_region3->Integral();
      double B3 = response_allcut_bkg_region3->Integral();
      double S4 = response_allcut_sig_region4->Integral();
      double B4 = response_allcut_bkg_region4->Integral();
      double eff1 = S1/S1_bef;
      double eff2 = S2/S2_bef;
      double eff3 = S3/S3_bef;
      double eff4 = S4/S4_bef;
      sob_vs_eff_1->Fill(eff1,S1/B1);
      sob_vs_eff_2->Fill(eff2,S2/B2);
      sob_vs_eff_3->Fill(eff3,S3/B3);
      sob_vs_eff_4->Fill(eff4,S4/B4);
      cout << "cutnn = " <<  cutnn << "; cut2jet = " << cut2jet << "; cutetajet = " << cutetajet << endl;
      cout << "S1/B1 = " << S1/B1 << "   " << "; eff1 = " << eff1 << endl;
      cout << "S2/B2 = " << S2/B2 << "   " << "; eff2 = " << eff2 << endl;
      cout << "S3/B3 = " << S3/B3 << "   " << "; eff3 = " << eff3 << endl;
      cout << "S4/B4 = " << S4/B4 << "   " << "; eff4 = " << eff4 << endl;
      cout << endl;
    }
  }
}

void gjet_response_fit_csa08::Fit()
{
  gROOT->SetStyle("Plain");
  
  gStyle->SetPalette(1);
  gStyle->SetOptStat(1111111);  // Show overflow, underflow + SumOfWeights 
  //  gStyle->SetStatStyle(0); // for a completely transparent stat box
  gStyle->SetOptFit(111110); 
  gStyle->SetOptFile(1); 
  
  gStyle->SetMarkerStyle(20);
  gStyle->SetMarkerSize(.3);
  gStyle->SetMarkerColor(1);
  
  gStyle->SetTitleBorderSize(0);  // no border around histogram title (font size can't be calculated anyways ...)
  
  gROOT->ForceStyle();
  gStyle->SetOptStat(0);
  char name[100];
  char nameout[100];
  sprintf(name,"%s","responsevspt");
  TCanvas *c0 = new TCanvas("c0","--c0--",472,0,800,800);
  TAxis *xaxis =  responsevspt_allcut_sig->GetXaxis();
  TAxis *yaxis =  responsevspt_allcut_sig->GetYaxis();
  int nbinx = responsevspt_allcut_sig->GetNbinsX();
  int nbiny = responsevspt_allcut_sig->GetNbinsY();
  
  TH1D temphist_sig("temphist_sig","",nbiny,yaxis->GetXmin(),yaxis->GetXmax());temphist_sig.Sumw2();
  temphist_sig.SetMarkerStyle(8);
  temphist_sig.SetMarkerColor(kBlue);
  temphist_sig.SetMarkerSize(.7);
  temphist_sig.SetStats(0);
  temphist_sig.SetXTitle("p_{T}(jet)/p_{T}(#gamma)");
  
  TH1D temphist_bkg("temphist_bkg","",nbiny,yaxis->GetXmin(),yaxis->GetXmax());temphist_bkg.Sumw2();
  temphist_bkg.SetMarkerStyle(8);
  temphist_bkg.SetMarkerColor(kBlue);
  temphist_bkg.SetMarkerSize(.7);
  temphist_bkg.SetStats(0);
  temphist_bkg.SetXTitle("p_{T}(jet)/p_{T}(#gamma)");
  
  TH1D temphist_all("temphist_all","",nbiny,yaxis->GetXmin(),yaxis->GetXmax());temphist_all.Sumw2();
  temphist_all.SetMarkerStyle(8);
  temphist_all.SetMarkerColor(kBlue);
  temphist_all.SetMarkerSize(.7);
  temphist_all.SetStats(0);
  temphist_all.SetXTitle("p_{T}(jet)/p_{T}(#gamma)");
  
  double bins[12];
  for (int i=0;i<8;i++){
    bins[i] = 30.+ 10*i;
  }
  for (int i=1;i<3;i++){
    bins[7+i] = 100.+ 50*i;
  }
  for (int i=1;i<2;i++){
    bins[9+i] = 300.+ 100*i;
  }
  for (int i=1;i<2;i++){
    bins[10+i] = 400.+ 300*i;
  }

  TH1D *biassum_sig = new TH1D("biassum_sig","biassum",11,bins);
  TH1D *resosum_sig = new TH1D("resosum_sig","resosum",11,bins);
  TH1D *distrsum_sig = new TH1D("distrsum_sig","distrsum",11,bins);
  
  TH1D *biassum_bkg = new TH1D("biassum_bkg","biassum",11,bins);
  TH1D *resosum_bkg = new TH1D("resosum_bkg","resosum",11,bins);
  
  TH1D *biassum_all = new TH1D("biassum_all","biassum",11,bins);
  TH1D *resosum_all = new TH1D("resosum_all","resosum",11,bins);
  
  for(int j=1; j<nbinx+1; j++){
    
    for(int i=1; i<nbiny+1; i++){      
      double binvalue = responsevspt_allcut_sig->GetBinContent(j,i);
      double binerror = responsevspt_allcut_sig->GetBinError(j,i);
      temphist_sig.SetBinContent(i,binvalue);          
      temphist_sig.SetBinError(i,binerror);          
      binvalue = responsevspt_allcut_bkg->GetBinContent(j,i);
      binerror = responsevspt_allcut_bkg->GetBinError(j,i);
      temphist_bkg.SetBinContent(i,binvalue);          
      temphist_bkg.SetBinError(i,binerror);          
      binvalue = responsevspt_allcut->GetBinContent(j,i);
      binerror = responsevspt_allcut->GetBinError(j,i);
      temphist_all.SetBinContent(i,binvalue);          
      temphist_all.SetBinError(i,binerror);          
    }
    
    Double_t params_sig[3] = {1000.,1.,.1};
    Double_t params_bkg[3] = {1000.,1.,.1};
    Double_t params_all[3] = {1000.,1.,.1};
    TF1 *gaussian = new TF1("gaussian","gaus",-1.,1.);
    gaussian->SetParameters(params_sig);
    gaussian->SetLineColor(kRed);
    gaussian->SetParLimits(0, 0., 10000.);
    gaussian->SetParLimits(1, 0., 2.);
    gaussian->SetParLimits(2, 0.00, 2.);
    gaussian->SetParLimits(0, 0.,temphist_sig.GetSum()*100.);
    gaussian->SetParameter(0,temphist_sig.GetSum());
    gaussian->SetParameter(1,1.);
    gaussian->SetParameter(2,.2);
    double minsigma = 1.5;
    if(j<5) minsigma = 1.;
    if(j<3) minsigma = .5;
    if(j<2) minsigma = .1;
    temphist_sig.Fit(gaussian,"mllq","pe");    
    temphist_sig.Fit(gaussian,"mllq","pe",gaussian->GetParameter(1)-gaussian->GetParameter(2),gaussian->GetParameter(1)+gaussian->GetParameter(2));        
    temphist_sig.Fit(gaussian,"mllq","pe",gaussian->GetParameter(1)-gaussian->GetParameter(2),gaussian->GetParameter(1)+2*gaussian->GetParameter(2));        
    temphist_sig.Fit(gaussian,"mllq","pe",gaussian->GetParameter(1)-minsigma*gaussian->GetParameter(2),gaussian->GetParameter(1)+3.*gaussian->GetParameter(2));        
    temphist_sig.Fit(gaussian,"mll","pe",gaussian->GetParameter(1)-minsigma*gaussian->GetParameter(2),gaussian->GetParameter(1)+3.*gaussian->GetParameter(2));        
    if(temphist_sig.Integral()<6) temphist_sig.Fit(gaussian,"mll","pe",gaussian->GetParameter(1)-0.2,gaussian->GetParameter(1)+0.2);  
    biassum_sig->SetBinContent(j,gaussian->GetParameter(1));
    resosum_sig->SetBinContent(j,gaussian->GetParameter(2));
    biassum_sig->SetBinError(j,gaussian->GetParError(1));
    resosum_sig->SetBinError(j,gaussian->GetParError(2));
    double numevents = temphist_sig.Integral();
    distrsum_sig->SetBinContent(j,numevents);
    sprintf(nameout,"%s%d%s","tempfits/tempfit_",j,".eps");
    c0->SaveAs(nameout); 
    cout << temphist_sig.Integral()<< "   " << gaussian->GetParameter(2)/sqrt(temphist_sig.Integral()) << "   " << gaussian->GetParError(1) << endl;;
    gaussian->Delete();
    
    TF1 *gaussian_all = new TF1("gaussian_all","gaus",-1.,1.);
//     gaussian_all->SetParameters(params_all);
    gaussian_all->SetLineColor(kRed);
    gaussian_all->SetParLimits(0, 0., temphist_all.GetSum()*2.);
    gaussian_all->SetParLimits(1, 0., 2.);
    gaussian_all->SetParLimits(2, 0.00, 2.);
    gaussian_all->SetParameter(0,temphist_all.GetSum()/10.);
//     temphist_all.Fit(gaussian_all,"mq","pe");    
    temphist_all.Fit(gaussian_all,"mllq","pe",gaussian_all->GetParameter(1)-gaussian_all->GetParameter(2),gaussian_all->GetParameter(1)+gaussian_all->GetParameter(2));        
    temphist_all.Fit(gaussian_all,"mllq","pe",gaussian_all->GetParameter(1)-gaussian_all->GetParameter(2),gaussian_all->GetParameter(1)+2*gaussian_all->GetParameter(2));        
    temphist_all.Fit(gaussian_all,"mllq","pe",gaussian_all->GetParameter(1)-1.5*gaussian_all->GetParameter(2),gaussian_all->GetParameter(1)+3*gaussian_all->GetParameter(2));        
//     biassum_all->SetBinContent(j,gaussian_all->GetParameter(1));
    resosum_all->SetBinContent(j,gaussian_all->GetParameter(2));
    biassum_all->SetBinError(j,gaussian_all->GetParError(1));
    resosum_all->SetBinError(j,gaussian_all->GetParError(2));      //       sprintf(nameout,"%s%d%s","tempfits/tempfit_",j,".eps");
//     gaussian_all->Delete();
    
    TF1 *gaussian_bkg = new TF1("gaussian_bkg","gaus",-1.,1.);
    gaussian_bkg->SetParameters(params_bkg);
    gaussian_bkg->SetLineColor(kRed);
    gaussian_bkg->SetParLimits(0, 0., temphist_bkg.GetSum()*2.);
    gaussian_bkg->SetParLimits(1,0 , 2.);
    gaussian_bkg->SetParLimits(2, 0.00, 2.);
    gaussian_bkg->SetParameter(0,temphist_bkg.GetSum()/10.);
    temphist_bkg.Fit(gaussian_bkg,"mllq","pe");    
    biassum_bkg->SetBinContent(j,gaussian_bkg->GetParameter(1));
    resosum_bkg->SetBinContent(j,gaussian_bkg->GetParameter(2));
    biassum_bkg->SetBinError(j,gaussian_bkg->GetParError(1));
    resosum_bkg->SetBinError(j,gaussian_bkg->GetParError(2));      //       sprintf(nameout,"%s%d%s","tempfits/tempfit_",j,".eps");
    gaussian_bkg->Delete();
    
  }
  
  //  biassum_sig->SetAxisRange(40.,3000.);
  biassum_sig->SetTitle("");
  biassum_sig->SetMinimum(0.3);
  biassum_sig->SetMaximum(1.1);
  biassum_sig->SetMarkerStyle(8);
  biassum_sig->SetMarkerSize(1.);
  biassum_sig->SetMarkerStyle(24);
  biassum_sig->SetMarkerColor(kRed);
  biassum_sig->SetLineColor(kRed);
  biassum_sig->SetXTitle("p_{T}[GeV/c]");
  biassum_sig->SetYTitle("#frac{p_{T}(jet)}{p_{T}(#gamma)}");
  biassum_sig->SetTitleOffset(.8,"Y");
  biassum_sig->Draw("pe");
  responsevspt_allcut_sig->Draw("same");
  biassum_sig->Draw("pesame");
  sprintf(nameout,"%s%s%s","biasoverlap_",name,"_sig.eps");
  c0->SaveAs(nameout);
  biassum_sig->Draw("pe");
  sprintf(nameout,"%s%s%s","bias_",name,"_sig.eps");
  c0->SaveAs(nameout);
  resosum_sig->SetMinimum(0);
  resosum_sig->SetMaximum(2.);
  resosum_sig->SetMarkerStyle(8);
  resosum_sig->SetMarkerColor(kRed);
  resosum_sig->Draw("pe");
  sprintf(nameout,"%s%s%s","reso_",name,"_sig.eps");
  c0->SaveAs(nameout);
  sprintf(nameout,"%s%s%s","reso_",name,"_sig.gif");
  c0->SaveAs(nameout);
  c0->SetLogy(1);
  distrsum_sig->SetTitle("");
  distrsum_sig->SetXTitle("p_{T}[GeV/c]");
  distrsum_sig->SetYTitle("Nev/fb^{-1}");
  distrsum_sig->SetTitleOffset(1.3,"Y");
  distrsum_sig->Draw("pe");
  sprintf(nameout,"%s%s%s","distr_",name,"_sig.eps");
  c0->SaveAs(nameout);
  

  c0->SetLogy(0);
  biassum_all->SetTitle("");
  biassum_all->SetMinimum(0.3);
  biassum_all->SetMaximum(1.1);
  biassum_all->SetMarkerStyle(8);
  biassum_all->SetMarkerSize(1.);
  biassum_all->SetXTitle("p_{T}[GeV/c]");
  biassum_all->SetYTitle("#frac{p_{T}(jet)}{p_{T}(#gamma)}");
  biassum_all->SetTitleOffset(.8,"Y");
  biassum_all->Draw("pe");
  sprintf(nameout,"%s%s%s","bias_",name,"_all.eps");
  c0->SaveAs(nameout);
  sprintf(nameout,"%s%s%s","bias_",name,"_all.gif");
  c0->SaveAs(nameout);
  
  TLegendEntry *legge;
  TLegend *leg;
  leg = new TLegend(0.45,0.15,0.8,0.35);
  leg->SetFillStyle(0); leg->SetBorderSize(0); leg->SetTextSize(0.05);
  leg->SetFillColor(0);
  legge = leg->AddEntry(biassum_sig, "#gamma jet only", "p");
  legge = leg->AddEntry(biassum_all, "#gamma jet + QCD bkg", "p");
  biassum_all->Draw("pe");
  biassum_sig->Draw("pesame");
  leg->Draw();
  sprintf(nameout,"%s%s%s","bias_",name,"_sig+all.eps");
  c0->SaveAs(nameout);
  sprintf(nameout,"%s%s%s","bias_",name,"_sig+all.gif");
  c0->SaveAs(nameout);
  

  biassum_bkg->SetMinimum(.5);
  biassum_bkg->SetMaximum(2.);
  biassum_bkg->SetMarkerStyle(8);
  biassum_bkg->SetMarkerColor(kRed);
  biassum_bkg->SetLineColor(kRed);
  biassum_bkg->Draw("pe");
  sprintf(nameout,"%s%s%s","bias_",name,"_bkg.eps");
  c0->SaveAs(nameout);

  
  
}

TChain * getchain(char *thechain) {

  TChain *chain = new TChain("AnaTree");
  std::cout << "Chaining ... " << thechain << std::endl;
  char pName[2000];
  char buffer[200];
  sprintf(buffer, "%s", thechain);
  ifstream is(buffer);
  cout << "files " << buffer <<  endl;
  while(is.getline(buffer, 200, '\n')){
    //    if (buffer[0] == '#') continue;
    sscanf(buffer, "%s", pName);
    std::cout << "   Add: " << buffer << std::endl;
    chain->Add(pName);
  }
  is.close();
  return chain;

}
