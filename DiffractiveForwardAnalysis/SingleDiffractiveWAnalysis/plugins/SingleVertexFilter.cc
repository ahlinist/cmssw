#ifndef DiffractiveForwardAnalysis_SingleDiffractiveWAnalysis_SingleVertexFilter_h
#define DiffractiveForwardAnalysis_SingleDiffractiveWAnalysis_SingleVertexFilter_h

#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class TH1F;

class SingleVertexFilter : public edm::EDFilter {
    public:
       explicit SingleVertexFilter(const edm::ParameterSet&);
       ~SingleVertexFilter();

       virtual void beginJob(const edm::EventSetup&);
       virtual bool filter(edm::Event&, const edm::EventSetup&);
   private:
       edm::InputTag vertexTag_;

       struct {
          TH1F* h_nVertex_;
       } histos_;

};

#endif

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TH1F.h"

using namespace reco;

SingleVertexFilter::SingleVertexFilter(const edm::ParameterSet& pset){
        vertexTag_ = pset.getParameter<edm::InputTag>("VertexTag"); 
}

SingleVertexFilter::~SingleVertexFilter(){}

void SingleVertexFilter::beginJob(const edm::EventSetup& setup){
        edm::Service<TFileService> fs;

        histos_.h_nVertex_ = fs->make<TH1F>("nVertex","Nr. of offline primary vertexes",10,0,10);
}

bool SingleVertexFilter::filter(edm::Event& event, const edm::EventSetup& setup){
        edm::Handle<edm::View<Vertex> > vertexCollectionH;
        event.getByLabel(vertexTag_,vertexCollectionH);
        const edm::View<Vertex>& vtxColl = *(vertexCollectionH.product());

        /*// Access primary vertex
        const Vertex& primaryVertex = vtxColl.front();
        bool goodPrimaryVertex = ((primaryVertex.isValid())&&(!primaryVertex.isFake()));

        // Skip event if not well reconstructed primary vertex
        if(!goodPrimaryVertex){
           edm::LogVerbatim("Analysis") << ">>>>>  Could not find any good primary vertex ..skipping";
           return false;
        }*/

        // Find number of good vertices; match one closest to muon vertex 
        int nGoodVertices = 0;
        for(edm::View<Vertex>::const_iterator vtx = vtxColl.begin(); vtx != vtxColl.end(); ++vtx){
           if(!vtx->isValid()) continue; // skip non-valid vertices
           if(vtx->isFake()) continue; // skip vertex from beam spot
           ++nGoodVertices;
        }
        histos_.h_nVertex_->Fill(nGoodVertices);

        //edm::LogVerbatim("Analysis") << " Number of reconstructed primary vertices in event: " << nGoodVertices;
        LogTrace("Analysis") << " Number of reconstructed primary vertices in event: " << nGoodVertices;
         

        // Select single-vertex events
        if(nGoodVertices != 1) {
           LogTrace("Analysis") << ">>>>>  Non single-vertex event ..skipping";
           return false;
        }

	return true;
}

DEFINE_FWK_MODULE(SingleVertexFilter);

