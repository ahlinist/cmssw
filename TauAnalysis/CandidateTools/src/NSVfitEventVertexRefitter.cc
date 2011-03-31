#include "TauAnalysis/CandidateTools/interface/NSVfitEventVertexRefitter.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "DataFormats/Math/interface/deltaR.h"

#include <TMath.h>

NSVfitEventVertexRefitter::NSVfitEventVertexRefitter(const edm::ParameterSet& cfg)
  : srcBeamSpot_(cfg.getParameter<edm::InputTag>("srcBeamSpot")),
    beamSpot_(0),
    trackBuilder_(0),
    vertexFitAlgorithm_(0)
{
  vertexFitAlgorithm_ = new KalmanVertexFitter(true);

  minNumTracksRefit_ = ( cfg.exists("minNumTracksRefit") ) ? cfg.getParameter<unsigned>("minNumTracksRefit") : 2;
}

NSVfitEventVertexRefitter::~NSVfitEventVertexRefitter()
{
  delete vertexFitAlgorithm_;
}

void NSVfitEventVertexRefitter::beginEvent(const edm::Event& evt, const edm::EventSetup& es)
{
//--- get beamspot
  edm::Handle<reco::BeamSpot> beamSpotHandle;
  evt.getByLabel(srcBeamSpot_, beamSpotHandle);
  beamSpot_ = beamSpotHandle.product();
  if ( !beamSpot_ ) {
    edm::LogError ("NSVfitEventVertexRefitter::beginEvent")
      << " Failed to access BeamSpot !!";
  }

//--- get pointer to TransientTrackBuilder
  edm::ESHandle<TransientTrackBuilder> trackBuilderHandle;
  es.get<TransientTrackRecord>().get("TransientTrackBuilder", trackBuilderHandle);
  trackBuilder_ = trackBuilderHandle.product();
  if ( !trackBuilder_ ) {
    edm::LogError ("NSVfitEventVertexRefitter::beginEvent")
      << " Failed to access TransientTrackBuilder !!";
  }
}

//-------------------------------------------------------------------------------
// auxiliary functions to compare two Tracks 
bool tracksMatchByDeltaR(const reco::Track* trk1, const reco::Track* trk2)
{
  if ( reco::deltaR(*trk1, *trk2) < 0.01 && trk1->charge() == trk2->charge() &&
       TMath::Abs(trk1->pt() - trk2->pt()) < 0.05*(trk1->pt() + trk2->pt()) )
    return true;
  else
    return false;
}

// auxiliary function to exclude tracks associated to tau lepton decay "leg"
// from primary event vertex refit
typedef std::map<const reco::Track*, reco::TransientTrack> TransientTrackMap;
void removeTracks(TransientTrackMap& pvTracks_toRefit, const std::vector<const reco::Track*>& svTracks)
{
  for ( std::vector<const reco::Track*>::const_iterator svTrack = svTracks.begin();
	svTrack != svTracks.end(); ++svTrack ) {

//--- remove track from list of tracks included in primary event vertex refit
//    if track matches by reference or in eta-phi
//    any of the tracks associated to tau lepton decay "leg"
    for ( TransientTrackMap::iterator pvTrack = pvTracks_toRefit.begin();
	  pvTrack != pvTracks_toRefit.end(); ++pvTrack ) {
      if ( tracksMatchByDeltaR(pvTrack->first, *svTrack) ) {
	pvTracks_toRefit.erase(pvTrack);
	break;
      }
    }
  }
}
//-------------------------------------------------------------------------------

TransientVertex NSVfitEventVertexRefitter::refit(const reco::Vertex* eventVertex, const std::vector<const reco::Track*>* svTracks)
{
//--- return (invalid) dummy vertex in case primary event vertex cannot be refitted,
//    due to insufficient information

  if ( !(eventVertex && beamSpot_ && trackBuilder_) ) return TransientVertex();

  std::vector<reco::TransientTrack> pvTracks_original;
  TransientTrackMap pvTrackMap_refit;
  for ( reco::Vertex::trackRef_iterator pvTrack = eventVertex->tracks_begin();
	pvTrack != eventVertex->tracks_end(); ++pvTrack ) {
    reco::TransientTrack pvTrack_transient = trackBuilder_->build(pvTrack->get());
    pvTracks_original.push_back(pvTrack_transient);
    pvTrackMap_refit.insert(std::make_pair(pvTrack->get(), pvTrack_transient));
  }

//--- exclude tracks associated to any one of the two tau lepton decay "legs"
//    from the primary event vertex refit
  if ( svTracks ) removeTracks(pvTrackMap_refit, *svTracks);

  std::vector<reco::TransientTrack> pvTracks_refit;
  for ( TransientTrackMap::iterator pvTrack = pvTrackMap_refit.begin();
	pvTrack != pvTrackMap_refit.end(); ++pvTrack ) {
    pvTracks_refit.push_back(pvTrack->second);
  }

//--- refit primary event vertex with "cleaned" track collection;
//    in case there are not "enough" tracks left to do the refit
//    after excluding the tracks associated to tau lepton decay "leg",
//    refit the primary event vertex with all tracks used in the original primary event vertex fit
  if ( pvTracks_refit.size() >= minNumTracksRefit_ ) {
    return vertexFitAlgorithm_->vertex(pvTracks_refit, *beamSpot_);
  } else {
    edm::LogWarning ("NSVfitEventVertexRefitter::refit")
      << "Insufficient tracks remaining after excluding tracks associated to tau decay products"
      << " --> skipping primary event vertex refit !!";
    // CV: need to enlarge errors on reconstructed event vertex position
    //     in SVfitLikelihoodDiTauTrackInfo in this case <-- FIXME
    return vertexFitAlgorithm_->vertex(pvTracks_original, *beamSpot_);
  }
}

TransientVertex NSVfitEventVertexRefitter::fitSecondaryVertex(const std::vector<const reco::Track*>& tracks) const 
{
//-- return dummy vertex for one-prong tau decays
//   for which tau decay vertex cannot be determined
  if ( tracks.size() < 2 || trackBuilder_ == 0 ) return TransientVertex();
  
//--- build transient tracks
  std::vector<reco::TransientTrack> transTracks;
  for ( std::vector<const reco::Track*>::const_iterator track = tracks.begin();
	track != tracks.end(); ++track ) {
    transTracks.push_back(trackBuilder_->build(*track));
  }
  
//--- fit tau decay vertex
  return vertexFitAlgorithm_->vertex(transTracks);
}
