#include "RecoParticleFlow/PFAnalyses/interface/ParticleFilter.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

using namespace std;
using namespace edm;

using namespace pftools;

ParticleFilter::ParticleFilter(const edm::ParameterSet& params) {
	LogDebug("ParticleFilter") << __PRETTY_FUNCTION__ << std::endl;

}

ParticleFilter::~ParticleFilter() {
	LogDebug("ParticleFilter") << __PRETTY_FUNCTION__ << std::endl;

}


void ParticleFilter::beginJob(const edm::EventSetup& setup) {

}

bool ParticleFilter::filter(edm::Event& event, const edm::EventSetup& setup) {
	LogDebug("ParticleFilter") << __PRETTY_FUNCTION__ << std::endl;


	LogInfo("ParticleFilter") << "\tBlindly accepting event as a pion candidate\n";
	return true;
}

void ParticleFilter::endJob() {

}

//define this as a plug-in
DEFINE_FWK_MODULE(ParticleFilter);

