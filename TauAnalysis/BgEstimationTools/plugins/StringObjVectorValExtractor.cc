#include "TauAnalysis/BgEstimationTools/plugins/StringObjVectorValExtractor.h"

#include "DataFormats/Common/interface/View.h"

template<typename T>
StringObjVectorValExtractor<T>::StringObjVectorValExtractor(const edm::ParameterSet& cfg)
  : stringObjFunction_(cfg.getParameter<std::string>("value"))
{
  src_ = cfg.getParameter<edm::InputTag>("src");
  
  value_ = cfg.getParameter<std::string>("value");
}

template<typename T>
StringObjVectorValExtractor<T>::~StringObjVectorValExtractor()
{
//--- nothing to be done yet...
}

template<typename T>
std::vector<double> StringObjVectorValExtractor<T>::operator()(const edm::Event& evt) const
{
  //std::cout << "<StringObjVectorValExtractor::operator()>:" << std::endl;
  //std::cout << " src = " << src_.label() << std::endl;
  //std::cout << " value = " << value_ << std::endl;

  std::vector<double> vec;
  typedef edm::View<T> patCollectionType;
  edm::Handle<patCollectionType> patObjects;
  evt.getByLabel(src_, patObjects);

  unsigned numPatObjects = patObjects->size();
  for ( unsigned i = 0; i < numPatObjects; ++i ) {
    edm::Ptr<T> patObjectPtr = patObjects->ptrAt(i);
    vec.push_back(stringObjFunction_(*patObjectPtr));
  }

  return vec;
}

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"

typedef StringObjVectorValExtractor<reco::Candidate> CandidateVectorValExtractor;
typedef StringObjVectorValExtractor<pat::Electron>   PATElectronVectorValExtractor;
typedef StringObjVectorValExtractor<pat::Muon>       PATMuonVectorValExtractor;
typedef StringObjVectorValExtractor<pat::Tau>        PATTauVectorValExtractor;
typedef StringObjVectorValExtractor<pat::Jet>        PATJetVectorValExtractor;
typedef StringObjVectorValExtractor<pat::MET>        PATMetVectorValExtractor;

#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEt.h"

typedef StringObjVectorValExtractor<DiCandidatePair> DiCandidatePairVectorValExtractor;
typedef StringObjVectorValExtractor<PATElecMuPair>   PATElecMuPairVectorValExtractor;
typedef StringObjVectorValExtractor<PATElecTauPair>  PATElecTauPairVectorValExtractor;
typedef StringObjVectorValExtractor<PATMuTauPair>    PATMuTauPairVectorValExtractor;
typedef StringObjVectorValExtractor<PATDiTauPair>    PATDiTauPairVectorValExtractor;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(ObjValVectorExtractorPluginFactory, CandidateVectorValExtractor,       "CandidateVectorValExtractor");
DEFINE_EDM_PLUGIN(ObjValVectorExtractorPluginFactory, PATElectronVectorValExtractor,     "PATElectronVectorValExtractor");
DEFINE_EDM_PLUGIN(ObjValVectorExtractorPluginFactory, PATMuonVectorValExtractor,         "PATMuonVectorValExtractor");
DEFINE_EDM_PLUGIN(ObjValVectorExtractorPluginFactory, PATTauVectorValExtractor,          "PATTauVectorValExtractor");
DEFINE_EDM_PLUGIN(ObjValVectorExtractorPluginFactory, PATJetVectorValExtractor,          "PATJetVectorValExtractor");
DEFINE_EDM_PLUGIN(ObjValVectorExtractorPluginFactory, PATMetVectorValExtractor,          "PATMetVectorValExtractor");

DEFINE_EDM_PLUGIN(ObjValVectorExtractorPluginFactory, DiCandidatePairVectorValExtractor, "DiCandidatePairVectorValExtractor");
DEFINE_EDM_PLUGIN(ObjValVectorExtractorPluginFactory, PATElecMuPairVectorValExtractor,   "PATElecMuPairVectorValExtractor");
DEFINE_EDM_PLUGIN(ObjValVectorExtractorPluginFactory, PATElecTauPairVectorValExtractor,  "PATElecTauPairVectorValExtractor");
DEFINE_EDM_PLUGIN(ObjValVectorExtractorPluginFactory, PATMuTauPairVectorValExtractor,    "PATMuTauPairVectorValExtractor");
DEFINE_EDM_PLUGIN(ObjValVectorExtractorPluginFactory, PATDiTauPairVectorValExtractor,    "PATDiTauPairVectorValExtractor");
