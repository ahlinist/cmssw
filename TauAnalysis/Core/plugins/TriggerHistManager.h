#ifndef TauAnalysis_Core_TriggerHistManager_h  
#define TauAnalysis_Core_TriggerHistManager_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "TauAnalysis/Core/interface/HistManagerBase.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include <string>
#include <vector>
#include <map>

class TriggerHistManager : public HistManagerBase 
{
 public: 
  explicit TriggerHistManager(const edm::ParameterSet&);
  ~TriggerHistManager();
  
 private: 
//--- histogram booking and filling functions 
//    inherited from HistManagerBase class
  void bookHistogramsImp();
  void fillHistogramsImp(const edm::Event&, const edm::EventSetup&, double);

//--- configuration parameters
  edm::InputTag l1GtReadoutRecordSrc_;
  edm::InputTag l1GtObjectMapRecordSrc_;
  edm::InputTag hltResultsSrc_;

  typedef std::vector<std::string> vstring;
  vstring l1Bits_;
  vstring hltPaths_;

//--- histograms
  std::map<std::string, MonitorElement*> hL1triggerBits_;
  std::map<std::string, MonitorElement*> hHLTresults_;
};

#endif  


