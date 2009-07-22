#ifndef RUNINFO_H_
#define RUNINFO_H_

#include <vector>
#include <TH1.h>
/*
 * Reflects the state of a veto decision.
 */
enum Quality {
	SURELYNOT = 0, UNLIKELY = 3, PROBABLY =7, DEFINITEYES = 10
};

namespace pftools {
class RunInfo {
public:
	RunInfo(unsigned debug = 0);
	
	virtual ~RunInfo();
	
	void reset();
	
	unsigned runNumber_;
	double beamEnergy_;
	int particlePDG_;
	double tableEta_;
	double tablePhi_;
	double ecalEta_;
	double ecalPhi_;
	double hcalEta_;
	double hcalPhi_;

	unsigned s1Min_;
	unsigned s1Max_;
	unsigned s2Min_;
	unsigned s2Max_;
	unsigned s4Min_;
	unsigned s4Max_;
	unsigned bh1Max_;
	unsigned bh2Max_;
	unsigned bh3Max_;
	unsigned bh4Max_;
	
	unsigned vmfMax_;
	unsigned vmbMax_;
	bool applyVMF_;
	
	
	int ck2Min_;
	int ck2Max_;
	int ck3Min_;
	int ck3Max_;
	bool applyCK2_;
	bool applyCK3_;
	bool applyTOF_;
	int tofMin_;
	int tofMax_;
	
	std::vector<int> vmx;


private:
	
	unsigned debug_;


};

}

std::ostream& operator<<(std::ostream& s, const pftools::RunInfo& h);

#endif /*RUNINFO_H_*/
