#include "TauAnalysis/RecoTools/plugins/VertexByParticleMatchSelector.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Common/interface/View.h"

#include <TMath.h>

VertexByParticleMatchSelectorImp::VertexByParticleMatchSelectorImp(const edm::ParameterSet& cfg )
{ 
  srcParticleCollections_ = cfg.getParameter<vInputTag>("srcParticles");
  dZ_ = cfg.getParameter<double>("dZ");
}

void VertexByParticleMatchSelectorImp::select(const edm::Handle<collection>& vertexCollection,
					      edm::Event& evt, const edm::EventSetup& es) 
{
  selected_.clear();

  for ( collection::const_iterator vertex = vertexCollection->begin();
	vertex != vertexCollection->end(); ++vertex ) {

    bool isMatchedToAllParticleCollections = true;

    for ( vInputTag::const_iterator srcParticleCollection = srcParticleCollections_.begin();
	  srcParticleCollection != srcParticleCollections_.end(); ++srcParticleCollection ) {
      
      edm::Handle<reco::CandidateView> particles;
      evt.getByLabel(*srcParticleCollection, particles);

      bool isMatchedToSingleParticleCollection = false;
      
      for ( reco::CandidateView::const_iterator particle = particles->begin(); 
	    particle != particles->end(); ++particle ) {
	if ( TMath::Abs(particle->vertex().z() - vertex->position().z()) < dZ_ ) isMatchedToSingleParticleCollection = true;
      }

      if ( !isMatchedToSingleParticleCollection ) isMatchedToAllParticleCollections = false;
    }

    if ( isMatchedToAllParticleCollections ) selected_.push_back(&(*vertex));
  }
}

#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"

#include "FWCore/Framework/interface/MakerMacros.h"

typedef ObjectSelector<VertexByParticleMatchSelectorImp> VertexByParticleMatchSelector;

DEFINE_FWK_MODULE(VertexByParticleMatchSelector);


