#ifndef TauAnalysis_Core_EventWeightHistManager_h  
#define TauAnalysis_Core_EventWeightHistManager_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "TauAnalysis/Core/interface/HistManagerBase.h"

#include <string>

class EventWeightHistManager : public HistManagerBase 
{
 public:
    explicit EventWeightHistManager(const edm::ParameterSet&);
  ~EventWeightHistManager();
  
 private: 
//--- histogram booking and filling functions 
//    inherited from HistManagerBase class
  void bookHistograms();
  void fillHistograms(const edm::Event&, const edm::EventSetup&, double);

//--- configuration parameters
  std::string dqmDirectory_store_;

//--- histograms
  MonitorElement* hEvtWeightPosUnweighted_;
  MonitorElement* hEvtWeightNegUnweighted_;

  MonitorElement* hEvtWeightPosWeighted_;
  MonitorElement* hEvtWeightNegWeighted_;

  int dqmError_;
};

#endif  


