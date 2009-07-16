#ifndef TauAnalysis_Core_PATLeptonRecoilEnergyHistManager_h  
#define TauAnalysis_Core_PATLeptonRecoilEnergyHistManager_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "TauAnalysis/Core/interface/HistManagerBase.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

template<typename T1, typename T2>
class PATLeptonRecoilEnergyHistManager : public HistManagerBase 
{
 public:  
  explicit PATLeptonRecoilEnergyHistManager(const edm::ParameterSet&);
  ~PATLeptonRecoilEnergyHistManager();
  
 private:
//--- histogram booking and filling functions 
//    inherited from HistManagerBase class
  void bookHistograms();
  void fillHistograms(const edm::Event&, const edm::EventSetup&);

//--- configuration parameters
  edm::InputTag leptonRecoilEnergySrc_;

  std::string dqmDirectory_store_;

//--- histograms
  MonitorElement* hEtSum_;
  MonitorElement* hNumObjects_;

  int dqmError_;
};

#endif  


