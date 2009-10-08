#include "ElectroWeakAnalysis/TauTriggerEfficiency/interface/L2TauEfficiencyAnalyzer.h"
#include "RecoTauTag/HLTProducers/interface/L2TauIsolationAlgs.h"

#include "Geometry/Records/interface/CaloGeometryRecord.h"

#include "Math/GenVector/VectorUtil.h"
#include <iostream>
#include <iomanip>
#include <fstream>


L2TauEfficiencyAnalyzer::L2TauEfficiencyAnalyzer()
{


}




void
L2TauEfficiencyAnalyzer::Setup(const edm::ParameterSet& iConfig,TTree* l2tree)
{
  l2TauInfoAssoc_ = iConfig.getParameter<edm::InputTag>("L2AssociationCollection");
  CaloTowers_   = iConfig.getUntrackedParameter<edm::InputTag>("CaloTowers");
  EERecHits_   = iConfig.getUntrackedParameter<edm::InputTag>("EERecHits");
  EBRecHits_   = iConfig.getUntrackedParameter<edm::InputTag>("EBRecHits");
  matchDR_ = iConfig.getParameter<double>("L2matchingDeltaR");
  outerCone_ = iConfig.getUntrackedParameter<double>("outerCone");
  innerCone_ = iConfig.getUntrackedParameter<double>("innerCone");
  crystalThresholdE_ = iConfig.getUntrackedParameter<double>("crystalThresholdEE");
  crystalThresholdB_ = iConfig.getUntrackedParameter<double>("crystalThresholdEB");


  //Setup Branches
  l2tree->Branch("L2ECALIsolationEt",&ecalIsol_Et,"L2ECALIsolationEt/F");
  l2tree->Branch("L2TowerIsolationEt",&towerIsol_Et,"L2TowerIsolationEt/F");
  l2tree->Branch("L2ClusterEtaRMS",&cl_etaRMS,"L2ClusterEtaRMS/F");
  l2tree->Branch("L2ClusterPhiRMS",&cl_phiRMS,"L2ClusterPhiRMS/F");
  l2tree->Branch("L2ClusterDeltaRRMS",&cl_drRMS,"L2ClusterDeltaRRMS/F");
  l2tree->Branch("L2NClustersInAnnulus",&cl_Nclusters,"L2NClustersInAnnulus/I");
  l2tree->Branch("L2SeedTowerEt",&seedTowerEt,"L2SeedTowerEt/F");
  l2tree->Branch("L2JetEt",&JetEt,"L2JetEt/F");
  l2tree->Branch("L2JetEta",&JetEta,"L2JetEta/F");
  l2tree->Branch("L2JetPhi",&JetPhi,"L2JetPhi/F");
  l2tree->Branch("L2HadEnergyFraction",&hadFraction,"L2HadEnergyFraction/F");
  l2tree->Branch("L2NTowers60",&NTowers60,"L2NTowers60/I");
  l2tree->Branch("L2NTowers90",&NTowers90,"L2NTowers90/I");
  l2tree->Branch("hasMatchedL2Jet",&hasL2Jet,"hasMatchedL2Jet/I");
  l2tree->Branch("PFEGIsolEt",&PFEGIsolEt,"PFEGIsolEt/F");
  l2tree->Branch("PFECALIsolationEt",&PFEcalIsol_Et,"PFECALIsolationEt/F");
  l2tree->Branch("PFHCALIsolationEt",&PFHcalIsol_Et,"PFHCALIsolationEt/F");
  l2tree->Branch("PFNEGammaCandsAnnulus",&NEGCandsInAnnulus,"PFNEGammaCandsAnnulus/I");
  l2tree->Branch("PFNHadCandsAnnulus",&NHadCandsInAnnulus,"PFNHadCandsAnnulus/I");
  l2tree->Branch("PFHighestClusterEt",&PFHighestClusterEt,"PFHighestClusterEt/F");
  l2tree->Branch("PFEGammaClusterEtaRMS",&PFEGEtaRMS,"PFEGammaClusterEtaRMS/F");
  l2tree->Branch("PFEGammaClusterPhiRMS",&PFEGPhiRMS,"PFEGammaClusterPhiRMS/F");
  l2tree->Branch("PFEGammaClusterDeltaRRMS",&PFEGDrRMS,"PFEGammaClusterDeltaRRMS/F");


}



