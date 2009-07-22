#ifndef FILTERDELEGATE_H_
#define FILTERDELEGATE_H_

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/ParticleFlowReco/interface/ParticleFiltrationDecision.h"

#include <string>

namespace pftools {

/**
 * @class ParticleFiltrationDelegate
 * @brief Abstract class to filter events based on various quality criteria
 *
 * Specifically devloped for testbeam appications.
 *
 * @author Jamie Ballin
 * @date April 2009
 *
 * @since CMSSW 3_1
 */
class ParticleFiltrationDelegate {
public:
	ParticleFiltrationDelegate();
	virtual ~ParticleFiltrationDelegate();

	ParticleFiltrationDecisionCollection isGoodParticle(edm::Event& event, const edm::EventSetup& setup) {
		return isGoodParticleCore(event, setup);
	}

	void getTags(const edm::ParameterSet& parameters);

	void init(const edm::ParameterSet& parameters) {
		getTags(parameters);
		initCore(parameters);
	}

	void startEvent(const edm::Event& event,
			const edm::EventSetup& setup) {
		return startEventCore(event, setup);
	}

	bool endEvent(const edm::Event& event) {
		return endEventCore(event);
	}

	void finish() {
		finishCore();
	}

protected:

	virtual void startEventCore(const edm::Event& event,
			const edm::EventSetup& setup) = 0;

	virtual bool endEventCore(const edm::Event& event) = 0;

	virtual ParticleFiltrationDecisionCollection isGoodParticleCore(edm::Event& event,
			const edm::EventSetup& setup) = 0;

	virtual void getTagsCore(const edm::ParameterSet& parameters) = 0;

	virtual void initCore(const edm::ParameterSet& parameters) = 0;

	virtual void finishCore() = 0;

	/* PDG code of the particles we're looking for */
	std::vector<int> pdgCodes_;
	/* Human readable string of particles we're looking for */
	std::string particle_;

	bool thisEventPasses_;
	int debug_;

	int nPasses_, nFails_;

	pftools::ParticleFiltrationDecision decision_;

};

}

#endif /* FILTERDELEGATE_H_ */
