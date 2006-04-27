#include "TrackingTools/TrackFitters/interface/KFTrajectoryFitter.h"
#include "TrackingTools/TrackFitters/interface/TrajectoryStateWithArbitraryError.h"
#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHit.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/SiStripDetId/interface/TIBDetId.h"
#include "DataFormats/SiStripDetId/interface/TOBDetId.h"
#include "DataFormats/SiStripDetId/interface/TIDDetId.h"
#include "DataFormats/SiStripDetId/interface/TECDetId.h"

KFTrajectoryFitter::~KFTrajectoryFitter() {

  delete thePropagator;
  delete theUpdator;
  delete theEstimator;

}


vector<Trajectory> KFTrajectoryFitter::fit(const Trajectory& aTraj) const {

  if(aTraj.empty()) return vector<Trajectory>();
 
  TM firstTM = aTraj.firstMeasurement();
  TSOS firstTsos = TrajectoryStateWithArbitraryError()(firstTM.updatedState());
  
  return fit(aTraj.seed(), aTraj.recHits(), firstTsos);
}

vector<Trajectory> KFTrajectoryFitter::fit(const TrajectorySeed& aSeed,
					   const edm::OwnVector<TransientTrackingRecHit>& hits) const{

  throw cms::Exception("TrackingTools/TrackFitters", 
		       "KFTrajectoryFitter::fit(TrajectorySeed, <TransientTrackingRecHit>) not implemented"); 

  return vector<Trajectory>();
}

