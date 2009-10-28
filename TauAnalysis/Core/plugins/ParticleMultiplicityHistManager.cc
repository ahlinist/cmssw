#include "TauAnalysis/Core/plugins/ParticleMultiplicityHistManager.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"

#include "TauAnalysis/Core/interface/histManagerAuxFunctions.h"

ParticleMultiplicityHistManager::histogramEntryType::histogramEntryType(const edm::ParameterSet& cfg)
  : me_(0)
{
  name_ = cfg.getParameter<std::string>("name");
  title_ = cfg.getParameter<std::string>("title");
  numBinsX_ = cfg.getParameter<unsigned>("numBinsX");
  xMin_ = cfg.getParameter<double>("xMin");
  xMax_ = cfg.getParameter<double>("xMax");
  src_ = cfg.getParameter<edm::InputTag>("src");
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

ParticleMultiplicityHistManager::ParticleMultiplicityHistManager(const edm::ParameterSet& cfg)
  : HistManagerBase(cfg)
{
  //std::cout << "<ParticleMultiplicityHistManager::ParticleMultiplicityHistManager>:" << std::endl;

  typedef std::vector<edm::ParameterSet> vParameterSet;
  vParameterSet cfgHistogramEntries = cfg.getParameter<vParameterSet>("histograms");
  for ( vParameterSet::const_iterator cfgHistogramEntry = cfgHistogramEntries.begin(); 
	cfgHistogramEntry != cfgHistogramEntries.end(); ++cfgHistogramEntry ) {
    histogramEntries_.push_back(histogramEntryType(*cfgHistogramEntry));
  }
}

ParticleMultiplicityHistManager::~ParticleMultiplicityHistManager()
{
//--- nothing to be done yet...
}

void ParticleMultiplicityHistManager::bookHistogramsImp()
{
  //std::cout << "<ParticleMultiplicityHistManager::bookHistogramsImp>:" << std::endl;
  
  for ( std::vector<histogramEntryType>::iterator histogramEntry = histogramEntries_.begin();
	histogramEntry != histogramEntries_.end(); ++histogramEntry ) {
    histogramEntry->me_ = book1D(histogramEntry->name_, histogramEntry->title_, 
				 histogramEntry->numBinsX_, histogramEntry->xMin_, histogramEntry->xMax_);
  }
}

void ParticleMultiplicityHistManager::fillHistogramsImp(const edm::Event& evt, const edm::EventSetup& es, double evtWeight)

{  
  //std::cout << "<ParticleMultiplicityHistManager::fillHistogramsImp>:" << std::endl; 

  for ( std::vector<histogramEntryType>::iterator histogramEntry = histogramEntries_.begin();
	histogramEntry != histogramEntries_.end(); ++histogramEntry ) {
    edm::Handle<reco::CandidateView> particles;
    evt.getByLabel(histogramEntry->src_, particles);
    unsigned numParticles = particles->size();
    histogramEntry->me_->Fill(numParticles, evtWeight);
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, ParticleMultiplicityHistManager, "ParticleMultiplicityHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, ParticleMultiplicityHistManager, "ParticleMultiplicityHistManager");

#include "TauAnalysis/Core/interface/HistManagerAdapter.h"

typedef HistManagerAdapter<ParticleMultiplicityHistManager> ParticleMultiplicityAnalyzer;

DEFINE_ANOTHER_FWK_MODULE(ParticleMultiplicityAnalyzer);
