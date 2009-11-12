#include "TauAnalysis/BgEstimationTools/plugins/PATDiTauChargeSignExtractor.h"

#include "DataFormats/Common/interface/View.h"

#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEt.h"

const double substituteValue = 1.e+3;

template <typename T1, typename T2>
PATDiTauChargeSignExtractor<T1,T2>::PATDiTauChargeSignExtractor(const edm::ParameterSet& cfg)
{
  src_ = cfg.getParameter<edm::InputTag>("src");

  index_ = ( cfg.exists("index") ) ? cfg.getParameter<unsigned>("index") : 0;
}

template <typename T1, typename T2>
PATDiTauChargeSignExtractor<T1,T2>::~PATDiTauChargeSignExtractor()
{
//--- nothing to be done yet...
}

template <typename T1, typename T2>
double PATDiTauChargeSignExtractor<T1,T2>::operator()(const edm::Event& evt) const
{
  typedef CompositePtrCandidateT1T2MEt<T1,T2> diTauType;
  typedef edm::View<diTauType> diTauCollectionType;
  edm::Handle<diTauCollectionType> diTaus;
  evt.getByLabel(src_, diTaus);

  if ( diTaus->size() > index_ ) {
    edm::Ptr<diTauType> diTauPtr = diTaus->ptrAt(index_);

    double leg1Charge = leg1ChargeExtractor_(*diTauPtr->leg1());
    double leg2Charge = leg2ChargeExtractor_(*diTauPtr->leg2());

    if ( (leg1Charge*leg2Charge) > 0. ) return +1.;
    if ( (leg1Charge*leg2Charge) < 0. ) return -1.;
    return 0.;
  }

  return substituteValue;
}

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

typedef PATDiTauChargeSignExtractor<pat::Electron, pat::Tau> PATElecTauPairChargeSignExtractor;
typedef PATDiTauChargeSignExtractor<pat::Muon, pat::Tau> PATMuTauPairChargeSignExtractor;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(ObjValExtractorPluginFactory, PATElecTauPairChargeSignExtractor, "PATElecTauPairChargeSignExtractor");
DEFINE_EDM_PLUGIN(ObjValExtractorPluginFactory, PATMuTauPairChargeSignExtractor, "PATMuTauPairChargeSignExtractor");
