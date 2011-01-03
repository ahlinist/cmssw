#include "ElectroWeakAnalysis/TauTriggerEfficiency/interface/HLTJetEfficiencyAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/JetReco/interface/CaloJet.h"

#include <TTree.h>

// Default constructor
HLTJetEfficiencyAnalyzer::HLTJetEfficiencyAnalyzer():
  jettree(0){}


HLTJetEfficiencyAnalyzer::~HLTJetEfficiencyAnalyzer(){
	delete jetpt;
	delete jeteta;
	delete jetphi;
}

void HLTJetEfficiencyAnalyzer::Setup(const edm::ParameterSet& iConfig,TTree *trigtree)
{
	HLTJetSource = iConfig.getParameter<edm::InputTag>("HLTJetSource");
	njets        = iConfig.getParameter<edm::int32>("HLTNJets");

  	jettree = trigtree;

	jetpt  = new float[njets];
	jeteta = new float[njets];
	jetphi = new float[njets];

  	// Setup branches
	for(int i = 0; i < njets; ++i){
		jettree->Branch(Form("HLTJet%uPt",i+1),&(jetpt[i]));
		jettree->Branch(Form("HLTJet%uEta",i+1),&(jeteta[i]));
		jettree->Branch(Form("HLTJet%uPhi",i+1),&(jetphi[i]));
	}
}

void HLTJetEfficiencyAnalyzer::fill(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

	reset();

	edm::Handle<reco::CaloJetCollection> hltjetHandle;
        iEvent.getByLabel(HLTJetSource, hltjetHandle);

	int index = 0;
	for(reco::CaloJetCollection::const_iterator i = hltjetHandle->begin();
	    i!= hltjetHandle->end() && index < njets; ++i){
		jetpt[index]  = i->pt();
		jeteta[index] = i->eta();
		jetphi[index] = i->phi();
		index++;
	}	
} 

void HLTJetEfficiencyAnalyzer::reset(){

	for(int i = 0; i < njets; ++i) {
		jetpt[i]  = 0;
		jeteta[i] = 0;
		jetphi[i] = 0;
	}
}
