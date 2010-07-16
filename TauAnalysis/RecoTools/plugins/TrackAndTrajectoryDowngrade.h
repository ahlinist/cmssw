#ifndef TauAnalysis_RecoTools_TrackAndTrajectoryDowngrade_h
#define TauAnalysis_RecoTools_TrackAndTrajectoryDowngrade_h

/** \class TrackAndTrajectoryDowngrade
 *
 * Randomly kill certain fraction of reco::Track objects
 * (including associated Trajectory and TrackExtra objects)
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.3 $
 *
 * $Id: TrackAndTrajectoryDowngrade.h,v 1.3 2010/04/28 14:51:15 jkolb Exp $
 *
 */

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"

#include <TRandom3.h>

class TrackAndTrajectoryDowngrade : public edm::EDProducer 
{
 public:
  explicit TrackAndTrajectoryDowngrade(const edm::ParameterSet&);
  ~TrackAndTrajectoryDowngrade() {}
    
  void produce(edm::Event&, const edm::EventSetup&); 

 private:
  edm::InputTag src_;

  bool copyExtras_;
  bool makeReKeyedSeeds_;

  double pDowngrade_;
  
  TRandom3 u_;
};

#endif
