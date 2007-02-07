#include <vector>
#include <Pi0ConversionAlgo.h>
#include <StubCandidate.h>
using namespace std::vector;
using namespace std::cout;

Pi0ConversionAlgo::Pi0ConversionAlgo(){



}

Pi0ConversionAlgo::~Pi0ConversionAlgo(){
}


Pi0ConversionAlgo::GetConversionR(float phi1, float pt1, float phi2, float pt2)
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

Pi0ConversionAlgo::GetConversionPhi(float phi1, float pt1, float phi2, float pt2)
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
  
  //In all of the expressions actually WRITTEN DOWN: Phi1 == Larger phi.  R1 == Radius of em at higher phi.
  //So if I want 
  //Okay, the actual conversion phi position is PhiConv = phi1 - Delta1
  //Again, Phi1 is supposed to be the larger of the two phi values.  Since this is effectively one
  //of the only places it matters, this is 
  float Delta1 =0;
  if (phi1 > phi2)
    Delta1 = (RadiusOfCurvature2/(RadiusOfCurvature1+RadiusOfCurvature2)) * Delta;
  else
    Delta1 = (RadiusOfCurvature1/(RadiusOfCurvature1+RadiusOfCurvature2)) * Delta;
  //Note that I cancelled the negative sign with Delta above.

  float PhiConv=-1;
  
  if (phi1 > phi2){
    PhiConv = phi1 - Delta1;
    if (PhiConv > TMath::Pi()*2.)
      PhiConv -=TMath::Pi()*2.;
    if (PhiConv < 0)
      PhiConv +=TMath::Pi()*2.;
  }

  else{
    PhiConv = phi2 - Delta1;
    if (PhiConv > TMath::Pi()*2.)
      PhiConv -=TMath::Pi()*2.;
    if (PhiConv < 0)
      PhiConv +=TMath::Pi()*2.;
  }
  
  return PhiConv;  
}

