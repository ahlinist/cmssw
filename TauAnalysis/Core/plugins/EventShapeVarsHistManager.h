#ifndef TauAnalysis_Core_EventShapeVarsHistManager_h  
#define TauAnalysis_Core_EventShapeVarsHistManager_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "TauAnalysis/Core/interface/HistManagerBase.h"

class EventShapeVarsHistManager : public HistManagerBase 
{
 public:
  explicit EventShapeVarsHistManager(const edm::ParameterSet&);
  ~EventShapeVarsHistManager();
  
 private: 
//--- histogram booking and filling functions 
//    inherited from HistManagerBase class
  void bookHistogramsImp();
  void fillHistogramsImp(const edm::Event&, const edm::EventSetup&, double);

//--- configuration parameters
  edm::InputTag src_;

//--- histograms
  MonitorElement* hThrust_;
  //MonitorElement* hOblateness_;
  
  MonitorElement* hCircularity_;
  MonitorElement* hIsotropy_;

  MonitorElement* hSphericity_;
  MonitorElement* hAplanarity_;
  MonitorElement* hC_;
  MonitorElement* hD_;
};

#endif  


