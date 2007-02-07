#ifndef RecoEcal_MaterialConversionTools_Pi0ConversionAlgo_h
#define RecoEcal_MaterialConversionTools_Pi0ConversionAlgo_h

#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include <vector>
#include <set>
class TrajectoryStateUpdator;
class MeasurementEstimator;
class PropagatorWithMaterial;
class SteppingHelixPropagator;


class Pi0ConversionAlgo
{
 private:
  bool debug_;

  //Control the verbosity.
  int debugLevel_;
  PropagatorWithMaterial *thePropagator;
  PropagatorWithMaterial *theRevPropagator;
  TrajectoryStateUpdator *theUpdator;
  MeasurementEstimator* theEstimator;
 public:
  enum DebugLevel { pDEBUG = 0, pINFO = 1, pERROR = 2 };    
  //The default constructor
  Pi0ConversionAlgo(){ }
  
  //The real constructor
  Pi0ConversionAlgo(DebugLevel debugLevel = pINFO);

  //This object is really just a library of functions for helping to
  //recontruct pi0 conversions.

  //Given the two electron phi, pt, estimate the Conversion vertex R.
   float GetConversionR(float phi1, float pt1, float phi2, float pt2);

   //Given the two electron phi, pt, estimate conversion phi.
   float GetConversionPhi(float phi1, float pt1, float phi2, float pt2);

   //Make roads, fill them with tracking hits, and select the best candidate sets.
   std::vector <TrackingRecHit*> GetStubHits(BasicCluster ele1, BasicCluster ele2,
					     Float_t RConv, Float_t PhiConv, 
					     std::vector <TrackingRecHit*> FullTracker,
					     TrackerDigiGeometryRecord *geom,
					     std::vector <TrackingRecHit*> Stub1,
					     std::vector <TrackingRecHit*> Stub2);


   ////////////////////////
   //These two functions, but for a couple of changes of my own
   //come from RoadSearchTrackCandidateMaker
   TrackCollection FitTrack( vector <TrackingRecHit> Stub, 
				      MeasurementTracker *theMeasurementTracker, 
				      TransientTrackingRecHitBuilder *ttrhBuilder,
				      const MagneticField *magField);

   std::vector<TrajectoryMeasurement> FindBestHit(const TrajectoryStateOnSurface& tsosBefore,
						 const std::set<const GeomDet*>& theDets,
						 edm::OwnVector<TrackingRecHit>& theHits);
   //////////////////////
   Vertex FitVertex(TrackCollection tracks);


};
 
#endif
