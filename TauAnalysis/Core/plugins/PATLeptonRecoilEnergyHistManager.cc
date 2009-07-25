#include "TauAnalysis/Core/plugins/PATLeptonRecoilEnergyHistManager.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "TauAnalysis/DQMTools/interface/generalAuxFunctions.h"

#include "TauAnalysis/Core/interface/histManagerAuxFunctions.h"

#include "AnalysisDataFormats/TauAnalysis/interface/PATLeptonRecoilEnergy.h"
#include "AnalysisDataFormats/TauAnalysis/interface/PATLeptonRecoilEnergyFwd.h"

//
//-----------------------------------------------------------------------------------------------------------------------
//

template<typename T1, typename T2>
PATLeptonRecoilEnergyHistManager<T1,T2>::PATLeptonRecoilEnergyHistManager(const edm::ParameterSet& cfg)
  : dqmError_(0)
{
  //std::cout << "<PATLeptonRecoilEnergyHistManager::PATLeptonRecoilEnergyHistManager>:" << std::endl;

  leptonRecoilEnergySrc_ = cfg.getParameter<edm::InputTag>("leptonRecoilEnergySource");
  //std::cout << " leptonRecoilEnergySrc = " << leptonRecoilEnergySrc_ << std::endl;

  dqmDirectory_store_ = cfg.getParameter<std::string>("dqmDirectory_store");
  //std::cout << " dqmDirectory_store = " << dqmDirectory_store_ << std::endl;
}

template<typename T1, typename T2>
PATLeptonRecoilEnergyHistManager<T1,T2>::~PATLeptonRecoilEnergyHistManager()
{
//--- nothing to be done yet...
}

template<typename T1, typename T2>
void PATLeptonRecoilEnergyHistManager<T1,T2>::bookHistograms()
{
  //std::cout << "<PATLeptonRecoilEnergyHistManager::bookHistograms>:" << std::endl;

  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("bookHistograms") << " Failed to access dqmStore --> histograms will NOT be booked !!";
    dqmError_ = 1;
    return;
  }

  DQMStore& dqmStore = (*edm::Service<DQMStore>());
  
  dqmStore.setCurrentFolder(dqmDirectory_store_);
  
  hEtSum_ = dqmStore.book1D("EtSum", "#Sigma E_{T}^{recoil}", 100, 0., 50.);
  hNumObjects_  = dqmStore.book1D("NumObjects", "Recoil Energy Objects", 50, -0.5, 49.5);
}

template<typename T1, typename T2>
void PATLeptonRecoilEnergyHistManager<T1,T2>::fillHistograms(const edm::Event& evt, const edm::EventSetup& es)
{  
  //std::cout << "<PATLeptonRecoilEnergyHistManager::fillHistograms>:" << std::endl; 

  if ( dqmError_ ) {
    edm::LogError ("fillHistograms") << " Failed to access dqmStore --> histograms will NOT be filled !!";
    return;
  }

  typedef std::vector<PATLeptonRecoilEnergy<T1,T2> > PATLeptonRecoilEnergyCollection;
  edm::Handle<PATLeptonRecoilEnergyCollection> leptonRecoilEnergyCollection;
  evt.getByLabel(leptonRecoilEnergySrc_, leptonRecoilEnergyCollection);
  
  for ( typename PATLeptonRecoilEnergyCollection::const_iterator leptonRecoilEnergy = leptonRecoilEnergyCollection->begin();
	leptonRecoilEnergy != leptonRecoilEnergyCollection->end(); ++leptonRecoilEnergy ) {
    hEtSum_->Fill(leptonRecoilEnergy->etSum());
    hNumObjects_->Fill(leptonRecoilEnergy->recoilEnergyObjects().size());
  }
}

#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/CaloTowers/interface/CaloTower.h"

typedef PATLeptonRecoilEnergyHistManager<pat::Tau, reco::Jet> TauRecoilEnergyFromJetsHistManager;
typedef PATLeptonRecoilEnergyHistManager<pat::Tau, CaloTower> TauRecoilEnergyFromCaloTowersHistManager;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, TauRecoilEnergyFromJetsHistManager, "TauRecoilEnergyFromJetsHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, TauRecoilEnergyFromJetsHistManager, "TauRecoilEnergyFromJetsHistManager");
DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, TauRecoilEnergyFromCaloTowersHistManager, "TauRecoilEnergyFromCaloTowersHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, TauRecoilEnergyFromCaloTowersHistManager, "TauRecoilEnergyFromCaloTowersHistManager");

#include "TauAnalysis/Core/interface/HistManagerAdapter.h"

typedef HistManagerAdapter<TauRecoilEnergyFromJetsHistManager> TauRecoilEnergyFromJetsAnalyzer;
typedef HistManagerAdapter<TauRecoilEnergyFromCaloTowersHistManager> TauRecoilEnergyFromCaloTowersAnalyzer;

DEFINE_ANOTHER_FWK_MODULE(TauRecoilEnergyFromJetsAnalyzer);
DEFINE_ANOTHER_FWK_MODULE(TauRecoilEnergyFromCaloTowersAnalyzer);
