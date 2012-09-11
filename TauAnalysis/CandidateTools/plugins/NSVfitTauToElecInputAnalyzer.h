#ifndef TauAnalysis_CandidateTools_NSVfitTauToElecInputAnalyzer_h
#define TauAnalysis_CandidateTools_NSVfitTauToElecInputAnalyzer_h

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitEventVertexRefitter.h"
#include "TauAnalysis/CandidateTools/interface/NSVfitDecayVertexFitter.h"

#include <string>

class NSVfitTauToElecInputAnalyzer : public edm::EDAnalyzer 
{
 public:
  // constructor 
  explicit NSVfitTauToElecInputAnalyzer(const edm::ParameterSet&);
    
  // destructor
  ~NSVfitTauToElecInputAnalyzer();
    
 private:
  void beginJob();
  void analyze(const edm::Event&, const edm::EventSetup&);

  std::string moduleLabel_;

  edm::InputTag srcGenParticles_;
  edm::InputTag srcElectrons_;
  edm::InputTag srcMuons_;
  edm::InputTag srcTaus_;
  edm::InputTag srcRecVertex_;

  typedef std::vector<edm::InputTag> vInputTag;
  vInputTag srcWeights_;

  NSVfitEventVertexRefitter* eventVertexFitAlgorithm_;
  NSVfitDecayVertexFitter* decayVertexFitAlgorithm_;

  std::string dqmDirectory_;

  MonitorElement* genTauPt_;
  MonitorElement* genTauEta_;
  MonitorElement* genTauPhi_;
  MonitorElement* genTauVisEnFrac_;
  MonitorElement* genTauDecayDistance_;
  MonitorElement* genTauDecayDistanceNormalized_;
  MonitorElement* genTau_phi_lab_;
  MonitorElement* genTau_gjAngle_;

  MonitorElement* genElectronPt_;
  MonitorElement* genElectronEta_;
  MonitorElement* genElectronPhi_;

  MonitorElement* recElectronDeltaPt_absolute_;
  MonitorElement* recElectronDeltaPt_relative_;
  MonitorElement* recElectronDeltaEta_;
  MonitorElement* recElectronDeltaPhi_;
  MonitorElement* recElectronDeltaVisMass_absolute_;
  MonitorElement* recElectronDeltaVisMass_relative_;

  MonitorElement* recLeadTrackDeltaPt_absolute_;
  MonitorElement* recLeadTrackDeltaPt_relative_;
  MonitorElement* recLeadTrackDeltaEta_;
  MonitorElement* recLeadTrackDeltaPhi_;
  MonitorElement* recLeadTrackNumHits_;
  MonitorElement* recLeadTrackNumPixelHits_;
  MonitorElement* recLeadTrackNormalizedChi2_;
  MonitorElement* recLeadTrackDCA_;
  MonitorElement* recLeadTrackPCApull3d_;
};

#endif   
