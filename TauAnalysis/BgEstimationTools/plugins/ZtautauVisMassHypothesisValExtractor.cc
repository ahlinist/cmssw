#include "TauAnalysis/BgEstimationTools/plugins/ZtautauVisMassHypothesisValExtractor.h"

#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/RefProd.h"
#include "DataFormats/Common/interface/AssociationVector.h"

#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEt.h"

const double substituteValue = -1.;

template <typename T1, typename T2>
ZtautauVisMassHypothesisValExtractor<T1,T2>::ZtautauVisMassHypothesisValExtractor(const edm::ParameterSet& cfg)
{
  src_ = cfg.getParameter<edm::InputTag>("src");
  srcDiTauCandidates_ = cfg.getParameter<edm::InputTag>("srcDiTauCandidates");

  index_ = ( cfg.exists("index") ) ? cfg.getParameter<unsigned>("index") : 0;
}

template <typename T1, typename T2>
ZtautauVisMassHypothesisValExtractor<T1,T2>::~ZtautauVisMassHypothesisValExtractor()
{
//--- nothing to be done yet...
}

template <typename T1, typename T2>
double ZtautauVisMassHypothesisValExtractor<T1,T2>::operator()(const edm::Event& evt) const
{
  typedef std::vector<CompositePtrCandidateT1T2MEt<T1,T2> > CompositePtrCandidateCollection;
  edm::Handle<CompositePtrCandidateCollection> diTauCandidates;
  evt.getByLabel(srcDiTauCandidates_, diTauCandidates);

  if ( diTauCandidates->size() > index_ ) {

    edm::Ref<CompositePtrCandidateCollection> diTauCandidateRef(diTauCandidates, index_);

    typedef edm::RefProd<CompositePtrCandidateCollection> CompositePtrCandidateRefProd;
    typedef std::vector<float> vfloat;
    typedef edm::AssociationVector<CompositePtrCandidateRefProd, vfloat> ZtautauVisMassHypothesisCollection;
    edm::Handle<ZtautauVisMassHypothesisCollection> visMassHypotheses;
    evt.getByLabel(src_, visMassHypotheses);

    return (*visMassHypotheses)[diTauCandidateRef];
  }

  return substituteValue;
}

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

typedef ZtautauVisMassHypothesisValExtractor<pat::Electron, pat::Tau> ZtautauVisMassHypothesisElecTauValExtractor;
typedef ZtautauVisMassHypothesisValExtractor<pat::Muon, pat::Tau> ZtautauVisMassHypothesisMuTauValExtractor;
typedef ZtautauVisMassHypothesisValExtractor<pat::Tau, pat::Tau> ZtautauVisMassHypothesisDiTauValExtractor;
typedef ZtautauVisMassHypothesisValExtractor<pat::Electron, pat::Muon> ZtautauVisMassHypothesisElecMuValExtractor;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(ObjValExtractorPluginFactory, ZtautauVisMassHypothesisElecTauValExtractor, "ZtautauVisMassHypothesisElecTauValExtractor");
DEFINE_EDM_PLUGIN(ObjValExtractorPluginFactory, ZtautauVisMassHypothesisMuTauValExtractor, "ZtautauVisMassHypothesisMuTauValExtractor");
DEFINE_EDM_PLUGIN(ObjValExtractorPluginFactory, ZtautauVisMassHypothesisDiTauValExtractor, "ZtautauVisMassHypothesisDiTauValExtractor");
DEFINE_EDM_PLUGIN(ObjValExtractorPluginFactory, ZtautauVisMassHypothesisElecMuValExtractor, "ZtautauVisMassHypothesisElecMuValExtractor");
