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
  : dqmError_(0)
{
  //std::cout << "<MEtTopologyHistManager::MEtTopologyHistManager>:" << std::endl;

  metTopologySrc_ = cfg.getParameter<edm::InputTag>("metTopologySource");
  //std::cout << " metTopologySrc = " << metTopologySrc_ << std::endl;

  dqmDirectory_store_ = cfg.getParameter<std::string>("dqmDirectory_store");
  //std::cout << " dqmDirectory_store = " << dqmDirectory_store_ << std::endl;
}

MEtTopologyHistManager::~MEtTopologyHistManager()
{
//--- nothing to be done yet...
}

void MEtTopologyHistManager::bookHistograms()
{
  //std::cout << "<MEtTopologyHistManager::bookHistograms>:" << std::endl;

  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("bookHistograms") << " Failed to access dqmStore --> histograms will NOT be booked !!";
    dqmError_ = 1;
    return;
  }

  DQMStore& dqmStore = (*edm::Service<DQMStore>());
  
  dqmStore.setCurrentFolder(dqmDirectory_store_);
  
  hVratio_ = dqmStore.book1D("Vratio", "V_{anti}/V_{parallel}", 102, -0.01, +1.01);
}

void MEtTopologyHistManager::fillHistograms(const edm::Event& evt, const edm::EventSetup& es)
{  
  //std::cout << "<MEtTopologyHistManager::fillHistograms>:" << std::endl; 

  if ( dqmError_ ) {
    edm::LogError ("fillHistograms") << " Failed to access dqmStore --> histograms will NOT be filled !!";
    return;
  }

  edm::Handle<MEtTopologyCollection> metTopologyCollection;
  evt.getByLabel(metTopologySrc_, metTopologyCollection);

  for ( MEtTopologyCollection::const_iterator metTopology = metTopologyCollection->begin();
	metTopology != metTopologyCollection->end(); ++metTopology ) {
    if ( metTopology->Vparallel() > 0. ) hVratio_->Fill(metTopology->Vanti()/metTopology->Vparallel());
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, MEtTopologyHistManager, "MEtTopologyHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, MEtTopologyHistManager, "MEtTopologyHistManager");

#include "TauAnalysis/Core/interface/HistManagerAdapter.h"

typedef HistManagerAdapter<MEtTopologyHistManager> MEtTopologyAnalyzer;

DEFINE_ANOTHER_FWK_MODULE(MEtTopologyAnalyzer);
