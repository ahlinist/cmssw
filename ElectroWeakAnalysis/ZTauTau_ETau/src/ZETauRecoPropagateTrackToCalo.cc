
#include "ElectroWeakAnalysis/ZTauTau_ETau/interface/ZETauRecoPropagateTrackToCalo.h"

GlobalPoint PropagateTrackToCalo (const reco::Track& fTrack,const MagneticField& fField,const Propagator& fPropagator)
{

  const double rBarrel = 143.;
  const double zEndcap = 320.;
  const double zVF = 1100.;
  const double rEndcapMin = zEndcap * tan ( 2*atan (exp (-3.)));
  const double rVFMin = zEndcap * tan ( 2*atan (exp (-5.191)));

  GlobalPoint trackPosition (fTrack.vx(), fTrack.vy(), fTrack.vz()); // reference point
  GlobalVector trackMomentum (fTrack.px(), fTrack.py(), fTrack.pz()); // reference momentum
  try {
    if (fTrack.extra().product ()) { // use outer point information, if available
      trackPosition =  GlobalPoint (fTrack.outerX(), fTrack.outerY(), fTrack.outerZ());
      trackMomentum = GlobalVector (fTrack.outerPx(), fTrack.outerPy(), fTrack.outerPz());
    }
  }
  catch (...) {}
 
  GlobalTrajectoryParameters trackParams(trackPosition, trackMomentum, fTrack.charge(), &fField);
  FreeTrajectoryState trackState (trackParams);
  
  // first propagate to barrel
  TrajectoryStateOnSurface 
    propagatedInfo = fPropagator.propagate (trackState, 
					    *Cylinder::build (Surface::PositionType (0,0,0),
							      Surface::RotationType(),
							      rBarrel)
					    );
  if (propagatedInfo.isValid()) {
    GlobalPoint result (propagatedInfo.globalPosition ());
    if (fabs (result.z()) < zEndcap) {
      // 	std::cout << "propagateTrackToCalo-> propagated to barrel:"
      // 		  << " x/y/z/r: " << result.x() << '/' << result.y() << '/' << result.z() << '/' << result.perp()
      // 		  << std::endl;
      return result;
    }
  }
  
  // failed with barrel, try endcap
  double zTarget = trackMomentum.z() > 0 ? zEndcap : -zEndcap;
  propagatedInfo = fPropagator.propagate (trackState, 
					  *Plane::build( Surface::PositionType(0, 0, zTarget),
							 Surface::RotationType())
					  );
  if (propagatedInfo.isValid()) {
    GlobalPoint result (propagatedInfo.globalPosition ());
    if (fabs (result.perp()) > rEndcapMin) {
      // 	std::cout << "propagateTrackToCalo-> propagated to endcap:"
      // 		  << " x/y/z/r: " << result.x() << '/' << result.y() << '/' << result.z() << '/' << result.perp()
      // 		  << std::endl;
      return result;
    }
  }
  // failed with endcap, try VF
  zTarget = trackMomentum.z() > 0 ? zVF : -zVF;
  propagatedInfo = fPropagator.propagate (trackState, 
					  *Plane::build( Surface::PositionType(0, 0, zTarget),
							 Surface::RotationType())
					  );
  if (propagatedInfo.isValid()) {
    GlobalPoint result (propagatedInfo.globalPosition ());
    if (fabs (result.perp()) > rVFMin) {
      // 	std::cout << "propagateTrackToCalo-> propagated to VF:"
      // 		  << " x/y/z/r: " << result.x() << '/' << result.y() << '/' << result.z() << '/' << result.perp()
      // 		  << std::endl;
      return result;
    }
  }
  // no luck
  //     std::cout << "propagateTrackToCalo-> failed to propagate track to calorimeter" << std::endl;
  return GlobalPoint (0, 0, 0);
}
