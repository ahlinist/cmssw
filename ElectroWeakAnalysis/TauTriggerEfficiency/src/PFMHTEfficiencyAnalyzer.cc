#include "ElectroWeakAnalysis/TauTriggerEfficiency/interface/PFMHTEfficiencyAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/JetReco/interface/PFJet.h"

#include <TTree.h>

// Default constructor
PFMHTEfficiencyAnalyzer::PFMHTEfficiencyAnalyzer():
  pfmhttree(0){}


PFMHTEfficiencyAnalyzer::~PFMHTEfficiencyAnalyzer(){}

void PFMHTEfficiencyAnalyzer::Setup(const edm::ParameterSet& iConfig,TTree *trigtree)
{
	PFJetSource = iConfig.getParameter<edm::InputTag>("PFJetSource");
	MHTJetThreshold = iConfig.getParameter<double>("MHTJetThreshold");

  	pfmhttree = trigtree;

  	// Setup branches
  	pfmhttree->Branch("HLTPFMHT", &hltPFMHT);
}

void PFMHTEfficiencyAnalyzer::fill(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

	hltPFMHT = 0.0;

        double hx = 0,
               hy = 0;

	edm::Handle<edm::View<reco::PFJet> > hjets;
	iEvent.getByLabel(PFJetSource, hjets);

	edm::PtrVector<reco::PFJet> jets = hjets->ptrVector();
	for(edm::PtrVector<reco::PFJet>::const_iterator iter = jets.begin(); iter != jets.end(); ++iter) {
                edm::Ptr<reco::PFJet> iJet = *iter;
		if(iJet->pt() < MHTJetThreshold) continue;
                hx += iJet->px();
                hy += iJet->py();
        }

	hltPFMHT = sqrt(hx*hx + hy*hy);
} 

