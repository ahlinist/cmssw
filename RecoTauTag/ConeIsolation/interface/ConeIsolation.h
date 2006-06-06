#ifndef RecoTauTag_ConeIsolation
#define RecoTauTag_ConeIsolation

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "RecoTauTag/ConeIsolation/interface/ConeIsolationAlgorithm.h"

class ConeIsolationTauTagAlgorithm;

class ConeIsolation : public edm::EDProducer {
   public:
      explicit ConeIsolation(const edm::ParameterSet&);
      ~ConeIsolation();


      virtual void produce(edm::Event&, const edm::EventSetup&);
 private:
      ConeIsolationTauTagAlgorithm* m_concreteAlgo;
      ConeIsolationAlgorithm m_algo;
};
#endif

