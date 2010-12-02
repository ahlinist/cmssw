#ifndef TauAnalysis_Core_TauHistManager_h  
#define TauAnalysis_Core_TauHistManager_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "TauAnalysis/Core/interface/HistManagerBase.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "TauAnalysis/Core/interface/FakeRateJetWeightExtractor.h"
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
  void bookHistogramsImp();
  void fillHistogramsImp(const edm::Event&, const edm::EventSetup&, double);

//--- auxiliary functions
  void bookTauHistograms(MonitorElement*&, MonitorElement*&, MonitorElement*&, const char*);
  void bookTauIsoConeSizeDepHistograms();
  void bookTauIdEfficiencyHistograms(std::vector<MonitorElement*>&, const char*, const std::vector<std::string>&);

  double getTauWeight(const pat::Tau&);

  void fillTauHistograms(const pat::Tau&, MonitorElement*, MonitorElement*, MonitorElement*, double);
  void fillTauDiscriminatorHistogram(MonitorElement*, const pat::Tau&, const char*, std::map<std::string, bool>&, double);
  void fillTauIsoHistograms(const pat::Tau&, double);
  void fillTauIsoConeSizeDepHistograms(const pat::Tau&, double);
  void fillTauIdEfficiencyHistograms(const pat::Tau&, double, std::vector<MonitorElement*>&, const std::vector<std::string>&);

//--- configuration parameters
  edm::InputTag tauSrc_;
  edm::InputTag vertexSrc_;
  edm::InputTag jetSrc_;
  edm::InputTag genParticleSrc_;

  typedef std::vector<int> vint;
  vint tauIndicesToPlot_;

  bool requireGenTauMatch_;

  bool useHPSpTaNCalgorithm_;

  bool makeIsoPtCtrlHistograms_;
  bool makeIsoPtConeSizeDepHistograms_;

  bool checkWeightConsistency_;

  unsigned numTauIsoConeSizes_;
  float tauIsoConeSizeIncr_;
  unsigned numTauIsoPtThresholds_;
  double tauIsoPtThresholdIncr_;

  typedef std::vector<std::string> vstring;
  vstring makeTauIdEfficiencyHistograms_;
  vstring makeTauFakeRateHistograms_;

//--- "helper" class for accessing weight values
//    associated to second tau decay products
//    (efficiency/fake-rate with which the tau-jet passes the tau id. criteria)
  std::vector<FakeRateJetWeightExtractor<pat::Tau>*> tauJetWeightExtractors_;

//--- histograms
   MonitorElement* hNumTaus_;

  MonitorElement* hTauPt_;
  MonitorElement* hTauEta_;
  MonitorElement* hTauPtVsEta_;
  MonitorElement* hTauPhi_;
  MonitorElement* hTauCharge_;
  MonitorElement* hTauJetRadius_;
  MonitorElement* hTauJetRadiusPtProfile_;
  MonitorElement* hTauJetRadiusEnProfile_;

  MonitorElement* hTauVisMass_;
  MonitorElement* hTauVisMassRes_;
  MonitorElement* hTauVisMassOneProngOnePi0_;
  MonitorElement* hTauVisMassResOneProngOnePi0_;
  MonitorElement* hTauVisMassOneProngTwoPi0s_;
  MonitorElement* hTauVisMassResOneProngTwoPi0s_;
  MonitorElement* hTauVisMassThreeProngNoPi0s_;
  MonitorElement* hTauVisMassResThreeProngNoPi0s_;
  MonitorElement* hTauVisMassThreeProngOnePi0_;
  MonitorElement* hTauVisMassResThreeProngOnePi0_;
  
  MonitorElement* hDistPionEnResOneProngOnePi0_;
  MonitorElement* hDistPionEnResOneProngTwoPi0s_;
  MonitorElement* hDistPionEnResThreeProngNoPi0s_;

  MonitorElement* hTauJetWeightPosLog_;
  MonitorElement* hTauJetWeightNegLog_;
  MonitorElement* hTauJetWeightZero_;
  MonitorElement* hTauJetWeightLinear_;

  MonitorElement* hTauEnCompToGen_;
  MonitorElement* hTauThetaCompToGen_;
  MonitorElement* hTauPhiCompToGen_;

  MonitorElement* hTauMatchingGenParticlePdgId_;
  MonitorElement* hTauMatchingFinalStateGenParticlePdgId_;
  MonitorElement* hTauMatchingGenTauDecayMode_;

  vint skipPdgIdsGenParticleMatch_;

  MonitorElement* hTauNumTracksSignalCone_;
  MonitorElement* hTauNumTracksIsoCone_;

  MonitorElement* hTauLeadTrkPt_;
  MonitorElement* hTauLeadTrkEta_;
  MonitorElement* hTauLeadTrkPhi_;
  MonitorElement* hTauLeadTrkMatchDist_;
  MonitorElement* hTauLeadTrkIPxy_;
  MonitorElement* hTauLeadTrkIPz_;
  MonitorElement* hTauLeadTrkNumHits_;
  MonitorElement* hTauLeadTrkNumPixelHits_;
  MonitorElement* hTauLeadTrkNumStripHits_;

  MonitorElement* hTauDiscriminatorByIsolation_;
  MonitorElement* hTauDiscriminatorByTrackIsolation_;
  MonitorElement* hTauDiscriminatorByEcalIsolation_;
  
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
  MonitorElement* hTauRecVsGenDecayMode_;

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

  MonitorElement* hTauDiscriminatorTaNCloose_;
  MonitorElement* hTauDiscriminatorTaNCmedium_;
  MonitorElement* hTauDiscriminatorTaNCtight_;

  MonitorElement* hTauDiscriminatorHPSloose_;
  MonitorElement* hTauDiscriminatorHPSmedium_;
  MonitorElement* hTauDiscriminatorHPStight_;

  MonitorElement* hTauTrkIsoPt_;
  MonitorElement* hTauEcalIsoPt_;
  MonitorElement* hTauHcalIsoPt_;
  MonitorElement* hTauIsoSumPt_;

  MonitorElement* hTauNumSignalPFChargedHadrons_;
  MonitorElement* hTauNumIsoPFChargedHadrons_;
  MonitorElement* hTauNumPFChargedHadrons_;

  MonitorElement* hTauNumSignalPFGammas_;
  MonitorElement* hTauNumIsoPFGammas_;
  MonitorElement* hTauNumPFGammas_;

  MonitorElement* hTauDeltaRnearestJet_;

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

  MonitorElement* hTauPFChargedHadronIsoPtCtrl_;
  MonitorElement* hTauPFGammaIsoPtCtrl_;

  std::vector<MonitorElement*> hTauParticleFlowIsoPtConeSizeDep_;
  std::vector<MonitorElement*> hTauPFChargedHadronIsoPtConeSizeDep_;
  std::vector<MonitorElement*> hTauPFNeutralHadronIsoPtConeSizeDep_;
  std::vector<MonitorElement*> hTauPFGammaIsoPtConeSizeDep_;

  reco::isodeposit::AbsVetos tauParticleFlowIsoParam_;

//--- parametrized values for tau id. efficiencies and fake-rates
//    stored in the pat::Tau
  std::vector<MonitorElement*> hTauIdEfficiencies_;
  std::vector<MonitorElement*> hTauFakeRates_;
};

#endif  


