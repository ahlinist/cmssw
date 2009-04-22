#ifndef CALIBRATABLEPRODUCER_H_
#define CALIBRATABLEPRODUCER_H_

#include <string>
#include <vector>

#include <TFile.h>
#include <TTree.h>
#include <TObject.h>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "RecoParticleFlow/PFAnalyses/interface/EventDelegate.h"

class CalibratableProducer : public edm::EDProducer {

public:
	explicit CalibratableProducer(const edm::ParameterSet& parameters);
	virtual ~CalibratableProducer();

private:

	virtual void beginJob(const edm::EventSetup& setup);
	virtual void produce(edm::Event& event, const edm::EventSetup& setup);
	virtual void endJob();

	pftools::EventDelegate* ed_;

	std::string edtype_;
	std::string dptype_;
	std::string tbtype_;
};

#endif /*CALIBRATABLEPRODUCER_H_*/
