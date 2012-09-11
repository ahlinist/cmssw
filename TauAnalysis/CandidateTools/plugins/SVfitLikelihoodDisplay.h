#ifndef TauAnalysis_CandidateTools_SVfitLikelihoodDisplay_h
#define TauAnalysis_CandidateTools_SVfitLikelihoodDisplay_h

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DQMServices/Core/interface/MonitorElement.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitEventVertexRefitter.h"
#include "TauAnalysis/CandidateTools/interface/NSVfitDecayVertexFitter.h"

#include <vector>
#include <string>
#include <fstream>

class SVfitLikelihoodDisplay : public edm::EDAnalyzer 
{
 public:
  // constructor 
  explicit SVfitLikelihoodDisplay(const edm::ParameterSet&);
    
  // destructor
  ~SVfitLikelihoodDisplay();
    
 private:
  void beginJob();
  void analyze(const edm::Event&, const edm::EventSetup&);
  void endJob();

  std::string moduleLabel_;

  edm::InputTag srcGenParticles_;
  edm::InputTag srcElectrons_;
  edm::InputTag srcMuons_;
  edm::InputTag srcTaus_;
  edm::InputTag srcMEt_;
  edm::InputTag srcMEtCov_;
  edm::InputTag srcVertices_;

  typedef std::vector<edm::InputTag> vInputTag;
  vInputTag srcWeights_;

  NSVfitEventVertexRefitter* eventVertexFitAlgorithm_;
  NSVfitDecayVertexFitter* decayVertexFitAlgorithm_;

  double sfProdVertexCov_;
  double sfDecayVertexCov_;

  int verbosity_;
};

#endif   
