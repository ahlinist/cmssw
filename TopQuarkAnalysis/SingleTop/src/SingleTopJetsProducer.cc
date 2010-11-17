/*
 *\Author: A. Orso M. Iorio 
 *
 *
 *\version  $Id: SingleTopJetsProducer.cc,v 1.3 2010/09/09 13:57:36 oiorio Exp $ 
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
#include "DataFormats/JetReco/interface/JPTJet.h"
#include "DataFormats/JetReco/interface/CaloJet.h"

#include "DataFormats/PatCandidates/interface/JetCorrFactors.h"

#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"


#include "FWCore/Framework/interface/Selector.h"



#include "TopQuarkAnalysis/SingleTop/interface/SingleTopJetsProducer.h"

#include <vector>
#include <memory>

#include "DataFormats/Math/interface/LorentzVector.h"


//using namespace pat;


SingleTopJetsProducer::SingleTopJetsProducer(const edm::ParameterSet& iConfig) 
{
  src_                 = iConfig.getParameter<edm::InputTag>	      ( "src" );
  eleSrc_                = iConfig.getParameter<edm::InputTag>	      ( "eleSrc" );
  muSrc_                = iConfig.getParameter<edm::InputTag>	      ( "muSrc" );

  ptCut_ = iConfig.getUntrackedParameter<double> ("ptCut",30);

  isJPT = iConfig.getUntrackedParameter<bool> ("isJPT",true);
  isPF = iConfig.getUntrackedParameter<bool> ("isPF",false);


produces<std::vector<pat::Jet> >();
//produces<std::vector<pat::Jet> >();

//edm::ParameterSet overlapPSet = iConfig.getParameter<edm::ParameterSet>("leptonsSrc");
  //  std::vector<edm::InputTag> names = overlapPSet.getParameterNamesForType< edm::InputTag >

}

void SingleTopJetsProducer::produce(edm::Event & iEvent, const edm::EventSetup & iEventSetup){



  edm::Handle<edm::ValueMap<reco::JetID> > jetIDs;
  iEvent.getByLabel("ak5JetID",jetIDs);


    
  edm::Handle<edm::View<pat::Jet> > jets;
  iEvent.getByLabel(src_,jets);
  
  edm::Handle<edm::View<reco::Candidate> > eles;
  iEvent.getByLabel(eleSrc_,eles);
  

  edm::Handle<edm::View<reco::Candidate> > mus;
  iEvent.getByLabel(muSrc_,mus);
  
  //  for(std::vector<edm::InputTag>::const_iterator it=names.begin();it != names.end;++it){}



  
  std::vector< pat::Jet > * finalJets = new std::vector< pat::Jet > ;

  
for(size_t i = 0; i < jets->size(); ++i){
  bool isIsolated = true; 
  bool condition = true;
    
    if(jets->at(i).isPFJet())isPF = true;   

    if(isPF)isJPT = false;
    if(isJPT)isPF = false;

    //    std::cout << " is PF " << isPF << " isJPT " << isJPT << std::endl;

    if(isJPT){
      //std::cout<< " mark 0 " <<std::endl; 

      const reco::JPTJet * jptJet = dynamic_cast<const reco::JPTJet *>(jets->at(i).originalObject()); 
      reco::CaloJet const * myCalo = dynamic_cast<reco::CaloJet const *>( &* jptJet->getCaloJetRef());
      condition = jptJet->pt()>ptCut_ && fabs(jptJet->eta())<5;
      //     std::cout <<  "jet fHPD "<< jets->at(i).jetID().fHPD<<" n90Hits  " << jets->at(i).jetID().n90Hits<< " calon90Hits "<< (*jetIDs)[jptJet->getCaloJetRef()].n90Hits<< " calofHPD " << (*jetIDs)[jptJet->getCaloJetRef()].fHPD <<std::endl;

      double fHPD = (*jetIDs)[jptJet->getCaloJetRef()].fHPD;
      double n90Hits = (*jetIDs)[jptJet->getCaloJetRef()].n90Hits;

      condition =  condition && (myCalo->emEnergyFraction()>0.01 &&  (fabs(myCalo->eta()) > 2.4 || ( n90Hits > 1 && fHPD < 0.98)) ) ;// jetID().fHPD()< 0.98;
       //std::cout <<"size"<<jets->size()<<" emenfrac " <<myCalo->emEnergyFraction()<<std::endl;
    }
    else if(isPF){
      
      condition = (jets->at(i).numberOfDaughters()>1 && jets->at(i).neutralHadronEnergyFraction() < 1 && jets->at(i).neutralEmEnergyFraction() < 1 && ((fabs(jets->at(i).eta())>2.4) || ( jets->at(i).chargedHadronEnergyFraction() > 0 && jets->at(i).chargedMultiplicity()>0)) && jets->at(i).pt()> ptCut_ && fabs(jets->at(i).eta())<5)  ;
      //      std::cout << " ndau " << jets->at(i).numberOfDaughters() << " neuHadEn "<< jets->at(i).neutralHadronEnergyFraction() << " EmFrac " <<jets->at(i).neutralEmEnergyFraction() << " eta " << fabs(jets->at(i).eta())<< " HadEnFrac " << jets->at(i).chargedHadronEnergyFraction() << " CharMul "<< jets->at(i).chargedMultiplicity() << " pt "<<jets->at(i).pt()<<" passes ? " << condition<<std::endl; 
    }
    if(!(condition) )continue; 
    
    //std::cout << "mark 2"<< std::endl;

    for(size_t j = 0; j < eles->size(); ++j){
      
      if(deltaR(jets->at(i),eles->at(j))<0.3) {
	isIsolated = false;
	break;
      }
    }

    //    std::cout<<" isIsolatedEle? " << isIsolated << std::endl;

    for(size_t j = 0; j < mus->size(); ++j){
      
      if(deltaR(jets->at(i),mus->at(j))<0.1) {
	isIsolated = false;
	break;
      }
    }

    //    std::cout<<" isIsolatedMu? " << isIsolated << std::endl;

    //std::cout << "mark 3"<< std::endl;

    if(isIsolated)finalJets->push_back(jets->at(i)); 
//std::cout << "mark 4"<< std::endl;
  } 
  
  
  std::auto_ptr< std::vector< pat::Jet > > finalJetsPtr(finalJets);
////std::cout << "mark 5"<< std::endl;  
  
  iEvent.put(finalJetsPtr);

//std::cout << "mark 6"<< std::endl;  
  
}

SingleTopJetsProducer::~SingleTopJetsProducer(){;}
DEFINE_FWK_MODULE(SingleTopJetsProducer);
