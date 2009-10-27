#include "TauAnalysis/Core/plugins/EventWeightHistManager.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "TauAnalysis/Core/interface/histManagerAuxFunctions.h"

#include <TMath.h>

EventWeightHistManager::EventWeightHistManager(const edm::ParameterSet& cfg)
  : HistManagerBase(cfg)
{
  //std::cout << "<EventWeightHistManager::EventWeightHistManager>:" << std::endl;
}

EventWeightHistManager::~EventWeightHistManager()
{
//--- nothing to be done yet...
}

void EventWeightHistManager::bookHistogramsImp()
{
  //std::cout << "<EventWeightHistManager::bookHistogramsImp>:" << std::endl;
  
  bookWeightHistograms(*dqmStore_, "EvtWeight", "Event Weight", 
		       hEvtWeightPosUnweighted_, hEvtWeightPosWeighted_, hEvtWeightNegUnweighted_, hEvtWeightNegWeighted_);
}



void EventWeightHistManager::fillHistogramsImp(const edm::Event& evt, const edm::EventSetup& es, double evtWeight)

{  
  //std::cout << "<EventWeightHistManager::fillHistogramsImp>:" << std::endl; 

  fillWeightHistograms(hEvtWeightPosUnweighted_, hEvtWeightPosWeighted_, hEvtWeightNegUnweighted_, hEvtWeightNegWeighted_, evtWeight);
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, EventWeightHistManager, "EventWeightHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, EventWeightHistManager, "EventWeightHistManager");

#include "TauAnalysis/Core/interface/HistManagerAdapter.h"

typedef HistManagerAdapter<EventWeightHistManager> EventWeightAnalyzer;

DEFINE_ANOTHER_FWK_MODULE(EventWeightAnalyzer);
