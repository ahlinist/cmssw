/*
 *\Author: A. Orso M. Iorio 
 *
 *
 *\version  $Id: TopProducer.cc $ 
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




#include "TopQuarkAnalysis/SingleTop/interface/TopProducer.h"

#include <vector>
#include <memory>



using namespace pat;


TopProducer::TopProducer(const edm::ParameterSet& iConfig) 
{
  // initialize the configurables
  electronsSrc_		          = iConfig.getParameter<edm::InputTag>	      ( "electronsSource" );
  muonsSrc_                 = iConfig.getParameter<edm::InputTag>	      ( "muonsSource" );
  jetsSrc_                 = iConfig.getParameter<edm::InputTag>	      ( "jetsSource" );
  METsSrc_                 = iConfig.getParameter<edm::InputTag>	      ( "METsSource" );

produces<std::vector<pat::Electron> >();
produces<std::vector<pat::Muon> >();
produces<std::vector<pat::Jet> >();
produces<std::vector<pat::MET> >();

//produces<std::vector< pat::TopLeptonic > >();
produces<std::vector< reco::NamedCompositeCandidate > >();
 
}

void TopProducer::produce(edm::Event & iEvent, const edm::EventSetup & iEventSetup){

edm::Handle<edm::View<pat::Electron> > electrons;
iEvent.getByLabel(electronsSrc_,electrons);


edm::Handle<edm::View<pat::Muon> > muons;
iEvent.getByLabel(muonsSrc_,muons);


edm::Handle<edm::View<pat::Jet> > jets;
iEvent.getByLabel(jetsSrc_,jets);


edm::Handle<edm::View<pat::MET> > mets;
iEvent.getByLabel(METsSrc_,mets);


////TEST FOR DUMMIES


 std::vector< reco::NamedCompositeCandidate > * TopCandidates = new std::vector<reco::NamedCompositeCandidate>();
// std::auto_ptr<pat::TopLeptonic> newTopLeptonic(new pat::TopLeptonic);

 
 if(electrons->size()>0 && jets->size()>0 && mets->size() > 0){

  const pat::Electron e0 = electrons->at(0);
  // const pat::Muon mu0 = Muons->at(0);
  const pat::MET met0 = mets->at(0);
  const pat::Jet j0 = jets->at(0);


  reco::NamedCompositeCandidate TopDummy;

  TopDummy.addDaughter(e0,"Lepton");
  TopDummy.addDaughter(met0,"MET");
  TopDummy.addDaughter(j0,"BJet");

  //  TopCandidates->push_back(TopDummy);
}

 //  int n = 0;

 for(size_t i = 0; i < electrons->size(); ++i){
   for(size_t j = 0; j < jets->size(); ++j){
     for(size_t m = 0; m < mets->size(); ++m){
       
       reco::NamedCompositeCandidate Top,W;

       Top.addDaughter(electrons->at(i),"Lepton");
       Top.addDaughter(electrons->at(i),"Electron");
       Top.addDaughter(jets->at(j),"BJet");
       Top.addDaughter(mets->at(m),"MET");
       
       Top.addDaughter(W,"W");
       
       W.setP4(electrons->at(i).p4()+mets->at(m).p4());
       Top.setP4(W.p4()+jets->at(j).p4());
       
       TopCandidates->push_back(Top);
     }
   }
 }

 
 for(size_t i = 0; i < muons->size(); ++i){
   for(size_t j = 0; j < jets->size(); ++j){
     for(size_t m = 0; m < mets->size(); ++m){
       
       reco::NamedCompositeCandidate Top,W;

       Top.addDaughter(muons->at(i),"Lepton");
       Top.addDaughter(muons->at(i),"Muon");
       Top.addDaughter(jets->at(j),"BJet");
       Top.addDaughter(mets->at(m),"MET");


       Top.addDaughter(W,"W");
       
       W.setP4(muons->at(i).p4()+mets->at(m).p4());
       Top.setP4(W.p4()+jets->at(j).p4());

       TopCandidates->push_back(Top);
     }
   }
 } 
 
 
 std::auto_ptr< std::vector< reco::NamedCompositeCandidate > > newTopCandidate(TopCandidates);
 
////////

//iEvent.put(newTopLeptonic);

iEvent.put(newTopCandidate);

}

TopProducer::~TopProducer(){;}

DEFINE_FWK_MODULE( TopProducer );
