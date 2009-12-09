#include <iostream>

#include "HeavyFlavorAnalysis/Examples/interface/HFDumpStuff.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"

#include "SimDataFormats/TrackingAnalysis/interface/TrackingVertexContainer.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingVertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "CommonTools/Statistics/interface/ChiSquared.h"

#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAna01Event.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaTrack.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaCand.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TGenCand.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaVertex.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaMuon.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TTrgObj.hh"

#include <TVector3.h>

// -- Yikes!
extern TAna01Event *gHFEvent;

using namespace std;
using namespace edm;


// ----------------------------------------------------------------------
HFDumpStuff::HFDumpStuff(const edm::ParameterSet& iConfig):
  fVerbose(iConfig.getUntrackedParameter<int>("verbose", 1)),
  fGenEventScaleLabel(iConfig.getUntrackedParameter<string>("GenEventScaleLabel", string("genEventScale"))),
  fCandidates1Label(iConfig.getUntrackedParameter<string>("Candidates1Label", string("JPsiToMuMu"))),
  fCandidates2Label(iConfig.getUntrackedParameter<string>("Candidates2Label", string("JPsiToMuMu"))),
  fCandidates3Label(iConfig.getUntrackedParameter<string>("Candidates3Label", string("JPsiToMuMu"))),
  fMETLabel(iConfig.getUntrackedParameter<string>("METLabel", string("corMetType1Icone5"))),
  fGenMETLabel(iConfig.getUntrackedParameter<string>("GenMETLabel", string("genMet"))),
  fPrimaryVertexLabel(iConfig.getUntrackedParameter<InputTag>("PrimaryVertexLabel", InputTag("offlinePrimaryVertices"))),
  fPrimaryVertexTracksLabel(iConfig.getUntrackedParameter<InputTag>("PrimaryVertexTracksLabel", InputTag("generalTracks")))
{
  cout << "----------------------------------------------------------------------" << endl;
  cout << "--- HFDumpStuff constructor" << endl;
  cout << "---  verbose:                    " << fVerbose << endl;
  cout << "---  PrimaryVertexLabel:         " << fPrimaryVertexLabel << endl;
  cout << "---  PrimaryVertexTracksLabel:   " << fPrimaryVertexTracksLabel << endl;
  cout << "---  GenEventScaleLabel:         " << fGenEventScaleLabel.c_str() << endl;
  cout << "---  METLabel:                   " << fMETLabel.c_str() << endl;
  cout << "---  GenMETLabel:                " << fGenMETLabel.c_str() << endl;
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
    if (fVerbose > 0) cout << "genEventScale " << fGenEventScaleLabel.c_str() << " not found " << endl;
  }


  // -- Primary vertex
  try {
    // -- get the collection of RecoTracks 
    edm::Handle<edm::View<reco::Track> > tracksView;
    iEvent.getByLabel(fPrimaryVertexTracksLabel, tracksView);

    edm::Handle<reco::VertexCollection> recoPrimaryVertexCollection;
    iEvent.getByLabel(fPrimaryVertexLabel, recoPrimaryVertexCollection);
    //    const reco::VertexCollection vertices = *(recoPrimaryVertexCollection.product());

    int isFake(-1); 
    double cov[9]; 
    for (reco::VertexCollection::const_iterator iv = recoPrimaryVertexCollection->begin(); iv != recoPrimaryVertexCollection->end(); ++iv) {
      TAnaVertex *pVtx = gHFEvent->addPV();
      ChiSquared chi2(iv->chi2(), iv->ndof());
      if (iv->isFake()) {
	isFake = 1; 
      } else {
	isFake = 0; 
      }

      if (fVerbose > 0) {
	cout << "PV found: isFake = " << isFake << endl;
      }
      pVtx->setInfo(chi2.value(), iv->ndof(), chi2.probability(), isFake, 0);
      pVtx->fPoint.SetXYZ(iv->x(),
			  iv->y(),
			  iv->z());

      for (int i = 0; i < 3; ++i) {
	for (int j = 0; j < 3; ++j) {
	  cov[i*3+j] = iv->covariance(i,j);
	}
      }
      pVtx->setCovXX(cov);

      //       Vertex::trackRef_iterator v1TrackIter;
      //       Vertex::trackRef_iterator v1TrackBegin = iv->tracks_begin();
      //       Vertex::trackRef_iterator v1TrackEnd   = iv->tracks_end();
      //       for (v1TrackIter = v1TrackBegin; v1TrackIter != v1TrackEnd; v1TrackIter++) {
      
      // 	for (unsigned int i = 0; i < tracksView->size(); ++i){    
      // 	  TrackBaseRef rTrackView(tracksView,i);
      // 	  if (rTrackView == v1TrackIter) {
      // 	    cout << "Found a track from PV" << endl;
      // 	    // 	    int it = v1TrackIter.index();
      // 	    // 	    pVtx->addTrack(it); 
      // 	  }
      // 	}
      //       }
    }
  } catch (cms::Exception &ex) {
    //    cout << ex.explainSelf() << endl;
    cout << "==>HFDumpStuff> primaryVertex " << fPrimaryVertexLabel << " not found " << endl;
  } 
  
  // -- Candidates list
  try {
    //    Handle<CandidateCollection> candidates1Handle;
    Handle<reco::CandidateView> candidates1Handle;
    iEvent.getByLabel(fCandidates1Label.c_str(), candidates1Handle);
    for (unsigned int i = 0; i < candidates1Handle->size(); ++ i ) {
      const Candidate &p = (*candidates1Handle)[i];
      cout << "==>HFDumpStuff>  candidates1 " << i << " id = " << p.pdgId() 
	   << " mass = " << p.mass()
	   << " pt = " << p.pt() 
	   << " phi = " << p.phi() 
	   << " eta = " << p.eta() 
	   << endl;
    }
  } catch (cms::Exception &ex) {
    //    cout << ex.explainSelf() << endl;
    cout << "==>HFDumpStuff> Candidate list " << fCandidates1Label.c_str() << " not found " << endl;
  }

  try {
    //    Handle<CandidateCollection> candidates2Handle;
    Handle<reco::CandidateView> candidates2Handle;
    iEvent.getByLabel(fCandidates2Label.c_str(), candidates2Handle);
    for (unsigned int i = 0; i < candidates2Handle->size(); ++ i ) {
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
    cout << "==>HFDumpStuff> Candidate list " << fCandidates2Label.c_str() << " not found " << endl;
  }

  try {
    //    Handle<CandidateCollection> candidates3Handle;
    Handle<reco::CandidateView> candidates3Handle;
    iEvent.getByLabel(fCandidates3Label.c_str(), candidates3Handle);
    for (unsigned int i = 0; i < candidates3Handle->size(); ++ i ) {
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
    cout << "==>HFDumpStuff> Candidate list " << fCandidates3Label.c_str() << " not found " << endl;
  }



  // -- MET
  try {
    Handle<CaloMETCollection> metHandle;
    iEvent.getByLabel(fMETLabel.c_str(), metHandle);
    const reco::CaloMET *met = &metHandle.product()->at(0);
    gHFEvent->fMET0 = TVector3(met->p4().x(), met->p4().y(), 0.);  
  } catch (cms::Exception &ex) {
    //    cout << ex.explainSelf() << endl;
    cout << "==>HFDumpStuff> MET " << fMETLabel.c_str() << " not found " << endl;
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
    cout << "==>HFDumpStuff> GenMET " << fGenMETLabel.c_str() << " not found " << endl;
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
