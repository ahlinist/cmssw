#ifndef RecoMuon_TrackingTools_MuonTrajectoryBuilder_H
#define RecoMuon_TrackingTools_MuonTrajectoryBuilder_H

/** \class MuonTrajectoryBuilder
 *  Base class for the Muon reco Trajectory Builder 
 *
 *  $Date: 2006/05/23 15:07:49 $
 *  $Revision: 1.4 $
 *  \author R. Bellan - INFN Torino
 */

#include "TrackingTools/PatternTools/interface/Trajectory.h"
#include <vector>

namespace edm {class ParameterSet; class EventSetup; class Event;}
class TrajectorySeed;

class MuonTrajectoryBuilder {
 public:
  typedef std::vector<Trajectory> TrajectoryContainer;
 public:
  
  /// Constructor with Parameter set
  MuonTrajectoryBuilder() {};
  MuonTrajectoryBuilder(const edm::ParameterSet& ) {};

  /// Destructor
  virtual ~MuonTrajectoryBuilder(){};

  /// Returns a vector of the reconstructed trajectories compatible with
  ///  the given seed.
      
  virtual TrajectoryContainer trajectories(const TrajectorySeed&) = 0;

  /// Pass the Event Setup to the algo at each event
  virtual void setES(const edm::EventSetup& setup) = 0;

  /// Pass the Event to the algo at each event
  virtual void setEvent(const edm::Event& event) = 0;
  
 private:
  
 protected:
  

};
#endif
