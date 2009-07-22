#ifndef TESTBEAMFILTRATIONDELEGATE_H_
#define TESTBEAMFILTRATIONDELEGATE_H_

#include "RecoParticleFlow/PFAnalyses/interface/ParticleFiltrationDelegate.h"

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


namespace pftools {
/**
 * @class TestbeamFiltrationDelegate
 * @brief Selects single pion MIP events from testbeam data
 *
 * There are many parameters available to control execution, such as:
 * a) Reject all events that aren't single pion candidates
 * b) Let clean particles through (including electrons etc)
 * c) Try to identify the particles explicitly
 *
 * A file of RunInfo objects is required, which contains the test beam parameters
 * for all runs. See TestbeamPreprocessor for details.
 *
 *	bool applyCleaningCuts_; - reject all events that aren't single MIP candidates
 *	bool computeVetos_; - just keep this true (legacy variable...)
 *	bool identifyCleanParticles_; - if we can disentangle protons/kaons/electrons/pion/muons, do so
 *	bool saveAllCleanParticles_; - pass everything as long as it's clean
 *	bool noiseMode_; - looks for events without any activity!
 *
 * @bug Probably a few :-(
 * @bug While we can "identifyCleanParticles", this information is not conveyed to the ParticleFiltrationDecision
 *
 * @author Jamie Ballin
 * @date April 2009
 */
class TestbeamFiltrationDelegate: public ParticleFiltrationDelegate {
public:
	TestbeamFiltrationDelegate();
	virtual ~TestbeamFiltrationDelegate();

protected:

	virtual void initCore(const edm::ParameterSet& parameters);

	virtual void startEventCore(const edm::Event& event,
			const edm::EventSetup& setup);

	virtual bool endEventCore(const edm::Event& event);

	/**
	 * Produces a collection (of just one entry) to be stuck into the event, identifying
	 * whether this event matches criteria given in the config files.
	 * @param event
	 * @param setup
	 * @return
	 */
	virtual ParticleFiltrationDecisionCollection isGoodParticleCore(edm::Event& event,
			const edm::EventSetup& setup);

	virtual void getTagsCore(const edm::ParameterSet& parameters);

	/**
	 * Prints a summary via LogInfo
	 * @param event
	 * @return
	 */
	virtual void finishCore();

	/**
	 * Returns a Quality for a muon candidate.
	 */
	Quality isNotMuon();

	/**
	 * Returns a Quality for a single MIP
	 */
	Quality isSingleMIP();

	/**
	 * Returns a Quality of this being a beam halo event
	 * @return
	 */
	Quality noBeamHalo();

	/**
	 * Retuns a quality if this is a pion according to the TOF chambers.
	 */
	Quality isTOFPion();

	/**
	 * Returns a quality if this is a pion in the cerenkov
	 */
	Quality isCerenkovPion();

	/**
	 * Likelihood for this being an electron according to Cerenkov
	 * @return
	 */
	Quality isCerenkovElectronCandidate();

	/**
	 * Likelihood for this being a proton or kaon according to Cerenkov
	 * @return
	 */
	Quality isCerenkovProtonKaonCandidate();

	/**
	 * Returns a quality for whether this particle was just a bad trigger, and therefore just noise
	 */
	Quality isNoiseCandidate();


	bool applyCleaningCuts_;
	bool computeVetos_;
	bool identifyCleanParticles_;
	bool saveAllCleanParticles_;
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
