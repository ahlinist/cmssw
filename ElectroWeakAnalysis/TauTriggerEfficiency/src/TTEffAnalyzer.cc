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
// $Id: TTEffAnalyzer.cc,v 1.71 2012/09/24 10:50:33 slehti Exp $
//
//

#include "ElectroWeakAnalysis/TauTriggerEfficiency/interface/TTEffAnalyzer.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "Math/GenVector/VectorUtil.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlock.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlockElement.h"
#include "DataFormats/ParticleFlowReco/interface/PFCluster.h"

#include "DataFormats/Common/interface/TriggerResults.h"

#include "DataFormats/Common/interface/MergeableCounter.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

using namespace std;

TTEffAnalyzer::TTEffAnalyzer(const edm::ParameterSet& iConfig):
  DoOfflineVariablesOnly_(iConfig.getParameter<bool>("DoOfflineVariablesOnly")),
  DoMCTauEfficiency_(iConfig.getParameter<bool>("DoMCTauEfficiency")),
  HLTResultsSource(iConfig.getParameter<edm::InputTag>("HltResults")),
  PFTaus_(iConfig.getParameter<edm::InputTag>("LoopingOver")),
  MCTaus_(iConfig.getParameter<edm::InputTag>("MCTauCollection")),
  MCParticles_(iConfig.getParameter<edm::InputTag>("GenParticleCollection")),
  pileupSummaryInfoSrc_(iConfig.getParameter<edm::InputTag>("PileupSummaryInfoSource")),
  offlinePrimaryVertexSrc_(iConfig.getParameter<edm::InputTag>("offlineVertexSrc")),
  hltPrimaryVertexSrc_(iConfig.getParameter<edm::InputTag>("hltVertexSrc")),
  PFTauIso_(iConfig.getParameter<std::string>("PFTauIsoDiscriminator")),
  PFTauMuonRej_(iConfig.getParameter<std::string>("PFTauMuonRejectionDiscriminator")),
  PFTauElectronRej_(iConfig.getParameter<std::string>("PFTauElectronRejectionDiscriminator")),
  rootFile_(iConfig.getParameter<std::string>("outputFileName")),
  PFTauDiscriminators_(iConfig.getParameter<std::vector<std::string> >("PFTauDiscriminators")),
  Counters_(iConfig.getParameter<std::vector<edm::InputTag> >("Counters")),
  MCMatchingCone(iConfig.getParameter<double>("MCMatchingCone"))
{
  // File setup
  _TTEffFile = TFile::Open(rootFile_.c_str(), "RECREATE");
  //_TTEffFile = TFile::Open("test.root", "RECREATE");
  // Tree setup
  _TTEffTree = new TTree("TTEffTree", "Tau Trigger Efficiency Tree");

  //reset vars
  b_event = 0;
  b_run = 0;
  b_lumi = 0;
  PFPt = 0.;
  PFInvPt = 0.;
  PFSignalSumPt = 0;
  PFEt = 0.;
  PFEta = -999.;
  PFPhi = -999.;
  PFProng = 0.;
  PFIso = 0;
  PFIsoSum = 0;
  PFIsoNTrks = 0;
  PFIsoTrkNHits = 0;
  PFIsoTrkChi2 = 1000.;
  PFIsoTrkPt = 0;
  PFEnergy = 0;
  PFMuonMatch = 0;
  PFTauMatch = 0;
  PFElectronMatch = 0;
  MCMatch = 0;
  MCTauEt = -1.;
  MCTauE = -1.;
  MCTauEta = -999.;
  MCTauPhi = -999.;

  pfJetChargedEmEnergy = 0.;
  pfJetChargedEmEnergyFraction = 0.;
  pfJetChargedHadronEnergy = 0.;
  pfJetChargedHadronEnergyFraction = 0.;
  pfJetNeutralEmEnergy = 0.;
  pfJetNeutralEmEnergyFraction = 0.;
  pfJetNeutralHadronEnergy = 0.;
  pfJetNeutralHadronEnergyFraction = 0.;

  _TTEffTree->Branch("event", &b_event);
  _TTEffTree->Branch("run", &b_run);
  _TTEffTree->Branch("lumi", &b_lumi);

  _TTEffTree->Branch("PFTauPt", &PFPt);
  _TTEffTree->Branch("PFTauInvPt", &PFInvPt);
  _TTEffTree->Branch("PFTauEt",&PFEt);
  _TTEffTree->Branch("PFTauEta", &PFEta);
  _TTEffTree->Branch("PFTauPhi", &PFPhi);
  _TTEffTree->Branch("PFTauProng", &PFProng);
  _TTEffTree->Branch("PFTauSignalCandsPtSum", &PFSignalSumPt);
  _TTEffTree->Branch("PFTauIso", &PFIso);
  _TTEffTree->Branch("PFTauIsoSum", &PFIsoSum);
  _TTEffTree->Branch("PFIsoNTrks", &PFIsoNTrks);
  _TTEffTree->Branch("PFIsoTrkNHits", &PFIsoTrkNHits);
  _TTEffTree->Branch("PFIsoTrkChi2", &PFIsoTrkChi2);
  _TTEffTree->Branch("PFIsoTrkPt", &PFIsoTrkPt);
  _TTEffTree->Branch("PFTauEnergy", &PFEnergy);
  _TTEffTree->Branch("PFClusterEtaRMS", &PFClusterEtaRMS);
  _TTEffTree->Branch("PFClusterPhiRMS", &PFClusterPhiRMS);
  _TTEffTree->Branch("PFClusterDrRMS", &PFClusterDrRMS);
  _TTEffTree->Branch("PFTauMatch", &PFTauMatch);
  _TTEffTree->Branch("PFMuonMatch", &PFMuonMatch);  
  _TTEffTree->Branch("PFElectronMatch", &PFElectronMatch);

  discriminators.resize(PFTauDiscriminators_.size());
  for(size_t i = 0; i < PFTauDiscriminators_.size(); ++i) {
    _TTEffTree->Branch(PFTauDiscriminators_[i].c_str(), &discriminators[i]);
  }

  _TTEffTree->Branch("PFJetChargedEmEnergy",             &pfJetChargedEmEnergy);
  _TTEffTree->Branch("PFJetChargedEmEnergyFraction",     &pfJetChargedEmEnergyFraction);
  _TTEffTree->Branch("PFJetChargedHadronEnergy",         &pfJetChargedHadronEnergy);
  _TTEffTree->Branch("PFJetChargedHadronEnergyFraction", &pfJetChargedHadronEnergyFraction);
  _TTEffTree->Branch("PFJetNeutralEmEnergy",             &pfJetNeutralEmEnergy);
  _TTEffTree->Branch("PFJetNeutralEmEnergyFraction",     &pfJetNeutralEmEnergyFraction);
  _TTEffTree->Branch("PFJetNeutralHadronEnergy",         &pfJetNeutralHadronEnergy);
  _TTEffTree->Branch("PFJetNeutralHadronEnergyFraction", &pfJetNeutralHadronEnergyFraction);

  _TTEffTree->Branch("MCMatch", &MCMatch);
  _TTEffTree->Branch("MCTauEt", &MCTauEt);
  _TTEffTree->Branch("MCTauE", &MCTauE);
  _TTEffTree->Branch("MCTauEta", &MCTauEta);
  _TTEffTree->Branch("MCTauPhi", &MCTauPhi);

  _TTEffTree->Branch("MCNPU", &nPU);

  std::vector<std::string> triggerNames = iConfig.getParameter<std::vector<std::string> >("HltPaths");
  hltBits.reserve(triggerNames.size());
  for(size_t i=0; i<triggerNames.size(); ++i)
    hltBits.push_back(TriggerBit(triggerNames[i]));
  for(size_t i=0; i<hltBits.size(); ++i)
    _TTEffTree->Branch(hltBits[i].name.c_str(), &(hltBits[i].value));

  if(!DoOfflineVariablesOnly_){
    _L1analyzer = new L1TauEfficiencyAnalyzer();
    _L2analyzer = new L2TauEfficiencyAnalyzer();
    _L25and3analyzer  = new L25and3TauEfficiencyAnalyzer();
    _PFMHTanalyzer = new PFMHTEfficiencyAnalyzer();
    _HLTJetAnalyzer = new HLTJetEfficiencyAnalyzer();
    _L1analyzer->Setup(iConfig,_TTEffTree);
    _L2analyzer->Setup(iConfig,_TTEffTree);
    _L25and3analyzer->Setup(iConfig,_TTEffTree);
    _PFMHTanalyzer->Setup(iConfig,_TTEffTree); 
    _HLTJetAnalyzer->Setup(iConfig,_TTEffTree);
  }
  _METanalyzer = new METEfficiencyAnalyzer();
  _MuonAnalyzer = new MuonAnalyzer();
  _METanalyzer->Setup(iConfig,_TTEffTree);
  _MuonAnalyzer->Setup(iConfig,_TTEffTree);

  h_counters = new TH1F("Counters","",Counters_.size(),0,Counters_.size());
  h_counters->SetDirectory(_TTEffFile);
}

