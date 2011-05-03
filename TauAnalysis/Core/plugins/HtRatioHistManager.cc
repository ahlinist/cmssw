#include "TauAnalysis/Core/plugins/HtRatioHistManager.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "TauAnalysis/CandidateTools/interface/generalAuxFunctions.h"

#include "TauAnalysis/Core/interface/histManagerAuxFunctions.h"

#include "AnalysisDataFormats/TauAnalysis/interface/HtRatio.h"
#include "AnalysisDataFormats/TauAnalysis/interface/HtRatioFwd.h"

//
//-----------------------------------------------------------------------------------------------------------------------
//

HtRatioHistManager::HtRatioHistManager(const edm::ParameterSet& cfg)
  : HistManagerBase(cfg)
{
 
  htRatioSrc_ = cfg.getParameter<edm::InputTag>("htRatioSource");
 }

HtRatioHistManager::~HtRatioHistManager()
{
//--- nothing to be done yet...
}

void HtRatioHistManager::bookHistogramsImp()
{
  
  hRatio_ = book1D("Ratio", "HT-ratio", 102, -0.01, +1.01);
}

void HtRatioHistManager::fillHistogramsImp(const edm::Event& evt, const edm::EventSetup& es, double evtWeight)
{  
  edm::Handle<HtRatioCollection> htRatioCollection;
  evt.getByLabel(htRatioSrc_, htRatioCollection);

  for ( HtRatioCollection::const_iterator iHtRatio = htRatioCollection->begin();
	iHtRatio != htRatioCollection->end(); ++iHtRatio ) {
    if ( iHtRatio->Ratio() > 0. ) hRatio_->Fill(iHtRatio->Ratio(), evtWeight);
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, HtRatioHistManager, "HtRatioHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, HtRatioHistManager, "HtRatioHistManager");

#include "TauAnalysis/Core/interface/HistManagerAdapter.h"

typedef HistManagerAdapter<HtRatioHistManager> HtRatioAnalyzer;

DEFINE_FWK_MODULE(HtRatioAnalyzer);
