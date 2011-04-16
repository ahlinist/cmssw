#include "TauAnalysis/CandidateTools/plugins/HtRatioProducer.h"
#include "AnalysisDataFormats/TauAnalysis/interface/HtRatio.h"
#include "AnalysisDataFormats/TauAnalysis/interface/HtRatioFwd.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/Ptr.h"

#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

#include "TauAnalysis/CandidateTools/interface/FetchCollection.h"

HtRatioProducer::HtRatioProducer(const edm::ParameterSet& cfg)
  : algorithm_(cfg)
{
  srcTau_ = cfg.getParameter<edm::InputTag>("srcTau");
  srcJet_ = cfg.getParameter<edm::InputTag>("srcJet");
  
  produces<HtRatioCollection>("");
}

HtRatioProducer::~HtRatioProducer()
{
//--- nothing to be done yet...
}

void HtRatioProducer::produce(edm::Event& evt, const edm::EventSetup& es)
{
  typedef edm::View<pat::Jet> jetCollectionType;
  edm::Handle<jetCollectionType> jets;
  pf::fetchCollection(jets, srcJet_, evt);

  typedef edm::View<pat::Tau> tauCollectionType;
  edm::Handle<tauCollectionType> tauCollection;
  pf::fetchCollection(tauCollection, srcTau_, evt);

  std::auto_ptr<HtRatioCollection> htRatioCollection(new HtRatioCollection());
  
  for ( unsigned iTau = 0, numTaus = tauCollection->size(); iTau < numTaus; ++iTau ){
	edm::Ptr<pat::Tau> tauPtr = tauCollection->ptrAt(iTau);
	HtRatio htRatio = algorithm_.buildHtRatio(tauPtr, *jets);
	htRatioCollection->push_back(htRatio);	
  }
  
  evt.put(htRatioCollection);
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(HtRatioProducer);



