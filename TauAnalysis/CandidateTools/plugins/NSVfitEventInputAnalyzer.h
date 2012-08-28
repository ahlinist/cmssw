#ifndef TauAnalysis_CandidateTools_NSVfitEventInputAnalyzer_h
#define TauAnalysis_CandidateTools_NSVfitEventInputAnalyzer_h

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitEventVertexRefitter.h"

#include <string>

class NSVfitEventInputAnalyzer : public edm::EDAnalyzer 
{
 public:
  // constructor 
  explicit NSVfitEventInputAnalyzer(const edm::ParameterSet&);
    
  // destructor
  ~NSVfitEventInputAnalyzer();
    
 private:
  void beginJob();
  void analyze(const edm::Event&, const edm::EventSetup&);

  std::string moduleLabel_;

  edm::InputTag srcGenParticles_;
  edm::InputTag srcElectrons_;
  edm::InputTag srcMuons_;
  edm::InputTag srcTaus_;
  edm::InputTag srcMEt_;
  edm::InputTag srcMEtCov_;
  edm::InputTag srcGenVertex_;
  edm::InputTag srcRecVertex_;
  
  NSVfitEventVertexRefitter* eventVertexFitAlgorithm_;

  typedef std::vector<edm::InputTag> vInputTag;
  vInputTag srcWeights_;

  std::string dqmDirectory_;

  MonitorElement* genMEtPt_;

  MonitorElement* recMEtDeltaPt_;
  MonitorElement* recMEtDeltaPparl_;
  MonitorElement* recMEtDeltaPperp_;

  MonitorElement* recMEtSigma2d_;
  MonitorElement* recMEtSigmaParl_;
  MonitorElement* recMEtSigmaPerp_;

  MonitorElement* recMEtPull2d_;
  MonitorElement* recMEtPullParl_;
  MonitorElement* recMEtPullPerp_;

  MonitorElement* genVertexR_;
  MonitorElement* genVertexPhi_;
  MonitorElement* genVertexZ_;

  MonitorElement* recVertexDeltaX_;
  MonitorElement* recVertexDeltaY_;
  MonitorElement* recVertexDeltaZ_;
  MonitorElement* recVertexNDoF_;
  MonitorElement* recVertexNormalizedChi2_;
  
  MonitorElement* recVertexSigmaX_;
  MonitorElement* recVertexSigmaY_;
  MonitorElement* recVertexSigmaZ_;

  MonitorElement* recVertexPull3d_;
  MonitorElement* recVertexPullX_;
  MonitorElement* recVertexPullY_;
  MonitorElement* recVertexPullZ_;
};

#endif   
