#ifndef ttHAnalysis_SimpleAnalysis_h
#define ttHAnalysis_SimpleAnalysis_h

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "DataFormats/Candidate/interface/OverlapChecker.h"
#include <string>
#include "TH1.h"
#include "TFile.h"

namespace edm {
  class ParameterSet;
}

namespace tth {
  
  class SimpleAnalysis : public edm::EDAnalyzer {
  public:
    SimpleAnalysis( const edm::ParameterSet & );
    ~SimpleAnalysis();
  private:
    void analyze( const edm::Event& , const edm::EventSetup& );
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
    OverlapChecker overlap;
    TFile file;
    TH1F  histoPt;
    TH1F  histoGoodPt;
    TH1F  h_wHadMass;
    TH1F  h_tLepMass;
    TH1F  h_tHadMass;
    TH1F  h_wLepMassBest;
    TH1F  h_wHadMassBest;
    TH1F  h_tLepMassBest;
    TH1F  h_tHadMassBest;
    TH1F  h_higgsMass;
    TH1F  h_nhiggs;
  };
}

#endif
