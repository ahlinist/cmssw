#ifndef TauAnalysis_Core_GenPhaseSpaceEventInfoHistManager_h  
#define TauAnalysis_Core_GenPhaseSpaceEventInfoHistManager_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "TauAnalysis/Core/interface/HistManagerBase.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include <vector>
#include <string>

class GenPhaseSpaceEventInfoHistManager : public HistManagerBase 
{
 public:
  
  explicit GenPhaseSpaceEventInfoHistManager(const edm::ParameterSet&);
  ~GenPhaseSpaceEventInfoHistManager();
  
  void bookHistograms(const edm::EventSetup&);
  void fillHistograms(const edm::Event&, const edm::EventSetup&);

 private:

//--- configuration parameters
  edm::InputTag src_;

  std::string dqmDirectory_store_;

//--- histograms
  MonitorElement* hPtHat_;
  MonitorElement* hLeadingElectronPt_;
  MonitorElement* hLeadingMuonPt_;
  MonitorElement* hLeadingTauLeptonPt_;
  MonitorElement* hLeadingElectronPtVsPtHat_;
  MonitorElement* hLeadingMuonPtVsPtHat_;
  MonitorElement* hLeadingTauLeptonPtVsPtHat_;
};

#endif  