L2TauEfficiencyAnalyzer::~L2TauEfficiencyAnalyzer()
{
}




void
L2TauEfficiencyAnalyzer::fill(const edm::Event& iEvent,const edm::EventSetup& iSetup,const LV& tau)
{
   using namespace edm;
   using namespace reco;


	   //Reset the variables
	   ecalIsol_Et=0.;
	   towerIsol_Et=0.;
	   cl_etaRMS=0.;
	   cl_phiRMS=0.;
	   cl_drRMS=0.;
	   cl_Nclusters=0;
	   seedTowerEt = 0.;
	   JetEt=0.;
	   JetEta=-999.;
	   JetPhi=-999.;
	   hadFraction =0.;
	   NTowers60=0;
	   NTowers90=0;
	   hasL2Jet=0;
	   PFEGIsolEt =0.;
	   PFEGEtaRMS = 0.;
	   PFHighestClusterEt =0.; 
	   PFEGEtaRMS =0.; 
	   PFEGPhiRMS = 0.;
	   PFEGDrRMS = 0.;
	   PFEcalIsol_Et = 0;
	   PFHcalIsol_Et = 0;
	   NEGCandsInAnnulus =0; 
	   NHadCandsInAnnulus =0;


	   //Fill the offline ECALIIsolation variable 
	   math::PtEtaPhiELorentzVectorCollection hits = getECALHits(tau,iEvent,iSetup);
	   PFEcalIsol_Et = isolatedEt(tau,hits );

	   math::PtEtaPhiELorentzVectorCollection towers = getHCALTowers(tau,iEvent,iSetup);
	   PFHcalIsol_Et = isolatedEt(tau,towers);

  //Now look if there is L2 Association in the evnt.If yes,match to the L2 and fill L2 Variables
  Handle<L2TauInfoAssociation> l2TauInfoAssoc; //Handle to the input (L2 Tau Info Association)
  iEvent.getByLabel(l2TauInfoAssoc_,l2TauInfoAssoc); //get the handle
  if(l2TauInfoAssoc.isValid()) {
    matchAndFillL2(tau,*l2TauInfoAssoc);
  } else {
    std::cout<<"No L2TauInfoAssociation in event!"<<std::endl;
  }
   

}


void
L2TauEfficiencyAnalyzer::fill(const edm::Event& iEvent,const edm::EventSetup& iSetup,const reco::PFTau& tau)
{
  fill(iEvent,iSetup,tau.p4());


  //Correlate Particle Flow and L2 Clustering cuts
  PFEGIsolEt = tau.isolationPFGammaCandsEtSum();
  PFHighestClusterEt = tau.maximumHCALPFClusterEt();
  NEGCandsInAnnulus = tau.isolationPFGammaCands().size();
  NHadCandsInAnnulus = tau.isolationPFChargedHadrCands().size() + tau.isolationPFNeutrHadrCands().size();

  std::vector<double> rms = clusterSeparation(tau.isolationPFGammaCands(),tau.signalPFCands());
  PFEGEtaRMS = rms[0];
  PFEGPhiRMS = rms[1];
  PFEGDrRMS = rms[2];




}

void
L2TauEfficiencyAnalyzer::fill(const edm::Event& iEvent,const edm::EventSetup& iSetup,const reco::CaloTau& tau)
{
  fill(iEvent,iSetup,tau.p4());
}

void
L2TauEfficiencyAnalyzer::fill(const edm::Event& iEvent,const edm::EventSetup& iSetup,const reco::GsfElectron& tau) {
  fill(iEvent,iSetup,tau.p4());
}


