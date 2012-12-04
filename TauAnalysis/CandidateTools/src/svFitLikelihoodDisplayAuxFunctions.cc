#include "TauAnalysis/CandidateTools/interface/svFitLikelihoodDisplayAuxFunctions.h"

#include "FWCore/Utilities/interface/Exception.h"

#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/TauReco/interface/PFTau.h"
#include "DataFormats/TauReco/interface/PFTauFwd.h"
#include "DataFormats/TauReco/interface/PFTauDecayMode.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/Math/interface/angle.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "TauAnalysis/CandidateTools/interface/candidateAuxFunctions.h"
#include "TauAnalysis/CandidateTools/interface/SVfitTrackExtrapolation.h"
#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"

#include <TMath.h>

#include <iomanip>

using namespace SVfit_namespace;

namespace
{
  bool isHigherPt(const reco::Track* track1, const reco::Track* track2)
  {
    return (track1->pt() > track2->pt());
  }

  reco::Candidate::LorentzVector compFlightP4(const AlgebraicVector3& decayVertex, const AlgebraicVector3& prodVertex, double p, double motherMass)
  {
    double pxFlight = decayVertex(0) - prodVertex(0);
    double pyFlight = decayVertex(1) - prodVertex(1);
    double pzFlight = decayVertex(2) - prodVertex(2);
    double pFlight  = TMath::Sqrt(square(pxFlight) + square(pyFlight) + square(pzFlight));
    pxFlight *= (p/pFlight);
    pyFlight *= (p/pFlight);
    pzFlight *= (p/pFlight);
    pFlight   = p;
    double enFlight = TMath::Sqrt(square(pFlight) + square(motherMass));
    reco::Candidate::LorentzVector p4Flight(pxFlight, pyFlight, pzFlight, enFlight);
    return p4Flight;
  }

  std::pair<reco::Candidate::LorentzVector, reco::Candidate::LorentzVector> 
  reconstructTauNeutrino(double motherTheta, double motherPhi, double motherMass, const reco::Candidate::LorentzVector& p4Vis, double invisMass, int& errorFlag)
  {
    double motherMass2 = square(motherMass);
    double visMass = p4Vis.mass();
    double visMass2 = square(visMass);
    double visEn = p4Vis.E();
    double visEn2 = square(visEn);
    
    double invisMass2 = square(invisMass);
    
    double cosMotherTheta = TMath::Cos(motherTheta);
    double sinMotherTheta = TMath::Sin(motherTheta);
    double cosMotherPhi   = TMath::Cos(motherPhi);
    double sinMotherPhi   = TMath::Sin(motherPhi);
    reco::Candidate::Vector u_parl = normalize(reco::Candidate::Vector(cosMotherPhi*sinMotherTheta, sinMotherPhi*sinMotherTheta, cosMotherTheta));
    reco::Candidate::Vector p3Vis(p4Vis.px(), p4Vis.py(), p4Vis.pz());
    reco::Candidate::Vector p3Vis_normalized = normalize(p3Vis);
    
    reco::Candidate::Vector normal_to_plane = normalize(compCrossProduct(u_parl, p3Vis_normalized));
    
    reco::Candidate::Vector u_perp = compCrossProduct(normal_to_plane, u_parl);
    
    double pVis_parl = SVfit_namespace::compScalarProduct(p3Vis, u_parl);
    double pVis_parl2 = square(pVis_parl);
    double pVis_perp = SVfit_namespace::compScalarProduct(p3Vis, u_perp);
    double pVis_perp2 = square(pVis_perp);
    
    double a = (motherMass2 - visMass2 - 2.*pVis_perp2 - invisMass2)*pVis_parl/(2.*(pVis_parl2 - visEn2));
    double r = square(motherMass2 - visMass2 - 2.*pVis_perp2 - invisMass2)*visEn2/(4.*square(pVis_parl2 - visEn2)) + visEn2*(pVis_perp2 + invisMass2)/(pVis_parl2 - visEn2);
    if ( r < 0. ) {
      r = 0.;
      errorFlag = 1;    
    }
    
    double pInvis_parl_plus = -a + TMath::Sqrt(r);
    double pInvis_parl_minus = -a - TMath::Sqrt(r);
    double pInvis_perp = -pVis_perp;
    
    double pxInvis_plus = pInvis_parl_plus*u_parl.x() + pInvis_perp*u_perp.x();
    double pyInvis_plus = pInvis_parl_plus*u_parl.y() + pInvis_perp*u_perp.y();
    double pzInvis_plus = pInvis_parl_plus*u_parl.z() + pInvis_perp*u_perp.z();
    double pInvis_plus2 = square(pxInvis_plus) + square(pyInvis_plus) + square(pzInvis_plus); 
    double enInvis_plus = TMath::Sqrt(pInvis_plus2 + invisMass2);
    reco::Candidate::LorentzVector p4Invis_plus(pxInvis_plus, pyInvis_plus, pzInvis_plus, enInvis_plus);
    
    double pxInvis_minus = pInvis_parl_minus*u_parl.x() + pInvis_perp*u_perp.x();
    double pyInvis_minus = pInvis_parl_minus*u_parl.y() + pInvis_perp*u_perp.y();
    double pzInvis_minus = pInvis_parl_minus*u_parl.z() + pInvis_perp*u_perp.z();
    double pInvis_minus2 = square(pxInvis_minus) + square(pyInvis_minus) + square(pzInvis_minus); 
    double enInvis_minus = TMath::Sqrt(pInvis_minus2 + invisMass2);
    reco::Candidate::LorentzVector p4Invis_minus(pxInvis_minus, pyInvis_minus, pzInvis_minus, enInvis_minus);
    
    return std::make_pair(p4Invis_plus, p4Invis_minus);
  }
}
  