vector<Trajectory> KFTrajectoryFitter::fit(const TrajectorySeed& aSeed,
					   const edm::OwnVector<TransientTrackingRecHit>& hits,
					   const TSOS& firstPredTsos) const {

  if(hits.empty()) return vector<Trajectory>();
  LogDebug("TrackingTools/TrackFitters")
    <<" +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n"
    <<" KFTrajectoryFitter::fit staring with "<<hits.size()<<" HITS \n"
    <<" INITIAL STATE "<<firstPredTsos<<"\n";
  

  Trajectory myTraj(aSeed, propagator()->propagationDirection());

  TSOS predTsos(firstPredTsos);
  if(!predTsos.isValid()) {
    edm::LogError("TrackingTools/TrackFitters") 
      << "KFTrajectoryFitter: predicted tsos of first measurement not valid!\n"
      << "predTsos" << predTsos << "\n";
    return vector<Trajectory>();
  } 
  TSOS currTsos;

  if((&*(hits.begin()))->isValid()) {
    //update
    currTsos = updator()->update(predTsos, *(hits.begin()));
    myTraj.push(TM(predTsos, currTsos, ((hits.begin())->clone() ),
		   estimator()->estimate(predTsos, *(hits.begin()) ).second));
  } else {
    currTsos = predTsos;
    myTraj.push(TM(predTsos, hits.begin()->clone() ));
  }
  const TransientTrackingRecHit & firsthit = (*(hits.begin()));
  
  if (firsthit.isValid()){
    
    LogDebug("TrackingTools/TrackFitters")
      <<" ----------------- FIRST HIT -----------------------\n"
      <<"  HIT IS AT R   "<<(firsthit).globalPosition().perp()<<"\n"
      <<"  HIT IS AT Z   "<<(firsthit).globalPosition().z()<<"\n"
      <<"  HIT IS AT Phi "<<(firsthit).globalPosition().phi()<<"\n"
      <<"  HIT IS AT Loc "<<(firsthit).localPosition()<<"\n"
      <<"  WITH LocError "<<(firsthit).localPositionError()<<"\n"
      <<"  HIT IS AT Glo "<<(firsthit).globalPosition()<<"\n"
      <<"SURFACE POSITION"<<"\n"
      <<(firsthit).det()->surface().position()<<"\n"
      <<"SURFACE ROTATION"<<"\n"
      <<(firsthit).det()->surface().rotation()<<"\n"
      <<" predTsos !"<<"\n"
      <<predTsos<<"\n"
      <<" currTsos !"<<"\n"
      <<currTsos<<"\n";
    LogDebug("TrackingTools/TrackFitters") <<"  GOING TO examine hit "<<(firsthit).geographicalId().rawId()<<"\n";
    if ((firsthit).geographicalId().subdetId() == StripSubdetector::TIB ) {
      LogDebug("TrackingTools/TrackFitters") <<" I am TIB "<<TIBDetId((firsthit).geographicalId()).layer()<<"\n";
    }else if ((firsthit).geographicalId().subdetId() == StripSubdetector::TOB ) { 
      LogDebug("TrackingTools/TrackFitters") <<" I am TOB "<<TOBDetId((firsthit).geographicalId()).layer()<<"\n";
    }else if ((firsthit).geographicalId().subdetId() == StripSubdetector::TEC ) { 
      LogDebug("TrackingTools/TrackFitters") <<" I am TEC "<<TECDetId((firsthit).geographicalId()).wheel()<<"\n";
    }else if ((firsthit).geographicalId().subdetId() == StripSubdetector::TID ) { 
      LogDebug("TrackingTools/TrackFitters") <<" I am TID "<<TIDDetId((firsthit).geographicalId()).wheel()<<"\n";
    }else{
      LogDebug("TrackingTools/TrackFitters") <<" I am Pixel "<<"\n";
    }
  }

  for(edm::OwnVector<TransientTrackingRecHit>::const_iterator ihit = hits.begin() + 1; 
      ihit != hits.end(); ihit++) {
    if ((*ihit).isValid()){
      LogDebug("TrackingTools/TrackFitters")
	<<" ----------------- NEW HIT -----------------------"<<"\n"
	<<"  HIT IS AT R   "<<(*ihit).globalPosition().perp()<<"\n"
	<<"  HIT IS AT Z   "<<(*ihit).globalPosition().z()<<"\n"
	<<"  HIT IS AT Phi "<<(*ihit).globalPosition().phi()<<"\n"
	<<"  HIT IS AT Loc "<<(*ihit).localPosition()<<"\n"
	<<"  WITH LocError "<<(*ihit).localPositionError()<<"\n"
	<<"  HIT IS AT Glo "<<(*ihit).globalPosition()<<"\n"
	<<"SURFACE POSITION"<<"\n"
	<<(*ihit).det()->surface().position()<<"\n"
	<<"SURFACE ROTATION"<<"\n"
	<<(*ihit).det()->surface().rotation()<<"\n";
      LogDebug("TrackingTools/TrackFitters") <<" GOING TO examine hit "<<(*ihit).geographicalId().rawId()<<"\n";
      if ((*ihit).geographicalId().subdetId() == StripSubdetector::TIB ) {
	LogDebug("TrackingTools/TrackFitters") <<" I am TIB "<<TIBDetId((*ihit).geographicalId()).layer()<<"\n";
      }else if ((*ihit).geographicalId().subdetId() == StripSubdetector::TOB ) { 
	LogDebug("TrackingTools/TrackFitters") <<" I am TOB "<<TOBDetId((*ihit).geographicalId()).layer()<<"\n";
      }else if ((*ihit).geographicalId().subdetId() == StripSubdetector::TEC ) { 
	LogDebug("TrackingTools/TrackFitters") <<" I am TEC "<<TECDetId((*ihit).geographicalId()).wheel()<<"\n";
      }else if ((*ihit).geographicalId().subdetId() == StripSubdetector::TID ) { 
	LogDebug("TrackingTools/TrackFitters") <<" I am TID "<<TIDDetId((*ihit).geographicalId()).wheel()<<"\n";
      }else{
	LogDebug("TrackingTools/TrackFitters") <<" I am Pixel "<<"\n";
      }
    }
    
    predTsos = propagator()->propagate(currTsos,
				       (*ihit).det()->surface());

    if(!predTsos.isValid()) {
      edm::LogError("TrackingTools/TrackFitters") 
	<<" SOMETHING WRONG !"<<"\n"
	<<"KFTrajectoryFitter: predicted tsos not valid!\n" 
	<<"current TSOS: "<<currTsos<< "\n";
      if((*ihit).isValid())
	edm::LogError("TrackingTools/TrackFitters")
	  << "next Surface: "<<(*ihit).det()->surface().position()<< "\n";
      return vector<Trajectory>();
    }
    
    if((*ihit).isValid()) {
      //update
      LogDebug("TrackingTools/TrackFitters") <<"THE HIT IS VALID: updating predTsos"<<"\n";
      currTsos = updator()->update(predTsos, *ihit);
      myTraj.push(TM(predTsos, currTsos, (*ihit).clone(),
		     estimator()->estimate(predTsos, *ihit).second));
    } else {
      LogDebug("TrackingTools/TrackFitters") <<"THE HIT IS NOT VALID: using currTsos"<<"\n";
      currTsos = predTsos;
      myTraj.push(TM(predTsos, (*ihit).clone() ));
    }
    LogDebug("TrackingTools/TrackFitters")
      <<" predTsos !"<<"\n"
      <<predTsos<<"\n"
      <<" currTsos !"<<"\n"
      <<currTsos<<"\n";
    //std::cout <<(*ihit).det()->surface().position()<<std::endl;
  }
  //
  // debug
  //
  //std::cout <<" Before RETURN IN KFTrajectoryFitter"<<std::endl;
  
  LogDebug("TrackingTools/TrackFitters") <<" Found 1 trajectory wit hits "<< myTraj.foundHits()<<"\n";
  
  return vector<Trajectory>(1, myTraj);
}

