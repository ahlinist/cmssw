#include "ElectroWeakAnalysis/TauTriggerEfficiency/interface/L1TauEfficiencyAnalyzer.h"

L1TauEfficiencyAnalyzer::L1TauEfficiencyAnalyzer(const edm::ParameterSet& iConfig) {

	L1extraTauJetSource = iConfig.getParameter<edm::InputTag>("L1extraTauJetSource");

	jetMatchingCone = iConfig.getParameter<double>("JetMatchingCone");

	nEvents		= 0;
	nSelectedEvents	= 0;
}

L1TauEfficiencyAnalyzer::~L1TauEfficiencyAnalyzer(){
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
                if(L1TauFound(iTau->p4())){

                }
          }
        }

}

void L1TauEfficiencyAnalyzer::beginJob(const edm::EventSetup& iSetup){}

void L1TauEfficiencyAnalyzer::endJob(){
        LogInfo("L1TauEfficiency") << "Events analyzed: " << nEvents << endl;
}

#include "PhysicsTools/Utilities/interface/deltaR.h"
bool L1TauEfficiencyAnalyzer::L1TauFound(const math::XYZTLorentzVector& p4){

	bool L1found = false;

        if(l1TauHandle.isValid()){
	  const L1JetParticleCollection & l1Taus = *(l1TauHandle.product());
	  L1JetParticleCollection::const_iterator iTau;
	  for(iTau = l1Taus.begin(); iTau != l1Taus.end(); ++iTau){
		double DR = deltaR(iTau->eta(),iTau->phi(),p4.eta(),p4.phi());
		if(DR < jetMatchingCone) L1found = true;
	  }
	}
	return L1found;
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(L1TauEfficiencyAnalyzer);


