#include "TauAnalysis/Core/interface/eventDumpAuxFunctions.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/TriggerNames.h"

#include "DataFormats/Common/interface/RefToBase.h"
#include "DataFormats/TrackReco/interface/HitPattern.h"

#include "PhysicsTools/JetMCUtils/interface/JetMCTag.h"
#include "PhysicsTools/Utilities/interface/deltaR.h"

#include "TauAnalysis/GenSimTools/interface/genParticleAuxFunctions.h"

#include <TMath.h>

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
	  << " phi = " <<  genMissingTransverseMomentum.phi()*180./TMath::Pi() << std::endl;
  *stream << std::endl;

  for ( edm::View<reco::GenParticle>::const_iterator genParticle = genParticles->begin(); 
	genParticle != genParticles->end(); ++genParticle ) {
    const reco::Particle::LorentzVector& genParticleMomentum = genParticle->p4();
    int pdgId = genParticle->pdgId();
    int status = genParticle->status();

//--- skip "documentation line" entries
//    (copied over to reco::GenParticle from HepMC product)
    if ( status == 3 ) continue;
    
    if ( pdgId == -11 || pdgId == +11 ) {
      *stream << "electron:" 
	      << " pdgId = " << pdgId << "," 
	      << " status = " << status << "," 
	      << " Pt = " << genParticleMomentum.pt() << "," 
	      << " eta = " << genParticleMomentum.eta() << "," 
	      << " phi = " << genParticleMomentum.phi()*180./TMath::Pi() << std::endl;
    }

    if ( pdgId == -13 || pdgId == +13 ) {
      *stream << "muon:" 
	      << " pdgId = " << pdgId << "," 
	      << " status = " << status << "," 
	      << " Pt = " << genParticleMomentum.pt() << "," 
	      << " eta = " << genParticleMomentum.eta() << "," 
	      << " phi = " << genParticleMomentum.phi()*180./TMath::Pi() << std::endl;
    }

    if ( pdgId == -15 || pdgId == +15 ) {
      *stream << "tau:" 
	      << " pdgId = " << pdgId << "," 
	      << " status = " << status << "," 
	      << " Pt = " << genParticleMomentum.pt() << "," 
	      << " eta = " << genParticleMomentum.eta() << "," 
	      << " phi = " << genParticleMomentum.phi()*180./TMath::Pi() << std::endl;

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
		<< " phi = " <<  genDecayProduct->phi()*180./TMath::Pi() << std::endl;
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
		<< " phi = " <<  genVisibleTauMomentum.phi()*180./TMath::Pi() << std::endl;
      }
      
      *stream << std::endl;
    }
  }
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void printTrackInfo(const edm::RefToBase<reco::Track>& track, const reco::Candidate::Point& vertex, bool printDxy, bool printDz,
		    std::ostream* stream)
{
  if ( track.isAvailable() && track.isNonnull() ) {
    *stream << "  Pt = " << track->pt() << std::endl;
    if ( printDxy ) *stream << "  dXY = " << track->dxy(vertex) << std::endl;
    if ( printDz  ) *stream << "  dZ = " << track->dz(vertex) << std::endl;
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
			     const reco::Candidate::Vector& direction, double vetoConeSize, double isolationConeSize, double minPt,
			     const reco::Vertex::Point& thePrimaryEventVertexPosition,
			     std::ostream* stream)
{
  if ( !stream ) {
    edm::LogError ("printTrackIsolationInfo") << " stream = NULL --> skipping !!";
    return;
  }
  
  unsigned iTrack = 0;
  for ( reco::TrackCollection::const_iterator recoTrack = recoTracks->begin(); 
	recoTrack != recoTracks->end(); ++recoTrack ) {
    if ( recoTrack->pt() > minPt ) {
      double dR = reco::deltaR(*recoTrack, direction);
      if ( dR > vetoConeSize      &&
	   dR < isolationConeSize ) {
	*stream << "Track(" << iTrack << "):" << std::endl;
	*stream << " dR = " << dR << std::endl;
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
	*stream << " dXY = " << recoTrack->dxy(thePrimaryEventVertexPosition) << std::endl;
	*stream << " dZ = " << recoTrack->dz(thePrimaryEventVertexPosition) << std::endl;
	++iTrack;
      }
    }
  }
}

void printPFCandidateIsolationInfo(const edm::Handle<reco::PFCandidateCollection>& pfCandidates, std::string pfCandidateLabel,
				   const reco::Candidate::Vector& direction, double vetoConeSize, double isolationConeSize, double minPt,
				   std::ostream* stream)
{
  if ( !stream ) {
    edm::LogError ("printPFCandidateIsolationInfo") << " stream = NULL --> skipping !!";
    return;
  }
  
  unsigned iPFCandidate = 0;
  for ( reco::PFCandidateCollection::const_iterator pfCandidate = pfCandidates->begin(); 
	pfCandidate != pfCandidates->end(); ++pfCandidate ) {
    if ( pfCandidate->pt() > minPt ) {
      double dR = reco::deltaR(*pfCandidate, direction);
      if ( dR > vetoConeSize      &&
	   dR < isolationConeSize ) {
	*stream << pfCandidateLabel << "(" << iPFCandidate << "):" << std::endl;
	*stream << " dR = " << dR << std::endl;
	*stream << " Pt = " << pfCandidate->pt() << std::endl;
	*stream << " theta = " << pfCandidate->theta()*180./TMath::Pi() << std::endl;
	*stream << " phi = " << pfCandidate->phi()*180./TMath::Pi() << std::endl;	
	++iPFCandidate;
      }
    }
  }
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

