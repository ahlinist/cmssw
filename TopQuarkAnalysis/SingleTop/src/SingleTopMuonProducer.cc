/*
 *\Author: A. Orso M. Iorio 
 *
 *
 *\version  $Id: SingleTopMuonProducer.cc,v 1.5 2010/11/08 08:26:42 oiorio Exp $ 
 */

// Single Top producer: produces a top candidate made out of a Lepton, a B jet and a MET

#include "PhysicsTools/PatAlgos/plugins/PATJetProducer.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"

#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/Common/interface/Association.h"
#include "DataFormats/Candidate/interface/CandAssociation.h"

#include "DataFormats/JetReco/interface/JetTracksAssociation.h"
#include "DataFormats/BTauReco/interface/JetTag.h"
#include "DataFormats/BTauReco/interface/TrackProbabilityTagInfo.h"
#include "DataFormats/BTauReco/interface/TrackIPTagInfo.h"
#include "DataFormats/BTauReco/interface/TrackCountingTagInfo.h"
#include "DataFormats/BTauReco/interface/SecondaryVertexTagInfo.h"
#include "DataFormats/BTauReco/interface/SoftLeptonTagInfo.h"

#include "DataFormats/Candidate/interface/CandMatchMap.h"
#include "SimDataFormats/JetMatching/interface/JetFlavourMatching.h"

#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "DataFormats/Math/interface/deltaR.h"

#include "DataFormats/PatCandidates/interface/JetCorrFactors.h"

#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"


#include "FWCore/Framework/interface/Selector.h"

#include "TopQuarkAnalysis/SingleTop/interface/SingleTopMuonProducer.h"


#include "DataFormats/Scalers/interface/DcsStatus.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "RecoEgamma/EgammaTools/interface/ConversionFinder.h"
#include "PhysicsTools/SelectorUtils/interface/SimpleCutBasedElectronIDSelectionFunctor.h"


#include <vector>
#include <memory>

#include "DataFormats/Math/interface/LorentzVector.h"


//using namespace pat;


SingleTopMuonProducer::SingleTopMuonProducer(const edm::ParameterSet& iConfig)
{
  src_                 = iConfig.getParameter<edm::InputTag>( "src" );
  jetsSrc_                 = iConfig.getParameter<edm::InputTag>( "jetsSrc" );
  useJetVeto_ = iConfig.getUntrackedParameter< bool >("useJetVeto",true);

  
  cut_ = iConfig.getParameter< std::string >("cut"); 
  
  produces<std::vector<pat::Muon> >();
}

void SingleTopMuonProducer::produce(edm::Event & iEvent, const edm::EventSetup & iEventSetup){

  ////std::cout << " mark 1 " << std::endl;
  edm::Handle<edm::View<pat::Muon> > muons;
  iEvent.getByLabel(src_,muons);

  edm::Handle<edm::View<pat::Jet > > jets;
  iEvent.getByLabel(jetsSrc_,jets);

  edm::Handle<edm::View<reco::Vertex> > vertices;
  iEvent.getByLabel("offlinePrimaryVertices",vertices);

  Selector cut(cut_);
  std::vector< pat::Muon > * finalMuons = new std::vector<pat::Muon>;

  bool isIsolated = true;

  for(size_t i = 0; i < muons->size(); ++i){
    
    if(!cut(muons->at(i)))continue; 
    
    //std::cout << " passes cut " << cut_ <<  std::endl;
    
    if(useJetVeto_){
      for(size_t j = 0; j < jets->size(); ++j){
	if(deltaR(muons->at(i),jets->at(j))<0.3) {
	  isIsolated = false;
	  break;
	}
      }
    }

    //    std::cout << "n vertices = "<<vertices->size() << std::endl; 
    for(size_t v = 0; v < vertices->size(); ++v){
      if(muons->at(i).vertex().z()-vertices->at(0).z()>=1) isIsolated = false;
    }
    
    if(!isIsolated)continue;
    
    finalMuons->push_back(muons->at(i));
    
  } 
 
  ////std::cout << " mark 7 " << std::endl;

std::auto_ptr< std::vector< pat::Muon > > finalMuonsPtr(finalMuons);
 

iEvent.put(finalMuonsPtr);

}

SingleTopMuonProducer::~SingleTopMuonProducer(){;}
DEFINE_FWK_MODULE(SingleTopMuonProducer);
