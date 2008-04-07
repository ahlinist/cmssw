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
  
  fNtot = 0; 
}


// ----------------------------------------------------------------------
HFGenFilter::~HFGenFilter() {  
  cout << "==> HFGenFilter> Total number of accepted events = " << fNtot << endl;
}


// ----------------------------------------------------------------------
bool HFGenFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  bool goodEvent(false);

  TGenCand *pGen, *pDau;
  int muDau(0);

  // -- filter events with at least one muon and pT>2.5GeV
  for (int ig = 0; ig < gHFEvent->nGenCands(); ++ig) {
    pGen = gHFEvent->getGenCand(ig);
    muDau = 0; 
    if (pGen->fDau1 > 0 && pGen->fDau2 > 0) {
      for (int id = pGen->fDau1;  id <= pGen->fDau2; ++id) {
        if ((id > -1) && (id < gHFEvent->nGenCands())) {
          pDau = gHFEvent->getGenCand(id); 
          if (13 == TMath::Abs(pDau->fID)) {
            ++muDau;
          }
        }
      }
    }

    if ((13 == TMath::Abs(pGen->fID)) || (muDau > 0)) {
      pGen->dump();
      if ((13 == TMath::Abs(pGen->fID)) && (pGen->fP.Perp() < 2.5)) {
        cout << "++++++++++++" << endl;
      }
      if ((13 == TMath::Abs(pGen->fID)) && (pGen->fP.Perp() > 2.5)) {
	goodEvent = true;
      }
    }

  }

  if (goodEvent) ++fNtot;
  return goodEvent;
}

