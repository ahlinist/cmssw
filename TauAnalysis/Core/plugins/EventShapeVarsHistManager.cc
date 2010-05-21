#include "TauAnalysis/Core/plugins/EventShapeVarsHistManager.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "TauAnalysis/Core/interface/histManagerAuxFunctions.h"

#include <TMath.h>

EventShapeVarsHistManager::EventShapeVarsHistManager(const edm::ParameterSet& cfg)
  : HistManagerBase(cfg)
{
  //std::cout << "<EventShapeVarsHistManager::EventShapeVarsHistManager>:" << std::endl;
  
  src_ = cfg.getParameter<edm::InputTag>("src");
  //std::cout << " src = " << src_ << std::endl;
}

EventShapeVarsHistManager::~EventShapeVarsHistManager()
{
//--- nothing to be done yet...
}

void EventShapeVarsHistManager::bookHistogramsImp()
{
  //std::cout << "<EventShapeVarsHistManager::bookHistogramsImp>:" << std::endl;
  
  hThrust_ = book1D("Thrust", "Thrust", 52, 0.49, 1.01);
  //hOblateness_ = book1D("Oblateness", "Oblateness", 102, -0.01, 1.01);
  
  hCircularity_ = book1D("Circularity", "Circularity", 52, 0.49, 1.01);
  hIsotropy_ = book1D("Isotropy", "Isotropy", 102, -0.01, 1.01);

  hSphericity_ = book1D("Sphericity", "Sphericity", 102, -0.01, 1.01);
  hAplanarity_ = book1D("Aplanarity", "Aplanarity", 52, -0.01, 0.51);
  hC_ = book1D("C", "C", 102, -0.01, 1.01);
  hD_ = book1D("D", "D", 102, -0.01, 1.01);
}

void fillHistogram(const edm::Event& evt, edm::InputTag& src, const char* instanceName, MonitorElement* me, double evtWeight)
{
  std::string instanceName_string = instanceName;

  edm::Handle<double> eventShapeVar;
  evt.getByLabel(src.label(), instanceName_string, eventShapeVar);

  me->Fill(*eventShapeVar, evtWeight);
}

void EventShapeVarsHistManager::fillHistogramsImp(const edm::Event& evt, const edm::EventSetup& es, double evtWeight)

{  
  //std::cout << "<EventShapeVarsHistManager::fillHistogramsImp>:" << std::endl; 

  fillHistogram(evt, src_, "thrust", hThrust_, evtWeight);
  //fillHistogram(evt, src_, "oblateness", hOblateness_, evtWeight);
  
  fillHistogram(evt, src_, "isotropy", hIsotropy_, evtWeight);
  fillHistogram(evt, src_, "circularity", hCircularity_, evtWeight);
  
  fillHistogram(evt, src_, "sphericity", hSphericity_, evtWeight);
  fillHistogram(evt, src_, "aplanarity", hAplanarity_, evtWeight);
  fillHistogram(evt, src_, "C", hC_, evtWeight);
  fillHistogram(evt, src_, "D", hD_, evtWeight);
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, EventShapeVarsHistManager, "EventShapeVarsHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, EventShapeVarsHistManager, "EventShapeVarsHistManager");

#include "TauAnalysis/Core/interface/HistManagerAdapter.h"

typedef HistManagerAdapter<EventShapeVarsHistManager> EventShapeVarsAnalyzer;

DEFINE_FWK_MODULE(EventShapeVarsAnalyzer);
