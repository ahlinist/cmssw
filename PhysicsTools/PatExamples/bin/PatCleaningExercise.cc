#include <vector>

#include "TH1.h"
#include "TFile.h"
#include <TROOT.h>
#include <TSystem.h>

#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "FWCore/ParameterSet/interface/ProcessDesc.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "FWCore/PythonParameterSet/interface/PythonProcessDesc.h"


using namespace std;
using namespace reco;
using namespace pat;


int main(int argc, char *argv[]){
  // load framework libraries
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();

  // get the python configuration
  PythonProcessDesc builder(argv[1]);
  const edm::ParameterSet& fwliteParameters = builder.processDesc()->getProcessPSet()->getParameter<edm::ParameterSet>("FWLiteParams");

  // now get each parameter
  std::string   input_   ( fwliteParameters.getParameter<std::string  >("input"   ) );
  std::string   overlaps_( fwliteParameters.getParameter<std::string  >("overlaps") );
  edm::InputTag jets_    ( fwliteParameters.getParameter<edm::InputTag>("jetSrc"  ) );

  // book a set of histograms
  fwlite::TFileService fs = fwlite::TFileService("analyzePatBasics.root");
  TFileDirectory theDir = fs.mkdir("analyzePatCleaning.root");
  TH1F* emfAllJets_    = theDir.make<TH1F>("emfAllJets"    , "f_{emf}(All Jets)"    ,  20,  0.,  1.);
  TH1F* emfCleanJets_  = theDir.make<TH1F>("emfCleanJets"  , "f_{emf}(Clean Jets)"  ,  20,  0.,  1.);
  TH1F* emfOverlapJets_= theDir.make<TH1F>("emfOverlapJets", "f_{emf}(Overlap Jets)",  20,  0.,  1.);
  TH1F* deltaRElecJet_ = theDir.make<TH1F>("deltaRElecJet" , "#DeltaR (elec, jet)"  ,  10,  0., 0.5);
  TH1F* elecOverJet_   = theDir.make<TH1F>("elecOverJet"   , "E_{elec}/E_{jet}"     , 100,  0.,  2.);
  TH1F* nOverlaps_     = theDir.make<TH1F>("nOverlaps"     , "Number of overlaps"   ,   5,  0.,  5.);
  
  // open input file (can be located on castor)
  TFile* inFile = TFile::Open(input_.c_str());

  // loop the events
  unsigned int iEvent=0;
  fwlite::Event ev(inFile); 
  for(ev.toBegin(); !ev.atEnd(); ++ev){
    edm::EventBase const & event = ev;

    // simple event counter
    if(iEvent>0 && iEvent%1==0){
      std::cout << "  processing event: " << iEvent << std::endl;
    }

    // handle to jet collection
    edm::Handle<std::vector<pat::Jet> > jets;
    event.getByLabel(jets_, jets);

    // loop over the jets in the event
    for( vector<pat::Jet>::const_iterator jet = jets->begin(); jet != jets->end(); jet++ ){
      if(jet->pt() > 20){
	emfAllJets_->Fill( jet->emEnergyFraction() );
	if(! jet->hasOverlaps(overlaps_)){
	  emfCleanJets_->Fill( jet->emEnergyFraction() );
	}
	else{
	  //get all overlaps
	  const reco::CandidatePtrVector overlaps = jet->overlaps(overlaps_);
	  nOverlaps_->Fill( overlaps.size() );
	  emfOverlapJets_->Fill( jet->emEnergyFraction() );
	  //loop over the overlaps
	  for( reco::CandidatePtrVector::const_iterator overlap = overlaps.begin(); overlap != overlaps.end(); overlap++){ 
	    float deltaR = reco::deltaR( (*overlap)->eta(), (*overlap)->phi(), jet->eta(), jet->phi() );
	    deltaRElecJet_->Fill( deltaR );
	    elecOverJet_->Fill( (*overlap)->energy(), jet->energy() );
	  }
	}
      }
    }
  }
  inFile->Close();
  return 0;
}