std::vector<double> 
L2TauEfficiencyAnalyzer::clusterSeparation(const reco::PFCandidateRefVector& isol_cands,const reco::PFCandidateRefVector& signal_cands)
{
  LV center(0.,0.,0.,0.);
  
  //find the weighted position
  if(isol_cands.size()>0)
  for(reco::PFCandidateRefVector::const_iterator i=isol_cands.begin();i!=isol_cands.end();++i)
    {
      center+=(*i)->p4();
    }

  if(signal_cands.size()>0)
  for(reco::PFCandidateRefVector::const_iterator i=signal_cands.begin();i!=signal_cands.end();++i)
    {
      center+=(*i)->p4();
    }

  //Now find the rms
  double sumet=0;
  double etarms=0;
  double phirms=0;
  double drrms=0;

  if(isol_cands.size()>0)
  for(reco::PFCandidateRefVector::const_iterator i=isol_cands.begin();i!=isol_cands.end();++i)
    {
      sumet+=(*i)->et();
      etarms+=(*i)->et()*pow((*i)->eta()-center.Eta(),2);
      phirms+=(*i)->et()*pow(ROOT::Math::VectorUtil::DeltaPhi(center,(*i)->p4()),2);
      drrms+=(*i)->et()*pow(ROOT::Math::VectorUtil::DeltaR(center,(*i)->p4()),2);

    }

  if(sumet<0.1)
    sumet=1;

  std::vector<double> out;
  out.push_back(etarms/sumet);
  out.push_back(phirms/sumet);
  out.push_back(drrms/sumet);

  return out;
}


void
L2TauEfficiencyAnalyzer::matchAndFillL2(const LV& jet,const L2TauInfoAssociation& l2iinfo)
{

  //Loop On the Collection and see if your tau jet is matched to one there
 //Also find the nearest Matched MC Particle to your Jet (to be complete)

 

 double delta_min=100.;
 L2TauInfoAssociation::const_iterator matchIt;

 if(l2iinfo.size()>0)
  for(L2TauInfoAssociation::const_iterator it = l2iinfo.begin();it!=l2iinfo.end();++it)
   {
	  const reco::CaloJet& l2Jet =*(it->key);
     	  double delta = ROOT::Math::VectorUtil::DeltaR(jet,l2Jet.p4().Vect());
	  if(delta<matchDR_)
	    {

	      if(delta<delta_min)
		{
		  delta_min=delta;
		  hasL2Jet=1;
		  const reco::L2TauIsolationInfo l2info = (it->val);
		  ecalIsol_Et=l2info.ecalIsolEt();
		  towerIsol_Et=l2info.hcalIsolEt();
		  cl_Nclusters=l2info.nEcalHits();
		  cl_etaRMS=l2info.ecalClusterShape()[0];
		  cl_phiRMS=l2info.ecalClusterShape()[1];
		  cl_drRMS=l2info.ecalClusterShape()[2];
		  seedTowerEt = l2info.seedHcalHitEt();
		  JetEt = l2Jet.et();
		  JetEta = l2Jet.eta();
		  JetPhi = l2Jet.phi();
		  //		  hadFraction = l2Jet.energyFractionHadronic();
		  //		  NTowers60 = l2Jet.n60();
		  //		  NTowers90 = l2Jet.n90();

		}
	    }
   }

}



math::PtEtaPhiELorentzVectorCollection 
L2TauEfficiencyAnalyzer::getHCALTowers(const LV& jet,const edm::Event& iEvent,const edm::EventSetup& iSetup)
{
  using namespace edm;
  using namespace reco;

  edm::Handle<CaloTowerCollection> caltowers;
  iEvent.getByLabel(CaloTowers_,caltowers);

  //Create a container for the hcal towers
  math::PtEtaPhiELorentzVectorCollection jetHcaltowers;
  
  for (CaloTowerCollection::const_iterator cnd=caltowers->begin(); cnd!=caltowers->end(); cnd++) {
    math::PtEtaPhiELorentzVector p(cnd->hadEt(), cnd->eta(), cnd->phi(), cnd->hadEnergy());
    if(ROOT::Math::VectorUtil::DeltaR(p,jet) <outerCone_) {
      jetHcaltowers.push_back(p);
    }
  }
  return jetHcaltowers;
}


