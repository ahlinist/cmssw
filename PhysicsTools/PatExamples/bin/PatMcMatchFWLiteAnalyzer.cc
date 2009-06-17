#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>

#include <TH2F.h>
#include <TROOT.h>
#include <TFile.h>
#include <TSystem.h>

#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"


int main(int argc, char* argv[]) 
{
  // ----------------------------------------------------------------------
  // First Part: 
  //
  //  * enable the AutoLibraryLoader 
  //  * book the histograms of interest 
  //  * open the input file
  // ----------------------------------------------------------------------

  // load framework libraries
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();
  
  // book a set of histograms
  TH2F* muonPt_  = new TH2F( "muonPt", "Muon pt", 60, 0., 300., 60, 0., 300. );
  muonPt_->SetXTitle( "gen." );
  muonPt_->SetYTitle( "reco." );
  TH2F* jetPt_   = new TH2F( "jetPt", "Jet Pt", 100, 0., 500., 100, 0., 500. );
  jetPt_->SetXTitle( "gen." );
  jetPt_->SetYTitle( "reco." );
  
  // open input file (can be located on castor)
  TFile* inFile = TFile::Open( "file:edmPatMcMatch.root" );

  // ----------------------------------------------------------------------
  // Second Part: 
  //
  //  * loop the events in the input file 
  //  * receive the collections of interest via fwlite::Handle
  //  * fill the histograms
  //  * after the loop close the input file
  // ----------------------------------------------------------------------

  // loop the events
  unsigned int iEvent=0;
  fwlite::Event event(inFile);
  for(event.toBegin(); !event.atEnd(); ++event, ++iEvent){
    // break loop after end of file is reached 
    // or after 1000 events have been processed
    if( iEvent==1000 ) break;
    
    // simple event counter
    if(iEvent>0 && iEvent%25==0){
      std::cout << "  processing event: " << iEvent << std::endl;
    }

    // fwlite::Handle to to muon collection
    fwlite::Handle<std::vector<pat::Muon> > muons;
    muons.getByLabel(event, "cleanLayer1Muons");
    // fwlite::Handle to to muon collection
    fwlite::Handle<std::vector<pat::Jet> > jets;
    jets.getByLabel(event, "cleanLayer1Jets");
    
    // loop muon collection and fill histograms
    for(unsigned i=0; i<muons->size(); ++i){
      const reco::GenParticle * genMuon = (*muons)[i].genParticle();
      if ( genMuon ) {
        muonPt_->Fill( genMuon->pt(), (*muons)[i].pt() );
      }
    }
    // loop jet collection and fill histograms
    for(unsigned i=0; i<jets->size(); ++i){
      const reco::GenJet * genJet = (*jets)[i].genJet();
      if ( genJet ) {
        jetPt_->Fill( genJet->pt(), (*jets)[i].pt() );
      }
    }
  }  
  // close input file
  inFile->Close();

  // ----------------------------------------------------------------------
  // Third Part: 
  //
  //  * open the output file 
  //  * write the histograms to the output file
  //  * close the output file
  // ----------------------------------------------------------------------
  
  //open output file
  TFile outFile( "rootPatMcMatch.root", "recreate" );
  outFile.mkdir("analyzeMcMatchPat");
  outFile.cd("analyzeMcMatchPat");
  muonPt_->Write( );
  jetPt_->Write( );
  outFile.Close();
  
  // ----------------------------------------------------------------------
  // Fourth Part: 
  //
  //  * never forgett to free the memory of the histograms
  // ----------------------------------------------------------------------

  // free allocated space
  delete muonPt_;
  delete jetPt_;
  
  // that's it!
  return 0;
}
