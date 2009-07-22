#include "RecoParticleFlow/PFAnalyses/interface/RunInfo.h"
#include <string>
#include <ostream>
#include <exception>
#include <iostream>
#include <TF1.h>

using namespace pftools;

std::ostream& operator<<(std::ostream& s, const RunInfo& h) {
	s << "RunInfo for run " << h.runNumber_ << ", energy " << h.beamEnergy_ << "\n";
	
	for(unsigned j(0); j < h.vmx.size(); ++j) {
		s << "\tvmx" << j << ": " << h.vmx[j] << "\n";
	}
	s << "\tvmf:" << h.vmfMax_ << "\n";
	s << "\ttofMin: " << h.tofMin_ << ", tofMax: " << h.tofMax_ << "\n";
	
	return s;
}

RunInfo::RunInfo(unsigned debug) : debug_(debug) {
	reset();
	if (debug_ > 0)
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	runNumber_ = 0;
}

RunInfo::~RunInfo() {
}

void RunInfo::reset() {

	runNumber_ = 0;
	beamEnergy_ = 0.0;
	particlePDG_ = 0;
	tableEta_ = 0.0;
	tablePhi_ = 0.0;
	ecalEta_ = 0.0;
	ecalPhi_ = 0.0;
	hcalEta_ = 0.0;
	hcalPhi_ = 0.0;
	//Reset values are for 50 GeV run,30384
	s1Min_ = 100;
	s1Max_ = 220;
	s2Min_ = 200;
	s2Max_ = 400;
	s4Min_ = 200;
	s4Max_ = 400;
	bh1Max_ = 380;
	bh2Max_ = 380;
	bh3Max_ = 230;
	bh4Max_ = 380;
	vmfMax_ = 0;
	vmbMax_ = 200;
	tofMin_ = 0;
	tofMax_ = 0;
	
	ck3Min_ = 0;
	ck3Max_ = 0;
	ck2Min_ = 0;
	ck2Max_ = 0;
	
	applyTOF_ = false;
	applyCK2_ = false;
	applyCK3_ = false;
	applyVMF_ = false;
	
	vmx.clear();

}