matchedTauDecayType::matchedTauDecayType() 
  : genTau_(0),
    recCandidate_(0),
    recLeadTrack_(0),
    recLeadTrackTrajectory_(0),
    trackBuilder_(0)
{}

matchedTauDecayType::matchedTauDecayType(const reco::GenParticle* genTau, const reco::Candidate* recCandidate,
					 const TransientTrackBuilder* trackBuilder, NSVfitDecayVertexFitter* decayVertexFitAlgorithm)
  : genTau_(genTau),
    recCandidate_(recCandidate),
    recLeadTrack_(0),
    recLeadTrackTrajectory_(0),
    trackBuilder_(trackBuilder)
{
  assert(genTau);
  genTauP4_ = genTau->p4();
  genTauCharge_ = genTau->charge();
  reco::Candidate::Point genTauProdVertex_point = genTau->vertex();
  genTauProdVertexPos_ = AlgebraicVector3(genTauProdVertex_point.x(), genTauProdVertex_point.y(), genTauProdVertex_point.z());
  genTauDecayMode_ = getGenTauDecayMode(genTau);
  isLeptonicDecay_ = (genTauDecayMode_ == "electron" || genTauDecayMode_ == "muon");
  findDaughters(genTau, genTauDecayProducts_, -1);
  genVisP4_ = getVisMomentum(genTauDecayProducts_);
  genInvisP4_ = getInvisMomentum(genTauDecayProducts_);
  reco::Candidate::Point genTauDecayVertex_point = getDecayVertex(genTau_);
  genTauDecayVertexPos_ = AlgebraicVector3(genTauDecayVertex_point.x(), genTauDecayVertex_point.y(), genTauDecayVertex_point.z());
  AlgebraicVector3 genDecayVertex_wrt_ProdVertex = genTauDecayVertexPos_ - genTauProdVertexPos_;
  genTauDecayDistance_ = TMath::Sqrt(norm2(genDecayVertex_wrt_ProdVertex));

  assert(recCandidate_);
  recVisP4_ = recCandidate_->p4();
  if ( dynamic_cast<const reco::GsfElectron*>(recCandidate_) ) {
    const reco::GsfElectron* electron = dynamic_cast<const reco::GsfElectron*>(recCandidate_);
    if ( !electron->gsfTrack().isNull() ) recTracks_.push_back(electron->gsfTrack().get());
    //else if ( !electron->closestCtfTrackRef().isNull() ) recTracks_.push_back(electron->closestCtfTrackRef().get());
    recTauDecayMode_ = reco::PFTauDecayMode::tauDecaysElectron;
  } else if ( dynamic_cast<const pat::Electron*>(recCandidate_) ) {
    const pat::Electron* electron = dynamic_cast<const pat::Electron*>(recCandidate_);
    if ( !electron->gsfTrack().isNull() ) recTracks_.push_back(electron->gsfTrack().get());
    //else if ( !electron->closestCtfTrackRef().isNull() ) recTracks_.push_back(electron->closestCtfTrackRef().get());
    recTauDecayMode_ = reco::PFTauDecayMode::tauDecaysElectron;
  } else if ( dynamic_cast<const reco::Muon*>(recCandidate_) ) {
    const reco::Muon* muon = dynamic_cast<const reco::Muon*>(recCandidate_);
    if ( !muon->innerTrack().isNull() ) recTracks_.push_back(muon->innerTrack().get());
    recTauDecayMode_ = reco::PFTauDecayMode::tauDecayMuon;
  } else if ( dynamic_cast<const pat::Muon*>(recCandidate_) ) {
    const pat::Muon* muon = dynamic_cast<const pat::Muon*>(recCandidate_);
    if ( !muon->innerTrack().isNull() ) recTracks_.push_back(muon->innerTrack().get());
    recTauDecayMode_ = reco::PFTauDecayMode::tauDecayMuon;
  } else if ( dynamic_cast<const reco::PFTau*>(recCandidate_) ) {
    const reco::PFTau* tau = dynamic_cast<const reco::PFTau*>(recCandidate_);
    const reco::PFCandidateRefVector& tauPFChargedHadrCands = tau->signalPFChargedHadrCands();
    for ( reco::PFCandidateRefVector::const_iterator tauPFChargedHadrCand = tauPFChargedHadrCands.begin();
	  tauPFChargedHadrCand != tauPFChargedHadrCands.end(); ++tauPFChargedHadrCand ) {
      if ( !(*tauPFChargedHadrCand)->trackRef().isNull() ) recTracks_.push_back((*tauPFChargedHadrCand)->trackRef().get());
      else if ( !(*tauPFChargedHadrCand)->gsfTrackRef().isNull() ) recTracks_.push_back((*tauPFChargedHadrCand)->gsfTrackRef().get());
    }
    recTauDecayMode_ = tau->decayMode();
  } else if ( dynamic_cast<const pat::Tau*>(recCandidate_) ) {
    const pat::Tau* tau = dynamic_cast<const pat::Tau*>(recCandidate_);
    if ( tau->isPFTau() ) {
      const reco::PFCandidateRefVector& tauPFChargedHadrCands = tau->signalPFChargedHadrCands();
      for ( reco::PFCandidateRefVector::const_iterator tauPFChargedHadrCand = tauPFChargedHadrCands.begin();
	    tauPFChargedHadrCand != tauPFChargedHadrCands.end(); ++tauPFChargedHadrCand ) {
	if ( !(*tauPFChargedHadrCand)->trackRef().isNull() ) recTracks_.push_back((*tauPFChargedHadrCand)->trackRef().get());
	else if ( !(*tauPFChargedHadrCand)->gsfTrackRef().isNull() ) recTracks_.push_back((*tauPFChargedHadrCand)->gsfTrackRef().get());
      }
      recTauDecayMode_ = tau->decayMode();
    } else if ( tau->isCaloTau() ) {
      const reco::TrackRefVector& tauSignalTracks = tau->signalTracks();
      for ( reco::TrackRefVector::const_iterator tauSignalTrack = tauSignalTracks.begin();
	    tauSignalTrack != tauSignalTracks.end(); ++tauSignalTrack ) {
	recTracks_.push_back(tauSignalTrack->get());
      }
      recTauDecayMode_ = reco::PFTauDecayMode::tauDecayOther; // CV: decay mode reconstruction not implemented for CaloTaus
    } else assert(0);
  } else {
    throw cms::Exception("matchedTauDecayType::matchedTauDecayType")
      << " Invalid Type of recCandidate passed as function Argument !!\n";
  }

  // sort tracks by **decreasing** Pt and determine "leading" (highest Pt) track
  std::sort(recTracks_.begin(), recTracks_.end(), isHigherPt);
 
  for ( std::vector<const reco::Track*>::const_iterator recTrack = recTracks_.begin();
	recTrack != recTracks_.end(); ++recTrack ) {
    const reco::HitPattern& trackHitPattern = (*recTrack)->hitPattern();
    if ( trackHitPattern.numberOfValidTrackerHits() >= 5 &&
	 trackHitPattern.numberOfValidPixelHits() >= 2 &&
	 (*recTrack)->normalizedChi2() < 20. &&
	 ((*recTrack)->ptError()/(*recTrack)->pt()) < 0.5 &&
	 (*recTrack)->pt() > 2.0 ) {
      recSelTracks_.push_back(*recTrack);
    }
  }

  if ( recSelTracks_.size() >= 1 ) {
    recLeadTrack_ = recSelTracks_.at(0);
    if ( trackBuilder_ ) recLeadTrackTrajectory_ = new reco::TransientTrack(trackBuilder_->build(*recLeadTrack_));
  }

  if ( decayVertexFitAlgorithm ) {
    if ( recSelTracks_.size() >= 2 ) {
      recTauDecayVertex_ = decayVertexFitAlgorithm->fitSecondaryVertex(recSelTracks_);
    }
    if ( recTauDecayVertex_.isValid() ) {
      recTauDecayVertexPos_(0) = recTauDecayVertex_.position().x();
      recTauDecayVertexPos_(1) = recTauDecayVertex_.position().y();
      recTauDecayVertexPos_(2) = recTauDecayVertex_.position().z();
      recTauDecayVertexCov_ = recTauDecayVertex_.positionError().matrix_new();
      hasRecTauDecayVertex_ = true;
    } else {
      recTauDecayVertexPos_(0) = 0.;
      recTauDecayVertexPos_(1) = 0.;
      recTauDecayVertexPos_(2) = 0.;
      for ( int iRow = 0; iRow < 3; ++iRow ) {
	for ( int iColumn = 0; iColumn < 3; ++iColumn ) {
	  recTauDecayVertexCov_(iRow, iColumn) = 1.e+3;
	}
      }
      hasRecTauDecayVertex_ = false;
    }
  }
}

