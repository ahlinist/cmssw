#ifndef TauAnalysis_Core_BinGridHistManager_h  
#define TauAnalysis_Core_BinGridHistManager_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/Core/interface/HistManagerBase.h"

#include "TauAnalysis/BgEstimationTools/interface/ObjValVectorExtractorBase.h"
#include "TauAnalysis/Core/interface/BinGrid.h"

#include <string>
#include <vector>
#include <map>

class BinGridHistManager : public HistManagerBase 
{
 public:  
  explicit BinGridHistManager(const edm::ParameterSet&);
  ~BinGridHistManager();
  
 private:
//--- histogram booking and filling functions 
//    inherited from HistManagerBase class
  void bookHistogramsImp();
  void fillHistogramsImp(const edm::Event&, const edm::EventSetup&, double);

//--- configuration parameters
  typedef std::vector<edm::ParameterSet> vParameterSet;
  vParameterSet cfgHistManagers_;

//--- binning service
  ObjValVectorExtractorBase* objValExtractor_;
  BinGrid* binGrid_;

//--- histograms managers
  typedef std::vector<HistManagerBase*> vHistManager;
  std::map<unsigned, vHistManager> histManagers_;
};

#endif  


