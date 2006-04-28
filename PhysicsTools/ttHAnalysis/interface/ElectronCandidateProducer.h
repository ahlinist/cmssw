#ifndef ttHAnalysis_ElectronCandidateProducer_h
#define ttHAnalysis_ElectronCandidateProducer_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include <string>

namespace edm {
  class ParameterSet;
}

namespace tth {
  
  class ElectronCandidateProducer : public edm::EDProducer {
  public:
    ElectronCandidateProducer( const edm::ParameterSet & );
    ~ElectronCandidateProducer();
  private:
    typedef reco::CandidateCollection Candidates;
    void produce( edm::Event& , const edm::EventSetup& );
    std::string source;
  };
}

#endif
