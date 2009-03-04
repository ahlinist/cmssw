#ifndef FILTERDELEGATE_H_
#define FILTERDELEGATE_H_

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <string>

namespace pftools {

class ParticleFiltrationDelegate {
public:
	ParticleFiltrationDelegate();
	virtual ~ParticleFiltrationDelegate();

	bool isGoodParticle(edm::Event& event, const edm::EventSetup& setup) {
		return isGoodParticleCore(event, setup);
	}

	void getTags(const edm::ParameterSet& parameters);

	void init(const edm::ParameterSet& parameters) {
		initCore(parameters);
	}

	void finish() {
		finishCore();
	}

protected:
	virtual bool isGoodParticleCore(edm::Event& event,
			const edm::EventSetup& setup);

	virtual void getTagsCore(const edm::ParameterSet& parameters) {

	}

	virtual void initCore(const edm::ParameterSet& parameters) {

	}

	virtual void finishCore() {
		edm::LogInfo("ParticleFiltrationDelegate") << __PRETTY_FUNCTION__
				<< ": hasn't been overwritten by subclass.\n";
	}

	/* PDG code of the particles we're looking for */
	std::vector<int> pdgCodes_;
	/* Human readable string of particles we're looking for */
	std::string particle_;

	bool thisEventPasses_;
	int debug_;

	int nPasses_, nFails_;

};

}

#endif /* FILTERDELEGATE_H_ */
