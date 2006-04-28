#ifndef ttHAnalysis_JetCandidateProducer_h
#define ttHAnalysis_JetCandidateProducer_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include <string>

namespace edm {
  class ParameterSet;
}

namespace tth {
  
  class JetCandidateProducer : public edm::EDProducer {
  public:
    JetCandidateProducer( const edm::ParameterSet & );
    ~JetCandidateProducer();
  private:
    typedef reco::CandidateCollection Candidates;
    void produce( edm::Event& , const edm::EventSetup& );
    std::string source;
  };
}

#endif
