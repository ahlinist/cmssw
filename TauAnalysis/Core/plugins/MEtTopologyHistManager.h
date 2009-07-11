#ifndef TauAnalysis_Core_MEtTopologyHistManager_h  
#define TauAnalysis_Core_MEtTopologyHistManager_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "TauAnalysis/Core/interface/HistManagerBase.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

class MEtTopologyHistManager : public HistManagerBase 
{
 public:  
  explicit MEtTopologyHistManager(const edm::ParameterSet&);
  ~MEtTopologyHistManager();
  
 private:
//--- histogram booking and filling functions 
//    inherited from HistManagerBase class
  void bookHistograms();
  void fillHistograms(const edm::Event&, const edm::EventSetup&);

//--- configuration parameters
  edm::InputTag metTopologySrc_;

  std::string dqmDirectory_store_;

//--- histograms
  MonitorElement* hVratio_;

  int dqmError_;
};

#endif  


