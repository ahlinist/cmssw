#ifndef PFPRETENDTRACKPRODUCER_H_
#define PFPRETENDTRACKPRODUCER_H_

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "RecoParticleFlow/PFTracking/interface/PFTrackTransformer.h"

#include "RecoParticleFlow/PFAnalyses/interface/RunInfo.h"

#include <map>
#include <vector>

/**
 * @class PFPretendTrackProducer
 * @brief Creates tracks for the 2006 testbeam environment where no explicit tracking was available
 *
 * Uses the RunInfo object to determine what eta/phi to propagate a perfect track to, starting
 * from the experimental origin (0, 0, 0)
 *
 * In 2006, the beam position was known to within 4 x 4 cm via veto counters. So this approach is fine
 * for any HCAL granularity study, but less so for ECAL.
 *
 * @author Jamie Ballin
 * @date April 2009
 */
class PFPretendTrackProducer : public edm::EDProducer {
public:
	PFPretendTrackProducer(const edm::ParameterSet&);
	virtual ~PFPretendTrackProducer();

private:
	virtual void beginJob(const edm::EventSetup&);
	virtual void endJob();
	virtual void produce(edm::Event&, const edm::EventSetup&);
	int debug_;
	pftools::RunInfo* thisRun_;

	PFTrackTransformer* transformer_;

	std::map<unsigned, pftools::RunInfo*> runInfos_;

	bool justCreateEmptyCollections_;

};



#endif /*PFPRETENDTRACKPRODUCER_H_*/
