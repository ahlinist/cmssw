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
  fCandidates1Label(iConfig.getUntrackedParameter<string>("Candidates1Label", string("JPsiToMuMu"))),
  fCandidates2Label(iConfig.getUntrackedParameter<string>("Candidates2Label", string("JPsiToMuMu"))),
  fCandidates3Label(iConfig.getUntrackedParameter<string>("Candidates3Label", string("JPsiToMuMu"))),
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


  // -- Candidates list
  try {
    Handle<CandidateCollection> candidates1Handle;
    iEvent.getByLabel(fCandidates1Label.c_str(), candidates1Handle);
    for (int i = 0; i < candidates1Handle->size(); ++ i ) {
      const Candidate &p = (*candidates1Handle)[i];
      cout << "==> candidates1 " << i << " id = " << p.pdgId() 
	   << " mass = " << p.mass()
	   << " pt = " << p.pt() 
	   << " phi = " << p.phi() 
	   << " eta = " << p.eta() 
	   << endl;
    }
  } catch (cms::Exception &ex) {
    //    cout << ex.explainSelf() << endl;
    cout << "Candidate list " << fCandidates1Label.c_str() << " not found " << endl;
  }

  try {
    Handle<CandidateCollection> candidates2Handle;
    iEvent.getByLabel(fCandidates2Label.c_str(), candidates2Handle);
    for (int i = 0; i < candidates2Handle->size(); ++ i ) {
      const Candidate &p = (*candidates2Handle)[i];
      cout << "==> candidates2 " << i << " id = " << p.pdgId() 
	   << " mass = " << p.mass()
	   << " pt = " << p.pt() 
	   << " phi = " << p.phi() 
	   << " eta = " << p.eta() 
	   << endl;
    }
  } catch (cms::Exception &ex) {
    //    cout << ex.explainSelf() << endl;
    cout << "Candidate list " << fCandidates2Label.c_str() << " not found " << endl;
  }

  try {
    Handle<CandidateCollection> candidates3Handle;
    iEvent.getByLabel(fCandidates3Label.c_str(), candidates3Handle);
    for (int i = 0; i < candidates3Handle->size(); ++ i ) {
      const Candidate &p = (*candidates3Handle)[i];
      cout << "==> candidates3 " << i << " id = " << p.pdgId() 
	   << " mass = " << p.mass()
	   << " pt = " << p.pt() 
	   << " phi = " << p.phi() 
	   << " eta = " << p.eta() 
	   << endl;
    }
  } catch (cms::Exception &ex) {
    //    cout << ex.explainSelf() << endl;
    cout << "Candidate list " << fCandidates3Label.c_str() << " not found " << endl;
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
