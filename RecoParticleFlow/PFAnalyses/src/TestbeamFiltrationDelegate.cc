#include "RecoParticleFlow/PFAnalyses/interface/TestbeamFiltrationDelegate.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

using namespace pftools;
using namespace edm;


TestbeamFiltrationDelegate::TestbeamFiltrationDelegate() {

}

TestbeamFiltrationDelegate::~TestbeamFiltrationDelegate() {

}

bool TestbeamFiltrationDelegate::isGoodParticleCore(edm::Event& event,
		const edm::EventSetup& setup) {

	return thisEventPasses_;
}

void TestbeamFiltrationDelegate::getTagsCore(const edm::ParameterSet& parameters) {

}

void TestbeamFiltrationDelegate::finishCore() {
	LogDebug("TestbeamFiltrationDelegate") << __PRETTY_FUNCTION__ << "\n";

	int total = goodPionsFound_ + protonKaonCandidates_ + electronCandidates_;

	//Make some plots
	LogInfo("TestbeamFiltrationDelegate")
			<< "------------------------------------------------------\n"
			<< "Summary:\n" << "\tPossible muons: " << muonCands_ << "\n"
			<< "\tPossible beam halo: " << beamHaloCands_ << "\n"
			<< "\tPossible non MIP cands: " << nonMipCands_ << "\n"
			<< "\tPossible non Cerenkov pions: " << cerenkovNonPions_ << "\n"
			<< "\tPossible non TOF pions: " << tofNonPions_ << "\n"
			<< "\tPossible electrons: " << electronCandidates_ << "\n"
			<< "\tPossible protons/kaons: " << protonKaonCandidates_ << "\n"
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
		LogProblem("TestbeamFiltrationDelegate") << "Total good particles found = 0?!\n";
	}
	LogInfo("TestbeamFiltrationDelegate")
			<< "------------------------------------------------------\n"
			<< "\tnPasses: " << nPasses_ << ", nFails: "
			<< nFails_ << "\n" << "Leaving "
			<< __PRETTY_FUNCTION__ << std::endl;
}
