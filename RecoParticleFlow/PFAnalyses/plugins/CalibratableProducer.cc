#include "RecoParticleFlow/PFAnalyses/interface/CalibratableProducer.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/ParticleFlowReco/interface/PFClusterFwd.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecHitFwd.h"
#include "DataFormats/ParticleFlowReco/interface/PFSimParticleFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

#include "FWCore/Framework/interface/ESHandle.h"

#include "RecoParticleFlow/PFAnalyses/interface/EventDelegate.h"
#include "RecoParticleFlow/PFAnalyses/interface/DipionDelegate.h"
#include "RecoParticleFlow/PFAnalyses/interface/TestbeamDelegate.h"

#include <iostream>
#include <exception>
#include <string>

using namespace std;
using namespace edm;
using namespace reco;
using namespace pftools;

CalibratableProducer::CalibratableProducer(const edm::ParameterSet& parameters) :
	dptype_("DipionDelegate"), tbtype_("TestbeamDelegate") {

	produces<std::vector<pftools::Calibratable> > ();

	edtype_ = parameters.getParameter<std::string> ("EventDelegateType");
	if (edtype_ == dptype_) {
		ed_ = new DipionDelegate();
	} else if (edtype_ == tbtype_) {
		ed_ = new TestbeamDelegate();
	} else {
		LogError("CalibratableProducer")
				<< " Couldn't find a suitable delegate! You asked for "
				<< edtype_ << std::endl;
		//Exception e(msg);
		//throw e;
	}

	ed_->init(parameters);

}

CalibratableProducer::~CalibratableProducer() {
	delete ed_;
	LogInfo("CalibratableProducer") << "... wishes you a nice day."
			<< std::endl;
}

void CalibratableProducer::beginJob(const edm::EventSetup& setup) {

}

void CalibratableProducer::produce(edm::Event& event,
		const edm::EventSetup& setup) {
	//create the empty collections
	std::auto_ptr<std::vector<pftools::Calibratable> > calibColl(
			new std::vector<pftools::Calibratable>());
	ed_->startEvent(event, setup, calibColl.get());
	ed_->processEvent(event, setup);
	ed_->endEvent();
	event.put(calibColl);

}

void CalibratableProducer::endJob() {
	ed_->finish();
}

//define this as a plug-in
DEFINE_FWK_MODULE( CalibratableProducer);

