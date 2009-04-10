#ifndef TESTBEAMDELEGATE_H_
#define TESTBEAMDELEGATE_H_

#include "RecoParticleFlow/PFAnalyses/interface/EventDelegate.h"
#include "UserCode/JamieBallinDictionary/interface/RunInfo.h"
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
#include "UserCode/JamieBallinDictionary/interface/ParticleFiltrationDecision.h"

#include <map>

#include <TH1.h>
#include <TH1F.h>
class TestbeamDelegate : public EventDelegate
{
public:
	TestbeamDelegate(bool isMC);
	virtual ~TestbeamDelegate();

	//The main workhorse
	virtual bool processEvent(const edm::Event& event,
			const edm::EventSetup& setup);

	virtual bool finish();

protected:
	virtual void initCore(const edm::ParameterSet& parameters);

	virtual void startEventCore(const edm::Event& event,
			const edm::EventSetup& setup);

	//Checks vetos
	//Return true if you want the particle written to the tree
	virtual bool endEventCore();

	virtual void startParticleCore();

	virtual void endParticleCore();

	/*
	 * Retrieves the tags listed below.
	 */
	virtual void getTagsCore(const edm::ParameterSet& parameters);


private:

	void extractCandidate(const reco::PFCandidate& cand);

	bool isMC_;
	bool applyCleaningCuts_;
	bool saveJustPions_;

	bool applyThresholdsToRawRecHits_;
	bool clustersFromCandidates_;
	bool rechitsFromCandidates_;
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

#endif /*TESTBEAMDELEGATE_H_*/
