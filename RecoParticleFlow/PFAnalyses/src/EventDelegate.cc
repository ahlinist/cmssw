#include "RecoParticleFlow/PFAnalyses/interface/EventDelegate.h"

#include <iostream>

using namespace pftools;
using namespace edm;
using namespace reco;

EventDelegate::EventDelegate() :
	thisEventPasses_(false), nWrites_(0), nFails_(0), nParticleWrites_(0), nParticleFails_(0) {
	if(debug_)
		std::cout << "Leaving "<< __PRETTY_FUNCTION__ << std::endl;
}

EventDelegate::~EventDelegate() {
	if(debug_)
		std::cout << "Leaving "<< __PRETTY_FUNCTION__ << std::endl;
}

void EventDelegate::init(TTree* tree, const edm::ParameterSet& parameters) {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	tree_ = tree;
	calib_ = new Calibratable();
	tree_->Branch("Calibratable", "pftools::Calibratable", &calib_, 32000, 2);
	getTags(parameters);
	std::cout << "\tEventDelegate initialiation complete.\n";
	initCore(parameters);

}

void EventDelegate::getTags(const edm::ParameterSet& parameters) {
	std::cout << __PRETTY_FUNCTION__ << std::endl;

	debug_ = parameters.getParameter<int>("debug");
	
	getTagsCore(parameters);

}

void EventDelegate::startEvent(const edm::Event& event,
		const edm::EventSetup& setup) {
	if(debug_ > 3)
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	thisEventPasses_ = true;
	thisParticlePasses_ = true;
	calib_->reset();

	startEventCore(event, setup);
}


bool EventDelegate::endEventCore() {
	return false;
}

void EventDelegate::endParticle() {
	endParticleCore();

}

bool EventDelegate::endEvent() {
	if(debug_ > 3)
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	bool ans = endEventCore();


	
	return ans;
}

bool EventDelegate::within(double origin, double bound, double test) {
	if (test < (origin + bound)) {
		if (test > (origin - bound)) {
			return true;
		}
	}
	return false;
}

double EventDelegate::deltaR(const double& eta1, const double& eta2,
		const double& phi1, const double& phi2) {
	double deltaEta = fabs(eta1 - eta2);
	double deltaPhi = fabs(phi1 - phi2);
	if (deltaPhi > M_PI) {
		deltaPhi = 2 * M_PI- deltaPhi;
	}
	return sqrt(pow(deltaEta, 2) + pow(deltaPhi, 2));
}


