#ifndef DiffractiveForwardAnalysis_SingleDiffractiveWAnalysis_SingleInteractionTMVAFilter_h
#define DiffractiveForwardAnalysis_SingleDiffractiveWAnalysis_SingleInteractionTMVAFilter_h

#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <vector>
#include <string>
#include <memory>

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
       int getNTracks(const edm::Event&, const edm::EventSetup&);
  
       std::auto_ptr<TMVA::Reader> reader_;

       edm::InputTag vertexTag_;
       edm::InputTag tracksTag_;

       edm::FileInPath weights_file_;
       double cutOnClassifier_;

       unsigned int thresholdIndexHF_;

       std::vector<std::string> variables_;
 
       TH1F* h_classifierOutput_; 

       struct EventData {
          int nVertex_;
          int nHFTowerPlus_;
          int nHFTowerMinus_;
          float xiPlus_;
          float xiMinus_;
          int nTracks_;
          int nTracksAssociatedToPV_;
          int nTracksAwayFromPV_;
          float forwardBackwardAsymmetryHFEnergy_;
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
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TH1F.h"
#include "TMVA/Reader.h"

using namespace reco;

SingleInteractionTMVAFilter::SingleInteractionTMVAFilter(const edm::ParameterSet& pset):
  reader_(new TMVA::Reader("!Color")),
  vertexTag_(pset.getParameter<edm::InputTag>("VertexTag")),
  tracksTag_(pset.getParameter<edm::InputTag>("TracksTag")),
  weights_file_(pset.getParameter<edm::FileInPath>("WeightsFile")),
  cutOnClassifier_(pset.getParameter<double>("CutOnClassifier")),
  thresholdIndexHF_(pset.getParameter<unsigned int>("ThresholdIndexHF")),
  variables_(pset.getParameter<std::vector<std::string> >("InputVariables"))
  {

  for(std::vector<std::string>::const_iterator it = variables_.begin(); it != variables_.end(); ++it){
     //FIXME
     if(*it == "nVertex")                               reader_->AddVariable("nVertex", &eventData_.nVertex_);
     else if(*it == "nHFTowerPlus")                     reader_->AddVariable("nHFTowerPlus", &eventData_.nHFTowerPlus_);
     else if(*it == "nHFTowerMinus")                    reader_->AddVariable("nHFTowerMinus", &eventData_.nHFTowerMinus_);
     else if(*it == "xiPlus")                           reader_->AddVariable("xiPlus", &eventData_.xiPlus_);
     else if(*it == "xiMinus")                          reader_->AddVariable("xiMinus", &eventData_.xiMinus_);
     else if(*it == "nTracks")                          reader_->AddVariable("nTracks", &eventData_.nTracks_);
     else if(*it == "nTracksAssociatedToPV")            reader_->AddVariable("nTracksAssociatedToPV", &eventData_.nTracksAssociatedToPV_);
     else if(*it == "nTracksAwayFromPV")                reader_->AddVariable("nTracksAwayFromPV", &eventData_.nTracksAwayFromPV_);
     else if(*it == "forwardBackwardAsymmetryHFEnergy") reader_->AddVariable("forwardBackwardAsymmetryHFEnergy", &eventData_.forwardBackwardAsymmetryHFEnergy_);
     else throw edm::Exception(edm::errors::Configuration) << " Variable " << *it << " not available";
  }

  reader_->BookMVA("My MVA method",weights_file_.fullPath().c_str());
}

SingleInteractionTMVAFilter::~SingleInteractionTMVAFilter(){}

void SingleInteractionTMVAFilter::beginJob(edm::EventSetup const& setup){
  edm::Service<TFileService> fs;
  h_classifierOutput_ = fs->make<TH1F>("classifierOutput","TMVA classifier output",200,-2.,2.);
}

bool SingleInteractionTMVAFilter::filter(edm::Event& event, const edm::EventSetup& setup){

  eventData_.nVertex_ = getNVertexes(event,setup);

  edm::Handle<std::vector<unsigned int> > nHFTowerPlus;
  event.getByLabel("hfTower","nHFplus",nHFTowerPlus);
  unsigned int nHF_plus = (*nHFTowerPlus.product())[thresholdIndexHF_];
  eventData_.nHFTowerPlus_ = nHF_plus;

  edm::Handle<std::vector<unsigned int> > nHFTowerMinus;
  event.getByLabel("hfTower","nHFminus",nHFTowerMinus);
  unsigned int nHF_minus = (*nHFTowerMinus.product())[thresholdIndexHF_];
  eventData_.nHFTowerMinus_ = nHF_minus;

  edm::Handle<double> xiTowerPlus;
  event.getByLabel("xiTower","xiTowerplus",xiTowerPlus);
  double xiTower_plus = *xiTowerPlus.product();
  eventData_.xiPlus_ = xiTower_plus;

  edm::Handle<double> xiTowerMinus;
  event.getByLabel("xiTower","xiTowerminus",xiTowerMinus);
  double xiTower_minus = *xiTowerMinus.product();
  eventData_.xiMinus_ = xiTower_minus;
 
  edm::Handle<unsigned int> trackMultiplicity;
  event.getByLabel("trackMultiplicity","trackMultiplicity",trackMultiplicity);

  edm::Handle<unsigned int> trackMultiplicityAssociatedToPV;
  event.getByLabel("trackMultiplicityAssociatedToPV","trackMultiplicity",trackMultiplicityAssociatedToPV);

  edm::Handle<unsigned int> trackMultiplicityAwayFromPV;
  event.getByLabel("trackMultiplicityAwayFromPV","trackMultiplicity",trackMultiplicityAwayFromPV);

  unsigned int nTracks = *trackMultiplicity.product();
  unsigned int nTracksAssociatedToPV = *trackMultiplicityAssociatedToPV.product();
  unsigned int nTracksAwayFromPV = *trackMultiplicityAwayFromPV.product();
        
  eventData_.nTracks_ = nTracks;
  eventData_.nTracksAssociatedToPV_ = nTracksAssociatedToPV; 
  eventData_.nTracksAwayFromPV_ = nTracksAwayFromPV;        

  edm::Handle<std::vector<double> > forwardBackwardAsymmetryHFEnergy;
  event.getByLabel("hfTower","FBAsymmetryFromHFEnergy",forwardBackwardAsymmetryHFEnergy);
  double fbAsymmetryEnergy = (*forwardBackwardAsymmetryHFEnergy.product())[thresholdIndexHF_];
  eventData_.forwardBackwardAsymmetryHFEnergy_ = fbAsymmetryEnergy;

  double tmvaOutput = reader_->EvaluateMVA("My MVA method");
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
