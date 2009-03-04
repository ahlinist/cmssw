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

	void getTags(const edm::ParameterSet& parameters) {
		getTagsCore(parameters);
	}

	/*
	 * Returns the collection in the event matching the Handle.
	 */
	template<class T> void getCollection(edm::Handle<T>& c,
			const edm::InputTag& tag, const edm::Event& event) const;

protected:
	virtual bool isGoodParticleCore(edm::Event& event,
			const edm::EventSetup& setup);

	virtual void getTagsCore(const edm::ParameterSet& parameters) {

	}

	/* PDG code of the particles we're looking for */
	std::vector<int> pdgCodes_;
	/* Human readable string of particles we're looking for */
	std::string particle_;

};

template<class T> void ParticleFiltrationDelegate::getCollection(
		edm::Handle<T>& c, const edm::InputTag& tag, const edm::Event& event) const {

	try {
		event.getByLabel(tag, c);
		if (!c.isValid()) {
			edm::LogProblem("ParticleFiltrationDelegate")
					<< "Warning! Collection for label " << tag
					<< " is not valid!" << std::endl;
		}
	} catch (cms::Exception& err) {
		edm::LogError("FilterDelegate") << "Couldn't get collection\n";
		throw err;

	}
}

}

#endif /* FILTERDELEGATE_H_ */
