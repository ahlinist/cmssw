#ifndef TESTBEAMDELEGATE_H_
#define TESTBEAMDELEGATE_H_

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

#include <map>

#include <TH1.h>
#include <TH1F.h>

namespace pftools {
/**
 * @class TestbeamDelegate
 * @brief EventDelegate specifically designed for extracting Calibratables from testbeam data
 *
 * @author Jamie Ballin
 * @date April 2009
 *
 * This ones a bit complex :) ...
 * TODO: DOCUMENTATION!
 * See private parameters below for specific explanations
 *
 */
class TestbeamDelegate: public pftools::EventDelegate {
public:
	TestbeamDelegate();
	virtual ~TestbeamDelegate();

	//The main workhorse
	virtual bool processEvent(const edm::Event& event,
			const edm::EventSetup& setup);

	virtual bool finish();

protected:
	virtual void initCore(const edm::ParameterSet& parameters);

	virtual void startEventCore(const edm::Event& event,
			const edm::EventSetup& setup);

	virtual bool endEventCore();

	virtual void startParticleCore();

	virtual void endParticleCore();

	/**
	 * Overrides parent class, since HCAL hits need fixing at 2006 testbeam setup
	 * @param hcalRechits
	 * @param geometry
	 * @param targetEta
	 * @param targetPhi
	 */
	virtual void extractHcalRecHits(const HBHERecHitCollection& hcalRechits,
			const CaloSubdetectorGeometry* geometry, double targetEta,
			double targetPhi);

private:

	bool applyCleaningCuts_;
	bool saveJustPions_;

	unsigned stripAnomalousEvents_;
	unsigned maxEventsFromEachRun_;
	unsigned eventsSeenInThisRun_;
	unsigned muonCands_;
	unsigned nonMipCands_;
	unsigned beamHaloCands_;
	unsigned cerenkovNonPions_;
	unsigned tofNonPions_;
	unsigned electronCandidates_;
	unsigned protonKaonCandidates_;
	unsigned goodPionsFound_;

	double deltaRRecHitsToCenterECAL_;
	double deltaRRecHitsToCenterHCAL_;
	double deltaRClustersToCenterECAL_;
	double deltaRClustersToCenterHCAL_;
	double deltaRPhotonsToTrack_;
	double deltaRNeutralsToTrack_;

	std::map<unsigned, pftools::RunInfo*> runInfos_;

	pftools::RunInfo* thisRun_;
	Quality conservatism_;

	edm::InputTag inputTagParticleFiltration_;

	edm::InputTag inputTagRawRecHitsEcal_;
	edm::InputTag inputTagRawRecHitsHcal_;

	edm::InputTag inputTagBeamCounters_;
	edm::InputTag inputTagTiming_;
	edm::InputTag inputTagRunData_;
	edm::InputTag inputTagEventPosition_;
	edm::InputTag inputTagTriggerData_;

	edm::InputTag inputTagClustersEcal_;
	edm::InputTag inputTagClustersHcal_;
	edm::InputTag inputTagRecHitsEcal_;
	edm::InputTag inputTagRecHitsHcal_;
	edm::InputTag inputTagPFCandidates_;

	edm::Handle<pftools::ParticleFiltrationDecisionCollection>* filtration_;

	edm::Handle<HcalTBBeamCounters>* beamCounters_;
	edm::Handle<HcalTBTiming>* timing_;
	edm::Handle<HcalTBRunData>* runData_;
	edm::Handle<HcalTBEventPosition> * eventPosition_;
	edm::Handle<HcalTBTriggerData> * triggerData_;

	edm::Handle<EcalRecHitCollection>* rawRecHitsEcal_;
	edm::Handle<HBHERecHitCollection>* rawRecHitsHcal_;

	edm::Handle<reco::PFClusterCollection>* clustersEcal_;
	edm::Handle<reco::PFClusterCollection>* clustersHcal_;
	edm::Handle<reco::PFRecHitCollection>* recHitsEcal_;
	edm::Handle<reco::PFRecHitCollection>* recHitsHcal_;
	edm::Handle<reco::PFCandidateCollection>* pfCandidates_;

};
}

#endif /*TESTBEAMDELEGATE_H_*/
