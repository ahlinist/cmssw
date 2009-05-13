#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

class XiEdmNtupleDumper : public edm::EDProducer {
public:
  XiEdmNtupleDumper( const edm::ParameterSet & );
private:
  void produce( edm::Event &, const edm::EventSetup & );
  edm::InputTag particlesTag_;
  edm::InputTag muonTag_;
  edm::InputTag calometTag_;            

  double comEnergy_;
 
  bool useMuons_;
  bool useMET_; 
  bool useCaloTowers_;

  double HFplusThresh_;
  double HFminusThresh_;
  double HEplusThresh_;
  double HEminusThresh_;
  double HBThresh_;
};

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/CaloTowers/interface/CaloTower.h"
#include "DataFormats/CaloTowers/interface/CaloTowerFwd.h"
#include "DataFormats/CaloTowers/interface/CaloTowerDetId.h"
#include "DataFormats/HcalDetId/interface/HcalSubdetector.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"

using namespace reco;

XiEdmNtupleDumper::XiEdmNtupleDumper(const edm::ParameterSet& pset) {
   comEnergy_ = pset.getParameter<double>("comEnergy");
   particlesTag_ = pset.getParameter<edm::InputTag>("ParticlesTag");

   useMuons_ = pset.getParameter<bool>("UseMuonInfo");
   if(useMuons_) muonTag_ = pset.getParameter<edm::InputTag>("MuonTag");

   useMET_ = pset.getParameter<bool>("UseMETInfo");
   if(useMET_) calometTag_ = pset.getParameter<edm::InputTag>("CaloMETTag");

   useCaloTowers_ = pset.getParameter<bool>("UseCaloTowers");
   if(useCaloTowers_){
      HFplusThresh_ = pset.getParameter<double>("HFPlusTowerThreshold");
      HFminusThresh_ = pset.getParameter<double>("HFMinusTowerThreshold");
      HEplusThresh_ = pset.getParameter<double>("HEPlusTowerThreshold");
      HEminusThresh_ = pset.getParameter<double>("HEMinusTowerThreshold");
      HBThresh_ = pset.getParameter<double>("HBTowerThreshold");
   } 

   std::string alias;
   produces<double>( alias = "xiPlus" ).setBranchAlias( alias );
   produces<double>( alias = "xiMinus" ).setBranchAlias( alias );
}

void XiEdmNtupleDumper::produce(edm::Event& event, const edm::EventSetup& setup) {

   edm::Handle<edm::View<Candidate> > particlesCollectionH;
   event.getByLabel(particlesTag_,particlesCollectionH);

   double xi_plus = 0.;
   double xi_minus = 0.; 
   for(edm::View<Candidate>::const_iterator part = particlesCollectionH->begin();
                                            part != particlesCollectionH->end(); ++part){

      if(useCaloTowers_){
         const CaloTower* caloTower = dynamic_cast<const CaloTower*>(&*part);
         // Check from which sub-detector
         bool hasHF = false;
         bool hasHE = false;
         bool hasHB = false;
         bool hasECAL = false;
         for(size_t iconst = 0; iconst < caloTower->constituentsSize(); ++iconst){
            DetId detId = caloTower->constituent(iconst);
            if(detId.det()==DetId::Hcal){
               HcalDetId hcalDetId(detId);
               if(hcalDetId.subdet()==HcalForward) hasHF = true;		
               else if(hcalDetId.subdet()==HcalEndcap) hasHE = true;
               else if(hcalDetId.subdet()==HcalBarrel) hasHB = true;
            } else if(detId.det()==DetId::Ecal) hasECAL = true;
         }

         int zside = caloTower->id().zside();
         double energy = caloTower->energy();

         double HFThresh = (zside > 0)?HFplusThresh_:HFminusThresh_;
         double HEThresh = (zside > 0)?HEplusThresh_:HEminusThresh_;
         if(hasHF){
            if(energy >= HFThresh){
               xi_plus += caloTower->et()*exp(caloTower->eta());
               xi_minus += caloTower->et()*exp(-caloTower->eta());
            }
         } else if(hasHE){
            if(energy >= HEThresh){
               xi_plus += caloTower->et()*exp(caloTower->eta());
               xi_minus += caloTower->et()*exp(-caloTower->eta());
            }
         } else if(hasHB){
            if(energy >= HBThresh_){
               xi_plus += caloTower->et()*exp(caloTower->eta());
               xi_minus += caloTower->et()*exp(-caloTower->eta());
            }
         }
      } else {// Implement thresholds only for HF
         xi_plus += part->et()*exp(part->eta());
         xi_minus += part->et()*exp(-part->eta());
      }
   }

   //Muons
   edm::Handle<edm::View<Candidate> > muonCollectionH; 
   event.getByLabel(muonTag_,muonCollectionH);

   math::XYZTLorentzVector summuons(0.,0.,0.,0.);
   for(edm::View<Candidate>::const_iterator muon = muonCollectionH->begin(); muon != muonCollectionH->end(); ++muon){
      if(!muon->isGlobalMuon()) continue;
      summuons += muon->p4();
      xi_plus += muon->et()*exp(muon->eta());
      xi_minus += muon->et()*exp(-muon->eta());
   }

   //Calo MET
   if(useMET_){// Assume for now uncorrected MET and correct for muons locally
      edm::Handle<edm::View<Candidate> > calometColl;
      event.getByLabel(calometTag_,calometColl);
      const Candidate& calomet = (*calometColl)[0];
	
      math::XYZTLorentzVector corrMET((calomet.px() - summuons.px()),(calomet.py() - summuons.py()),0.,sqrt((calomet.px() - summuons.px())*(calomet.px() - summuons.px()) + (calomet.py() - summuons.py())*(calomet.py() - summuons.py())));

      //assume eta = 0 for MET
      xi_plus += corrMET.pt();
      xi_minus += corrMET.pt();
   }

   xi_plus /= comEnergy_;
   xi_minus /= comEnergy_;

   std::auto_ptr<double> xiPlus(new double(xi_plus));
   std::auto_ptr<double> xiMinus(new double(xi_minus));

   event.put( xiPlus, "xiPlus" );
   event.put( xiMinus, "xiMinus" );
}

DEFINE_FWK_MODULE(XiEdmNtupleDumper);
 
