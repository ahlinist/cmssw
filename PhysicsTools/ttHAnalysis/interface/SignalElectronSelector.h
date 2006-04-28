#ifndef ttHAnalysis_SignalElectronSelector_h
#define ttHAnalysis_SignalElectronSelector_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include <string>

namespace edm {
  class ParameterSet;
}

namespace tth {
  
  class SignalElectronSelector : public edm::EDProducer {
  public:
    SignalElectronSelector( const edm::ParameterSet & );
    ~SignalElectronSelector();
  private:
    typedef reco::CandidateCollection Candidates;
    void produce( edm::Event& , const edm::EventSetup& );
    std::string source;
  };
}

#endif
