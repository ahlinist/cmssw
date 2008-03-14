#include "DataFormats/ParticleFlowReco/interface/GsfPFRecTrack.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "Math/GenVector/PositionVector3D.h" 
#include "DataFormats/Math/interface/Point3D.h" 
// #include "FWCore/MessageLogger/interface/MessageLogger.h"

using namespace reco;
GsfPFRecTrack::GsfPFRecTrack(double charge, 
			     AlgoType_t algoType, 
			     int trackId, 
			     const reco::GsfTrackRef& gsftrackRef,
			     const edm::Ref<std::vector<PFRecTrack> >& kfpfrectrackref) : 
  PFRecTrack(charge,algoType), 
  gsfTrackRef_(gsftrackRef),
  kfPFRecTrackRef_(kfpfrectrackref),
  pfBremVec_(0)
{
trackId_=trackId;
}


  
void GsfPFRecTrack::addBrem(const reco::PFBrem& brem){
  pfBremVec_.push_back(brem);
}


