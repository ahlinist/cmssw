#include "TauAnalysis/Core/plugins/PATLeptonRecoilEnergyHistManager.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "TauAnalysis/DQMTools/interface/generalAuxFunctions.h"

#include "TauAnalysis/Core/interface/histManagerAuxFunctions.h"

//
//-----------------------------------------------------------------------------------------------------------------------
//

template<typename T1, typename T2>
PATLeptonRecoilEnergyHistManager<T1,T2>::PATLeptonRecoilEnergyHistManager(const edm::ParameterSet& cfg)
  : HistManagerBase(cfg)
{
  //std::cout << "<PATLeptonRecoilEnergyHistManager::PATLeptonRecoilEnergyHistManager>:" << std::endl;

  leptonRecoilEnergySrc_ = cfg.getParameter<edm::InputTag>("leptonRecoilEnergySource");
  //std::cout << " leptonRecoilEnergySrc = " << leptonRecoilEnergySrc_ << std::endl;

  leptonWeightExtractors_ = getTauJetWeightExtractors<T1>(cfg, "leptonWeightSource");

  std::string normalization_string = cfg.getParameter<std::string>("normalization");
  normMethod_ = getNormMethod(normalization_string, "leptons");
}

template<typename T1, typename T2>
PATLeptonRecoilEnergyHistManager<T1,T2>::~PATLeptonRecoilEnergyHistManager()
{
  for ( typename std::vector<FakeRateJetWeightExtractor<T1>*>::iterator it = leptonWeightExtractors_.begin();
	it != leptonWeightExtractors_.end(); ++it ) {
    delete (*it);
  }
}

template<typename T1, typename T2>
void PATLeptonRecoilEnergyHistManager<T1,T2>::bookHistogramsImp()
{
  //std::cout << "<PATLeptonRecoilEnergyHistManager::bookHistogramsImp>:" << std::endl;
  
  bookWeightHistograms(*dqmStore_, "LeptonWeight", "Lepton Weight", 
		       hLeptonWeightPosUnweighted_, hLeptonWeightPosWeighted_, 
		       hLeptonWeightNegUnweighted_, hLeptonWeightNegWeighted_);

  hEtSum_ = book1D("EtSum", "#Sigma E_{T}^{recoil}", 100, 0., 50.);
  hNumObjects_  = book1D("NumObjects", "Recoil Energy Objects", 50, -0.5, 49.5);
}

template<typename T1, typename T2>
double PATLeptonRecoilEnergyHistManager<T1,T2>::getLeptonWeight(const PATLeptonRecoilEnergy<T1,T2>& leptonRecoilEnergy)
{
  return getTauJetWeight<T1>(*leptonRecoilEnergy.lepton(), leptonWeightExtractors_);
}

template<typename T1, typename T2>
void PATLeptonRecoilEnergyHistManager<T1,T2>::fillHistogramsImp(const edm::Event& evt, const edm::EventSetup& es, double evtWeight)
{  
  //std::cout << "<PATLeptonRecoilEnergyHistManager::fillHistogramsImp>:" << std::endl; 

  typedef std::vector<PATLeptonRecoilEnergy<T1,T2> > PATLeptonRecoilEnergyCollection;
  edm::Handle<PATLeptonRecoilEnergyCollection> leptonRecoilEnergyCollection;
  evt.getByLabel(leptonRecoilEnergySrc_, leptonRecoilEnergyCollection);
  
  double leptonWeightSum = 0.;
  for ( typename PATLeptonRecoilEnergyCollection::const_iterator leptonRecoilEnergy = leptonRecoilEnergyCollection->begin();
	leptonRecoilEnergy != leptonRecoilEnergyCollection->end(); ++leptonRecoilEnergy ) {
    leptonWeightSum += getLeptonWeight(*leptonRecoilEnergy);
  }

  for ( typename PATLeptonRecoilEnergyCollection::const_iterator leptonRecoilEnergy = leptonRecoilEnergyCollection->begin();
	leptonRecoilEnergy != leptonRecoilEnergyCollection->end(); ++leptonRecoilEnergy ) {

    double leptonWeight = getLeptonWeight(*leptonRecoilEnergy);
    double weight = getWeight(evtWeight, leptonWeight, leptonWeightSum);

    fillWeightHistograms(hLeptonWeightPosUnweighted_, hLeptonWeightPosWeighted_, 
			 hLeptonWeightNegUnweighted_, hLeptonWeightNegWeighted_, leptonWeight);

    hEtSum_->Fill(leptonRecoilEnergy->etSum(), weight);
    hNumObjects_->Fill(leptonRecoilEnergy->recoilEnergyObjects().size(), weight);
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
