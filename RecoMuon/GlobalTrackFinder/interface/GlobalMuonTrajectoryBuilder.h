#ifndef GlobalTrackFinder_GlobalMuonTrajectoryBuilder_H
#define GlobalTrackFinder_GlobalMuonTrajectoryBuilder_H

/** \class GlobalMuonTrajectoryBuilder
 *  class to build muon trajectory
 *
 *  $Date: 2006/06/26 23:56:21 $
 *  $Revision: 1.3 $
 *  \author Chang Liu - Purdue University
 */

#include "RecoMuon/TrackingTools/interface/MuonReconstructionEnumerators.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrajectorySeed/interface/TrajectorySeedCollection.h"
#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"


class CkfTrajectoryBuilder;
class TrajectorySmoother;
class TrajectoryCleaner;
class BasicTrajectorySeed; 
class GlobalMuonReFitter; 
class TrajectoryFitter;
class Propagator;
class Trajectory;
class TrackerGeometry;
class TrajectoryStateOnSurface;
class TransientTrackingRecHit;
class TransientTrackingRecHitBuilder;
class MagneticField;

namespace edm {class ParameterSet;}

class GlobalMuonTrajectoryBuilder{

public:

  typedef edm::OwnVector< const TransientTrackingRecHit>  RecHitContainer;
  typedef std::vector<Trajectory> TC;
  typedef TC::const_iterator TI;
  typedef std::pair<Trajectory, reco::TrackRef*> TrajWithTkLabel;
  typedef std::vector<TrajWithTkLabel> TTC;

 
  /// constructor
  GlobalMuonTrajectoryBuilder(const edm::ParameterSet& par);
          
  /// destructor 
  ~GlobalMuonTrajectoryBuilder();

  TC trajectories(const reco::TrackRef* staTrack,
                  const edm::Event&, 
                  const edm::EventSetup&);

  std::vector<reco::TrackRef*> chosenTrackerTrackRef() const;

  private:

    /// initialize algorithms
    void init();

    TC build(const reco::TrackRef* staTrack,
              const std::vector<Trajectory>& tkTrajs);

    /// choose a set of Track that match given standalone Track
    void chooseTrackerTracks(const reco::TrackRef* staTrack,
                             const reco::TrackCollection&) const;
 
    /// get silicon tracker Trajectories from track Track and Seed directly
    TC getTrackerTraj(const reco::TrackRef*) const;

    /// get silicon tracker Trajectories from track Track and Seed directly
    TC getTrackerTrajs(const TrajectoryFitter* theFitter,
                       const Propagator * thePropagator,
                       edm::OwnVector<const TransientTrackingRecHit>& hits,
                       TrajectoryStateOnSurface& theTSOS,
                       const TrajectorySeedCollection& seeds) const;

    /// get silicon tracker trajectories by local pattern recognition
    TC getTrackerTrajs(const reco::TrackRef&, int&, int&, int&, int&) const;

   void setES(const edm::EventSetup& setup,
              edm::ESHandle<TrackerGeometry>& theG,
              edm::ESHandle<MagneticField>& theMF,
              edm::ESHandle<TrajectoryFitter>& theFitter,
              edm::ESHandle<Propagator>& thePropagator,
              edm::ESHandle<TransientTrackingRecHitBuilder>& theBuilder);

    /// check muon RecHits
    void checkMuonHits(const reco::TrackRef&, RecHitContainer&, RecHitContainer&, std::vector<int>&) const;

    /// select muon RecHits
    RecHitContainer selectMuonHits(const Trajectory&, const std::vector<int>&) const;

    /// choose final trajectory
    const Trajectory* chooseTrajectory(const std::vector<Trajectory*>&) const;

    /// match two trajectories
    TC::const_iterator matchTrajectories(const Trajectory&, TC&) const;

    /// calculate chi2 probability (-ln(P))
    double trackProbability(const Trajectory&) const;    

    /// print all RecHits of a trajectory
    void printHits(const RecHitContainer&) const;


 private:
  
    CkfTrajectoryBuilder*  theTrajectoryBuilder;
    TrajectorySmoother* theTrajectorySmoother;
    TrajectoryCleaner*  theTrajectoryCleaner;
    GlobalMuonReFitter* theRefitter;

    ReconstructionDirection theDirection;
    int   theMuonHitsOption;
    float thePtCut;
    float theProbCut;
    int   theHitThreshold;
    float theDTChi2Cut;
    float theCSCChi2Cut;
    float theRPCChi2Cut;
    std::vector<reco::TrackRef*> theTkTrackRef; 

};
#endif
