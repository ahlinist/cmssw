/*
 * CollisionDelegate.h
 *
 *  Created on: 02-Dec-2009
 *      Author: jamie
 */

#ifndef COLLISIONDELEGATE_H_
#define COLLISIONDELEGATE_H_


#include "RecoParticleFlow/PFAnalyses/interface/EventDelegate.h"
#include "RecoParticleFlow/PFAnalyses/interface/RunInfo.h"
#include "TBDataFormats/HcalTBObjects/interface/HcalTBBeamCounters.h"
#include "TBDataFormats/HcalTBObjects/interface/HcalTBEventPosition.h"
#include "TBDataFormats/HcalTBObjects/interface/HcalTBRunData.h"
#include "TBDataFormats/HcalTBObjects/interface/HcalTBTiming.h"
#include "TBDataFormats/HcalTBObjects/interface/HcalTBTriggerData.h"
#include "DataFormats/ParticleFlowReco/interface/PFCluster.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitFwd.h"
#include "DataFormats/HcalRecHit/interface/HBHERecHit.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/ParticleFlowReco/interface/ParticleFiltrationDecision.h"

#include "RecoParticleFlow/PFAnalyses/interface/IsolatedParticleExtractor.h"

#include <map>

#include <TH1.h>
#include <TH1F.h>

namespace pftools {

class CollisionDelegate : public EventDelegate {
public:
	CollisionDelegate();
	virtual ~CollisionDelegate();

	//The main workhorse
	virtual bool processEvent(const edm::Event& event,
			const edm::EventSetup& setup);


protected:
	virtual void initCore(const edm::ParameterSet& parameters);

	virtual void startEventCore(const edm::Event& event,
			const edm::EventSetup& setup);

	virtual bool endEventCore();

	virtual void startParticleCore();

	virtual void endParticleCore();

	bool isMC_;



	pftools::IsolatedParticleExtractor* ipe_;


	edm::InputTag inputTagRawRecHitsEcalEB_;
	edm::InputTag inputTagRawRecHitsEcalEE_;
	edm::InputTag inputTagRawRecHitsEcalES_;
	edm::InputTag inputTagRawRecHitsHcal_;
	edm::InputTag inputTagClustersEcal_;
	edm::InputTag inputTagClustersHcal_;
	edm::InputTag inputTagRecHitsEcal_;
	edm::InputTag inputTagRecHitsHcal_;
	edm::InputTag inputTagRecTracks_;
	edm::InputTag inputTagSimParticles_; // MC only, :-)
	edm::InputTag inputTagPFCandidates_;


	edm::Handle<EcalRecHitCollection>* rawRecHitsEcalEB_;
	edm::Handle<EcalRecHitCollection>* rawRecHitsEcalEE_;
	edm::Handle<EcalRecHitCollection>* rawRecHitsEcalES_;
	edm::Handle<HBHERecHitCollection>* rawRecHitsHcal_;
	edm::Handle<reco::PFClusterCollection>* clustersEcal_;
	edm::Handle<reco::PFClusterCollection>* clustersHcal_;
	edm::Handle<reco::PFRecHitCollection>* recHitsEcal_;
	edm::Handle<reco::PFRecHitCollection>* recHitsHcal_;
	edm::Handle<reco::PFCandidateCollection>* pfCandidates_;
	edm::Handle<reco::PFRecTrackCollection>* recTracks_;
	edm::Handle<reco::PFSimParticleCollection>* simParticles_;
};

}
#endif /* COLLISIONDELEGATE_H_ */
