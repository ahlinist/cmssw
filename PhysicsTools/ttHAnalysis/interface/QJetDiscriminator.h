#ifndef ttHAnalysis_QJetDiscriminator_h
#define ttHAnalysis_QJetDiscriminator_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include <string>

namespace edm {
  class ParameterSet;
}

namespace tth {
  
  class QJetDiscriminator : public edm::EDProducer {
  public:
    QJetDiscriminator( const edm::ParameterSet & );
    ~QJetDiscriminator();
  private:
    typedef reco::CandidateCollection Candidates;
    void produce( edm::Event& , const edm::EventSetup& );
    std::string source;
  };
}

#endif
