#ifndef TauAnalysis_Core_MuonHistManager_h  
#define TauAnalysis_Core_MuonHistManager_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "TauAnalysis/Core/interface/HistManagerBase.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "DataFormats/PatCandidates/interface/Muon.h"

#include <vector>
#include <string>

class MuonHistManager : public HistManagerBase 
{
 public:
  
  explicit MuonHistManager(const edm::ParameterSet&);
  ~MuonHistManager();
  
  void bookHistograms(const edm::EventSetup&);
  void fillHistograms(const edm::Event&, const edm::EventSetup&);

 private:

//--- private functions
  void bookMuonHistograms(DQMStore&, MonitorElement*&, MonitorElement*&, MonitorElement*&, const char*);
  
  void fillMuonHistograms(const pat::Muon&, MonitorElement*, MonitorElement*, MonitorElement*);
  void fillMuonIsoHistograms(const pat::Muon&);
  void fillMuonIsoConeSizeDepHistograms(const pat::Muon&);

//--- configuration parameters
  edm::InputTag muonSrc_;
  edm::InputTag vertexSrc_;

  std::string dqmDirectory_store_;

  bool requireGenMuonMatch_;

  unsigned numMuonIsoConeSizes_;
  double muonIsoConeSizeIncr_;
  unsigned numMuonIsoPtThresholds_;
  double muonIsoPtThresholdIncr_;

//--- histograms
  MonitorElement* hMuonPt_;
  MonitorElement* hMuonEta_;
  MonitorElement* hMuonPtVsEta_;
  MonitorElement* hMuonPhi_;

  MonitorElement* hMuonPtCompToGen_;
  MonitorElement* hMuonThetaCompToGen_;
  MonitorElement* hMuonPhiCompToGen_;

  MonitorElement* hMuonTrackIPxy_;
  MonitorElement* hMuonTrackIPz_;

  MonitorElement* hMuonEcalDeposits_;
  MonitorElement* hMuonHcalDeposits_;
  MonitorElement* hMuonCaloDeposits_;
  MonitorElement* hMuonCaloCompatibility_;

  MonitorElement* hMuonNumberOfChambers_;
  MonitorElement* hMuonSegmentCompatibility_;

  MonitorElement* hMuonTrkIsoPt_;
  MonitorElement* hMuonEcalIsoPt_;
  MonitorElement* hMuonHcalIsoPt_;
  MonitorElement* hMuonIsoSumPt_;

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
};

#endif  


