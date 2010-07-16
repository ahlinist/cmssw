#ifndef DiffractiveForwardAnalysis_SingleDiffractiveWAnalysis_XiTowerDumper_h
#define DiffractiveForwardAnalysis_SingleDiffractiveWAnalysis_XiTowerDumper_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

class XiTowerDumper : public edm::EDProducer {
public:
  XiTowerDumper( const edm::ParameterSet & );
private:
  void produce( edm::Event &, const edm::EventSetup & );
  edm::InputTag calotowersTag_;
  edm::InputTag muonTag_;
  edm::InputTag calometTag_;            

  double comEnergy_;

  bool useMET_; 
  double HFplusThresh_;
  double HFminusThresh_;
  double HEplusThresh_;
  double HEminusThresh_;
  double HBThresh_;
};

#endif

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataFormats/CaloTowers/interface/CaloTower.h"
#include "DataFormats/CaloTowers/interface/CaloTowerFwd.h"
#include "DataFormats/CaloTowers/interface/CaloTowerDetId.h"
#include "DataFormats/HcalDetId/interface/HcalSubdetector.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CaloMETFwd.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"

using namespace reco;

XiTowerDumper::XiTowerDumper(const edm::ParameterSet& conf){
        comEnergy_ = conf.getParameter<double>("comEnergy"); 
	calotowersTag_ = conf.getParameter<edm::InputTag>("CaloTowersTag");
	muonTag_ = conf.getParameter<edm::InputTag>("MuonTag");

	useMET_ = conf.getParameter<bool>("UseMETInfo");
	if(useMET_) calometTag_ = conf.getParameter<edm::InputTag>("CaloMETTag");

	HFplusThresh_ = conf.getParameter<double>("HFPlusTowerThreshold");
	HFminusThresh_ = conf.getParameter<double>("HFMinusTowerThreshold");
	HEplusThresh_ = conf.getParameter<double>("HEPlusTowerThreshold");
        HEminusThresh_ = conf.getParameter<double>("HEMinusTowerThreshold");
	HBThresh_ = conf.getParameter<double>("HBTowerThreshold");

	std::string alias;
	produces<double>( alias = "xiTowerplus" ).setBranchAlias( alias );
	produces<double>( alias = "xiTowerminus" ).setBranchAlias( alias );
}

void XiTowerDumper::produce(edm::Event& evt, const edm::EventSetup& iSetup) {
	//Calo Towers
	/*edm::Handle<CandidateCollection> calotowercands;  
	evt.getByLabel(calotowersTag_,calotowercands);

	CandidateCollection::const_iterator cand = calotowercands->begin();
	CaloTowerRef towerRef = cand->get<CaloTowerRef>();
			
	const CaloTowerCollection *towerCollection = towerRef.product();*/

        edm::Handle<CaloTowerCollection> towerCollectionH;
        evt.getByLabel(calotowersTag_,towerCollectionH);
        const CaloTowerCollection& towerCollection = *(towerCollectionH.product());
 
	double xi_towers_plus = 0.;
	double xi_towers_minus = 0.; 
	for(CaloTowerCollection::const_iterator calotower = towerCollection.begin(); calotower != towerCollection.end(); ++calotower){
		bool hasHF = false;
		bool hasHE = false;
		bool hasHB = false;
		bool hasECAL = false;
		for(size_t iconst = 0; iconst < calotower->constituentsSize(); ++iconst){
			DetId detId = calotower->constituent(iconst);
			if(detId.det()==DetId::Hcal){
				HcalDetId hcalDetId(detId);
				if(hcalDetId.subdet()==HcalForward) hasHF = true;		
                                else if(hcalDetId.subdet()==HcalEndcap) hasHE = true;
				else if(hcalDetId.subdet()==HcalBarrel) hasHB = true;
			} else if(detId.det()==DetId::Ecal) hasECAL = true;
		}

		int zside = calotower->id().zside();
		double energy = calotower->energy();

		double HFThresh = (zside > 0)?HFplusThresh_:HFminusThresh_;
		double HEThresh = (zside > 0)?HEplusThresh_:HEminusThresh_;
		if(hasHF){
			if(energy >= HFThresh){
				xi_towers_plus += calotower->et()*exp(calotower->eta());
				xi_towers_minus += calotower->et()*exp(-calotower->eta());
			}
		} else if(hasHE){
			if(energy >= HEThresh){
				xi_towers_plus += calotower->et()*exp(calotower->eta());
				xi_towers_minus += calotower->et()*exp(-calotower->eta());
			}
		} else if(hasHB){
			if(energy >= HBThresh_){
                        	xi_towers_plus += calotower->et()*exp(calotower->eta());
                        	xi_towers_minus += calotower->et()*exp(-calotower->eta());
                        }
		}
	}
	
	//Muons
	edm::Handle<MuonCollection> muonCollectionH; 
        evt.getByLabel(muonTag_,muonCollectionH);

	math::XYZTLorentzVector summuons(0.,0.,0.,0.);
	for(MuonCollection::const_iterator muon = muonCollectionH->begin(); muon != muonCollectionH->end(); ++muon){
                if(!muon->isGlobalMuon()) continue;
		summuons += muon->p4();
		xi_towers_plus += muon->et()*exp(muon->eta());
                xi_towers_minus += muon->et()*exp(-muon->eta());
	}

	//Calo MET
	if(useMET_){
		edm::Handle<reco::CaloMETCollection> calometColl;
		evt.getByLabel(calometTag_,calometColl);
		const reco::Candidate& calomet = (*calometColl)[0];
	
		math::XYZTLorentzVector corrMET((calomet.px() - summuons.px()),(calomet.py() - summuons.py()),0.,sqrt((calomet.px() - summuons.px())*(calomet.px() - summuons.px()) + (calomet.py() - summuons.py())*(calomet.py() - summuons.py())));

		//assume eta = 0 for MET
		xi_towers_plus += corrMET.pt();
		xi_towers_minus += corrMET.pt();
	}

	xi_towers_plus = xi_towers_plus/comEnergy_;
	xi_towers_minus = xi_towers_minus/comEnergy_;

	std::auto_ptr<double> xiTowerplus(new double(xi_towers_plus));
	std::auto_ptr<double> xiTowerminus(new double(xi_towers_minus));

	evt.put( xiTowerplus, "xiTowerplus" );
	evt.put( xiTowerminus, "xiTowerminus" );
}

DEFINE_FWK_MODULE(XiTowerDumper);
 
