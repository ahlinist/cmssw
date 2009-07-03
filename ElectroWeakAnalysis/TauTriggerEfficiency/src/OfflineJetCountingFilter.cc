// -*- C++ -*-
//
// Package:    OfflineJetCountingFilter
// Class:      OfflineJetCountingFilter
// 
/**\class OfflineJetCountingFilter OfflineJetCountingFilter.cc ElectroWeakAnalysis/OfflineJetCountingFilter/src/OfflineJetCountingFilter.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Matti Kortelainen
//         Created:  Thu Sep 18 11:07:16 CEST 2008
// $Id: OfflineTauIDFilter.cc,v 1.5 2008/11/20 14:08:59 slehti Exp $
// Modified from OfflineTauIDFilter to OfflineJetCountingFilter 03072009/S.Lehti
//
//

#include "ElectroWeakAnalysis/TauTriggerEfficiency/interface/OfflineJetCountingFilter.h"
#include "DataFormats/JetReco/interface/CaloJet.h"

using namespace edm;

OfflineJetCountingFilter::OfflineJetCountingFilter(const edm::ParameterSet& iConfig) {

	jetSource = iConfig.getParameter<InputTag>("JetSource");
	jetEtMin  = iConfig.getParameter<double>("RawJetEtMin");
	jetEtaMax = iConfig.getParameter<double>("RawJetEtaMax");
	nJetsMax  = iConfig.getParameter<int>("MaxNumberOfJets");

  	nEvents       = 0;
  	nPassedEvents = 0;
}


OfflineJetCountingFilter::~OfflineJetCountingFilter(){}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool OfflineJetCountingFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  	Handle<reco::CaloJetCollection> theJetHandle;
  	iEvent.getByLabel(jetSource,theJetHandle);

  	bool passed = false;
  	++nEvents;

	int nJets = 0;
  	if(theJetHandle.isValid()){
		const reco::CaloJetCollection & calojets = *(theJetHandle.product());		
		for(unsigned int i = 0; i < calojets.size(); ++i){
			if(calojets[i].pt() < jetEtMin) continue;
			if(fabs(calojets[i].pt()) > jetEtaMax) continue;
			nJets++;
		}
	}
	if(nJets <= nJetsMax) {
		passed = true;
		++nPassedEvents;
	}

  	return passed;
}

// ------------ method called once each job just before starting event loop  ------------
void OfflineJetCountingFilter::beginJob(const edm::EventSetup&){}

// ------------ method called once each job just after ending the event loop  ------------
void OfflineJetCountingFilter::endJob() {
  	LogInfo("OfflineJetCountingFilter") << "Total " << nEvents << " events, "
                                            << "from which " << nPassedEvents 
                                            << " events passed the filter" << std::endl; 
}

//define this as a plug-in
DEFINE_FWK_MODULE(OfflineJetCountingFilter);
