
// Package:         RecoEcal/ConversionMaterialTools
// Class:           Pi0ConversionAlgo
// 
// Description:     Creates roads based on BasicClusters in ECAL
//                  Selects hits in these roads 
//                  Fits hits into tracks
//                  Vertexes tracks together
//
// Original Author: Andrew Askew, askew@fnal.gov
// Created:         Wed Feb 7 13:00:00 UTC 2006

#include <cmath>
#include <vector>
#include "RecoEgamma/MaterialConversionTools/interface/Pi0ConversionAlgo.h"
#include "RecoEgamma/MaterialConversionTools/interface/StubCandidate.h" 

#include "DataFormats/EgammaReco/interface/BasicCluster.h"

#include "DataFormats/TrackCandidate/interface/TrackCandidate.h"
#include "DataFormats/TrackReco/interface/TrackExtra.h"
#include "DataFormats/TrackCandidate/interface/TrackCandidateCollection.h"
#include "DataFormats/Common/interface/OwnVector.h"
 
#include "RecoTracker/TrackProducer/interface/TrackingRecHitLessFromGlobalPosition.h"
 
#include "Geometry/CommonDetUnit/interface/TrackingGeometry.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
 
#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h" 
 
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "TrackingTools/GeomPropagators/interface/AnalyticalPropagator.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"
#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHitBuilder.h" 
#include "TrackingTools/Records/interface/TransientRecHitRecord.h" 

#include "TrackingTools/PatternTools/interface/Trajectory.h"
#include "TrackingTools/MaterialEffects/interface/PropagatorWithMaterial.h"
//#include "TrackingTools/GsfTracking/interface/GsfPropagatorWithMaterial.h"
//#include "TrackingTools/GsfTracking/interface/GsfChi2MeasurementEstimator.h"

#include "TrackingTools/KalmanUpdators/interface/KFUpdator.h"
//#include "TrackingTools/GsfTracking/interface/GsfMaterialEffectsUpdator.h"
#include "TrackingTools/PatternTools/interface/TrajectoryStateUpdator.h"
#include "TrackingTools/PatternTools/interface/MeasurementEstimator.h"
#include "TrackingTools/KalmanUpdators/interface/Chi2MeasurementEstimatorBase.h"
#include "TrackingTools/KalmanUpdators/interface/Chi2MeasurementEstimator.h"
#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHitBuilder.h"
#include "TrackingTools/PatternTools/interface/TrajectoryMeasurement.h"

#include "TrackingTools/TrajectoryCleaning/interface/TrajectoryCleaner.h"
#include "TrackingTools/TrajectoryCleaning/interface/TrajectoryCleanerBySharedHits.h"
#include "RecoTracker/MeasurementDet/interface/MeasurementTracker.h"
#include "RecoTracker/TkDetLayers/interface/GeometricSearchTracker.h"
#include "RecoTracker/TkSeedGenerator/interface/FastHelix.h"
#include "RecoTracker/RoadSearchSeedFinder/interface/RoadSearchSeedFinderAlgorithm.h"
#include "TrackingTools/TrackFitters/interface/KFTrajectorySmoother.h"
#include "TrackingTools/TrackFitters/interface/KFTrajectoryFitter.h"

#include "RecoTracker/Record/interface/CkfComponentsRecord.h"
#include "TrackingTools/TrajectoryState/interface/BasicSingleTrajectoryState.h"
#include "TrackPropagation/SteppingHelixPropagator/interface/SteppingHelixPropagator.h"
#include "TrackingTools/PatternTools/interface/TSCPBuilderNoMaterial.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h"
#include "RecoVertex/AdaptiveVertexFit/interface/AdaptiveVertexFitter.h"
#include "RecoVertex/VertexPrimitives/interface/CachingVertex.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"


#include <TMath.h>

using namespace std; 
using namespace reco;

Pi0ConversionAlgo::Pi0ConversionAlgo(){
  debug_ = false;
  maxHitChi2_ = 100;
  theEstimator = new Chi2MeasurementEstimator(100);
  theUpdator = new KFUpdator();
  // need this to sort recHits, sorting done after getting seed because propagationDirection is needed
  // get tracker geometry

}

Pi0ConversionAlgo::Pi0ConversionAlgo(DebugLevel debugLevel, float maxHitChi2)
{
  debugLevel_ = debugLevel;
  if (debugLevel_ == 0)
    debug_ = true;
  else debug_ = false;
  maxHitChi2_ = maxHitChi2;
  theEstimator = new Chi2MeasurementEstimator(100);
  theUpdator = new KFUpdator();
  // need this to sort recHits, sorting done after getting seed because propagationDirection is needed
  // get tracker geometry


}

Pi0ConversionAlgo::~Pi0ConversionAlgo(){

  delete theUpdator;
  delete theEstimator;
}


float Pi0ConversionAlgo::GetConversionR(float phi1, float pt1, float phi2, float pt2) 
{
  float ConversionR=-1; 
  float CONSTFORCURVE = 1e9 / 299792458.; //Need a constant to put everything into measurement units.
  float MAGFIELD = 4.;//In Tesla
  float RadiusOfECAL = 120.;//in cm.
  float RadiusOfCurvature1 = (pt1 * CONSTFORCURVE / MAGFIELD) * 100.;//in cm.
  float RadiusOfCurvature2 = (pt2 * CONSTFORCURVE / MAGFIELD) * 100.;
  float Delta = 0; //Delta is defined as the magnitude of the difference between phi1 and phi2.
  float DeltaRaw = fabs(phi2 - phi1);
  if (DeltaRaw > TMath::Pi()) Delta = 2. * TMath::Pi() - DeltaRaw;
  else Delta = DeltaRaw;
  //With Calorimeter Radius, the two radii of curvature, and the final phi difference, estimte the conversion radius
  //Note:  Use of R1, R2, is arbitrary here, since I only deal in magnitudes (sum and product of positive numbers, difference in phi).
  //The ordering only matters in PHI (calculation below).
  float ReducedCurvature = (RadiusOfCurvature1*RadiusOfCurvature2)/(RadiusOfCurvature1 + RadiusOfCurvature2);
  ConversionR = RadiusOfECAL - sqrt( 2. * RadiusOfECAL * ReducedCurvature * Delta);
  return ConversionR;
}

