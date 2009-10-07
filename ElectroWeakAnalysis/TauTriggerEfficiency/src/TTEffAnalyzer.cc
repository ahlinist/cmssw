// -*- C++ -*-
//
// Package:    TTEffAnalyzer
// Class:      TTEffAnalyzer
// 
/**\class TTEffAnalyzer TTEffAnalyzer.cc ElectroWeakAnalysis/TTEffAnalyzer/src/TTEffAnalyzer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Chi Nhan Nguyen
//         Created:  Wed Oct  1 13:04:54 CEST 2008
// $Id: TTEffAnalyzer.cc,v 1.29 2009/06/29 19:35:25 chinhan Exp $
//
//

#include "ElectroWeakAnalysis/TauTriggerEfficiency/interface/TTEffAnalyzer.h"
#include "Math/GenVector/VectorUtil.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlock.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlockElement.h"
#include "DataFormats/ParticleFlowReco/interface/PFCluster.h"

TTEffAnalyzer::TTEffAnalyzer(const edm::ParameterSet& iConfig):
  DoMCTauEfficiency_(iConfig.getParameter<bool>("DoMCTauEfficiency")),
  PFTaus_(iConfig.getParameter<edm::InputTag>("PFTauCollection")),
  PFTauIso_(iConfig.getParameter<edm::InputTag>("PFTauIsoCollection")),
  MCTaus_(iConfig.getParameter<edm::InputTag>("MCTauCollection")),
  rootFile_(iConfig.getParameter<std::string>("outputFileName")),
  MCMatchingCone(iConfig.getParameter<double>("MCMatchingCone"))
{
  // File setup
  _TTEffFile = TFile::Open(rootFile_.c_str(), "RECREATE");
  //_TTEffFile = TFile::Open("test.root", "RECREATE");
  // Tree setup
  _TTEffTree = new TTree("TTEffTree", "Tau Trigger Efficiency Tree");

  //reset vars
  PFPt = 0.;
  PFInvPt = 0.;
  PFEt = 0.;
  PFEta = 0.;
  PFPhi = 0.;
  PFProng = 0.;
  PFIso = 0;
  PFIsoSum = 0;
  PFEnergy = 0;
  PFTauMatch = 0;
  MCMatch = 0;
  MCTauEt = -1.;
  MCTauE = -1.;
  MCTauEta = -1.;
  MCTauPhi = -1.;

  _TTEffTree->Branch("PFTauPt", &PFPt, "PFTauPt/F");
  _TTEffTree->Branch("PFTauInvPt", &PFInvPt, "PFTauInvPt/F");
  _TTEffTree->Branch("PFTauEt",&PFEt,"PFTauEt/F");
  _TTEffTree->Branch("PFTauEta", &PFEta, "PFTauEta/F");
  _TTEffTree->Branch("PFTauPhi", &PFPhi, "PFTauPhi/F");
  _TTEffTree->Branch("PFTauProng", &PFProng, "PFTauProng/F");
  _TTEffTree->Branch("PFTauIso", &PFIso, "PFTauIso/F");
  _TTEffTree->Branch("PFTauIsoSum", &PFIsoSum, "PFTauIsoSum/F");
  _TTEffTree->Branch("PFTauEnergy", &PFEnergy, "PFTauEnergy/F");
  _TTEffTree->Branch("PFClusterEtaRMS", &PFClusterEtaRMS, "PFClusterEtaRMS/F");
  _TTEffTree->Branch("PFClusterPhiRMS", &PFClusterPhiRMS, "PFClusterPhiRMS/F");
  _TTEffTree->Branch("PFClusterDrRMS", &PFClusterDrRMS, "PFClusterDrRMS/F");
  _TTEffTree->Branch("PFTauMatch", &PFTauMatch, "PFTauMatch/I");
  
  _TTEffTree->Branch("MCMatch", &MCMatch, "MCMatch/I");
  _TTEffTree->Branch("MCTauEt", &MCTauEt, "MCTauEt/F");
  _TTEffTree->Branch("MCTauE", &MCTauE, "MCTauE/F");
  _TTEffTree->Branch("MCTauEta", &MCTauEta, "MCTauEta/F");
  _TTEffTree->Branch("MCTauPhi", &MCTauPhi, "MCTauPhi/F");

  _L1analyzer.Setup(iConfig,_TTEffTree);
  _L2analyzer.Setup(iConfig,_TTEffTree);
  _L25and3analyzer.Setup(iConfig,_TTEffTree);
}

TTEffAnalyzer::~TTEffAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
TTEffAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   edm::Handle<CaloTauCollection> caloTaus;
   edm::Handle<reco::GsfElectronCollection> electronTaus;

   if( DoMCTauEfficiency_ ) { // this is to calculate efficiencies per MC tau candidate
     iEvent.getByLabel(MCTaus_, mcTaus);
     iEvent.getByLabel(PFTaus_, PFTaus);
     loop(iEvent,iSetup, *mcTaus);
   } 
   else {
     if(iEvent.getByLabel(PFTaus_, PFTaus)) {
       iEvent.getByLabel(PFTauIso_,thePFTauDiscriminatorByIsolation);
       if(!thePFTauDiscriminatorByIsolation.isValid()) {
	 cout<<"No DiscriminatorByIsolation with label "<<PFTauIso_<<" found!"<<endl;
       }
       iEvent.getByLabel(MCTaus_, mcTaus);
       loop(iEvent,iSetup,*PFTaus);
     }
     else if(iEvent.getByLabel(PFTaus_, caloTaus)) {
       loop(iEvent,iSetup, *caloTaus);
     }
     else if(iEvent.getByLabel(PFTaus_, electronTaus)) {
       loop(iEvent,iSetup, *electronTaus);
     }
   }
   
   // For electron lorentzvectors, add similar clauses
}

void TTEffAnalyzer::fillLV(const LorentzVector& tau,unsigned int i) {
  PFPt = tau.Pt();
  PFEt = tau.Et();
  PFEta = tau.Eta();
  PFPhi = tau.Phi();
}

void TTEffAnalyzer::fill(const reco::GsfElectron& tau,unsigned int i) {
  fillLV(tau.p4());
}

void
TTEffAnalyzer::fill(const reco::PFTau& tau,unsigned int i) {
using namespace reco;
  PFInvPt = 0.;
  PFIso = 0;

// Here is workaround, though not beautiful, to access PFIso info with PFRef
// While keeping the generic structure of loop method
  const PFTauRef thisTauRef(PFTaus,i);
  if(thePFTauDiscriminatorByIsolation.isValid()){
	const PFTauDiscriminator & ds = *(thePFTauDiscriminatorByIsolation.product());
	PFIso = ds[thisTauRef];// it should crash if CMSSW cannot find a match between TauRef and Iso collection
                            // Then check configuration files to make sure a correct pair is being fed into TTEFF
  }
  MCMatch = 0;
  if(mcTaus.isValid()){
    for(unsigned int k = 0 ; k < mcTaus->size(); k++){
      //Mike B: Changed this to ROOT ::Math since it was confused which deltaR to use
      if( ROOT::Math::VectorUtil::DeltaR(PFTaus->at(i).p4(),mcTaus->at(k)) < MCMatchingCone ){ // match within 0.2 cone
         MCMatch = 1;
	 MCTauE = mcTaus->at(k).energy();
	 MCTauEt = mcTaus->at(k).Et();
	 MCTauEta = mcTaus->at(k).Eta();
	 MCTauPhi = mcTaus->at(k).Phi();
        break;
      }
    }
  }
  if(thisTauRef->leadPFChargedHadrCand().isNonnull()) PFInvPt = 1./thisTauRef->leadPFChargedHadrCand()->pt();
  // Fill common variables
  fillLV(PFTaus->at(i).p4());

  // Fill #signal tracks, and PtSum in isolation annulus 
  PFProng  = PFTaus->at(i).signalPFChargedHadrCands().size(); // check config file
  PFIsoSum = PFTaus->at(i).isolationPFChargedHadrCandsPtSum();
  PFEnergy = PFTaus->at(i).energy();

  /*
  //get RMS Values of Candidates
  std::vector<double> rms = clusterSeparation(tau.isolationPFGammaCands(),tau.signalPFCands());
  PFEGEtaRMS = rms[0];
  PFEGPhiRMS = rms[1];
  PFEGDrRMS = rms[2];
  */
  std::vector<double> rms;
  clusterShape(tau, rms);
  PFClusterEtaRMS = rms[0];
  PFClusterPhiRMS = rms[1];
  PFClusterDrRMS = rms[2];
}