math::PtEtaPhiELorentzVectorCollection 
L2TauEfficiencyAnalyzer::getECALHits(const LV& jet,const edm::Event& iEvent,const edm::EventSetup& iSetup)
{

using namespace edm;
using namespace reco;

  //Init Geometry
  ESHandle<CaloGeometry> geometry;
  iSetup.get<CaloGeometryRecord>().get(geometry);

  //Create ECAL Geometry
  const CaloSubdetectorGeometry* EB = geometry->getSubdetectorGeometry(DetId::Ecal,EcalBarrel);
  const CaloSubdetectorGeometry* EE = geometry->getSubdetectorGeometry(DetId::Ecal,EcalEndcap);

  //Handle To the ECAL
  Handle<EBRecHitCollection> EBRecHits;
  Handle<EERecHitCollection> EERecHits;

  //Create a container for the hits
  math::PtEtaPhiELorentzVectorCollection jetRecHits;

  //Loop on the barrel hits
  if(iEvent.getByLabel( EBRecHits_, EBRecHits))
     for(EBRecHitCollection::const_iterator hit = EBRecHits->begin();hit!=EBRecHits->end();++hit)
       {
	 //get Detector Geometry
	 const CaloCellGeometry* this_cell = EB->getGeometry(hit->detid());
	 GlobalPoint posi = this_cell->getPosition();
	 double energy = hit->energy();
	 double eta = posi.eta();
	 double phi = posi.phi();
	 double theta = posi.theta();
	 if(theta > M_PI) theta = 2 * M_PI- theta;
	 double et = energy * sin(theta);
	 math::PtEtaPhiELorentzVector p(et, eta, phi, energy);
	 if(ROOT::Math::VectorUtil::DeltaR(p,jet) <outerCone_)
	   if(p.energy()>crystalThresholdB_)
	     jetRecHits.push_back(p);
       }

 if(iEvent.getByLabel( EERecHits_, EERecHits))
     for(EERecHitCollection::const_iterator hit = EERecHits->begin();hit!=EERecHits->end();++hit)
       {
	 //get Detector Geometry
	 const CaloCellGeometry* this_cell = EE->getGeometry(hit->detid());
	 GlobalPoint posi = this_cell->getPosition();
	 double energy = hit->energy();
	 double eta = posi.eta();
	 double phi = posi.phi();
	 double theta = posi.theta();
	 if(theta > M_PI) theta = 2 * M_PI- theta;
	 double et = energy * sin(theta);
	 math::PtEtaPhiELorentzVector p(et, eta, phi, energy);
	 if(ROOT::Math::VectorUtil::DeltaR(p,jet) < outerCone_)
	   if(p.energy()>crystalThresholdE_)
	     jetRecHits.push_back(p);
       }
 return jetRecHits;
}



double 
L2TauEfficiencyAnalyzer::isolatedEt(const LV& jet,const math::PtEtaPhiELorentzVectorCollection& towers ) const
{
  
  double eRMin= 0.;
  double eRMax =0.;
  
  for(math::PtEtaPhiELorentzVectorCollection::const_iterator u = towers.begin();u!=towers.end();++u)
	{
	  double delta = ROOT::Math::VectorUtil::DeltaR(jet, *u);
	  if(delta<outerCone_)
	    eRMax+=u->pt();
	  if(delta<innerCone_)
	    eRMin+= u->pt();
	}
    
  double etIsol = eRMax - eRMin;
  return etIsol;
}

