#include "TauAnalysis/Core/plugins/BinGridHistManager.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"

#include <iostream>
#include <iomanip>

BinGridHistManager::BinGridHistManager(const edm::ParameterSet& cfg)
  : HistManagerBase(cfg)
{
  //std::cout << "<BinGridHistManager::BinGridHistManager>:" << std::endl;

  edm::ParameterSet cfgBinning = cfg.getParameter<edm::ParameterSet>("binning");
  objValExtractor_ = ObjValVectorExtractorPluginFactory::get()->create("MultiObjValExtractor", cfgBinning);
  binGrid_ = new BinGrid(cfgBinning); 

  cfgHistManagers_ = cfg.getParameter<vParameterSet>("histManagers");
}

BinGridHistManager::~BinGridHistManager()
{
  delete objValExtractor_;
  delete binGrid_;

  for ( std::map<unsigned, vHistManager>::iterator histManagerList = histManagers_.begin();
	histManagerList != histManagers_.end(); ++histManagerList ) {
    for ( vHistManager::iterator histManager = histManagerList->second.begin();
	  histManager != histManagerList->second.end(); ++histManager ) {
      delete (*histManager);
    }
  }
}

void BinGridHistManager::bookHistogramsImp()
{
  //std::cout << "<BinGridHistManager::bookHistogramsImp>:" << std::endl;

  unsigned numRegions = binGrid_->numBins();
  for ( unsigned iRegion = 0; iRegion < numRegions; ++iRegion ) {
    //std::cout << "iRegion = " << iRegion << std::endl;

    std::ostringstream regionName;
    regionName << "region" << std::setw(2) << std::setfill('0') << (iRegion + 1);
    //std::cout << " regionName = " << regionName.str() << std::endl;
    
    std::string dqmDirectory_region = dqmDirectoryName(dqmDirectory_store_).append(regionName.str());
    //std::cout << " dqmDirectory_region = " << dqmDirectory_region << std::endl;

    for ( vParameterSet::const_iterator cfgHistManager = cfgHistManagers_.begin();
	  cfgHistManager != cfgHistManagers_.end(); ++cfgHistManager ) {
      std::string histManagerType = cfgHistManager->getParameter<std::string>("pluginType");
      //std::cout << " histManagerType = " << histManagerType << std::endl;

      std::string dqmDirectory_histmanager = cfgHistManager->getParameter<std::string>("dqmDirectory_store");
      std::string dqmDirectory_store = dqmDirectoryName(dqmDirectory_region).append(dqmDirectory_histmanager);
      //std::cout << " dqmDirectory_store = " << dqmDirectory_store << std::endl;

      edm::ParameterSet cfgHistManager_region(*cfgHistManager);
      cfgHistManager_region.addParameter<std::string>("dqmDirectory_store", dqmDirectory_store);

      HistManagerBase* histManager = HistManagerPluginFactory::get()->create(histManagerType, cfgHistManager_region);
      histManager->beginJob();
      histManagers_[iRegion].push_back(histManager);
    }
  }
}

void BinGridHistManager::fillHistogramsImp(const edm::Event& evt, const edm::EventSetup& es, double evtWeight)

{  
  //std::cout << "<BinGridHistManager::fillHistogramsImp>:" << std::endl; 

  std::vector<double> x = (*objValExtractor_)(evt);
  unsigned iRegion = binGrid_->binNumber(x);

//--- skip processing event if region outside bin-grid
  if ( iRegion < 0 ) return;

  //std::cout << "iRegion = " << iRegion << std::endl;

//--- fill collection of histograms corresponding to region;
//    print error message if no histogram managers defined
//    for that region in bin-grid
  std::map<unsigned, vHistManager>::iterator histManagerList = histManagers_.find(iRegion);
  if ( histManagerList == histManagers_.end() ) {
    edm::LogError ("BinGridHistManager::fillHistogramsImp") << " No histogram Managers defined for region = " << iRegion 
							    << " --> skipping !!";
    return;
  }

  for ( vHistManager::iterator histManager = histManagerList->second.begin();
	histManager != histManagerList->second.end(); ++histManager ) {
    (*histManager)->analyze(evt, es, evtWeight);
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, BinGridHistManager, "BinGridHistManager");

#include "TauAnalysis/Core/interface/HistManagerAdapter.h"

typedef HistManagerAdapter<BinGridHistManager> BinGridAnalyzer;

DEFINE_FWK_MODULE(BinGridAnalyzer);
