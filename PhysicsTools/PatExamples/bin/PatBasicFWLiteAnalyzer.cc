#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>

#include <TH1F.h>
#include <TROOT.h>
#include <TFile.h>
#include <TSystem.h>

#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"


int main(int argc, char* argv[]) 
{
  // load framework libraries
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();
  
  // book a set of histograms
  TH1F* muonPt_  = new TH1F("muonPt", "pt",    100,  0.,300.);
  TH1F* muonEta_ = new TH1F("muonEta","eta",   100, -3.,  3.);
  TH1F* muonPhi_ = new TH1F("muonPhi","phi",   100, -5.,  5.);  
  
  // open input file (can be located on castor)
  TFile* inFile = TFile::Open( "dcap://dcache-ses-cms.desy.de/pnfs/desy.de/cms/tier2/store/user/rwolf/ttbar/patTuple_PATv2_ttbar_madgraph_1.root" );
  // loop the events
  unsigned int iEvent=0;
  fwlite::Event event(inFile);
  for(event.toBegin(); !event.atEnd(); ++event, ++iEvent){
    // break loop after end of file is reached 
    // or after 1000 events have been processed
    if( iEvent==1000 ) break;
    
    // simple event counter
    if(iEvent>0 && iEvent%100==0){
      std::cout << "  processing event: " << iEvent << std::endl;
    }

    // fwlite::Handle to to muon collection
    fwlite::Handle<std::vector<pat::Muon> > muons;
    muons.getByLabel(event, "selectedLayer1Muons");
    
    // loop muon collection and fill histograms
    for(unsigned i=0; i<muons->size(); ++i){
      muonPt_ ->Fill( (*muons)[i].pt()  );
      muonEta_->Fill( (*muons)[i].eta() );
      muonPhi_->Fill( (*muons)[i].phi() );
    }
  }  
  // close input file
  inFile->Close();
  
  //open output file
  TFile outFile( "analyzePatBasics.root", "recreate" );
  outFile.mkdir(".analyzeBasicPat");
  outFile.cd(".analyzeBasicPat");
  muonPt_ ->Write( );
  muonEta_->Write( );
  muonPhi_->Write( );
  outFile.Close();
  
  // free allocated space
  delete muonPt_;
  delete muonEta_;
  delete muonPhi_;
  
  // that's it!
  return 0;
}
