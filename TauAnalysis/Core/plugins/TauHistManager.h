#ifndef TauAnalysis_Core_TauHistManager_h  
#define TauAnalysis_Core_TauHistManager_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "TauAnalysis/Core/interface/HistManagerBase.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

#include <vector>
#include <string>

class TauHistManager : public HistManagerBase 
{
 public:  
  explicit TauHistManager(const edm::ParameterSet&);
  ~TauHistManager();
  
 private:
//--- histogram booking and filling functions 
//    inherited from HistManagerBase class
  void bookHistograms();
  void fillHistograms(const edm::Event&, const edm::EventSetup&);

//--- auxiliary functions
  void bookTauHistograms(DQMStore&, MonitorElement*&, MonitorElement*&, MonitorElement*&, const char*);
  
  void fillTauHistograms(const pat::Tau&, MonitorElement*, MonitorElement*, MonitorElement*);
  void fillTauDiscriminatorHistogram(MonitorElement*, const pat::Tau&, const char*, std::map<std::string, bool>&);
  void fillTauIsoHistograms(const pat::Tau&);
  void fillTauIsoConeSizeDepHistograms(const pat::Tau&);

//--- configuration parameters
  edm::InputTag tauSrc_;
  edm::InputTag vertexSrc_;
  edm::InputTag genParticleSrc_;

  typedef std::vector<int> vint;
  vint tauIndicesToPlot_;

  std::string dqmDirectory_store_;

  bool requireGenTauMatch_;

  unsigned numTauIsoConeSizes_;
  float tauIsoConeSizeIncr_;
  unsigned numTauIsoPtThresholds_;
  double tauIsoPtThresholdIncr_;

//--- histograms
  MonitorElement* hNumTaus_;

  MonitorElement* hTauPt_;
  MonitorElement* hTauEta_;
  MonitorElement* hTauPtVsEta_;
  MonitorElement* hTauPhi_;

  MonitorElement* hTauEnCompToGen_;
  MonitorElement* hTauThetaCompToGen_;
  MonitorElement* hTauPhiCompToGen_;

  MonitorElement* hTauMatchingGenParticlePdgId_;

  MonitorElement* hTauNumTracksSignalCone_;
  MonitorElement* hTauNumTracksIsoCone_;

  MonitorElement* hTauLeadTrkPt_;
  MonitorElement* hTauLeadTrkEta_;
  MonitorElement* hTauLeadTrkPhi_;
  MonitorElement* hTauLeadTrkMatchDist_;
  MonitorElement* hTauLeadTrkIPxy_;
  MonitorElement* hTauLeadTrkIPz_;

  MonitorElement* hTauDiscriminatorAgainstElectrons_;
  MonitorElement* hTauEmFraction_;
  MonitorElement* hTauHcalTotOverPLead_;
  MonitorElement* hTauHcalMaxOverPLead_;
  MonitorElement* hTauHcal3x3OverPLead_;
  MonitorElement* hTauEcalStripSumEOverPLead_;
  MonitorElement* hTauBremsRecoveryEOverPLead_;
  MonitorElement* hTauCaloEOverPLead_;

  MonitorElement* hTauDiscriminatorAgainstMuons_;

  MonitorElement* hTauRecDecayMode_;

  MonitorElement* hTauTaNCoutputOneProngNoPi0s_;
  MonitorElement* hTauTaNCoutputOneProngOnePi0_;
  MonitorElement* hTauTaNCoutputOneProngTwoPi0s_;
  MonitorElement* hTauTaNCoutputThreeProngNoPi0s_;
  MonitorElement* hTauTaNCoutputThreeProngOnePi0_;
  MonitorElement* hTauTaNCoutputTransform_;

  MonitorElement* hTauDiscriminatorTaNCfrOnePercent_;
  MonitorElement* hTauDiscriminatorTaNCfrHalfPercent_;
  MonitorElement* hTauDiscriminatorTaNCfrQuarterPercent_;
  MonitorElement* hTauDiscriminatorTaNCfrTenthPercent_;

  MonitorElement* hTauTrkIsoPt_;
  MonitorElement* hTauEcalIsoPt_;
  MonitorElement* hTauHcalIsoPt_;
  MonitorElement* hTauIsoSumPt_;

  MonitorElement* hTauTrkIsoEnProfile_;
  MonitorElement* hTauTrkIsoPtProfile_;
  MonitorElement* hTauTrkIsoEtaDistProfile_;
  MonitorElement* hTauTrkIsoPhiDistProfile_;

//--- IsoDeposits reconstructed from ECAL and HCAL recHits/CaloTowers and reco::Tracks
//    (not implemented in reco::PFTau/pat::Tau yet...)

//--- IsoDeposits reconstructed from Partcile Flow
  MonitorElement* hTauParticleFlowIsoPt_;
  MonitorElement* hTauPFChargedHadronIsoPt_;
  MonitorElement* hTauPFNeutralHadronIsoPt_;
  MonitorElement* hTauPFGammaIsoPt_;

  std::vector<MonitorElement*> hTauParticleFlowIsoPtConeSizeDep_;
  std::vector<MonitorElement*> hTauPFChargedHadronIsoPtConeSizeDep_;
  std::vector<MonitorElement*> hTauPFNeutralHadronIsoPtConeSizeDep_;
  std::vector<MonitorElement*> hTauPFGammaIsoPtConeSizeDep_;

  reco::isodeposit::AbsVetos tauParticleFlowIsoParam_;

  int dqmError_;
};

#endif  


