#ifndef DiffractiveForwardAnalysis_SingleDiffractiveWAnalysis_SingleVertexFilter_h
#define DiffractiveForwardAnalysis_SingleDiffractiveWAnalysis_SingleVertexFilter_h

#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class SingleVertexFilter : public edm::EDFilter {
    public:
       explicit SingleVertexFilter(const edm::ParameterSet&);
       ~SingleVertexFilter();

       virtual bool filter(edm::Event&, const edm::EventSetup&);
   private:
       edm::InputTag vertexTag_;
};

#endif

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

using namespace reco;

SingleVertexFilter::SingleVertexFilter(const edm::ParameterSet& pset){
        vertexTag_ = pset.getParameter<edm::InputTag>("VertexTag"); 
}

SingleVertexFilter::~SingleVertexFilter(){}

bool SingleVertexFilter::filter(edm::Event& event, const edm::EventSetup& setup){
        edm::Handle<edm::View<Vertex> > vertexCollectionH;
        event.getByLabel(vertexTag_,vertexCollectionH);
        const edm::View<Vertex>& vtxColl = *(vertexCollectionH.product());

        // Access primary vertex
        const Vertex& primaryVertex = vtxColl.front();
        bool goodPrimaryVertex = ((primaryVertex.isValid())&&(!primaryVertex.isFake()));

        // Skip event if not well reconstructed primary vertex
        if(!goodPrimaryVertex){
           edm::LogVerbatim("Analysis") << ">>>>>  Could not find any good primary vertex ..skipping";
           return false;
        }

        // Find number of good vertices; match one closest to muon vertex 
        int nGoodVertices = 0;
        for(edm::View<Vertex>::const_iterator vtx = vtxColl.begin(); vtx != vtxColl.end(); ++vtx){
           if(!vtx->isValid()) continue; // skip non-valid vertices
           if(vtx->isFake()) continue; // skip vertex from beam spot
           ++nGoodVertices;
        }

        edm::LogVerbatim("Analysis") << " Number of reconstructed primary vertices in event: " << nGoodVertices;

        // Select single-vertex events
        if(nGoodVertices != 1) {
           edm::LogVerbatim("Analysis") << ">>>>>  Non single-vertex event ..skipping";
           return false;
        }

	return true;
}

DEFINE_FWK_MODULE(SingleVertexFilter);

