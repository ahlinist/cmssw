#include "HeavyFlavorAnalysis/Examples/interface/HFGenFilter.h"

#include "DataFormats/Common/interface/Handle.h"

#include "FWCore/Framework/interface/ESHandle.h"

#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAna00Event.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaTrack.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaCand.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TGenCand.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaVertex.hh"


// -- Yikes!
extern TAna00Event *gHFEvent;
extern TFile       *gHFFile;

using namespace edm;
using namespace std;

// ----------------------------------------------------------------------
HFGenFilter::HFGenFilter(const edm::ParameterSet& iConfig) {
  cout << "----------------------------------------------------------------------" << endl;
  cout << "--- HFGenFilter constructor" << endl;
  cout << "----------------------------------------------------------------------" << endl;
  
  fNgood= 0;
  fNtot = 0; 
}


// ----------------------------------------------------------------------
HFGenFilter::~HFGenFilter() {  
  cout << "==> HFGenFilter> Total number of accepted/total events = " << fNgood << "/" << fNtot << endl;
}


// ----------------------------------------------------------------------
bool HFGenFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  bool goodEvent(false);

  TGenCand *pGen, *pDau;

  ++fNtot;
  cout << "==> HFGenFilter> new  event  " << fNtot;

  // -- filter events with at least one muon and pT>2.5GeV
  for (int ig = 0; ig < gHFEvent->nGenCands(); ++ig) {
    pGen = gHFEvent->getGenCand(ig);
    if ((13 == TMath::Abs(pGen->fID)) && (pGen->fP.Perp() > 5.)) {
      goodEvent = true;
    }
  }

  if (goodEvent) {
    ++fNgood;
    cout << " accepted" << endl;
  } else {
    cout << " not accepted" << endl;
  }
  return goodEvent;
}