std::vector <TrackingRecHit*> Pi0ConversionAlgo::GetStubHits(BasicCluster ele1, BasicCluster ele2,
							     Float_t RConv, Float_t PhiConv, 
							     std::vector <TrackingRecHit*> FullTracker,
							     TrackerDigiGeometryRecord *geom,
							     std::vector <TrackingRecHit*> Stub1,
							     std::vector <TrackingRecHit*> Stub2)
{
  //Bits of this were originally in RSAdapter, and ConversionCandidate, but StubCandidate is
  //used almost untouched.
  

  vector <TrackingRecHit*> hits_pos_stub1;
  vector <TrackingRecHit*> hits_neg_stub1;

  vector <TrackingRecHit*> hits_pos_stub2;
  vector <TrackingRecHit*> hits_neg_stub2;

  //Make stubs
  StubCandidate posstub1(ele1.x(), ele1.y(), ele1.z(), ele1.energy(),0,RConv,PhiConv,1);
  StubCandidate negstub1(ele1.x(), ele1.y(), ele1.z(), ele1.energy(),0,RConv,PhiConv,-1);
  StubCandidate posstub2(ele2.x(), ele2.y(), ele2.z(), ele2.energy(),0,RConv,PhiConv,1);
  StubCandidate negstub2(ele2.x(), ele2.y(), ele2.z(), ele2.energy(),0,RConv,PhiConv,-1);
  //Using this constructor, the roads should be created.  Remember to check this when debugging.  
  //Select hits that are in road.

  
  //Make one loop through the hits, and then you're done.
  for (int th=0;th < int(FullTracker.size()); ++th){
    //get globalpoint position, and feed hit to stubs
    GlobalPoint position = geom->idToDet((FullTracker[th]).geographicalId())->surface().toGlobal((FullTracker[th]).localPosition());

    //Can I auto stiletto-ize?  Get "IsHitInRoad", and Chi2XY for hit.  Whoever has smallest Chi2 gets the hit.
    bool Pos1 =   posstub1.IsInRoad(position);
    bool Pos2 =   posstub2.IsInRoad(position);
    bool Neg1 =   negstub1.IsInRoad(position);
    bool Neg2 =   negstub2.IsInRoad(position);
    bool HitTaken = false;

    double Pos1Chi2XY = 1e16;
    double Pos2Chi2XY = 1e16;
    double Neg1Chi2XY = 1e16;
    double Neg2Chi2XY = 1e16;
    if (Pos2) Pos2Chi2XY = posstub2.Chi2Hit(position);
    if (Neg1) Neg1Chi2XY = negstub1.Chi2Hit(position);
    if (Neg2) Neg2Chi2XY = negstub2.Chi2Hit(position);
    if (Pos1) Pos1Chi2XY = posstub1.Chi2Hit(position);

    //Check hit against first stub:
    //If it HAS the hit, and the smallest chi2, add it.
    if (Pos1Chi2XY < Pos2Chi2XY
	&& Pos1Chi2XY < Neg1Chi2XY
	&& Pos1Chi2XY < Neg2Chi2XY
	&& Pos1){
      //Stub1 gets the hit.
      HitTaken = true;
      hits_pos_stub1.push_back(FullTracker[th]);
    }
    
    if (!HitTaken){
      if (Pos2Chi2XY < Neg1Chi2XY
	  && Pos2Chi2XY < Neg2Chi2XY
	  && Pos2){
	HitTaken = true;
	hits_pos_stub2.push_back(FullTracker[th]);
      }
    }
    if (!HitTaken){
      if (Neg1Chi2XY < Neg2Chi2XY
	  && Neg1){
	HitTaken = true;
	hits_neg_stub1.push_back(FullTracker[th]);
      }
    }
    if (!HitTaken){
      if (Neg2){
	HitTaken = true;
	hits_pos_stub2.push_back(FullTracker[th]);
      }
    }
    
  }//Done Looping over all the hits.
  

  //Decide on Stub1
  //Ok, so who is the best stub?  First select on hits, then select on chi2.
  if (hits_pos_stub1.size() > hits_neg_stub1.size() && hits_pos_stub1.size()!=hits_neg_stub1.size()){
    for (int i=0;i< hits_pos_stub1.size();++i)
      Stub1.push_back(hits_pos_stub1[i]);
  }
  else if ( hits_pos_stub1.size()!=hits_neg_stub1.size()) {
    for (int i=0;i< hits_neg_stub1.size();++i)
      Stub1.push_back(hits_neg_stub1[i]);
  }
  if (hits_pos_stub1.size() == hits_neg_stub1.size()){
    //Test total chi2
    vector <GlobalPoint> stubPos1;
    vector <GlobalPoint> stubNeg1;
    for (int i=0;i<hits_pos_stub1.size();++i){
      GlobalPoint position = geom->idToDet((hits_pos_stub1[i]).geographicalId())->surface().toGlobal((hits_pos_stub1[i]).localPosition());
      stubPos1.push_back(position);
    }
    for (int i=0;i<hits_neg_stub1.size();++i){
      GlobalPoint position = geom->idToDet((hits_neg_stub1[i]).geographicalId())->surface().toGlobal((hits_neg_stub1[i]).localPosition());
      stubNeg1.push_back(position);
    }
    double TotChi2Pos1 = posstub1.Chi2XY(stubPos1);
    double TotChi2Neg1 = negstub1.Chi2XY(stubNeg1);
    if (TotChi2Pos1 < TotChi2Neg1){
      for (int i=0;i< hits_pos_stub1.size();++i)
	Stub1.push_back(hits_pos_stub1[i]);
    }
    else{
      for (int i=0;i< hits_neg_stub1.size();++i)
      Stub1.push_back(hits_neg_stub1[i]);
    }
  }

  //////////////////////////
  //Decide on Stub2
  //Ok, so who is the best stub?  First select on hits, then select on chi2.
  if (hits_pos_stub2.size() > hits_neg_stub2.size() && hits_pos_stub2.size()!=hits_neg_stub2.size()){
    for (int i=0;i< hits_pos_stub2.size();++i)
      Stub2.push_back(hits_pos_stub2[i]);
  }
  else if ( hits_pos_stub2.size()!=hits_neg_stub2.size()) {
    for (int i=0;i< hits_neg_stub2.size();++i)
      Stub2.push_back(hits_neg_stub2[i]);
  }
  if (hits_pos_stub2.size() == hits_neg_stub2.size()){
    //Test total chi2
    vector <GlobalPoint> stubPos2;
    vector <GlobalPoint> stubNeg2;
    for (int i=0;i<hits_pos_stub2.size();++i){
      GlobalPoint position = geom->idToDet((hits_pos_stub2[i]).geographicalId())->surface().toGlobal((hits_pos_stub2[i]).localPosition());
      stubPos2.push_back(position);
    }
    for (int i=0;i<hits_neg_stub2.size();++i){
      GlobalPoint position = geom->idToDet((hits_neg_stub2[i]).geographicalId())->surface().toGlobal((hits_neg_stub2[i]).localPosition());
      stubNeg2.push_back(position);
    }
    double TotChi2Pos2 = posstub2.Chi2XY(stubPos2);
    double TotChi2Neg2 = negstub2.Chi2XY(stubNeg2);
    if (TotChi2Pos2 < TotChi2Neg2){
      for (int i=0;i< hits_pos_stub2.size();++i)
	Stub2.push_back(hits_pos_stub2[i]);
    }
    else{
      for (int i=0;i< hits_neg_stub2.size();++i)
      Stub2.push_back(hits_neg_stub2[i]);
    }
  }

  //Done making hit collections!
}


