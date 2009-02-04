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
  void bookMuonHistograms(DQMStore& dqmStore, MonitorElement*& hMuonPt, MonitorElement*& hMuonEta, MonitorElement*& hMuonPhi, const char* histoSetName);
  
  void fillMuonHistograms(const std::vector<pat::Muon>& patMuons, MonitorElement* hMuonPt, MonitorElement* hMuonEta, MonitorElement* hMuonPhi);
  void fillMuonIsoHistograms(const std::vector<pat::Muon>& patMuons);
  void fillMuonIsoConeSizeDepHistograms(const std::vector<pat::Muon>& patMuons);

//--- configuration parameters
  edm::InputTag muonSrc_;

  std::string dqmDirectory_store_;

  bool requireGenMuonMatch_;

  unsigned numMuonIsoConeSizes_;
  float muonIsoConeSizeIncr_;

//--- histograms
  MonitorElement* hMuonPt_;
  MonitorElement* hMuonEta_;
  MonitorElement* hMuonPtVsEta_;
  MonitorElement* hMuonPhi_;

  MonitorElement* hMuonTrackIP_;

  MonitorElement* hMuonEcalDeposits_;
  MonitorElement* hMuonHcalDeposits_;
  MonitorElement* hMuonCaloDeposits_;
  MonitorElement* hMuonCaloCompatibility_;

  MonitorElement* hMuonNumberOfChambers_;
  MonitorElement* hMuonSegmentCompatibility_;

  MonitorElement* hMuonTrkIsoPt_;
  MonitorElement* hMuonEcalIsoPt_;
  MonitorElement* hMuonHcalIsoPt_;

  std::vector<MonitorElement*> hMuonTrkIsoPtConeSizeDep_;
  std::vector<MonitorElement*> hMuonEcalIsoPtConeSizeDep_;
  std::vector<MonitorElement*> hMuonHcalIsoPtConeSizeDep_;
};

#endif  


