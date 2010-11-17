/*
 *\Author: A. Orso M. Iorio 
 *
 *
 *\version  $Id: TopCosThetaStarDumper.cc,v 1.2 2010/11/09 14:38:29 oiorio Exp $ 
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


#include "TopQuarkAnalysis/SingleTop/interface/TopCosThetaStarDumper.h"
#include "TopQuarkAnalysis/SingleTop/interface/CandidateBooster.h"


#include <vector>
#include <memory>

#include "DataFormats/Math/interface/LorentzVector.h"


//using namespace pat;


TopCosThetaStarDumper::TopCosThetaStarDumper(const edm::ParameterSet& iConfig) 
{
  // initialize the configurables
  //  jetsSrc_                 = iConfig.getParameter<edm::InputTag>( "jetsSource" );
  //  topsSrc_                 = iConfig.getParameter<edm::InputTag>( "topsSource" );

tChanSrc_ = iConfig.getParameter<edm::InputTag>( "tChanSource" );
  
produces<std::vector<double> >("cosThetaLJ");
produces<std::vector<double> >("cosThetaStar");
produces<std::vector<double> >("cosThetaBJ");
produces<std::vector<double> >("leptonJetDeltaR");
produces<std::vector<double> >("bJetPt");
produces<std::vector<double> >("topWTransverseMass");
}
void TopCosThetaStarDumper::produce(edm::Event & iEvent, const edm::EventSetup & iEventSetup){


  //edm::Handle<edm::View<reco::Candidate> > jets;
//iEvent.getByLabel(jetsSrc_,jets);

//edm::Handle<edm::View<reco::Candidate> > tops;
//iEvent.getByLabel(topsSrc_,tops);

edm::Handle<edm::View<reco::Candidate> > tChan;
iEvent.getByLabel(tChanSrc_,tChan);


 std::vector<double> *cosThetaLJ = new std::vector<double>(), *cosThetaStar = new std::vector<double>(), *topWTransverseMass = new std::vector<double>(), *cosThetaBJ = new std::vector<double>(), *leptonJetDeltaR = new std::vector<double>(), *bJetPt = new std::vector<double>();

 double cosThetaLJTmp(0), cosThetaStarTmp(0),topWTransverseMassTmp(0),cosThetaBJTmp(0),leptonJetDeltaRTmp(0),bJetPtTmp(0);


   for( edm::View<reco::Candidate>::const_iterator it_tChan = tChan->begin();it_tChan != tChan->end();++it_tChan){
     
     CenterOfMassBooster booster(*it_tChan->daughter("Top"));
     //reco::CandidateBaseRef candToBoostRef(edm::Ref<reco::Candidate>(,));
     leptonJetDeltaRTmp = (double)deltaR(*it_tChan->daughter("Top")->daughter("BJet"),*it_tChan->daughter("Top")->daughter("Lepton"));
     bJetPtTmp = (double)(it_tChan->daughter("Top")->daughter("BJet")->pt());
     bJetPt->push_back(bJetPtTmp);

     leptonJetDeltaR->push_back(leptonJetDeltaRTmp);
     
     reco::Candidate * candToBoost = it_tChan->clone();
     booster.set(*candToBoost);
     
     const reco::Candidate * Lepton = candToBoost->daughter("Top")->daughter("Lepton"); 
     const reco::Candidate * BJet = candToBoost->daughter("Top")->daughter("BJet"); 
     const reco::Candidate * MET    = candToBoost->daughter("Top")->daughter("MET");          
     
     topWTransverseMassTmp = sqrt(pow(Lepton->et()+MET->pt(),2) - pow(Lepton->px()+MET->px(),2) - pow(Lepton->py()+MET->py(),2) );

     topWTransverseMass->push_back(topWTransverseMassTmp);
     
     //   for( edm::View<reco::Candidate>::const_iterator it_jets = jets->begin();it_jets != jets->end();++it_jets){
     
     const reco::Candidate * Jet = candToBoost->daughter("LightJet");   
     
     cosThetaLJTmp = ((Lepton->px()*Jet->px()) + (Lepton->py()*Jet->py()) + (Lepton->pz()*Jet->pz()))/(Lepton->p()*Jet->p()); 
     cosThetaBJTmp = ((BJet->px()*Jet->px()) + (BJet->py()*Jet->py()) + (BJet->pz()*Jet->pz()))/(BJet->p()*Jet->p()); 

      cosThetaStarTmp = cos(Lepton->theta());
      if(Jet->pz()>0) cosThetaStarTmp = cos(Lepton->theta());
      else cosThetaStarTmp = cos(TMath::Pi() - Lepton->theta());
      
      
      cosThetaStar->push_back(cosThetaStarTmp);
      cosThetaLJ->push_back(cosThetaLJTmp);
      cosThetaBJ->push_back(cosThetaBJTmp);

   }

   
   std::auto_ptr< std::vector< double > > cosThetaLJPoi(cosThetaLJ), cosThetaStarPoi(cosThetaStar),topWTransverseMassPoi(topWTransverseMass), cosThetaBJPoi(cosThetaBJ), leptonJetDeltaRPoi(leptonJetDeltaR), bJetPtPoi(bJetPt);
   
   iEvent.put(cosThetaLJPoi,"cosThetaLJ");
   iEvent.put(cosThetaStarPoi,"cosThetaStar");
   iEvent.put(topWTransverseMassPoi,"topWTransverseMass");
   iEvent.put(cosThetaBJPoi,"cosThetaBJ");
   iEvent.put(leptonJetDeltaRPoi,"leptonJetDeltaR");
   iEvent.put(bJetPtPoi,"bJetPt")

;
 
}

TopCosThetaStarDumper::~TopCosThetaStarDumper(){;}

DEFINE_FWK_MODULE( TopCosThetaStarDumper );
