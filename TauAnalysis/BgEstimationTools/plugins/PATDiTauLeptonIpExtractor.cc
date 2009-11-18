#include "TauAnalysis/BgEstimationTools/plugins/PATDiTauLeptonIpExtractor.h"

#include "DataFormats/Common/interface/View.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEt.h"

const double substituteValue = 1.e+3;

template <typename T1, typename T2>
PATDiTauLeptonIpExtractor<T1,T2>::PATDiTauLeptonIpExtractor(const edm::ParameterSet& cfg)
  : error_(0)
{
  src_ = cfg.getParameter<edm::InputTag>("src");
  vertexSrc_ = cfg.getParameter<edm::InputTag>("vertexSource");

  leg_ = cfg.getParameter<unsigned>("leg");
  if ( leg_ != 1 && leg_ != 2 ) {
    edm::LogError("PATDiTauLeptonIpExtractor") << " Configuration parameter 'leg' = " << leg_ << " invalid !!";
    error_ = 1;
  }

  index_ = ( cfg.exists("index") ) ? cfg.getParameter<unsigned>("index") : 0;
}

template <typename T1, typename T2>
PATDiTauLeptonIpExtractor<T1,T2>::~PATDiTauLeptonIpExtractor()
{
//--- nothing to be done yet...
}

template <typename T1, typename T2>
double PATDiTauLeptonIpExtractor<T1,T2>::operator()(const edm::Event& evt) const
{
  if ( error_ ) return substituteValue;

  typedef CompositePtrCandidateT1T2MEt<T1,T2> diTauType;
  typedef edm::View<diTauType> diTauCollectionType;
  edm::Handle<diTauCollectionType> diTaus;
  evt.getByLabel(src_, diTaus);

  if ( diTaus->size() > index_ ) {
    edm::Ptr<diTauType> diTauPtr = diTaus->ptrAt(index_);

    edm::Handle<reco::VertexCollection> primaryEventVertexCollection;
    evt.getByLabel(vertexSrc_, primaryEventVertexCollection);
    if ( primaryEventVertexCollection->size() < 1 ) {
      return substituteValue;
    } else if ( primaryEventVertexCollection->size() > 1 ) {
      edm::LogError ("PATDiTauLeptonIpExtractor::select") << " Cannot have more than one primary event vertex --> skipping !!";
      return substituteValue;
    } 

    const reco::Vertex& thePrimaryEventVertex = (*primaryEventVertexCollection->begin());

    const reco::Track* track = 0;
    if ( leg_ == 1 ) track = trackExtractorLeg1_(*diTauPtr->leg1());
    else if ( leg_ == 2 ) track = trackExtractorLeg2_(*diTauPtr->leg2());
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

typedef PATDiTauLeptonIpExtractor<pat::Electron, pat::Tau> PATElecTauPairLeptonIpExtractor;
typedef PATDiTauLeptonIpExtractor<pat::Electron, pat::Muon> PATElecMuPairLeptonIpExtractor;
typedef PATDiTauLeptonIpExtractor<pat::Muon, pat::Tau> PATMuTauPairLeptonIpExtractor;
typedef PATDiTauLeptonIpExtractor<pat::Tau, pat::Tau> PATDiTauPairLeptonIpExtractor;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(ObjValExtractorPluginFactory, PATElecTauPairLeptonIpExtractor, "PATElecTauPairLeptonIpExtractor");
DEFINE_EDM_PLUGIN(ObjValExtractorPluginFactory, PATElecMuPairLeptonIpExtractor, "PATElecMuPairLeptonIpExtractor");
DEFINE_EDM_PLUGIN(ObjValExtractorPluginFactory, PATMuTauPairLeptonIpExtractor, "PATMuTauPairLeptonIpExtractor");
DEFINE_EDM_PLUGIN(ObjValExtractorPluginFactory, PATDiTauPairLeptonIpExtractor, "PATDiTauPairLeptonIpExtractor");