TTEffAnalyzer::~TTEffAnalyzer()
{
}


//
// member functions
//

// ------------ method called to for each event  ------------
void
TTEffAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   b_event = iEvent.id().event();
   b_run = iEvent.run();
   b_lumi = iEvent.luminosityBlock();

//   nPU = 0;
   /*
     // Old, for Summer11 MC
   edm::Handle<std::vector<PileupSummaryInfo> > hpileup;
   iEvent.getByLabel(pileupSummaryInfoSrc_, hpileup);
   if(hpileup.isValid()) {
     int npv = 0;
     for(std::vector<PileupSummaryInfo>::const_iterator iPV = hpileup->begin(); iPV != hpileup->end(); ++iPV) {
       npv += iPV->getPU_NumInteractions();
     }
     nPU = npv/3.;
   }
   */

   nGoodOfflinePV_ = 0;
   edm::Handle<edm::View<reco::Vertex> > hoffvertex;
   if(iEvent.getByLabel(offlinePrimaryVertexSrc_, hoffvertex)){
     nGoodOfflinePV_ = hoffvertex->size();
   }

   // See https://twiki.cern.ch/twiki/bin/view/CMS/PileupMCReweightingUtilities
   edm::Handle<std::vector<PileupSummaryInfo> >  hpu;
   iEvent.getByLabel(pileupSummaryInfoSrc_, hpu);
   if(hpu.isValid()) {
   	float n0 = -1;
   	//float nm1 = -1;
   	//float np1 = -1;
   	for(std::vector<PileupSummaryInfo>::const_iterator iPV = hpu->begin(); iPV != hpu->end(); ++iPV) {
   	   //if(iPV->getBunchCrossing() == -1)
   	   //  nm1 = iPV->getTrueNumInteractions();
   	   //else 
	   if(iPV->getBunchCrossing() == 0)
   	     n0 = iPV->getTrueNumInteractions();
   	   //else if(iPV->getBunchCrossing() == 1)
   	     //np1 = iPV->getTrueNumInteractions();
   	}
   	if(n0 < 0) throw cms::Exception("Assert") << "VertexWeight: Didn't find the number of interactions for BX 0" << std::endl;;

   	nPU = n0;
   }

   primaryVertexIsValid_ = false;
   edm::Handle<edm::View<reco::Vertex> > hvertices; 
   if(iEvent.getByLabel(hltPrimaryVertexSrc_, hvertices)){
     primaryVertexIsValid_ = hvertices->size() > 0;
   }

   edm::Handle<edm::View<reco::Candidate> > genericTaus;

   if( DoMCTauEfficiency_ ) { // this is to calculate efficiencies per MC tau candidate
     iEvent.getByLabel(MCTaus_, mcTaus);
     iEvent.getByLabel(PFTaus_, PFTaus);
     if(!mcTaus.isValid()) {
       throw cms::Exception("Configuration") << "MCTauCollection not found for label " << MCTaus_ << std::endl;
     }

     loop(iEvent,iSetup, *mcTaus);
   } 
   else {
     if(iEvent.getByLabel(PFTaus_, PFTaus)) {
       iEvent.getByLabel(MCTaus_, mcTaus);
       iEvent.getByLabel(MCParticles_, mcParticles);
       loop(iEvent,iSetup,*PFTaus);
     }
     else if(iEvent.getByLabel(PFTaus_, genericTaus)) {
       loop(iEvent,iSetup, *genericTaus);
     }
     else {
       throw cms::Exception("Configuration") << "LoopingOver collection not found for label " << PFTaus_ << std::endl;
     }
   }
   
   // For electron lorentzvectors, add similar clauses
}

