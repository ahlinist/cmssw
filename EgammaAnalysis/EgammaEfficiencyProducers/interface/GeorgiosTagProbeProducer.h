#ifndef EgammaEfficiencyProducers_GeorgiosTagProbeProducer_h
#define EgammaEfficiencyProducers_GeorgiosTagProbeProducer_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Common/interface/EDProduct.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <string>
#include <memory>

#include "EgammaAnalysis/EgammaEfficiencyAlgos/interface/GeorgiosTagProbeAlgo.h"

class GeorgiosTagProbeProducer : public edm::EDProducer
{
   public:
      explicit GeorgiosTagProbeProducer(const edm::ParameterSet&);
      virtual ~GeorgiosTagProbeProducer(){};
      virtual void beginJob(const edm::EventSetup&);
      virtual void produce(edm::Event &, const edm::EventSetup&);

   private:
      const edm::ParameterSet conf_;
      GeorgiosTagProbeAlgo algo_;
};

#endif

