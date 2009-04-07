#include "TauAnalysis/RecoTools/plugins/PATLeptonIpSelector.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"

template <typename T>
PATLeptonIpSelector<T>::PATLeptonIpSelector(const edm::ParameterSet& cfg) 
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

template <typename T>
PATLeptonIpSelector<T>::~PATLeptonIpSelector() 
{
//--- nothing to be done yet...
}

template <typename T>
void PATLeptonIpSelector<T>::select(const edm::Handle<collection>& patLeptonCollection,
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

    const reco::Track* track = trackExtractor_(*patLepton);
    if ( track ) {
      double ip = track->dxy(thePrimaryEventVertex.position());

      if ( (!applyIpMin_ || ip > ipMin_) &&
	   (!applyIpMax_ || ip < ipMax_) ) {
	selected_.push_back(&(*patLepton));
      }
    }
  }
}

#include "PhysicsTools/UtilAlgos/interface/ObjectSelector.h"

typedef ObjectSelector<PATLeptonIpSelector<pat::Electron> > PATElectronIpSelector;
typedef ObjectSelector<PATLeptonIpSelector<pat::Muon> > PATMuonIpSelector;
typedef ObjectSelector<PATLeptonIpSelector<pat::Tau> > PATTauIpSelector;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_ANOTHER_FWK_MODULE(PATElectronIpSelector);
DEFINE_ANOTHER_FWK_MODULE(PATMuonIpSelector);
DEFINE_ANOTHER_FWK_MODULE(PATTauIpSelector);
