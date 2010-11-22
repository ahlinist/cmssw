#include "TauAnalysis/RecoTools/plugins/VertexByTrackPtSumSelector.h"

VertexByTrackPtSumSelectorImp::VertexByTrackPtSumSelectorImp(const edm::ParameterSet& cfg )
{ 
  trackPtSumMin_ = cfg.getParameter<double>("trackPtSumMin");
}

void VertexByTrackPtSumSelectorImp::select(const edm::Handle<collection>& vertexCollection,
					   edm::Event& evt, const edm::EventSetup& es) 
{
  selected_.clear();

  for ( collection::const_iterator vertex = vertexCollection->begin();
	vertex != vertexCollection->end(); ++vertex ) {
    double trackPtSum = 0.;
    for ( reco::Vertex::trackRef_iterator track = vertex->tracks_begin();
	  track != vertex->tracks_end(); ++track ) {
      trackPtSum += (*track)->pt();
    }

    if ( trackPtSum > trackPtSumMin_ ) {
      selected_.push_back(&(*vertex));
    }
  }
}

#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"

#include "FWCore/Framework/interface/MakerMacros.h"

typedef ObjectSelector<VertexByTrackPtSumSelectorImp> VertexByTrackPtSumSelector;

DEFINE_FWK_MODULE(VertexByTrackPtSumSelector);