void matchedTauDecayType::print(std::ostream& stream) const
{
  stream << "genTau: En = " << genTauP4_.E() << ", Pt = " << genTauP4_.pt() << "," 
	 << " eta = " << genTauP4_.eta() << ", phi = " << genTauP4_.phi() << std::endl;
  stream << "production vertex(gen): x = " << genTauProdVertexPos_(0) << ", y = " << genTauProdVertexPos_(1) << ", z = " << genTauProdVertexPos_(2) << std::endl;
  unsigned idx_daughter = 0;
  for ( std::vector<const reco::GenParticle*>::const_iterator genTauDecayProduct = genTauDecayProducts_.begin();
	genTauDecayProduct != genTauDecayProducts_.end(); ++genTauDecayProduct ) {
    //if ( TMath::Abs((*genTauDecayProduct)->charge()) > 0.5 && (*genTauDecayProduct)->status() == 1 ) {
    if ( (*genTauDecayProduct)->status() == 1 ) {
      stream << "daughter #" << idx_daughter << ": En = " << (*genTauDecayProduct)->energy() << ", Pt = " << (*genTauDecayProduct)->pt() << "," 
	     << " eta = " << (*genTauDecayProduct)->eta() << ", phi = " << (*genTauDecayProduct)->phi() 
	     << " (pdgId = " << (*genTauDecayProduct)->pdgId() << ", charge = " << (*genTauDecayProduct)->charge() << ")" << std::endl;
      ++idx_daughter;
    }
  }
  stream << " vis: En = " << genVisP4_.E() << ", Pt = " << genVisP4_.pt() << "," 
	 << " eta = " << genVisP4_.eta() << ", phi = " << genVisP4_.phi() 
	 << " (Px = " << genVisP4_.px() << ", Py = " << genVisP4_.py() << ", mass = " << genVisP4_.mass() << ")" << std::endl;
  stream << " X = " << (genVisP4_.E()/genTauP4_.E()) << std::endl;
  stream << " vis(rf): P = " << boostToCOM(genTauP4_, genVisP4_).P() << std::endl;
  stream << " angle(vis,tau) = " << angle(genVisP4_, genTauP4_) << std::endl;
  stream << " invis: En = " << genInvisP4_.E() << ", Pt = " << genInvisP4_.pt() << "," 
	 << " eta = " << genInvisP4_.eta() << ", phi = " << genInvisP4_.phi() 
	 << " (Px = " << genInvisP4_.px() << ", Py = " << genInvisP4_.py() << ", mass = " << genInvisP4_.mass() << ")" << std::endl;
  stream << " angle(vis,invis) = " << angle(genVisP4_, genInvisP4_) << std::endl;
  stream << "decay vertex(gen): x = " << genTauDecayVertexPos_(0) << ", y = " << genTauDecayVertexPos_(1) << ", z = " << genTauDecayVertexPos_(2) 
	 << " (d = " << genTauDecayDistance_ << ")" << std::endl;
  reco::Candidate::LorentzVector p4Flight = compFlightP4(genTauDecayVertexPos_, genTauProdVertexPos_, genTauP4_.P(), tauLeptonMass);
  stream << " flight-path: En = " << p4Flight.E() << ", P = " << p4Flight.P() << "," 
	 << " eta = " << p4Flight.eta() << ", phi = " << p4Flight.phi() << std::endl;
  int errorFlag = 0;
  std::pair<const reco::Candidate::LorentzVector, const reco::Candidate::LorentzVector> p4Invis_reconstructed = 
    reconstructTauNeutrino(genTauP4_.theta(), genTauP4_.phi(), tauLeptonMass, genVisP4_, genInvisP4_.mass(), errorFlag);
  if ( !errorFlag ) {
    stream << " invis(rec+): En = " << p4Invis_reconstructed.first.E() << ", Pt = " << p4Invis_reconstructed.first.pt() << "," 
	   << " eta = " << p4Invis_reconstructed.first.eta() << ", phi = " << p4Invis_reconstructed.first.phi() << " "
	   << "(qjAngle = " << gjAngleFromLabMomenta(genTauP4_, genTauP4_ - p4Invis_reconstructed.first) << "," 
	   << " phi_lab = " << phiLabFromLabMomenta(genTauP4_, genVisP4_) << ")" << std::endl;
    stream << " invis(rec-): En = " << p4Invis_reconstructed.second.E() << ", Pt = " << p4Invis_reconstructed.second.pt() << "," 
	   << " eta = " << p4Invis_reconstructed.second.eta() << ", phi = " << p4Invis_reconstructed.second.phi() << " "
	   << "(qjAngle = " << gjAngleFromLabMomenta(genTauP4_, genTauP4_ - p4Invis_reconstructed.second) << "," 
	   << " phi_lab = " << phiLabFromLabMomenta(genTauP4_, genVisP4_) << ")" << std::endl;
  }
  stream << "decay-mode = " << genTauDecayMode_ << std::endl;
  stream << " vis(rec): En = " << recVisP4_.E() << ", Pt = " << recVisP4_.pt() << "," 
	 << " eta = " << recVisP4_.eta() << ", phi = " << recVisP4_.phi() 
	 << " (Px = " << recVisP4_.px() << ", Py = " << recVisP4_.py() << ", mass = " << recVisP4_.mass() << ")" << std::endl;
  unsigned idx_track = 0;
  for ( std::vector<const reco::Track*>::const_iterator recTrack = recSelTracks_.begin();
	recTrack != recSelTracks_.end(); ++recTrack ) {
    stream << "Track #" << idx_track << ": Pt = " << (*recTrack)->pt() << "," 
	   << " eta = " << (*recTrack)->eta() << ", phi = " << (*recTrack)->phi() 
	   << " (charge = " << (*recTrack)->charge() << ", chi2 = " << (*recTrack)->normalizedChi2() << ")" << std::endl;
    reco::Candidate::Point refPoint = (*recTrack)->referencePoint();
    stream << " reference Point: x = " << refPoint.x() << ", y = " << refPoint.y() << ", z = " << refPoint.z() << std::endl;
    if ( trackBuilder_ ) { 
      reco::TransientTrack recTrackTrajectory = trackBuilder_->build(*recTrack);
      SVfitTrackExtrapolation track_extrapolation(recTrackTrajectory, genTauDecayVertexPos_);
      if ( !track_extrapolation.errorFlag() ) {
	const AlgebraicVector3& track_pcaPos = track_extrapolation.point_of_closest_approach();
	const AlgebraicMatrix33& track_pcaCov = track_extrapolation.covariance();
	AlgebraicVector3 pcaPos_wrt_genTauDecayVertexPos = track_pcaPos - genTauDecayVertexPos_;
	AlgebraicVector3 tauFlightPath_unit = normalize(AlgebraicVector3(p4Flight.px(), p4Flight.py(), p4Flight.pz()));
	AlgebraicVector3 u1, u2, u3;
	compLocalCoordinates(tauFlightPath_unit, u1, u2, u3); 
	AlgebraicVector3 residual_U = transformToLocalCoordinatesPos(pcaPos_wrt_genTauDecayVertexPos, u1, u2, u3);
	AlgebraicMatrix33 track_pcaCov_U = transformToLocalCoordinatesCov(track_pcaCov, u1, u2, u3); 
	stream << "DCA(decay vertex): " 
	       << " dU1 = " << residual_U(0) << " +/- " << TMath::Sqrt(track_pcaCov_U(0, 0)) << "," 
	       << " dU2 = " << residual_U(1) << " +/- " << TMath::Sqrt(track_pcaCov_U(1, 1)) 
	       << " (rho = " << track_pcaCov_U(1, 0)/TMath::Sqrt(track_pcaCov_U(0, 0)*track_pcaCov_U(1, 1)) << ")" << std::endl;
      }
    }
    ++idx_track;
  }
  if ( hasRecTauDecayVertex_ ) {
    AlgebraicVector3 recDecayVertex_wrt_ProdVertex = recTauDecayVertexPos_ - genTauProdVertexPos_;
    std::cout << " decay vertex(rec):" 
	      << " x = " << recTauDecayVertexPos_(0) << " +/- " << TMath::Sqrt(recTauDecayVertexCov_(0, 0)) << "," 
	      << " y = " << recTauDecayVertexPos_(1) << " +/- " << TMath::Sqrt(recTauDecayVertexCov_(1, 1)) << "," 
	      << " z = " << recTauDecayVertexPos_(2) << " +/- " << TMath::Sqrt(recTauDecayVertexCov_(2, 2)) 
	      << " (d = " << TMath::Sqrt(norm2(recDecayVertex_wrt_ProdVertex)) << ", chi2 = " << recTauDecayVertex_.normalisedChiSquared() << ")" << std::endl;
    AlgebraicVector3 recDecayVertex_wrt_genDecayVertex = recTauDecayVertexPos_ - genTauDecayVertexPos_;
    std::cout << "distance(rec-gen):" 
	      << " x = " << recDecayVertex_wrt_genDecayVertex(0) << " +/- " << TMath::Sqrt(recTauDecayVertexCov_(0, 0)) << "," 
	      << " y = " << recDecayVertex_wrt_genDecayVertex(1) << " +/- " << TMath::Sqrt(recTauDecayVertexCov_(1, 1)) << "," 
	      << " z = " << recDecayVertex_wrt_genDecayVertex(2) << " +/- " << TMath::Sqrt(recTauDecayVertexCov_(2, 2)) 
	      << " (d = " << TMath::Sqrt(norm2(recDecayVertex_wrt_genDecayVertex)) << ")" << std::endl;
    printMatrix("recTauDecayVertexCov", recTauDecayVertexCov_);
  } else {
    stream << "decay vertex(rec): NA" << std::endl;
  }
}
  
