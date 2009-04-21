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
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"

#include "RecoParticleFlow/PFAnalyses/interface/EventDelegate.h"

#include <iostream>
#include <vector>
#include <utility>



class DipionDelegate : public EventDelegate {

public:
	DipionDelegate();

	virtual ~DipionDelegate() {
		LogDebug("DipionDelegate") << "Leaving "<< __PRETTY_FUNCTION__ << std::endl;
	}

	//The main workhorse
	virtual bool processEvent(const edm::Event& event,
			const edm::EventSetup& setup);


protected:

	/*
	 * Matches PFCandidates to Sim Particles in dipion events, but making sure
	 * they are within deltaEta_, deltaPhi_ of each other at the ECAL surface.
	 *
	 * Returns a vector of pairs of unsigneds - the indices in the input vector.
	 */
//	std::vector<std::pair<unsigned, unsigned > > associate(
//			const std::vector<reco::PFSimParticle>& sims,
//			const std::vector<reco::PFCandidate>& cands);

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
	virtual void initCore(const edm::ParameterSet& parameters);

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
	edm::InputTag inputTagRawHitsEcalEB_;
	edm::InputTag inputTagRawHitsEcalEE_;
	edm::InputTag inputTagRawHitsHcal_;


	//collections
	edm::Handle<reco::PFCandidateCollection>* pfCandidates_;
	edm::Handle<reco::PFSimParticleCollection>* simParticles_;
	edm::Handle<reco::PFClusterCollection>* clustersEcal_;
	edm::Handle<reco::PFClusterCollection>* clustersHcal_;
	edm::Handle<reco::PFRecHitCollection>* recHitsEcal_;
	edm::Handle<reco::PFRecHitCollection>* recHitsHcal_;
	edm::Handle<reco::PFRecTrackCollection>* recTracks_;

	edm::Handle<EcalRecHitCollection>* rawRecHitsEcalEB_;
	edm::Handle<EcalRecHitCollection>* rawRecHitsEcalEE_;
	edm::Handle<HBHERecHitCollection>* rawRecHitsHcal_;

	edm::Handle<std::vector<PCaloHit> >* simCaloHitsEcalEB_;
	edm::Handle<std::vector<PCaloHit> >* simCaloHitsEcalEE_;
	edm::Handle<std::vector<PCaloHit> >* simCaloHitsHcal_;


};


#endif /*DIPIONDELEGATE_H_*/
