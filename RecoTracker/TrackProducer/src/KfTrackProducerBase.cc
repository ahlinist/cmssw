#include "RecoTracker/TrackProducer/interface/KfTrackProducerBase.h"
// system include files
#include <memory>
// user include files
#include "DataFormats/TrackReco/interface/TrackResiduals.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "TrackingTools/GeomPropagators/interface/Propagator.h"
#include "TrackingTools/PatternTools/interface/Trajectory.h"

#include "TrackingTools/PatternTools/interface/TrajTrackAssociation.h"

#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"

#include "RecoTracker/TrackProducer/interface/ClusterRemovalRefSetter.h"
#include "TrajectoryToResiduals.h"

void KfTrackProducerBase::putInEvt(edm::Event& evt,
				 std::auto_ptr<TrackingRecHitCollection>& selHits,
				 std::auto_ptr<reco::TrackCollection>& selTracks,
				 std::auto_ptr<reco::TrackExtraCollection>& selTrackExtras,
				 std::auto_ptr<std::vector<Trajectory> >&   selTrajectories,
				 AlgoProductCollection& algoResults)
{

  TrackingRecHitRefProd rHits = evt.getRefBeforePut<TrackingRecHitCollection>();
  reco::TrackExtraRefProd rTrackExtras = evt.getRefBeforePut<reco::TrackExtraCollection>();

  edm::Ref<reco::TrackExtraCollection>::key_type idx = 0;
  edm::Ref<reco::TrackExtraCollection>::key_type hidx = 0;
  edm::Ref<reco::TrackCollection>::key_type iTkRef = 0;
  edm::Ref< std::vector<Trajectory> >::key_type iTjRef = 0;
  std::map<unsigned int, unsigned int> tjTkMap;

  for(AlgoProductCollection::iterator i=algoResults.begin(); i!=algoResults.end();i++){
    Trajectory * theTraj = (*i).first;
    if(trajectoryInEvent_) {
      selTrajectories->push_back(*theTraj);
      iTjRef++;
    }

    // const TrajectoryFitter::RecHitContainer& transHits = theTraj->recHits(useSplitting);  // NO: the return type in Trajectory is by VALUE
    TrajectoryFitter::RecHitContainer transHits = theTraj->recHits(useSplitting);

    reco::Track * theTrack = (*i).second.first;
    
    // Hits are going to be re-sorted along momentum few lines later. 
    // Therefore the direction stored in the TrackExtra 
    // has to be "alongMomentum" as well. Anyway, this direction can be differnt from the one of the orignal
    // seed! The name seedDirection() for the Track's method (and the corresponding data member) is
    // misleading and should be changed into something like "hitsDirection()". TO BE FIXED!
    PropagationDirection seedDir = alongMomentum;

    LogDebug("TrackProducer") << "In KfTrackProducerBase::putInEvt - seedDir=" << seedDir;

    reco::Track t = * theTrack;
    selTracks->push_back( t );
    iTkRef++;

    // Store indices in local map (starts at 0)
    if(trajectoryInEvent_) tjTkMap[iTjRef-1] = iTkRef-1;
    
    //sets the outermost and innermost TSOSs
    TrajectoryStateOnSurface outertsos;
    TrajectoryStateOnSurface innertsos;
    unsigned int innerId, outerId;
    
    // ---  NOTA BENE: the convention is to sort hits and measurements "along the momentum".
    // This is consistent with innermost and outermost labels only for tracks from LHC collision
    if (theTraj->direction() == alongMomentum) {
      outertsos = theTraj->lastMeasurement().updatedState();
      innertsos = theTraj->firstMeasurement().updatedState();
      outerId = theTraj->lastMeasurement().recHit()->geographicalId().rawId();
      innerId = theTraj->firstMeasurement().recHit()->geographicalId().rawId();
    } else { 
      outertsos = theTraj->firstMeasurement().updatedState();
      innertsos = theTraj->lastMeasurement().updatedState();
      outerId = theTraj->firstMeasurement().recHit()->geographicalId().rawId();
      innerId = theTraj->lastMeasurement().recHit()->geographicalId().rawId();
    }
    // ---
    //build the TrackExtra
    GlobalPoint v = outertsos.globalParameters().position();
    GlobalVector p = outertsos.globalParameters().momentum();
    math::XYZVector outmom( p.x(), p.y(), p.z() );
    math::XYZPoint  outpos( v.x(), v.y(), v.z() );
    v = innertsos.globalParameters().position();
    p = innertsos.globalParameters().momentum();
    math::XYZVector inmom( p.x(), p.y(), p.z() );
    math::XYZPoint  inpos( v.x(), v.y(), v.z() );

    reco::TrackExtraRef teref= reco::TrackExtraRef ( rTrackExtras, idx ++ );
    reco::Track & track = selTracks->back();
    track.setExtra( teref );
    
    //======= I want to set the second hitPattern here =============
    if (theSchool.isValid())
      {
	NavigationSetter setter( *theSchool );
	setSecondHitPattern(theTraj,track);
      }
    //==============================================================
    
    selTrackExtras->push_back( reco::TrackExtra (outpos, outmom, true, inpos, inmom, true,
						 outertsos.curvilinearError(), outerId,
						 innertsos.curvilinearError(), innerId,
    						 seedDir, theTraj->seedRef()));


    reco::TrackExtra & tx = selTrackExtras->back();
    
    // ---  NOTA BENE: the convention is to sort hits and measurements "along the momentum".
    // This is consistent with innermost and outermost labels only for tracks from LHC collisions
    size_t i = 0;
    if (theTraj->direction() == alongMomentum) {
      for( TrajectoryFitter::RecHitContainer::const_iterator j = transHits.begin();
	   j != transHits.end(); j ++ ) {
	if ((**j).hit()!=0){
	  TrackingRecHit * hit = (**j).hit()->clone();
	  track.setHitPattern( * hit, i ++ );
	  selHits->push_back( hit );
	  tx.add( TrackingRecHitRef( rHits, hidx ++ ) );
	}
      }
    }else{
      for( TrajectoryFitter::RecHitContainer::const_iterator j = transHits.end()-1;
	   j != transHits.begin()-1; --j ) {
	if ((**j).hit()!=0){
	  TrackingRecHit * hit = (**j).hit()->clone();
	  track.setHitPattern( * hit, i ++ );
	  selHits->push_back( hit );
	tx.add( TrackingRecHitRef( rHits, hidx ++ ) );
	}
      }
    }
    // ----
    tx.setResiduals(trajectoryToResiduals(*theTraj));

    delete theTrack;
    delete theTraj;
  }

  // Now we can re-set refs to hits, as they have already been cloned
  if (rekeyClusterRefs_) {
      ClusterRemovalRefSetter refSetter(evt, clusterRemovalInfo_);
      for (TrackingRecHitCollection::iterator it = selHits->begin(), ed = selHits->end(); it != ed; ++it) {
          refSetter.reKey(&*it);
      }
  }

  LogTrace("TrackingRegressionTest") << "========== TrackProducer Info ===================";
  LogTrace("TrackingRegressionTest") << "number of finalTracks: " << selTracks->size();
  for (reco::TrackCollection::const_iterator it = selTracks->begin(); it != selTracks->end(); it++) {
    LogTrace("TrackingRegressionTest") << "track's n valid and invalid hit, chi2, pt, eta : " 
				       << it->found() << " , " 
				       << it->lost()  <<" , " 
				       << it->normalizedChi2() << " , "
				       << it->pt() << " , "
				       << it->eta() ;
  }
  LogTrace("TrackingRegressionTest") << "=================================================";
  
  
  rTracks_ = evt.put( selTracks );
  evt.put( selTrackExtras );
  evt.put( selHits );

  if(trajectoryInEvent_) {
    edm::OrphanHandle<std::vector<Trajectory> > rTrajs = evt.put(selTrajectories);

    // Now Create traj<->tracks association map
    std::auto_ptr<TrajTrackAssociationCollection> trajTrackMap( new TrajTrackAssociationCollection() );
    for ( std::map<unsigned int, unsigned int>::iterator i = tjTkMap.begin(); 
          i != tjTkMap.end(); i++ ) {
      edm::Ref<std::vector<Trajectory> > trajRef( rTrajs, (*i).first );
      edm::Ref<reco::TrackCollection>    tkRef( rTracks_, (*i).second );
      trajTrackMap->insert( edm::Ref<std::vector<Trajectory> >( rTrajs, (*i).first ),
                            edm::Ref<reco::TrackCollection>( rTracks_, (*i).second ) );
    }
    evt.put( trajTrackMap );
  }
}

