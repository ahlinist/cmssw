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
  
  void bookHistograms(const edm::EventSetup&);
  void fillHistograms(const edm::Event&, const edm::EventSetup&);

 private:

//--- private functions
  void bookTauHistograms(DQMStore& dqmStore, MonitorElement*& hTauPt, MonitorElement*& hTauEta, MonitorElement*& hTauPhi, const char* histoSetName);
  
  void fillTauHistograms(const std::vector<pat::Tau>& patTaus, MonitorElement* hTauPt, MonitorElement* hTauEta, MonitorElement* hTauPhi);
  void fillTauIsoHistograms(const std::vector<pat::Tau>& patTaus);
  void fillTauIsoConeSizeDepHistograms(const std::vector<pat::Tau>& patTaus);

//--- configuration parameters
  edm::InputTag tauSrc_;
  edm::InputTag vertexSrc_;

  std::string dqmDirectory_store_;

  bool requireGenTauMatch_;

  unsigned numTauIsoConeSizes_;
  float tauIsoConeSizeIncr_;
  unsigned numTauIsoPtThresholds_;
  double tauIsoPtThresholdIncr_;

//--- histograms
  MonitorElement* hTauPt_;
  MonitorElement* hTauEta_;
  MonitorElement* hTauPtVsEta_;
  MonitorElement* hTauPhi_;

  MonitorElement* hTauEnCompToGen_;
  MonitorElement* hTauThetaCompToGen_;
  MonitorElement* hTauPhiCompToGen_;

  MonitorElement* hTauLeadTrkPt_;
  MonitorElement* hTauLeadTrkEta_;
  MonitorElement* hTauLeadTrkPhi_;
  MonitorElement* hTauLeadTrkMatchDist_;
  MonitorElement* hTauLeadTrkIPxy_;
  MonitorElement* hTauLeadTrkIPz_;

  MonitorElement* hTauTrkIsoEnProfile_;
  MonitorElement* hTauTrkIsoPtProfile_;
  MonitorElement* hTauTrkIsoEtaDistProfile_;
  MonitorElement* hTauTrkIsoPhiDistProfile_;

  MonitorElement* hTauTrkIsoPt_;
  MonitorElement* hTauEcalIsoPt_;
  MonitorElement* hTauHcalIsoPt_;
  MonitorElement* hTauIsoSumPt_;

  std::vector<MonitorElement*> hTauTrkIsoPtConeSizeDep_;
  std::vector<MonitorElement*> hTauEcalIsoPtConeSizeDep_;
  std::vector<MonitorElement*> hTauHcalIsoPtConeSizeDep_;
};

#endif  


