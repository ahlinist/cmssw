#include "TauAnalysis/Core/interface/GenericEventDump.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMap.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/GenMETFwd.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"

#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"

#include "TauAnalysis/Core/interface/eventDumpAuxFunctions.h"
#include "TauAnalysis/Core/interface/eventAuxFunctions.h"
#include "TauAnalysis/Core/interface/histManagerAuxFunctions.h"
#include "TauAnalysis/DQMTools/interface/generalAuxFunctions.h"
#include "TauAnalysis/CandidateTools/interface/candidateAuxFunctions.h"
#include "TauAnalysis/GenSimTools/interface/genParticleAuxFunctions.h"

#include <iostream>
#include <fstream>

edm::InputTag getInputTag(const edm::ParameterSet& cfg, const char* parameter)
{
  return ( cfg.exists(parameter) ) ? cfg.getParameter<edm::InputTag>(parameter) : edm::InputTag();
}

GenericEventDump::GenericEventDump(const edm::ParameterSet& cfg): EventDumpBase(cfg)
{
  //std::cout << "<GenericEventDump::GenericEventDump>:" << std::endl;

  l1GtReadoutRecordSource_ = getInputTag(cfg, "l1GtReadoutRecordSource");
  l1GtObjectMapRecordSource_ = getInputTag(cfg, "l1GtObjectMapRecordSource");
  l1BitsToPrint_ = ( cfg.exists("l1BitsToPrint") ) ? 
    cfg.getParameter<vstring>("l1BitsToPrint") : vstring();
  
  hltResultsSource_ = getInputTag(cfg, "hltResultsSource");
  hltPathsToPrint_ = ( cfg.exists("hltPathsToPrint") ) ? 
    cfg.getParameter<vstring>("hltPathsToPrint") : vstring();
  
  genParticleSource_ = getInputTag(cfg, "genParticleSource");
  genJetSource_ = getInputTag(cfg, "genJetSource");
  genTauJetSource_ = getInputTag(cfg, "genTauJetSource");
  genEventInfoSource_ = getInputTag(cfg, "genEventInfoSource");
  
  patElectronSource_ = getInputTag(cfg, "electronSource");
  patMuonSource_ = getInputTag(cfg, "muonSource");
  patTauSource_ = getInputTag(cfg, "tauSource");
  printTauIdEfficiencies_ = cfg.exists("printTauIdEfficiencies") ? 
    cfg.getParameter<bool>("printTauIdEfficiencies") : false;
  
  diTauCandidateSource_ = getInputTag(cfg, "diTauCandidateSource");
  
  patCaloMEtSource_ = getInputTag(cfg, "caloMEtSource");
  patPFMEtSource_ = getInputTag(cfg, "pfMEtSource");
  genMEtSource_ = getInputTag(cfg, "genMEtSource");
  
  skipPdgIdsGenParticleMatch_.push_back(12);
  skipPdgIdsGenParticleMatch_.push_back(14);
  skipPdgIdsGenParticleMatch_.push_back(16);
  
  patJetSource_ = getInputTag(cfg, "jetSource");
  
  recoTrackSource_ = getInputTag(cfg, "recoTrackSource");
  recoVertexSource_ = getInputTag(cfg, "recoVertexSource");
  
  pfChargedHadronSource_ = getInputTag(cfg, "pfChargedHadronSource");
  pfGammaSource_ = getInputTag(cfg, "pfGammaSource");
  pfNeutralHadronSource_ = getInputTag(cfg, "pfNeutralHadronSource");
  pfCandidateSource_ = getInputTag(cfg, "pfCandidateSource");
  
  doGenInfo_ = ( cfg.exists("doGenInfo") ) ? cfg.getParameter<bool>("doGenInfo") : true;
}

void printMatchingGenParticleTypes(const char* header_matched, 
				   unsigned numMatchingMuons, 
				   unsigned numMatchingElectrons, 
				   unsigned numMatchingTauJets,
				   unsigned numMatchingBottomQuarks,
				   unsigned numMatchingCharmQuarks,
				   unsigned numMatchingGluons,
				   unsigned numMatchingLightQuarks,
				   const char* header_unmatched, 
				   unsigned numUndeterminedMatches,
				   std::ostream* stream)
{
  if ( stream ) {
    *stream << header_matched << ":" << std::endl;
    *stream << " Muons = " << numMatchingMuons << std::endl;
    *stream << " Electrons = " << numMatchingElectrons << std::endl;
    *stream << " Tau-Jets = " << numMatchingTauJets << std::endl;
    *stream << " b-Quarks = " << numMatchingBottomQuarks << std::endl;
    *stream << " c-Quarks = " << numMatchingCharmQuarks << std::endl;
    *stream << " Gluons = " << numMatchingGluons << std::endl;
    *stream << " u,d,s-Quarks = " << numMatchingLightQuarks << std::endl;
    *stream << header_unmatched << " = " << numUndeterminedMatches << std::endl;
    *stream << std::endl;
  }
}

