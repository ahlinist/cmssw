#include "TauAnalysis/Core/plugins/MEtTopologyHistManager.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "TauAnalysis/DQMTools/interface/generalAuxFunctions.h"

#include "TauAnalysis/Core/interface/histManagerAuxFunctions.h"

#include "AnalysisDataFormats/TauAnalysis/interface/MEtTopology.h"
#include "AnalysisDataFormats/TauAnalysis/interface/MEtTopologyFwd.h"

//
//-----------------------------------------------------------------------------------------------------------------------
//

MEtTopologyHistManager::MEtTopologyHistManager(const edm::ParameterSet& cfg)
  : HistManagerBase(cfg)
{
  //std::cout << "<MEtTopologyHistManager::MEtTopologyHistManager>:" << std::endl;

  metTopologySrc_ = cfg.getParameter<edm::InputTag>("metTopologySource");
  //std::cout << " metTopologySrc = " << metTopologySrc_ << std::endl;
}

MEtTopologyHistManager::~MEtTopologyHistManager()
{
//--- nothing to be done yet...
}

void MEtTopologyHistManager::bookHistogramsImp()
{
  //std::cout << "<MEtTopologyHistManager::bookHistogramsImp>:" << std::endl;
  
  hVratio_ = book1D("Vratio", "V_{anti}/V_{parallel}", 102, -0.01, +1.01);
}

void MEtTopologyHistManager::fillHistogramsImp(const edm::Event& evt, const edm::EventSetup& es, double evtWeight)
{  
  //std::cout << "<MEtTopologyHistManager::fillHistogramsImp>:" << std::endl; 

  edm::Handle<MEtTopologyCollection> metTopologyCollection;
  evt.getByLabel(metTopologySrc_, metTopologyCollection);

  for ( MEtTopologyCollection::const_iterator metTopology = metTopologyCollection->begin();
	metTopology != metTopologyCollection->end(); ++metTopology ) {
    if ( metTopology->Vparallel() > 0. ) hVratio_->Fill(metTopology->Vanti()/metTopology->Vparallel(), evtWeight);
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, MEtTopologyHistManager, "MEtTopologyHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, MEtTopologyHistManager, "MEtTopologyHistManager");

#include "TauAnalysis/Core/interface/HistManagerAdapter.h"

typedef HistManagerAdapter<MEtTopologyHistManager> MEtTopologyAnalyzer;

DEFINE_ANOTHER_FWK_MODULE(MEtTopologyAnalyzer);