#ifdef MOVEAROUND

#include <TrackingTools/DetLayers/interface/DetLayer.h>
#include <DataFormats/TrackingRecHit/interface/InvalidTrackingRecHit.h>
void KfTrackProducerBase::setSecondHitPattern(Trajectory* traj,
					      reco::Track& track){
  using namespace std;
  //cout << endl << "===== traj #measurements: " << traj->measurements().size() << endl;

  // Nota Bene: At the moment the trajectories has the measurements with reversed sorting after the track smoothing. 
  // Therefore the lastMeasurement is the inner one (for LHC-like tracks)
  if(traj->firstMeasurement().updatedState().isValid() &&
     traj->lastMeasurement().updatedState().isValid()){
    FreeTrajectoryState*  outerState = traj->firstMeasurement().updatedState().freeState();    
    FreeTrajectoryState*  innerState = traj->lastMeasurement().updatedState().freeState();    
    const DetLayer* outerLayer = traj->firstMeasurement().layer();
    const DetLayer* innerLayer = traj->lastMeasurement().layer();

    if (!outerLayer || !innerLayer){
      //means  that the trajectory was fit/smoothed in a special case: not setting those pointers
      LogDebug("TrackProducer") << "the trajectory was fit/smoothed in a special case: not setting those pointers";
      return;
    }
    
    //    LogDebug("TrackProducer") << "-- calling compLayer for inner..";
    std::vector< const DetLayer * > innerCompLayers = innerLayer->compatibleLayers(*innerState,oppositeToMomentum);
    //    LogDebug("TrackProducer") << "-- calling compLayer for outer..";
    std::vector< const DetLayer * > outerCompLayers = outerLayer->compatibleLayers(*outerState,alongMomentum);

    LogDebug("TrackProducer")
      << "inner DetLayer  sub: " 
      << innerLayer->subDetector() <<"\n"
      << "outer DetLayer  sub: " 
      << outerLayer->subDetector() << "\n"
      << "innerstate position rho: " << innerState->position().perp() << " z: "<< innerState->position().z()<<"\n"
      << "innerstate state pT: " << innerState->momentum().perp() << " pz: "<< innerState->momentum().z()<<"\n"
      << "outerstate position rho: " << outerState->position().perp() << " z: "<< outerState->position().z()<<"\n"
      << "outerstate state pT: " << outerState->momentum().perp() << " pz: "<< outerState->momentum().z()<<"\n"

      << "innerLayers: " << innerCompLayers.size() << "\n"
      << "outerLayers: " << outerCompLayers.size() << "\n";

    int counter = 0;
    for(vector<const DetLayer *>::const_iterator it=innerCompLayers.begin(); it!=innerCompLayers.end();
	++it,++counter){
      if ((*it)->basicComponents().empty()) {
	//this should never happen. but better protect for it
	edm::LogWarning("TrackProducer")<<"a detlayer with no components: I cannot figure out a DetId from this layer. please investigate.";
	continue;
      }
      InvalidTrackingRecHit  tmpHit((*it)->basicComponents().front()->geographicalId(),TrackingRecHit::missing);
      track.setTrackerExpectedHitsInner(tmpHit,counter); 
      //      LogDebug("TrackProducer")<< "counter: " << counter;
    }
    
    counter=0;
    for(vector<const DetLayer *>::const_iterator it=outerCompLayers.begin(); it!=outerCompLayers.end();
	++it,++counter){
      if ((*it)->basicComponents().empty()){
	//this should never happen. but better protect for it
	edm::LogWarning("TrackProducer")<<"a detlayer with no components: I cannot figure out a DetId from this layer. please investigate.";
	continue;
      }
      InvalidTrackingRecHit  tmpHit((*it)->basicComponents().front()->geographicalId(),TrackingRecHit::missing);
      track.setTrackerExpectedHitsOuter(tmpHit,counter); 
      //      LogDebug("TrackProducer")<< "counter: " << counter;
    }
  }else{
    cout << "inner or outer state was invalid" << endl;
  }


  //DetLayer* layer = traj.lastLayer();

}

#endif