template <class T>
void TTEffAnalyzer::loop(const edm::Event& iEvent,const edm::EventSetup& iSetup, const T& collection) {
  for(typename T::const_iterator particle = collection.begin(); particle != collection.end(); ++particle) {
    // Fill common variables
    fill(*particle);
    if(!DoOfflineVariablesOnly_){
      fillHLTinfo(iEvent);
    }

    // Call individual analyzers
    if(!DoOfflineVariablesOnly_){
      _L1analyzer->fill(iEvent,iSetup, *particle);
      _L2analyzer->fill(iEvent,iSetup, *particle);
      _L25and3analyzer->fill(iEvent, *particle);
      _HLTJetAnalyzer->fill(iEvent,iSetup);
      _PFMHTanalyzer->fill(iEvent,iSetup);
    }
    _METanalyzer->fill(iEvent,iSetup);
    _MuonAnalyzer->fill(iEvent,iSetup, *particle);

    // Finally, fill the entry to tree
    _TTEffTree->Fill();
  }
}

void TTEffAnalyzer::fillHLTinfo(const edm::Event& iEvent){

  // Store HLT trigger bits
  edm::Handle<edm::TriggerResults> hltresults;
  iEvent.getByLabel(HLTResultsSource,hltresults);
  if(!hltresults.isValid()) {
    edm::LogWarning("TTEffAnalyzer") << "%L1TauEffAnalyzer -- No HltResults found! " << std::endl;
    return;
  }
  else {
    //    size_t ntrigs = hltresults->size();
    const edm::TriggerNames& triggerNames = iEvent.triggerNames(*hltresults);

    for(size_t i=0; i<hltBits.size(); ++i) {
      size_t itrig = triggerNames.triggerIndex(hltBits[i].name);
      if(itrig == triggerNames.size()) {
        hltBits[i].value = false;
        continue;
      }
      hltBits[i].value = hltresults->accept(itrig);
    }
  }
}

