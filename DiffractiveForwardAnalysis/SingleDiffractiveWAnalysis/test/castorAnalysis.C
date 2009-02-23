#include "TH1F.h"
#include "TH2F.h"
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
   fileNames.push_back("/tmp/antoniov/POMWIG_SDPlusWmunu_EdmDump_InitialLumPU_1.root");
   
   // Chain the input files
   fwlite::ChainEvent ev(fileNames);
   
   // Create output file
   TFile* hfile = new TFile("analysisCastor_histos.root","recreate","data histograms");

   // Book Histograms
   int nBins = 20;
   TH1F* h_nCastorGenPlus = new TH1F("nCastorGenPlus","Castor gen mult. plus",nBins,0,nBins);
   TH1F* h_nCastorTowerPlus = new TH1F("nCastorTowerPlus","Castor Towers mult. plus",nBins,0,nBins);
   TH2F* h_nCastorGenvsCastorTowerPlus = new TH2F("nCastorGenVsCastorTowerPlus","Castor Gen vs Castor towers mult. plus",nBins,0,nBins,nBins,0,nBins);

   // Loop over the events
   int thresholdIndexCastor = 22;
 
   int nEvts = 0;
   for( ev.toBegin(); ! ev.atEnd(); ++ev) {

     if(nEvts == maxEvents) break;	
     ++nEvts;
     if(verbose) std::cout << ">>> Event number: " << nEvts << endl;	
     // Access collections
     fwlite::Handle<std::vector<unsigned int> > acceptedPhiSliceplus; 
     acceptedPhiSliceplus.getByLabel(ev,"castorGen","acceptedPhiSliceplus");

     unsigned int nAccPhiSliceplus = 0;
     for(unsigned int i = 0; i < acceptedPhiSliceplus->size(); i++){
        if((*acceptedPhiSliceplus)[i] == 1) nAccPhiSliceplus++;
     }

     if(verbose) std::cout << ">>> Number of castor slices (gen) = " << nAccPhiSliceplus << std::endl;

     fwlite::Handle<std::vector<unsigned int> > nCastorTowerPlus;
     nCastorTowerPlus.getByLabel(ev,"castorTower","nCastorTowerPlus");

     unsigned int nCastorGen_plus = nAccPhiSliceplus;
     unsigned int nCastorTwr_plus = (*nCastorTowerPlus)[thresholdIndexCastor];

     h_nCastorGenPlus->Fill(nCastorGen_plus);
     h_nCastorTowerPlus->Fill(nCastorTwr_plus);
     h_nCastorGenvsCastorTowerPlus->Fill(nCastorGen_plus,nCastorTwr_plus);
   }  // End loop over events
  
  // Write output file
  hfile->cd();	
  hfile->Write();
  hfile->Close();

}

