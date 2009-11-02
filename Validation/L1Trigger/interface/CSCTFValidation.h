#ifndef CSCTFValidation_h
#define CSCTFValidation_h

// system include files
#include <memory>
#include <unistd.h>

// Framework include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
// DQM include files
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"
// TF include files
#include <iostream>

class CSCTFValidation : public edm::EDAnalyzer {
private:
	edm::InputTag genProd, cscProd;
	DQMStore * dbe;
	//Efficiency
	MonitorElement* denEta, *Q1Eta, *Q2Eta, *Q3Eta, *etaEffQ1, *etaEffQ2, *etaEffQ3;
	MonitorElement* fidPtDen, *tfAnyTrk, *tfPt3p5Trk, *tfPt7p0Trk, *tfPt20Trk, *tfPt40Trk, *tfPt60Trk;
	MonitorElement* ptEffTfPtAny, *ptEffTfPt3p5, *ptEffTfPt7p0,	*ptEffTfPt20, *ptEffTfPt40, *ptEffTfPt60;
	//Resolution
	MonitorElement* phiRes, *etaRes, *ptRes, *ptResEtaHi, *ptResEtaLow, *ptResFidPt;
	//Timing
	MonitorElement* tfBx;
	std::string outFile;
	std::string m_dirName;
	
public:
	void analyze(edm::Event const& e, edm::EventSetup const& es);
	void endJob(void);
	void beginJob(edm::EventSetup const&);
	explicit CSCTFValidation(edm::ParameterSet const& ps);
	virtual ~CSCTFValidation() {}
};

DEFINE_FWK_MODULE(CSCTFValidation);

#endif
