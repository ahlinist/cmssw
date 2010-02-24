#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"

#include "TauAnalysis/Skimming/plugins/SkimPurityAnalyzer.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <iostream>

SkimPurityAnalyzer::SkimPurityAnalyzer(const edm::ParameterSet& cfg)
{
	mode_ = cfg.getParameter<std::string>("mode");
	src_ = cfg.getParameter<edm::InputTag>("src");
	
	nEvents_ = 0;
	nRealElectrons_ = 0;
	nFakeElectrons_ = 0;
	nRealTaus_ = 0;
	nFakeTaus_ = 0;

	nElecTau_ = 0;
	nElecMu_ = 0;
	nMuTau_ = 0;
}

SkimPurityAnalyzer::~SkimPurityAnalyzer()
{
  //edm::LogInfo("~SkimPurityAnalyzer") << " Number of Events processed: >>> " << numEventsProcessed_ << " <<<";
}

void SkimPurityAnalyzer::beginJob(const edm::EventSetup& es)
{
// nothing to be done yet...
}

void SkimPurityAnalyzer::analyze(const edm::Event& evt, const edm::EventSetup& es)
{
  	edm::Handle<edm::View<reco::GenParticle> > genParticles;

	evt.getByLabel(src_, genParticles);

	nEvents_++;
	
	if(mode_ == std::string("ZtoElecTau")) {
		bool elec = false;
		bool tau = false;
		
		for ( edm::View<reco::GenParticle>::const_iterator genParticle = genParticles->begin();
				genParticle != genParticles->end(); ++genParticle ) {
			// skip documentation entries
			if( genParticle->status() == 3) ++genParticle;
			
			int pdgId = genParticle->pdgId();
			
			// get electron cand
			if( genParticle->numberOfMothers() != 1 ) ++genParticle;
			if( pdgId == -11 || pdgId == 11) {
				const reco::GenParticle* genMother = dynamic_cast<const reco::GenParticle*>(genParticle->mother(0));
				if( genMother->pdgId() == 15 || genMother->pdgId() == -15 ) {nRealElectrons_++; elec = true;}
				else nFakeElectrons_++;
			}
		}
		if(elec && tau) nElecTau_++;
	}

}

void SkimPurityAnalyzer::endJob()
{
	std::cout << "[SkimPurityAnalyzer::endJob] Number of Events processed: >>> " << nEvents_ << " <<<" << std::endl;	
	//edm::LogInfo("SkimPurityAnalyzer::endJob") << " Number of Events processed: >>> " << nEvents_ << " <<<";	
	
	if(mode_ == std::string("ZtoElecTau")) {
		std::cout << "[SkimPurityAnalyzer] Number of real electronnic tau decays: >>> " << nRealElectrons_ << " <<<" << std::endl;	
		//edm::LogInfo("SkimPurityAnalyzer") << " Number of real electronnic tau decays: >>> " << nRealElectrons_ << " <<<";	
		//edm::LogInfo("SkimPurityAnalyzer") << " Number of fake electrons: >>> " << nFakeElectrons_ << " <<<";	
		//edm::LogInfo("SkimPurityAnalyzer") << " Number ofevents with real hadronic tau decays: >>> " << nRealTaus_ << " <<<";	
		//edm::LogInfo("SkimPurityAnalyzer") << " Number of fake taus: >>> " << nFakeTaus_ << " <<<";	
		//edm::LogInfo("SkimPurityAnalyzer") << " Number of events with real hadronic + electronic tau decays: >>> " << nElecTau_ << " <<<";	
	}
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(SkimPurityAnalyzer);


