#include "TauAnalysis/BgEstimationTools/plugins/NumObjExtractor.h"

#include "DataFormats/Common/interface/View.h"

template<typename T>
NumObjExtractor<T>::NumObjExtractor(const edm::ParameterSet& cfg)
{
  src_ = cfg.getParameter<edm::InputTag>("src");
}

template<typename T>
NumObjExtractor<T>::~NumObjExtractor()
{
//--- nothing to be done yet...
}

template<typename T>
double NumObjExtractor<T>::operator()(const edm::Event& evt) const
{
  typedef edm::View<T> patCollectionType;
  edm::Handle<patCollectionType> patObjects;
  evt.getByLabel(src_, patObjects);

  return patObjects->size();
}

#include "DataFormats/Candidate/interface/Candidate.h"

typedef NumObjExtractor<reco::Candidate> NumCandidateExtractor;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(ObjValExtractorPluginFactory, NumCandidateExtractor, "NumCandidateExtractor");
