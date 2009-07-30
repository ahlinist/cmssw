#include "TauAnalysis/BgEstimationTools/plugins/PATMuonAntiPionExtractor.h"

#include "DataFormats/Common/interface/View.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"

PATMuonAntiPionExtractor::PATMuonAntiPionExtractor(const edm::ParameterSet& cfg)
{
  coeffCaloComp_ = cfg.getParameter<double>("CaloCompCoefficient");
  coeffSegmComp_ = cfg.getParameter<double>("SegmCompCoefficient");

  src_ = cfg.getParameter<edm::InputTag>("src");

  index_ = ( cfg.exists("index") ) ? cfg.getParameter<unsigned>("index") : 0;
}

PATMuonAntiPionExtractor::~PATMuonAntiPionExtractor()
{
//--- nothing to be done yet...
}

double PATMuonAntiPionExtractor::operator()(const edm::Event& evt) const
{
  typedef edm::View<pat::Muon> patMuonCollectionType;
  edm::Handle<patMuonCollectionType> patMuons;
  evt.getByLabel(src_, patMuons);

  if ( patMuons->size() > index_ ) {
    edm::Ptr<pat::Muon> patMuonPtr = patMuons->ptrAt(index_);

    double discriminant = coeffCaloComp_*muon::caloCompatibility(*patMuonPtr) 
                         + coeffSegmComp_*muon::segmentCompatibility(*patMuonPtr);

    return discriminant;
  } else {
    return -1.;
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(ObjValExtractorPluginFactory, PATMuonAntiPionExtractor, "PATMuonAntiPionExtractor");
