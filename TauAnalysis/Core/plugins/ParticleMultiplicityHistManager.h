#ifndef TauAnalysis_Core_ParticleMultiplicityHistManager_h  
#define TauAnalysis_Core_ParticleMultiplicityHistManager_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "TauAnalysis/Core/interface/HistManagerBase.h"

#include <string>
#include <vector>

class ParticleMultiplicityHistManager : public HistManagerBase 
{
  struct histogramEntryType
  {
    histogramEntryType(const edm::ParameterSet&);
    ~histogramEntryType() {}
    std::string name_;
    std::string title_;
    unsigned numBinsX_;
    double xMin_;
    double xMax_;
    edm::InputTag src_;
    MonitorElement* me_;
  };

 public:
  explicit ParticleMultiplicityHistManager(const edm::ParameterSet&);
  ~ParticleMultiplicityHistManager();
  
 private: 
//--- histogram booking and filling functions 
//    inherited from HistManagerBase class
  void bookHistogramsImp();
  void fillHistogramsImp(const edm::Event&, const edm::EventSetup&, double);

//--- histograms
  std::vector<histogramEntryType> histogramEntries_;
};

#endif  


