#include "RecoParticleFlow/PFAnalyses/interface/ParticleFilter.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "RecoParticleFlow/PFAnalyses/interface/TestbeamFiltrationDelegate.h"

using namespace std;
using namespace edm;

using namespace pftools;

ParticleFilter::ParticleFilter(const edm::ParameterSet& parameters) :
	pfd_(0), tbfdType_("TestbeamFiltrationDelegate") {
	LogDebug("ParticleFilter") << __PRETTY_FUNCTION__ << std::endl;

	pfdType_ = parameters.getParameter<std::string> (
			"ParticleFiltrationDelegateType");
	if (pfdType_ == tbfdType_) {
		pfd_ = new TestbeamFiltrationDelegate();

	} else {
		LogError("ExtractionAnalyzer") << "Couldn't find suitable delegate!"
				<< std::endl;
		//Exception e(msg);
		//throw e;
	}
	if (pfd_ != 0)
		pfd_->init(parameters);

}

ParticleFilter::~ParticleFilter() {
	LogDebug("ParticleFilter") << __PRETTY_FUNCTION__ << std::endl;

}

void ParticleFilter::beginJob(const edm::EventSetup& setup) {

}

bool ParticleFilter::filter(edm::Event& event, const edm::EventSetup& setup) {
	LogDebug("ParticleFilter") << __PRETTY_FUNCTION__ << std::endl;

	LogInfo("ParticleFilter")
			<< "\tBlindly accepting event as a pion candidate\n";
	return true;
}

void ParticleFilter::endJob() {

}

//define this as a plug-in
DEFINE_FWK_MODULE( ParticleFilter);

