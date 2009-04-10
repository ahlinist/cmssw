#ifndef DIPIONDELEGATE_H_
#define DIPIONDELEGATE_H_

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/ParticleFlowReco/interface/PFSimParticleFwd.h"
#include "DataFormats/ParticleFlowReco/interface/PFTrack.h"

#include "SimDataFormats/CaloHit/interface/PCaloHit.h"

#include "RecoParticleFlow/PFAnalyses/interface/EventDelegate.h"

#include <iostream>
#include <vector>
#include <utility>



class DipionDelegate : public EventDelegate {

public:
	DipionDelegate(bool isMC);

	virtual ~DipionDelegate() {
		LogDebug("DipionDelegate") << "Leaving "<< __PRETTY_FUNCTION__ << std::endl;
	}

	//The main workhorse
	virtual bool processEvent(const edm::Event& event,
			const edm::EventSetup& setup);

	virtual bool finish();

protected:

	/*
	 * Matches PFCandidates to Sim Particles in dipion events, but making sure
	 * they are within deltaEta_, deltaPhi_ of each other at the ECAL surface.
	 *
	 * Returns a vector of pairs of unsigneds - the indices in the input vector.
	 */
	std::vector<std::pair<unsigned, unsigned > > associate(
			const std::vector<reco::PFSimParticle>& sims,
			const std::vector<reco::PFCandidate>& cands);

	//Checks vetos
	//Return true if you want the particle written to the tree
	virtual bool endEventCore();

	virtual void startEventCore(const edm::Event& event,
			const edm::EventSetup& setup);

	virtual void startParticleCore();

	virtual void endParticleCore();

	/*
	 * Retrieves the tags listed below.
	 */
	virtual void getTagsCore(const edm::ParameterSet& parameters);

private:


	//Collection input tags
	edm::InputTag inputTagCandidates_;
	edm::InputTag inputTagSimParticles_;
	edm::InputTag inputTagClustersEcal_;
	edm::InputTag inputTagClustersHcal_;
	edm::InputTag inputTagRecHitsEcal_;
	edm::InputTag inputTagRecHitsHcal_;
	edm::InputTag inputTagRecTracks_;
	edm::InputTag inputTagCaloHitsEcalEB_;
	edm::InputTag inputTagCaloHitsEcalEE_;
	edm::InputTag inputTagCaloHitsHcal_;


	//collections
	edm::Handle<reco::PFCandidateCollection>* pfCandidates_;
	edm::Handle<reco::PFSimParticleCollection>* simParticles_;
	edm::Handle<reco::PFClusterCollection>* clustersEcal_;
	edm::Handle<reco::PFClusterCollection>* clustersHcal_;
	edm::Handle<reco::PFRecHitCollection>* recHitsEcal_;
	edm::Handle<reco::PFRecHitCollection>* recHitsHcal_;
	edm::Handle<reco::PFRecTrackCollection>* recTracks_;

	edm::Handle<std::vector<PCaloHit> >* simCaloHitsEcalEB_;
	edm::Handle<std::vector<PCaloHit> >* simCaloHitsEcalEE_;
	edm::Handle<std::vector<PCaloHit> >* simCaloHitsHcal_;

	//Extra tags
	bool isMC_;
	bool useSimAsTrack_;
	bool clustersFromCandidates_;
	bool rechitsFromCandidates_;
	bool neutralMode_;
	bool noSimDaughters_;
	double deltaEta_;
	double deltaPhi_;
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
	int pionPdg_;

	/*
	 * Worker method to fill the Calibratable with sim information.
	 */
	void extractSimParticle(const reco::PFSimParticle& sim, unsigned index);

	/*
	 * Selects primary sim particles from the event, vetoing any that don't meet certain criteria.
	 */
	std::vector<unsigned> findPrimarySimParticles(
			const std::vector<reco::PFSimParticle>& sims);

	std::vector<unsigned> findCandidatesInDeltaR(const reco::PFTrack& pft,
			const std::vector<reco::PFCandidate>& cands, const double& deltaR);

	template<typename T> std::vector<unsigned> findObjectsInDeltaR(const reco::PFTrack& pft,
			const std::vector<T>& objects, const double& deltaR);

	unsigned findClosestRecTrack(const reco::PFSimParticle& sim,
			const std::vector<reco::PFRecTrack>& tracks);

	double findSimToTrackDeltaR(const reco::PFSimParticle& sim,
			const reco::PFTrack& track);

	/*
	 * Worker method to fill the Calibratable with Candidate information.
	 */
	void extractCandidate(const reco::PFCandidate& cand);

	void extractTrack(const reco::PFTrack& track);


};


#endif /*DIPIONDELEGATE_H_*/
