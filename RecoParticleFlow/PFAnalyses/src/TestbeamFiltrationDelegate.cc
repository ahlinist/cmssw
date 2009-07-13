#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "RecoParticleFlow/PFAnalyses/interface/operations.h"
#include "RecoParticleFlow/PFAnalyses/interface/TestbeamFiltrationDelegate.h"

using namespace pftools;
using namespace edm;
using namespace std;
using namespace reco;

#include <exception>

#include <TFile.h>
#include <TTree.h>

TestbeamFiltrationDelegate::TestbeamFiltrationDelegate() :
	applyCleaningCuts_(true), computeVetos_(true),
			identifyCleanParticles_(true), saveAllCleanParticles_(true),
			muonCands_(0), nonMipCands_(0), beamHaloCands_(0),
			cerenkovNonPions_(0), tofNonPions_(0), electronCandidates_(0),
			protonKaonCandidates_(0), goodPionsFound_(0), pureNoiseEvents_(0) {
	LogInfo("TestbeamFiltrationDelegate") << __PRETTY_FUNCTION__
			<< ": looking for single charged pions." << std::endl;
	pdgCodes_.clear();
	pdgCodes_.push_back(211);
	pdgCodes_.push_back(-211);
	particle_ = "Pi+-";
	decision_.filtrationProvenance_ = "TestbeamFiltrationDelegate";
}

TestbeamFiltrationDelegate::~TestbeamFiltrationDelegate() {
	LogDebug("TestbeamFiltrationDelegate") << __PRETTY_FUNCTION__ << std::endl;

}

void TestbeamFiltrationDelegate::startEventCore(const edm::Event& event,
		const edm::EventSetup& setup) {

	//first check for new run!
	bool runok(true);
	if (event.run() != thisRun_->runNumber_) {
		LogInfo("TestbeamFiltrationDelegate") << __PRETTY_FUNCTION__
				<< ": New run detected :" << event.run() << ".\n";
		thisRun_ = runInfos_[event.run()];
		if(!thisRun_)
			runok = false;
	}
	if (!runok) {
		LogError("TestbeamFiltrationDelegate") << __PRETTY_FUNCTION__
				<< ": problem looking up run info?!" << std::endl;
		thisEventPasses_ = false;
		return;
	}
	thisEventPasses_ = false;

	runData_ = new Handle<HcalTBRunData> ;
	timing_ = new Handle<HcalTBTiming> ;
	eventPosition_ = new Handle<HcalTBEventPosition> ;
	beamCounters_ = new Handle<HcalTBBeamCounters> ;
	triggerData_ = new Handle<HcalTBTriggerData> ;

	getCollection(*runData_, inputTagRunData_, event);
	getCollection(*timing_, inputTagTiming_, event);
	getCollection(*eventPosition_, inputTagEventPosition_, event);
	getCollection(*beamCounters_, inputTagBeamCounters_, event);
	getCollection(*triggerData_, inputTagTriggerData_, event);

}

//Checks vetos
//Return true if you want the particle passed
bool TestbeamFiltrationDelegate::endEventCore(const edm::Event& event) {
	if (thisEventPasses_)
		++nPasses_;
	else
		++nFails_;

	delete runData_;
	delete timing_;
	delete eventPosition_;
	delete beamCounters_;

	return thisEventPasses_;

}

