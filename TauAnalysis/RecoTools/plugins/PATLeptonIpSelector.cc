#include "TauAnalysis/RecoTools/plugins/PATLeptonIpSelector.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"

template <typename T, typename TExtr>
PATLeptonIpSelector<T,TExtr>::PATLeptonIpSelector(const edm::ParameterSet& cfg) 
{
  vertexSrc_ = cfg.getParameter<edm::InputTag>("vertexSource");

  if ( cfg.exists("IpMin") ) {
    ipMin_ = cfg.getParameter<double>("IpMin");
    applyIpMin_ = true;
  } else {
    applyIpMin_ = false;
  }

  if ( cfg.exists("IpMax") ) {
    ipMax_ = cfg.getParameter<double>("IpMax");
    applyIpMax_ = true;
  } else {
    applyIpMax_ = false;
  }
}

template <typename T, typename TExtr>
PATLeptonIpSelector<T,TExtr>::~PATLeptonIpSelector() 
{
//--- nothing to be done yet...
}

template <typename T, typename TExtr>
void PATLeptonIpSelector<T,TExtr>::select(const edm::Handle<collection>& patLeptonCollection,
					  edm::Event& evt, const edm::EventSetup& es) 
{
  selected_.clear();
  
  edm::Handle<reco::VertexCollection> primaryEventVertexCollection;
  evt.getByLabel(vertexSrc_, primaryEventVertexCollection);
  if ( primaryEventVertexCollection->size() < 1 ) {
    return;
  } else if ( primaryEventVertexCollection->size() > 1 ) {
    edm::LogError ("PATLeptonIpSelector::select") << " Cannot have more than one primary event vertex --> skipping !!";
    return;
  } 

  const reco::Vertex& thePrimaryEventVertex = (*primaryEventVertexCollection->begin());

  for ( typename collection::const_iterator patLepton = patLeptonCollection->begin(); 
	patLepton != patLeptonCollection->end(); ++patLepton ) {

    bool ipIsValid = false;
    double ip = ipExtractor_(*patLepton, thePrimaryEventVertex, ipIsValid);
    if ( ipIsValid && (!applyIpMin_ || ip > ipMin_) &&
	              (!applyIpMax_ || ip < ipMax_) ) {
      selected_.push_back(&(*patLepton));
    }
  }
}

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

struct PATElectronIpExtractor
{
  double operator()(const pat::Electron& electron, const reco::Vertex& primaryEventVertex, bool& isValid) 
  {
    if ( electron.gsfTrack().isNonnull() ) {
      isValid = true;
      return electron.gsfTrack()->dxy(primaryEventVertex.position());
    } else {
      isValid = false;
      return 0.;
    }
  }
};

struct PATMuonIpExtractor
{
  double operator()(const pat::Muon& muon, const reco::Vertex& primaryEventVertex, bool& isValid) 
  {
    if ( muon.track().isNonnull() ) {
      isValid = true;
      return muon.track()->dxy(primaryEventVertex.position());
    } else {
      isValid = false;
      return 0.;
    }
  }
};

struct PATTauIpExtractor
{
  double operator()(const pat::Tau& tau, const reco::Vertex& primaryEventVertex, bool& isValid) 
  {
    if ( tau.leadTrack().isNonnull() ) {
      isValid = true;
      return tau.leadTrack()->dxy(primaryEventVertex.position());
    } else {
      isValid = false;
      return 0.;
    }
  }
};

#include "PhysicsTools/UtilAlgos/interface/ObjectSelector.h"

typedef ObjectSelector<PATLeptonIpSelector<pat::Electron, PATElectronIpExtractor> > PATElectronIpSelector;
typedef ObjectSelector<PATLeptonIpSelector<pat::Muon, PATMuonIpExtractor> > PATMuonIpSelector;
typedef ObjectSelector<PATLeptonIpSelector<pat::Tau, PATTauIpExtractor> > PATTauIpSelector;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_ANOTHER_FWK_MODULE(PATElectronIpSelector);
DEFINE_ANOTHER_FWK_MODULE(PATMuonIpSelector);
DEFINE_ANOTHER_FWK_MODULE(PATTauIpSelector);