void matchRecToGenTauDecays(const edm::View<reco::Candidate>& candidates, const std::vector<const reco::GenParticle*>& genTaus, double dRmatch,
			    std::vector<matchedTauDecayType*>& matchedTauDecays, const TransientTrackBuilder* trackBuilder, NSVfitDecayVertexFitter* decayVertexFitAlgorithm)
{
  for ( std::vector<const reco::GenParticle*>::const_iterator genTau = genTaus.begin();
	genTau != genTaus.end(); ++genTau ) {
    //std::cout << "genTau: Pt = " << (*genTau)->pt() << ", eta = " << (*genTau)->eta() << ", phi = " << (*genTau)->phi() << std::endl;
    double dRmin = 1.e+3;
    const reco::Candidate* candidate_matched = 0;
    for ( edm::View<reco::Candidate>::const_iterator candidate = candidates.begin();
	  candidate != candidates.end(); ++candidate ) {
      //std::cout << "candidate: Pt = " << candidate->pt() << ", eta = " << candidate->eta() << ", phi = " << candidate->phi() << std::endl;
      double dR = deltaR(candidate->p4(), (*genTau)->p4());
      //std::cout << " dR = " << dR << std::endl;
      if ( dR < dRmin ) {
	candidate_matched = &(*candidate);
	dRmin = dR;
      }
    }
    if ( dRmin < dRmatch ) matchedTauDecays.push_back(new matchedTauDecayType(*genTau, candidate_matched, trackBuilder, decayVertexFitAlgorithm));
  }
}


