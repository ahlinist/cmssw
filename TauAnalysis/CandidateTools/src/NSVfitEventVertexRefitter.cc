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
  std::string algorithm = cfg.getParameter<std::string>("algorithm");
  if      ( algorithm == "KalmanVertexFitter"   ) vertexFitAlgorithm_ = new KalmanVertexFitter();
  else if ( algorithm == "AdaptiveVertexFitter" ) vertexFitAlgorithm_ = new AdaptiveVertexFitter();  
  else throw cms::Exception("NSVfitEventVertexRefitter")
    << " Invalid Configuration Parameter 'algorithm' = " << algorithm << " !!\n";
  
  minNumTracksRefit_ = ( cfg.exists("minNumTracksRefit") ) ?
    cfg.getParameter<unsigned>("minNumTracksRefit") : 2;

  applyBeamSpotConstraint_ = cfg.getParameter<bool>("applyBeamSpotConstraint");

  verbosity_ = cfg.exists("verbosity") ?
    cfg.getParameter<int>("verbosity") : 0;
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
  if ( beamSpot_ ) {
    beamSpotState_ = VertexState(*beamSpot_);    
    beamSpotIsValid_ = (beamSpotState_.error().cxx() > 0. &&
			beamSpotState_.error().cyy() > 0. &&
			beamSpotState_.error().czz() > 0.);
  } else {
    edm::LogError ("NSVfitEventVertexRefitter::beginEvent")
      << " Failed to access BeamSpot !!";
    beamSpotIsValid_ = false;
  }

//--- get pointer to TransientTrackBuilder
  edm::ESHandle<TransientTrackBuilder> trackBuilderHandle;
  es.get<TransientTrackRecord>().get("TransientTrackBuilder", trackBuilderHandle);
  trackBuilder_ = trackBuilderHandle.product();
  if ( !trackBuilder_ ) 
    throw cms::Exception("NSVfitEventVertexRefitter::beginEvent")
      << " Failed to access TransientTrackBuilder !!\n";
}

//-------------------------------------------------------------------------------
// auxiliary functions to compare two Tracks 
bool tracksMatchByDeltaR(const reco::Track* trk1, const reco::Track* trk2)
{
  if ( reco::deltaR(*trk1, *trk2) < 1.e-2 && trk1->charge() == trk2->charge() ) return true;
  else return false;
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

void printTracks(const TransientTrackMap& pvTrackMap)
{
  int idx = 0;
  for ( TransientTrackMap::const_iterator pvTrack = pvTrackMap.begin();
	pvTrack != pvTrackMap.end(); ++pvTrack ) {
    std::cout << "Track #" << idx << ": Pt = " << pvTrack->first->pt() << "," 
	      << " eta = " << pvTrack->first->eta() << ", phi = " << pvTrack->first->phi() 
	      << " (charge = " << pvTrack->first->charge() << ", chi2 = " << pvTrack->first->normalizedChi2() << ")" << std::endl;
    ++idx;
  }
}

TransientVertex NSVfitEventVertexRefitter::refit(const reco::Vertex* eventVertex, const std::vector<const reco::Track*>* svTracks) const
{
//--- return (invalid) dummy vertex in case primary event vertex cannot be refitted,
//    due to insufficient information

  if ( verbosity_ >= 1 ) {
    std::cout << "<NSVfitEventVertexRefitter::refit>:" << std::endl;
    std::cout << " vertex: x = " << eventVertex->position().x() << ", y = " << eventVertex->position().y() << ", z = " << eventVertex->position().z() << std::endl;
    std::cout << " #svTracks = " << svTracks->size() << ":" << std::endl;  
    if ( verbosity_ >= 2 ) {
      int idx = 0;
      for ( std::vector<const reco::Track*>::const_iterator svTrack = svTracks->begin();
	    svTrack != svTracks->end(); ++svTrack ) {
	std::cout << "Track #" << idx << ": Pt = " << (*svTrack)->pt() << "," 
		  << " eta = " << (*svTrack)->eta() << ", phi = " << (*svTrack)->phi() 
		  << " (charge = " << (*svTrack)->charge() << ", chi2 = " << (*svTrack)->normalizedChi2() << ")" << std::endl;
	++idx;
      }
    }
    std::cout << " beam-spot:" 
	      << " x = " << beamSpot_->position().x() << " +/- " << TMath::Sqrt(beamSpotState_.error().cxx()) << "," 
	      << " y = " << beamSpot_->position().y() << " +/- " << TMath::Sqrt(beamSpotState_.error().cyy()) << "," 
	      << " y = " << beamSpot_->position().z() << " +/- " << TMath::Sqrt(beamSpotState_.error().czz()) << std::endl;
  }

  if ( !(eventVertex && beamSpot_) ) return TransientVertex();
  assert(trackBuilder_);

  std::vector<reco::TransientTrack> pvTracks_original;
  TransientTrackMap pvTrackMap_refit;
  for ( reco::Vertex::trackRef_iterator pvTrack = eventVertex->tracks_begin();
	pvTrack != eventVertex->tracks_end(); ++pvTrack ) {
    reco::TransientTrack pvTrack_transient = trackBuilder_->build(pvTrack->get());
    pvTracks_original.push_back(pvTrack_transient);
    pvTrackMap_refit.insert(std::make_pair(pvTrack->get(), pvTrack_transient));
  }
  if ( verbosity_ >= 1 ) {
    std::cout << "#pvTracks (before excl. leptons) = " << pvTrackMap_refit.size() << std::endl;
    if ( verbosity_ >= 2 ) printTracks(pvTrackMap_refit);
  }

//--- exclude tracks associated to any one of the two tau lepton decay "legs"
//    from the primary event vertex refit
  if ( svTracks ) removeTracks(pvTrackMap_refit, *svTracks);
  if ( verbosity_ >= 1 ) {
    std::cout << "#pvTracks (after excl. leptons) = " << pvTrackMap_refit.size() << std::endl;
    if ( verbosity_ >= 2 ) printTracks(pvTrackMap_refit);
  }

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
    if ( applyBeamSpotConstraint_ && beamSpotIsValid_ ) return vertexFitAlgorithm_->vertex(pvTracks_refit, *beamSpot_);
    else return vertexFitAlgorithm_->vertex(pvTracks_refit);
  } else {
    edm::LogWarning ("NSVfitEventVertexRefitter::refit")
      << "Insufficient tracks remaining after excluding tracks associated to tau decay products"
      << " --> skipping primary event vertex refit !!";
    // CV: need to enlarge errors on reconstructed event vertex position
    //     in SVfitLikelihoodDiTauTrackInfo in this case <-- FIXME    
    if ( applyBeamSpotConstraint_ && beamSpotIsValid_ ) return vertexFitAlgorithm_->vertex(pvTracks_original, *beamSpot_);
    else return vertexFitAlgorithm_->vertex(pvTracks_original);
  }
}
