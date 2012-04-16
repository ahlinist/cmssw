#include "TauAnalysis/RecoTools/plugins/PATLeptonDzSelector.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"

template <typename T>
PATLeptonDzSelector<T>::PATLeptonDzSelector(const edm::ParameterSet& cfg) 
  : moduleLabel_(cfg.getParameter<std::string>("@module_label"))
{
  vertexSrc_ = cfg.getParameter<edm::InputTag>("vertexSource");

  dzMax_ = cfg.getParameter<double>("dzMax");
}

template <typename T>
PATLeptonDzSelector<T>::~PATLeptonDzSelector() 
{
//--- nothing to be done yet...
}

template <typename T>
void PATLeptonDzSelector<T>::select(const edm::Handle<collection>& patLeptonCollection,
				    edm::Event& evt, const edm::EventSetup& es) 
{
  //std::cout << "<PATLeptonDzSelector::select>:" << std::endl;
  //std::cout << " moduleLabel = " << moduleLabel_ << std::endl; 

  selected_.clear();
  
  edm::Handle<reco::VertexCollection> primaryEventVertexCollection;
  evt.getByLabel(vertexSrc_, primaryEventVertexCollection);
  if ( primaryEventVertexCollection->size() < 1 ) {
    return;
  } 

  const reco::Vertex& thePrimaryEventVertex = (*primaryEventVertexCollection->begin());

  for ( typename collection::const_iterator patLepton = patLeptonCollection->begin(); 
	patLepton != patLeptonCollection->end(); ++patLepton ) {
    //std::cout << "lepton: Pt = " << patLepton->pt() << ","
    //	        << " eta = " << patLepton->eta() << ", phi = " << patLepton->phi() << std::endl;
    const reco::Track* track = trackExtractor_(*patLepton);
    if ( track ) {
      double dz = track->dz(thePrimaryEventVertex.position());
      //std::cout << " has track, dZ = " << dz << std::endl;
      if ( TMath::Abs(dz) < dzMax_ ) selected_.push_back(&(*patLepton));
    } else {
      //std::cout << " has no track" << std::endl;
    }
  }
}

#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"

typedef ObjectSelector<PATLeptonDzSelector<pat::Electron> > PATElectronDzSelector;
typedef ObjectSelector<PATLeptonDzSelector<pat::Muon> > PATMuonDzSelector;
typedef ObjectSelector<PATLeptonDzSelector<pat::Tau> > PATTauDzSelector;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(PATElectronDzSelector);
DEFINE_FWK_MODULE(PATMuonDzSelector);
DEFINE_FWK_MODULE(PATTauDzSelector);
