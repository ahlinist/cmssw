#include "ElectroWeakAnalysis/TauTriggerEfficiency/interface/L2TauEfficiencyAnalyzer.h"
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
  matchDR_ = iConfig.getParameter<double>("L2matchingDeltaR");

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
L2TauEfficiencyAnalyzer::fill(const edm::Event& iEvent,const LV& tau)
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
	   JetEta=0.;
	   JetPhi=0.;
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
	   NEGCandsInAnnulus =0; 
	   NHadCandsInAnnulus =0;



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
L2TauEfficiencyAnalyzer::fill(const edm::Event& iEvent,const reco::PFTau& tau)
{
  fill(iEvent,tau.p4());


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
L2TauEfficiencyAnalyzer::fill(const edm::Event& iEvent,const reco::CaloTau& tau)
{
  fill(iEvent,tau.p4());
}

void
L2TauEfficiencyAnalyzer::fill(const edm::Event& iEvent,const reco::GsfElectron& tau) {
  fill(iEvent,tau.p4());
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
		  ecalIsol_Et=l2info.ECALIsolConeCut;
		  towerIsol_Et=l2info.TowerIsolConeCut;
		  cl_Nclusters=l2info.ECALClusterNClusters;
		  cl_etaRMS=l2info.ECALClusterEtaRMS;
		  cl_phiRMS=l2info.ECALClusterPhiRMS;
		  cl_drRMS=l2info.ECALClusterDRRMS;
		  seedTowerEt = l2info.SeedTowerEt;
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





