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
  
  void bookHistograms(const edm::EventSetup&);
  void fillHistograms(const edm::Event&, const edm::EventSetup&);

 private:

//--- private functions
  void bookElectronHistograms(DQMStore& dqmStore, 
			      MonitorElement*& hElectronPt, MonitorElement*& hElectronEta, MonitorElement*& hElectronPhi, const char* histoSetName);
  
  void fillElectronHistograms(const std::vector<pat::Electron>& patElectrons, 
			      MonitorElement* hElectronPt, MonitorElement* hElectronEta, MonitorElement* hElectronPhi);
  void fillElectronIsoHistograms(const std::vector<pat::Electron>& patElectrons);
  void fillElectronIsoConeSizeDepHistograms(const std::vector<pat::Electron>& patElectrons);

//--- configuration parameters
  edm::InputTag electronSrc_;
  edm::InputTag vertexSrc_;

  std::string dqmDirectory_store_;

  bool requireGenElectronMatch_;

  unsigned numElectronIsoConeSizes_;
  float electronIsoConeSizeIncr_;
  unsigned numElectronIsoPtThresholds_;
  double electronIsoPtThresholdIncr_;

//--- histograms
  MonitorElement* hElectronPt_; 
  MonitorElement* hElectronEta_;
  MonitorElement* hElectronPtVsEta_;
  MonitorElement* hElectronPhi_;

  MonitorElement* hElectronTrackPt_;
  MonitorElement* hElectronTrackIPxy_;
  MonitorElement* hElectronTrackIPz_;

  MonitorElement* hElectronTrkIsoPt_;
  MonitorElement* hElectronEcalIsoPt_;
  MonitorElement* hElectronHcalIsoPt_;
  MonitorElement* hElectronIsoSumPt_;
  
  MonitorElement* hElectronTrkIsoValProfile_;
  MonitorElement* hElectronTrkIsoEtaDistProfile_;
  MonitorElement* hElectronTrkIsoPhiDistProfile_;

  MonitorElement* hElectronEcalIsoValProfile_;
  MonitorElement* hElectronEcalIsoEtaDistProfile_;
  MonitorElement* hElectronEcalIsoPhiDistProfile_;

  MonitorElement* hElectronHcalIsoValProfile_;
  MonitorElement* hElectronHcalIsoEtaDistProfile_;
  MonitorElement* hElectronHcalIsoPhiDistProfile_;

  std::vector<MonitorElement*> hElectronTrkIsoPtConeSizeDep_;
  std::vector<MonitorElement*> hElectronEcalIsoPtConeSizeDep_;
  std::vector<MonitorElement*> hElectronHcalIsoPtConeSizeDep_;
};

#endif  


