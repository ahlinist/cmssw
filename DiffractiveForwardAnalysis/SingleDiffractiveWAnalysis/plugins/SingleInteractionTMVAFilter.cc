#ifndef DiffractiveForwardAnalysis_SingleDiffractiveWAnalysis_SingleInteractionTMVAFilter_h
#define DiffractiveForwardAnalysis_SingleDiffractiveWAnalysis_SingleInteractionTMVAFilter_h

#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class TH1F;
namespace TMVA{
  class Reader;
}

class SingleInteractionTMVAFilter : public edm::EDFilter {
    public:
       explicit SingleInteractionTMVAFilter(const edm::ParameterSet&);
       ~SingleInteractionTMVAFilter();

       virtual void beginJob(edm::EventSetup const&);
       virtual bool filter(edm::Event&, const edm::EventSetup&);
   private:
       int getNVertexes(edm::Event&, const edm::EventSetup&);

       edm::InputTag vertexTag_;

       TMVA::Reader* reader_;
  
       edm::FileInPath weights_file_;
       double cutOnClassifier_;
       TH1F* h_classifierOutput_; 

       struct EventData {
          int nVertex_;
          float xiPlus_;
       } eventData_;//FIXME: put in a single place
};

#endif

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "Utilities/General/interface/FileInPath.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "TH1F.h"
#include "TMVA/Reader.h"

using namespace reco;

SingleInteractionTMVAFilter::SingleInteractionTMVAFilter(const edm::ParameterSet& pset){
       vertexTag_ = pset.getParameter<edm::InputTag>("VertexTag");
       weights_file_ = pset.getParameter<edm::FileInPath>("WeightsFile");
       cutOnClassifier_ = pset.getParameter<double>("CutOnClassifier");
}

SingleInteractionTMVAFilter::~SingleInteractionTMVAFilter(){}

void SingleInteractionTMVAFilter::beginJob(edm::EventSetup const& setup){
        edm::Service<TFileService> fs;
        h_classifierOutput_ = fs->make<TH1F>("classifierOutput","TMVA classifier output",100,0.,1.);

        reader_ = new TMVA::Reader("!Color");
        reader_->AddVariable("nVertex", &eventData_.nVertex_);
        reader_->AddVariable("xiPlus", &eventData_.xiPlus_);

        reader_->BookMVA("Likelihood method",weights_file_.fullPath().c_str());
}

bool SingleInteractionTMVAFilter::filter(edm::Event& event, const edm::EventSetup& setup){

        eventData_.nVertex_ = getNVertexes(event,setup);

        edm::Handle<double> xiTowerPlus;
        event.getByLabel("xiTower","xiTowerplus",xiTowerPlus);
        double xiTower_plus = *xiTowerPlus.product();
        eventData_.xiPlus_ = xiTower_plus;
         
        double tmvaOutput = reader_->EvaluateMVA("Likelihood method");
        h_classifierOutput_->Fill(tmvaOutput);
  
	return (tmvaOutput > cutOnClassifier_);
}

int SingleInteractionTMVAFilter::getNVertexes(edm::Event& event, const edm::EventSetup& setup){
        // Access vertex collection
        edm::Handle<edm::View<Vertex> > vertexCollectionH;
        event.getByLabel(vertexTag_,vertexCollectionH);
        const edm::View<Vertex>& vtxColl = *(vertexCollectionH.product());

        // Find number of good vertices
        int nGoodVertices = 0;
        for(edm::View<Vertex>::const_iterator vtx = vtxColl.begin(); vtx != vtxColl.end(); ++vtx){
           if(!vtx->isValid()) continue; // skip non-valid vertices
           if(vtx->isFake()) continue; // skip vertex from beam spot
           ++nGoodVertices;
        }

        return nGoodVertices; 
}

DEFINE_FWK_MODULE(SingleInteractionTMVAFilter);

