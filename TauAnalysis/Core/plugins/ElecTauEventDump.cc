#include "TauAnalysis/Core/plugins/ElecTauEventDump.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ServiceRegistry/interface/Service.h"

#include "TauAnalysis/Core/interface/eventDumpAuxFunctions.h"
#include "TauAnalysis/DQMTools/interface/generalAuxFunctions.h"

#include <TMath.h>

ElecTauEventDump::ElecTauEventDump(const edm::ParameterSet& cfg)
  : GenericEventDump(cfg)
{
  //std::cout << "<ElecTauEventDump::ElecTauEventDump>:" << std::endl;
}

ElecTauEventDump::~ElecTauEventDump()
{
//--- nothing to be done yet...
}

void ElecTauEventDump::print(const edm::Event& iEvent, const edm::EventSetup& iSetup, 
			     const std::vector<std::pair<std::string, bool> >& filterResults_cumulative, 
			     const std::vector<std::pair<std::string, bool> >& filterResults_individual, 
			     double eventWeight) const
{  
  //std::cout << "<ElecTauEventDump::print>:" << std::endl; 

  printEventHeaderInfo(iEvent, eventWeight);

  printEventTriggerInfo(iEvent);

  printEventSelectionInfo(filterResults_cumulative, filterResults_individual, outputStream_);

  *outputStream_ << ">>GENERATOR LEVEL INFORMATION<<" << std::endl;

  edm::Handle<edm::View<reco::GenParticle> > genParticleCollection;
  iEvent.getByLabel(genParticleSource_, genParticleCollection);
  edm::Handle<edm::View<reco::GenJet> > genTauJetCollection;
  iEvent.getByLabel(genTauJetSource_, genTauJetCollection);
  printGenParticleInfo(genParticleCollection, genTauJetCollection, outputStream_);

  *outputStream_ << ">>RECONSTRUCTION LEVEL INFORMATION<<" << std::endl;

  printElectronInfo(iEvent);
  printMuonInfo(iEvent);
  printTauInfo(iEvent);

  printDiTauCandidateInfo(iEvent);

  printZeeInfo(iEvent);

  printMissingEtInfo(iEvent);

  printJetInfo(iEvent);
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/TauReco/interface/CaloTau.h"
#include "DataFormats/TauReco/interface/CaloTauFwd.h"
#include "DataFormats/TauReco/interface/PFTau.h"
#include "DataFormats/TauReco/interface/PFTauFwd.h"

#include "PhysicsTools/Utilities/interface/deltaR.h"

const std::string genElectronsFromZsCollectionSource = "genElectronsFromZs";
const std::string recoElectronCollectionSource = "pixelMatchGsfElectrons";
const std::string recoElectronTrackCollectionSource = "pixelMatchGsfFit";
const std::string recoCaloJetCollectionSource = "iterativeCone5CaloJets";
const std::string recoPFJetCollectionSource = "iterativeCone5PFJets";
const std::string recoCaloTauCollectionSource = "caloRecoTauProducer";
const std::string recoPFTauCollectionSource = "shrinkingConePFTauProducer";

const double dRmatch = 0.5;

void ElecTauEventDump::printZeeInfo(const edm::Event& iEvent) const
{
  edm::Handle<edm::View<reco::GenParticle> > genElectronsFromZsCollection;
  iEvent.getByLabel(genElectronsFromZsCollectionSource, genElectronsFromZsCollection);
  edm::Handle<reco::GsfElectronCollection> recoElectronCollection;
  iEvent.getByLabel(recoElectronCollectionSource, recoElectronCollection);
  edm::Handle<reco::GsfTrackCollection> recoElectronTrackCollection;
  iEvent.getByLabel(recoElectronTrackCollectionSource, recoElectronTrackCollection);
  edm::Handle<reco::CaloJetCollection> recoCaloJetCollection;
  iEvent.getByLabel(recoCaloJetCollectionSource, recoCaloJetCollection);
  edm::Handle<reco::PFJetCollection> recoPFJetCollection;
  iEvent.getByLabel(recoPFJetCollectionSource, recoPFJetCollection);
  edm::Handle<reco::CaloTauCollection> recoCaloTauCollection;
  iEvent.getByLabel(recoCaloTauCollectionSource, recoCaloTauCollection);
  edm::Handle<reco::PFTauCollection> recoPFTauCollection;
  iEvent.getByLabel(recoPFTauCollectionSource, recoPFTauCollection);

  for ( edm::View<reco::GenParticle>::const_iterator genElectron = genElectronsFromZsCollection->begin(); 
	genElectron != genElectronsFromZsCollection->end(); ++genElectron ) {
    *outputStream_ << "genElectronFromZs:" 
		   << " pdgId = " << genElectron->pdgId() << "," 
		   << " status = " << genElectron->status() << "," 
		   << " Pt = " << genElectron->pt() << "," 
		   << " eta = " << genElectron->eta() << "," 
		   << " phi = " << genElectron->phi() << std::endl;
    
    for ( reco::GsfElectronCollection::const_iterator recoElectron = recoElectronCollection->begin();
	  recoElectron != recoElectronCollection->end(); ++recoElectron ) {
      if ( reco::deltaR(genElectron->p4(), recoElectron->p4()) < dRmatch ) {
	*outputStream_ << " " << "gsfElectron:" 
		       << " Pt = " << recoElectron->pt() << "," 
		       << " eta = " << recoElectron->eta() << "," 
		       << " phi = " << recoElectron->phi() << std::endl;
      }
    }
    for ( reco::GsfTrackCollection::const_iterator recoElectronTrack = recoElectronTrackCollection->begin();
	  recoElectronTrack != recoElectronTrackCollection->end(); ++recoElectronTrack ) {
      if ( reco::deltaR(genElectron->eta(), genElectron->phi(), recoElectronTrack->eta(), recoElectronTrack->phi()) < dRmatch ) {
	*outputStream_ << " " << "gsfElectronTrack:" 
		       << " Pt = " << recoElectronTrack->pt() << "," 
		       << " eta = " << recoElectronTrack->eta() << "," 
		       << " phi = " << recoElectronTrack->phi() << std::endl;
      }
    }

    for ( reco::CaloJetCollection::const_iterator recoCaloJet = recoCaloJetCollection->begin();
	  recoCaloJet != recoCaloJetCollection->end(); ++recoCaloJet ) {
      if ( reco::deltaR(genElectron->p4(), recoCaloJet->p4()) < dRmatch ) {
	*outputStream_ << " " << "caloJet:" 
		       << " Pt = " << recoCaloJet->pt() << "," 
		       << " eta = " << recoCaloJet->eta() << "," 
		       << " phi = " << recoCaloJet->phi() << std::endl;
      }
    }
    for ( reco::PFJetCollection::const_iterator recoPFJet = recoPFJetCollection->begin();
	  recoPFJet != recoPFJetCollection->end(); ++recoPFJet ) {
      if ( reco::deltaR(genElectron->p4(), recoPFJet->p4()) < dRmatch ) {
	*outputStream_ << " " << "pfJet:" 
		       << " Pt = " << recoPFJet->pt() << "," 
		       << " eta = " << recoPFJet->eta() << "," 
		       << " phi = " << recoPFJet->phi() << std::endl;
      }
    }

    for ( reco::CaloTauCollection::const_iterator recoCaloTau = recoCaloTauCollection->begin();
	  recoCaloTau != recoCaloTauCollection->end(); ++recoCaloTau ) {
      if ( reco::deltaR(genElectron->p4(), recoCaloTau->p4()) < dRmatch ) {
	*outputStream_ << " " << "caloTau:" 
		       << " Pt = " << recoCaloTau->pt() << "," 
		       << " eta = " << recoCaloTau->eta() << "," 
		       << " phi = " << recoCaloTau->phi() << std::endl;
      }
    }
    for ( reco::PFTauCollection::const_iterator recoPFTau = recoPFTauCollection->begin();
	  recoPFTau != recoPFTauCollection->end(); ++recoPFTau ) {
      if ( reco::deltaR(genElectron->p4(), recoPFTau->p4()) < dRmatch ) {
	*outputStream_ << " " << "pfTau:" 
		       << " Pt = " << recoPFTau->pt() << "," 
		       << " eta = " << recoPFTau->eta() << "," 
		       << " phi = " << recoPFTau->phi() << std::endl;
      }
    }

    *outputStream_ << std::endl;
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(EventDumpPluginFactory, ElecTauEventDump, "ElecTauEventDump");
