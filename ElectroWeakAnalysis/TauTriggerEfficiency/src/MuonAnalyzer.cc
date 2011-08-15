#include "ElectroWeakAnalysis/TauTriggerEfficiency/interface/MuonAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"

#include <TTree.h>
#include <iostream>

// Default constructor
MuonAnalyzer::MuonAnalyzer():
  muontree(0){}


MuonAnalyzer::~MuonAnalyzer(){}

void MuonAnalyzer::Setup(const edm::ParameterSet& iConfig,TTree *trigtree)
{
	MuonSource = iConfig.getParameter<edm::InputTag>("MuonSource");
        muTauPairSource = iConfig.getParameter<edm::InputTag>("MuonTauPairSource");
	ptMin      = iConfig.getParameter<double>("MuonPtMin");
	etaMax     = iConfig.getParameter<double>("MuonEtaMax");

  	muontree = trigtree;

  	// Setup branches
  	muontree->Branch("MuonPt", &muonPt);
	muontree->Branch("MuonEta",&muonEta);
	muontree->Branch("MuonPhi",&muonPhi);
        muontree->Branch("MuonIso03SumPt", &muonIso03SumPt);
        muontree->Branch("MuonIso03EmEt", &muonIso03EmEt);
        muontree->Branch("MuonIso03HadEt", &muonIso03HadEt);
	muontree->Branch("NMuons",&nMuons, "NMuons/I");

        muontree->Branch("MuonTauInvMass", &muTauInvMass);
;
}

void MuonAnalyzer::fill(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

	muonPt  = -999;
	muonEta = -999;
	muonPhi = -999;
	nMuons  = 0;

        muTauInvMass = 0;

//
	edm::Handle<reco::MuonCollection> muons;
        iEvent.getByLabel(MuonSource, muons);

	if(muons.isValid()) {
	  for(size_t i = 0; i < muons->size(); ++i){
		if(muons->at(i).pt() < ptMin) continue;
		if(fabs(muons->at(i).eta()) > etaMax) continue;
		// add check that the muon is from the primary vertex?
		if(muonPt < 0) { // assuming muons are pt ordered, saving the first
		  muonPt  = muons->at(i).pt();
		  muonEta = muons->at(i).eta();
		  muonPhi = muons->at(i).phi();
                  muonIso03SumPt = muons->at(i).isolationR03().sumPt;
                  muonIso03EmEt = muons->at(i).isolationR03().emEt;
                  muonIso03HadEt = muons->at(i).isolationR03().hadEt;
		}
		nMuons++;
	  }
	}

        edm::Handle<edm::View<reco::Candidate> > hcands;
        iEvent.getByLabel(muTauPairSource, hcands);
        if(hcands.isValid() && !hcands->empty()) {
          muTauInvMass = hcands->at(0).mass();
        }
}

