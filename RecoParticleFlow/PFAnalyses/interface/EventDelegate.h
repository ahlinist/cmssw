#ifndef EVENTDELEGATE_H_
#define EVENTDELEGATE_H_

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/ParticleFlowReco/interface/PFSimParticle.h"
#include "DataFormats/ParticleFlowReco/interface/PFSimParticleFwd.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecTrackFwd.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecHit.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecHitFwd.h"
#include "DataFormats/ParticleFlowReco/interface/PFClusterFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"

#include "DataFormats/ParticleFlowReco/interface/Calibratable.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "RecoParticleFlow/PFAnalyses/interface/operations.h"

#include <map>
#include <string>
#include <vector>
#include <iostream>

#include <TTree.h>
#include <TH2F.h>

class EventDelegate {
public:

	virtual ~EventDelegate();

	//Call this straight after construction
	virtual void init(TTree* tree, const edm::ParameterSet& parameters);

	virtual void init(const edm::ParameterSet& parameters);

	void startEvent(const edm::Event& event, const edm::EventSetup& setup,
			std::vector<pftools::Calibratable>* eventCalibs = 0);

	/*
	 * Subclasses may generally record several particles from one event.
	 * They should call this at the start of each one.
	 */
	void startParticle() {
		startParticleCore();
	}

	/*
	 * Subclasses should of course override this.
	 * Returns true if the event was 'valid' according to whatever vetos have been set.
	 */
	virtual bool processEvent(const edm::Event& event,
			const edm::EventSetup& setup) {
		return true;
	}

	bool endEvent();

	/*
	 * Not yet implemented.
	 */
	bool getVeto(const std::string param) const {
		//return vetos_(param);
		std::cout << __PRETTY_FUNCTION__
				<< ": Not yet implemented! Returning true." << std::endl;
		return true;
	}

	void setVeto(const std::string param, bool onOff) {
		vetos_[param] = onOff;
	}

	void endParticle();

	/*
	 * General cleanup for last event.
	 */
	virtual bool finish() {
		return true;
	}

	void getTags(const edm::ParameterSet& parameters);


protected:
	//Subclasses should override these methods to request their own tags
	virtual void getTagsCore(const edm::ParameterSet& parameters) {

	}

	//Subclasses should override these methods.
	virtual void startEventCore(const edm::Event& event,
			const edm::EventSetup& setup) {
	}

	virtual bool endEventCore();

	virtual void startParticleCore() {
	}

	virtual void endParticleCore() {
	}

	virtual void initCore(const edm::ParameterSet& parameters) {

	}

	EventDelegate();

	/*
	 * Returns true if:
	 * 		origin - bound <= test < origin + bound
	 */
	bool within(double origin, double bound, double test);

//	double deltaR(const double& eta1, const double& eta2, const double& phi1,
//			const double& phi2);

	//Debug?
	int debug_;
	//True if this event was generally successful
	bool thisEventPasses_;
	bool thisParticlePasses_;
	//Simple on/off vetos
	std::map<std::string, bool> vetos_;


	std::vector<pftools::Calibratable>* thisEventCalibs_;
	//Some diagnostic histograms;
	TH2F* hClusterCount_;

	edm::Service<TFileService> fileservice_;

	//The Calibratable object
	pftools::Calibratable* calib_;
	//Increment nWrites_ for every record written to the tree
	//Incremement nFails_ for every event/particle not written to the tree (thisEventPasses_ == false)
	unsigned nWrites_, nFails_;
	unsigned nParticleWrites_, nParticleFails_;

	//The Calibratable tree
	TTree* tree_;

};



#endif /*EVENTDELEGATE_H_*/
