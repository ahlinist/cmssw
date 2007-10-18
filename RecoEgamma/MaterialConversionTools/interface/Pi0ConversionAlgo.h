#ifndef RecoEgamma_MaterialConversionTools_Pi0ConversionAlgo_h
#define RecoEgamma_MaterialConversionTools_Pi0ConversionAlgo_h

#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include <vector>
#include <set>
class TrajectoryStateUpdator;
class TrajectoryMeasurement;
class TrackerDigiGeometryRecord;
class TrackerGeometry;
class MeasurementEstimator;
class MeasurementTracker;
class PropagatorWithMaterial;
class SteppingHelixPropagator;
class TrackingRecHit;
class TransientTrackingRecHitBuilder;
class TrajectoryStateOnSurface;
class MagneticField;
class GeomDet;
class CachingVertex;
class TransientTrack;
/* class GsfChi2MeasurementEstimator; */
/* class GsfPropagatorWithMaterial; */
/* class GsfMaterialEffectsUpdator; */

class Pi0ConversionAlgo
{
 private:
  bool debug_;

  //Control the verbosity.
  int debugLevel_;
  float maxHitChi2_;
   TrajectoryStateUpdator *theUpdator; 
   MeasurementEstimator* theEstimator; 
//  GsfMaterialEffectsUpdator *theUpdator;
//  GsfChi2MeasurementEstimator *theEstimator;

 public:
  enum DebugLevel { pDEBUG = 0, pINFO = 1, pERROR = 2 };    
  //The default constructor
  Pi0ConversionAlgo();
  ~Pi0ConversionAlgo();
  //The real constructor
  Pi0ConversionAlgo(DebugLevel debugLevel = pINFO, float maxchi2=100);

  //This object is really just a library of functions for helping to
  //recontruct pi0 conversions.

  //Given the two electron phi, pt, estimate the Conversion vertex R.
   float GetConversionR(float phi1, float pt1, float phi2, float pt2);

   //Given the two electron phi, pt, estimate conversion phi.
   float GetConversionPhi(float phi1, float pt1, float phi2, float pt2);

   //Make roads, fill them with tracking hits, and select the best candidate sets.
   void GetStubHits(reco::BasicCluster ele1, reco::BasicCluster ele2,
		    Float_t RConv, Float_t PhiConv, 
		    std::vector <TrackingRecHit*> *FullTracker,
		    const TrackerGeometry *geom,
		    std::vector <TrackingRecHit*> *Stub1,
		    std::vector <TrackingRecHit*> *Stub2, int&, int&);
   

   ////////////////////////
   //These two functions, but for a couple of changes of my own
   //come from RoadSearchTrackCandidateMaker
   void FitTrack( std::vector <TrackingRecHit*> Stub, 
		  const MeasurementTracker *theMeasurementTracker, 
		  const TransientTrackingRecHitBuilder *ttrhBuilder,
		  const MagneticField *magField,
		  const TrackerGeometry *geom, const edm::EventSetup& iSetup, 
		  reco::TrackCollection &trkCan,
		  reco::TrackExtraCollection &trkColl,
		  edm::OwnVector <TrackingRecHit> &returnhits);

   std::vector<TrajectoryMeasurement> FindBestHit(const TrajectoryStateOnSurface& tsosBefore,
						 const std::set<const GeomDet*>& theDets,
						  edm::OwnVector<TrackingRecHit>& theHits,
						  const TrackerGeometry *geom,
						  const TransientTrackingRecHitBuilder *ttrhBuilder,
						  const MeasurementTracker *theMeasurementTracker,
						  const PropagatorWithMaterial *thePropagator);


/*    std::vector<TrajectoryMeasurement> FindBestHit(const TrajectoryStateOnSurface& tsosBefore, */
/* 						 const std::set<const GeomDet*>& theDets, */
/* 						  edm::OwnVector<TrackingRecHit>& theHits, */
/* 						  const TrackerGeometry *geom, */
/* 						  const TransientTrackingRecHitBuilder *ttrhBuilder, */
/* 						  const MeasurementTracker *theMeasurementTracker, */
/* 						  const GsfPropagatorWithMaterial *thePropagator); */
   //////////////////////
   CachingVertex FitVertex(std::vector<reco::TransientTrack> tracks);


};
 
#endif
