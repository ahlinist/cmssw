#include "ElectroWeakAnalysis/TauTriggerEfficiency/interface/METEfficiencyAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CaloMETFwd.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/PFMETFwd.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include <TTree.h>

// Default constructor
METEfficiencyAnalyzer::METEfficiencyAnalyzer():
  mettree(0){}


METEfficiencyAnalyzer::~METEfficiencyAnalyzer(){}

void METEfficiencyAnalyzer::Setup(const edm::ParameterSet& iConfig,TTree *trigtree)
{
	HLTMETSource = iConfig.getParameter<edm::InputTag>("HLTMETSource");
	METSource = iConfig.getParameter<edm::InputTag>("METSource");
	MCSource  = iConfig.getParameter<edm::InputTag>("GenParticleCollection");

  	mettree = trigtree;

  	// Setup branches
  	mettree->Branch("HLTMET", &hltMET);
	mettree->Branch("MET",    &MET);
	mettree->Branch("MCMET",  &mcMET);
}

void METEfficiencyAnalyzer::fill(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

	hltMET = 0.0;
	MET    = 0.0;
	mcMET  = 0.0;

//
	edm::Handle<reco::CaloMETCollection> hltmetHandle;
        iEvent.getByLabel(HLTMETSource, hltmetHandle);

	hltMET = (hltmetHandle->front() ).et();

//

	edm::Handle<reco::PFMETCollection> metHandle;
	iEvent.getByLabel(METSource, metHandle);

	MET = (metHandle->front() ).et();

//

	mcMET = -1;
	edm::Handle<reco::GenParticleCollection> mcEventHandle;
        iEvent.getByLabel(MCSource, mcEventHandle);

	if(mcEventHandle.isValid()) {
	  const reco::GenParticleCollection& genParticles(*mcEventHandle);

          double mcMetX = 0;
          double mcMetY = 0;

          reco::GenParticleCollection::const_iterator i;
          for(i = genParticles.begin(); i!= genParticles.end(); ++i){
		int id = i->pdgId();
                if(abs(id) == 12 || abs(id) == 14 || abs(id) == 16){
                        mcMetX += i->px();
                        mcMetY += i->py();
                }
	  }

	  mcMET = sqrt(mcMetX*mcMetX + mcMetY*mcMetY);
	}
} 

