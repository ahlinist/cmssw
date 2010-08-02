#include <iostream>

#include "HeavyFlavorAnalysis/InclB/interface/HFDumpVertex.h"

#include "DataFormats/Common/interface/Handle.h"

#include "FWCore/Framework/interface/ESHandle.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/Candidate/interface/CandMatchMap.h"
#include "DataFormats/Candidate/interface/Candidate.h"

#include "DataFormats/Common/interface/Ref.h"

#include "HeavyFlavorAnalysis/InclB/rootio/TAna00Event.hh"
#include "HeavyFlavorAnalysis/InclB/rootio/TAnaTrack.hh"
#include "HeavyFlavorAnalysis/InclB/rootio/TAnaCand.hh"
#include "HeavyFlavorAnalysis/InclB/rootio/TGenCand.hh"
#include "HeavyFlavorAnalysis/InclB/rootio/TAnaVertex.hh"

// -- Yikes!
extern TAna00Event *gHFEvent;

using namespace std;
using namespace edm;
using namespace reco;


// ----------------------------------------------------------------------
HFDumpVertex::HFDumpVertex(const edm::ParameterSet& iConfig):  
  fVerbose(iConfig.getUntrackedParameter<int>("verbose", 0)),
  fVertexLabel(iConfig.getUntrackedParameter<string>("vertexLabel", string("offlinePrimaryVertices"))),
  fVertexTracksLabel(iConfig.getUntrackedParameter<string>("vertexTracksLabel", string("generalTracks")))
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

  ///////////////////
  //tracks (jet constituents)
  Handle<reco::TrackCollection> candidates1Handle;
  iEvent.getByLabel("generalTracks", candidates1Handle); 

  std::vector<const Track *> trkcands;
  trkcands.clear();
  vector<const Track *>::const_iterator trkfound = trkcands.begin(); 

  //get tracks 
  for (unsigned int i = 0; i < candidates1Handle->size(); ++ i ) {
    
    const Track &p2 = (*candidates1Handle)[i];
    const Track *  p1 = &p2;
    trkcands.push_back( & * p1 );
  }

  /////////////////////////

  nevt++;

  /////////////////
  ////////////////
  ////////////////
  // -- Primary vertex
  int bestPV(-1), bestN(-1), cnt(0); 
  try {
    // -- get the collection of RecoTracks 
    edm::Handle<edm::View<reco::Track> > tracksView;
    iEvent.getByLabel(fVertexTracksLabel.c_str(), tracksView);

    edm::Handle<reco::VertexCollection> recoPrimaryVertexCollection;
    iEvent.getByLabel(fVertexLabel.c_str(), recoPrimaryVertexCollection);
    //    const reco::VertexCollection vertices = *(recoPrimaryVertexCollection.product());

    int isFake(-1); 
    double cov[9]; 
    for (reco::VertexCollection::const_iterator iv = recoPrimaryVertexCollection->begin(); iv != recoPrimaryVertexCollection->end(); ++iv) {
      TAnaVertex *pVtx = gHFEvent->addPV();
    
      if (iv->isFake()) {
	isFake = 1; 
      } else {
	isFake = 0; 
      }
      
      int ntracks = iv->tracksSize(); 
      if (0 == isFake) {
	if (ntracks > bestN) {
	  bestN  = ntracks; 
	  bestPV = cnt;
	}
      }

      if (fVerbose > 1) {
	cout << "PV found: isFake = " << isFake << " with " << ntracks << " tracks" << endl;
      }
     
      pVtx->setInfo(iv->chi2(), int(iv->ndof()), 1., isFake, ntracks);
      pVtx->fPoint.SetXYZ(iv->x(),
			  iv->y(),
			  iv->z());

      for (int i = 0; i < 3; ++i) {
	for (int j = 0; j < 3; ++j) {
	  cov[i*3+j] = iv->covariance(i,j);
	}
      }
      pVtx->setCovXX(cov);
      for(reco::Vertex::trackRef_iterator tr=iv->tracks_begin(); tr!=iv->tracks_end(); ++tr){
	
	const reco::Track & ttr = *tr->get();
	
	int index  = -1;
	trkfound = find( trkcands.begin(), trkcands.end(), &(ttr) );
	if ( trkfound != trkcands.end() ) index = trkfound - trkcands.begin() ;
	//if (fVerbose) cout << index << " pt " << ttr.pt() << " phi " << ttr.phi() << " eta " << ttr.eta() << endl;
	pVtx->addTrack(index);
	
      }
      
      if (fVerbose > 0) {
	cout << "===> Event " << iEvent.id() << endl;
	cout << "===> Vertex " << endl;
	pVtx->dump();
      }
    
      ++cnt; 
    }
  } catch (cms::Exception &ex) {
    //    cout << ex.explainSelf() << endl;
    if (fVerbose > 0) cout << "==>HFDumpStuff> primaryVertex " << fVertexLabel << " not found " << endl;
  } 
     
  if (fVerbose > 0) cout << "The best pV is at position: " << bestPV  << " and has " << bestN << " tracks" << endl;


  /////////////////
  //////////////////
  /////////////////












}

// ------------ method called once each job just before starting event loop  ------------
void  HFDumpVertex::beginJob() {
  nevt=0;
 
}

// ------------ method called once each job just after ending the event loop  ------------
void  HFDumpVertex::endJob() { 
 
  cout << "HFDumpVertex>     Summary: Events processed: " << nevt << endl;
}

//define this as a plug-in
DEFINE_FWK_MODULE(HFDumpVertex);
