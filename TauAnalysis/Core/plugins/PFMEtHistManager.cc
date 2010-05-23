#include "TauAnalysis/Core/plugins/PFMEtHistManager.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/Common/interface/View.h"

#include "TauAnalysis/Core/interface/histManagerAuxFunctions.h"

#include <TMath.h>

PFMEtHistManager::PFMEtHistManager(const edm::ParameterSet& cfg)
  : MEtHistManager(cfg)
{
  //std::cout << "<PFMEtHistManager::PFMEtHistManager>:" << std::endl;
}

PFMEtHistManager::~PFMEtHistManager()
{
//--- nothing to be done yet...
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, PFMEtHistManager, "PFMEtHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, PFMEtHistManager, "PFMEtHistManager");

#include "TauAnalysis/Core/interface/HistManagerAdapter.h"

typedef HistManagerAdapter<PFMEtHistManager> PFMEtAnalyzer;

DEFINE_FWK_MODULE(PFMEtAnalyzer);