void TTEffAnalyzer::fill(const reco::CaloTau& tau,unsigned int i) {
  fillLV(tau.p4());
} 

void TTEffAnalyzer::fill(const LorentzVector& tau,unsigned int i) {
using namespace reco;

  MCMatch = 0;
  if(mcTaus.isValid()){
    MCTauE = mcTaus->at(i).energy();
    MCTauEt = mcTaus->at(i).Et();
    MCTauEta = mcTaus->at(i).Eta();
    MCTauPhi = mcTaus->at(i).Phi();
  }

  
  PFInvPt = 0.;
  PFIso = 0;
  PFTauMatch = 0;
  if(PFTaus.isValid()){
    for(unsigned int k = 0 ; k < PFTaus->size(); k++){
      if( ROOT::Math::VectorUtil::DeltaR(PFTaus->at(k).p4(),mcTaus->at(i)) < MCMatchingCone ){

	PFTauMatch = 1;	
	const PFTauRef thisTauRef(PFTaus,k);
	if(thePFTauDiscriminatorByIsolation.isValid()){
	  const PFTauDiscriminator & ds = *(thePFTauDiscriminatorByIsolation.product());
	  PFIso = ds[thisTauRef];
	}

	if(thisTauRef->leadPFChargedHadrCand().isNonnull()) PFInvPt = 1./thisTauRef->leadPFChargedHadrCand()->pt();
	// Fill common variables
	fillLV(PFTaus->at(k).p4());

	// Fill #signal tracks, and PtSum in isolation annulus 
	PFProng  = PFTaus->at(k).signalPFChargedHadrCands().size(); // check config file
	PFIsoSum = PFTaus->at(k).isolationPFChargedHadrCandsPtSum();
	PFEnergy = PFTaus->at(k).energy();
	
	std::vector<double> rms;
	clusterShape(PFTaus->at(k), rms);
	PFClusterEtaRMS = rms[0];
	PFClusterPhiRMS = rms[1];
	PFClusterDrRMS = rms[2];
	
        break;
      }
    }
  }

}

