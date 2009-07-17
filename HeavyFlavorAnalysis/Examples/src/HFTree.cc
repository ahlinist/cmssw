#include <iostream>

#include "HeavyFlavorAnalysis/Examples/interface/HFTree.h"


#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1.h>

#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAna01Event.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaTrack.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaCand.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TGenCand.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaVertex.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaMuon.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TTrgObj.hh"

// -- Yikes!
TAna01Event  *gHFEvent;
TFile        *gHFFile;

using namespace::std;

// ----------------------------------------------------------------------
HFTree::HFTree(const edm::ParameterSet& iConfig) {
  using namespace std;
  cout << "----------------------------------------------------------------------" << endl;
  cout << "--- HFTree constructor" << endl;
  cout << "----------------------------------------------------------------------" << endl;
  fFile = new TFile(iConfig.getParameter<string>("fileName").c_str(), "RECREATE");
  fTree = new TTree("T1","CMSSW HF tree");
  fEvent = new TAna01Event(0);
  fTree->Branch("TAna01Event", "TAna01Event", &fEvent, 256000/8, 1);

  TH1D *h1 = new TH1D("h1", "h1", 20, 0., 20.);

  gHFEvent = fEvent;
  gHFFile  = fFile;

}


// ----------------------------------------------------------------------
HFTree::~HFTree() {
  
  // -- Save output
  fFile->cd();
  fTree->Write();
  fFile->Write();
  fFile->Close();
  delete fFile;
}


// ----------------------------------------------------------------------
void HFTree::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  gHFEvent->fRunNumber   = iEvent.id().run();
  gHFEvent->fEventNumber = iEvent.id().event();
    
  cout << "HFTree> filling tree for run: " << gHFEvent->fRunNumber
       << " event: "  << gHFEvent->fEventNumber 
       << " pthat: "  << gHFEvent->fPtHat 
       << " MET0: "   << gHFEvent->fMET0.Mag()
       << " genMET: " << gHFEvent->fGenMET.Mag()
       << endl;
  fTree->Fill();
  gHFEvent->Clear();
}

// ------------ method called once each job just before starting event loop  ------------
void  HFTree::beginJob(const edm::EventSetup&) {
}

// ------------ method called once each job just after ending the event loop  ------------
void  HFTree::endJob() {
}

//define this as a plug-in
//DEFINE_FWK_MODULE(HFTree);
