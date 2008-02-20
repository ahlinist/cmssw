#include <iostream>

#include "HeavyFlavorAnalysis/Examples/interface/HFDumpStuff.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"

#include "FWCore/Framework/interface/ESHandle.h"

#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAna00Event.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaTrack.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaCand.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TGenCand.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaVertex.hh"

#include <TVector3.h>

// -- Yikes!
extern TAna00Event *gHFEvent;

using namespace std;
using namespace edm;


// ----------------------------------------------------------------------
HFDumpStuff::HFDumpStuff(const edm::ParameterSet& iConfig):
  fGenEventScaleLabel(iConfig.getUntrackedParameter<string>("GenEventScaleLabel", string("genEventScale"))),
  fMETLabel(iConfig.getUntrackedParameter<string>("METLabel", string("corMetType1Icone5"))),
  fGenMETLabel(iConfig.getUntrackedParameter<string>("GenMETLabel", string("genMet")))
{
  cout << "----------------------------------------------------------------------" << endl;
  cout << "--- HFDumpStuff constructor" << endl;
  cout << "--- " << fGenEventScaleLabel.c_str() << endl;
  cout << "--- " << fMETLabel.c_str() << endl;
  cout << "--- " << fGenMETLabel.c_str() << endl;
  cout << "----------------------------------------------------------------------" << endl;
}


// ----------------------------------------------------------------------
HFDumpStuff::~HFDumpStuff() {
  
}


// ----------------------------------------------------------------------
void HFDumpStuff::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

  // -- pthat 
  gHFEvent->fPtHat = -1.; 
  try {
    edm::Handle<double> genEventScaleHandle;
    iEvent.getByLabel(fGenEventScaleLabel.c_str(), genEventScaleHandle);
    gHFEvent->fPtHat = *genEventScaleHandle;
  } catch (cms::Exception &ex) {
    //    cout << ex.explainSelf() << endl;
    cout << "genEventScale " << fGenEventScaleLabel.c_str() << " not found " << endl;
  }


  // -- MET
  try {
    Handle<CaloMETCollection> metHandle;
    iEvent.getByLabel(fMETLabel.c_str(), metHandle);
    const reco::CaloMET *met = &metHandle.product()->at(0);
    gHFEvent->fMET0 = TVector3(met->p4().x(), met->p4().y(), 0.);  
  } catch (cms::Exception &ex) {
    //    cout << ex.explainSelf() << endl;
    cout << "MET " << fMETLabel.c_str() << " not found " << endl;
  }


  // -- MET from generator level
  try{
    Handle<GenMETCollection> gen;
    iEvent.getByLabel(fGenMETLabel.c_str(), gen);
    const GenMETCollection *genmetcol = gen.product();
    const GenMET genmet = genmetcol->front();
    gHFEvent->fGenMET = TVector3(genmet.px(), genmet.py(), 0.);  
  } catch (cms::Exception &ex) {
    //    cout << ex.explainSelf() << endl;
    cout << "GenMET " << fGenMETLabel.c_str() << " not found " << endl;
  }

  cout << "==HFDumpStuff> GenMET =  (" 
       << gHFEvent->fGenMET.X() << "," 
       << gHFEvent->fGenMET.Y() << "), mag = " 
       << gHFEvent->fGenMET.Mag() 
       << endl;

  cout << "==HFDumpStuff> MET =  (" 
       << gHFEvent->fMET0.X() << "," 
       << gHFEvent->fMET0.Y() << "), mag = " 
       << gHFEvent->fMET0.Mag() 
       << endl;
}

// ------------ method called once each job just before starting event loop  ------------
void  HFDumpStuff::beginJob(const edm::EventSetup& setup) {
}

// ------------ method called once each job just after ending the event loop  ------------
void  HFDumpStuff::endJob() {
}

//define this as a plug-in
//DEFINE_FWK_MODULE(HFDumpStuff);
