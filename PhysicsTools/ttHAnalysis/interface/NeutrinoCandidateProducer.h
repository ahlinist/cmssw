#ifndef ttHAnalysis_NeutrinoCandidateProducer_h
#define ttHAnalysis_NeutrinoCandidateProducer_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include <string>

namespace edm {
  class ParameterSet;
}

namespace tth {
  
  class NeutrinoCandidateProducer : public edm::EDProducer {
  public:
    NeutrinoCandidateProducer( const edm::ParameterSet & );
    ~NeutrinoCandidateProducer();
  private:
    typedef reco::CandidateCollection Candidates;
    void produce( edm::Event& , const edm::EventSetup& );
    std::string sourceEl;
    std::string sourceMet;
  };
}

#endif
