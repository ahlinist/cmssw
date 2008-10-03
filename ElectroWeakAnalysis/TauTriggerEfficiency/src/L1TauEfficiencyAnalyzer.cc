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
  L1extraTauJetSource = iConfig.getParameter<edm::InputTag>("L1extraTauJetSource");
  jetMatchingCone = iConfig.getParameter<double>("JetMatchingCone");
  //rootFile_ = iConfig.getParameter<std::string>("outputFileName");
  nEvents = 0; nSelectedEvents = 0;

  l1tree = trigtree;

  // Setup branches
  l1tree->Branch("L1JetPt", &jetPt, "L1JetPt/F");
  l1tree->Branch("L1JetEta", &jetEta, "L1JetEta/F");
  l1tree->Branch("L1JetPhi", &jetPhi, "L1JetPhi/F");
  l1tree->Branch("hasMatchedL1Jet", &hasL1Jet, "hasMatchedL1Jet/I");
}

void L1TauEfficiencyAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup ){
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
		if(L1TauFound(iTau->p4())){

		}
	  }
	}

        Handle<PFTauCollection> pfTauHandle;
        try{
          iEvent.getByLabel("IdentifiedTaus",pfTauHandle);
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

            if(L1TauFound(iTau->p4())){
            }
            //l1tree->Fill();
          }
        }
}

void L1TauEfficiencyAnalyzer::beginEvent(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  iEvent.getByLabel(L1extraTauJetSource, l1TauHandle);

  jetPt = 0.0;
  jetEta = 0.0;
  jetPhi = 0.0;
  hasL1Jet = 0;
}

void L1TauEfficiencyAnalyzer::analyzeTau(const reco::PFTau& tau, const edm::EventSetup& iSetup) {
  if(l1TauHandle.isValid()) {  
    const L1JetParticleCollection & l1Taus = *(l1TauHandle.product());
    L1JetParticleCollection::const_iterator iTau;

    float minDR = 99999999.;
    for(iTau = l1Taus.begin(); iTau != l1Taus.end(); ++iTau){
      double DR = deltaR(iTau->eta(), iTau->phi(), tau.eta(), tau.phi());
      if(DR < jetMatchingCone) {
        minDR = DR;
        jetPt = iTau->pt();
        jetEta = iTau->eta();
        jetPhi = iTau->phi();
        hasL1Jet = 1;
      }
    }
  }
} 

void L1TauEfficiencyAnalyzer::beginJob(const edm::EventSetup& iSetup){}

void L1TauEfficiencyAnalyzer::endJob(){
        LogInfo("L1TauEfficiency") << "Events analyzed: " << nEvents << endl;
        //l1file->Write();
}

bool L1TauEfficiencyAnalyzer::L1TauFound(const math::XYZTLorentzVector& p4){

	bool L1found = false;

        if(l1TauHandle.isValid()){
	  const L1JetParticleCollection & l1Taus = *(l1TauHandle.product());
	  L1JetParticleCollection::const_iterator iTau;

          float minDR = 99999999.;
	  for(iTau = l1Taus.begin(); iTau != l1Taus.end(); ++iTau){
		double DR = deltaR(iTau->eta(),iTau->phi(),p4.eta(),p4.phi());
		if(DR < jetMatchingCone) {
                  minDR = DR;
                  jetPt = iTau->pt();
                  jetEta = iTau->eta();
                  jetPhi = iTau->phi();
                  hasL1Jet = 1;

                  L1found = true;
                }
	  }
	}
	return L1found;
}

#include "FWCore/Framework/interface/MakerMacros.h"
//DEFINE_FWK_MODULE(L1TauEfficiencyAnalyzer);


