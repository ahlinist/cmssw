#include "FastSimDataFormats/External/interface/FastL1BitInfo.h"

#include "ElectroWeakAnalysis/TauTriggerEfficiency/interface/L1TauEfficiencyAnalyzer.h"
#include "PhysicsTools/Utilities/interface/deltaR.h"

// Default constructor
L1TauEfficiencyAnalyzer::L1TauEfficiencyAnalyzer()
{}


L1TauEfficiencyAnalyzer::~L1TauEfficiencyAnalyzer(){
  /*
	cout << endl;
	cout << "Events analyzed " << nEvents << endl;
	cout << endl;
  */
}

void L1TauEfficiencyAnalyzer::Setup(const edm::ParameterSet& iConfig,TTree *trigtree)
{
  PFTauCollectionSource = iConfig.getParameter<edm::InputTag>("PFTauCollection");
  L1extraTauJetSource = iConfig.getParameter<edm::InputTag>("L1extraTauJetSource");
  L1extraCentralJetSource = iConfig.getParameter<edm::InputTag>("L1extraCentralJetSource");
  L1bitInfoSource = iConfig.getParameter<edm::InputTag>("L1bitInfoSource");
  jetMatchingCone = iConfig.getParameter<double>("L1JetMatchingCone");
  //rootFile_ = iConfig.getParameter<std::string>("outputFileName");
  nEvents = 0; nSelectedEvents = 0;

  l1tree = trigtree;

  // Setup branches
  l1tree->Branch("L1JetPt", &jetPt, "L1JetPt/F");
  l1tree->Branch("L1JetEt", &jetPt, "L1JetEt/F");
  l1tree->Branch("L1JetEta", &jetEta, "L1JetEta/F");
  l1tree->Branch("L1JetPhi", &jetPhi, "L1JetPhi/F");
  l1tree->Branch("L1TauVeto", &hasTauVeto, "L1TauVeto/B");
  l1tree->Branch("L1EmTauVeto", &hasEmTauVeto, "L1EmTauVeto/B");
  l1tree->Branch("L1HadTauVeto", &hasHadTauVeto, "L1HadTauVeto/B");
  l1tree->Branch("L1IsolationVeto", &hasIsolationVeto, "L1IsolationVeto/B");
  l1tree->Branch("L1SumEtBelowThreshold", &hasSumEtBelowThres, "L1SumEtBelowThrehold/B");
  l1tree->Branch("L1MaxEt", &hasMaxEt, "L1MaxEt/B");
  l1tree->Branch("L1Soft", &hasSoft, "L1Soft/B");
  l1tree->Branch("L1Hard", &hasHard, "L1Hard/B");
  l1tree->Branch("hasMatchedL1Jet", &hasL1Jet, "hasMatchedL1Jet/B");
  l1tree->Branch("hasMatchedL1TauJet", &hasL1TauJet, "hasMatchedL1TauJet/B");
  l1tree->Branch("hasMatchedL1CenJet", &hasL1CenJet, "hasMatchedL1CenJet/B");
}

void L1TauEfficiencyAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup ){
        Handle<L1JetParticleCollection> l1TauHandle;
	nEvents++;

	try{
          iEvent.getByLabel(L1extraTauJetSource,l1TauHandle);
	}catch(...) {;}


	Handle<CaloTauCollection> caloTauHandle;
	try{
          iEvent.getByLabel("IdentifiedTaus",caloTauHandle);
        }catch(...) {;}

	if(caloTauHandle.isValid()){
	  const CaloTauCollection & caloTaus = *(caloTauHandle.product());

	  LogDebug("L1TauEfficiency") << "calotau collection size " << caloTaus.size() << endl;

	  CaloTauCollection::const_iterator iTau;
          for(iTau = caloTaus.begin(); iTau != caloTaus.end(); ++iTau){
            //if(L1TauFound(iTau->p4())){
            //}
	  }
	}

        Handle<PFTauCollection> pfTauHandle;
        try{
          iEvent.getByLabel(PFTauCollectionSource,pfTauHandle);
        }catch(...) {;}

        if(pfTauHandle.isValid()){
          const PFTauCollection & pfTaus = *(pfTauHandle.product());

          LogDebug("L1TauEfficiency") << "pftau collection size " << pfTaus.size() << endl;

          PFTauCollection::const_iterator iTau;
          for(iTau = pfTaus.begin(); iTau != pfTaus.end(); ++iTau){
            jetPt = 0;
            jetEta = 0;
            jetPhi = 0;
            hasL1Jet = 0;

            //if(L1TauFound(iTau->p4())){
            //}
            //l1tree->Fill();
          }
        }
}

