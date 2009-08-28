#include "TauAnalysis/Core/plugins/EventWeightHistManager.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include <TMath.h>

EventWeightHistManager::EventWeightHistManager(const edm::ParameterSet& cfg)
  : dqmError_(0)
{
  //std::cout << "<EventWeightHistManager::EventWeightHistManager>:" << std::endl;

  dqmDirectory_store_ = cfg.getParameter<std::string>("dqmDirectory_store");
  //std::cout << " dqmDirectory_store = " << dqmDirectory_store_ << std::endl;
}

EventWeightHistManager::~EventWeightHistManager()
{
//--- nothing to be done yet...
}

void EventWeightHistManager::bookHistograms()
{
  //std::cout << "<EventWeightHistManager::bookHistograms>:" << std::endl;

  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("bookHistograms") << " Failed to access dqmStore --> histograms will NOT be booked !!";
    dqmError_ = 1;
    return;
  }

  DQMStore& dqmStore = (*edm::Service<DQMStore>());
  
  dqmStore.setCurrentFolder(dqmDirectory_store_);
  
  hEvtWeightPosUnweighted_ = dqmStore.book1D("EvtWeightPosUnweighted", "log10(|Event Weight|), Event Weight #geq 0 (unweighted)", 74, -6.2, 1.2);
  hEvtWeightNegUnweighted_ = dqmStore.book1D("EvtWeightNegUnweighted", "log10(|Event Weight|), Event Weight #leq 0 (unweighted)", 74, -6.2, 1.2);

  hEvtWeightPosWeighted_ = dqmStore.book1D("EvtWeightPosWeighted", "log10(|Event Weight|), Event Weight #geq 0 (weighted)", 74, -6.2, 1.2);
  hEvtWeightNegWeighted_ = dqmStore.book1D("EvtWeightNegWeighted", "log10(|Event Weight|), Event Weight #leq 0 (weighted)", 74, -6.2, 1.2);
}

void fillEvtWeightHistograms(MonitorElement* hEvtWeight_weighted, MonitorElement* hEvtWeight_unweighted, double evtWeight)
{
  double log10EvtWeight = ( evtWeight > 0. ) ? TMath::Log10(evtWeight) : -1.e+3;

  if ( log10EvtWeight >  1. ) log10EvtWeight =  1.15;
  if ( log10EvtWeight < -6. ) log10EvtWeight = -6.15;

  hEvtWeight_weighted->Fill(log10EvtWeight, evtWeight);
  hEvtWeight_unweighted->Fill(log10EvtWeight);
}

void EventWeightHistManager::fillHistograms(const edm::Event& evt, const edm::EventSetup& es, double evtWeight)

{  
  //std::cout << "<EventWeightHistManager::fillHistograms>:" << std::endl; 

  if ( dqmError_ ) {
    edm::LogError ("fillHistograms") << " Failed to access dqmStore --> histograms will NOT be filled !!";
    return;
  }

  if ( evtWeight >= 0. ) fillEvtWeightHistograms(hEvtWeightPosUnweighted_, hEvtWeightPosWeighted_, evtWeight);
  if ( evtWeight <= 0. ) fillEvtWeightHistograms(hEvtWeightNegUnweighted_, hEvtWeightNegWeighted_, -evtWeight);
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, EventWeightHistManager, "EventWeightHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, EventWeightHistManager, "EventWeightHistManager");

#include "TauAnalysis/Core/interface/HistManagerAdapter.h"

typedef HistManagerAdapter<EventWeightHistManager> EventWeightAnalyzer;

DEFINE_ANOTHER_FWK_MODULE(EventWeightAnalyzer);