ParticleFiltrationDecisionCollection TestbeamFiltrationDelegate::isGoodParticleCore(
		edm::Event& event, const edm::EventSetup& setup) {

	LogDebug("TestbeamFiltrationDelegate") << __PRETTY_FUNCTION__ << std::endl;

	char vetosPassed = 31;
	if (computeVetos_) {
		if (isNotMuon() < DEFINITEYES) {
			++muonCands_;
			decision_.type_ = ParticleFiltrationDecision::MUON;
			if (applyCleaningCuts_)
				thisEventPasses_ = false;
			vetosPassed -= 4;
		}
		if (isSingleMIP() < DEFINITEYES) {
			++nonMipCands_;
			if (applyCleaningCuts_)
				thisEventPasses_ = false;
			vetosPassed -= 1;
		}
		if (noBeamHalo() < DEFINITEYES) {
			++beamHaloCands_;
			if (applyCleaningCuts_)
				thisEventPasses_ = false;
			vetosPassed -= 2;
		}
		if (isCerenkovPion() < DEFINITEYES) {
			++cerenkovNonPions_;
			if (applyCleaningCuts_)
				thisEventPasses_ = false;
			vetosPassed -= 16;
		}
		if (isTOFPion() < DEFINITEYES) {
			++tofNonPions_;
			if (applyCleaningCuts_)
				thisEventPasses_ = false;
			vetosPassed -= 8;
		}

	}
	Quality electron(UNLIKELY);
	Quality proton(UNLIKELY);
	//If it passes singleMip, Beam halo and not muon counters, it may be an electron or proton
	if (vetosPassed == 7 || vetosPassed == 15 || vetosPassed == 23) {
		electron = isCerenkovElectronCandidate();
		proton = isCerenkovProtonKaonCandidate();

		if (electron == DEFINITEYES && proton == DEFINITEYES) {
			//something's gone awry!
			decision_.type_ = ParticleFiltrationDecision::OTHER;
			if (debug_ > 3)
				LogInfo("TestbeamFiltrationDelegate")
						<< "\tCould be both an electron and a proton?\n";
		} else {
			if (electron == DEFINITEYES) {
				++electronCandidates_;
				decision_.type_ = ParticleFiltrationDecision::ELECTRON;
				if (debug_ > 4)
					LogInfo("TestbeamFiltrationDelegate")
							<< "\tIt's an electron.\n";
			} else if (proton == DEFINITEYES) {
				++protonKaonCandidates_;
				decision_.type_ = ParticleFiltrationDecision::PROTON_KAON;
				if (debug_ > 4)
					LogInfo("TestbeamFiltrationDelegate")
							<< "\tIt's a proton/kaon.\n";
			}
			if (electron == DEFINITEYES || proton == DEFINITEYES) {
				if (saveAllCleanParticles_)
					thisEventPasses_ = true;
			}
		}
	}

	if (vetosPassed == 31) {
		decision_.type_ = ParticleFiltrationDecision::PION;
		++goodPionsFound_;
		thisEventPasses_ = true;
	}

	if (noiseMode_) {
		//Select events where there certainly wasn't any particle!
		thisEventPasses_ = false;
		if (isNoiseCandidate() > UNLIKELY && isNotMuon() == DEFINITEYES
				&& vetosPassed != 31) {
			decision_.type_ == ParticleFiltrationDecision::NOISE;
			++pureNoiseEvents_;
			thisEventPasses_ = true;
		}
	}

	if (debug_ > 3 && thisEventPasses_)
		LogInfo("TestbeamFiltrationDelegate")
				<< "\tEvent PASSES cut criteria.\n";

	if (debug_ > 3 && !thisEventPasses_)
		LogInfo("TestbeamFiltrationDelegate")
				<< "\tEvent FAILS cut criteria.\n";

	decision_.vetosPassed_ = vetosPassed;
	ParticleFiltrationDecisionCollection coll;
	coll.push_back(decision_);
	return coll;
}

void TestbeamFiltrationDelegate::getTagsCore(
		const edm::ParameterSet& parameters) {

	try {
		inputTagBeamCounters_ = parameters.getParameter<InputTag> (
				"BeamCounters");
		inputTagTiming_ = parameters.getParameter<InputTag> ("Timing");
		inputTagEventPosition_ = parameters.getParameter<InputTag> (
				"EventPosition");
		inputTagRunData_ = parameters.getParameter<InputTag> ("RunData");
		inputTagTriggerData_
				= parameters.getParameter<InputTag> ("TriggerData");

	} catch (exception& e) {
		LogError("TestbeamFiltrationDelegate") << "Error getting parameters."
				<< std::endl;
		throw e;
	}

}

