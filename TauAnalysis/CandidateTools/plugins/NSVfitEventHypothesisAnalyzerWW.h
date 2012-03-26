#ifndef TauAnalysis_CandidateTools_NSVfitEventHypothesisAnalyzerWW_h
#define TauAnalysis_CandidateTools_NSVfitEventHypothesisAnalyzerWW_h

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "DataFormats/Candidate/interface/Candidate.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitEventHypothesis.h"

#include <TMath.h>

#include <string>

class NSVfitEventHypothesisAnalyzerWW : public edm::EDAnalyzer 
{
 public:
  // constructor 
  explicit NSVfitEventHypothesisAnalyzerWW(const edm::ParameterSet&);
    
  // destructor
  ~NSVfitEventHypothesisAnalyzerWW();
    
 private:
  void beginJob();
  void analyze(const edm::Event&, const edm::EventSetup&);
  void endJob();

  std::string moduleLabel_;

  edm::InputTag srcEventHypotheses_;
  edm::InputTag srcGenMass_;
  
  typedef std::vector<edm::InputTag> vInputTag;
  vInputTag srcWeights_;

  std::string dqmDirectory_;

  MonitorElement* svFitIsValidSolution_;
  MonitorElement* svFitMass_;
  MonitorElement* svFitMassVsGenMass_;
  MonitorElement* svFitMassDivGenMass_;

  long numEvents_processed_;
  double numEventsWeighted_processed_;

  int verbosity_;
};

#endif   
