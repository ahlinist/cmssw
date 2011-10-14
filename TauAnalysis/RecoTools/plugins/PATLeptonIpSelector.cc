#include "TauAnalysis/RecoTools/plugins/PATLeptonIpSelector.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"

template <typename T>
PATLeptonIpSelector<T>::PATLeptonIpSelector(const edm::ParameterSet& cfg);
    ipMin_(0),
    ipMax_(1000),
    ipZmax_(1000)
{
  vertexSrc_ = cfg.getParameter<edm::InputTag>("vertexSource");

  if ( cfg.exists("IpMin") ) 
    ipMin_ = cfg.getParameter<double>("IpMin");

  if ( cfg.exists("IpMax") ) 
    ipMax_ = cfg.getParameter<double>("IpMax");
  
  if ( cfg.exists("IpzMax") ) 
    ipZmax_ = cfg.getParameter<double>("IpzMax");
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
  } 

  const reco::Vertex& thePrimaryEventVertex = (*primaryEventVertexCollection->begin());

  for ( typename collection::const_iterator patLepton = patLeptonCollection->begin(); 
	patLepton != patLeptonCollection->end(); ++patLepton ) {

    const reco::Track* track = trackExtractor_(*patLepton);
    if ( track ) {
      double ip = fabs(track->dxy(thePrimaryEventVertex.position()));
      double ipz = fabs(track->dz(thePrimaryEventVertex.position()));

      if ( ip > ipMin_ && ip < ipMax_ && ipz < ipZmax_ ) {
	    selected_.push_back(&(*patLepton));
      }
    }
  }
}

#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"

typedef ObjectSelector<PATLeptonIpSelector<pat::Electron> > PATElectronIpSelector;
typedef ObjectSelector<PATLeptonIpSelector<pat::Muon> > PATMuonIpSelector;
typedef ObjectSelector<PATLeptonIpSelector<pat::Tau> > PATTauIpSelector;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(PATElectronIpSelector);
DEFINE_FWK_MODULE(PATMuonIpSelector);
DEFINE_FWK_MODULE(PATTauIpSelector);
