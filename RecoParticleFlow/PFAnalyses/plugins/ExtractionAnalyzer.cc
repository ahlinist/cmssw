#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/ParticleFlowReco/interface/PFClusterFwd.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecHitFwd.h"
#include "DataFormats/ParticleFlowReco/interface/PFSimParticleFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

#include "FWCore/Framework/interface/ESHandle.h"

#include "RecoParticleFlow/PFAnalyses/interface/ExtractionAnalyzer.h"
#include "RecoParticleFlow/PFAnalyses/interface/EventDelegate.h"
#include "RecoParticleFlow/PFAnalyses/interface/DipionDelegate.h"
#include "RecoParticleFlow/PFAnalyses/interface/TestbeamDelegate.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
//#include "RecoParticleFlow/PFAnalyses/interface/TestDelegate.h"

#include <iostream>
#include <exception>
#include <string>

using namespace std;
using namespace edm;
using namespace reco;
using namespace pftools;

ExtractionAnalyzer::ExtractionAnalyzer(const edm::ParameterSet& parameters) :
	dptype_("DipionDelegate"), tbtype_("TestbeamDelegate") {
	LogInfo("ExtractionAnalyzer")  << __PRETTY_FUNCTION__ << "\n";
	edtype_ = parameters.getParameter<std::string>("EventDelegateType");
	if (edtype_ == dptype_) {
		ed_ = new DipionDelegate();
	} else if (edtype_ == tbtype_) {
		ed_ = new TestbeamDelegate();
//	} else if(edtype_ == testtype_) {
//		ed_ = new TestDelegate();
	} else {
		LogError("ExtractionAnalyzer") << "Couldn't find suitable delegate!" << std::endl;
		//Exception e(msg);
		//throw e;
	}
	tree_ = fileservice_->make<TTree>("Extraction", "");

	ed_->init(tree_, parameters);
}

ExtractionAnalyzer::~ExtractionAnalyzer() {
	LogDebug("ExtractionAnalyzer") << __PRETTY_FUNCTION__ << std::endl;
	delete ed_;
	LogInfo("ExtractionAnalyzer")  << "\tExtractionAnalyzer wishes you a nice day."<< std::endl;
	//delete outputfile_;
}

template<typename T, typename A> T* ExtractionAnalyzer::makeRootObject(const A& arg) {
	T* t = fileservice_->make<T>(arg);
	return t;
}

template<typename T> T* ExtractionAnalyzer::makeRootObject() {
	return fileservice_->make<T>();
}

void ExtractionAnalyzer::beginJob(const edm::EventSetup& setup) {

}

void ExtractionAnalyzer::analyze(const edm::Event& event,
		const edm::EventSetup& setup) {
	ed_->startEvent(event, setup);
	ed_->processEvent(event, setup);
	ed_->endEvent();

}

void ExtractionAnalyzer::endJob() {
	LogInfo("ExtractionAnalyzer")  << __PRETTY_FUNCTION__ << "\n";
	ed_->finish();
}


//define this as a plug-in
DEFINE_FWK_MODULE(ExtractionAnalyzer);

