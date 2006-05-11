#ifndef ttHAnalysis_SimpleAnalysis_h
#define ttHAnalysis_SimpleAnalysis_h

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "DataFormats/Candidate/interface/OverlapChecker.h"
#include <string>
#include "TH1.h"

namespace edm {
  class ParameterSet;
}

namespace tth {
  
  class SimpleAnalysis : public edm::EDAnalyzer {
  public:
    SimpleAnalysis( const edm::ParameterSet & );
  private:
    void beginJob( const edm::EventSetup & );
    void analyze( const edm::Event& , const edm::EventSetup& );
    void endJob();
    std::string allElectrons;
    std::string goodElectrons;
    std::string signalElectrons;
    std::string allNeutrinos;
    std::string wLepCandidates;
    std::string allJets;
    std::string bJets;
    std::string qJets;
    std::string wHadCandidates;
    std::string tLepCandidates;
    std::string tHadCandidates;
    std::string higgsCandidates;
    std::string histoFileName;
    OverlapChecker overlap;
    TH1F  histoPt;
    TH1F  h_wHadMass;
    TH1F  h_tLepMass;
    TH1F  h_tHadMass;
    TH1F  h_wHadMassBest;
    TH1F  h_tLepMassBest;
    TH1F  h_tHadMassBest;
    TH1F  h_higgsMass;
    TH1F  h_nhiggs;
  };
}

#endif
