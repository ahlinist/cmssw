#include <iostream>

#include "HeavyFlavorAnalysis/InclB/interface/HFDumpSimTracks.h"

#include "DataFormats/Common/interface/Handle.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "SimTracker/Records/interface/TrackAssociatorRecord.h"
#include "SimTracker/TrackAssociation/interface/TrackAssociatorBase.h" 
#include "SimTracker/TrackAssociation/interface/TrackAssociatorByChi2.h"
#include "SimDataFormats/Track/interface/SimTrack.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"

#include "DataFormats/TrackerRecHit2D/interface/SiTrackerGSRecHit2D.h" 
#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"
#include "DataFormats/TrackReco/interface/TrackExtraFwd.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"

#include "FWCore/Framework/interface/ESHandle.h"

#include "HeavyFlavorAnalysis/InclB/rootio/TAna00Event.hh"
#include "HeavyFlavorAnalysis/InclB/rootio/TAnaTrack.hh"
#include "HeavyFlavorAnalysis/InclB/rootio/TAnaCand.hh"
#include "HeavyFlavorAnalysis/InclB/rootio/TGenCand.hh"
#include "HeavyFlavorAnalysis/InclB/rootio/TAnaVertex.hh"

#include "DataFormats/Candidate/interface/CandMatchMap.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "DataFormats/Common/interface/RefToBase.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h" 

#include "CommonTools/TrackerMap/interface/TrackerMap.h"

#include "DataFormats/JetReco/interface/BasicJetCollection.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "DataFormats/GeometryVector/interface/GlobalVector.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/IPTools/interface/IPTools.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"


#include <TFile.h>
#include <TH1.h>


// -- Yikes!
extern TAna00Event *gHFEvent;

using namespace std;
using namespace edm;
using namespace reco;


// ----------------------------------------------------------------------
HFDumpSimTracks::HFDumpSimTracks(const edm::ParameterSet& iConfig):
  fTracksLabel(iConfig.getUntrackedParameter<string>("tracksLabel", string("g4SimHits"))), 
  fVerbose(iConfig.getUntrackedParameter<int>("verbose", 0)) {
  cout << "----------------------------------------------------------------------" << endl;
  cout << "--- HFDumpSimTracks constructor" << endl;
  cout << "--- " << fVerbose << endl;
  cout << "--- " << fTracksLabel.c_str() << endl;
  cout << "----------------------------------------------------------------------" << endl;
}


// ----------------------------------------------------------------------
HFDumpSimTracks::~HFDumpSimTracks() {
  
}


// ----------------------------------------------------------------------
void HFDumpSimTracks::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) { 

  
  nevt++;
  if (fVerbose>0) cout << "==>HFDumpSimTracks> new  event:  " << nevt << endl; 

  // -- get the collection of SimTracks 
  Handle<edm::SimTrackContainer> simtracks;
  iEvent.getByLabel(fTracksLabel.c_str(), simtracks);
  const edm::SimTrackContainer& trks = *(simtracks.product());
  SimTrackContainer::const_iterator trksiter;
  SimTrackContainer::const_iterator trksiter2;

  edm::Handle<edm::SimVertexContainer> SimVtx;
  iEvent.getByLabel(fTracksLabel.c_str(),SimVtx);
  const edm::SimVertexContainer& vtxs = *(SimVtx.product());
  


 
  if (fVerbose > 0) cout << "===> SimTracks " << trks.size() << endl;
  TAnaTrack *pTrack; 
 
  int i=0;
  for (trksiter = trks.begin(); trksiter != trks.end(); trksiter++) {

    pTrack = gHFEvent->addSimTrack();
    pTrack->fIndex = i;
    pTrack->fPlab.SetPtEtaPhi(trksiter->momentum().pt(),
			      trksiter->momentum().eta(),
			      trksiter->momentum().phi()
			      ); 

    pTrack->fQ = trksiter->charge();
    pTrack->fGenIndex = trksiter->genpartIndex()-1;//index in GEN block
    pTrack->fMCID     = trksiter->type();//pdgid
    pTrack->fMuID        = -9999 ;//index of parent in sim tracks
  
    int vertexIndex=trksiter->vertIndex();
    int parentIndex=-1;
    if (vertexIndex>-1) {
      parentIndex = (*SimVtx)[vertexIndex].parentIndex();
    }
    if (parentIndex>-1) {
      int j=0;
      for (trksiter2 = trks.begin(); trksiter2 != trks.end(); trksiter2++) {
	if (trksiter2->trackId()==parentIndex) {
	  pTrack->fMuID        = j ;
	  break;
	}
	j++;
      }

    }
      
    
  
    pTrack->fHighPurity  = -9999;
    pTrack->fChi2        = -9999;
    pTrack->fDof         = -9999;
    pTrack->fHits        = -9999;
    pTrack->fMuonCSCHits = -9999;
    pTrack->fMuonDTHits  = -9999; 
    pTrack->fMuonRPCHits = -9999;
    pTrack->fMuonHits    = -9999; 
 
    
    pTrack->fBPIXHits    = -9999;
    pTrack->fFPIXHits    = -9999; 
    pTrack->fPixelHits   = -9999; 
    
    pTrack->fStripHits   = -9999;
    pTrack->fTECHits     = -9999;
    pTrack->fTIBHits     = -9999;
    pTrack->fTIDHits     = -9999;
    pTrack->fTOBHits     = -9999;
    
    pTrack->fBPIXLayers  = -9999;
    pTrack->fFPIXLayers  = -9999; 
    pTrack->fPixelLayers = -9999; 
    pTrack->fStripLayers = -9999;
    pTrack->fTECLayers   = -9999;
    pTrack->fTIBLayers   = -9999;
    pTrack->fTIDLayers   = -9999;
    pTrack->fTOBLayers   = -9999;
       
    pTrack->fTip         = -9999 ;
    pTrack->fTipE        = -9999 ;
    pTrack->fTip3d       = -9999 ;
    pTrack->fTip3dE      = -9999 ;
    pTrack->fLip         = -9999 ;
    pTrack->fLipE        = -9999 ;
 
    i++;

  }
  
  
}

// ------------ method called once each job just before starting event loop  ------------
//void  HFDumpSimTracks::beginJob(const edm::EventSetup& setup) {
void  HFDumpSimTracks::beginJob() {

 
  nevt = 0;
}

// ------------ method called once each job just after ending the event loop  ------------
void  HFDumpSimTracks::endJob() {
  cout << "HFDumpSimTracks>     Summary: Events processed: " << nevt << endl;
}







//define this as a plug-in
//DEFINE_FWK_MODULE(HFDumpSimTracks);
