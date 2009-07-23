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
#include "DataFormats/ParticleFlowReco/interface/PFTrack.h"
#include "SimDataFormats/CaloHit/interface/PCaloHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/ParticleFlowReco/interface/PFTrajectoryPoint.h"

#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/EcalAlgo/interface/EcalEndcapGeometry.h"
#include "Geometry/EcalAlgo/interface/EcalBarrelGeometry.h"

#include "DataFormats/ParticleFlowReco/interface/Calibratable.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "RecoParticleFlow/PFAnalyses/interface/operations.h"
#include "RecoParticleFlow/PFAnalyses/interface/Initialisable.h"

#include <map>
#include <string>
#include <vector>
#include <iostream>

#include <TTree.h>
#include <TH2F.h>
namespace pftools {

/**
 * @class EventDelegate
 * @brief Workhorse base class to extract salient information about the event and put it in 'Calibratable'
 *
 * Implementations should implement processEvent and all the core methods in the protected domain.
 * For each particle deemed worthy of a Calibratable instance, call startParticle. Fill the
 * calibratable with the pointer 'calib_', and when done, call endParticle. Repeat as necessary.
 *
 * Set thisEventPasses_ if all Calibratables found in this event should not be written to the tree/event
 * Set thisParticlePasses_ if that particular Calibratable should not be written to the tree/event.
 *
 *
 * @author Jamie Ballin
 * @date April 2009
 *
 *
 */
class EventDelegate: public pftools::Initialisable {
public:

	virtual ~EventDelegate();

	//Call this straight after construction
	virtual void init(TTree* tree, const edm::ParameterSet& parameters);

	//Call this straight after construction
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
			const edm::EventSetup& setup) = 0;

	bool endEvent();

	void endParticle();

	/*
	 * General cleanup for last event.
	 */
	virtual bool finish();

protected:

	//Subclasses should override these methods to request their own tags
	virtual void initCore(const edm::ParameterSet& parameters) = 0;

	//Subclasses should override these methods.
	virtual void startEventCore(const edm::Event& event,
			const edm::EventSetup& setup) = 0;

	virtual bool endEventCore() = 0;

	virtual void startParticleCore() = 0;

	/**
	 * THIS WON'T WORK PROPERLY FOR EDPRODUCERS!
	 */
	virtual void endParticleCore() = 0;

	EventDelegate();

	/*
	 * Selects primary sim particles from the event, vetoing any that don't meet certain criteria.
	 */
	std::vector<unsigned> findPrimarySimParticles(const std::vector<
			reco::PFSimParticle>& sims);

	std::vector<unsigned> findCandidatesInDeltaR(const reco::PFTrack& pft,
			const std::vector<reco::PFCandidate>& cands, const double& deltaR);

	template<typename T> std::vector<unsigned> findObjectsInDeltaR(
			const reco::PFTrack& pft, const std::vector<T>& objects,
			const double deltaR);

	unsigned findClosestRecTrack(const reco::PFSimParticle& sim,
			const std::vector<reco::PFRecTrack>& tracks);

	double findSimToTrackDeltaR(const reco::PFSimParticle& sim,
			const reco::PFTrack& track);

	/*
	 * Worker method to fill the Calibratable with Candidate information.
	 */
	void extractCandidate(const reco::PFCandidate& cand);

	void extractTrack(const reco::PFTrack& track);

	//
	//	virtual void extractRawRecHits();

	virtual void extractEcalPFRecHits(
			const std::vector<reco::PFRecHit>& ecalRechits, const std::vector<
					unsigned>& indices);

	virtual void extractHcalPFRecHits(
			const std::vector<reco::PFRecHit>& hcalRechits, const std::vector<
					unsigned>& indices);

	virtual void extractEcalPFClusters(
			const std::vector<reco::PFCluster>& ecalClusters,
			const std::vector<unsigned>& indices);

	virtual void extractHcalPFClusters(
			const std::vector<reco::PFCluster>& hcalClusters,
			const std::vector<unsigned>& indices);

	virtual void extractEBRecHits(const EcalRecHitCollection& ecalRechits,
			const EcalBarrelGeometry* geometry, double targetEta = 0.,
			double targetPhi = 0.);

	virtual void extractEERecHits(const EcalRecHitCollection& ecalRechits,
			const EcalEndcapGeometry* geometry, double targetEta = 0.,
			double targetPhi = 0.);

	virtual void extractHcalRecHits(const HBHERecHitCollection& hcalRechits,
			const CaloSubdetectorGeometry* geometry, double targetEta = 0.,
			double targetPhi = 0.);

	//	virtual void extractPFClusters();

	/*
	 * Worker method to fill the Calibratable with sim information.
	 */
	void extractSimParticle(const reco::PFSimParticle& sim, unsigned index);

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
	bool noSimDaughters_;
	bool isMC_;
	bool useSimAsTrack_;
	bool clustersFromCandidates_;
	bool rechitsFromCandidates_;
	bool neutralMode_;

	double deltaRCandToTrack_;
	double deltaRRechitsToTrack_;
	double deltaRClustersToTrack_;

	unsigned nPanesEcalCaloWindow_;
	unsigned nPanesHcalCaloWindow_;
	unsigned nRingsEcalCaloWindow_;
	unsigned nRingsHcalCaloWindow_;
	double deltaREcalCaloWindow_;
	double deltaRHcalCaloWindow_;

	//this is useful for extensibility or subclassing :-)
	int particlePdg_;

	//Some diagnostic histograms;
	TH2F* hClusterCount_;

	edm::Service<TFileService> fileservice_;

	//The current Calibratable object
	pftools::Calibratable* calib_;
	std::vector<pftools::Calibratable>* thisEventCalibs_;
	//Increment nWrites_ for every record written to the tree
	//Incremement nFails_ for every event/particle not written to the tree (thisEventPasses_ == false)
	unsigned nWrites_, nFails_;
	unsigned nParticleWrites_, nParticleFails_;

	//The Calibratable tree
	TTree* tree_;

private:
	void getTags(const edm::ParameterSet& parameters);

};

template<typename T> std::vector<unsigned> EventDelegate::findObjectsInDeltaR(
		const reco::PFTrack& pft, const std::vector<T>& objects,
		const double deltaRCut) {
	using namespace edm;
	using namespace reco;

	unsigned index(0);
	std::vector<unsigned> answers;
	for (typename std::vector<T>::const_iterator oit = objects.begin(); oit
			!= objects.end(); ++oit) {
		T obj = *oit;
		double rhEta = obj.positionREP().eta();
		double rhPhi = obj.positionREP().phi();
		//TODO: add trajectory point as method argument
		double
				trEta =
						pft.trajectoryPoint(PFTrajectoryPoint::ECALEntrance).positionREP().Eta();
		double
				trPhi =
						pft.trajectoryPoint(PFTrajectoryPoint::ECALEntrance).positionREP().Phi();

		if (pftools::deltaR(rhEta, trEta, rhPhi, trPhi) < deltaRCut) {
			//accept
			answers.push_back(index);
		}
		++index;
	}
	return answers;
}



}

#endif /*EVENTDELEGATE_H_*/
