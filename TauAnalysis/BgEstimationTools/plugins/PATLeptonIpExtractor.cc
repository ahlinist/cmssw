#include "TauAnalysis/BgEstimationTools/plugins/PATLeptonIpExtractor.h"

#include "DataFormats/Common/interface/View.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

const double substituteValue = 1.e+3;

template <typename T>
PATLeptonIpExtractor<T>::PATLeptonIpExtractor(const edm::ParameterSet& cfg)
{
  src_ = cfg.getParameter<edm::InputTag>("src");
  vertexSrc_ = cfg.getParameter<edm::InputTag>("vertexSource");

  index_ = ( cfg.exists("index") ) ? cfg.getParameter<unsigned>("index") : 0;
}

template <typename T>
PATLeptonIpExtractor<T>::~PATLeptonIpExtractor()
{
//--- nothing to be done yet...
}

template <typename T>
double PATLeptonIpExtractor<T>::operator()(const edm::Event& evt) const
{
  typedef edm::View<T> patLeptonCollectionType;
  edm::Handle<patLeptonCollectionType> patLeptons;
  evt.getByLabel(src_, patLeptons);

  if ( patLeptons->size() > index_ ) {
    edm::Ptr<T> patLeptonPtr = patLeptons->ptrAt(index_);

    edm::Handle<reco::VertexCollection> primaryEventVertexCollection;
    evt.getByLabel(vertexSrc_, primaryEventVertexCollection);
    if ( primaryEventVertexCollection->size() < 1 ) {
      return substituteValue;
    } else if ( primaryEventVertexCollection->size() > 1 ) {
      edm::LogError ("PATLeptonIpExtractor::select") << " Cannot have more than one primary event vertex --> skipping !!";
      return substituteValue;
    } 

    const reco::Vertex& thePrimaryEventVertex = (*primaryEventVertexCollection->begin());

    const reco::Track* track = trackExtractor_(*patLeptonPtr);
    if ( track ) {
      double ip = track->dxy(thePrimaryEventVertex.position());

      return ip;
    } 
  }

  return substituteValue;
}

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

typedef PATLeptonIpExtractor<pat::Electron> PATElectronIpExtractor;
typedef PATLeptonIpExtractor<pat::Muon> PATMuonIpExtractor;
typedef PATLeptonIpExtractor<pat::Tau> PATTauIpExtractor;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(ObjValExtractorPluginFactory, PATElectronIpExtractor, "PATElectronIpExtractor");
DEFINE_EDM_PLUGIN(ObjValExtractorPluginFactory, PATMuonIpExtractor, "PATMuonIpExtractor");
DEFINE_EDM_PLUGIN(ObjValExtractorPluginFactory, PATTauIpExtractor, "PATTauIpExtractor");
