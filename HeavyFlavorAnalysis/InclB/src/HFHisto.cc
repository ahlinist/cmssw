#include <iostream>

#include "HeavyFlavorAnalysis/InclB/interface/HFHisto.h"

#include <TROOT.h>
#include <TFile.h>
#include <TH1.h>

#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAna00Event.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaTrack.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaCand.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TGenCand.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaVertex.hh"

using namespace::std;

// ----------------------------------------------------------------------
HFHisto::HFHisto(const edm::ParameterSet& iConfig) {
  using namespace std;
  cout << "----------------------------------------------------------------------" << endl;
  cout << "--- HFHisto constructor" << endl;
  cout << "----------------------------------------------------------------------" << endl;
  fFile = new TFile(iConfig.getParameter<string>("fileName").c_str(), "RECREATE");
  fHisto = new TH1I("nevt", "number of events ", 1000, 0., 1000.);

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
void  HFHisto::beginJob(const edm::EventSetup&) {
  nevt=0;
}

// ------------ method called once each job just after ending the event loop  ------------
void  HFHisto::endJob() { 
  cout << "HFHisto> Summary: Events processed: " << nevt << endl;

}

//define this as a plug-in
//DEFINE_FWK_MODULE(HFHisto);