float Pi0ConversionAlgo::GetConversionPhi(float phi1, float pt1, float phi2, float pt2)
{
  //What the frack?  The range of phi is supposed to be 0, 2pi
  if (phi1 < 0) phi1 +=TMath::Pi()*2.;
  if (phi2 < 0) phi2 +=TMath::Pi()*2.;
  
  float CONSTFORCURVE = 1e9 / 299792458.; //Need a constant to put everything into measurement units.
  float MAGFIELD = 4.;//In Tesla
  //  float RadiusOfECAL = 120.;//in cm.
  float RadiusOfCurvature1 = (pt1 * CONSTFORCURVE / MAGFIELD) * 100.;//in cm.
  float RadiusOfCurvature2 = (pt2 * CONSTFORCURVE / MAGFIELD) * 100.;
  //Before going any further, check if phi1 > phi2
  float PHI1=0;
  float PHI2=0;
  float ARR1 =0;
  float ARR2 =0;
  float fidiff = fabs(phi1-phi2);
  if (debug_){
    std::cout << "Conversion phi calculation" << std::endl;
    std::cout << "Radius1: " << RadiusOfCurvature1 << std::endl;
    std::cout << "Radius2: " << RadiusOfCurvature2 << std::endl;
    std::cout << "Phi1: " << phi1 << std::endl;
    std::cout << "Phi2: " << phi2 << std::endl;
  }
  if (phi1 > phi2 && fidiff < TMath::Pi()){
    PHI1 = phi1;
    PHI2 = phi2;
    ARR1 = RadiusOfCurvature1;
    ARR2 = RadiusOfCurvature2;
  }
  if (phi2 > phi1 && fidiff < TMath::Pi()){
    PHI1 = phi2;
    PHI2 = phi1;
    ARR1 = RadiusOfCurvature2;
    ARR2 = RadiusOfCurvature1;
  }
  if (fidiff > TMath::Pi()){
    if (phi1 < TMath::Pi()){
      PHI1 = phi1 + TMath::Pi()*2.;
      PHI2 = phi2;
      ARR1 = RadiusOfCurvature1;
      ARR2 = RadiusOfCurvature2;
    }
    else{
      PHI1 = phi2 + TMath::Pi()*2.;
      PHI2 = phi1;
      ARR1 = RadiusOfCurvature2;
      ARR2 = RadiusOfCurvature1;
    }
  }
  if (debug_){
    std::cout << "Chose: " << std::endl;
    std::cout << "PHI1: " << PHI1 << std::endl;
    std::cout << "PHI2: " << PHI2 << std::endl;
    std::cout << "ARR1: " << ARR1 << std::endl;
    std::cout << "ARR2: " << ARR2 << std::endl;
  }

  float Delta = PHI2 - PHI1;//This should be negative
  float Delta1 = -1.*(ARR2/(ARR1+ARR2)) * Delta;

  if (debug_) std::cout << "Delta1: " << Delta1 << endl;

  //Note that I cancelled the negative sign with Delta above.

  float PhiConv=-1;
  PhiConv = PHI1 - Delta1;
  if (PhiConv > TMath::Pi()*2.)
    PhiConv -=TMath::Pi()*2.;
  if (PhiConv < 0)
    PhiConv +=TMath::Pi()*2.;
  if (debug_) std::cout << "returning: " << PhiConv << std::endl;
  return PhiConv;  
}

void Pi0ConversionAlgo::GetStubHits(BasicCluster ele1, BasicCluster ele2,
				    Float_t RConv, Float_t PhiConv, 
				    std::vector <TrackingRecHit*> *FullTracker,
				    const TrackerGeometry *geom,
				    std::vector <TrackingRecHit*> *Stub1,
				    std::vector <TrackingRecHit*> *Stub2,
				    int &char1,int &char2)
{
  //Bits of this were originally in RSAdapter, and ConversionCandidate, but StubCandidate is
  //used almost untouched.
  

//   vector <TrackingRecHit*> hits_stub1;
//   vector <TrackingRecHit*> hits_stub2;

  //Make stubs
  //Unique case: Identify the positive and negative charged stubs (indicates just the sign
  //of curvature) from the phi positions of the clusters.
  Float_t Phi1 = ele1.phi();
  Float_t Phi2 = ele2.phi();
  Int_t charge1=0;
  Int_t charge2=0;
  
  Float_t PhiDiff = Phi1 - Phi2;
  if (fabs(PhiDiff) < TMath::Pi()){
    if (PhiDiff > 0) {
      charge1 = 1;
      charge2 = -1;
    }
    else{
      charge1 = -1;
      charge2 = 1;
    }
  }
  else{
    if(Phi1 >TMath::Pi() 
       && Phi2 < (TMath::Pi()/2.)){
      charge1 = -1;
      charge2 = 1;
    }
    else{
      charge1 = 1;
      charge2 = -1;
    }
  }
  char1 = charge1;
  char2 = charge2;
  StubCandidate Stub1S(ele1.x(), ele1.y(), ele1.z(), ele1.energy(),0,RConv,PhiConv,charge1);
  StubCandidate Stub2S(ele2.x(), ele2.y(), ele2.z(), ele2.energy(),0,RConv,PhiConv,charge2);
 
  //Using this constructor, the roads should be created.  Remember to check this when debugging.  
  //Select hits that are in road.
  //  std::cout << "----------------------------------" << std::endl;
  //  std::cout << "Making stub collections: " << std::endl;
  if (debug_) std::cout << "Total tracker hits: " << FullTracker->size() << std::endl;
  //Make one loop through the hits, and then you're done.
  for (int th=0;th < int(FullTracker->size()); ++th){
    //get globalpoint position, and feed hit to stubs
    TrackingRecHit *htr = (*FullTracker)[th];
    
    GlobalPoint position = geom->idToDet( 
					 htr->geographicalId()
					 )->surface().toGlobal(
							       htr->localPosition());
    if (th==0 && debug_){
      std::cout << "dumping first hit: " << std::endl;
      std::cout << "X: " << position.x() << std::endl;
      std::cout << "Y: " << position.y() << std::endl;
      std::cout << "Z: " << position.z() << std::endl;
    }
    float position_err_xx = htr->localPositionError().xx();
    float position_err_yy = htr->localPositionError().yy();
//     if (debug_){
//       std::cout << "----------" << std::endl;
//       std::cout << "Hit error local yy: " << position_err_yy << std::endl;
//       std::cout << "Hit error local xx: " << position_err_xx << std::endl;
//     }
    //    std::cout << "Hit: "<<th <<"  R position: " << position.perp() << std::endl;
    //Can I auto stiletto-ize?  Get "IsHitInRoad", and Chi2XY for hit.  Whoever has smallest Chi2 gets the hit.
    bool S1 =   Stub1S.IsInRoad(position, position_err_xx, position_err_yy);
    bool S2 =   Stub2S.IsInRoad(position, position_err_xx, position_err_yy);
    bool HitTaken = false;

    double S1Chi2XY = 1e16;
    double S2Chi2XY = 1e16;
    if (S1) S1Chi2XY = Stub1S.Chi2Hit(position);
    if (S2) S2Chi2XY = Stub2S.Chi2Hit(position);
    
    //Check hit against first stub:
    //If it HAS the hit, and the smallest chi2, add it.
    if (S1 && S1Chi2XY < S2Chi2XY && S1Chi2XY < maxHitChi2_)
      {
	//Stub1 gets the hit.
	HitTaken = true;
	Stub1->push_back((*FullTracker)[th]);
	if (debug_) std::cout<< "Took hit with chi2xy: " << S1Chi2XY << std::endl;
      }
    
    if (!HitTaken){
      if (S2 && S2Chi2XY < S1Chi2XY && S2Chi2XY < maxHitChi2_){
	HitTaken = true;
	Stub2->push_back((*FullTracker)[th]);
	if (debug_) std::cout << "Took hit with chi2xy: " << S2Chi2XY << std::endl;
      }
    }

  
  }//Done Looping over all the hits.
  


  //Done making hit collections!
}


