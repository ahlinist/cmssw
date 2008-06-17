#ifndef EgammaIsolationProducers_ElectronRefToValueProducer_h
#define EgammaIsolationProducers_ElectronRefToValueProducer_h


#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

class ElectronRefToValueProducer : public edm::EDProducer {
 public:
  explicit ElectronRefToValueProducer(const edm::ParameterSet&);
  ~ElectronRefToValueProducer();
  
  virtual void produce(edm::Event&, const edm::EventSetup&);

 private:
  edm::InputTag electronProducer_;

  edm::ParameterSet conf_;

};


#endif
