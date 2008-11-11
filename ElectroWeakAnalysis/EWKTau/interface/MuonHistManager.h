#ifndef MuonHistManager_h  
#define MuonHistManager_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "DataFormats/PatCandidates/interface/Muon.h"

#include <vector>
#include <string>

class MuonHistManager : public edm::EDAnalyzer {

 public:
  
  explicit MuonHistManager(const edm::ParameterSet&);
  ~MuonHistManager();
  
  virtual void beginJob(const edm::EventSetup&);
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();

 private:

//--- private functions
  void bookMuonHistograms(DQMStore& dqmStore, MonitorElement*& hMuonPt, MonitorElement*& hMuonEta, MonitorElement*& hMuonPhi, const char* histoSetName);
  
  void fillMuonHistograms(const std::vector<pat::Muon>& patMuons, MonitorElement* hMuonPt, MonitorElement* hMuonEta, MonitorElement* hMuonPhi, const char* selVar);
  void fillMuonIsoHistograms(const std::vector<pat::Muon>& patMuons);
  void fillMuonIsoConeSizeDepHistograms(const std::vector<pat::Muon>& patMuons);

//--- configuration parameters
  edm::InputTag muonSrc_;

  std::string outputFileName_;
  std::string outputDirectoryName_;

  std::string muonKineSelVar_;
  std::string muonHLTmatchSelVar_;
  std::string muonTrkIsoSelVar_;
  std::string muonEcalIsoSelVar_;
  std::string muonHcalIsoSelVar_;
  std::string muonIdSelVar_;
  std::string muonTrkIpSelVar_;

  bool requireGenMuonMatch_;

  unsigned numMuonIsoConeSizes_;
  float muonIsoConeSizeIncr_;

//--- histograms
  MonitorElement* hMuonPt_;
  MonitorElement* hMuonEta_;
  MonitorElement* hMuonPtVsEta_;
  MonitorElement* hMuonPhi_;

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

  MonitorElement* hMuonKineSelPt_;
  MonitorElement* hMuonKineSelEta_;
  MonitorElement* hMuonKineSelPhi_;

  MonitorElement* hMuonHLTmatchSelPt_;
  MonitorElement* hMuonHLTmatchSelEta_;
  MonitorElement* hMuonHLTmatchSelPhi_;

  MonitorElement* hMuonTrkIsoSelPt_;
  MonitorElement* hMuonTrkIsoSelEta_;
  MonitorElement* hMuonTrkIsoSelPhi_;

  MonitorElement* hMuonEcalIsoSelPt_;
  MonitorElement* hMuonEcalIsoSelEta_;
  MonitorElement* hMuonEcalIsoSelPhi_;

  MonitorElement* hMuonHcalIsoSelPt_;
  MonitorElement* hMuonHcalIsoSelEta_;
  MonitorElement* hMuonHcalIsoSelPhi_;

  MonitorElement* hMuonIdSelPt_; 
  MonitorElement* hMuonIdSelEta_;
  MonitorElement* hMuonIdSelPhi_;

  MonitorElement* hMuonTrkIpSelPt_;
  MonitorElement* hMuonTrkIpSelEta_;
  MonitorElement* hMuonTrkIpSelPhi_;
};

#endif  


