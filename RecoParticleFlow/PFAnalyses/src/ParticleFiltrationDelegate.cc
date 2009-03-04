#include "RecoParticleFlow/PFAnalyses/interface/ParticleFiltrationDelegate.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

using namespace pftools;
using namespace edm;
using namespace std;

ParticleFiltrationDelegate::ParticleFiltrationDelegate() :
	particle_("Pi+-") {
	LogDebug("ParticleFiltrationDelegate") << __PRETTY_FUNCTION__ << std::endl;
	pdgCodes_.push_back(211);
	pdgCodes_.push_back(-211);

}

ParticleFiltrationDelegate::~ParticleFiltrationDelegate() {
	LogDebug("ParticleFiltrationDelegate") << __PRETTY_FUNCTION__ << std::endl;
}

bool ParticleFiltrationDelegate::isGoodParticleCore(edm::Event& event,
		const edm::EventSetup& setup) {
	LogProblem("ParticleFiltrationDelegate")
			<< "Subclass of ParticleFiltrationDelegate hasn't overridden isGoodParticleCore! Letting event pass anyway..."
			<< std::endl;
	return true;
}

