#ifndef TauAnalysis_Core_SysUncertaintyHistManager_h  
#define TauAnalysis_Core_SysUncertaintyHistManager_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/Core/interface/HistManagerBase.h"

#include <string>
#include <vector>
#include <map>

class SysUncertaintyHistManager : public HistManagerBase 
{
 public:  
  explicit SysUncertaintyHistManager(const edm::ParameterSet&);
  ~SysUncertaintyHistManager();
  
 private:
//--- histogram booking and filling functions 
//    inherited from HistManagerBase class
  void bookHistogramsImp();
  void fillHistogramsImp(const edm::Event&, const edm::EventSetup&, double);

//--- configuration parameters
  typedef std::vector<edm::ParameterSet> vParameterSet;
  vParameterSet cfgHistManagerEntries_;

//--- histograms managers
  typedef std::vector<HistManagerBase*> vHistManager;
  std::map<std::string, vHistManager> histManagers_;
};

#endif  


