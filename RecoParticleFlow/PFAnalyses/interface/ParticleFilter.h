#ifndef PARTICLEFILTER_H_
#define PARTICLEFILTER_H_

#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "RecoParticleFlow/PFAnalyses/interface/ParticleFiltrationDelegate.h"

#include "UserCode/JamieBallinDictionary/interface/ParticleFiltrationDecision.h"

class ParticleFilter: public edm::EDFilter {

public:
	explicit ParticleFilter(const edm::ParameterSet&);
	virtual ~ParticleFilter();

private:
	virtual void beginJob(const edm::EventSetup&);
	virtual bool filter(edm::Event&, const edm::EventSetup&);
	virtual void endJob();

	pftools::ParticleFiltrationDelegate* pfd_;


	std::string pfdType_;
	std::string tbfdType_;

};

#endif /* PARTICLEFILTER_H_ */