// ------------ method called once each job just before starting event loop  ------------
void 
TTEffAnalyzer::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
TTEffAnalyzer::endJob() {
  //std::cout <<  "In endJob" << std::endl;
  _TTEffFile->Write();
  //std::cout << "After write" << std::endl; 
  _TTEffFile->Close();
  //std::cout << "After close" << std::endl;
  //delete _TTEffTree;
  //std::cout << "After delete" << std::endl;
}

//define this as a plug-in
//DEFINE_FWK_MODULE(TTEffAnalyzer);


std::vector<double> 
TTEffAnalyzer::clusterSeparation(const reco::PFCandidateRefVector& isol_cands,const reco::PFCandidateRefVector& signal_cands)
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



struct RecHitPtComparator {
  bool operator()(const math::XYZTLorentzVector v1, const math::XYZTLorentzVector v2) const {
    return v1.pt() > v2.pt();
  }
};

void TTEffAnalyzer::clusterShape(const reco::PFTau& tau, std::vector<double>& rms) const {
  // Get PFCandidates
  math::XYZTLorentzVectorCollection clusters;
  getPFClusters(tau.signalPFCands(), clusters);
  getPFClusters(tau.isolationPFCands(), clusters);

  std::sort(clusters.begin(), clusters.end(), RecHitPtComparator());

  clusterShape(clusters, rms);
}

