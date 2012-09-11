#ifndef TauAnalysis_CandidateTools_NSVfitTauToHadInputAnalyzer_h
#define TauAnalysis_CandidateTools_NSVfitTauToHadInputAnalyzer_h

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

class NSVfitTauToHadInputAnalyzer : public edm::EDAnalyzer 
{
 public:
  // constructor 
  explicit NSVfitTauToHadInputAnalyzer(const edm::ParameterSet&);
    
  // destructor
  ~NSVfitTauToHadInputAnalyzer();
    
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
  MonitorElement* genTauVisMass_;
  MonitorElement* genTauDecayDistance_;
  MonitorElement* genTauDecayDistanceNormalized_;
  MonitorElement* genTauDecayMode_;
  MonitorElement* genTau_phi_lab_;
  MonitorElement* genTau_gjAngle_;

  MonitorElement* genLeadTrackPt_;
  MonitorElement* genLeadTrackEta_;
  MonitorElement* genLeadTrackPhi_;

  MonitorElement* recTauHadDeltaPt_absolute_;
  MonitorElement* recTauHadDeltaPt_relative_;
  MonitorElement* recTauHadDeltaEta_;
  MonitorElement* recTauHadDeltaPhi_;
  MonitorElement* recTauHadDeltaVisMass_absolute_;
  MonitorElement* recTauHadDeltaVisMass_relative_;
  MonitorElement* recTauDecayMode_;

  MonitorElement* recLeadTrackDeltaPt_absolute_;
  MonitorElement* recLeadTrackDeltaPt_relative_;
  MonitorElement* recLeadTrackDeltaEta_;
  MonitorElement* recLeadTrackDeltaPhi_;
  MonitorElement* recLeadTrackNumHits_;
  MonitorElement* recLeadTrackNumPixelHits_;
  MonitorElement* recLeadTrackNormalizedChi2_;
  MonitorElement* recLeadTrackDCA_;
  MonitorElement* recLeadTrackPCApull3d_;

  MonitorElement* recDecayVertexDeltaParl_;
  MonitorElement* recDecayVertexDeltaPerp_;
  MonitorElement* recDecayVertexDeltaZ_;
  MonitorElement* recDecayVertexPull3d_;
  MonitorElement* recDecayVertexNDoF_;
  MonitorElement* recDecayVertexNormalizedChi2_;
};

#endif   
