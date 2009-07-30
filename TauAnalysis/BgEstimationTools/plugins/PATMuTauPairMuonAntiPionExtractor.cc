#include "TauAnalysis/BgEstimationTools/plugins/PATMuTauPairMuonAntiPionExtractor.h"

#include "DataFormats/Common/interface/View.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"

#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEt.h"

PATMuTauPairMuonAntiPionExtractor::PATMuTauPairMuonAntiPionExtractor(const edm::ParameterSet& cfg)
{
  coeffCaloComp_ = cfg.getParameter<double>("CaloCompCoefficient");
  coeffSegmComp_ = cfg.getParameter<double>("SegmCompCoefficient");

  src_ = cfg.getParameter<edm::InputTag>("src");

  index_ = ( cfg.exists("index") ) ? cfg.getParameter<unsigned>("index") : 0;
}

PATMuTauPairMuonAntiPionExtractor::~PATMuTauPairMuonAntiPionExtractor()
{
//--- nothing to be done yet...
}

double PATMuTauPairMuonAntiPionExtractor::operator()(const edm::Event& evt) const
{
  typedef edm::View<PATMuTauPair> muTauPairCollectionType;
  edm::Handle<muTauPairCollectionType> muTauPairs;
  evt.getByLabel(src_, muTauPairs);

  if ( muTauPairs->size() > index_ ) {
    edm::Ptr<PATMuTauPair> muTauPairPtr = muTauPairs->ptrAt(index_);

    double discriminant = coeffCaloComp_*muon::caloCompatibility(*muTauPairPtr->leg1()) 
                         + coeffSegmComp_*muon::segmentCompatibility(*muTauPairPtr->leg1());

    return discriminant;
  } else {
    return -1.;
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(ObjValExtractorPluginFactory, PATMuTauPairMuonAntiPionExtractor, "PATMuTauPairMuonAntiPionExtractor");