void TTEffAnalyzer::clusterShape(const math::XYZTLorentzVectorCollection& clusters, std::vector<double>& rms) const {
  double eta_rms =0;
  double phi_rms =0;
  double dr_rms = 0;
  double sumpt = 0;

  if(clusters.size() > 0) {
    math::XYZTLorentzVector direction(0,0,0,0);
    for(math::XYZTLorentzVectorCollection::const_iterator c = clusters.begin(); c!=clusters.end(); ++c) {
      direction += *c;
    }

    for(math::XYZTLorentzVectorCollection::const_iterator c = clusters.begin(); c!=clusters.end(); ++c) {
      eta_rms += c->pt() * pow(c->eta() - direction.eta(), 2);
      phi_rms += c->pt() * pow(ROOT::Math::VectorUtil::DeltaPhi(*c,direction), 2);
      dr_rms  += c->pt() * pow(ROOT::Math::VectorUtil::DeltaR(*c,direction),   2);
      sumpt   += c->pt();			   
    }
  }
  else {
    eta_rms=0.;
    phi_rms=0.;
    dr_rms =0.;
    sumpt=1.;
  }

  rms.push_back(eta_rms/sumpt);
  rms.push_back(phi_rms/sumpt);
  rms.push_back(dr_rms/sumpt);
}

void TTEffAnalyzer::getPFClusters(const PFCandidateRefVector& pfCands, math::XYZTLorentzVectorCollection& clusters) const {
  // Against double counting of PFClusters
  // See also RecoTauTag/RecoTau/src/PFRecoTauAlgorithm.cc
  std::vector<math::XYZPoint> hcalPosV;
  std::vector<math::XYZPoint> ecalPosV;
  
  for(unsigned int i=0; i<pfCands.size(); ++i) {
    const reco::PFCandidate::ElementsInBlocks& el = pfCands[i]->elementsInBlocks();
    for(reco::PFCandidate::ElementsInBlocks::const_iterator iter = el.begin(); iter != el.end(); ++iter) {
      const reco::PFBlock& block = *(iter->first);
      unsigned int block_index = iter->second;

      const edm::OwnVector<reco::PFBlockElement>& elements = block.elements();
      assert(block_index < elements.size());

      const reco::PFBlockElement& element = elements[block_index];

      //element.Dump();
      //PFClusterRef foo = element.clusterRef();
      //std::cout << " Block index " << block_index << " type " << element.type() << " is available " << foo.isAvailable() << " is null " << foo.isNull() << std::endl;

      // If referenced cluster is not available, ignore it
      // It seems that the PFBlockElements without cluster ref have
      // the type reco::PFBlockElement::TRACK, which of course
      // shouldn't be used. So this seems to be more or less correct.
      if(!element.clusterRef().isAvailable())
        continue;

      math::XYZPoint cluster_pos = element.clusterRef()->position();
      double energy = element.clusterRef()->energy();

      if(element.type() == reco::PFBlockElement::HCAL) {
        if(!checkPos(hcalPosV, cluster_pos)) {
          hcalPosV.push_back(cluster_pos);
          clusters.push_back(math::XYZTLorentzVector(cluster_pos.x(), cluster_pos.y(), cluster_pos.z(), energy));
        }
      }
      else if(element.type() == reco::PFBlockElement::ECAL) {
        if(!checkPos(ecalPosV, cluster_pos)) {
          ecalPosV.push_back(cluster_pos);
          clusters.push_back(math::XYZTLorentzVector(cluster_pos.x(), cluster_pos.y(), cluster_pos.z(), energy));
        }
      }
    }
  }
}


bool TTEffAnalyzer::checkPos(const std::vector<math::XYZPoint>& CalPos, const math::XYZPoint& CandPos) const {
  for (unsigned int i=0; i<CalPos.size(); i++)
    if (CalPos[i] == CandPos)
      return true;

  return false;
}
