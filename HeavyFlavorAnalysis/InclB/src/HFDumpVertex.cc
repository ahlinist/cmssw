#include <iostream>

#include "HeavyFlavorAnalysis/InclB/interface/HFDumpVertex.h"

#include "DataFormats/Common/interface/Handle.h"

#include "FWCore/Framework/interface/ESHandle.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAna00Event.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaTrack.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaCand.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TGenCand.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaVertex.hh"

// -- Yikes!
extern TAna00Event *gHFEvent;

using namespace std;
using namespace edm;


// ----------------------------------------------------------------------
HFDumpVertex::HFDumpVertex(const edm::ParameterSet& iConfig):  
  fVerbose(iConfig.getUntrackedParameter<int>("verbose", 0)),
  fVertexLabel(iConfig.getUntrackedParameter<string>("vertexLabel", string("offlinePrimaryVertices")))
{
  cout << "----------------------------------------------------------------------" << endl;
  cout << "--- HFDumpVertex constructor" << endl;
  cout << "--- " << fVertexLabel.c_str() << endl;
  cout << "----------------------------------------------------------------------" << endl;
}


// ----------------------------------------------------------------------
HFDumpVertex::~HFDumpVertex() {
  
}


// ----------------------------------------------------------------------
void HFDumpVertex::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

  nevt++;

  // -- get the collection of RecoTracks 
  edm::Handle<reco::VertexCollection> vertices;
  iEvent.getByLabel( fVertexLabel.c_str(), vertices);

  // -- Adding vertex to ntuple ...
  TAnaVertex *pVtx = new TAnaVertex();
    
  int nvtx=0;
  for(reco::VertexCollection::const_iterator v=vertices->begin(); v!=vertices->end() && nvtx<1; ++v){
    pVtx->fPoint.SetXYZ(v->x(),
			v->y(),
			v->z());
    
    pVtx->setInfo(v->chi2(), int(v->ndof()), 1., 1, 1);
    pVtx->fDxy=0;
    pVtx->fDxyE=0;
    pVtx->fCxy=0;
    pVtx->fD3d=0;
    pVtx->fD3dE=0;
    pVtx->fC3d=0;
    gHFEvent->fPrimaryVertex = *pVtx;
    for(reco::Vertex::trackRef_iterator tr=v->tracks_begin(); tr!=v->tracks_end(); ++tr){
      //pVtx->addTrack((*tr).index());
    }
        
    nvtx++;
  }
  if (nvtx==0) {
    pVtx->fPoint.SetXYZ(-9999,
			-9999,
			-9999);
    pVtx->setInfo(-9999., -9999, -9999., -9999, -9999);
    pVtx->fDxy=-9999;
    pVtx->fDxyE=-9999;
    pVtx->fCxy=-9999;
    pVtx->fD3d=-9999;
    pVtx->fD3dE=-9999;
    pVtx->fC3d=-9999;
    gHFEvent->fPrimaryVertex = *pVtx;
    gHFEvent->fEventBits = gHFEvent->fEventBits + 1024;
  } 
  
  if (fVerbose > 0) {
    cout << "===> Vertex " << endl;
    pVtx->dump();
  }

}

// ------------ method called once each job just before starting event loop  ------------
void  HFDumpVertex::beginJob(const edm::EventSetup& setup) {
  nevt=0;
 
}

// ------------ method called once each job just after ending the event loop  ------------
void  HFDumpVertex::endJob() {
  cout << "HFDumpVertex> Summary: Events processed: " << nevt << endl;
}

//define this as a plug-in
//DEFINE_FWK_MODULE(HFDumpVertex);
