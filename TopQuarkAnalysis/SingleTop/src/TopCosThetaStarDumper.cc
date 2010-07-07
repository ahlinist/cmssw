/*
 *\Author: A. Orso M. Iorio 
 *
 *
 *\version  $Id: TopCosThetaStarDumper.cc,v 1.8 2010/03/31 12:30:22 oiorio Exp $ 
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

#include <vector>
#include <memory>

#include "DataFormats/Math/interface/LorentzVector.h"


//using namespace pat;


TopCosThetaStarDumper::TopCosThetaStarDumper(const edm::ParameterSet& iConfig) 
{
  // initialize the configurables
  jetsSrc_                 = iConfig.getParameter<edm::InputTag>( "jetsSource" );
  topsSrc_                 = iConfig.getParameter<edm::InputTag>( "topsSource" );
  
produces<std::vector<double> >("cosThetaLJ");
produces<std::vector<double> >("cosThetaStar");
produces<std::vector<double> >("cosThetaBJ");
produces<std::vector<double> >("topWTransverseMass");
}
void TopCosThetaStarDumper::produce(edm::Event & iEvent, const edm::EventSetup & iEventSetup){


edm::Handle<edm::View<reco::Candidate> > jets;
iEvent.getByLabel(jetsSrc_,jets);

edm::Handle<edm::View<reco::Candidate> > tops;
iEvent.getByLabel(topsSrc_,tops);


 std::vector<double> *cosThetaLJ = new std::vector<double>(), *cosThetaStar = new std::vector<double>(), *topWTransverseMass = new std::vector<double>(), *cosThetaBJ = new std::vector<double>();

 double cosThetaLJTmp(0), cosThetaStarTmp(0),topWTransverseMassTmp(0),cosThetaBJTmp(0);

   for( edm::View<reco::Candidate>::const_iterator it_tops = tops->begin();it_tops != tops->end();++it_tops){

     const reco::Candidate * Lepton = it_tops->daughter("Lepton"); 
     const reco::Candidate * BJet = it_tops->daughter("BJet"); 
     const reco::Candidate * MET    = it_tops->daughter("MET");          
     
    topWTransverseMassTmp = sqrt(pow(Lepton->et()+MET->pt(),2) - pow(Lepton->px()+MET->px(),2) - pow(Lepton->py()+MET->py(),2) );

    topWTransverseMass->push_back(topWTransverseMassTmp);
 
 for( edm::View<reco::Candidate>::const_iterator it_jets = jets->begin();it_jets != jets->end();++it_jets){
   
      cosThetaLJTmp = ((Lepton->px()*it_jets->px()) + (Lepton->py()*it_jets->py()) + (Lepton->pz()*it_jets->pz()))/(Lepton->p()*it_jets->p()); 
      cosThetaBJTmp = ((BJet->px()*it_jets->px()) + (BJet->py()*it_jets->py()) + (BJet->pz()*it_jets->pz()))/(BJet->p()*it_jets->p()); 

      cosThetaStarTmp = cos(Lepton->theta());
      if(it_jets->pz()>0) cosThetaStarTmp = cos(Lepton->theta());
      else cosThetaStarTmp = cos(TMath::Pi() - Lepton->theta());
     
 
      cosThetaStar->push_back(cosThetaStarTmp);
      cosThetaLJ->push_back(cosThetaLJTmp);
      cosThetaBJ->push_back(cosThetaBJTmp);
    }
   }


   std::auto_ptr< std::vector< double > > cosThetaLJPoi(cosThetaLJ), cosThetaStarPoi(cosThetaStar),topWTransverseMassPoi(topWTransverseMass), cosThetaBJPoi(cosThetaBJ);

   iEvent.put(cosThetaLJPoi,"cosThetaLJ");
   iEvent.put(cosThetaStarPoi,"cosThetaStar");
   iEvent.put(topWTransverseMassPoi,"topWTransverseMass");
   iEvent.put(cosThetaBJPoi,"cosThetaBJ");

}

TopCosThetaStarDumper::~TopCosThetaStarDumper(){;}

DEFINE_FWK_MODULE( TopCosThetaStarDumper );
