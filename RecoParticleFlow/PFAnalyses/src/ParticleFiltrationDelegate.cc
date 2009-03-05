#include "RecoParticleFlow/PFAnalyses/interface/ParticleFiltrationDelegate.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

using namespace pftools;
using namespace edm;
using namespace std;


ParticleFiltrationDelegate::ParticleFiltrationDelegate() :
	particle_("Pi+-"), thisEventPasses_(true), debug_(0), nPasses_(0), nFails_(0) {
	LogDebug("ParticleFiltrationDelegate") << __PRETTY_FUNCTION__ << std::endl;
	pdgCodes_.push_back(211);
	pdgCodes_.push_back(-211);

}

ParticleFiltrationDelegate::~ParticleFiltrationDelegate() {
	LogDebug("ParticleFiltrationDelegate") << __PRETTY_FUNCTION__ << std::endl;
}

void ParticleFiltrationDelegate::getTags(const edm::ParameterSet& parameters) {
	LogDebug("ParticleFiltrationDelegate") << __PRETTY_FUNCTION__ << std::endl;
	debug_ = parameters.getParameter<int> ("debug");
	getTagsCore(parameters);

}