GenericEventDump::~GenericEventDump()
{
  if ( doGenInfo_ ) {
//--- print counts of different types of particles faking reconstructed electrons,
//    muons and tau-jets
    printMatchingGenParticleTypes("Number of reconstructed Electrons matching generated", 
				  numRecoElectronsMatchingGenMuons_, 
				  numRecoElectronsMatchingGenElectrons_,
				  numRecoElectronsMatchingGenTauJets_,
				  numRecoElectronsMatchingGenBottomQuarks_,
				  numRecoElectronsMatchingGenCharmQuarks_,
				  numRecoElectronsMatchingGenGluons_,
				  numRecoElectronsMatchingGenLightQuarks_,
				  "Number of reconstructed Electrons not matched to generator level information",
				  numRecoElectronsUndeterminedGenMatch_,
				  outputStream_);
    printMatchingGenParticleTypes("Number of reconstructed Muons matching generated", 
				  numRecoMuonsMatchingGenMuons_, 
				  numRecoMuonsMatchingGenElectrons_,
				  numRecoMuonsMatchingGenTauJets_,
				  numRecoMuonsMatchingGenBottomQuarks_,
				  numRecoMuonsMatchingGenCharmQuarks_,
				  numRecoMuonsMatchingGenGluons_,
				  numRecoMuonsMatchingGenLightQuarks_,
				  "Number of reconstructed Muons not matched to generator level information",
				  numRecoMuonsUndeterminedGenMatch_,
				  outputStream_);
    printMatchingGenParticleTypes("Number of reconstructed Tau-Jets matching generated", 
				  numRecoTauJetsMatchingGenMuons_, 
				  numRecoTauJetsMatchingGenElectrons_,
				  numRecoTauJetsMatchingGenTauJets_,
				  numRecoTauJetsMatchingGenBottomQuarks_,
				  numRecoTauJetsMatchingGenCharmQuarks_,
				  numRecoTauJetsMatchingGenGluons_,
				  numRecoTauJetsMatchingGenLightQuarks_,
				  "Number of reconstructed Tau-Jets not matched to generator level information",
				  numRecoTauJetsUndeterminedGenMatch_,
				  outputStream_);
  }
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void GenericEventDump::analyze(const edm::Event& evt, const edm::EventSetup& es, 
			       const GenericAnalyzer_namespace::filterResults_type& filterResults_cumulative, 
			       const GenericAnalyzer_namespace::filterResults_type& filterResults_individual, 
			       double eventWeight) 
{
  //std::cout << "<GenericEventDump::analyze>:" << std::endl;
  
  EventDumpBase::analyze(evt, es, filterResults_cumulative, filterResults_individual, eventWeight);
  
  if ( doGenInfo_ ) countFakeParticles(evt);
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

bool matchesGenParticleType(const reco::GenParticleCollection& genParticleCollection, int pdgId)
{
  for ( reco::GenParticleCollection::const_iterator genParticle = genParticleCollection.begin(); 
	genParticle != genParticleCollection.end(); ++genParticle ) {
    
    //--- genParticle with pdgId given as function argument contained in collection
    if ( genParticle->pdgId() == pdgId ) return true;
  }
  
  //--- genParticle with pdgId given as function argument **not** contained in collection
  return false;
}

void countMatchingGenParticleTypes(const reco::Particle::LorentzVector& recoMomentum,
				   const edm::Event& evt,
				   const edm::InputTag& genParticleSrc,
				   const edm::InputTag& genTauJetSrc,
				   unsigned& numMatchingMuons, 
				   unsigned& numMatchingElectrons, 
				   unsigned& numMatchingTauJets,
				   unsigned& numMatchingBottomQuarks,
				   unsigned& numMatchingCharmQuarks,
				   unsigned& numMatchingGluons,
				   unsigned& numMatchingLightQuarks,
				   unsigned& numUndeterminedMatches)
{
//--- select genParticles matching direction of reconstructed particle
//    within cone of size dR = 0.5;
//    require generated transverse momentum to be at least half of reconstructed transverse momentum
  edm::Handle<edm::View<reco::GenParticle> > genParticleCollection;
  evt.getByLabel(genParticleSrc, genParticleCollection);
  reco::GenParticleCollection matchingGenParticles;
  for ( edm::View<reco::GenParticle>::const_iterator genParticle = genParticleCollection->begin(); 
	genParticle != genParticleCollection->end(); ++genParticle ) {
    
//--- skip "documentation line" entries
//    (copied over to reco::GenParticle from HepMC product)
    if ( genParticle->status() == 3 ) continue;
    
    if ( genParticle->pt() > 0.50*recoMomentum.pt() &&
	 reco::deltaR(genParticle->p4(), recoMomentum) < 0.5 ) {
      matchingGenParticles.push_back(*genParticle);
    }
  }
  
//--- select genTauJets matching direction of reconstructed particle
//    within cone of size dR = 0.5; 
//    require generated visible momentum of tau-jet to be at least half of reconstructed momentum
  edm::Handle<edm::View<reco::GenJet> > genTauJetCollection;
  evt.getByLabel(genTauJetSrc, genTauJetCollection);
  reco::GenJetCollection matchingGenTauJets;
  for ( edm::View<reco::GenJet>::const_iterator genTauJet = genTauJetCollection->begin(); 
	genTauJet != genTauJetCollection->end(); ++genTauJet ) {
    
    if ( genTauJet->pt() > 0.50*recoMomentum.pt() &&
	 reco::deltaR(genTauJet->p4(), recoMomentum) < 0.5 ) {
      matchingGenTauJets.push_back(*genTauJet);
    }
  }
  
//--- count matched generator level particles and tau-jets in the order
//     electron, muon, tau-jet, b-quark, c-quark, gluon, uds-quarks;
//    count also number of cases in which matching failed 
//    (e.g. due to matching transverse momentum requirement)
  if ( matchesGenParticleType(matchingGenParticles, 11) ) {
    ++numMatchingMuons;
  } else if ( matchesGenParticleType(matchingGenParticles, 13) ) {
    ++numMatchingElectrons;
  } else if ( matchingGenTauJets.size() >= 1 ) {
    ++numMatchingTauJets;
  } else if ( matchesGenParticleType(matchingGenParticles, 5) ) {
    ++numMatchingBottomQuarks;
  } else if ( matchesGenParticleType(matchingGenParticles, 4) ) {
    ++numMatchingCharmQuarks;
  } else if ( matchesGenParticleType(matchingGenParticles, 21) ) {
    ++numMatchingGluons;
  } else if ( matchesGenParticleType(matchingGenParticles, 1) ||
	      matchesGenParticleType(matchingGenParticles, 2) ||
	      matchesGenParticleType(matchingGenParticles, 3) ) {
    ++numMatchingLightQuarks;
  } else {
    ++numUndeterminedMatches;
  }
}

void GenericEventDump::countFakeParticles(const edm::Event& evt)
{
//--- count different types of particles faking reconstructed electrons
  if ( patElectronSource_.label() != "" ) {
    edm::Handle<pat::ElectronCollection> patElectrons;
    evt.getByLabel(patElectronSource_, patElectrons);
    for ( pat::ElectronCollection::const_iterator patElectron = patElectrons->begin(); 
	  patElectron != patElectrons->end(); ++patElectron ) {
      countMatchingGenParticleTypes(patElectron->p4(), 
				    evt, genParticleSource_, genTauJetSource_,
				    numRecoElectronsMatchingGenMuons_, 
				    numRecoElectronsMatchingGenElectrons_,
				    numRecoElectronsMatchingGenTauJets_,
				    numRecoElectronsMatchingGenBottomQuarks_,
				    numRecoElectronsMatchingGenCharmQuarks_,
				    numRecoElectronsMatchingGenGluons_,
				    numRecoElectronsMatchingGenLightQuarks_,
				    numRecoElectronsUndeterminedGenMatch_);
    }
  }
  
//--- count different types of particles faking reconstructed muons
  if ( patMuonSource_.label() != "" ) {
    edm::Handle<pat::MuonCollection> patMuons;
    evt.getByLabel(patMuonSource_, patMuons);
    for ( pat::MuonCollection::const_iterator patMuon = patMuons->begin(); 
	  patMuon != patMuons->end(); ++patMuon ) {
      countMatchingGenParticleTypes(patMuon->p4(), 
				    evt, genParticleSource_, genTauJetSource_,
				    numRecoMuonsMatchingGenMuons_, 
				    numRecoMuonsMatchingGenElectrons_,
				    numRecoMuonsMatchingGenTauJets_,
				    numRecoMuonsMatchingGenBottomQuarks_,
				    numRecoMuonsMatchingGenCharmQuarks_,
				    numRecoMuonsMatchingGenGluons_,
				    numRecoMuonsMatchingGenLightQuarks_,
				    numRecoMuonsUndeterminedGenMatch_);
    }
  }

//--- count different types of particles faking reconstructed tau-jets
  if ( patTauSource_.label() != "" ) {
    edm::Handle<pat::TauCollection> patTaus;
    evt.getByLabel(patTauSource_, patTaus);
    for ( pat::TauCollection::const_iterator patTau = patTaus->begin(); 
	  patTau != patTaus->end(); ++patTau ) {
      countMatchingGenParticleTypes(patTau->p4(), 
				    evt, genParticleSource_, genTauJetSource_,
				    numRecoTauJetsMatchingGenMuons_, 
				    numRecoTauJetsMatchingGenElectrons_,
				    numRecoTauJetsMatchingGenTauJets_,
				    numRecoTauJetsMatchingGenBottomQuarks_,
				    numRecoTauJetsMatchingGenCharmQuarks_,
				    numRecoTauJetsMatchingGenGluons_,
				    numRecoTauJetsMatchingGenLightQuarks_,
				    numRecoTauJetsUndeterminedGenMatch_);
    }
  }
}
//
//-----------------------------------------------------------------------------------------------------------------------
//

void GenericEventDump::printEventHeaderInfo(const edm::Event& evt, double eventWeight) const
{
  if ( !outputStream_ ) {
    edm::LogError ("printEventHeaderInfo") << " outputStream = NULL --> skipping !!";
    return;
  }
  
  edm::RunNumber_t runNumber = evt.id().run();
  edm::LuminosityBlockNumber_t lumiSectionNumber = evt.luminosityBlock();
  edm::EventNumber_t eventNumber = evt.id().event();
  
  *outputStream_ << "Run: " << runNumber << std::endl;
  *outputStream_ << "LumiSection: " << lumiSectionNumber << std::endl;
  *outputStream_ << "Event: " << eventNumber << std::endl;
  *outputStream_ << std::endl;
  *outputStream_ << " weight = " << eventWeight << std::endl;
  *outputStream_ << std::endl;
}

void GenericEventDump::printEventTriggerInfo(const edm::Event& evt) const
{
  if ( !outputStream_ ) {
    edm::LogError ("printEventTriggerInfo") << " outputStream = NULL --> skipping !!";
    return;
  }
  
  if ( l1GtReadoutRecordSource_.label() != "" && l1GtObjectMapRecordSource_.label() != "" ) {
    edm::Handle<L1GlobalTriggerReadoutRecord> l1GtReadoutRecord;
    evt.getByLabel(l1GtReadoutRecordSource_, l1GtReadoutRecord);
    edm::Handle<L1GlobalTriggerObjectMapRecord> l1GtObjectMapRecord;
    evt.getByLabel(l1GtObjectMapRecordSource_, l1GtObjectMapRecord);
    
    DecisionWord l1GtDecision = l1GtReadoutRecord->decisionWord();
    const std::vector<L1GlobalTriggerObjectMap>& l1GtObjectMaps = l1GtObjectMapRecord->gtObjectMap();
/*
    for ( std::vector<L1GlobalTriggerObjectMap>::const_iterator l1GtObjectMap = l1GtObjectMaps.begin();
      l1GtObjectMap != l1GtObjectMaps.end(); ++l1GtObjectMap ) {
      std::string l1Bit_i = (*l1GtObjectMap).algoName();
      std::cout << " l1Bit_i = " << l1Bit_i << std::endl;
    }
 */
    *outputStream_ << "L1 Trigger Decisions:" << std::endl;
    
    for ( vstring::const_iterator l1Bit = l1BitsToPrint_.begin();
	  l1Bit != l1BitsToPrint_.end(); ++l1Bit ) {
      bool isMatch = false;
      for ( std::vector<L1GlobalTriggerObjectMap>::const_iterator l1GtObjectMap = l1GtObjectMaps.begin();
	    l1GtObjectMap != l1GtObjectMaps.end(); ++l1GtObjectMap ) {
	std::string l1Bit_i = (*l1GtObjectMap).algoName();
	if ( l1Bit_i == (*l1Bit) ) {
	  int index = (*l1GtObjectMap).algoBitNumber();
	  std::string l1TriggerDecision = ( l1GtDecision[index] ) ? "passed" : "failed";
	  *outputStream_ << " " << (*l1Bit) << " " << l1TriggerDecision << std::endl;
	  isMatch = true;
	}
      }
      
      if ( !isMatch ) {
	edm::LogError ("printEventTriggerInfo") << " Undefined L1 bit = " << (*l1Bit) << " --> skipping !!";
	continue;
      }
    }
  }
  
  if ( hltResultsSource_.label() != "" ) {
    edm::Handle<edm::TriggerResults> hltResults;
    evt.getByLabel(hltResultsSource_, hltResults);
    
    const edm::TriggerNames& triggerNames = evt.triggerNames(*hltResults);
/*
    for ( edm::TriggerNames::Strings::const_iterator triggerName = triggerNames.triggerNames().begin();
          triggerName != triggerNames.triggerNames().end(); ++triggerName ) {
      unsigned int index = triggerNames.triggerIndex(*triggerName);
      if ( index < triggerNames.size() ) {
        std::string triggerDecision = ( hltResults->accept(index) ) ? "passed" : "failed";
    
        std::cout << " triggerName = " << (*triggerName) << " " << triggerDecision << std::endl;
      }
    }
 */    
    *outputStream_ << "HLT Decisions:" << std::endl;
    
    for ( std::vector<std::string>::const_iterator hltPath = hltPathsToPrint_.begin();
	  hltPath != hltPathsToPrint_.end(); ++hltPath ) {
      unsigned int index = triggerNames.triggerIndex(*hltPath);
      if ( index < triggerNames.size() ) {
	std::string hltDecision = ( hltResults->accept(index) ) ? "passed" : "failed";	
	*outputStream_ << " " << (*hltPath) << " " << hltDecision << std::endl;
      } else {
	edm::LogError ("printEventTriggerInfo") << " Undefined trigger Path = " << (*hltPath) << " --> skipping !!";
	continue;
      }
    }
    
    *outputStream_ << std::endl;
  }
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void GenericEventDump::printJetInfo(const edm::Event& evt) const
{
  if ( !outputStream_ ) {
    edm::LogError ("printJetInfo") << " outputStream = NULL --> skipping !!";
    return;
  }
  
  if ( patJetSource_.label() != "" ) {
    edm::Handle<pat::JetCollection> patJets;
    evt.getByLabel(patJetSource_, patJets);
    
    unsigned iJet = 0;
    for ( pat::JetCollection::const_iterator patJet = patJets->begin(); 
	  patJet != patJets->end(); ++patJet ) {

      if ( !patJet->et() > 10. ) continue;

      *outputStream_ << "Jet(" << iJet << "):" << std::endl;
      *outputStream_ << " Et = " << patJet->et() << std::endl;
      *outputStream_ << " theta = " << patJet->theta()*180./TMath::Pi() 
		     << " (eta = " << patJet->eta() << ")" << std::endl;
      *outputStream_ << " phi = " << patJet->phi()*180./TMath::Pi() << std::endl;
      *outputStream_ << " Tracks" << std::endl;
      double trackPtSum = 0.;
      for ( reco::TrackRefVector::const_iterator track = patJet->associatedTracks().begin();
	    track != patJet->associatedTracks().end(); ++track ) {
	printTrackInfo(*track, patJet->vertex(), false, false, outputStream_);
	trackPtSum += (*track)->pt();
      }
      *outputStream_ << "  sum Pt = " << trackPtSum << std::endl;
      ++iJet;
    }
    
    *outputStream_ << std::endl;
  }
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void printMissingEtInfo_i(const edm::Event& evt, const edm::InputTag& src, std::ostream& stream, const char* label)
{
  if ( src.label() != "" ) {
    
    edm::Handle<pat::METCollection> patMETs;
    evt.getByLabel(src, patMETs);
    
    for ( pat::METCollection::const_iterator patMET = patMETs->begin(); 
	  patMET != patMETs->end(); ++patMET ) {
      
      stream << label 
	     << " Et = " << patMET->pt() << "," 
	     << " phi = " <<  patMET->phi()*180./TMath::Pi() 
	     << " (Px = " << patMET->px() << ", Py = " << patMET->py() << ")" << std::endl;
      if ( patMET->genMET() != NULL ) {
	const reco::GenMET* genMET = patMET->genMET();	
	stream << " associated genMET:" 
	       << "  Et = " << genMET->pt() << "," 
	       << "  phi = " <<  genMET->phi()*180./TMath::Pi() 
	       << " (Px = " << genMET->px() << ", Py = " << genMET->py() << ")" << std::endl;
      } else {
	stream << "no genMET associated to PAT MET !!" << std::endl;
      }
    }
  }
}

void printJetMatchingInfo_i(const edm::Event& evt, const edm::InputTag& srcRecoJet, const edm::InputTag& srcGenJet, 
			    std::ostream& stream, const char* label)
{
  std::cout << label << " matching:" << std::endl;

  edm::Handle<edm::View<reco::Jet> > recoJets;
  evt.getByLabel(srcRecoJet, recoJets);
  
  edm::Handle<reco::GenJetCollection> genJets;
  evt.getByLabel(srcGenJet, genJets);
  
  std::vector<const reco::Jet*> recoJets_matched;
  
  const double maxPtDifference = 2.5;
  
  for ( reco::GenJetCollection::const_iterator genJet = genJets->begin();
	genJet != genJets->end(); ++genJet ) {
//--- CV: genJet includes neutrinos,
//        so cannot simply take genJet four-vector, but need to compute visible momentum
    reco::Candidate::LorentzVector genJetVisMomentum = getVisMomentum(genJet->getGenConstituents(), 1);
    
    if ( !(genJet->pt() > maxPtDifference) ) continue;
    
    const reco::Jet* recoJet_matched = 0;
    double dRmin = 1.e+3;
    
    for ( edm::View<reco::Jet>::const_iterator recoJet = recoJets->begin(); 
	  recoJet != recoJets->end(); ++recoJet ) {
      double dR = reco::deltaR(genJetVisMomentum, recoJet->p4());
      if ( dR < 0.5 && dR < dRmin ) {
	recoJet_matched = &(*recoJet);
	dRmin = dR;
      }
    }
    
    double dPx = ( recoJet_matched ) ? genJetVisMomentum.px() - recoJet_matched->px() : genJetVisMomentum.px();
    double dPy = ( recoJet_matched ) ? genJetVisMomentum.py() - recoJet_matched->py() : genJetVisMomentum.py();
    
    if ( TMath::Sqrt(dPx*dPx + dPy*dPy) > maxPtDifference ) {
      std::cout << "genJet not matching recoJet: Pt = " << genJetVisMomentum.pt() << ", eta = " << genJetVisMomentum.eta() << "," 
		<< " phi = " << genJetVisMomentum.phi()*180./TMath::Pi() << std::endl; 
      std::cout << "(genPx = " << genJetVisMomentum.px() << ", genPy = " << genJetVisMomentum.py();
      if ( recoJet_matched ) std::cout << "; recoPx = " << recoJet_matched->px() << ", recoPy = " << recoJet_matched->py();
      std::cout << ")" << std::endl;
/*
      std::cout << "genJet constituents:" << std::endl;
      std::vector<const reco::GenParticle*> genJetConstituents = genJet->getGenConstituents();
      for ( std::vector<const reco::GenParticle*>::const_iterator genJetConstituent = genJetConstituents.begin();
            genJetConstituent != genJetConstituents.end(); ++genJetConstituent ) {
        std::cout << " pdgId = " << (*genJetConstituent)->pdgId() << ", status = " << (*genJetConstituent)->status() << ":"
	          << " Pt = " << (*genJetConstituent)->pt() << ", eta = " << (*genJetConstituent)->eta() << "," 
		  << " phi = " << (*genJetConstituent)->phi()*180./TMath::Pi() << std::endl; 
      }      
 */
    }

    recoJets_matched.push_back(recoJet_matched);
  }
  
  for ( edm::View<reco::Jet>::const_iterator recoJet = recoJets->begin(); 
	recoJet != recoJets->end(); ++recoJet ) {
    
    if ( !(recoJet->pt() > maxPtDifference) ) continue;
    
    bool isMatched = false;
    
    for ( std::vector<const reco::Jet*>::const_iterator recoJet_matched = recoJets_matched.begin(); 
	  recoJet_matched != recoJets_matched.end(); ++recoJet_matched ) {
      if ( &(*recoJet) == (*recoJet_matched) ) isMatched = true;
    }
    
    if ( !isMatched ) {
      std::cout << "recoJet not matching (any) genJet: Pt = " << recoJet->pt() << ", eta = " << recoJet->eta() << "," 
		<< " phi = " << recoJet->phi()*180./TMath::Pi() << std::endl; 
      std::cout << "(recoPx = " << recoJet->px() << ", recoPy = " << recoJet->py() << ")" << std::endl;
    }
  }
}

void GenericEventDump::printMissingEtInfo(const edm::Event& evt) const
{
//--- print-out PAT/reco missing Et information

  if ( !outputStream_ ) {
    edm::LogError ("printMissingEtInfo") << " outputStream = NULL --> skipping !!";
    return;
  }
  
  printMissingEtInfo_i(evt, patCaloMEtSource_, *outputStream_, "recoCaloMET:");
  printMissingEtInfo_i(evt, patPFMEtSource_, *outputStream_, "recoPFMET:");
  
  if ( doGenInfo_ ) {
    
    printJetMatchingInfo_i(evt, edm::InputTag("ak5PFJets"), genJetSource_, *outputStream_, "PFJet");
    printJetMatchingInfo_i(evt, edm::InputTag("ak5CaloJets"), genJetSource_, *outputStream_, "CaloJet");
    
    edm::Handle<edm::View<reco::GenParticle> > genParticleCollection;
    evt.getByLabel(genParticleSource_, genParticleCollection);
    
    reco::Candidate::LorentzVector genNeutrinos(0,0,0,0);
    
    for ( edm::View<reco::GenParticle>::const_iterator genParticle = genParticleCollection->begin(); 
	  genParticle != genParticleCollection->end(); ++genParticle ) {
      
      if ( genParticle->status() == 1 && isNeutrino(&(*genParticle)) )genNeutrinos += genParticle->p4();
    }
    
    *outputStream_ << "sum(gen. Neutrinos):" 
		   << " Et = " << genNeutrinos.pt() << "," 
		   << " phi = " <<  genNeutrinos.phi()*180./TMath::Pi() 
		   << " (Px = " << genNeutrinos.px() << ", Py = " << genNeutrinos.py() << ")" << std::endl;
    
    reco::Candidate::LorentzVector invisibleHighEtaGenJets(0,0,0,0);
    
    edm::Handle<reco::GenJetCollection> genJets;
    evt.getByLabel(genJetSource_, genJets);
    
    for ( reco::GenJetCollection::const_iterator genJet = genJets->begin();
	  genJet != genJets->end(); ++genJet ) {
      if ( TMath::Abs(genJet->eta()) > 5.0 ) invisibleHighEtaGenJets += genJet->p4();
    }
    
    *outputStream_ << "sum(gen. Jets @ |eta| > 5.0):" 
		   << " Et = " << invisibleHighEtaGenJets.pt() << "," 
		   << " phi = " <<  invisibleHighEtaGenJets.phi()*180./TMath::Pi() 
		   << " (Px = " << invisibleHighEtaGenJets.px() << ", Py = " << invisibleHighEtaGenJets.py() << ")" << std::endl;
    
    reco::Candidate::LorentzVector invisibleHighEtaGenParticles(0,0,0,0);
    
    for ( edm::View<reco::GenParticle>::const_iterator genParticle = genParticleCollection->begin(); 
	  genParticle != genParticleCollection->end(); ++genParticle ) {
      
      if ( genParticle->status() == 1 && TMath::Abs(genParticle->eta()) > 5.0 ) invisibleHighEtaGenParticles += genParticle->p4();
    }
    
    *outputStream_ << "sum(gen. Particles @ |eta| > 5.0):" 
		   << " Et = " << invisibleHighEtaGenParticles.pt() << "," 
		   << " phi = " <<  invisibleHighEtaGenParticles.phi()*180./TMath::Pi() 
		   << " (Px = " << invisibleHighEtaGenParticles.px() << ", Py = " << invisibleHighEtaGenParticles.py() << ")" << std::endl;
    
    *outputStream_ << std::endl;
  }
}

