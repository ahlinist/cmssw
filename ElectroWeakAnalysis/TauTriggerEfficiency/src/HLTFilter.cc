#include "ElectroWeakAnalysis/TauTriggerEfficiency/interface/HLTFilter.h"

HLTFilter::HLTFilter(const edm::ParameterSet& iConfig) {

//	hlNames = edm::Service<edm::service::TriggerNamesService>()->getTrigPaths();

	HLTResults   = iConfig.getParameter<InputTag>("HLTResults");
	HLTSelection = iConfig.getParameter<vector<InputTag> >("HLTSelection");

	nEvents		= 0;
	nSelectedEvents	= 0;
}

HLTFilter::~HLTFilter(){
}

bool HLTFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup ){

	bool triggerdecision = false;
	nEvents++;

	Handle<TriggerResults> hltHandle;
        iEvent.getByLabel(HLTResults,hltHandle);


        if(hltHandle.isValid()){
                //cout << "trigger table size " << hltHandle->size() << endl;

//                TriggerNames triggerNames;
//                triggerNames.init(*hltHandle);
//                vector<string> hlNames = triggerNames.triggerNames();
                edm::TriggerResults tr = *hltHandle;
                bool fromPSetRegistry;
                Service<service::TriggerNamesService> tns;
                tns->getTrigPaths(tr, hlNames, fromPSetRegistry);

                int n = 0;
                for(vector<string>::const_iterator i = hlNames.begin();
                                                   i!= hlNames.end(); i++){
                        //cout << "trigger: " << *i << " "
                        //     << hltHandle->accept(n) << endl;

                        for(vector<InputTag>::const_iterator iSelect = HLTSelection.begin();
                                                             iSelect!= HLTSelection.end(); iSelect++){
                                if(iSelect->label() == *i && hltHandle->accept(n) == 1) {
                                        triggerdecision = true;
                                        //cout << "event triggered with " << *i << endl;
                                }
                        }
                        n++;
                }
        }


	if(triggerdecision) nSelectedEvents++;
	return triggerdecision;
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(HLTFilter);
