
#include "ElectroWeakAnalysis/TauTriggerEfficiency/interface/OfflineTauIDProducer.h"

OfflineTauIDProducer::OfflineTauIDProducer(const edm::ParameterSet& iConfig) {
	produces< PFTauCollection >().setBranchAlias("identifiedPfTaus");
        produces< CaloTauCollection >().setBranchAlias("identifiedCaloTaus");

	tauProducer                 = iConfig.getParameter<InputTag>("PFTauProducer");

	jetPtMin		    = iConfig.getParameter<double>("JetPtMin");
	matchingConeSize            = iConfig.getParameter<double>("MatchingCone");
        signalConeSizeFormula       = iConfig.getParameter<string>("TrackerSignalConeSizeFormula"); // E, ET recognised as variables
	signalConeSizeMin           = iConfig.getParameter<double>("TrackerSignalConeSize_min");
	signalConeSizeMax           = iConfig.getParameter<double>("TrackerSignalConeSize_max");
	isolationConeSize           = iConfig.getParameter<double>("IsolationCone");
	ptLeadingTrackMin           = iConfig.getParameter<double>("LeadTrack_minPt");
	ptOtherTracksMin            = iConfig.getParameter<double>("Track_minPt");
	metric		            = iConfig.getParameter<string>("Metric");// can be DR,angle,area
	isolationAnnulus_Tracksmaxn = iConfig.getParameter<int>("Isolation_Tracksmaxn");

	nEvents		= 0;
	nSelectedEvents	= 0;
}

OfflineTauIDProducer::~OfflineTauIDProducer(){
}

void OfflineTauIDProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup ){

	bool select = false;
	nEvents++;

	PFTauCollection* pfTaus = new PFTauCollection;

        Handle<PFTauCollection> thePFTauHandle;
        try{
          iEvent.getByLabel(tauProducer,thePFTauHandle);
        }catch(...) {;}

        if(thePFTauHandle.isValid()){
          const PFTauCollection & taus = *(thePFTauHandle.product());

          PFTauCollection::const_iterator iTau;
          for(iTau = taus.begin(); iTau != taus.end(); iTau++){
		if(iTau->pt() < jetPtMin) continue;
	        if(!iTau->leadTrack()) continue;
                if(tauTag(const_cast<reco::PFTau&>(*iTau))) {
			select = true;
			pfTaus->push_back(*iTau); 
		}
          }
        }

	CaloTauCollection* caloTaus = new CaloTauCollection;

       	Handle<CaloTauCollection> theCaloTauHandle;
       	try{
       	  iEvent.getByLabel("caloRecoTauProducer",theCaloTauHandle);
       	}catch(...) {;}

       	if(theCaloTauHandle.isValid()){
       	  const CaloTauCollection & taus = *(theCaloTauHandle.product());

       	  CaloTauCollection::const_iterator iTau;
       	  for(iTau = taus.begin(); iTau != taus.end(); iTau++){
		if(iTau->pt() < jetPtMin) continue;
       	        if(!iTau->leadTrack()) continue;
       	        if(tauTag(const_cast<reco::CaloTau&>(*iTau))) {
			select = true;
                        caloTaus->push_back(*iTau);
                }
      	  }
	}

	if(select) nSelectedEvents++;
        LogDebug("OfflineTauIDProducer") << "taus " << pfTaus->size() << " " << caloTaus->size() << endl;
        auto_ptr< PFTauCollection > pf(pfTaus);
        iEvent.put(pf);

        auto_ptr< CaloTauCollection > calo(caloTaus);
        iEvent.put(calo);
}

bool OfflineTauIDProducer::tauTag(reco::CaloTau& tau){

        CaloTauElementsOperators theCaloTauElementsOperators(tau);

        TFormula formula = TauTagTools::computeConeSizeTFormula(signalConeSizeFormula, "OfflineTauIDProcuder::tauTag(reco::CaloTau)");
        double signalConeSize = theCaloTauElementsOperators.computeConeSize(formula, signalConeSizeMin, signalConeSizeMax);

        LogDebug("OfflineTauIDProducer") << "CaloTau computed signal cone " << signalConeSize;

        double discriminator = theCaloTauElementsOperators.discriminatorByIsolTracksN(
                                metric,
                                matchingConeSize,
                                ptLeadingTrackMin,
                                ptOtherTracksMin,
                                metric,
                                signalConeSize,
                                metric,
                                isolationConeSize,
                                isolationAnnulus_Tracksmaxn);

	bool tagged = false;
	if(discriminator != 0) tagged = true;
	return tagged;
}

bool OfflineTauIDProducer::tauTag(reco::PFTau& tau){

	PFTauElementsOperators thePFTauElementsOperators(tau);
	
        TFormula formula = TauTagTools::computeConeSizeTFormula(signalConeSizeFormula, "OfflineTauIDProcuder::tauTag(reco::PFTau)");
        double signalConeSize = thePFTauElementsOperators.computeConeSize(formula, signalConeSizeMin, signalConeSizeMax);

        LogDebug("OfflineTauIDProducer") << "PFTau   computed signal cone " << signalConeSize;
	
        double discriminator = thePFTauElementsOperators.discriminatorByIsolTracksN(
                                metric,
                                matchingConeSize,
                                ptLeadingTrackMin,
                                ptOtherTracksMin,
                                metric,
                                signalConeSize,
                                metric,
                                isolationConeSize,
                                isolationAnnulus_Tracksmaxn);
        bool tagged = false;
        if(discriminator != 0) tagged = true;
        return tagged;
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(OfflineTauIDProducer);
