#ifndef TESTBEAMFILTRATIONDELEGATE_H_
#define TESTBEAMFILTRATIONDELEGATE_H_

#include "RecoParticleFlow/PFAnalyses/interface/ParticleFiltrationDelegate.h"

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

namespace pftools {
class TestbeamFiltrationDelegate: public ParticleFiltrationDelegate {
public:
	TestbeamFiltrationDelegate();
	virtual ~TestbeamFiltrationDelegate();

protected:
	virtual bool isGoodParticleCore(edm::Event& event,
			const edm::EventSetup& setup);

	virtual void getTagsCore(const edm::ParameterSet& parameters);

	virtual void finishCore();

	bool applyCleaningCuts_;
	bool computeVetos_;
	bool applyThresholdsToRawRecHits_;
	bool identifyCleanParticles_;
	bool saveAllCleanParticles_;
	bool clustersFromCandidates_;
	unsigned stripAnomalousEvents_;
	unsigned muonCands_;
	unsigned nonMipCands_;
	unsigned beamHaloCands_;
	unsigned cerenkovNonPions_;
	unsigned tofNonPions_;
	unsigned electronCandidates_;
	unsigned protonKaonCandidates_;
	unsigned goodPionsFound_;

	std::map<unsigned, jamieb::RunInfo*> runInfos_;

	jamieb::RunInfo* thisRun_;
	Quality conservatism_;

	edm::InputTag inputTagRawRecHitsEcal_;
	edm::InputTag inputTagRawRecHitsHcal_;

	edm::InputTag inputTagBeamCounters_;
	edm::InputTag inputTagTiming_;
	edm::InputTag inputTagRunData_;
	edm::InputTag inputTagEventPosition_;
	edm::InputTag inputTagTriggerData_;

	edm::Handle<HcalTBBeamCounters>* beamCounters_;
	edm::Handle<HcalTBTiming>* timing_;
	edm::Handle<HcalTBRunData>* runData_;
	edm::Handle<HcalTBEventPosition> * eventPosition_;
	edm::Handle<HcalTBTriggerData> * triggerData_;

	edm::Handle<EcalRecHitCollection>* rawRecHitsEcal_;
	edm::Handle<HBHERecHitCollection>* rawRecHitsHcal_;



};
}
#endif /* TESTBEAMFILTRATIONDELEGATE_H_ */
