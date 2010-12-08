#ifndef TauAnalysis_Core_MuonHistManager_h  
#define TauAnalysis_Core_MuonHistManager_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "DataFormats/PatCandidates/interface/Muon.h"

#include "TauAnalysis/Core/interface/HistManagerBase.h"

#include "TauAnalysis/RecoTools/interface/ParticlePFIsolationExtractor.h"

#include <vector>
#include <string>

class MuonHistManager : public HistManagerBase 
{
 public:  
  explicit MuonHistManager(const edm::ParameterSet&);
  ~MuonHistManager();
  
 private:
//--- histogram booking and filling functions 
//    inherited from HistManagerBase class
  void bookHistogramsImp();
  void fillHistogramsImp(const edm::Event&, const edm::EventSetup&, double);

//--- auxiliary functions
  void bookMuonHistograms(MonitorElement*&, MonitorElement*&, MonitorElement*&, const char*);
  void bookMuonIsoConeSizeDepHistograms();

  double getMuonWeight(const pat::Muon&);

  void fillMuonHistograms(const pat::Muon&, MonitorElement*, MonitorElement*, MonitorElement*, double);
  void fillMuonIsoHistograms(const pat::Muon&, const reco::PFCandidateCollection&, double);
  void fillMuonIsoConeSizeDepHistograms(const pat::Muon&, double);

//--- configuration parameters
  edm::InputTag muonSrc_;
  edm::InputTag vertexSrc_;
  edm::InputTag beamSpotSrc_;
  edm::InputTag jetSrc_;
  edm::InputTag pfCandidateSrc_;
  edm::InputTag genParticleSrc_;

  bool requireGenMuonMatch_;

  typedef std::vector<int> vint;
  vint skipPdgIdsGenParticleMatch_;

  PATMuonPFIsolationExtractor* pfCombIsoExtractor_;

  bool makeIsoPtCtrlHistograms_;
  bool makeIsoPtConeSizeDepHistograms_;

  unsigned numMuonIsoConeSizes_;
  double muonIsoConeSizeIncr_;
  unsigned numMuonIsoPtThresholds_;
  double muonIsoPtThresholdIncr_;

//--- histograms
  MonitorElement* hNumMuons_;

  MonitorElement* hMuonPt_;
  MonitorElement* hMuonEta_;
  MonitorElement* hMuonPtVsEta_;
  MonitorElement* hMuonPtVsAbsEta_;
  MonitorElement* hMuonPhi_;
  MonitorElement* hMuonCharge_;

  MonitorElement* hMuonTypeGlobal_;
  MonitorElement* hMuonTypeTracker_;
  MonitorElement* hMuonTypeStandAlone_;
  MonitorElement* hMuonTypeCalo_;

  MonitorElement* hMuonWeightPosLog_;
  MonitorElement* hMuonWeightNegLog_;
  MonitorElement* hMuonWeightZero_;
  MonitorElement* hMuonWeightLinear_;

  MonitorElement* hGenMuonPt_;
  MonitorElement* hGenMuonEta_;
  MonitorElement* hGenMuonPhi_;

  MonitorElement* hMuonPtCompToGen_;
  MonitorElement* hMuonThetaCompToGen_;
  MonitorElement* hMuonPhiCompToGen_;

  MonitorElement* hMuonMatchingGenParticlePdgId_;

  MonitorElement* hMuonNumTrackerHits_;
  MonitorElement* hMuonNumPixelHits_;
  MonitorElement* hMuonNumStripHits_;

  MonitorElement* hMuonTrackIPxy_;
  MonitorElement* hMuonTrackIPz_;

  MonitorElement* hMuonTrackIPxyBeamSpot_;
  MonitorElement* hMuonGlobalTrackIPxyBeamSpot_;

  MonitorElement* hMuonGlobalTrackChi2red_; // (normalized) chi^2 of global muon track fit per degree of freedom

  MonitorElement* hMuonTime_;

