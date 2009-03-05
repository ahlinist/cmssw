#ifndef PFPRETENDTRACKPRODUCER_H_
#define PFPRETENDTRACKPRODUCER_H_

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "RecoParticleFlow/PFTracking/interface/PFTrackTransformer.h"

#include "UserCode/JamieBallinDictionary/interface/RunInfo.h"

#include <map>
#include <vector>

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

};



#endif /*PFPRETENDTRACKPRODUCER_H_*/
