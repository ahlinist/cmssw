#include "ElectroWeakAnalysis/TauTriggerEfficiency/interface/MuonAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/MuonReco/interface/Muon.h"

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
        pfIsoChargedSrc = iConfig.getParameter<edm::InputTag>("MuonPFIsoValueCharged");
        pfIsoNeutralSrc = iConfig.getParameter<edm::InputTag>("MuonPFIsoValueNeutral");
        pfIsoGammaSrc = iConfig.getParameter<edm::InputTag>("MuonPFIsoValueGamma");
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
        muontree->Branch("MuonPFIsoChargedPt", &muonPFIsoCharged);
        muontree->Branch("MuonPFIsoNeutralEt", &muonPFIsoNeutral);
        muontree->Branch("MuonPFIsoGammaEt", &muonPFIsoGamma);
	muontree->Branch("NMuons",&nMuons, "NMuons/I");

        muontree->Branch("MuonTauInvMass", &muTauInvMass);
;
}

void MuonAnalyzer::fill(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

	muonPt  = -999;
	muonEta = -999;
	muonPhi = -999;
        muonIso03SumPt = -999;
        muonIso03EmEt = -999;
        muonIso03HadEt = -999;
        muonPFIsoCharged = -999;
        muonPFIsoNeutral = -999;
        muonPFIsoGamma = -999;
	nMuons  = 0;

        muTauInvMass = 0;

//
	edm::Handle<edm::View<reco::Muon> > muons;
        iEvent.getByLabel(MuonSource, muons);

        typedef edm::ValueMap<double> IsoMap;
        edm::Handle<IsoMap> pfIsoCharged;
        iEvent.getByLabel(pfIsoChargedSrc, pfIsoCharged);
        edm::Handle<IsoMap> pfIsoNeutral;
        iEvent.getByLabel(pfIsoNeutralSrc, pfIsoNeutral);
        edm::Handle<IsoMap> pfIsoGamma;
        iEvent.getByLabel(pfIsoGammaSrc, pfIsoGamma);

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

                  muonPFIsoCharged = (*pfIsoCharged)[muons->refAt(i)];
                  muonPFIsoNeutral = (*pfIsoNeutral)[muons->refAt(i)];
                  muonPFIsoGamma = (*pfIsoGamma)[muons->refAt(i)];
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

