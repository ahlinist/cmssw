#ifndef TauAnalysis_Core_MEtTopologyHistManager_h  
#define TauAnalysis_Core_MEtTopologyHistManager_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "TauAnalysis/Core/interface/HistManagerBase.h"
#include "DQMServices/Core/interface/MonitorElement.h"

class HtRatioHistManager : public HistManagerBase 
{
 public:  
  explicit HtRatioHistManager(const edm::ParameterSet&);
  ~HtRatioHistManager();
  
 private:
//--- histogram booking and filling functions 
//    inherited from HistManagerBase class
  void bookHistogramsImp();
  void fillHistogramsImp(const edm::Event&, const edm::EventSetup&, double);

//--- configuration parameters
  edm::InputTag htRatioSrc_;

//--- histograms
  MonitorElement* hRatio_;
};

#endif  


