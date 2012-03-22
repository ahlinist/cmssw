#ifndef TauAnalysis_CandidateTools_NSVfitEventHypothesisCorrelationAnalyzer_h
#define TauAnalysis_CandidateTools_NSVfitEventHypothesisCorrelationAnalyzer_h

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "DataFormats/Candidate/interface/Candidate.h"

#include <TMath.h>

#include <string>

class NSVfitEventHypothesisCorrelationAnalyzer : public edm::EDAnalyzer 
{
 public:
  // constructor 
  explicit NSVfitEventHypothesisCorrelationAnalyzer(const edm::ParameterSet&);
    
  // destructor
  ~NSVfitEventHypothesisCorrelationAnalyzer();
    
 private:
  void beginJob();
  void analyze(const edm::Event&, const edm::EventSetup&);
  void endJob();

  std::string moduleLabel_;

  edm::InputTag srcEventHypotheses1_;
  edm::InputTag srcEventHypotheses2_;
  
  typedef std::vector<edm::InputTag> vInputTag;
  vInputTag srcWeights_;

  int idxResonance_;

  std::string dqmDirectory_;

  MonitorElement* svFitIsValidSolution1vs2_;
  MonitorElement* svFitMass1vs2_;
  MonitorElement* svFitSigma1vs2_;
  MonitorElement* svFitSigmaUp1vs2_;
  MonitorElement* svFitSigmaDown1vs2_;
  MonitorElement* svFitMass1vsSigma2_;
  MonitorElement* svFitMass1vsSigmaUp2_;
  MonitorElement* svFitMass1vsSigmaDown2_;
  MonitorElement* svFitMass2vsSigma1_;
  MonitorElement* svFitMass2vsSigmaUp1_;
  MonitorElement* svFitMass2vsSigmaDown1_;

  long numEvents_processed_;
  double numEventsWeighted_processed_;
};

#endif   
