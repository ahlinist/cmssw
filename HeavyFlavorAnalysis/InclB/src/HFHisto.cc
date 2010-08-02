#include <iostream>

#include "HeavyFlavorAnalysis/InclB/interface/HFHisto.h"

#include <TROOT.h>
#include <TFile.h>
#include <TH1.h>

#include "HeavyFlavorAnalysis/InclB/rootio/TAna00Event.hh"
#include "HeavyFlavorAnalysis/InclB/rootio/TAnaTrack.hh"
#include "HeavyFlavorAnalysis/InclB/rootio/TAnaCand.hh"
#include "HeavyFlavorAnalysis/InclB/rootio/TGenCand.hh"
#include "HeavyFlavorAnalysis/InclB/rootio/TAnaVertex.hh"

extern TAna00Event *gHFEvent;

using namespace::std;

// ----------------------------------------------------------------------
HFHisto::HFHisto(const edm::ParameterSet& iConfig) {
  using namespace std;
  cout << "----------------------------------------------------------------------" << endl;
  cout << "--- HFHisto constructor" << endl;
  cout << "----------------------------------------------------------------------" << endl;
  fFile       = new TFile(iConfig.getParameter<string>("fileName").c_str(), "RECREATE");
  fHisto      = new TH1I("nevt", "number of events ", 1000, 0., 1000.);
 
}


// ----------------------------------------------------------------------
HFHisto::~HFHisto() {
  
  // -- Save output
  fFile->cd();
  fHisto->Write();
  fFile->Write();
  fFile->Close();
  delete fFile;
}


// ----------------------------------------------------------------------
void HFHisto::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

  nevt++;
  fHisto->SetBinContent(nevt,1);

  
}

// ------------ method called once each job just before starting event loop  ------------
void  HFHisto::beginJob() {
  nevt=0;
}

// ------------ method called once each job just after ending the event loop  ------------
void  HFHisto::endJob() { 
  cout << "HFHisto>          Summary: Events processed: " << nevt << endl;
 
}

//define this as a plug-in
DEFINE_FWK_MODULE(HFHisto);
