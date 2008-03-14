#ifndef PFTrackTransformer_H
#define PFTrackTransformer_H

#include "RecoParticleFlow/PFBlockAlgo/interface/PFGeometry.h"
#include "DataFormats/GeometrySurface/interface/Surface.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecTrack.h"
#include "DataFormats/ParticleFlowReco/interface/GsfPFRecTrack.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/GeometryVector/interface/GlobalVector.h"
/// \brief Abstract
/*!
\author Michele Pioppi
\date January 2007

 PFTrackTransformer is the class used to transform 
 pairs tracks-trajectories in pfrectracks that can be 
 used in the PFlow FW.
 Other utilities: 
 Get TSOS on surface defined in PFGEometry 
 Evaluate the surface corresponding to the maximum shower
*/


class Trajectory;
class PFTrackTransformer{



 public:
  PFTrackTransformer();
  ~PFTrackTransformer();


  /// Add points to a PFTrack. return false if a TSOS is invalid
  bool addPoints(reco::PFRecTrack& pftrack, 
		 const reco::Track& track,
		 const Trajectory& traj) const; 

  bool addPointsAndBrems(reco::GsfPFRecTrack& pftrack, 
			 const reco::Track& track,
			 const Trajectory& traj,
			 const bool& GetMode) const; 
  

  void OnlyProp(){
    onlyprop_=true;
  }
  bool  onlyprop_;
  
 private:

};

#endif