void L1TauEfficiencyAnalyzer::fill(const edm::Event& iEvent, const reco::PFTau& tau) {
	fill(iEvent,tau.p4());
}

void L1TauEfficiencyAnalyzer::fill(const edm::Event& iEvent, const reco::CaloTau& tau) {
        fill(iEvent,tau.p4());
}

void L1TauEfficiencyAnalyzer::fill(const edm::Event& iEvent, const LorentzVector& tau) {

  // Reset variables
  jetPt = 0.0;
  jetEt = 0.0;
  jetEta = 0.0;
  jetPhi = 0.0;
  hasL1Jet = 0;
  hasL1TauJet = 0;
  hasL1CenJet = 0;
  hasTauVeto = 0;
  hasEmTauVeto = 0;
  hasHadTauVeto = 0;
  hasIsolationVeto = 0;
  hasSumEtBelowThres = 0;
  hasMaxEt = 0;
  hasSoft = 0;
  hasHard = 0;

  // Get data from event 
  Handle<FastL1BitInfoCollection> bitInfos;
  iEvent.getByLabel(L1bitInfoSource, bitInfos);
  if(!bitInfos.isValid())
    return;

  Handle<L1JetParticleCollection> l1TauHandle;
  iEvent.getByLabel(L1extraTauJetSource, l1TauHandle);
  if(!l1TauHandle.isValid())
    return;

  Handle<L1JetParticleCollection> l1CentralJetHandle;
  iEvent.getByLabel(L1extraCentralJetSource, l1CentralJetHandle);
  if(!l1CentralJetHandle.isValid())
    return;

  // Process L1 triggered taus
  const L1JetParticleCollection & l1Taus = *(l1TauHandle.product());
  L1JetParticleCollection::const_iterator iJet;

  // Match for PF tau and L1extra tau
  float minDR = 99999999.;
  for(iJet = l1Taus.begin(); iJet != l1Taus.end(); ++iJet) {
    if(iJet->et() <= 5.)
      continue;

    double DR = deltaR(iJet->eta(), iJet->phi(), tau.Eta(), tau.Phi());
    if(DR < jetMatchingCone && DR < minDR) {
      minDR = DR;
      jetPt = iJet->pt();
      jetEt = iJet->et();
      jetEta = iJet->eta();
      jetPhi = iJet->phi();
      hasL1Jet = 1;
      hasL1TauJet = 1;
    }
  }

  // If not found, match PF tau and L1extra central jet
  if(!hasL1Jet) {
    for(iJet = l1CentralJetHandle->begin(); iJet != l1CentralJetHandle->end(); ++iJet) {
      if(iJet->et() <= 5.)
        continue;

      double DR = deltaR(iJet->eta(), iJet->phi(), tau.Eta(), tau.Phi());
      if(DR < jetMatchingCone && DR < minDR) {
        minDR = DR;
        jetPt = iJet->pt();
        jetEt = iJet->et();
        jetEta = iJet->eta();
        jetPhi = iJet->phi();
        hasL1Jet = 1;
        hasL1CenJet = 1;
      }
    }
  }
  
  // If match found, find the corresponding bit field
  if(hasL1Jet) {
    for(FastL1BitInfoCollection::const_iterator bitInfo = bitInfos->begin(); bitInfo != bitInfos->end(); ++bitInfo) {
      if(fabs(bitInfo->getEta() - jetEta) < 0.1 &&
         fabs(bitInfo->getPhi() - jetPhi) < 0.1) {

        hasTauVeto = bitInfo->getTauVeto() ? 1 : 0;
        hasEmTauVeto = bitInfo->getEmTauVeto() ? 1 : 0;
        hasHadTauVeto = bitInfo->getHadTauVeto() ? 1 : 0;
        hasIsolationVeto = bitInfo->getIsolationVeto() ? 1 : 0;
        hasSumEtBelowThres = bitInfo->getSumEtBelowThres() ? 1 : 0;
        hasMaxEt = bitInfo->getMaxEt() ? 1 : 0;
        hasSoft = bitInfo->getSoft() ? 1 : 0;
        hasHard = bitInfo->getHard() ? 1 : 0;
      }
    }
  }
} 

void L1TauEfficiencyAnalyzer::beginJob(const edm::EventSetup& iSetup){}

void L1TauEfficiencyAnalyzer::endJob(){
        LogInfo("L1TauEfficiency") << "Events analyzed: " << nEvents << endl;
        //l1file->Write();
}

#include "FWCore/Framework/interface/MakerMacros.h"
//DEFINE_FWK_MODULE(L1TauEfficiencyAnalyzer);


