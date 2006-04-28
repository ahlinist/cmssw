#ifndef ttHAnalysis_ElectronSelector_h
#define ttHAnalysis_ElectronSelector_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include <string>

namespace edm {
  class ParameterSet;
}

namespace tth {
  
  class ElectronSelector : public edm::EDProducer {
  public:
    ElectronSelector( const edm::ParameterSet & );
    ~ElectronSelector();
  private:
    typedef reco::CandidateCollection Candidates;
    void produce( edm::Event& , const edm::EventSetup& );
    std::string source;
  };
}

#endif
