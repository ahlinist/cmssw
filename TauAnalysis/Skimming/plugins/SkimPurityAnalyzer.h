#ifndef TauAnalysis_Skimming_SkimPurityAnalyzer_h
#define TauAnalysis_Skimming_SkimPurityAnalyzer_h

/** \class SkimPurityAnalyzer
 *
 * Utility class to calculate the signal purity of a RECO file
 * 
 * \author Jeff Kolb, Notre Dame
 *
 * \version $Revision: 1.1 $
 *
 */

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include <string>

class SkimPurityAnalyzer : public edm::EDAnalyzer 
{
 public:
  // constructor 
  explicit SkimPurityAnalyzer(const edm::ParameterSet&);
    
  // destructor
  virtual ~SkimPurityAnalyzer();
    
 private:
  void beginJob(const edm::EventSetup&);
  void analyze(const edm::Event&, const edm::EventSetup&);
  void endJob();

  std::string mode_;
  edm::InputTag src_;
  
  int nEvents_;
  int nRealElectrons_;
  int nFakeElectrons_;
  int nRealTaus_;
  int nFakeTaus_;
  int nRealMuons_;
  int nFakeMuons_;
 
  int nElecTau_;
  int nMuTau_;
  int nElecMu_;

};

#endif   

