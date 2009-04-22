#ifndef EXTRACTIONANALYZER_H_
#define EXTRACTIONANALYZER_H_

#include <string>
#include <vector>

#include <TFile.h>
#include <TTree.h>
#include <TObject.h>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"


#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"
#include "RecoParticleFlow/PFAnalyses/interface/EventDelegate.h"

class ExtractionAnalyzer : public edm::EDAnalyzer {
public:
	explicit ExtractionAnalyzer(const edm::ParameterSet& parameters);
	virtual ~ExtractionAnalyzer();

	//Totally gratuituous over-exposure of fileservice!
	//but quite fun to get experience of templatage
	//You can then do: this.<TH1F>make("myHisto");
	template<typename T, typename A> T* makeRootObject(const A& arg);

	template<typename T> T* makeRootObject();

	edm::Service<TFileService>* getFileService() {
		return &fileservice_;
	}

private:


	virtual void beginJob(const edm::EventSetup& setup);
	virtual void analyze(const edm::Event& event, const edm::EventSetup& setup);
	virtual void endJob();

	pftools::EventDelegate* ed_;
	TTree* tree_;
	edm::Service<TFileService> fileservice_;
	std::vector<TObject*> userObjects_;

	bool isMC_;
	std::string edtype_;
	std::string dptype_;
	std::string tbtype_;
	//std::string testtype_;


};


#endif /*EXTRACTIONANALYZER_H_*/
