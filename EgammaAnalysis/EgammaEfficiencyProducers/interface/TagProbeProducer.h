#ifndef EgammaEfficiencyProducers_TagProbeProducer_h
#define EgammaEfficiencyProducers_TagProbeProducer_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Common/interface/EDProduct.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <string>
#include <memory>
#include "EgammaAnalysis/EgammaEfficiencyAlgos/interface/TagProbeAlgo.h"

class TagProbeProducer : public edm::EDProducer
{
   public:
      explicit TagProbeProducer(const edm::ParameterSet&);
      virtual ~TagProbeProducer(){};
      virtual void beginJob(const edm::EventSetup&);
      virtual void produce(edm::Event &, const edm::EventSetup&);

   private:
      const edm::ParameterSet conf_;
      TagProbeAlgo algo_;
};

#endif

