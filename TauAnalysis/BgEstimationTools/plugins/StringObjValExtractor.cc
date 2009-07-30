#include "TauAnalysis/BgEstimationTools/plugins/StringObjValExtractor.h"

#include "DataFormats/Common/interface/View.h"

template<typename T>
StringObjValExtractor<T>::StringObjValExtractor(const edm::ParameterSet& cfg)
  : stringObjFunction_(cfg.getParameter<std::string>("value"))
{
  src_ = cfg.getParameter<edm::InputTag>("src");

  index_ = ( cfg.exists("index") ) ? cfg.getParameter<unsigned>("index") : 0;
}

template<typename T>
StringObjValExtractor<T>::~StringObjValExtractor()
{
//--- nothing to be done yet...
}

template<typename T>
double StringObjValExtractor<T>::operator()(const edm::Event& evt) const
{
  typedef edm::View<T> patCollectionType;
  edm::Handle<patCollectionType> patObjects;
  evt.getByLabel(src_, patObjects);

  if ( patObjects->size() > index_ ) {
    edm::Ptr<T> patObjectPtr = patObjects->ptrAt(index_);

    return stringObjFunction_(*patObjectPtr);
  } else {
    return -1.;
  }
}

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"

typedef StringObjValExtractor<reco::Candidate> CandidateValExtractor;
typedef StringObjValExtractor<pat::Electron> PATElectronValExtractor;
typedef StringObjValExtractor<pat::Muon> PATMuonValExtractor;
typedef StringObjValExtractor<pat::Tau> PATTauValExtractor;
typedef StringObjValExtractor<pat::Jet> PATJetValExtractor;
typedef StringObjValExtractor<pat::MET> PATMetValExtractor;

#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEt.h"

typedef StringObjValExtractor<DiCandidatePair> DiCandidatePairValExtractor;
typedef StringObjValExtractor<PATElecMuPair> PATElecMuPairValExtractor;
typedef StringObjValExtractor<PATElecTauPair> PATElecTauPairValExtractor;
typedef StringObjValExtractor<PATMuTauPair> PATMuTauPairValExtractor;
typedef StringObjValExtractor<PATDiTauPair> PATDiTauPairValExtractor;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(ObjValExtractorPluginFactory, CandidateValExtractor, "CandidateValExtractor");
DEFINE_EDM_PLUGIN(ObjValExtractorPluginFactory, PATElectronValExtractor, "PATElectronValExtractor");
DEFINE_EDM_PLUGIN(ObjValExtractorPluginFactory, PATMuonValExtractor, "PATMuonValExtractor");
DEFINE_EDM_PLUGIN(ObjValExtractorPluginFactory, PATTauValExtractor, "PATTauValExtractor");
DEFINE_EDM_PLUGIN(ObjValExtractorPluginFactory, PATJetValExtractor, "PATJetValExtractor");
DEFINE_EDM_PLUGIN(ObjValExtractorPluginFactory, PATMetValExtractor, "PATMetValExtractor");

DEFINE_EDM_PLUGIN(ObjValExtractorPluginFactory, DiCandidatePairValExtractor, "DiCandidatePairValExtractor");
DEFINE_EDM_PLUGIN(ObjValExtractorPluginFactory, PATElecMuPairValExtractor, "PATElecMuPairValExtractor");
DEFINE_EDM_PLUGIN(ObjValExtractorPluginFactory, PATElecTauPairValExtractor, "PATElecTauPairValExtractor");
DEFINE_EDM_PLUGIN(ObjValExtractorPluginFactory, PATMuTauPairValExtractor, "PATMuTauPairValExtractor");
DEFINE_EDM_PLUGIN(ObjValExtractorPluginFactory, PATDiTauPairValExtractor, "PATDiTauPairValExtractor");