void TestbeamFiltrationDelegate::initCore(const edm::ParameterSet& parameters) {

	applyCleaningCuts_ = parameters.getParameter<bool> ("applyCleaningCuts");
	computeVetos_ = parameters.getParameter<bool> ("computeVetos");
	identifyCleanParticles_ = parameters.getParameter<bool> (
			"identifyCleanParticles");
	saveAllCleanParticles_ = parameters.getParameter<bool> (
			"saveAllCleanParticles");

	noiseMode_ = parameters.getParameter<bool> ("noiseMode");

	std::string cuts = parameters.getParameter<std::string> ("runinfo_cuts");
	TFile* file = TFile::Open(cuts.c_str());
	TTree* tree(0);
	thisRun_ = new RunInfo();
	LogInfo("TestbeamFiltrationDelegate") << "Resurrecting run infos from "
			<< cuts << "\n";

	if (noiseMode_)
		LogInfo("TestbeamFiltrationDelegate")
				<< "Running with noiseMode = true: this means you'll only get noise events and no particles! Run on high energy beams please.\n";

	if (file != 0) {
		tree = (TTree*) file->FindObjectAny("RunInfo");
		if (tree != 0) {
			//Found tree successfully... loop over entries and resurrect infos
			tree->SetBranchAddress("RunInfo", &thisRun_);
			if (debug_ > 3) {
				LogInfo("TestbeamFiltrationDelegate") << "Cut file has "
						<< tree->GetEntries() << " entries." << std::endl;

			}
			for (unsigned entry(0); entry < tree->GetEntries(); ++entry) {
				tree->GetEntry(entry);
				//Copy run info
				if (debug_ > 4) {
					LogInfo("TestbeamFiltrationDelegate")
							<< "Copying run info for run "
							<< thisRun_->runNumber_ << std::endl;
					//LogInfo("TestbeamFiltrationDelegate") << *thisRun_;
				}
				RunInfo* aRun = new RunInfo(*thisRun_);
				runInfos_[thisRun_->runNumber_] = aRun;
			}
		} else {
			LogError("TestbeamFiltrationDelegate") << "Tree pointer is null!"
					<< std::endl;
		}
		file->Close();
	} else {
		LogError("TestbeamFiltrationDelegate")
				<< "Couldn't open run info file!" << std::endl;
	}
}

void TestbeamFiltrationDelegate::finishCore() {
	LogDebug("TestbeamFiltrationDelegate") << __PRETTY_FUNCTION__ << "\n";

	int total = goodPionsFound_ + protonKaonCandidates_ + electronCandidates_;

	//Summarise running
	LogInfo("TestbeamFiltrationDelegate")
			<< "------------------------------------------------------\n"
			<< "Summary of TestbeamFiltrationDelegate's activities:\n"
			<< "\tPossible muons: " << muonCands_ << "\n"
			<< "\tPossible beam halo: " << beamHaloCands_ << "\n"
			<< "\tPossible non MIP cands: " << nonMipCands_ << "\n"
			<< "\tPossible non Cerenkov pions: " << cerenkovNonPions_ << "\n"
			<< "\tPossible non TOF pions: " << tofNonPions_ << "\n"
			<< "\tPossible electrons: " << electronCandidates_ << "\n"
			<< "\tPossible protons/kaons: " << protonKaonCandidates_ << "\n"
			<< "\tPossible pure noise events: " << pureNoiseEvents_ << "\n"
			<< "\tClass-31 pion candidates: " << goodPionsFound_ << "\n";

	if (total != 0) {
		int pionPer = static_cast<int> (100 * goodPionsFound_ / total);
		int protonPer = static_cast<int> (100 * protonKaonCandidates_ / total);
		int elecPer = static_cast<int> (100 * electronCandidates_ / total);

		LogInfo("TestbeamFiltrationDelegate")
				<< "------------------------------------------------------\n"
				<< "Beam composition (%): \t pion/electron/proton-kaon\n"
				<< "\t\t" << pionPer << " / " << elecPer << " / " << protonPer
				<< "\n";
	} else {
		LogProblem("TestbeamFiltrationDelegate")
				<< "Total good particles found = 0?!\n";
	}

	if (noiseMode_)
		LogInfo("TestbeamFiltrationDelegate")
				<< "(polite reminder): Ran with noiseMode = true! Run on high energy (> 50 GeV) beams please.\n";

	LogInfo("TestbeamFiltrationDelegate")
			<< "------------------------------------------------------\n"
			<< "\tnPasses: " << nPasses_ << ", nFails: " << nFails_ << "\n"
			<< "Leaving " << __PRETTY_FUNCTION__ << std::endl;
}