  MonitorElement* hMuonEcalDeposits_;
  MonitorElement* hMuonHcalDeposits_;
  MonitorElement* hMuonCaloDeposits_;
  MonitorElement* hMuonCaloCompatibility_;

  MonitorElement* hMuonNumberOfChambers_;
  MonitorElement* hMuonNumberOfSegments_;
  MonitorElement* hMuonSegmentCompatibility_;

//--- IsoDeposits reconstructed from ECAL and HCAL recHits/CaloTowers and reco::Tracks
  MonitorElement* hMuonTrkIsoPt_;
  MonitorElement* hMuonTrkIsoPtVsMuonPt_;
  MonitorElement* hMuonTrkIsoPtRel_;
  MonitorElement* hMuonEcalIsoPt_;
  MonitorElement* hMuonEcalIsoPtVsMuonPt_;
  MonitorElement* hMuonEcalIsoPtRel_;
  MonitorElement* hMuonHcalIsoPt_;
  MonitorElement* hMuonHcalIsoPtRel_;
  MonitorElement* hMuonTrackEcalIsoSumPt_;
  MonitorElement* hMuonTrackEcalIsoSumPtVsMuonPt_;
  MonitorElement* hMuonTrackEcalIsoSumPtRel_;
  MonitorElement* hMuonTrackEcalHcalIsoSumPt_;
  MonitorElement* hMuonTrackEcalHcalIsoSumPtVsMuonPt_;
  MonitorElement* hMuonTrackEcalHcalIsoSumPtRel_;

  MonitorElement* hMuonDeltaRnearestJet_;

  MonitorElement* hMuonTrkIsoValProfile_;
  MonitorElement* hMuonTrkIsoEtaDistProfile_;
  MonitorElement* hMuonTrkIsoPhiDistProfile_;

  MonitorElement* hMuonEcalIsoValProfile_;
  MonitorElement* hMuonEcalIsoEtaDistProfile_;
  MonitorElement* hMuonEcalIsoPhiDistProfile_;

  MonitorElement* hMuonHcalIsoValProfile_;
  MonitorElement* hMuonHcalIsoEtaDistProfile_;
  MonitorElement* hMuonHcalIsoPhiDistProfile_;

  std::vector<MonitorElement*> hMuonTrkIsoPtConeSizeDep_;
  std::vector<MonitorElement*> hMuonEcalIsoPtConeSizeDep_;
  std::vector<MonitorElement*> hMuonHcalIsoPtConeSizeDep_;

  reco::isodeposit::AbsVetos muonTrkIsoParam_;
  reco::isodeposit::AbsVetos muonEcalIsoParam_;
  reco::isodeposit::AbsVetos muonHcalIsoParam_;

//--- IsoDeposits reconstructed from Partcile Flow
  MonitorElement* hMuonParticleFlowIsoPt_;  
  MonitorElement* hMuonParticleFlowIsoPtRel_;  
  MonitorElement* hMuonPFChargedHadronIsoPt_;
  MonitorElement* hMuonPFChargedHadronIsoPtRel_;
  MonitorElement* hMuonPFNeutralHadronIsoPt_;
  MonitorElement* hMuonPFNeutralHadronIsoPtRel_;
  MonitorElement* hMuonPFGammaIsoPt_;
  MonitorElement* hMuonPFGammaIsoPtRel_;

  MonitorElement* hMuonPFCombIsoPt_; 
  MonitorElement* hMuonPFCombIsoPtRel_;  

  MonitorElement* hMuonPFChargedHadronIsoPtCtrl_;
  MonitorElement* hMuonPFGammaIsoPtCtrl_;

  std::vector<MonitorElement*> hMuonParticleFlowIsoPtConeSizeDep_;
  std::vector<MonitorElement*> hMuonPFChargedHadronIsoPtConeSizeDep_;
  std::vector<MonitorElement*> hMuonPFNeutralHadronIsoPtConeSizeDep_;
  std::vector<MonitorElement*> hMuonPFGammaIsoPtConeSizeDep_;

  reco::isodeposit::AbsVetos muonParticleFlowIsoParam_;
};

#endif  


