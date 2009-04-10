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

	virtual void initCore(const edm::ParameterSet& parameters);

	virtual void startEventCore(const edm::Event& event,
			const edm::EventSetup& setup);

	virtual bool endEventCore(const edm::Event& event);

	virtual ParticleFiltrationDecisionCollection isGoodParticleCore(edm::Event& event,
			const edm::EventSetup& setup);

	virtual void getTagsCore(const edm::ParameterSet& parameters);

	virtual void finishCore();

	/*
	 * Returns a Quality for a muon candidate.
	 */
	Quality isNotMuon();

	/*
	 * Returns a Quality for a single MIP
	 */
	Quality isSingleMIP();

	Quality noBeamHalo();

	/*
	 * Retuns a quality if this is a pion according to the TOF chambers.
	 */
	Quality isTOFPion();

	/*
	 * Returns a quality if this is a pion in the cerenkov
	 */
	Quality isCerenkovPion();

	Quality isCerenkovElectronCandidate();

	Quality isCerenkovProtonKaonCandidate();

	/*
	 * Returns a quality for whether this particle was just a bad trigger, and therefore just noise
	 */
	Quality isNoiseCandidate();

	bool applyCleaningCuts_;
	bool computeVetos_;
	bool identifyCleanParticles_;
	bool saveAllCleanParticles_;
	bool clustersFromCandidates_;

	bool noiseMode_;

	unsigned muonCands_;
	unsigned nonMipCands_;
	unsigned beamHaloCands_;
	unsigned cerenkovNonPions_;
	unsigned tofNonPions_;
	unsigned electronCandidates_;
	unsigned protonKaonCandidates_;
	unsigned goodPionsFound_;
	unsigned pureNoiseEvents_;

	std::map<unsigned, pftools::RunInfo*> runInfos_;

	pftools::RunInfo* thisRun_;
	Quality conservatism_;

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

};
}
#endif /* TESTBEAMFILTRATIONDELEGATE_H_ */