Quality TestbeamFiltrationDelegate::isNotMuon() {

	HcalTBBeamCounters counters = **beamCounters_;
	Quality isNotMuonQuality(DEFINITEYES);

	if (counters.VMBadc() > thisRun_->vmbMax_)
		isNotMuonQuality = SURELYNOT;

	//If VLE, has to pass VMX cuts too
	//TODO: reimplement VM8 with its dodgy non gaussian pedestal
	if (thisRun_->applyVMF_) {
		if (counters.VM1adc() < thisRun_->vmx[0] && counters.VM2adc()
				< thisRun_->vmx[1] && counters.VM3adc() < thisRun_->vmx[2]
				&& counters.VM4adc() < thisRun_->vmx[3] && counters.VM5adc()
				< thisRun_->vmx[4] && counters.VM6adc() < thisRun_->vmx[5]
				&& counters.VM7adc() < thisRun_->vmx[6] && counters.VMFadc()
				< thisRun_->vmfMax_) {
			isNotMuonQuality = DEFINITEYES;
			if (counters.VM8adc() > thisRun_->vmx[7]) {
				isNotMuonQuality = UNLIKELY;
			}
		} else {
			isNotMuonQuality = UNLIKELY;
		}
	}

	if (debug_ > 4) {
		LogDebug("TestbeamFiltrationDelegate") << "\tEvent isn't a muon? :"
				<< isNotMuonQuality << "\n";
	}

	return isNotMuonQuality;

}

Quality TestbeamFiltrationDelegate::isCerenkovPion() {
	if (!thisRun_->applyCK2_ && !thisRun_->applyCK3_) {
		return DEFINITEYES;
	}

	Quality isCerenkovPion(DEFINITEYES);
	HcalTBBeamCounters counters = **beamCounters_;

	if (thisRun_->applyCK2_) {
		if (counters.CK2adc() > thisRun_->ck2Max_ || counters.CK2adc()
				< thisRun_->ck2Min_)
			isCerenkovPion = UNLIKELY;
	}

	if (thisRun_->applyCK3_) {
		if (counters.CK3adc() > thisRun_->ck3Max_ || counters.CK3adc()
				< thisRun_->ck3Min_)
			isCerenkovPion = UNLIKELY;
	}

	return isCerenkovPion;
}

Quality TestbeamFiltrationDelegate::isCerenkovElectronCandidate() {

	Quality electronQuality(DEFINITEYES);
	HcalTBBeamCounters counters = **beamCounters_;

	//CK2 uniquely determines electrons
	if (thisRun_->applyCK2_) {
		if (counters.CK2adc() <= thisRun_->ck2Max_)
			electronQuality = UNLIKELY;
	}

	return electronQuality;

}

Quality TestbeamFiltrationDelegate::isCerenkovProtonKaonCandidate() {

	Quality protonQuality(DEFINITEYES);
	HcalTBBeamCounters counters = **beamCounters_;

	if (thisRun_->applyCK3_) {
		if (counters.CK3adc() < thisRun_->ck2Max_) {
			//Probably a pion
			protonQuality = UNLIKELY;
		}
	}

	if (thisRun_->applyCK2_) {
		if (counters.CK2adc() >= thisRun_->ck2Max_) {
			//Probably an electron
			protonQuality = UNLIKELY;
		}
	}

	return protonQuality;
}

Quality TestbeamFiltrationDelegate::isTOFPion() {
	if (!thisRun_->applyTOF_) {
		return DEFINITEYES;
	}

	Quality tofPionQuality(UNLIKELY);
	HcalTBTiming timing = **timing_;
	//Beam energy 1 to 10 GeV
	//Compute TOF

	double tofS = timing.TOF1Stime() - timing.TOF2Stime();
	double tofJ = timing.TOF1Jtime() - timing.TOF2Jtime();
	double meanTOF = (tofS + tofJ) / 2.0;
	if (debug_ > 4) {
		LogDebug("TestbeamFiltrationDelegate") << "\tTOFS = " << tofS
				<< ",\tTOFJ = " << tofJ << ",\tmeanTOF: " << meanTOF << "\n";
	}
	//if ((tofS > thisRun_->tofMin_ && tofS < thisRun_->tofMax_) && (tofJ > thisRun_->tofMin_ && tofJ < thisRun_->tofMax_))
	if (meanTOF > thisRun_->tofMin_ && meanTOF < thisRun_->tofMax_)
		tofPionQuality = DEFINITEYES;

	return tofPionQuality;

}

