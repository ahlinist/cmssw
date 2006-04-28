#ifndef ttHAnalysis_MissingEtProducer_h
#define ttHAnalysis_MissingEtProducer_h

#include "FWCore/Framework/interface/EDProducer.h"
#include <string>

namespace edm {
  class ParameterSet;
}

namespace tth {
  
  class MissingEtProducer : public edm::EDProducer {
  public:
    MissingEtProducer( const edm::ParameterSet & );
    ~MissingEtProducer();
  private:
    void produce( edm::Event& , const edm::EventSetup& );
    std::string source;
  };
}

#endif
