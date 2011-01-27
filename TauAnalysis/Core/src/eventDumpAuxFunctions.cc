#include "TauAnalysis/Core/interface/eventDumpAuxFunctions.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/TrackReco/interface/HitPattern.h"

#include "PhysicsTools/JetMCUtils/interface/JetMCTag.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "TauAnalysis/GenSimTools/interface/genParticleAuxFunctions.h"
#include "TauAnalysis/CandidateTools/interface/candidateAuxFunctions.h"

#include <TMath.h>

#include <fstream>

std::ostream* getOutputOptions(const edm::ParameterSet& cfg, bool& isOutputFile, int& error)
{
  //std::cout << "<getOutputOptions>:" << std::endl;

  std::ostream* outputStream = 0;

  if ( cfg.exists("output") ) {
    std::string output = cfg.getParameter<std::string>("output");
    //std::cout << " output = " << output << std::endl;

    if ( output == "std::cout" ) {
      //std::cout << "--> setting outputStream to std::cout." << std::endl;
      outputStream = &std::cout;
      isOutputFile = false;
    } else if ( output == "std::cerr" ) {
      //std::cout << "--> setting outputStream to std::cerr." << std::endl;
      outputStream = &std::cerr;
      isOutputFile = false;
    } else if ( output != "" ) {
      //std::cout << "--> setting outputStream to std::ofstream." << std::endl;
      outputStream = new std::ofstream(output.data(), std::ios::out);
      isOutputFile = true;
    } else {
      edm::LogError ("getOutputOptions") 
	<< " Invalid Configuration Parameter output = " << output << " --> skipping !!";
      error = 1;
    }
  } else {
    //std::cout << " no outputFileName specified in configuration parameters" << std::endl;
    //std::cout << "--> setting outputStream to std::cout." << std::endl;
    
    outputStream = &std::cout;
    isOutputFile = false;
  }

  return outputStream;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void printEventSelectionInfo(const GenericAnalyzer_namespace::filterResults_type& filterResults_cumulative, 
			     const GenericAnalyzer_namespace::filterResults_type& filterResults_individual, std::ostream* stream)
{
  if ( !stream ) {
    edm::LogError ("printSelectionInfo") << " stream = NULL --> skipping !!";
    return;
  }

  *stream << "filterResults:" << std::endl;

  for ( GenericAnalyzer_namespace::filterResults_type::const_iterator filterResult_cumulative = filterResults_cumulative.begin();
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

void printGenParticleInfo(const reco::GenParticleCollection& genParticles,
			  const reco::GenJetCollection& genTauJets,
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

  for ( reco::GenParticleCollection::const_iterator genParticle = genParticles.begin(); 
	genParticle != genParticles.end(); ++genParticle ) {
    const reco::Particle::LorentzVector& genParticleMomentum = genParticle->p4();
    int pdgId = genParticle->pdgId();
    int status = genParticle->status();

//--- skip "documentation line" entries
//    (copied over to reco::GenParticle from HepMC product)
    if ( status == 3 ) continue;
    
    std::string charge = ( genParticle->charge() > 0. ) ? "+" : "-";
    
    if ( pdgId == -11 || pdgId == +11 ) {
      *stream << "electron" << charge << ":"
	      //<< " pdgId = " << pdgId << "," 
	      << " status = " << status << "," 
	      << " Pt = " << genParticleMomentum.pt() << "," 
	      << " eta = " << genParticleMomentum.eta() << "," 
	      << " phi = " << genParticleMomentum.phi()*180./TMath::Pi() << std::endl;
    }

    if ( pdgId == -13 || pdgId == +13 ) {
      *stream << "muon" << charge << ":"
	      //<< " pdgId = " << pdgId << "," 
	      << " status = " << status << "," 
	      << " Pt = " << genParticleMomentum.pt() << "," 
	      << " eta = " << genParticleMomentum.eta() << "," 
	      << " phi = " << genParticleMomentum.phi()*180./TMath::Pi() << std::endl;
    }

    if ( pdgId == -15 || pdgId == +15 ) {
      *stream << "tau" << charge << ":"
	      //<< " pdgId = " << pdgId << "," 
	      << " status = " << status << "," 
	      << " Pt = " << genParticleMomentum.pt() << "," 
	      << " eta = " << genParticleMomentum.eta() << "," 
	      << " phi = " << genParticleMomentum.phi()*180./TMath::Pi() << std::endl;
      *stream << " (prod. vertex: x = " << genParticle->vertex().x() << ","
	      << " y = " << genParticle->vertex().y() << ", z = " << genParticle->vertex().z() << ")" << std::endl;

//--- find genTauJet associated with generated tau 
//    (match by closest distance in eta-phi)
      double dRmin = 0.;
      reco::GenJetCollection::const_iterator genTauJet_matched = genTauJets.end();
      for ( reco::GenJetCollection::const_iterator genTauJet = genTauJets.begin(); 
	    genTauJet != genTauJets.end(); ++genTauJet ) {
	const reco::Particle::LorentzVector& genTauJetMomentum = genTauJet->p4();

	double dR = reco::deltaR(genParticleMomentum, genTauJetMomentum);
	if ( dR < dRmin || genTauJet_matched == genTauJets.end() ) {
	  genTauJet_matched = genTauJet;
	  dRmin = dR;
	}
      }

      if ( genTauJet_matched == genTauJets.end() ) {
	edm::LogError ("printGenParticleInfo") << " Failed to find matching TauGenJet --> skipping !!";
	continue;
      }
      
//--- find tau lepton decay products in list of generated particles
      reco::GenParticleCollection genDecayProducts = getStableDecayProducts(*genParticle);
      for ( reco::GenParticleCollection::const_iterator genDecayProduct = genDecayProducts.begin(); 
	    genDecayProduct != genDecayProducts.end(); ++genDecayProduct ) {
	*stream << "tauDecayProduct:" 
		<< " pdgId = " << genDecayProduct->pdgId() << "," 
		<< " status = " << genDecayProduct->status() << "," 
		<< " Pt = " << genDecayProduct->pt() << "," 
		<< " eta = " << genDecayProduct->eta() << "," 
		<< " phi = " <<  genDecayProduct->phi()*180./TMath::Pi() << std::endl;
	if ( TMath::Abs(genDecayProduct->charge()) > 0.5 )
	  *stream << " (prod. vertex: x = " << genDecayProduct->vertex().x() << ","
		  << " y = " << genDecayProduct->vertex().y() << ", z = " << genDecayProduct->vertex().z() << ")" << std::endl;
      }

//--- reconstruct tau lepton decay mode
      std::string tauDecayMode = JetMCTagUtils::genTauDecayMode(*genTauJet_matched);
      *stream << "tauDecayMode = " << tauDecayMode << std::endl;

//--- reconstruct visible momentum of decay products
      reco::Particle::LorentzVector genVisibleTauMomentum = getVisMomentum(&(*genParticle), &genParticles);
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
		<< " phi = " <<  genVisibleTauMomentum.phi()*180./TMath::Pi() 
		<< " mass = " << genVisibleTauMomentum.mass() << std::endl;
      }

      *stream << "neutrino mass = " << getInvisMomentum(&(*genParticle), &genParticles);
      
      *stream << std::endl;
    }
  }
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void printVertexInfo(const reco::Candidate::Point& vertex, std::ostream* stream)
{
  *stream << "  x = " << vertex.x() << std::endl;
  *stream << "  y = " << vertex.y() << std::endl;
  *stream << "  z = " << vertex.z() << std::endl;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void printTrackHitPatternInfo(const reco::TrackRef& recoTrack, std::ostream* stream)
{
  if ( isValidRef(recoTrack) ) {
    const reco::HitPattern& hitPattern = recoTrack->hitPattern();

    int numHits = hitPattern.numberOfHits();
    for ( int iHit = 0; iHit < numHits; ++iHit ) {
      if ( stream ) hitPattern.printHitPattern(iHit, *stream);
    }
  }
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