void Pi0ConversionAlgo::FitTrack( vector <TrackingRecHit*> Stub, 
				  const MeasurementTracker *theMeasurementTracker, 
				  const TransientTrackingRecHitBuilder *ttrhBuilder,
				  const MagneticField *magField,
				  const TrackerGeometry *geom,
				  const edm::EventSetup& es,
				  reco::TrackCollection &trkCan,
				  reco::TrackExtraCollection &trkColl,
				  edm::OwnVector<TrackingRecHit> &returnhits
				  )
{
  //Around 900 lines of code regarding the fitting of the candidate hits into a track.  That is
  //of course, presuming that we've GOT the right hits.
  if (debug_) std::cout << "Creating output collection, trajectory cleaner, and Trajectory collection" << std::endl;
  TrackCandidateCollection output;
  std::vector<reco::Track> trackvec; 
  // Create the trajectory cleaner 
  TrajectoryCleanerBySharedHits theTrajectoryCleaner;
  vector<Trajectory> FinalTrajectories;
   PropagatorWithMaterial *thePropagator = new PropagatorWithMaterial(alongMomentum,.1,&(*magField), .785); 
  //GsfPropagatorWithMaterial *thePropagator = new PropagatorWithMaterial(alongMomentum,.000511,&(*magField), .785); 

  if (debug_) std::cout << "creating propagator with material" << std::endl;
  PropagatorWithMaterial *theRevPropagator = new PropagatorWithMaterial(oppositeToMomentum,.1,&(*magField), .785); 
  AnalyticalPropagator prop(magField,anyDirection);
  TrajectoryStateTransform transformer;
  if (debug_) std::cout << "Prior to creating smoother." << std::endl;
  KFTrajectorySmoother theSmoother(*theRevPropagator, *theUpdator, *theEstimator);
 
  if (debug_) std::cout << std::endl << std::endl
			<< "*** NEW EVENT: Clean Clouds input size: " << Stub.size() << std::endl;

  int i_c = 0;
  int nchit = 0;
  //This WAS the beginning of a loop.  However, right now we'll try to fit one stub at a time.
  {//This is a placeholder for the loop over the clouds

    //Die screaming if too many hits.  What's too many?  Don't know yet.
    if (Stub.size()>500) return;


    // fill rechits from cloud into new OwnVector
    //Clouds have their own function for this, I just fill in the vector.
    //    edm::OwnVector<TrackingRecHit> recHits = cloud->recHits();
    edm::OwnVector<TrackingRecHit> recHits;
    for (int i=0;i<int(Stub.size());++i){
      recHits.push_back(Stub[i]->clone());
    }
    nchit = recHits.size();
    
    vector<Trajectory> CloudTrajectories;
    
    //Not Sure what to do with these!
//     RoadSearchCloud::SeedRefs theSeeds = cloud->seeds();
//     RoadSearchCloud::SeedRefs::const_iterator iseed;
    //Additional code may be needed!

    //////////////HIT SORTING BY RADIUS
    //The following piece of code is very familiar.  The ORCA version does effectively the same thing,
    //so no change is necessary.
    //    recHits.sort(TrackingRecHitLessFromGlobalPosition(((TrackingGeometry*)(&(*tracker))),alongMomentum));
    recHits.sort(TrackingRecHitLessFromGlobalPosition(geom, alongMomentum));
    // make a list of layers in cloud and mark stereo layers
    const int theNumHitCut = 4;
    const int max_layers = 128;
    const DetLayer* layers[max_layers];
    bool lstereo[max_layers];
    int nhits_l[max_layers];
    int nlayers = 0;
    
    map<const DetLayer*, int> cloud_layer_reference; // for debugging
    std::multimap<const DetLayer*, const TrackingRecHit* > cloud_layer_map;
    std::multimap<const DetLayer*, const TrackingRecHit* >::const_iterator hiter;
    for (edm::OwnVector<TrackingRecHit>::const_iterator ih=recHits.begin(); ih!=recHits.end(); ++ih) {
      const DetLayer* hitLayer =
	theMeasurementTracker->geometricSearchTracker()->detLayer(ih->geographicalId());
      int ilayer = -1;
      hiter = cloud_layer_map.find(hitLayer);
      if (ih == recHits.begin() || hitLayer != layers[nlayers-1]) {
	
	if (hiter == cloud_layer_map.end()) {// new layer
	  if (nlayers >= max_layers) break; // should never happen
	  layers[nlayers] = hitLayer;
	  lstereo[nlayers] = false;
	  nhits_l[nlayers] = 0;
	  cloud_layer_reference.insert(make_pair(layers[nlayers], nlayers));
	  ilayer = nlayers;
	  ++nlayers;
	}
	else{
	  map<const DetLayer*, int>::const_iterator ilyr = cloud_layer_reference.find(hitLayer);
	  ilayer = ilyr->second;
	}
      }
      else {
	ilayer = nlayers-1;
      }
      ++nhits_l[ilayer];
      if (ih->localPositionError().yy()<1.) lstereo[ilayer] = true;      
      cloud_layer_map.insert(make_pair(hitLayer, &(*ih)));
      GlobalPoint gp = geom->idToDet(ih->geographicalId())->surface().toGlobal(ih->localPosition());
      if (debug_) std::cout << "Hit "<< ih-recHits.begin()
                            << " r/z = "
                            << gp.perp() << " " << gp.z()
                            <<" in layer " << hitLayer << " layer number " << ilayer
                            << " with " << nhits_l[ilayer] << "  hits " << std::endl;
    }
    if (debug_) std::cout<<"CLOUD LAYER MAP SIZE IS " << cloud_layer_map.size() << std::endl;
    //END HIT SORTING BY RADIUS
    

    if (debug_) std::cout <<"Cloud "<<i_c<<" has "<<recHits.size()<<" hits in " << nlayers << " layers ";
    ++i_c;
    

    // try to start from all layers until the chunk is too short
    //
    
    // already probed layers
    set<const DetLayer*> prev_layers;
    
    //Road search default is 7, I have pushed this back to five (note that 5 was commented out in the
    //original code.
    const int min_chunk_length = 6;
    //const int min_chunk_length = 7;
    
    for (int ilayer0 = 0; ilayer0 <= nlayers-min_chunk_length; ++ilayer0) {
      
      vector<Trajectory> ChunkTrajectories;
      vector<Trajectory> CleanChunks;
      
      if (debug_) std::cout  << "*** START NEW CHUNK --> layer range (" << ilayer0 << "-" << nlayers-1 << ")";
      
      // skip hits from previous layer
      if (ilayer0>0) prev_layers.insert(layers[ilayer0-1]);
      
      // collect hits from the starting layer
      edm::OwnVector<TrackingRecHit> recHits_start;
      for (edm::OwnVector<TrackingRecHit>::const_iterator ih = recHits.begin();
	   ih != recHits.end(); ++ih) {
	if (theMeasurementTracker->geometricSearchTracker()->detLayer(ih->geographicalId()) == layers[ilayer0]) {
	  recHits_start.push_back(ih->clone());
	}
      }
      
      //
      // Step 1: find small tracks (chunks) made of hits
      // in layers with low occupancy
      //
      
      // find layers with small number of hits
      // TODO: try to keep earliest layers + at least one stereo layer
      multimap<int, const DetLayer*> layer_map;
      map<const DetLayer*, int> layer_reference; // for debugging
      // skip starting layer, as it is always included
      for (int ilayer = ilayer0+1; ilayer < nlayers; ++ilayer) {
	layer_map.insert(make_pair(nhits_l[ilayer], layers[ilayer]));
	layer_reference.insert(make_pair(layers[ilayer], ilayer));
      }
      
      
      
      if (debug_) std::cout<<std::endl<<"   Available layers are: ";
      for (multimap<int, const DetLayer*>::iterator ilm1 = layer_map.begin();
	   ilm1 != layer_map.end(); ++ilm1) {
	map<const DetLayer*, int>::iterator ilr = layer_reference.find(ilm1->second);
	if (ilr != layer_reference.end() && debug_) 
	  std::cout << "Layer " << ilr->second << " with " << nhits_l[ilr->second]<<" hits" <<std::endl;;
      }
      if (debug_) cout << std::endl;
      
      
      // consider the best nfound_min layers + other layers with only one hit
      
      // This has implications, based on the way we locate the hits.  
      // For now, use only the low occupancy layers in the first pass
      const int nfound_min = min_chunk_length-1;
      //const int nfound_min = 4;
      multimap<int, const DetLayer*>::iterator ilm = layer_map.begin();
      int ngoodlayers = 0;
      while (ilm != layer_map.end()) {
	//HERE is another change by A. A, this code had been changed to ONLY use layers with one hit.
	//for conversions I have reverted this.
	if (ngoodlayers >= nfound_min && ilm->first > 1) break;
	//    if (ilm->first > 1) break;
	//	map<const DetLayer*, int>::iterator ilr = layer_reference.find(ilm->second);
	//std::cout<<"Layer " << ilr->second << " with " << ilm->first << " hits added " << std::endl;
	++ngoodlayers;
	++ilm;
      }
      set<const DetLayer*> good_layers;
      if (debug_) std::cout << " With useful layers: ";
      for (multimap<int, const DetLayer*>::iterator ilm1 = layer_map.begin();
	   ilm1 != ilm; ++ilm1) {
	good_layers.insert(ilm1->second);
	map<const DetLayer*, int>::iterator ilr = layer_reference.find(ilm1->second);
	if (ilr != layer_reference.end() && debug_) std::cout << " " << ilr->second;
      }
      if (debug_) cout << std::endl;
      
      // choose intermediate layers
      const int max_middle_layers = 2;
      const DetLayer* middle_layers[max_middle_layers] = {0};
      int n_middle_layers = 0;
      
      //This is not really fair.  This is another place where for conversions a change
      //might need to be made.  We've got fewer-than few-hit tracking for tracks
      //not from the origin.  Can we we allow these to be non-stereo?  Ask Kevin/Steve?
      for (int ilayer = ilayer0+1; ilayer<nlayers; ++ilayer) {
	// only use useful layers
	if (good_layers.find(layers[ilayer]) == good_layers.end()) continue;
	// only use stereo layers
	if (!lstereo[ilayer]) continue;
	middle_layers[n_middle_layers] = layers[ilayer];
	if (++n_middle_layers >= max_middle_layers) break;
      }
      
      edm::OwnVector<TrackingRecHit> recHits_middle;
      for (int ml = 0; ml < n_middle_layers; ++ml) {
	for (edm::OwnVector<TrackingRecHit>::const_iterator ih = recHits.begin();
	     ih != recHits.end(); ++ih) {
	  if (theMeasurementTracker->geometricSearchTracker()->detLayer(ih->geographicalId()) == middle_layers[ml]) {
	    recHits_middle.push_back(ih->clone());
	  }
	}
      }
 
      edm::OwnVector<TrackingRecHit>& recHits_inner = recHits_start;
      edm::OwnVector<TrackingRecHit>& recHits_outer = recHits_middle;
      
      // collect hits in useful layers
      vector<pair<const DetLayer*, edm::OwnVector<TrackingRecHit> > > goodHits;
      for(edm::OwnVector<TrackingRecHit>::const_iterator ihit = recHits.begin();
	  ihit != recHits.end(); ihit++) {
	// only use useful layers
        const DetLayer* thisLayer =
	  theMeasurementTracker->geometricSearchTracker()->detLayer(ihit->geographicalId());
	if (thisLayer == layers[ilayer0] ||
	    (good_layers.find(thisLayer) != good_layers.end() &&
	     prev_layers.find(thisLayer) == prev_layers.end())) {
	  if (!goodHits.empty() && goodHits.back().first == thisLayer) {
	    goodHits.back().second.push_back(ihit->clone());
	  } else {
	    edm::OwnVector<TrackingRecHit> rhc;
	    rhc.push_back(ihit->clone());
	    goodHits.push_back(make_pair(thisLayer, rhc));
	  }
	}
      }
 
      
      // try various hit combinations
      //Reminder that outer hits actually come from the list of middle layers.
      for (edm::OwnVector<TrackingRecHit>::const_iterator innerHit = recHits_inner.begin();
	   innerHit != recHits_inner.end(); ++innerHit) {
	for (edm::OwnVector<TrackingRecHit>::iterator outerHit = recHits_outer.begin();
	     outerHit != recHits_outer.end(); ++outerHit) {
	  
	  GlobalPoint inner = geom->idToDet(innerHit->geographicalId())->surface().toGlobal(innerHit->localPosition());
	  GlobalPoint outer = geom->idToDet(outerHit->geographicalId())->surface().toGlobal(outerHit->localPosition());
	  
	  const DetLayer* innerHitLayer =
	    theMeasurementTracker->geometricSearchTracker()->detLayer(innerHit->geographicalId());
	  const DetLayer* outerHitLayer =
	    theMeasurementTracker->geometricSearchTracker()->detLayer(outerHit->geographicalId());
	  
	  if (debug_) std::cout << "Try trajectory with Inner Hit on Layer " << ilayer0 << " and  " ;
	  map<const DetLayer*, int>::iterator ilro = layer_reference.find(outerHitLayer);
	  if (ilro != layer_reference.end() && debug_) std::cout << "Outer Hit on Layer " << ilro->second ;
	  if (debug_){ std::cout << std::endl;
	    cout << "inner hit: r/phi/z = "<< inner.perp() << " " << inner.phi() << " " << inner.z() ;
	    cout << "outer hit: r/phi/z = "<< outer.perp() << " " << outer.phi() << " " << outer.z() ;
	  }
	  // hits should be reasonably separated in r
	  const double dRmin = 0.1; // cm
	  if (outer.perp() - inner.perp() < dRmin) continue;

	  //This is known to possibly be an issue with the conversion finding...
	  GlobalPoint vertexPos(0,0,0);
	  const double dr2 = 0.0015*0.0015;
	  const double dz2 = 5.3*5.3;
	  GlobalError vertexErr(dr2,
				0, dr2,
				0, 0, dz2);
	  //TrivialVertex vtx( vertexPos, vertexErr);
	  //FastHelix helix(outerHit.globalPosition(),
          //              innerHit.globalPosition(),
	  //              vtx.position());
	  FastHelix helix(outer, inner, vertexPos, es);
	  if (!helix.isValid()) continue;
          AlgebraicSymMatrix C(5,1);
	  float zErr = vertexErr.czz();
	  float transverseErr = vertexErr.cxx(); // assume equal cxx cyy
	  C[3][3] = transverseErr;
	  C[4][4] = zErr;
	  CurvilinearTrajectoryError initialError(C);
	  FreeTrajectoryState fts( helix.stateAtVertex().parameters(), initialError);
	  //                       RoadSearchSeedFinderAlgorithm::initialError( *outerHit, *innerHit,
	  //                                  vertexPos, vertexErr));
	  if (debug_){
	  std::cout << "Free Trajectory State Parameters: " << std::endl;
	  std::cout << "TransverseMomentum: " << fts.momentum().perp() << std::endl;
	  std::cout << "R position: " << sqrt(pow(fts.position().x(),2) + pow(fts.position().y(),2)) << std::endl;
	  std::cout << "This look okay?" << std::endl;
	  }
	  vector<Trajectory> rawTrajectories;
	  
	  Trajectory seedTraj(*(new TrajectorySeed()), alongMomentum);
	  
	  // Need to put the first hit on the trajectory
	  const TrajectoryStateOnSurface innerState = 
	    thePropagator->propagate(fts,geom->idToDet(innerHit->geographicalId())->surface());
	  if ( !innerState.isValid()) {
	    if (debug_) std::cout<<"*******DISASTER ********* seed doesn't make it to first hit!!!!!" << std::endl;
	    continue;
	  }
	  TransientTrackingRecHit::RecHitPointer intrhit = ttrhBuilder->build(&(*innerHit));
	  MeasurementEstimator::HitReturnType est = theEstimator->estimate(innerState, *intrhit);
	  if (!est.first) continue;         
	  TrajectoryStateOnSurface innerUpdated= theUpdator->update( innerState,*intrhit);                         
	  TrajectoryMeasurement tm = TrajectoryMeasurement(innerState, innerUpdated, &(*intrhit),est.second,innerHitLayer);
	  seedTraj.push(tm,est.second);
	  
	  rawTrajectories.push_back(seedTraj);
	  // now loop on hits
	  for (vector<pair<const DetLayer*, edm::OwnVector<TrackingRecHit> > >::iterator ilhv = goodHits.begin();
	       ilhv != goodHits.end(); ++ilhv) {
	    edm::OwnVector<TrackingRecHit>& hits = ilhv->second;
	    vector<Trajectory> newTrajectories;
	    
	    if (debug_){
	      map<const DetLayer*, int>::iterator ilr = cloud_layer_reference.find(ilhv->first);
	      if (ilr != cloud_layer_reference.end())
		std::cout << "extrapolating " << rawTrajectories.size() 
			  << " trajectories to layer " << ilr->second 
			  << " which has  " << hits.size() << " hits " << std::endl;
	    }
	    
	    for (vector<Trajectory>::const_iterator itr = rawTrajectories.begin();
		 itr != rawTrajectories.end(); ++itr) {
	      Trajectory traj = *itr;
	      for(edm::OwnVector<TrackingRecHit>::const_iterator ihit = hits.begin();
		  ihit != hits.end(); ihit++) {
		const DetLayer* thisLayer =
		  theMeasurementTracker->geometricSearchTracker()->detLayer(ihit->geographicalId());
		if (thisLayer == innerHitLayer){
		  // Right now we are assuming that ONLY single hit layers are in this initial collection
		  //if (thisLayer == innerHitLayer && !(ihit->recHit() == innerHit->recHit())){
		  //  if (debug_) std::cout<<"On inner hit layer, but have wrong hit?!?!?" << std::endl;
		  continue;
		}
		//if (thisLayer == outerHitLayer && !(ihit == outerHit)){
		//  continue;
		//      }
		// extrapolate
		vector<Trajectory> newResult;
		
		TransientTrackingRecHit::RecHitPointer rhit = ttrhBuilder->build(&(*ihit));		
		if (debug_){
		  if (rhit->isValid()) {
		    cout << "RecHit " << ihit-hits.begin()
			 << ", det " << rhit->det() << ", r/phi/z = "
			 << rhit->globalPosition().perp() << " "
			 << rhit->globalPosition().phi() << " "
			 << rhit->globalPosition().z();
		  } else {
		    LogDebug("RoadSearch") << "RecHit " << ihit-hits.begin()
					   << " (invalid)";
		  }
		}
 
		const GeomDet* det = geom->idToDet(rhit->geographicalId());
		
		TrajectoryStateOnSurface predTsos;
		TrajectoryStateOnSurface currTsos;
		
		if (traj.measurements().empty()) {
		  predTsos = thePropagator->propagate(fts, det->surface());
		} else {
		  currTsos = traj.measurements().back().updatedState();
		  predTsos = thePropagator->propagate(currTsos, det->surface());
		}
		if (!predTsos.isValid()){
		  continue;
		}
		TrajectoryMeasurement tm;
		if (debug_){
		  std::cout<< "trajectory " << itr-rawTrajectories.begin() 
			   << " at r/z=" <<  det->surface().position().perp() 
			   << "  " <<  det->surface().position().z() 
			   << ", hit " << ihit-hits.begin()
			   << "local prediction " << predTsos.localPosition().x() 
			   << " +- " << sqrt(predTsos.localError().positionError().xx()) 
			   << ", hit at " << rhit->localPosition().x() << " +- " << sqrt(rhit->localPositionError().xx())
			   << std::endl;
		}
 
                 // update
		MeasurementEstimator::HitReturnType est = theEstimator->estimate(predTsos, *rhit);
		if (debug_) std::cout << "estimation: " << est.first << " " << est.second << std::endl;
		if (!est.first) continue;
		currTsos = theUpdator->update(predTsos, *rhit);
		tm = TrajectoryMeasurement(predTsos, currTsos, &(*rhit),est.second,thisLayer);
		traj.push(tm,est.second);
		newTrajectories.push_back(traj);
	      }//Loop over hits on this layer
	    }//loop over trajectories
	    
            if (newTrajectories.empty()) {
              if (debug_) std::cout<<" Could not add the hit in this layer " << std::endl;
	      // layer missed
              continue;
	    }
	    rawTrajectories = newTrajectories;
	  }//Loop over layer, hit vector pairs, goodHits (ihlv)
	  if (rawTrajectories.size()==0) if (debug_) std::cout<<" --> yields ZERO raw trajectories!" << std::endl;
	  for (vector<Trajectory>::const_iterator it = rawTrajectories.begin();
	       it != rawTrajectories.end(); it++) {
	    if (debug_) std::cout << " --> yields trajectory has "<<it->recHits().size()<<" hits with chi2="
				  <<it->chiSquared()<<" and is valid? "<<it->isValid() <<std::endl;
	  }//iterate over rawtrajectories
	  vector<Trajectory> rawCleaned;
	  theTrajectoryCleaner.clean(rawTrajectories);
	  for (vector<Trajectory>::const_iterator itr = rawTrajectories.begin();
	       itr != rawTrajectories.end(); ++itr) {
	    // see how many layers have been found
	    if (!itr->isValid()) continue;
	    set<const DetLayer*> used_layers;
	    Trajectory::DataContainer tmv = itr->measurements();
	    for (Trajectory::DataContainer::iterator itm = tmv.begin();
		 itm != tmv.end(); ++itm) {
	      TransientTrackingRecHit::ConstRecHitPointer rh = itm->recHit();
	      if (!rh->isValid()) continue;
	      used_layers.insert(theMeasurementTracker->geometricSearchTracker()->detLayer(rh->geographicalId()));
	    }
	    /*
	      if (debug_) {
	      cout<<"Raw trajectory " << itr-rawTrajectories.begin() << " has " << used_layers.size() 
	      << " used layers:  ";
	      for (set<const DetLayer*>::iterator iul = used_layers.begin();
	      iul != used_layers.end(); ++iul) {
	      map<const DetLayer*, int>::iterator ilr = layer_reference.find(*iul);
	      if (ilr != layer_reference.end() && debug_) cout << " " << ilr->second;
	      }
	      cout<<endl;
	      }
	    */
	    if ((int)used_layers.size() < nfound_min) continue;
	    int nlostlayers = ngoodlayers - used_layers.size();
	    const int nlost_max = 2;
	    //May need to revisit this at some point.  then again, given
	    //the probable number of available layers, we're probably on the razor's edge
	    //anyway.
	    if (nlostlayers > nlost_max) continue;	    
	    rawCleaned.push_back(*itr);
	  }
	  if (!rawCleaned.empty()) {
	    ChunkTrajectories.insert(ChunkTrajectories.end(), rawCleaned.begin(), rawCleaned.end());
	  }
	}//Loop over outer_hits, which is really the middle hits.
      }//Loop over the inner hits.
       // At this point we have made all the trajectories from the low occupancy layers
       // We clean these trajectories first, and then try to add hits from the skipped layers
      
       //    }
      if (debug_) 
	std::cout << "Clean the " << ChunkTrajectories.size()<<" trajectories for this chunk" << std::endl;
      // clean the intermediate result
      theTrajectoryCleaner.clean(ChunkTrajectories);
      for (vector<Trajectory>::const_iterator it = ChunkTrajectories.begin();
	   it != ChunkTrajectories.end(); it++) {
	if (it->isValid()) {
	  CleanChunks.push_back(*it);
	  /*
	    std::cout << "ChunkTrajectory has "<<it->recHits().size()<<" hits with chi2="
	    <<it->chiSquared()<<" and is valid? "<<it->isValid() << std::endl;
	    std::cout << "Dump trajectory measurements" << endl;
	    Trajectory::DataContainer tms = it->measurements();
	    for (Trajectory::DataContainer::iterator i=tms.begin();i!=tms.end();++i){
	    std::cout<< "TSOS for measurement " <<i-tms.begin()
	    <<" with estimate " << i->estimate();
	    std::cout <<"  at position "
	    <<(*i).recHit()->det()->surface().toGlobal((*i).recHit()->localPosition())<<std::endl;
	    }
	  */
	  
	}
      }
      if (debug_) std::cout <<"After cleaning there are " << CleanChunks.size() 
			    << " trajectories for this chunk" << std::endl;
       
      //
      // Step 2: recover measurements from busy layers
      //
 
      vector<Trajectory> extendedChunks;       
      // see if there are layers that we skipped
      set<const DetLayer*> skipped_layers;
      map<int, const DetLayer*> skipped_layer_detmap;
      
      //Remember recHits (hell, it's only 400 lines up)?  This is the original stub hits.  To extend the chunk, look through the
      //entire original set of hits, and try to fill in measurements..
      for (edm::OwnVector<TrackingRecHit>::const_iterator ih = recHits.begin();
	   ih != recHits.end(); ++ih) {
	const DetLayer* thisLayer =
	  theMeasurementTracker->geometricSearchTracker()->detLayer(ih->geographicalId());
	if (thisLayer != layers[ilayer0] &&
	    good_layers.find(thisLayer) == good_layers.end() &&
	    prev_layers.find(thisLayer) == prev_layers.end()) {
	  skipped_layers.insert(thisLayer);
	  map<const DetLayer*, int>::iterator ilr = layer_reference.find(thisLayer);
	  if (ilr != layer_reference.end())
	    skipped_layer_detmap.insert(make_pair(ilr->second,thisLayer));
	  else
	    if (debug_) std::cout<<"Couldn't find thisLayer to insert into map..."<<std::endl;
	}
      }//Loop over the original hits.
      
      if (debug_){
	if (skipped_layers.empty()) {
	  std::cout << "all layers have been used" << std::endl;
	} 
	else {
	  //std::cout<< std::endl<< " SKIPPED_LAYER_DETMAP dump: size is " << skipped_layer_detmap.size()<<std::endl;
	  std::cout << "There are " << skipped_layer_detmap.size() << " skipped layers:";
	  for (map<int, const DetLayer*>::const_iterator imap = skipped_layer_detmap.begin();
	       imap!=skipped_layer_detmap.end(); imap++){
	    //std::cout<< "Layer " <<imap->first <<" and DetLayer* " << imap->second << std::endl;
	    std::cout<< " " <<imap->first;
	  }
	  std::cout << std::endl;
	}
      }//if debug
      
      //Loop over your previously cleaned 'chunks'
      for (vector<Trajectory>::const_iterator i = CleanChunks.begin();
	   i != CleanChunks.end(); i++) {
	//I'm not sure how you got here if you're not valid.
	if (!(*i).isValid()) continue;

	if (debug_) std::cout<< "Now process CleanChunk trajectory " << i-CleanChunks.begin() << std::endl;
	//if there's nothing to add, and you're a valid trajectory, you go to the next step.
	if (skipped_layers.empty() && int(i->measurements().size()) >= theNumHitCut) {
	  extendedChunks.insert(extendedChunks.end(), *i);
	} 
	//otherwise, if I CAN add hits, I should at least try.
	else {	  
	  Trajectory temptraj = *i;
	  Trajectory::DataContainer tmv = (*i).measurements();

	  //ingenious.  If you can't possibly get to the minimum size, then don't waste time.
	  if (int(tmv.size()+skipped_layer_detmap.size()) < theNumHitCut) continue;

	  map<const DetLayer*, int> used_layer_detmap;

	  for (Trajectory::DataContainer::const_iterator ih=tmv.begin();
	       ih!=tmv.end();++ih){
	    const DetLayer* Layer =
	      theMeasurementTracker->geometricSearchTracker()->detLayer(ih->recHit()->geographicalId());      
	    map<const DetLayer*, int>::iterator ilr = cloud_layer_reference.find(Layer);
	    if (ilr != cloud_layer_reference.end()){
	      used_layer_detmap.insert(make_pair(Layer,ilr->second));
	      if (debug_) std::cout << "Add DetLayer " << Layer << " to used_layer_detmap for layer "
				    << ilr->second << std::endl;
	    }
	    else
	      if (debug_) std::cout<<"Couldn't find thisLayer to insert into map..."<<std::endl;
	  }
	   
	  for(set<const DetLayer*>::const_iterator lyiter = good_layers.begin();
	      lyiter!= good_layers.end();++lyiter){
	    const DetLayer* thisLayer = *lyiter;
	    map<const DetLayer*, int>::iterator ilr = used_layer_detmap.find(thisLayer);
	    if (ilr == used_layer_detmap.end()){
	      //Add this layer to the skipped layers
	      map<const DetLayer*, int>::iterator il = cloud_layer_reference.find(thisLayer);
	      skipped_layer_detmap.insert(make_pair(il->second,thisLayer));
	      if (debug_) {
		std::cout << "Can't find a hit on layer Hit #"<< il->second << std::endl;
	      }
	    }
	  }
	  
	  for (Trajectory::DataContainer::const_iterator ih=tmv.begin();
	       ih!=tmv.end();++ih){
	    TransientTrackingRecHit::ConstRecHitPointer rh = ih->recHit();
	    const DetLayer* Layer =
	      theMeasurementTracker->geometricSearchTracker()->detLayer(rh->geographicalId());      
	    map<const DetLayer*, int>::iterator ilr = cloud_layer_reference.find(Layer);
	    if (ilr != cloud_layer_reference.end())
	      if (debug_) std::cout << "   Hit #"<<ih-tmv.begin() << " of " << tmv.size()
				    <<" is on Layer " 
				    << ilr->second << std::endl;
	      else 
		if (debug_) std::cout << "   Layer for Hit #"<<ih-tmv.begin() <<" can't be found " << std::endl;
	  }
 
	  // Loop over the layers in the cloud
          
	  Trajectory::DataContainer::const_iterator im = tmv.begin();
	  map<int, const DetLayer*>::const_iterator imap = skipped_layer_detmap.begin();
	  
	  TrajectoryMeasurement firstMeasurement = i->firstMeasurement();
	  const DetLayer* firstDetLayer = 
	    theMeasurementTracker->geometricSearchTracker()->detLayer(firstMeasurement.recHit()->geographicalId());
	  
	  std::vector<Trajectory> freshStart = theSmoother.trajectories(*i);
	  if (!freshStart.empty()){
	    if (debug_) std::cout<< "Smoothing of trajectory " <<i-CleanChunks.begin() << " has succeeded with " 
				 <<freshStart.begin()->measurements().size() << " hits.  Now add hits." <<std::endl;
	  }
	  else {
	    if (debug_) std::cout<< "Smoothing of trajectory " <<i-CleanChunks.begin() <<" has failed"<<std::endl;
	    continue;
	  }
	  TrajectoryStateOnSurface NewFirstTsos = freshStart.begin()->lastMeasurement().updatedState();
	  TransientTrackingRecHit::ConstRecHitPointer rh = freshStart.begin()->lastMeasurement().recHit();
	  PTrajectoryStateOnDet* pFirstState = TrajectoryStateTransform().persistentState(NewFirstTsos,
											  rh->geographicalId().rawId());
	  edm::OwnVector<TrackingRecHit> newHits;
	  newHits.push_back(rh->hit()->clone());
	  TrajectorySeed tmpseed = TrajectorySeed(*pFirstState, 
						  newHits,
						  alongMomentum);
	  
	  Trajectory newTrajectory(tmpseed,tmpseed.direction());
	  
	  const GeomDet* det = geom->idToDet(rh->geographicalId());
	  TrajectoryStateOnSurface invalidState(new BasicSingleTrajectoryState(det->surface()));
	  newTrajectory.push(TrajectoryMeasurement(invalidState, NewFirstTsos, rh, 0, firstDetLayer));
	  
	  map<const DetLayer*, int>::iterator ilr = cloud_layer_reference.find(firstDetLayer);
	  int firstlyr = 0;
	  if (ilr != cloud_layer_reference.end() ){
	    if (debug_) std::cout << "   First hit is on layer  " << ilr->second << std::endl;
	    firstlyr = ilr->second;
	    ++im;
	  }
	  
	  for (int ilayer = firstlyr+1; ilayer < nlayers; ++ilayer) {
	    if (debug_) std::cout<<"   Layer " << ilayer ;
	    
	    TrajectoryStateOnSurface predTsos;
	    TrajectoryStateOnSurface currTsos;
	    TrajectoryMeasurement tm;
	    
	    if (layers[ilayer] == imap->second) {
	      if (debug_) std::cout<<" is one of the skipped layers " << std::endl;
	      
	      //collect hits in the skipped layer
	      edm::OwnVector<TrackingRecHit> skipped_hits;
	      set<const GeomDet*> dets;
	      for (edm::OwnVector<TrackingRecHit>::const_iterator ih = recHits.begin();
		   ih != recHits.end(); ++ih) {
		if (theMeasurementTracker->geometricSearchTracker()->detLayer(ih->geographicalId())
		    // == *isl) {
		    == imap->second) {
		  skipped_hits.push_back(ih->clone());
		  dets.insert(geom->idToDet(ih->geographicalId()));
		}
	      }
	      
	      map<const DetLayer*, int>::iterator ilr = layer_reference.find(imap->second);
	      if (ilr != layer_reference.end()) 
		if (debug_)
		  std::cout<<"   ---> probing missing hits (nh="<< skipped_hits.size() << ", nd=" << dets.size() 
			   << ")  in layer " << ilr->second <<std::endl;
	      
	      const TrajectoryStateOnSurface theTSOS = newTrajectory.lastMeasurement().updatedState();
	      std::vector<TrajectoryMeasurement> theGoodHits = FindBestHit(theTSOS,dets,skipped_hits, geom, ttrhBuilder,theMeasurementTracker, thePropagator);
	      if (!theGoodHits.empty())
		newTrajectory.push(theGoodHits.front(),theGoodHits.front().estimate());
	      ++imap;
	    }
	    else {
	      TransientTrackingRecHit::ConstRecHitPointer rh = im->recHit();
	      if (rh->isValid() && 
		  theMeasurementTracker->geometricSearchTracker()->detLayer(rh->geographicalId()) == layers[ilayer]) {
		if (debug_) std::cout<<" has a good hit " << std::endl;
		++im;
		
		const GeomDet* det = geom->idToDet(rh->geographicalId());
		
		if (newTrajectory.measurements().empty())
		  if (debug_) std::cout<<"How the heck does this have no measurements!!!" <<std::endl;
		
		
		currTsos = newTrajectory.measurements().back().updatedState();
		predTsos = thePropagator->propagate(currTsos, det->surface());
		if (!predTsos.isValid()) continue;
		MeasurementEstimator::HitReturnType est = theEstimator->estimate(predTsos, *rh);
		if (!est.first) {
		  if (debug_) std::cout<<"Failed to add one of the original hits on a low occupancy layer!!!!" << std::endl;
		  continue;
		}
		currTsos = theUpdator->update(predTsos, *rh);
		tm = TrajectoryMeasurement(predTsos, currTsos, &(*rh),est.second,layers[ilayer]);
		newTrajectory.push(tm,est.second);		
	      }
	      else {
		if (debug_) std::cout<<" has no hit" << std::endl;
	      }
	    }       
	  }//ilayer loop!  At last!
	  
	  if (debug_) std::cout<<"Finished loop over layers in cloud.  Trajectory now has " <<newTrajectory.measurements().size()
			       << " hits. " << std::endl;
	  // DO WE NEED TO SMOOTH THIS TRAJECTORY?
	  //newSmoothed = theSmoother.trajectories(newTrajectory);
          //if (newSmoothed.empty()){
	  //  std::cout<< " Smoothing of new trajectory has failed. " <<std::endl;
	  // }
	  //std::cout<< " Smoothing has succeeded. " <<std::endl;
	  if (int(newTrajectory.measurements().size()) >= theNumHitCut)
	    extendedChunks.insert(extendedChunks.end(), newTrajectory);
	}
      }
      if (debug_) std::cout<< "Extended chunks: " << extendedChunks.size() << endl;
      //if (!extendedChunks.empty()) {
      //  smoothedResult.insert(smoothedResult.end(), extendedChunks.begin(), extendedChunks.end());
      //  break;
      //}
      if (debug_) std::cout<< "Now Clean the extended chunks " <<std::endl;
      theTrajectoryCleaner.clean(extendedChunks);
      for (vector<Trajectory>::const_iterator it = extendedChunks.begin();
	   it != extendedChunks.end(); it++) {
	if (it->isValid()) CloudTrajectories.push_back(*it);
      }
    }
    
    // ********************* END NEW ADDITION
    
    if (debug_) std::cout<< "Finished with the cloud, so clean the " 
			 << CloudTrajectories.size() << " cloud trajectories "<<std::endl ;
    theTrajectoryCleaner.clean(CloudTrajectories);
    for (vector<Trajectory>::const_iterator it = CloudTrajectories.begin();
	 it != CloudTrajectories.end(); it++) {
      if (it->isValid()) FinalTrajectories.push_back(*it);
    }
    
    if (debug_) std::cout<<" Final trajectories now has size " << FinalTrajectories.size()<<std::endl ;
    
  } // End loop over Cloud Collection
  
  if (debug_) std::cout<< " Finished loop over all clouds " <<std::endl;
  theTrajectoryCleaner.clean(FinalTrajectories);
  for (vector<Trajectory>::const_iterator it = FinalTrajectories.begin();
       it != FinalTrajectories.end(); it++) {
    if (debug_) std::cout<< " Trajectory has "<<it->recHits().size()<<" hits with chi2="
			 <<it->chiSquared()<<" and is valid? "<<it->isValid()<<std::endl;
    if (it->isValid()){
      
      edm::OwnVector<TrackingRecHit> goodHits;
      //edm::OwnVector<const TransientTrackingRecHit> ttHits = it->recHits(); 
      //for (edm::OwnVector<const TransientTrackingRecHit>::const_iterator rhit=ttHits.begin(); 
      TransientTrackingRecHit::ConstRecHitContainer ttHits = it->recHits();           
      for (TransientTrackingRecHit::ConstRecHitContainer::const_iterator rhit=ttHits.begin(); 
	   rhit!=ttHits.end(); ++rhit){
	goodHits.push_back((*rhit)->hit()->clone());
      }
      
      if (debug_) std::cout<<" Trajectory has " << goodHits.size() << " good hits "<<std::endl;
      // clone 
      //TrajectorySeed seed = *((*ref).clone());
      PTrajectoryStateOnDet state = it->seed().startingState();
      TrajectoryStateOnSurface firstState;
      
      // check if Trajectory from seed is on first hit of the cloud, if not, propagate
      // exclude if first state on first hit is not valid
      
      DetId FirstHitId = (*(it->recHits().begin()))->geographicalId();
      if (debug_) std::cout<< " FirstHitId is null ? "<< FirstHitId.null()<<std::endl;
      
      // propagate back to first hit
      
      TrajectoryMeasurement firstMeasurement = it->measurements()[0];
      ////if (it->recHits().begin()->geographicalId().rawId() != state.detId()) {
      const GeomDet* det = geom->idToDet(FirstHitId);
      firstState = prop.propagate(firstMeasurement.updatedState(), det->surface());
      
      if (firstState.isValid() == false) continue;
      
      state = *(transformer.persistentState(firstState,FirstHitId.rawId()));
      
      //std::cout<<"This track candidate has " << goodHits.size() << " hits "<<std::endl ;
      TrackCandidate canner = TrackCandidate(goodHits,it->seed(),state);
      output.push_back(TrackCandidate(goodHits,it->seed(),state));
      //Try to make actual tracks:
      //variable declarations
      
      std::vector<Trajectory> trajVec;
      //reco::Track * theTrack;
      Trajectory * theTraj; 
      KFTrajectoryFitter *theFitter = new KFTrajectoryFitter(thePropagator, theUpdator, theEstimator);
      //perform the fit: the result's size is 1 if it succeded, 0 if fails
      //This piece of code is MAGICALLY HORRIFIC
      TransientTrackingRecHit::RecHitContainer hits;
      const TrackCandidate::range& recHitVec=canner.recHits();
      float ndof=0;       
      for (edm::OwnVector<TrackingRecHit>::const_iterator i=recHitVec.first;
	   i!=recHitVec.second; i++){
	hits.push_back(ttrhBuilder->build(&(*i) ));
	if ((*i).isValid()){
	  ndof = ndof + (i->dimension())*(i->weight());
	}
      }
      //
      
      //    TrajectoryStateOnSurface trajSOS(detstate.parameters(), (geom->idToDet(FirstHitId))->surface(), magField);

      trajVec = theFitter->fit(canner.seed(), hits, firstState);
    
      TrajectoryStateOnSurface innertsos;
      if (trajVec.size() != 0){
      
	theTraj = new Trajectory( trajVec.front() );
	
	if (theTraj->direction() == alongMomentum) {
	  innertsos = theTraj->firstMeasurement().updatedState();
	} else { 
	  innertsos = theTraj->lastMeasurement().updatedState();
	}
	
      
	TSCPBuilderNoMaterial tscpBuilder;
	//I'm a bit worried about this one...      
	TrajectoryStateClosestToPoint tscp = tscpBuilder(*(innertsos.freeState()),
							 GlobalPoint(0,0,0) );//FIXME Correct?
	
	PerigeeTrajectoryParameters::ParameterVector param = tscp.perigeeParameters();
	
	PerigeeTrajectoryError::CovarianceMatrix covar = tscp.perigeeError();
	
	reco::Track theTrack(theTraj->chiSquared(),
				   int(ndof),//FIXME fix weight() in TrackingRecHit
				   param,tscp.pt(),
				   covar);
	//sets the outermost and innermost TSOSs
	TrajectoryStateOnSurface outertsos;
	unsigned int innerId, outerId;
	if (theTraj->direction() == alongMomentum) {
	  outertsos = theTraj->lastMeasurement().updatedState();
	  outerId = theTraj->lastMeasurement().recHit()->geographicalId().rawId();
	  innerId = theTraj->firstMeasurement().recHit()->geographicalId().rawId();
	} else { 
	  outertsos = theTraj->firstMeasurement().updatedState();
	  outerId = theTraj->firstMeasurement().recHit()->geographicalId().rawId();
	  innerId = theTraj->lastMeasurement().recHit()->geographicalId().rawId();
	}
	//build the TrackExtra
	GlobalPoint v = outertsos.globalParameters().position();
	GlobalVector p = outertsos.globalParameters().momentum();
	math::XYZVector outmom( p.x(), p.y(), p.z() );
	math::XYZPoint  outpos( v.x(), v.y(), v.z() );
	v = innertsos.globalParameters().position();
	p = innertsos.globalParameters().momentum();
	math::XYZVector inmom( p.x(), p.y(), p.z() );
	math::XYZPoint  inpos( v.x(), v.y(), v.z() );

	reco::TrackExtra extr(outpos, outmom, true, inpos, inmom, true,
			      outertsos.curvilinearError(), outerId,
			      innertsos.curvilinearError(), innerId);
// 	for (int ju =0;ju<int(recHitVec.size());ju++)
// 	  extr.add((*recHitVec[ju]).clone());


	//	theTrack->setExtra( teref );
	trkCan.push_back(theTrack);
	trkColl.push_back(extr);
      //edm::OwnVector<const TransientTrackingRecHit> ttHits = it->recHits(); 
      //for (edm::OwnVector<const TransientTrackingRecHit>::const_iterator rhit=ttHits.begin(); 
	TransientTrackingRecHit::ConstRecHitContainer tttHits = it->recHits();           
	for (TransientTrackingRecHit::ConstRecHitContainer::const_iterator rhit=tttHits.begin(); 
	     rhit!=tttHits.end(); ++rhit){
	  returnhits.push_back((*rhit)->hit()->clone());
	}
// 	for (int i=0;i<goodHits.size();++i){
// 	  //	  TrackingRecHit hitter = goodHits[i];
// 	  returnhits.push_back(goodHits[i]);
// 	}
      
      }
      delete theFitter;
    }
  }
  
  if (debug_) std::cout<< "Found " << output.size() << " track candidates."<<std::endl;  
  if (debug_) std::cout<< "Found " << trkCan.size() << " tracks."<<std::endl;  
  delete thePropagator;
  delete theRevPropagator;

  //  return trackvec;
}

