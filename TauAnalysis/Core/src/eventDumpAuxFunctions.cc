#include "TauAnalysis/Core/interface/eventDumpAuxFunctions.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/TriggerNames.h"

#include "DataFormats/Common/interface/RefToBase.h"
#include "DataFormats/TrackReco/interface/HitPattern.h"

#include "PhysicsTools/JetMCUtils/interface/JetMCTag.h"
#include "PhysicsTools/Utilities/interface/deltaR.h"

#include "TauAnalysis/GenSimTools/interface/genParticleAuxFunctions.h"

#include <TMath.h>

void printEventHeaderInfo(const edm::Event& iEvent, double eventWeight, std::ostream* stream)
{
  if ( !stream ) {
    edm::LogError ("printEventHeaderInfo") << " stream = NULL --> skipping !!";
    return;
  }

  edm::RunNumber_t runNumber = iEvent.id().run();
  edm::EventNumber_t eventNumber = iEvent.id().event();

  *stream << "Run: " << runNumber << std::endl;
  *stream << "Event: " << eventNumber << std::endl;
  *stream << std::endl;
  *stream << " weight = " << eventWeight << std::endl;
  *stream << std::endl;
}

void printEventTriggerInfo(edm::Handle<edm::TriggerResults>& triggerResults, std::vector<std::string> triggerPathsToPrint, std::ostream* stream)
{
  if ( !stream ) {
    edm::LogError ("printEventTriggerInfo") << " stream = NULL --> skipping !!";
    return;
  }

  edm::TriggerNames triggerNames;
  triggerNames.init(*triggerResults);

  //for ( edm::TriggerNames::Strings::const_iterator triggerName = triggerNames.triggerNames().begin();
  //	  triggerName != triggerNames.triggerNames().end(); ++triggerName ) {
  //  unsigned int index = triggerNames.triggerIndex(*triggerName);
  //  if ( index < triggerNames.size() ) {
  //    std::string triggerDecision = ( triggerResults->accept(index) ) ? "passed" : "failed";
  //    
  //    std::cout << " triggerName = " << (*triggerName) << " " << triggerDecision << std::endl;
  //  }
  //}

  *stream << "triggerDecisions:" << std::endl;

  for ( std::vector<std::string>::const_iterator triggerPath = triggerPathsToPrint.begin();
	triggerPath != triggerPathsToPrint.end(); ++triggerPath ) {
    unsigned int index = triggerNames.triggerIndex(*triggerPath);
    if ( index < triggerNames.size() ) {
      std::string triggerDecision = ( triggerResults->accept(index) ) ? "passed" : "failed";

      *stream << " " << (*triggerPath) << " " << triggerDecision << std::endl;
    } else {
      edm::LogError ("printEventTriggerInfo") << " Undefined trigger Path = " << (*triggerPath) << " --> skipping !!";
      continue;
    }
  }

  *stream << std::endl;
}