void TTEffAnalyzer::fillLV(const LorentzVector& tau) {
  PFPt = tau.Pt();
  PFEt = tau.Et();
  PFEta = tau.Eta();
  PFPhi = tau.Phi();
}

void TTEffAnalyzer::fill(const reco::Candidate& tau) {
  fillLV(tau.p4());
}

void
TTEffAnalyzer::fill(const pat::Tau& tau) {
using namespace reco;
  PFInvPt = 0.;
  PFIso = 0;

  pfJetChargedEmEnergy = 0.;
  pfJetChargedEmEnergyFraction = 0.;
  pfJetChargedHadronEnergy = 0.;
  pfJetChargedHadronEnergyFraction = 0.;
  pfJetNeutralEmEnergy = 0.;
  pfJetNeutralEmEnergyFraction = 0.;
  pfJetNeutralHadronEnergy = 0.;
  pfJetNeutralHadronEnergyFraction = 0.;


  PFIso = tau.tauID(PFTauIso_);
  PFMuonMatch = tau.tauID(PFTauMuonRej_) > 0.5;
  PFElectronMatch = tau.tauID(PFTauElectronRej_) > 0.5;

  for(size_t iDiscr = 0; iDiscr < PFTauDiscriminators_.size(); ++iDiscr) {
	discriminators[iDiscr] = tau.tauID(PFTauDiscriminators_[iDiscr]);
  }

  MCMatch = 0;
  if(mcTaus.isValid()){
    for(unsigned int k = 0 ; k < mcTaus->size(); k++){
      //Mike B: Changed this to ROOT ::Math since it was confused which deltaR to use
      if( ROOT::Math::VectorUtil::DeltaR(tau.p4(),mcTaus->at(k)) < MCMatchingCone ){ // match within 0.2 cone
         MCMatch = 1;
	 MCTauE = mcTaus->at(k).energy();
	 MCTauEt = mcTaus->at(k).Et();
	 MCTauEta = mcTaus->at(k).Eta();
	 MCTauPhi = mcTaus->at(k).Phi();
        break;
      }
    }
  }
  if(mcParticles.isValid()){
    for(unsigned int k = 0 ; k < mcParticles->size(); k++){
      if(abs(mcParticles->at(k).pdgId()) != 11 && abs(mcParticles->at(k).pdgId()) != 13) continue;
      if( ROOT::Math::VectorUtil::DeltaR(tau.p4(),mcParticles->at(k).p4()) < MCMatchingCone ) {
	if(abs(mcParticles->at(k).pdgId()) == 11 ) MCMatch = 11;
	if(abs(mcParticles->at(k).pdgId()) == 13 ) MCMatch = 13;
      }
    }
  }
  if(tau.leadPFChargedHadrCand().isNonnull()) PFInvPt = 1./tau.leadPFChargedHadrCand()->pt();
  // Fill common variables
  fillLV(tau.p4());

  // Fill #signal tracks, and PtSum in isolation annulus 
  PFProng  = tau.signalPFChargedHadrCands().size(); // check config file
  PFIsoSum = tau.isolationPFChargedHadrCandsPtSum();
  PFIsoNTrks = tau.isolationPFChargedHadrCands().size();
  PFEnergy = tau.energy();
  
  // get the parameters of the tracks in the isolation region...
  
  const PFCandidateRefVector& theIsoCands = tau.isolationPFChargedHadrCands();
  for(PFCandidateRefVector::const_iterator vIt = theIsoCands.begin(); vIt != theIsoCands.end(); ++vIt){
    const TrackRef theTrkRef = (*vIt)->trackRef();
    if(theTrkRef.isNonnull()){
      PFIsoTrkChi2 = theTrkRef->chi2();
      PFIsoTrkNHits = theTrkRef->numberOfValidHits();
      PFIsoTrkPt = theTrkRef->pt();
    }
  }
  
  PFSignalSumPt = 0;
  const PFCandidateRefVector& theSignalCands = tau.signalPFChargedHadrCands();
  for(PFCandidateRefVector::const_iterator vIt = theSignalCands.begin(); vIt != theSignalCands.end(); ++vIt){
     if((*vIt) == tau.leadPFChargedHadrCand()) continue;
     PFSignalSumPt += (*vIt)->pt();
  }

  //PFElectronMatch = PFTaus->at(i).electronPreIDDecision();?

  if(DoOfflineVariablesOnly_) return;

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

  // HCAL+ECAL energies and fractions
////  const reco::PFTauTagInfoRef& tauTagInfo = thisTauRef->pfTauTagInfoRef();
////  const reco::PFJetRef& tauJet = tauTagInfo->pfjetRef();
  const reco::PFJetRef& tauJet = tau.pfJetRef();
  pfJetChargedEmEnergy             = tauJet->chargedEmEnergy();
  pfJetChargedEmEnergyFraction     = tauJet->chargedEmEnergyFraction();
  pfJetChargedHadronEnergy         = tauJet->chargedHadronEnergy();
  pfJetChargedHadronEnergyFraction = tauJet->chargedHadronEnergyFraction();
  pfJetNeutralEmEnergy             = tauJet->neutralEmEnergy();
  pfJetNeutralEmEnergyFraction     = tauJet->neutralEmEnergyFraction();
  pfJetNeutralHadronEnergy         = tauJet->neutralHadronEnergy();
  pfJetNeutralHadronEnergyFraction = tauJet->neutralHadronEnergyFraction();


}

