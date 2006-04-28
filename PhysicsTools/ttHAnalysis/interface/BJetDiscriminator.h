#ifndef ttHAnalysis_BJetDiscriminator_h
#define ttHAnalysis_BJetDiscriminator_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include <string>

namespace edm {
  class ParameterSet;
}

namespace tth {
  
  class BJetDiscriminator : public edm::EDProducer {
  public:
    BJetDiscriminator( const edm::ParameterSet & );
    ~BJetDiscriminator();
  private:
    typedef reco::CandidateCollection Candidates;
    void produce( edm::Event& , const edm::EventSetup& );
    std::string source;
  };
}

#endif