void printEventSelectionInfo(const std::vector<std::pair<std::string, bool> >& filterResults_cumulative, 
			     const std::vector<std::pair<std::string, bool> >& filterResults_individual, std::ostream* stream)
{
  if ( !stream ) {
    edm::LogError ("printSelectionInfo") << " stream = NULL --> skipping !!";
    return;
  }

  *stream << "filterResults:" << std::endl;

  for ( std::vector<std::pair<std::string, bool> >::const_iterator filterResult_cumulative = filterResults_cumulative.begin();
	filterResult_cumulative != filterResults_cumulative.end(); ++filterResult_cumulative ) {
    const std::string& filterName = filterResult_cumulative->first;
    bool filterPassed_cumulative = filterResult_cumulative->second;

    std::string filterDecision = ( filterPassed_cumulative ) ? "passed" : "FAILED";
 
    *stream << " " << filterName << " " << filterDecision << std::endl;
  }

  *stream << std::endl;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void printGenParticleInfo(edm::Handle<edm::View<reco::GenParticle> >& genParticles,
			  edm::Handle<edm::View<reco::GenJet> >& genTauJets,
			  std::ostream* stream)
{
  if ( !stream ) {
    edm::LogError ("printGenParticleInfo") << " stream = NULL --> skipping !!";
    return;
  }

  reco::Particle::LorentzVector genMissingTransverseMomentum = getGenMissingTransverseMomentum(genParticles);
  *stream << "missing transverse momentum:" 
	  << " Pt = " << genMissingTransverseMomentum.pt() << "," 
	  << " phi = " <<  genMissingTransverseMomentum.phi() << std::endl;
  *stream << std::endl;

  for ( edm::View<reco::GenParticle>::const_iterator genParticle = genParticles->begin(); 
	genParticle != genParticles->end(); ++genParticle ) {
    const reco::Particle::LorentzVector& genParticleMomentum = genParticle->p4();
    int pdgId = genParticle->pdgId();
    int status = genParticle->status();

//--- skip "documentation line" entries
//    (copied over to reco::GenParticle from HepMC product)
    if ( status == 3 ) continue;
    
    if ( pdgId == -15 || pdgId == +15 ) {
      *stream << "tau:" 
	      << " pdgId = " << pdgId << "," 
	      << " status = " << status << "," 
	      << " Pt = " << genParticleMomentum.pt() << "," 
	      << " eta = " << genParticleMomentum.eta() << "," 
	      << " phi = " << genParticleMomentum.phi() << std::endl;

//--- find genTauJet associated with generated tau 
//    (match by closest distance in eta-phi)
      double dRmin = 0.;
      edm::View<reco::GenJet>::const_iterator genTauJet_matched = genTauJets->end();
      for ( edm::View<reco::GenJet>::const_iterator genTauJet = genTauJets->begin(); 
	    genTauJet != genTauJets->end(); ++genTauJet ) {
	const reco::Particle::LorentzVector& genTauJetMomentum = genTauJet->p4();

	double dR = reco::deltaR(genParticleMomentum, genTauJetMomentum);
	if ( dR < dRmin || genTauJet_matched == genTauJets->end() ) {
	  genTauJet_matched = genTauJet;
	  dRmin = dR;
	}
      }

      if ( genTauJet_matched == genTauJets->end() ) {
	edm::LogError ("printGenParticleInfo") << " Failed to find matching TauGenJet --> skipping !!";
	continue;
      }
      
//--- find tau lepton decay products in list of generated particles
      reco::GenParticleCollection genDecayProducts = getStableDecayProducts(genParticles, *genParticle);
      for ( reco::GenParticleCollection::const_iterator genDecayProduct = genDecayProducts.begin(); 
	    genDecayProduct != genDecayProducts.end(); ++genDecayProduct ) {
	*stream << "tauDecayProduct:" 
		<< " pdgId = " << genDecayProduct->pdgId() << "," 
		<< " status = " << genDecayProduct->status() << "," 
		<< " Pt = " << genDecayProduct->pt() << "," 
		<< " eta = " << genDecayProduct->eta() << "," 
		<< " phi = " <<  genDecayProduct->phi() << std::endl;
      }

//--- reconstruct tau lepton decay mode
      std::string tauDecayMode = JetMCTagUtils::genTauDecayMode(*genTauJet_matched);
      *stream << "tauDecayMode = " << tauDecayMode << std::endl;

//--- reconstruct visible momentum of decay products
      reco::Particle::LorentzVector genVisibleTauMomentum = getGenVisibleTauMomentum(*genTauJet_matched);
      if ( tauDecayMode == "oneProng0Pi0"    || 
	   tauDecayMode == "oneProng1Pi0"    || 
	   tauDecayMode == "oneProng2Pi0"    || 
	   tauDecayMode == "oneProngOther"   ||
	   tauDecayMode == "threeProng0Pi0"  || 
	   tauDecayMode == "threeProng1Pi0"  || 
	   tauDecayMode == "threeProngOther" ||
	   tauDecayMode == "rare" ) {
	*stream << "visible tau-jet:" 
		<< " Pt = " << genVisibleTauMomentum.pt() << "," 
		<< " eta = " << genVisibleTauMomentum.eta() << "," 
		<< " phi = " <<  genVisibleTauMomentum.phi() << std::endl;
      }
      
      *stream << std::endl;
    }
  }
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void printTrackInfo(const edm::RefToBase<reco::Track>& track, std::ostream* stream)
{
  if ( track.isAvailable() && !track.isNull() ) {
    *stream << "  Pt = " << track->pt() << std::endl;
    *stream << "  d0 = " << track->d0() << std::endl;
  } else {
    *stream << "  none." << std::endl;
  }
}

void printVertexInfo(const reco::Candidate::Point& vertex, std::ostream* stream)
{
  *stream << "  x = " << vertex.x() << std::endl;
  *stream << "  y = " << vertex.y() << std::endl;
  *stream << "  z = " << vertex.z() << std::endl;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void printTrackIsolationInfo(const edm::Handle<reco::TrackCollection>& recoTracks, 
			     const reco::Candidate::Vector& direction, double coneSize,
			     std::ostream* stream)
{
  if ( !stream ) {
    edm::LogError ("printTrackIsolationInfo") << " stream = NULL --> skipping !!";
    return;
  }
  
  unsigned iTrack = 0;
  for ( reco::TrackCollection::const_iterator recoTrack = recoTracks->begin(); 
	recoTrack != recoTracks->end(); ++recoTrack ) {
    double dR = reco::deltaR(*recoTrack, direction);
    if ( dR < coneSize ) {
      *stream << "Track(" << iTrack << "):" << std::endl;
      *stream << " Pt = " << recoTrack->pt() << std::endl;
      *stream << " theta = " << recoTrack->theta()*180./TMath::Pi() << std::endl;
      *stream << " phi = " << recoTrack->phi()*180./TMath::Pi() << std::endl;
      *stream << " Hits" << std::endl;
      const reco::HitPattern& hitPattern = recoTrack->hitPattern();
      *stream << "  numberOfValidTrackerHits = " << hitPattern.numberOfValidTrackerHits() << std::endl;
      *stream << "   numberOfValidPixelHits = " << hitPattern.numberOfValidPixelHits() << std::endl;
      *stream << "   numberOfValidStripHits = " << hitPattern.numberOfValidStripHits() << std::endl;
      *stream << "  trackerLayersWithMeasurement = " << hitPattern.trackerLayersWithMeasurement() << std::endl;
      *stream << "   pixelLayersWithMeasurement = " << hitPattern.pixelLayersWithMeasurement() << std::endl;
      *stream << "   stripLayersWithMeasurement = " << hitPattern.stripLayersWithMeasurement() << std::endl;
      ++iTrack;
    }
  }

  *stream << std::endl;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void printElectronInfo(edm::Handle<pat::ElectronCollection>& patElectrons, std::ostream* stream)
{
  if ( !stream ) {
    edm::LogError ("printElectronInfo") << " stream = NULL --> skipping !!";
    return;
  }

  unsigned iElectron = 0;
  for ( pat::ElectronCollection::const_iterator patElectron = patElectrons->begin(); 
	patElectron != patElectrons->end(); ++patElectron ) {
    *stream << "Electron(" << iElectron << "):" << std::endl;
    *stream << " Pt = " << patElectron->pt() << std::endl;
    *stream << " theta = " << patElectron->theta()*180./TMath::Pi() << " (eta = " << patElectron->eta() << ")" << std::endl;
    *stream << " phi = " << patElectron->phi()*180./TMath::Pi() << std::endl;
    *stream << " Track" << std::endl;
    printTrackInfo(edm::RefToBase<reco::Track>(patElectron->track()), stream);
    *stream << " gsf Track" << std::endl;
    printTrackInfo(edm::RefToBase<reco::Track>(patElectron->gsfTrack()), stream);
    *stream << " vertex" << std::endl;
    printVertexInfo(patElectron->vertex(), stream);
    ++iElectron;
  }

  *stream << std::endl;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void printMuonInfo(edm::Handle<pat::MuonCollection>& patMuons, std::ostream* stream)
{
  if ( !stream ) {
    edm::LogError ("printMuonInfo") << " stream = NULL --> skipping !!";
    return;
  }

  unsigned iMuon = 0;
  for ( pat::MuonCollection::const_iterator patMuon = patMuons->begin(); 
	patMuon != patMuons->end(); ++patMuon ) {
    *stream << "Muon(" << iMuon << "):" << std::endl;
    *stream << " Pt = " << patMuon->pt() << std::endl;
    *stream << " theta = " << patMuon->theta()*180./TMath::Pi() << " (eta = " << patMuon->eta() << ")" << std::endl;
    *stream << " phi = " << patMuon->phi()*180./TMath::Pi() << std::endl;
    *stream << " inner Track" << std::endl;
    printTrackInfo(edm::RefToBase<reco::Track>(patMuon->innerTrack()), stream);
    *stream << " outer Track" << std::endl;
    printTrackInfo(edm::RefToBase<reco::Track>(patMuon->outerTrack()), stream);
    *stream << " global Track" << std::endl;
    printTrackInfo(edm::RefToBase<reco::Track>(patMuon->globalTrack()), stream);
    *stream << " trackIso = " << patMuon->trackIso() << std::endl;
    /*    
    void printTrackIsolationInfo(const edm::Handle<reco::TrackCollection>& recoTracks, 
			     const reco::Candidate::Vector& direction, double coneSize,
			     std::ostream* stream)
    */
    *stream << " caloIso = " << patMuon->caloIso() << std::endl;
    *stream << " ecalIso = " << patMuon->ecalIso() << std::endl;
    *stream << " hcalIso = " << patMuon->hcalIso() << std::endl;
    *stream << " vertex" << std::endl;
    printVertexInfo(patMuon->vertex(), stream);
    ++iMuon; 
  }

  *stream << std::endl;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void printTauInfo(edm::Handle<pat::TauCollection>& patTaus, std::ostream* stream)
{
  if ( !stream ) {
    edm::LogError ("printTauInfo") << " stream = NULL --> skipping !!";
    return;
  }

  unsigned iTau = 0;
  for ( pat::TauCollection::const_iterator patTau = patTaus->begin(); 
	patTau != patTaus->end(); ++patTau ) {
    *stream << "Tau(" << iTau << "):" << std::endl;
    *stream << " Et = " << patTau->et() << std::endl;
    *stream << " theta = " << patTau->theta()*180./TMath::Pi() << " (eta = " << patTau->eta() << ")" << std::endl;
    *stream << " phi = " << patTau->phi()*180./TMath::Pi() << std::endl;
    *stream << " leading Track" << std::endl;
    printTrackInfo(edm::RefToBase<reco::Track>(patTau->leadTrack()), stream);
    *stream << " #signal Tracks = " << patTau->signalTracks().size() << std::endl;
    *stream << " tauId" << std::endl;
    *stream << "  leadingTrackFinding = " << patTau->tauID("leadingTrackFinding") << std::endl;
    *stream << "  leadingTrackPtCut = " << patTau->tauID("leadingTrackPtCut") << std::endl;
    *stream << "  trackIsolation = " << patTau->tauID("trackIsolation") << std::endl;
    *stream << "  ecalIsolation = " << patTau->tauID("ecalIsolation") << std::endl;
    *stream << " eVeto = " << patTau->tauID("againstElectron") << std::endl;
    *stream << " muVeto = " << patTau->tauID("againstMuon") << std::endl;
    *stream << " vertex" << std::endl;
    printVertexInfo(patTau->vertex(), stream);
    ++iTau;
  }

  *stream << std::endl;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void printMissingEtInfo(edm::Handle<pat::METCollection>& patMETs, std::ostream* stream)
{
//--- print-out PAT/reco missing Et information
  
  if ( !stream ) {
    edm::LogError ("printMissingEtInfo") << " stream = NULL --> skipping !!";
    return;
  }

  for ( pat::METCollection::const_iterator patMET = patMETs->begin(); 
	patMET != patMETs->end(); ++patMET ) {
    
    *stream << "PAT MET:" 
	    << " Et = " << patMET->pt() << "," 
	    << " phi = " <<  patMET->phi() << std::endl;
    *stream << " isCaloMET = " << patMET->isCaloMET() << std::endl;

    if ( patMET->genMET() != NULL ) {
      const reco::GenMET* genMET = patMET->genMET();

      *stream << "genMET:" 
	      << " Et = " << genMET->pt() << "," 
	      << " phi = " <<  genMET->phi() << std::endl;
      
    } else {
      *stream << "no genMET associated to PAT MET !!" << std::endl;
    }
  }

  *stream << std::endl;
}

