#include "TauAnalysis/Core/plugins/wTauNuEventDump.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

#include "TauAnalysis/Core/interface/eventDumpAuxFunctions.h"
#include "TauAnalysis/DQMTools/interface/generalAuxFunctions.h"

#include <TMath.h>

wTauNuEventDump::wTauNuEventDump(const edm::ParameterSet& cfg)
  : GenericEventDump(cfg)
{
  //std::cout << "<wTauNuEventDump::wTauNuEventDump>:" << std::endl;

  tauDump_ = makeObjectDump(cfg, "PATTauDump");  
}

wTauNuEventDump::~wTauNuEventDump()
{
//--- nothing to be done yet...
}

void wTauNuEventDump::print(const edm::Event& iEvent, const edm::EventSetup& iSetup, 
			   const std::vector<std::pair<std::string, bool> >& filterResults_cumulative, 
			   const std::vector<std::pair<std::string, bool> >& filterResults_individual, 
			   double eventWeight) const
{  
  printEventHeaderInfo(iEvent, eventWeight);

  printEventSelectionInfo(filterResults_cumulative, filterResults_individual, outputStream_);


  if( doGenInfo_){
	*outputStream_ << ">>GENERATOR LEVEL INFORMATION<<" << std::endl;
	
	edm::Handle<edm::View<reco::GenParticle> > genParticleCollection;
	iEvent.getByLabel(genParticleSource_, genParticleCollection);
	edm::Handle<edm::View<reco::GenJet> > genTauJetCollection;
	iEvent.getByLabel(genTauJetSource_, genTauJetCollection);
	edm::Handle<edm::View<reco::GenJet> > genJetCollection;
	iEvent.getByLabel("iterativeCone5GenJets", genJetCollection);
	
	edm::Handle<pat::JetCollection> patJets;
	iEvent.getByLabel(patJetSource_, patJets);
  
	printGenParticleInfo(genParticleCollection, genTauJetCollection, outputStream_);
	//  printGenJetInfo(genJetCollection, patJets, outputStream_);
	
	bool matched = false;
	for( edm::View<reco::GenJet>::const_iterator genJet = genJetCollection->begin();genJet != genJetCollection->end(); ++genJet){
	  if(genJet->pt() > 10){
		if(TMath::Abs(genJet->eta()) > 3)
		  *outputStream_<<"HIGH ETA GEN-JET!!! ";
		*outputStream_<<"gen-jet: Pt = "<<genJet->pt()<<", eta = "
					  <<genJet->eta()<<", phi = "
					  <<genJet->phi()*180./TMath::Pi()<<std::endl;
		matched = false;
		for ( pat::JetCollection::const_iterator patJet = patJets->begin(); patJet != patJets->end(); ++patJet ) {
		  
		  if( reco::deltaR(patJet->p4(), genJet->p4() ) < 0.5 ){
			*outputStream_<<"Matched with pat-jet: Pt = "<<patJet->pt()<<", eta = "<<patJet->eta()<<", phi = "<<patJet->phi()*180./TMath::Pi()<<std::endl;
			matched = true;
		  }
		}
		if(matched == false)
		  *outputStream_<<"NO MATCH!!!"<<std::endl;
	  }
	}
  }
  *outputStream_ << ">>RECONSTRUCTION LEVEL INFORMATION<<" << std::endl;

  tauDump_->print(iEvent, iSetup);

  printMissingEtInfo(iEvent);

  printJetInfo(iEvent);  
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(EventDumpPluginFactory, wTauNuEventDump, "wTauNuEventDump");
