#ifndef TauAnalysis_Core_ElectronHistManager_h  
#define TauAnalysis_Core_ElectronHistManager_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "TauAnalysis/Core/interface/HistManagerBase.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "DataFormats/PatCandidates/interface/Electron.h"

#include <vector>
#include <string>

class ElectronHistManager : public HistManagerBase 
{
 public:
  explicit ElectronHistManager(const edm::ParameterSet&);
  ~ElectronHistManager();
  
 private:
//--- histogram booking and filling functions 
//    inherited from HistManagerBase class
  void bookHistograms();
  void fillHistograms(const edm::Event&, const edm::EventSetup&);

//--- auxiliary functions
  void bookElectronHistograms(DQMStore&, MonitorElement*&, MonitorElement*&, MonitorElement*&, const char*);
  
  void fillElectronHistograms(const pat::Electron&, MonitorElement*, MonitorElement*, MonitorElement*);
  void fillElectronIsoHistograms(const pat::Electron&);
  void fillElectronIsoConeSizeDepHistograms(const pat::Electron&);

//--- configuration parameters
  edm::InputTag electronSrc_;
  edm::InputTag vertexSrc_;
  edm::InputTag genParticleSrc_;

  std::string dqmDirectory_store_;

  bool requireGenElectronMatch_;

  unsigned numElectronIsoConeSizes_;
  double electronIsoConeSizeIncr_;
  unsigned numElectronIsoPtThresholds_;
  double electronIsoPtThresholdIncr_;
  double electronEtaMaxBarrel_;
  double electronEtaMinEndcap_;

//--- histograms
  MonitorElement* hNumElectrons_; 

  MonitorElement* hElectronPt_; 
  MonitorElement* hElectronEta_;
  MonitorElement* hElectronPtVsEta_;
  MonitorElement* hElectronPhi_;

  MonitorElement* hElectronEnCompToGen_;
  MonitorElement* hElectronThetaCompToGen_;
  MonitorElement* hElectronPhiCompToGen_;

  MonitorElement* hElectronMatchingGenParticlePdgId_;

  MonitorElement* hElectronTrackPt_;
  MonitorElement* hElectronTrackIPxy_;
  MonitorElement* hElectronTrackIPz_;

  MonitorElement* hElectronSuperclEnOverTrackMomBarrel_;
  MonitorElement* hElectronSuperclEnOverTrackMomEndcap_;

  MonitorElement* hElectronIdRobust_;
  
//--- IsoDeposits reconstructed from ECAL and HCAL recHits/CaloTowers and reco::Tracks
  MonitorElement* hElectronTrkIsoPt_;
  MonitorElement* hElectronEcalIsoPt_;
  MonitorElement* hElectronEcalIsoPtBarrel_;
  MonitorElement* hElectronEcalIsoPtEndcap_;
  MonitorElement* hElectronHcalIsoPt_;
  MonitorElement* hElectronIsoSumPt_;
  MonitorElement* hElectronTrkIsoPtRel_;
  MonitorElement* hElectronEcalIsoPtRel_;
  MonitorElement* hElectronEcalIsoPtBarrelRel_;
  MonitorElement* hElectronEcalIsoPtEndcapRel_;
  MonitorElement* hElectronHcalIsoPtRel_;
  MonitorElement* hElectronIsoSumPtRel_;
  
  MonitorElement* hElectronTrkIsoValProfile_;
  MonitorElement* hElectronTrkIsoEtaDistProfile_;
  MonitorElement* hElectronTrkIsoPhiDistProfile_;

  MonitorElement* hElectronEcalIsoValProfile_;
  MonitorElement* hElectronEcalIsoEtaDistProfile_;
  MonitorElement* hElectronEcalIsoPhiDistProfile_;

  MonitorElement* hElectronHcalIsoValProfile_;
  MonitorElement* hElectronHcalIsoEtaDistProfile_;
  MonitorElement* hElectronHcalIsoPhiDistProfile_;

  reco::isodeposit::AbsVetos electronTrkIsoParam_;
  reco::isodeposit::AbsVetos electronEcalIsoParam_;
  reco::isodeposit::AbsVetos electronHcalIsoParam_;

  std::vector<MonitorElement*> hElectronTrkIsoPtConeSizeDep_;
  std::vector<MonitorElement*> hElectronEcalIsoPtConeSizeDep_;
  std::vector<MonitorElement*> hElectronHcalIsoPtConeSizeDep_;

//--- IsoDeposits reconstructed from Partcile Flow
  MonitorElement* hElectronParticleFlowIsoPt_;
  MonitorElement* hElectronPFChargedHadronIsoPt_;
  MonitorElement* hElectronPFNeutralHadronIsoPt_;
  MonitorElement* hElectronPFGammaIsoPt_;

  std::vector<MonitorElement*> hElectronParticleFlowIsoPtConeSizeDep_;
  std::vector<MonitorElement*> hElectronPFChargedHadronIsoPtConeSizeDep_;
  std::vector<MonitorElement*> hElectronPFNeutralHadronIsoPtConeSizeDep_;
  std::vector<MonitorElement*> hElectronPFGammaIsoPtConeSizeDep_;

  reco::isodeposit::AbsVetos electronParticleFlowIsoParam_;

  int dqmError_;
};

#endif  