void TTEffAnalyzer::fill(const LorentzVector& mcTau) {
using namespace reco;

  MCMatch = 0;
  MCTauE = mcTau.energy();
  MCTauEt = mcTau.Et();
  MCTauEta = mcTau.Eta();
  MCTauPhi = mcTau.Phi();

  
  PFInvPt = 0.;
  PFIso = 0;
  PFTauMatch = 0;
  if(PFTaus.isValid()){
    for(unsigned int k = 0 ; k < PFTaus->size(); k++){
      if( ROOT::Math::VectorUtil::DeltaR(PFTaus->at(k).p4(),mcTau) < MCMatchingCone ){

	PFTauMatch = 1;
        const pat::Tau& tau = PFTaus->at(k);
        PFIso = tau.tauID(PFTauIso_);
        PFMuonMatch = tau.tauID(PFTauMuonRej_) > 0.5;
        PFElectronMatch = tau.tauID(PFTauElectronRej_) > 0.5;

	if(tau.leadPFChargedHadrCand().isNonnull()) PFInvPt = 1./tau.leadPFChargedHadrCand()->pt();
	// Fill common variables
	fillLV(tau.p4());

	// Fill #signal tracks, and PtSum in isolation annulus 
	PFProng  = tau.signalPFChargedHadrCands().size(); // check config file
	PFIsoSum = tau.isolationPFChargedHadrCandsPtSum();
	PFEnergy = tau.energy();
	
	std::vector<double> rms;
	clusterShape(tau, rms);
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
TTEffAnalyzer::beginJob()
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

void TTEffAnalyzer::endLuminosityBlock(const edm::LuminosityBlock & lumi, const edm::EventSetup & setup) {
   	// Counters
	edm::Handle<edm::MergeableCounter> count;

	for(size_t i = 0; i < Counters_.size(); ++i){
		lumi.getByLabel(Counters_[i], count);
		int value = count->value;
		if(h_counters->GetEntries()) value += h_counters->GetBinContent(i+1);
		h_counters->SetBinContent(i+1,value);
		h_counters->GetXaxis()->SetBinLabel(i+1,(Counters_[i].label()).c_str());
	}
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

void TTEffAnalyzer::clusterShape(const pat::Tau& tau, std::vector<double>& rms) const {
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