Reco::Track Pi0ConversionAlgo::FitTrack( vector <TrackingRecHit> Stub, MeasurementTracker *theMeasurementTracker, TransientTrackingRecHitBuilder *ttrhBuilder,
					 const MagneticField *magField)
{
  //Around 900 lines of code regarding the fitting of the candidate hits into a track.  That is
  //of course, presuming that we've GOT the right hits.
  
 
  // Create the trajectory cleaner 
  TrajectoryCleanerBySharedHits theTrajectoryCleaner;
  vector<Trajectory> FinalTrajectories;
 
  // need this to sort recHits, sorting done after getting seed because propagationDirection is needed
  // get tracker geometry
  
  //Made this use the electron mass, make sure to check the number of 0s.
  thePropagator = new PropagatorWithMaterial(alongMomentum,.000511,&(*magField)); 
  theRevPropagator = new PropagatorWithMaterial(oppositeToMomentum,.000511,&(*magField)); 
  AnalyticalPropagator prop(magField,anyDirection);
  TrajectoryStateTransform transformer;
   
  KFTrajectorySmoother theSmoother(*theRevPropagator, *theUpdator, *theEstimator);
 
  if (debug_) std::cout << std::endl << std::endl
			<< "*** NEW EVENT: Clean Clouds input size: " << Stub.size() << std::endl;

  int i_c = 0;
  int nchit = 0;
  //This WAS the beginning of a loop.  However, right now we'll try to fit one stub at a time.
  {//This is a placeholder for the loop over the clouds


    // fill rechits from cloud into new OwnVector
    //Clouds have their own function for this, I just fill in the vector.
    //    edm::OwnVector<TrackingRecHit> recHits = cloud->recHits();
    edm::OwnVector<TrackingRecHit> recHits;
    for (int i=0;i<Stub.size();++i){
      recHits.push_back(Stub[i].clone());
    }
    nchit = recHits.size();
    
    vector<Trajectory> CloudTrajectories;
    
    //Not Sure what to do with these!
    RoadSearchCloud::SeedRefs theSeeds = cloud->seeds();
    RoadSearchCloud::SeedRefs::const_iterator iseed;
    //Additional code may be needed!

    //////////////HIT SORTING BY RADIUS
    //The following piece of code is very familiar.  The ORCA version does effectively the same thing,
    //so no change is necessary.
    recHits.sort(TrackingRecHitLessFromGlobalPosition(((TrackingGeometry*)(&(*tracker))),alongMomentum));
    
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
      GlobalPoint gp = tracker->idToDet(ih->geographicalId())->surface().toGlobal(ih->localPosition());
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
    const int min_chunk_length = 5;
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
      //const int nfound_min = min_chunk_length-1;
      const int nfound_min = 4;
      multimap<int, const DetLayer*>::iterator ilm = layer_map.begin();
      int ngoodlayers = 0;
      while (ilm != layer_map.end()) {
	//HERE is another change by A. A, this code had been changed to ONLY use layers with one hit.
	//for conversions I have reverted this.
	if (ngoodlayers >= nfound_min && ilm->first > 1) break;
	//    if (ilm->first > 1) break;
	map<const DetLayer*, int>::iterator ilr = layer_reference.find(ilm->second);
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
	  
	  GlobalPoint inner = tracker->idToDet(innerHit->geographicalId())->surface().toGlobal(innerHit->localPosition());
	  GlobalPoint outer = tracker->idToDet(outerHit->geographicalId())->surface().toGlobal(outerHit->localPosition());
	  
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
	  vector<Trajectory> rawTrajectories;
	  
	  Trajectory seedTraj(*(new TrajectorySeed()), alongMomentum);
	  
	  // Need to put the first hit on the trajectory
	  const TrajectoryStateOnSurface innerState = 
	    thePropagator->propagate(fts,tracker->idToDet(innerHit->geographicalId())->surface());
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
	if (skipped_layers.empty() && i->measurements().size() >= theNumHitCut) {
	  extendedChunks.insert(extendedChunks.end(), *i);
	} 
	//otherwise, if I CAN add hits, I should at least try.
	else {	  
	  Trajectory temptraj = *i;
	  Trajectory::DataContainer tmv = (*i).measurements();

	  //ingenious.  If you can't possibly get to the minimum size, then don't waste time.
	  if (tmv.size()+skipped_layer_detmap.size() < theNumHitCut) continue;

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
	      std::vector<TrajectoryMeasurement> theGoodHits = FindBestHit(theTSOS,dets,skipped_hits);
	      if (!theGoodHits.empty())
		newTrajectory.push(theGoodHits.front(),theGoodHits.front().estimate());
	      ++imap;
	    }
	    else {
	      TransientTrackingRecHit::ConstRecHitPointer rh = im->recHit();
	      if (rh->isValid() && 
		  theMeasurementTracker->geometricSearchTracker()->detLayer(rh->geographicalId()) == layers[ilayer]) {
803                 if (debug_) std::cout<<" has a good hit " << std::endl;
804                 ++im;
805 
806                 const GeomDet* det = geom->idToDet(rh->geographicalId());
807             
808                 if (newTrajectory.measurements().empty())
809                   if (debug_) std::cout<<"How the heck does this have no measurements!!!" <<std::endl;
810 
811 
812                 currTsos = newTrajectory.measurements().back().updatedState();
813                 predTsos = thePropagator->propagate(currTsos, det->surface());
814                 if (!predTsos.isValid()) continue;
815                 MeasurementEstimator::HitReturnType est = theEstimator->estimate(predTsos, *rh);
816                 if (!est.first) {
817                   if (debug_) std::cout<<"Failed to add one of the original hits on a low occupancy layer!!!!" << std::endl;
818                   continue;
819                 }
820                 currTsos = theUpdator->update(predTsos, *rh);
821                 tm = TrajectoryMeasurement(predTsos, currTsos, &(*rh),est.second,layers[ilayer]);
822                 newTrajectory.push(tm,est.second);
823 
824               }
825               else {
826                 if (debug_) std::cout<<" has no hit" << std::endl;
827               }
828             }       
829           }
830 
831           if (debug_) std::cout<<"Finished loop over layers in cloud.  Trajectory now has " <<newTrajectory.measurements().size()
832                    << " hits. " << std::endl;
833           // DO WE NEED TO SMOOTH THIS TRAJECTORY?
834           //newSmoothed = theSmoother.trajectories(newTrajectory);
835           //if (newSmoothed.empty()){
836           //  std::cout<< " Smoothing of new trajectory has failed. " <<std::endl;
837           // }
838           //std::cout<< " Smoothing has succeeded. " <<std::endl;
839           if (newTrajectory.measurements().size() >= theNumHitCut)
840             extendedChunks.insert(extendedChunks.end(), newTrajectory);
841         }
842       }
843       if (debug_) std::cout<< "Extended chunks: " << extendedChunks.size() << endl;
844       //if (!extendedChunks.empty()) {
845       //  smoothedResult.insert(smoothedResult.end(), extendedChunks.begin(), extendedChunks.end());
846       //  break;
847       //}
848       if (debug_) std::cout<< "Now Clean the extended chunks " <<std::endl;
849       theTrajectoryCleaner.clean(extendedChunks);
850       for (vector<Trajectory>::const_iterator it = extendedChunks.begin();
851            it != extendedChunks.end(); it++) {
852         if (it->isValid()) CloudTrajectories.push_back(*it);
853       }
854     }
855 
856 // ********************* END NEW ADDITION
857 
858     if (debug_) std::cout<< "Finished with the cloud, so clean the " 
859              << CloudTrajectories.size() << " cloud trajectories "<<std::endl ;
860     theTrajectoryCleaner.clean(CloudTrajectories);
861     for (vector<Trajectory>::const_iterator it = CloudTrajectories.begin();
862          it != CloudTrajectories.end(); it++) {
863       if (it->isValid()) FinalTrajectories.push_back(*it);
864     }
865 
866     if (debug_) std::cout<<" Final trajectories now has size " << FinalTrajectories.size()<<std::endl ;
867 
868   } // End loop over Cloud Collection
869 
870   if (debug_) std::cout<< " Finished loop over all clouds " <<std::endl;
871     theTrajectoryCleaner.clean(FinalTrajectories);
872     for (vector<Trajectory>::const_iterator it = FinalTrajectories.begin();
873          it != FinalTrajectories.end(); it++) {
874       if (debug_) std::cout<< " Trajectory has "<<it->recHits().size()<<" hits with chi2="
875                              <<it->chiSquared()<<" and is valid? "<<it->isValid()<<std::endl;
876       if (it->isValid()){
877 
878         edm::OwnVector<TrackingRecHit> goodHits;
879         //edm::OwnVector<const TransientTrackingRecHit> ttHits = it->recHits(); 
880         //for (edm::OwnVector<const TransientTrackingRecHit>::const_iterator rhit=ttHits.begin(); 
881         TransientTrackingRecHit::ConstRecHitContainer ttHits = it->recHits();           
882         for (TransientTrackingRecHit::ConstRecHitContainer::const_iterator rhit=ttHits.begin(); 
883              rhit!=ttHits.end(); ++rhit){
884           goodHits.push_back((*rhit)->hit()->clone());
885         }
886 
887         if (debug_) std::cout<<" Trajectory has " << goodHits.size() << " good hits "<<std::endl;
888         // clone 
889         //TrajectorySeed seed = *((*ref).clone());
890         PTrajectoryStateOnDet state = it->seed().startingState();
891         TrajectoryStateOnSurface firstState;
892 
893         // check if Trajectory from seed is on first hit of the cloud, if not, propagate
894         // exclude if first state on first hit is not valid
895       
896         DetId FirstHitId = (*(it->recHits().begin()))->geographicalId();
897         if (debug_) std::cout<< " FirstHitId is null ? "<< FirstHitId.null()<<std::endl;
898 
899         // propagate back to first hit
900 
901         TrajectoryMeasurement firstMeasurement = it->measurements()[0];
902         ////if (it->recHits().begin()->geographicalId().rawId() != state.detId()) {
903         const GeomDet* det = geom->idToDet(FirstHitId);
904           // const GeomDet* detState = geom->idToDet(DetId(state.detId())  );
905           
906           //TrajectoryStateOnSurface before(transformer.transientState(state,  &(detState->surface()), magField));
907           // firstState = prop.propagate(before, det->surface());
908         firstState = prop.propagate(firstMeasurement.updatedState(), det->surface());
909           
910           if (firstState.isValid() == false) continue;
911 
912           state = *(transformer.persistentState(firstState,FirstHitId.rawId()));
913 
914           //std::cout<<"This track candidate has " << goodHits.size() << " hits "<<std::endl ;
915 
916         output.push_back(TrackCandidate(goodHits,it->seed(),state));
917       }
918     }
919 
920 
921     if (debug_) std::cout<< "Found " << output.size() << " track candidates."<<std::endl;
922 
923 }
924
RoadSearchTrackCandidateMakerAlgorithm::FindBestHit(const TrajectoryStateOnSurface& tsosBefore,
998                                                     const std::set<const GeomDet*>& theDets,
999                                                     edm::OwnVector<TrackingRecHit>& theHits)
1000 {
1001 
1002   std::vector<TrajectoryMeasurement> theBestHits;
1003 
1004   bool found_one = false;
1005   double bestchi = 10000.0;
1006   // extrapolate to all detectors from the list
1007   map<const GeomDet*, TrajectoryStateOnSurface> dmmap;
1008   for (set<const GeomDet*>::iterator idet = theDets.begin();
1009        idet != theDets.end(); ++idet) {
1010     TrajectoryStateOnSurface predTsos = thePropagator->propagate(tsosBefore, (**idet).surface());
1011     if (predTsos.isValid()) {
1012       dmmap.insert(make_pair(*idet, predTsos));
1013     }
1014   }
1015   // evaluate hit residuals
1016   map<const GeomDet*, TrajectoryMeasurement> dtmmap;
1017   for (edm::OwnVector<TrackingRecHit>::const_iterator ih = theHits.begin();
1018        ih != theHits.end(); ++ih) {
1019     const GeomDet* det = geom->idToDet(ih->geographicalId());
1020     //if (*isl != theMeasurementTracker->geometricSearchTracker()->detLayer(ih->geographicalId())) 
1021     //  cout <<" You don't know what you're doing !!!!" << endl;
1022     
1023     map<const GeomDet*, TrajectoryStateOnSurface>::iterator idm = dmmap.find(det);
1024     if (idm == dmmap.end()) continue;
1025     TrajectoryStateOnSurface predTsos = idm->second;
1026     TransientTrackingRecHit::RecHitPointer rhit = ttrhBuilder->build(&(*ih));
1027     MeasurementEstimator::HitReturnType est = theEstimator->estimate(predTsos, *rhit);
1028     //std::cout<< "hit " << ih-theHits.begin() 
1029     //     << ": est = " << est.first << " " << est.second  <<std::endl;
1030     
1031     
1032     // Take the best hit on any Det
1033     if (est.first) {
1034       TrajectoryStateOnSurface currTsos = theUpdator->update(predTsos, *rhit);
1035       found_one = true;
1036       if (est.second < bestchi){
1037         if(!theBestHits.empty()){
1038           theBestHits.erase(theBestHits.begin());
1039         }
1040         bestchi = est.second;
1041         TrajectoryMeasurement tm(predTsos, currTsos, &(*rhit),est.second,
1042                                  theMeasurementTracker->geometricSearchTracker()->detLayer(ih->geographicalId()));      
1043         theBestHits.push_back(tm);
1044       }
1045     }
1046   }
1047 
1048   if (theBestHits.empty()){
1049     if (debug_) std::cout<< "no hits to add! " <<std::endl;
1050   }
1051   else{
1052     for (std::vector<TrajectoryMeasurement>::const_iterator im=theBestHits.begin();im!=theBestHits.end();++im)
1053       if (debug_) std::cout<<" Measurement on layer "
1054                           << theMeasurementTracker->geometricSearchTracker()->detLayer(im->recHit()->geographicalId())
1055                           << " with estimate " << im->estimate()<<std::endl ;
1056   }
1057   //std::cout<<" Measurement returned with estimate "<< theBestHit.estimate() << std::endl ;
1058   
1059   return theBestHits;
1060
