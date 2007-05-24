#ifndef EgammaEfficiencyProducers_EmObjectProducer_h
#define EgammaEfficiencyProducers_EmObjectProducer_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Common/interface/EDProduct.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <string>
#include <memory>
#include "EgammaAnalysis/EgammaEfficiencyAlgos/interface/EmObjectAlgo.h"

class EmObjectProducer : public edm::EDProducer
{
   public:
      explicit EmObjectProducer(const edm::ParameterSet&);
      virtual ~EmObjectProducer(){};
      virtual void beginJob(const edm::EventSetup&);
      virtual void produce(edm::Event &, const edm::EventSetup&);

   private:
      const edm::ParameterSet conf_;
      std::auto_ptr<EmObjectAlgo> algo_;
};

#endif

