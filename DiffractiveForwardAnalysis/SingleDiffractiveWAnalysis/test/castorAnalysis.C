#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"
#include "TProfile.h"
#include "TString.h"
#include "TFile.h"
#include "TStyle.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TLegend.h"
#include "TRandom.h"
#include "TTree.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream> 
#include <vector>
// Trick to make CINT happy
//#include "DataFormats/FWLite/interface/Handle.h"
//#include "DataFormats/FWLite/interface/Event.h"
#if !defined(__CINT__) && !defined(__MAKECINT__)
//Headers for the data items
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/Common/interface/ValueMap.h"
#endif

void castorAnalysis(int maxEvents = -1, bool verbose = false) {
   // Create a vector of input files
   vector<string> fileNames;
   fileNames.push_back("rfio:/castor/cern.ch/user/a/antoniov/Analysis/SingleDiffractiveWmunu/InitialLumiPileUP/POMWIG_SDPlusWmunu_EdmDump_InitialLumPU_1.root");
   fileNames.push_back("rfio:/castor/cern.ch/user/a/antoniov/Analysis/SingleDiffractiveWmunu/InitialLumiPileUP/POMWIG_SDPlusWmunu_EdmDump_InitialLumPU_2.root");
   
   // Chain the input files
   fwlite::ChainEvent ev(fileNames);
   
   // Create output file
   TFile* hfile = new TFile("analysisCastor_histos.root","recreate","data histograms");

   // Book Histograms
   int nBins = 20;

   /*TH1F* h_nCastorGenPlus = new TH1F("nCastorGenPlus","Castor gen mult. plus",nBins,0,nBins);
   TH1F* h_nCastorTowerPlus = new TH1F("nCastorTowerPlus","Castor Towers mult. plus",nBins,0,nBins);
   TH2F* h_nCastorGenvsCastorTowerPlus = new TH2F("nCastorGenVsCastorTowerPlus","Castor Gen vs Castor towers mult. plus",nBins,0,nBins,nBins,0,nBins);*/

   size_t nThresholds = 50;
   TH1F* h_nCastorGenPlus = new TH1F("nCastorGenPlus","Castor gen mult. plus",nBins,0,nBins);
   TH1F* h_sumECastorGenPlus = new TH1F("sumECastorGenPlus","Castor gen sum E plus",100,0.,200.);
   vector<TH1F*> histos_nCastorTowerPlus;
   vector<TH2F*> histos_nCastorGenvsCastorTowerPlus;
   vector<TH1F*> histos_sumECastorTowerPlus;
   vector<TH2F*> histos_sumECastorGenvsCastorTowerPlus;
   char hName[50];
   for(size_t ithresh = 0; ithresh < nThresholds; ++ithresh){
     sprintf(hName,"nCastorTowerPlus_%d",ithresh);
     histos_nCastorTowerPlus.push_back(new TH1F(hName,"Castor Towers mult. plus",nBins,0,nBins));
     if(verbose) cout << "Created histogram " << (histos_nCastorTowerPlus.back())->GetName() << endl;
     sprintf(hName,"nCastorGenvsCastorTowerPlus_%d",ithresh);
     histos_nCastorGenvsCastorTowerPlus.push_back(new TH2F(hName,"Castor Gen vs Castor towers mult. plus",nBins,0,nBins,nBins,0,nBins));
     if(verbose) cout << "Created histogram " << (histos_nCastorGenvsCastorTowerPlus.back())->GetName() << endl;

     sprintf(hName,"sumECastorTowerPlus_%d",ithresh);
     histos_sumECastorTowerPlus.push_back(new TH1F(hName,"Castor Towers sum E plus",100,0.,200.));
     if(verbose) cout << "Created histogram " << (histos_sumECastorTowerPlus.back())->GetName() << endl;
     sprintf(hName,"sumECastorGenvsCastorTowerPlus_%d",ithresh);
     histos_sumECastorGenvsCastorTowerPlus.push_back(new TH2F(hName,"Castor Gen vs Castor towers sum E plus",100,0.,200.,100,0.,200.));
     if(verbose) cout << "Created histogram " << (histos_sumECastorGenvsCastorTowerPlus.back())->GetName() << endl;
   }

   // Loop over the events
   //int thresholdIndexCastor = 22;
 
   int nEvts = 0;
   for( ev.toBegin(); ! ev.atEnd(); ++ev) {

     if(nEvts == maxEvents) break;	
     ++nEvts;
     if(verbose) std::cout << ">>> Event number: " << nEvts << endl;	
     // Access castor gen
     fwlite::Handle<std::vector<unsigned int> > acceptedPhiSliceplus; 
     acceptedPhiSliceplus.getByLabel(ev,"castorGen","acceptedPhiSliceplus");

     fwlite::Handle<std::vector<double> > energyPhiSliceplus;
     energyPhiSliceplus.getByLabel(ev,"castorGen","energyPhiSliceplus");

     unsigned int nAccPhiSliceplus = 0;
     double sumECastorGen_plus = 0.;
     for(unsigned int i = 0; i < acceptedPhiSliceplus->size(); i++){
        if((*acceptedPhiSliceplus)[i] == 1) nAccPhiSliceplus++;
        sumECastorGen_plus += (*energyPhiSliceplus)[i];
     }

     unsigned int nCastorGen_plus = nAccPhiSliceplus;
     if(verbose) std::cout << ">>> Number of castor slices (gen) = " << nCastorGen_plus << std::endl;
     if(verbose) std::cout << ">>> Sum(E) of castor slices (gen) = " << sumECastorGen_plus << std::endl;
     h_nCastorGenPlus->Fill(nCastorGen_plus);
     h_sumECastorGenPlus->Fill(sumECastorGen_plus);

     // Access castor tower multiplicities
     fwlite::Handle<std::vector<unsigned int> > nCastorTowerPlus;
     nCastorTowerPlus.getByLabel(ev,"castorTower","nCastorTowerPlus");

     fwlite::Handle<std::vector<double> > sumECastorTowerPlus;
     sumECastorTowerPlus.getByLabel(ev,"castorTower","sumECastorTowerPlus");

     // Loop over thresholds
     for(size_t ithresh = 0; ithresh < nThresholds; ++ithresh){
        unsigned int nCastorTwr_plus = (*nCastorTowerPlus)[ithresh];
        histos_nCastorTowerPlus[ithresh]->Fill(nCastorTwr_plus);
        histos_nCastorGenvsCastorTowerPlus[ithresh]->Fill(nCastorGen_plus,nCastorTwr_plus);
        double sumECastorTwr_plus = (*sumECastorTowerPlus)[ithresh];
        histos_sumECastorTowerPlus[ithresh]->Fill(sumECastorTwr_plus);
        histos_sumECastorGenvsCastorTowerPlus[ithresh]->Fill(sumECastorGen_plus,sumECastorTwr_plus);
     }
     /*h_nCastorGenPlus->Fill(nCastorGen_plus);
     h_nCastorTowerPlus->Fill(nCastorTwr_plus);
     h_nCastorGenvsCastorTowerPlus->Fill(nCastorGen_plus,nCastorTwr_plus);*/
   }  // End loop over events
  
   // Get TProfiles from each 2d histo
   vector<TProfile*> prof_nCastorGenvsCastorTowerPlus;
   vector<TProfile*> prof_sumECastorGenvsCastorTowerPlus;
   TH1F* h_fitInterceptMult = new TH1F("fitInterceptMult","fitInterceptMult",nThresholds,0,nThresholds);
   TH1F* h_fitSlopeMult = new TH1F("fitSlopeMult","fitSlopeMult",nThresholds,0,nThresholds);
   TH1F* h_fitInterceptSumE = new TH1F("fitInterceptSumE","fitInterceptSumE",nThresholds,0,nThresholds);
   TH1F* h_fitSlopeSumE = new TH1F("fitSlopeSumE","fitSlopeSumE",nThresholds,0,nThresholds);
   for(size_t ithresh = 0; ithresh < nThresholds; ++ithresh){
     TProfile* prof = histos_nCastorGenvsCastorTowerPlus[ithresh]->ProfileX();
     TProfile* prof_sumE = histos_sumECastorGenvsCastorTowerPlus[ithresh]->ProfileX();
     TF1* g1 = new TF1("fitFunc","pol1");
     TF1* g2 = new TF1("fitFuncSumE","pol1");
     prof->Fit(g1);
     prof_sumE->Fit(g2);
     double interceptPar = g1->GetParameter(0);
     double slopePar = g1->GetParameter(1);
     int bin = ithresh + 1;
     h_fitInterceptMult->SetBinContent(bin,interceptPar);
     h_fitSlopeMult->SetBinContent(bin,slopePar);
     interceptPar = g2->GetParameter(0);
     slopePar = g2->GetParameter(1);
     h_fitInterceptSumE->SetBinContent(bin,interceptPar);
     h_fitSlopeSumE->SetBinContent(bin,slopePar);
     prof_nCastorGenvsCastorTowerPlus.push_back(prof);
     prof_sumECastorGenvsCastorTowerPlus.push_back(prof_sumE);
   }

   // Write output file
   hfile->cd();
   h_fitInterceptMult->Write();
   h_fitSlopeMult->Write();
   h_fitInterceptSumE->Write();
   h_fitSlopeSumE->Write();
   for(size_t ithresh = 0; ithresh < nThresholds; ++ithresh){
     prof_nCastorGenvsCastorTowerPlus[ithresh]->Write();	
     prof_sumECastorGenvsCastorTowerPlus[ithresh]->Write();
   }
   hfile->Write();
   hfile->Close();
}