std::vector<TrajectoryMeasurement> Pi0ConversionAlgo::FindBestHit(const TrajectoryStateOnSurface& tsosBefore,
								  const std::set<const GeomDet*>& theDets,
								  edm::OwnVector<TrackingRecHit>& theHits,
								  const TrackerGeometry *geom,
								  const TransientTrackingRecHitBuilder *ttrhBuilder,
								  const MeasurementTracker *theMeasurementTracker,
								  const PropagatorWithMaterial *thePropagator)
{
  
  std::vector<TrajectoryMeasurement> theBestHits;
  
  bool found_one = false;
  double bestchi = 10000.0;
  // extrapolate to all detectors from the list
  map<const GeomDet*, TrajectoryStateOnSurface> dmmap;
  for (set<const GeomDet*>::iterator idet = theDets.begin();
       idet != theDets.end(); ++idet) {
    TrajectoryStateOnSurface predTsos = thePropagator->propagate(tsosBefore, (**idet).surface());
    if (predTsos.isValid()) {
      dmmap.insert(make_pair(*idet, predTsos));
    }
  }
  // evaluate hit residuals
  map<const GeomDet*, TrajectoryMeasurement> dtmmap;
  for (edm::OwnVector<TrackingRecHit>::const_iterator ih = theHits.begin();
       ih != theHits.end(); ++ih) {
    const GeomDet* det = geom->idToDet(ih->geographicalId());
    //if (*isl != theMeasurementTracker->geometricSearchTracker()->detLayer(ih->geographicalId())) 
    //  cout <<" You don't know what you're doing !!!!" << endl;
    
    map<const GeomDet*, TrajectoryStateOnSurface>::iterator idm = dmmap.find(det);
    if (idm == dmmap.end()) continue;
    TrajectoryStateOnSurface predTsos = idm->second;
    TransientTrackingRecHit::RecHitPointer rhit = ttrhBuilder->build(&(*ih));
    MeasurementEstimator::HitReturnType est = theEstimator->estimate(predTsos, *rhit);
    //std::cout<< "hit " << ih-theHits.begin() 
    //     << ": est = " << est.first << " " << est.second  <<std::endl;
    
    // Take the best hit on any Det
    if (est.first) {
      TrajectoryStateOnSurface currTsos = theUpdator->update(predTsos, *rhit);
      found_one = true;
      if (est.second < bestchi){
	if(!theBestHits.empty()){
	  theBestHits.erase(theBestHits.begin());
	}
	bestchi = est.second;
	TrajectoryMeasurement tm(predTsos, currTsos, &(*rhit),est.second,
				 theMeasurementTracker->geometricSearchTracker()->detLayer(ih->geographicalId()));      
	theBestHits.push_back(tm);
      }
    }
  }
  
  if (theBestHits.empty()){
    if (debug_) std::cout<< "no hits to add! " <<std::endl;
  }
  else{
    for (std::vector<TrajectoryMeasurement>::const_iterator im=theBestHits.begin();im!=theBestHits.end();++im)
      if (debug_) std::cout<<" Measurement on layer "
			   << theMeasurementTracker->geometricSearchTracker()->detLayer(im->recHit()->geographicalId())
                           << " with estimate " << im->estimate()<<std::endl ;
  }
  //std::cout<<" Measurement returned with estimate "<< theBestHit.estimate() << std::endl ;
  
  return theBestHits;
}


CachingVertex Pi0ConversionAlgo::FitVertex(std::vector<reco::TransientTrack> tracks){
  KalmanVertexFitter fit_vtx(false);
  //AdaptiveVertexFitter fit_vtx;
  CachingVertex vt = fit_vtx.vertex(tracks);
  return vt;

}
