#include <iostream>

#include "HeavyFlavorAnalysis/InclB/interface/HFTree.h"

#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TH2D.h>

#include "HeavyFlavorAnalysis/InclB/rootio/TAna00Event.hh"
#include "HeavyFlavorAnalysis/InclB/rootio/TAnaTrack.hh"
#include "HeavyFlavorAnalysis/InclB/rootio/TAnaCand.hh"
#include "HeavyFlavorAnalysis/InclB/rootio/TGenCand.hh"
#include "HeavyFlavorAnalysis/InclB/rootio/TAnaVertex.hh"

// -- Yikes!
TAna00Event  *gHFEvent;

using namespace::std;

// ----------------------------------------------------------------------
HFTree::HFTree(const edm::ParameterSet& iConfig) {
  using namespace std;
  cout << "----------------------------------------------------------------------" << endl;
  cout << "--- HFTree constructor" << endl;
  cout << "----------------------------------------------------------------------" << endl;
  fFile = new TFile(iConfig.getParameter<string>("fileName").c_str(), "RECREATE");
  fTree = new TTree("T1","CMSSW HF tree");
  fEvent = new TAna00Event(0);
  fTree->Branch("TAna00Event", "TAna00Event", &fEvent, 256000/8, 1);
 
  gHFEvent = fEvent;
  nevt=0;

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
  
  nevt++;
  gHFEvent->fRunNumber   = iEvent.id().run();
  gHFEvent->fEventNumber = iEvent.id().event();
  gHFEvent->fLumiSection = iEvent.id().luminosityBlock();
  gHFEvent->fEventWeight = 1;
  fTree->Fill();
  gHFEvent->Clear();
}

// ------------ method called once each job just before starting event loop  ------------
void  HFTree::beginJob() {
}

// ------------ method called once each job just after ending the event loop  ------------
void  HFTree::endJob() { 
  cout << "HFTree>           Summary: Events processed: " << nevt << endl; 
  
}

//define this as a plug-in
DEFINE_FWK_MODULE(HFTree);