Quality TestbeamFiltrationDelegate::isSingleMIP() {

	HcalTBBeamCounters counters = **beamCounters_;
	HcalTBTriggerData triggers = **triggerData_;
	HcalTBTiming timing = **timing_;

	Quality singleMipQuality(DEFINITEYES);
	if (debug_ > 5)
		LogDebug("TestbeamFiltrationDelegate") << "\tS124adcs = "
				<< counters.S1adc() << ", " << counters.S2adc() << ", "
				<< counters.S4adc() << "\n";

	if (counters.S1adc() > thisRun_->s1Min_ && counters.S1adc()
			< thisRun_->s1Max_) {
		if (counters.S2adc() > thisRun_->s2Min_ && counters.S2adc()
				< thisRun_->s2Max_) {
			if (counters.S4adc() > thisRun_->s4Min_ && counters.S4adc()
					< thisRun_->s4Max_) {
				singleMipQuality = DEFINITEYES;
			} else
				singleMipQuality = PROBABLY;
		} else
			singleMipQuality = PROBABLY;
	} else
		singleMipQuality = PROBABLY;

	//Test beam peculiarities!
	if (isNoiseCandidate() > UNLIKELY)
		singleMipQuality = UNLIKELY;

	//	if (counters.S1adc() < thisRun_->s1Max_ && counters.S2adc()
	//			< thisRun_->s2Max_ && counters.S4adc() < thisRun_->s4Max_)
	//		singleMipQuality = DEFINITEYES;
	//	else
	//		singleMipQuality = PROBABLY;

	if (debug_ > 4) {
		LogDebug("TestbeamFiltrationDelegate") << "\tEvent is a single MIP? :"
				<< singleMipQuality << "\n";
	}
	return singleMipQuality;
}

Quality TestbeamFiltrationDelegate::isNoiseCandidate() {
	Quality noiseQuality(UNLIKELY);

	HcalTBTriggerData triggers = **triggerData_;
	HcalTBTiming timing = **timing_;
	HcalTBBeamCounters counters = **beamCounters_;

	if (counters.S1adc() < thisRun_->s1Min_ && counters.S2adc()
			< thisRun_->s2Min_ && counters.S4adc() < thisRun_->s4Min_)
		noiseQuality = PROBABLY;

	//Test beam peculiarities!
	if (!triggers.wasBeamTrigger())
		noiseQuality = PROBABLY;

	if (timing.ttcL1Atime() == 0)
		noiseQuality = PROBABLY;

	return noiseQuality;
}

Quality TestbeamFiltrationDelegate::noBeamHalo() {

	HcalTBBeamCounters counters = **beamCounters_;
	Quality beamHaloAbsenceQuality(DEFINITEYES);

	if (debug_ > 5)
		LogDebug("TestbeamFiltrationDelegate") << "\tBH1234adcs = "
				<< counters.BH1adc() << ", " << counters.BH2adc() << ", "
				<< counters.BH3adc() << ", " << counters.BH4adc() << "\n";

	if (counters.BH1adc() < thisRun_->bh1Max_ && counters.BH2adc()
			< thisRun_->bh2Max_ && counters.BH3adc() < thisRun_->bh3Max_
			&& counters.BH4adc() < thisRun_->bh4Max_)
		beamHaloAbsenceQuality = DEFINITEYES;
	else
		beamHaloAbsenceQuality = UNLIKELY;

	if (debug_ > 4) {
		LogDebug("TestbeamFiltrationDelegate") << "\tEvent has no beam halo? :"
				<< beamHaloAbsenceQuality << "\n";
	}
	return beamHaloAbsenceQuality;
}

