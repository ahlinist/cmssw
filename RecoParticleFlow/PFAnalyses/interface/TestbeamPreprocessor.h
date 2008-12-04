#ifndef TESTBEAMPREPROCESSOR_H_
#define TESTBEAMPREPROCESSOR_H_

#include <TTree.h>
#include "UserCode/JamieBallinDictionary/interface/RunInfo.h"

#include <string>
#include <vector>
#include <utility>
#include <map>
#include <iostream>
#include <TH1F.h>

class TestbeamPreprocessor
{
public:
	TestbeamPreprocessor(bool withMonitoring = true);
	virtual ~TestbeamPreprocessor();
	
	void processEvents(std::string filename, int runNumber);
	
	void processEvents(int runNumber, TTree* events);
	
	void finish(std::string filename);
	
	std::map<int, std::vector<int> > getKnownEnergiesAndRuns() {
		return energiesAndRuns_;
	}
	
private:
	
	void processBeamCounters(TTree* events, RunInfo& ri);
	
	void processScintillatorCounters(TTree* events, RunInfo& ri);
	
	void processCerenkovCounters(TTree* events, RunInfo& ri);
	
	void processMuonVetos(TTree* events, RunInfo& ri);
	
	void processTOF(TTree* events, RunInfo& ri);
	
	void processPositioning(TTree* events, RunInfo& ri);
	
	std::pair<double, double> fitStabilisedGaussian(TH1* histo);
	
	void populateEnergiesAndRuns();
	
	std::vector<RunInfo> runInfos_;
	
	std::map<int, std::vector<TObject*> > monitoring_;	
	
	bool withMonitoring_;
	
	std::map<int, std::vector<int> > energiesAndRuns_;
	
	
};

#endif /*TESTBEAMPREPROCESSOR_H_*/
