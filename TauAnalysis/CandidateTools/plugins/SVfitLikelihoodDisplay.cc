#include "TauAnalysis/CandidateTools/plugins/SVfitLikelihoodDisplay.h"

#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DQMServices/Core/interface/DQMStore.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/TauReco/interface/PFTau.h"
#include "DataFormats/TauReco/interface/PFTauFwd.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/PFMETFwd.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Math/interface/angle.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"

#include "TauAnalysis/CandidateTools/interface/candidateAuxFunctions.h"
#include "TauAnalysis/CandidateTools/interface/SVfitTrackExtrapolation.h"
#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"
#include "AnalysisDataFormats/TauAnalysis/interface/PFMEtSignCovMatrix.h"

#include <TMath.h>
#include <TMatrixD.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TPaveText.h>
#include <TMarker.h>
#include <TStyle.h>
#include <TROOT.h>

#include <iostream>
#include <iomanip>

using namespace SVfit_namespace;

SVfitLikelihoodDisplay::SVfitLikelihoodDisplay(const edm::ParameterSet& cfg)
  : moduleLabel_(cfg.getParameter<std::string>("@module_label"))
{
  srcGenParticles_ = cfg.getParameter<edm::InputTag>("srcGenParticles");
  srcElectrons_    = cfg.getParameter<edm::InputTag>("srcElectrons");
  srcMuons_        = cfg.getParameter<edm::InputTag>("srcMuons");
  srcTaus_         = cfg.getParameter<edm::InputTag>("srcTaus");
  srcMEt_          = cfg.getParameter<edm::InputTag>("srcMEt");
  srcMEtCov_       = cfg.getParameter<edm::InputTag>("srcMEtCov");
  srcVertices_     = cfg.getParameter<edm::InputTag>("srcVertices");

  srcWeights_      = cfg.getParameter<vInputTag>("srcWeights");

  edm::ParameterSet cfgVertexFitAlgo(cfg);
  cfgVertexFitAlgo.addParameter<unsigned>("minNumTracksRefit", 2);
  vertexFitAlgo_ = new NSVfitEventVertexRefitter(cfgVertexFitAlgo);
}

SVfitLikelihoodDisplay::~SVfitLikelihoodDisplay()
{
  delete vertexFitAlgo_;
}

void SVfitLikelihoodDisplay::beginJob()
{
// nothing to be done yet...
}

//-------------------------------------------------------------------------------
// auxiliary functions for vector algebra

namespace
{
  AlgebraicVector3 compDecayPosition(const AlgebraicVector3& origin, double d, const AlgebraicVector3& direction)
  {
    return AlgebraicVector3(origin(0) + d*direction(0), origin(1) + d*direction(1), origin(2) + d*direction(2));
  }
  
  double compScalarProduct(const AlgebraicVector3& p1, const AlgebraicVector3& p2)
  {
    return (p1(0)*p2(0) + p1(1)*p2(1) + p1(2)*p2(2));
  }

  double norm2(const AlgebraicVector3& p)
  {
    return square(p(0)) + square(p(1)) + square(p(2));
  }
}
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
// auxiliary functions for vertex reconstruction

namespace
{
  std::pair<AlgebraicVector3, AlgebraicMatrix33>
  fitDecayVertex(const std::vector<const reco::Track*>& tauSignalTracks, 
		 const AlgebraicVector3& genProdVertexPos, const AlgebraicVector3& genDecayVertexPos, const NSVfitEventVertexRefitter* vertexFitAlgo, 
		 int verbosity = 1)
  {
    TransientVertex recDecayVertex = vertexFitAlgo->fitSecondaryVertex(tauSignalTracks);
    AlgebraicVector3 recDecayVertexPos(recDecayVertex.position().x(), recDecayVertex.position().y(), recDecayVertex.position().z());

    AlgebraicMatrix33 recDecayVertexCov = recDecayVertex.positionError().matrix_new();

    if ( verbosity ) {
      std::cout << "<fitDecayVertex>:" << std::endl;
      AlgebraicVector3 genDecayVertex_wrt_ProdVertex = genDecayVertexPos - genProdVertexPos;
      std::cout << " decay vertex(gen):" 
		<< " x = " << genDecayVertexPos(0) << ", y = " << genDecayVertexPos(1) << ", z = " << genDecayVertexPos(2) 	      
		<< " (d = " << TMath::Sqrt(norm2(genDecayVertex_wrt_ProdVertex)) << ")" << std::endl;
      AlgebraicVector3 recDecayVertex_wrt_ProdVertex = recDecayVertexPos - genProdVertexPos;
      std::cout << " decay vertex(rec):" 
		<< " x = " << recDecayVertexPos(0) << " +/- " << TMath::Sqrt(recDecayVertexCov(0, 0)) << "," 
		<< " y = " << recDecayVertexPos(1) << " +/- " << TMath::Sqrt(recDecayVertexCov(1, 1)) << "," 
		<< " z = " << recDecayVertexPos(2) << " +/- " << TMath::Sqrt(recDecayVertexCov(2, 2)) 
		<< " (d = " << TMath::Sqrt(norm2(recDecayVertex_wrt_ProdVertex)) << ", chi2 = " << recDecayVertex.normalisedChiSquared() << ")" << std::endl;
      AlgebraicVector3 recDecayVertex_wrt_genDecayVertex = recDecayVertexPos - genDecayVertexPos;
      std::cout << "distance(rec-gen):" 
		<< " x = " << recDecayVertex_wrt_genDecayVertex(0) << " +/- " << TMath::Sqrt(recDecayVertexCov(0, 0)) << "," 
		<< " y = " << recDecayVertex_wrt_genDecayVertex(1) << " +/- " << TMath::Sqrt(recDecayVertexCov(1, 1)) << "," 
		<< " z = " << recDecayVertex_wrt_genDecayVertex(2) << " +/- " << TMath::Sqrt(recDecayVertexCov(2, 2)) 
		<< " (d = " << TMath::Sqrt(norm2(recDecayVertex_wrt_genDecayVertex)) << ")" << std::endl;
    }

    return std::make_pair(recDecayVertexPos, recDecayVertexCov);
  }

  AlgebraicVector3 compIntersection_of_lines(const AlgebraicVector3& offset1, const AlgebraicVector3& slope1, 
					     const AlgebraicVector3& offset2, const AlgebraicVector3& slope2)
  {
    // CV: algorithm taken from http://www.softsurfer.com/Archive/algorithm_0106/algorithm_0106.htm

    AlgebraicVector3 w0(offset1(0) - offset2(0), offset1(1) - offset2(1), offset1(2) - offset2(2));

    double a = compScalarProduct(slope1, slope1);
    double b = compScalarProduct(slope1, slope2);
    double c = compScalarProduct(slope2, slope2);
    double d = compScalarProduct(slope1, w0);
    double e = compScalarProduct(slope2, w0);
    
    double denominator = (a*c - square(b));
    assert(denominator >= 0.);
    double s, t;
    if ( denominator > 0. ) {
      s = (b*e - c*d)/denominator;
      t = (a*e - b*d)/denominator;
    } else {
      edm::LogWarning ("compIntersection_of_lines")
	<< "Lines given as arguments are parallel --> returning s = t = 0 !!";
      s = 0.;
      t = 0.;
    }
    
    AlgebraicVector3 solution1(offset1(0) + s*slope1(0), offset1(1) + s*slope1(1), offset1(2) + s*slope1(2));
    AlgebraicVector3 solution2(offset2(0) + t*slope2(0), offset2(1) + t*slope2(1), offset2(2) + t*slope2(2));
    //std::cout << "solution1: x = " << solution1(0) << ", y = " << solution1(1) << ", z = " << solution1(2) << std::endl;
    //std::cout << "solution2: x = " << solution2(0) << ", y = " << solution2(1) << ", z = " << solution2(2) << std::endl;
    //const double epsilon = 1.e-4;
    //assert(TMath::Abs((solution1(0) - solution2(0))/TMath::Max(1., solution1(0) + solution2(0))) < epsilon);
    //assert(TMath::Abs((solution1(1) - solution2(1))/TMath::Max(1., solution1(1) + solution2(1))) < epsilon);
    //assert(TMath::Abs((solution1(2) - solution2(2))/TMath::Max(1., solution1(2) + solution2(2))) < epsilon);
    
    return solution1;
  }
}
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
// auxiliary functions for kinematic reconstruction

namespace
{
  reco::Candidate::Vector normalize(const reco::Candidate::Vector& p)
  {
    double p_x = p.x();
    double p_y = p.y();
    double p_z = p.z();
    double mag2 = square(p_x) + square(p_y) + square(p_z);
    if ( mag2 <= 0. ) return p;
    double mag = TMath::Sqrt(mag2);
    return reco::Candidate::Vector(p_x/mag, p_y/mag, p_z/mag);
  }

  double compScalarProduct(const reco::Candidate::Vector& p1, const reco::Candidate::Vector& p2)
  {
    return (p1.x()*p2.x() + p1.y()*p2.y() + p1.z()*p2.z());
  }
  
  reco::Candidate::Vector compCrossProduct(const reco::Candidate::Vector& p1, const reco::Candidate::Vector& p2)
  {
    double p3_x = p1.y()*p2.z() - p1.z()*p2.y();
    double p3_y = p1.z()*p2.x() - p1.x()*p2.z();
    double p3_z = p1.x()*p2.y() - p1.y()*p2.x();
    return reco::Candidate::Vector(p3_x, p3_y, p3_z);
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
    
    double pVis_parl = compScalarProduct(p3Vis, u_parl);
    double pVis_parl2 = square(pVis_parl);
    double pVis_perp = compScalarProduct(p3Vis, u_perp);
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

  reco::Candidate::LorentzVector compFlightP4(const AlgebraicVector3& decayVertex, AlgebraicVector3& prodVertex, double p, double motherMass)
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
  
  reco::Candidate::LorentzVector compTauP4(double gjAngle, double phi_lab, const reco::Candidate::LorentzVector& p4Vis, double invisMass, double motherMass, 
					   double& pVis_rf, reco::Candidate::Vector& tauFlight)
  {
    // CV: code taken from TauAnalysis/CandidateTools/src/NSVfitTauDecayBuilder.cc
    
    double visMass = p4Vis.mass();
    double pVis_lab = p4Vis.P();
    
    reco::Candidate::Vector p3Vis_unit = normalize(reco::Candidate::Vector(p4Vis.px(), p4Vis.py(), p4Vis.pz()));
    
    pVis_rf = pVisRestFrame(visMass, invisMass, motherMass);
    
    double angleVis_lab = gjAngleToLabFrame(pVis_rf, gjAngle, pVis_lab);
    
    double pTau_lab = motherMomentumLabFrame(visMass, pVis_rf, gjAngle, pVis_lab, motherMass);
    //std::cout << "pTau_lab = " << pTau_lab << std::endl;
    
    tauFlight = motherDirection(p3Vis_unit, angleVis_lab, phi_lab);
    //std::cout << "tauFlight: eta = " << tauFlight.eta() << ", phi = " << tauFlight.phi() << std::endl;
    
    reco::Candidate::LorentzVector p4Tau = motherP4(tauFlight.Unit(), pTau_lab, motherMass);
    return p4Tau;
  }

  double phiLabFromLabMomenta(const reco::Candidate::LorentzVector& motherP4, const reco::Candidate::LorentzVector& visP4)
  {
    reco::Candidate::Vector u_z = normalize(reco::Candidate::Vector(visP4.px(), visP4.py(), visP4.pz()));
    reco::Candidate::Vector u_y = normalize(compCrossProduct(reco::Candidate::Vector(0., 0., 1.), u_z));
    reco::Candidate::Vector u_x = compCrossProduct(u_y, u_z);
    
    reco::Candidate::Vector p3Mother_unit = normalize(reco::Candidate::Vector(motherP4.px(), motherP4.py(), motherP4.pz()));
    
    double phi_lab = TMath::ATan2(compScalarProduct(p3Mother_unit, u_y), compScalarProduct(p3Mother_unit, u_x));
    return phi_lab;
  }
}
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
// miscellaneous auxiliary functions

namespace
{
  bool isHigherPt(const reco::Track* track1, const reco::Track* track2)
  {
    return (track1->pt() > track2->pt());
  }

  void printVector(const std::string& label, const AlgebraicVector3& p)
  {
    std::cout << label << ": x = " << p(0) << ", y = " << p(1) << ", z = " << p(2) << std::endl;
  }

  void printMatrix(const std::string& label, AlgebraicMatrix33& m)
  {
    std::cout << label << ":" << std::endl;
    for ( unsigned iRow = 0; iRow < 3; ++iRow ) {
      std::cout << " |";
      for ( unsigned iColumn = 0; iColumn < 3; ++iColumn ) {
	std::cout << " " << std::setw(12) << m(iRow, iColumn);
      }
      std::cout << " |" << std::endl;
    }
  }
}
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
// likelihood functions

namespace
{
  double negLogLikelihoodExponentialDecay(double tauFlightPath, double p)
  {
    // CV: code copied from TauAnalysis/CandidateTools/plugins/NSVfitTauDecayLikelihoodTrackInfo.cc
    double a = (p/tauLeptonMass)*cTauLifetime;
    if ( tauFlightPath < 0. ) tauFlightPath = 0.;
    return TMath::Log(a) + tauFlightPath/a;
  }

  double backwardsPenaltyTerm(const AlgebraicVector3& flight_direction, double fligt_distance, const reco::Candidate::LorentzVector& visP4)
  {
//--- add penalty term to -log(likelihood)
//    in case the flight correction moves the SV 'behind' the primary vertex.

    return 0.;

    double penalty = 0.;
    if ( (flight_direction(0)*visP4.px() + flight_direction(1)*visP4.py() + flight_direction(2)*visP4.pz()) < 0. ) {
      penalty += 1.e+4*fligt_distance*fligt_distance;
    }
    
    return penalty;
  }
  
  double negLogLikelihoodTauToLepDecay(double gjAngle, double X, double invisMass)
  {
    // CV: code copied from TauAnalysis/CandidateTools/plugins/NSVfitTauToLepLikelihoodMatrixElement.cc
    //    (for the case that tau lepton is unpolarized)
    
    double invisMass2 = square(invisMass);
    
    double prob = 1.;
    if ( invisMass < TMath::Sqrt((1. - X)*tauLeptonMass2) ) { // LB: physical solution
      prob = (13./tauLeptonMass4)*(tauLeptonMass2 - invisMass2)*(tauLeptonMass2 + 2.*invisMass2)*invisMass;
    } else {                                                  // LB: unphysical solution
      double invisMass_limit  = TMath::Sqrt((1. - X)*tauLeptonMass2);
      double invisMass2_limit = square(invisMass_limit);
      prob = (13./tauLeptonMass4)*(tauLeptonMass2 - invisMass2_limit)*(tauLeptonMass2 + 2.*invisMass2_limit)*invisMass_limit;
      prob /= (1. + 1.e+6*square(invisMass - invisMass_limit));
    }
    
    if ( prob > 0. ) return -TMath::Log(prob);  
    else return 1.e+37;
  }
  
  double negLogLikelihoodTauToHadDecay(double gjAngle, double pVis_rf, double X, double visMass)
  {
    // CV: code copied from TauAnalysis/CandidateTools/plugins/NSVfitTauToHadLikelihoodPhaseSpace.cc
    
    double visMass2 = square(visMass);
    
    double prob = tauLeptonMass/(2.*pVis_rf);
    if ( X < (visMass2/tauLeptonMass2) ) {
      double X_limit = visMass2/tauLeptonMass2;
      prob /= (1. + 1.e+6*square(X - X_limit));
    } else if ( X > 1. ) {
      double X_limit = 1.;
      prob /= (1. + 1.e+6*square(X - X_limit));
    }
    
    if ( prob > 0. ) return -TMath::Log(prob);  
    else return 1.e+37;
  }
  
  double negLogLikelihoodTrackInfo1Prong(const AlgebraicVector3& eventVertexPos, const AlgebraicMatrix33& eventVertexCov,
					 double pTau_lab, const reco::Candidate::LorentzVector& p4Vis, const AlgebraicVector3& tauFlightPath_unit, 
					 const reco::TransientTrack& track_trajectory, 
					 const AlgebraicVector3& track_refPoint, const AlgebraicVector3& track_direction)
  {
    AlgebraicVector3 pcaPos = compIntersection_of_lines(eventVertexPos, tauFlightPath_unit, track_refPoint, track_direction);
    //AlgebraicVector3 pcaPos_wrt_vertex = pcaPos - eventVertexPos;
    //std::cout << "pcaPos(initial):" 
    //	      << " x = " << pcaPos(0) << ", y = " << pcaPos(1) << ", z = " << pcaPos(2) 
    //	      << " (d = " << TMath::Sqrt(norm2(pcaPos_wrt_vertex)) << ")" << std::endl;      
    
    int iteration = 0;
    const int maxIterations = 10;
    bool hasConverged = false;      
    double prob = 0.;
    do {
      SVfitTrackExtrapolation track_extrapolation_update(track_trajectory, pcaPos);
      const AlgebraicVector3& track_direction_update = track_extrapolation_update.tangent();
      const AlgebraicVector3& track_refPoint_update = track_extrapolation_update.point_of_closest_approach();
      AlgebraicVector3 pcaPos_update = compIntersection_of_lines(
	eventVertexPos, tauFlightPath_unit, track_refPoint_update, track_direction_update);
      //AlgebraicVector3 pcaPos_wrt_vertex_update = pcaPos_update - eventVertexPos;
      //std::cout << "pcaPos(iteration #" << iteration << "):" 
      //	        << " x = " << pcaPos_update(0) << ", y = " << pcaPos_update(1) << ", z = " << pcaPos_update(2) 
      //	        << " (d = " << TMath::Sqrt(norm2(pcaPos_wrt_vertex_update)) << ")" << std::endl;    
      double diff_pca_update2 = norm2(pcaPos_update - pcaPos);
      if ( diff_pca_update2 < 1.e-10 ) {
	hasConverged = true;	 
	
	double decayDistance = TMath::Sqrt(norm2(pcaPos - eventVertexPos));
	double dMax  = 2.5000;
	double dStep = 0.0025;
	for ( double decayDistance_shift = -decayDistance; decayDistance_shift <= dMax; decayDistance_shift += dStep ) {
	  AlgebraicVector3 expectedDecayVertexPos = compDecayPosition(eventVertexPos, decayDistance + decayDistance_shift, tauFlightPath_unit);
	  AlgebraicMatrix33 expectedDecayVertexCov = eventVertexCov;
	  
	  SVfitTrackExtrapolation track_extrapolation_shifted(track_trajectory, expectedDecayVertexPos);
	  AlgebraicVector3 reconstructedDecayVertexPos = track_extrapolation_shifted.point_of_closest_approach();
	  AlgebraicMatrix33 reconstructedDecayVertexCov = track_extrapolation_shifted.covariance();
	  
	  AlgebraicVector3 residual = reconstructedDecayVertexPos - expectedDecayVertexPos;
	  double residual2 = norm2(residual);
	  AlgebraicMatrix33 reconstructed_wrt_expectedDecayVertexCov = reconstructedDecayVertexCov;
	  for ( unsigned iRow = 0; iRow < 3; ++iRow ) {
	    for ( unsigned iColumn = 0; iColumn < 3; ++iColumn ) {
	      reconstructed_wrt_expectedDecayVertexCov(iRow, iColumn) += expectedDecayVertexCov(iRow, iColumn);
	    }
	  }
	  double sigma2 = ROOT::Math::Similarity(residual, reconstructed_wrt_expectedDecayVertexCov)/residual2;
	  double nll_dca = -logGaussian(TMath::Sqrt(residual2), TMath::Sqrt(sigma2));
	  double nll_lifetime = negLogLikelihoodExponentialDecay(decayDistance + decayDistance_shift, pTau_lab);	 
	  double nll_penalty = backwardsPenaltyTerm(tauFlightPath_unit, decayDistance + decayDistance_shift, p4Vis);
	  double nll = nll_dca + nll_lifetime + nll_penalty;
	  //if ( TMath::Abs(decayDistance_shift) < 1.e-4 ) {
	  //  std::cout << "residual: x = " << residual(0) << ", y = " << residual(1) << ", z = " << residual(2) 
	  //	      << " ( |residual| = " << TMath::Sqrt(norm2(residual)) << ", sigma = " << TMath::Sqrt(sigma2) << ")" << std::endl;
	  //  std::cout << "--> -log(P) = " << nll
	  //	      << " ( -log(P_dca) = " << nll_dca << ", -log(P_lifetime) = " << nll_lifetime << ", -log(P_penalty) = " << nll_penalty << ")" << std::endl;
	  //}
	  prob += (TMath::Exp(-nll)*dStep);
	}
      } else {
	pcaPos = pcaPos_update;
      }
      ++iteration;
    } while ( (!hasConverged) && iteration < maxIterations ); 
    if ( iteration == maxIterations ) {
      //edm::LogWarning ("negLogLikelihoodTrackInfo1Prong")
      //  << "Failed to converge on dca computation !!";
      prob = 0.;
    }
    
    //std::cout << "prob = " << prob << std::endl;
    
    if ( prob > 0. ) return -TMath::Log(prob);
    else return 1.e+37;
  }
  
  double negLogLikelihoodTrackInfo3Prong(const AlgebraicVector3& eventVertexPos, const AlgebraicMatrix33& eventVertexCov,
					 double pTau_lab, const reco::Candidate::LorentzVector& p4Vis, const AlgebraicVector3& tauFlightPath_unit,
					 const AlgebraicVector3& reconstructedDecayVertexPos, const AlgebraicMatrix33& reconstructedDecayVertexCov)
  {
    std::cout << "<negLogLikelihoodTrackInfo3Prong>:" << std::endl;
    AlgebraicVector3 decayVertex_wrt_eventVertexPos = reconstructedDecayVertexPos - eventVertexPos;
    printVector("decayVertex_wrt_eventVertexPos", decayVertex_wrt_eventVertexPos);
    double decayDistance_projection = compScalarProduct(decayVertex_wrt_eventVertexPos, tauFlightPath_unit);
    std::cout << "decayDistance_projection = " << decayDistance_projection << std::endl;
    AlgebraicVector3 expectedDecayVertexPos = compDecayPosition(eventVertexPos, decayDistance_projection, tauFlightPath_unit);
    printVector("expectedDecayVertexPos", expectedDecayVertexPos);
    AlgebraicMatrix33 expectedDecayVertexCov = eventVertexCov;
    printMatrix("expectedDecayVertexCov", expectedDecayVertexCov);
    
    AlgebraicVector3 residual = reconstructedDecayVertexPos - expectedDecayVertexPos;
    printVector("residual", residual);
    double residual2 = norm2(residual);
    std::cout << "residual2 = " << residual2 << std::endl;
    AlgebraicMatrix33 reconstructed_wrt_expectedDecayVertexCov = reconstructedDecayVertexCov;
    for ( unsigned iRow = 0; iRow < 3; ++iRow ) {
      for ( unsigned iColumn = 0; iColumn < 3; ++iColumn ) {
	reconstructed_wrt_expectedDecayVertexCov(iRow, iColumn) += expectedDecayVertexCov(iRow, iColumn);
      }
    }
    printMatrix("reconstructed_wrt_expectedDecayVertexCov", reconstructed_wrt_expectedDecayVertexCov);
    double sigma2 = ROOT::Math::Similarity(residual, reconstructed_wrt_expectedDecayVertexCov)/residual2;
    std::cout << "sigma2 = " << sigma2 << std::endl;
    double nll_dca = -logGaussian(TMath::Sqrt(residual2), TMath::Sqrt(sigma2));
    double nll_lifetime = negLogLikelihoodExponentialDecay(decayDistance_projection, pTau_lab);	 
    double nll_penalty = backwardsPenaltyTerm(tauFlightPath_unit, decayDistance_projection, p4Vis);
    double nll = nll_dca + nll_lifetime + nll_penalty;
    std::cout << "residual: x = " << residual(0) << ", y = " << residual(1) << ", z = " << residual(2) 
    	      << " ( |residual| = " << TMath::Sqrt(norm2(residual)) << ", sigma = " << TMath::Sqrt(sigma2) << ")" << std::endl;
    std::cout << "--> -log(P) = " << nll
    	      << " ( -log(P_dca) = " << nll_dca << ", -log(P_lifetime) = " << nll_lifetime << ", -log(P_penalty) = " << nll_penalty << ")" << std::endl;
    
    return nll;
  }
  
  double negLogLikelihoodMEt(const reco::Candidate::LorentzVector& genMEt, const reco::Candidate::LorentzVector& recMEt, 
			     double recMEtCovDet, const TMatrixD& recMEtCovInverse)
  {
    // code copied from TauAnalysis/CandidateTools/plugins/NSVfitEventLikelihoodMEt2.cc
    
    if ( recMEtCovDet != 0. ) {
      double residualPx = recMEt.px() - genMEt.px();
      double residualPy = recMEt.py() - genMEt.py();
      
      double term1 = TMath::Log(2.*TMath::Pi()) + 0.5*TMath::Log(TMath::Abs(recMEtCovDet));
      double term2 = 0.5*(residualPx*(recMEtCovInverse(0,0)*residualPx + recMEtCovInverse(0,1)*residualPy)
			  + residualPy*(recMEtCovInverse(1,0)*residualPx + recMEtCovInverse(1,1)*residualPy));
      return term1 + term2;
    } else {
      return 0.;
    }
  }
}
//-------------------------------------------------------------------------------

namespace
{
  void showHistogram1d(double canvasSizeX, double canvasSizeY,
		       TH1* histogram,
		       std::vector<std::string>& labelTextLines, double labelTextSize,
		       double labelPosX, double labelPosY, double labelSizeX, double labelSizeY,
		       const std::string& xAxisTitle, double xAxisOffset,
		       double yMax,
		       double parameterX_true, 
		       const std::string& outputFileName)
  {
    gROOT->SetBatch(true);
    
    gStyle->SetPalette(1,0);
    
    TCanvas* canvas = new TCanvas("canvas", "canvas", canvasSizeX, canvasSizeY);
    canvas->SetFillColor(10);
    canvas->SetBorderSize(2);
    
    canvas->SetLeftMargin(0.14);
    canvas->SetBottomMargin(0.12);
    
    histogram->SetTitle("");
    histogram->SetStats(false);
    histogram->SetMaximum(yMax);
    
    TAxis* xAxis = histogram->GetXaxis();
    xAxis->SetTitle(xAxisTitle.data());
    xAxis->SetTitleOffset(xAxisOffset);
    
    histogram->SetLineColor(1);
    histogram->SetLineWidth(2);
    histogram->Draw("hist");
    
    TMarker marker(parameterX_true, 0.10, 34);
    marker.SetMarkerColor(1);
    marker.SetMarkerSize(2);
    marker.Draw();
    
    TPaveText* label = new TPaveText(labelPosX, labelPosY, labelPosX + labelSizeX, labelPosY + labelSizeY, "brNDC");
    for ( std::vector<std::string>::const_iterator labelTextLine = labelTextLines.begin();
	  labelTextLine != labelTextLines.end(); ++labelTextLine ) {
      label->AddText(labelTextLine->data());
    }
    label->SetFillColor(10);
    label->SetBorderSize(0);
    label->SetTextColor(1);
    label->SetTextAlign(12);
    label->SetTextSize(labelTextSize);
    label->Draw();
    
    canvas->Update();
    std::string outputFileName_plot = "plots/";
    size_t idx = outputFileName.find_last_of('.');
    outputFileName_plot.append(std::string(outputFileName, 0, idx));
    //if ( idx != std::string::npos ) canvas->Print(std::string(outputFileName_plot).append(std::string(outputFileName, idx)).data());
    //canvas->Print(std::string(outputFileName_plot).append(".png").data());
    //canvas->Print(std::string(outputFileName_plot).append(".pdf").data());
    canvas->Print(std::string(outputFileName_plot).append(".root").data());
  
    delete label;
    delete canvas;  
  }
  
  void showHistogram2d(double canvasSizeX, double canvasSizeY,
		       TH1* histogram,
		       std::vector<std::string>& labelTextLines, double labelTextSize,
		       double labelPosX, double labelPosY, double labelSizeX, double labelSizeY,
		       const std::string& xAxisTitle, double xAxisOffset,
		       const std::string& yAxisTitle, double yAxisOffset,
		       double zMax,
		       double parameterX_true, double parameterY_true,
		       const std::string& outputFileName)
  {
    gROOT->SetBatch(true);
    
    gStyle->SetPalette(1,0);
    
    TCanvas* canvas = new TCanvas("canvas", "canvas", canvasSizeX, canvasSizeY);
    canvas->SetFillColor(10);
    canvas->SetBorderSize(2);
    
    canvas->SetLeftMargin(0.14);
    canvas->SetBottomMargin(0.12);
    
    histogram->SetTitle("");
    histogram->SetStats(false);
    histogram->SetMaximum(zMax);
    
    TAxis* xAxis = histogram->GetXaxis();
    xAxis->SetTitle(xAxisTitle.data());
    xAxis->SetTitleOffset(xAxisOffset);
    
    TAxis* yAxis = histogram->GetYaxis();
    yAxis->SetTitle(yAxisTitle.data());
    yAxis->SetTitleOffset(yAxisOffset);
    
    histogram->Draw("COLZ");
    
    TMarker marker(parameterX_true, parameterY_true, 34);
    marker.SetMarkerColor(1);
    marker.SetMarkerSize(2);
    marker.Draw();
    
    TPaveText* label = new TPaveText(labelPosX, labelPosY, labelPosX + labelSizeX, labelPosY + labelSizeY, "brNDC");
    for ( std::vector<std::string>::const_iterator labelTextLine = labelTextLines.begin();
	  labelTextLine != labelTextLines.end(); ++labelTextLine ) {
      label->AddText(labelTextLine->data());
    }
    label->SetFillColor(10);
    label->SetBorderSize(0);
    label->SetTextColor(1);
    label->SetTextAlign(12);
    label->SetTextSize(labelTextSize);
    label->Draw();

    canvas->Update();
    std::string outputFileName_plot = "plots/";
    size_t idx = outputFileName.find_last_of('.');
    outputFileName_plot.append(std::string(outputFileName, 0, idx));
    //if ( idx != std::string::npos ) canvas->Print(std::string(outputFileName_plot).append(std::string(outputFileName, idx)).data());
    //canvas->Print(std::string(outputFileName_plot).append(".png").data());
    //canvas->Print(std::string(outputFileName_plot).append(".pdf").data());
    canvas->Print(std::string(outputFileName_plot).append(".root").data());
    
    delete label;
    delete canvas;  
  }
  
  void addParameterReference(unsigned idx, 
			     double* parameter_original, int numBins_original, double& parameterMin_original, double& parameterMax_original, 
			     double& parameter_original_true, const std::string& axisLabel_original,
			     double*& parameter_reference1, int& numBins_reference1, double& parameterMin_reference1, double& parameterMax_reference1, 
			     double& parameter_reference1_true, std::string& xAxisLabel,
			     double*& parameter_reference2, int& numBins_reference2, double& parameterMin_reference2, double& parameterMax_reference2, 
			     double& parameter_reference2_true, std::string& yAxisLabel,
			     unsigned numParameters)
  {
    if        ( numParameters == 0 ) {
      parameter_reference1 = parameter_original;
      numBins_reference1 = numBins_original;
      parameterMin_reference1 = parameterMin_original;
      parameterMax_reference1 = parameterMax_original;    
      parameter_reference1_true = parameter_original_true;
      xAxisLabel = Form(axisLabel_original.data(), idx);
    } else if ( numParameters == 1 ) {
      parameter_reference2 = parameter_original;
      numBins_reference2 = numBins_original;
      parameterMin_reference2 = parameterMin_original;
      parameterMax_reference2 = parameterMax_original;
      parameter_reference2_true = parameter_original_true;
      yAxisLabel = Form(axisLabel_original.data(), idx);
    } 
  }
  
  void updateParametersToVary(unsigned idx, bool doVaryTheta, bool doVaryPhi, bool doVaryInvisMass,
			      double* theta, int& thetaNumBins, double& thetaMin, double& thetaMax, double theta_true, 
			      double* phi, int& phiNumBins, double& phiMin, double& phiMax, double phi_true, 
			      double* invisMass, int& invisMassNumBins, double& invisMassMin, double& invisMassMax, double invisMass_true, 
			      double*& parameter1, int& parameter1NumBins, double& parameter1Min, double& parameter1Max, double& parameter1_true, std::string& xAxisLabel,
			      double*& parameter2, int& parameter2NumBins, double& parameter2Min, double& parameter2Max, double& parameter2_true, std::string& yAxisLabel,
			      unsigned& numParametersToVary)
  {
    if ( doVaryTheta ) {
      thetaMin = 0.;
      thetaMax = TMath::Pi();
      addParameterReference(idx, theta, thetaNumBins, thetaMin, thetaMax, theta_true, "#theta_{%u} / Rad", 
			    parameter1, parameter1NumBins, parameter1Min, parameter1Max, parameter1_true, xAxisLabel,
			    parameter2, parameter2NumBins, parameter2Min, parameter2Max, parameter2_true, yAxisLabel, numParametersToVary);
      ++numParametersToVary;
    }
    if ( doVaryPhi ) {
      phiMin = -TMath::Pi();
      phiMax = +TMath::Pi();
      addParameterReference(idx, phi, phiNumBins, phiMin, phiMax, phi_true, "#phi_{%u} / Rad",
			    parameter1, parameter1NumBins, parameter1Min, parameter1Max, parameter1_true, xAxisLabel,
			    parameter2, parameter2NumBins, parameter2Min, parameter2Max, parameter2_true, yAxisLabel, numParametersToVary);
      ++numParametersToVary;
    }
    if ( doVaryInvisMass ) {
      invisMassMin = 0.;
      invisMassMax = 1.8;
      addParameterReference(idx, invisMass, invisMassNumBins, invisMassMin, invisMassMax, invisMass_true, "M_{#nu#nu}^{%u} / GeV",
			    parameter1, parameter1NumBins, parameter1Min, parameter1Max, parameter1_true, xAxisLabel,
			    parameter2, parameter2NumBins, parameter2Min, parameter2Max, parameter2_true, yAxisLabel, numParametersToVary);
      ++numParametersToVary;
    }
  }

  void makeLikelihoodPlot(const reco::Candidate::LorentzVector& genP4Vis1, const reco::Candidate::LorentzVector& genP4Invis1, const std::string& genTauDecayMode1,
			  const AlgebraicVector3& genProdVertexPos1, const AlgebraicVector3& genDecayVertexPos1, 
			  const reco::Candidate::LorentzVector& recP4Vis1, const std::vector<const reco::Track*>& recTracks1,
			  bool doVaryTheta1, bool doVaryPhi1, bool doVaryInvisMass1,
			  bool addLikelihoodTauDecayKine1, bool addLikelihoodTrackInfo1, 
			  const reco::Candidate::LorentzVector& genP4Vis2, const reco::Candidate::LorentzVector& genP4Invis2, const std::string& genTauDecayMode2,
			  const AlgebraicVector3& genProdVertexPos2, const AlgebraicVector3& genDecayVertexPos2, 
			  const reco::Candidate::LorentzVector& recP4Vis2, const std::vector<const reco::Track*>& recTracks2,
			  bool doVaryTheta2, bool doVaryPhi2, bool doVaryInvisMass2,
			  bool addLikelihoodTauDecayKine2, bool addLikelihoodTrackInfo2, 
			  const AlgebraicVector3& eventVertexPos, const AlgebraicMatrix33& eventVertexCov,
			  const reco::Candidate::LorentzVector& recMEt, const TMatrixD& recMEtCov,
			  bool addLikelihoodMEt,
			  const TransientTrackBuilder* trackBuilder, const NSVfitEventVertexRefitter* vertexFitAlgo, 
			  const edm::Event& evt, const std::string& outputFileName)
  {
    std::cout << "<makeLikelihoodPlot>:" << std::endl;
    
    const reco::Track* recLeadTrack1 = recTracks1.at(0);
    assert(recLeadTrack1);
    reco::TransientTrack recLeadTrack1_trajectory = trackBuilder->build(*recLeadTrack1);
    SVfitTrackExtrapolation recLeadTrack1_extrapolation(recLeadTrack1_trajectory, eventVertexPos);
    const AlgebraicVector3& recLeadTrackDirection1 = recLeadTrack1_extrapolation.tangent();
    const AlgebraicVector3& recLeadTrackRefPoint1 = eventVertexPos;
    
    AlgebraicVector3 recDecayVertexPos1;
    AlgebraicMatrix33 recDecayVertexCov1;
    if ( recTracks1.size() >= 2 ) {
      std::pair<AlgebraicVector3, AlgebraicMatrix33> recDecayVertex_pair = fitDecayVertex(recTracks1, genProdVertexPos1, genDecayVertexPos1, vertexFitAlgo, 0);
      recDecayVertexPos1 = recDecayVertex_pair.first;
      recDecayVertexCov1 = recDecayVertex_pair.second;
    }
    
    const reco::Track* recLeadTrack2 = recTracks2.at(0);
    assert(recLeadTrack2);
    reco::TransientTrack recLeadTrack2_trajectory = trackBuilder->build(*recLeadTrack2);
    SVfitTrackExtrapolation recLeadTrack2_extrapolation(recLeadTrack2_trajectory, eventVertexPos);
    const AlgebraicVector3& recLeadTrackDirection2 = recLeadTrack2_extrapolation.tangent();
    const AlgebraicVector3& recLeadTrackRefPoint2 = eventVertexPos;
    
    AlgebraicVector3 recDecayVertexPos2;
    AlgebraicMatrix33 recDecayVertexCov2;
    if ( recTracks2.size() >= 2 ) {
      std::pair<AlgebraicVector3, AlgebraicMatrix33> recDecayVertex_pair = fitDecayVertex(recTracks2, genProdVertexPos2, genDecayVertexPos2, vertexFitAlgo, 0);
      recDecayVertexPos2 = recDecayVertex_pair.first;
      printVector("recDecayVertexPos2", recDecayVertexPos2);
      recDecayVertexCov2 = recDecayVertex_pair.second;
      printMatrix("recDecayVertexCov2", recDecayVertexCov2);
    }
    
    double recMEtCovDet = recMEtCov.Determinant();
    TMatrixD recMEtCovInverse = recMEtCov;
    if ( recMEtCovDet != 0. ) recMEtCovInverse.Invert();
    
    reco::Candidate::LorentzVector genP4Tau1 = genP4Vis1 + genP4Invis1;
    double gjAngle1_true   = gjAngleFromLabMomenta(genP4Tau1, genP4Vis1);
    double gjAngle1Min     = gjAngle1_true;
    double gjAngle1Max     = gjAngle1_true;
    double phi_lab1_true   = phiLabFromLabMomenta(genP4Tau1, genP4Vis1);
    double phi_lab1Min     = phi_lab1_true;
    double phi_lab1Max     = phi_lab1_true;
    double invisMass1_true = genP4Invis1.mass();
    double invisMass1Min   = invisMass1_true;
    double invisMass1Max   = invisMass1_true;
    
    reco::Candidate::LorentzVector genP4Tau2 = genP4Vis2 + genP4Invis2;
    double gjAngle2_true   = gjAngleFromLabMomenta(genP4Tau2, genP4Vis2);
    double gjAngle2Min     = gjAngle2_true;
    double gjAngle2Max     = gjAngle2_true;
    double phi_lab2_true   = phiLabFromLabMomenta(genP4Tau2, genP4Vis2);
    double phi_lab2Min     = phi_lab2_true;
    double phi_lab2Max     = phi_lab2_true;
    double invisMass2_true = genP4Invis2.mass();
    double invisMass2Min   = invisMass2_true;
    double invisMass2Max   = invisMass2_true;
    
    int gjAngleNumBins   = 100;
    int phi_labNumBins   = 100;
    int invisMassNumBins = 100;
    
    double gjAngle1, phi_lab1, invisMass1;
    double gjAngle2, phi_lab2, invisMass2;
    
    double* parameter1 = 0;
    int parameter1NumBins;
    double parameter1Min, parameter1Max, parameter1_true; 
    std::string xAxisLabel;
    double* parameter2 = 0;
    int parameter2NumBins;
    double parameter2Min, parameter2Max, parameter2_true; 
    std::string yAxisLabel;
    unsigned numParametersToVary = 0;
    updateParametersToVary(1, doVaryTheta1, doVaryPhi1, doVaryInvisMass1, 
			   &gjAngle1, gjAngleNumBins, gjAngle1Min, gjAngle1Max, gjAngle1_true,
			   &phi_lab1, phi_labNumBins, phi_lab1Min, phi_lab1Max, phi_lab1_true, 
			   &invisMass1, invisMassNumBins, invisMass1Min, invisMass1Max, invisMass1_true, 
			   parameter1, parameter1NumBins, parameter1Min, parameter1Max, parameter1_true, xAxisLabel, 
			   parameter2, parameter2NumBins, parameter2Min, parameter2Max, parameter2_true, yAxisLabel,
			   numParametersToVary);
    updateParametersToVary(2, doVaryTheta2, doVaryPhi2, doVaryInvisMass2, 
			   &gjAngle2, gjAngleNumBins, gjAngle2Min, gjAngle2Max, gjAngle2_true,
			   &phi_lab2, phi_labNumBins, phi_lab2Min, phi_lab2Max, phi_lab2_true, 
			   &invisMass2, invisMassNumBins, invisMass2Min, invisMass2Max, invisMass2_true, 
			   parameter1, parameter1NumBins, parameter1Min, parameter1Max, parameter1_true, xAxisLabel, 
			   parameter2, parameter2NumBins, parameter2Min, parameter2Max, parameter2_true, yAxisLabel,
			   numParametersToVary);
    
    int gjAngleNumBins1   = ( gjAngle1Max   > gjAngle1Min   ) ? gjAngleNumBins   : 1;
    int phi_labNumBins1   = ( phi_lab1Max   > phi_lab1Min   ) ? phi_labNumBins   : 1;
    int invisMassNumBins1 = ( invisMass1Max > invisMass1Min ) ? invisMassNumBins : 1;
    std::cout << " numBins(1): gjAngle = " << gjAngleNumBins1 << ", phi_lab = " << phi_labNumBins1 << ", invisMass = " << invisMassNumBins1 << std::endl;
    
    int gjAngleNumBins2   = ( gjAngle2Max   > gjAngle2Min   ) ? gjAngleNumBins   : 1;
    int phi_labNumBins2   = ( phi_lab2Max   > phi_lab2Min   ) ? phi_labNumBins   : 1;
    int invisMassNumBins2 = ( invisMass2Max > invisMass2Min ) ? invisMassNumBins : 1;
    std::cout << " numBins(2): gjAngle = " << gjAngleNumBins2 << ", phi_lab = " << phi_labNumBins2 << ", invisMass = " << invisMassNumBins2 << std::endl;
    
    bool isLeptonicDecay1 = (genTauDecayMode1 == "electron" || genTauDecayMode1 == "muon");
    bool isLeptonicDecay2 = (genTauDecayMode2 == "electron" || genTauDecayMode2 == "muon");
    
    if ( numParametersToVary == 0 || numParametersToVary > 2 )
      throw cms::Exception("makeLikelihoodPlot")
	<< " Number of Parameters to vary must be either one or two !!\n";
    if ( doVaryInvisMass1 && !isLeptonicDecay1 ) 
      throw cms::Exception("makeLikelihoodPlot")
	<< " Cannot vary Mass of invisible decay products of first tau, decay mode = " << genTauDecayMode1 << " !!\n";
    if ( doVaryInvisMass2 && !isLeptonicDecay2 )
      throw cms::Exception("makeLikelihoodPlot")
	<< " Cannot vary Mass of invisible decay products of second tau, decay mode = " << genTauDecayMode2 << " !!\n";
    
    TH1* histogram_likelihood = 0;
    TH1* histogram_mass       = 0;
    if      ( numParametersToVary == 1 ) {
      histogram_likelihood = new TH1D("makeLikelihoodPlot_histogram_likelihood", 
				      "makeLikelihoodPlot_histogram_likelihood", 
				      parameter1NumBins, parameter1Min, parameter1Max);
      histogram_mass       = new TH1D("makeLikelihoodPlot_histogram_mass", 
				      "makeLikelihoodPlot_histogram_mass",      
				      parameter1NumBins, parameter1Min, parameter1Max);
    } else if ( numParametersToVary == 2 ) {
      histogram_likelihood = new TH2D("makeLikelihoodPlot_histogram_likelihood", 
				      "makeLikelihoodPlot_histogram_likelihood", 
				      parameter1NumBins, parameter1Min, parameter1Max, parameter2NumBins, parameter2Min, parameter2Max);
      histogram_mass       = new TH2D("makeLikelihoodPlot_histogram_mass", 
				      "makeLikelihoodPlot_histogram_mass",       
				      parameter1NumBins, parameter1Min, parameter1Max, parameter2NumBins, parameter2Min, parameter2Max);  
    } else assert(0);
    
    TAxis* xAxis = histogram_likelihood->GetXaxis();
    TAxis* yAxis = histogram_likelihood->GetYaxis();
    
    int numPoints = 0;
    for ( int gjAngleBin1 = 0; gjAngleBin1 < gjAngleNumBins1; ++gjAngleBin1 ) { 
      for ( int phi_labBin1 = 0; phi_labBin1 < phi_labNumBins1; ++phi_labBin1 ) {
	for ( int invisMassBin1 = 0; invisMassBin1 < invisMassNumBins1; ++invisMassBin1 ) {
	  //for ( int gjAngleBin2 = 0; gjAngleBin2 < gjAngleNumBins2; ++gjAngleBin2 ) {    
	  for ( double gjAngle2 = 2.5; gjAngle2 <= 2.5; gjAngle2 += 0.5 ) {
	  //for ( int phi_labBin2 = 0; phi_labBin2 < phi_labNumBins2; ++phi_labBin2 ) {
	    for ( double phi_lab2 = -2.0; phi_lab2 <= -2.0; phi_lab2 += 0.5 ) {
	      for ( int invisMassBin2 = 0; invisMassBin2 < invisMassNumBins2; ++invisMassBin2 ) {
		
		if ( numPoints > 0 && (numPoints % 1000) == 0 ) std::cout << " computing point = " << numPoints << std::endl;
		
		gjAngle1   = gjAngle1Min   + (gjAngleBin1   + 0.5)*(gjAngle1Max   - gjAngle1Min  )/gjAngleNumBins1;
		phi_lab1   = phi_lab1Min   + (phi_labBin1   + 0.5)*(phi_lab1Max   - phi_lab1Min  )/phi_labNumBins1;
		invisMass1 = invisMass1Min + (invisMassBin1 + 0.5)*(invisMass1Max - invisMass1Min)/invisMassNumBins1;
		double pVis_rf1;
		reco::Candidate::Vector tauFlight1;
		reco::Candidate::LorentzVector p4Tau1 = compTauP4(gjAngle1, phi_lab1, recP4Vis1, invisMass1, tauLeptonMass, pVis_rf1, tauFlight1);
		double X1 = recP4Vis1.E()/p4Tau1.E();
		double tauFlight1_mag = TMath::Sqrt(tauFlight1.mag2());
		AlgebraicVector3 tauFlightPath1_unit(tauFlight1.x()/tauFlight1_mag, tauFlight1.y()/tauFlight1_mag, tauFlight1.z()/tauFlight1_mag);
		
		//gjAngle2   = gjAngle2Min   + (gjAngleBin2   + 0.5)*(gjAngle2Max   - gjAngle2Min  )/gjAngleNumBins2;
		//phi_lab2   = phi_lab2Min   + (phi_labBin2   + 0.5)*(phi_lab2Max   - phi_lab2Min  )/phi_labNumBins2;
		invisMass2 = invisMass2Min + (invisMassBin2 + 0.5)*(invisMass2Max - invisMass2Min)/invisMassNumBins2;
		double pVis_rf2;
		reco::Candidate::Vector tauFlight2;
		reco::Candidate::LorentzVector p4Tau2 = compTauP4(gjAngle2, phi_lab2, recP4Vis2, invisMass2, tauLeptonMass, pVis_rf2, tauFlight2);
		double X2 = recP4Vis2.E()/p4Tau2.E();
		double tauFlight2_mag = TMath::Sqrt(tauFlight2.mag2());
		AlgebraicVector3 tauFlightPath2_unit(tauFlight2.x()/tauFlight2_mag, tauFlight2.y()/tauFlight2_mag, tauFlight2.z()/tauFlight2_mag);
		
		reco::Candidate::LorentzVector genMEt = (p4Tau1 - recP4Vis1) + (p4Tau2 - recP4Vis2);
		
		double negLogP = 0.;
		
		if ( addLikelihoodTauDecayKine1 ) {
		  if ( isLeptonicDecay1 ) negLogP += negLogLikelihoodTauToLepDecay(gjAngle1, X1, invisMass1);
		  else negLogP += negLogLikelihoodTauToHadDecay(gjAngle1, pVis_rf1, X1, recP4Vis1.mass());
		}
		if ( addLikelihoodTrackInfo1 ) {
		  if ( recTracks1.size() >= 2 ) negLogP += negLogLikelihoodTrackInfo3Prong(
		    eventVertexPos, eventVertexCov, p4Tau1.P(), recP4Vis1, tauFlightPath1_unit, recDecayVertexPos1, recDecayVertexCov1);
		  else negLogP += negLogLikelihoodTrackInfo1Prong(
		    eventVertexPos, eventVertexCov, p4Tau1.P(), recP4Vis1, tauFlightPath1_unit, recLeadTrack1_trajectory, recLeadTrackRefPoint1, recLeadTrackDirection1);
		}
		
		if ( addLikelihoodTauDecayKine2 ) {
		  if ( isLeptonicDecay2 ) negLogP += negLogLikelihoodTauToLepDecay(gjAngle2, X2, invisMass2);
		  else negLogP += negLogLikelihoodTauToHadDecay(gjAngle2, pVis_rf2, X2, recP4Vis2.mass());
		}
		if ( addLikelihoodTrackInfo2 ) {
		  if ( recTracks2.size() >= 2 ) negLogP += negLogLikelihoodTrackInfo3Prong(
		    eventVertexPos, eventVertexCov, p4Tau2.P(), recP4Vis2, tauFlightPath2_unit, recDecayVertexPos2, recDecayVertexCov2);
		  else negLogP += negLogLikelihoodTrackInfo1Prong(
		    eventVertexPos, eventVertexCov, p4Tau2.P(), recP4Vis2, tauFlightPath2_unit, recLeadTrack2_trajectory, recLeadTrackRefPoint2, recLeadTrackDirection2);
		}
		
		if ( addLikelihoodMEt ) {		
		  negLogP += negLogLikelihoodMEt(genMEt, recMEt, recMEtCovDet, recMEtCovInverse);
		}
		
		double mass = (p4Tau1 + p4Tau2).mass();
		
		if        ( numParametersToVary == 1 ) {
		  int binX = xAxis->FindBin(*parameter1);
		  histogram_likelihood->SetBinContent(binX, negLogP);
		  histogram_mass->SetBinContent(binX, mass);
		} else if ( numParametersToVary == 2 ) {
		  int binX = xAxis->FindBin(*parameter1);
		  int binY = yAxis->FindBin(*parameter2);
		  histogram_likelihood->SetBinContent(binX, binY, negLogP);
		  histogram_mass->SetBinContent(binX, binY, mass);
		} else assert(0);
		
		++numPoints;
	      }
	    }
	  }
	}
      }
    }
    
    double minBinContent = 1.e+37;
    for ( int iBinX = 1; iBinX <= xAxis->GetNbins(); ++iBinX ) {
      for ( int iBinY = 1; iBinY <= yAxis->GetNbins(); ++iBinY ) {
	double binContent = histogram_likelihood->GetBinContent(iBinX, iBinY);
	if ( binContent < minBinContent ) minBinContent = binContent;
      }
    }
    
    for ( int iBinX = 1; iBinX <= xAxis->GetNbins(); ++iBinX ) {
      for ( int iBinY = 1; iBinY <= yAxis->GetNbins(); ++iBinY ) {
	double binContent = histogram_likelihood->GetBinContent(iBinX, iBinY);
	if ( binContent < 1.e+3 ) {
	  histogram_likelihood->SetBinContent(iBinX, iBinY, binContent - minBinContent);
	} else {
	  histogram_likelihood->SetBinContent(iBinX, iBinY, 0.);
	  histogram_mass->SetBinContent(iBinX, iBinY, 0.);
	}
      }
    }
    
    edm::RunNumber_t runNumber = evt.id().run();
    edm::LuminosityBlockNumber_t lumiSectionNumber = evt.luminosityBlock();
    edm::EventNumber_t eventNumber = evt.id().event();
    
    size_t idx = outputFileName.find_last_of('.');
    std::string outputFileName_plot = std::string(outputFileName, 0, idx);
    outputFileName_plot = Form("%s_run%i_ls%i_ev%i", outputFileName_plot.data(), runNumber, lumiSectionNumber, eventNumber);
    std::string outputFileName_likelihood = std::string(outputFileName_plot).append("_likelihood");
    if ( idx != std::string::npos ) outputFileName_likelihood.append(std::string(outputFileName_likelihood, idx));
    std::string outputFileName_mass = std::string(outputFileName_plot).append("_mass");
    if ( idx != std::string::npos ) outputFileName_mass.append(std::string(outputFileName_mass, idx));
    
    std::vector<std::string> labelTextLines_empty;
    
    if (        numParametersToVary == 1 ) {
      showHistogram1d(800, 600,
		      histogram_likelihood,
		      labelTextLines_empty, 0.050, 
		      0.12, 0.91, 0.77, 0.08,
		      xAxisLabel.data(), 1.3, 
		      2.,
		      parameter1_true, 
		      outputFileName_likelihood);
      showHistogram1d(800, 600,
		      histogram_mass,
		      labelTextLines_empty, 0.050, 
		      0.12, 0.91, 0.77, 0.08,
		      xAxisLabel.data(), 1.3, 
		      250.,
		      parameter1_true, 
		      outputFileName_mass);
    } else if ( numParametersToVary == 2 ) {
      showHistogram2d(800, 800,
		      histogram_likelihood,
		      labelTextLines_empty, 0.050, 
		      0.12, 0.91, 0.77, 0.08,
		      xAxisLabel.data(), 1.3, 
		      yAxisLabel.data(), 1.3,
		      2.,
		      parameter1_true, parameter2_true,
		      outputFileName_likelihood);
      showHistogram2d(800, 800,
		      histogram_mass,
		      labelTextLines_empty, 0.050, 
		      0.12, 0.91, 0.77, 0.08,
		      xAxisLabel.data(), 1.3, 
		      yAxisLabel.data(), 1.3,
		      250.,
		      parameter1_true, parameter2_true,
		      outputFileName_mass);
    } else assert(0);
    
    delete histogram_likelihood;
    delete histogram_mass;
  }
  
  struct matchedTauDecayType
  {
    matchedTauDecayType() 
      : genTau_(0),
	recCandidate_(0),
	verbosity_(0)
    {}
    matchedTauDecayType(const reco::GenParticle* genTau, const reco::Candidate* recCandidate) 
      : genTau_(genTau),
	recCandidate_(recCandidate),
	verbosity_(1)
    {
      assert(genTau);
      genTauP4_ = genTau->p4();
      reco::Candidate::Point genTauProdVertex_point = genTau->vertex();
      genTauProdVertex_ = AlgebraicVector3(genTauProdVertex_point.x(), genTauProdVertex_point.y(), genTauProdVertex_point.z());
      genTauDecayMode_ = getGenTauDecayMode(genTau);
      isLeptonicDecay_ = (genTauDecayMode_ == "electron" || genTauDecayMode_ == "muon");
      std::vector<const reco::GenParticle*> genTauDecayProducts;
      findDaughters(genTau, genTauDecayProducts, -1);
      genVisP4_ = getVisMomentum(genTauDecayProducts);
      genInvisP4_ = getInvisMomentum(genTauDecayProducts);
      reco::Candidate::Point genTauDecayVertex_point = getDecayVertex(genTau_);
      genTauDecayVertex_ = AlgebraicVector3(genTauDecayVertex_point.x(), genTauDecayVertex_point.y(), genTauDecayVertex_point.z());
      
      assert(recCandidate_);
      recVisP4_ = recCandidate_->p4();
      if ( dynamic_cast<const reco::GsfElectron*>(recCandidate_) ) {
	const reco::GsfElectron* electron = dynamic_cast<const reco::GsfElectron*>(recCandidate_);
	if ( !electron->gsfTrack().isNull() ) recTracks_.push_back(electron->gsfTrack().get());
      } else if ( dynamic_cast<const reco::Muon*>(recCandidate_) ) {
	const reco::Muon* muon = dynamic_cast<const reco::Muon*>(recCandidate_);
	if ( !muon->innerTrack().isNull() ) recTracks_.push_back(muon->innerTrack().get());
      } else if ( dynamic_cast<const reco::PFTau*>(recCandidate_) ) {
	const reco::PFTau* tau = dynamic_cast<const reco::PFTau*>(recCandidate_);
	const reco::PFCandidateRefVector& tauPFChargedHadrCands = tau->signalPFChargedHadrCands();
	for ( reco::PFCandidateRefVector::const_iterator tauPFChargedHadrCand = tauPFChargedHadrCands.begin();
	      tauPFChargedHadrCand != tauPFChargedHadrCands.end(); ++tauPFChargedHadrCand ) {
	  if ( !(*tauPFChargedHadrCand)->trackRef().isNull() ) recTracks_.push_back((*tauPFChargedHadrCand)->trackRef().get());
	}
      } else {
	throw cms::Exception("matchedTauDecayType::matchedTauDecayType")
	  << " Invalid Type of recCandidate passed as function Argument !!\n";
      }
      
      // sort tracks by **decreasing** Pt and determine "leading" (highest Pt) track
      std::sort(recTracks_.begin(), recTracks_.end(), isHigherPt);
      const reco::Track* recLeadTrack = recTracks_.at(0);
      assert(recLeadTrack);
      recLeadTrack_.push_back(recLeadTrack);
      
      if ( verbosity_ ) {
	std::cout << "genTau: En = " << genTauP4_.E() << ", P = " << genTauP4_.P() << "," 
		  << " eta = " << genTauP4_.eta() << ", phi = " << genTauP4_.phi() << std::endl;
	std::cout << " production vertex: x = " << genTauProdVertex_(0) << ", y = " << genTauProdVertex_(1) << ", z = " << genTauProdVertex_(2) << std::endl;
	unsigned idx_daughter = 0;
	for ( std::vector<const reco::GenParticle*>::const_iterator genTauDecayProduct = genTauDecayProducts.begin();
	      genTauDecayProduct != genTauDecayProducts.end(); ++genTauDecayProduct ) {
	  if ( TMath::Abs((*genTauDecayProduct)->charge()) > 0.5 && (*genTauDecayProduct)->status() == 1 ) {
	    std::cout << "daughter #" << idx_daughter << ": En = " << (*genTauDecayProduct)->energy() << ", Pt = " << (*genTauDecayProduct)->pt() << "," 
		      << " eta = " << (*genTauDecayProduct)->eta() << ", phi = " << (*genTauDecayProduct)->phi() 
		      << " (charge = " << (*genTauDecayProduct)->charge() << ")" << std::endl;
	    ++idx_daughter;
	  }
	}
	std::cout << " vis: En = " << genVisP4_.E() << ", P = " << genVisP4_.P() << "," 
		  << " eta = " << genVisP4_.eta() << ", phi = " << genVisP4_.phi() 
		  << " (mass = " << genVisP4_.mass() << ")" << std::endl;
	std::cout << " X = " << (genVisP4_.E()/genTauP4_.E()) << std::endl;
	std::cout << " vis(rf): P = " << boostToCOM(genTauP4_, genVisP4_).P() << std::endl;
	std::cout << " angle(vis,tau) = " << angle(genVisP4_, genTauP4_) << std::endl;
	std::cout << " invis: En = " << genInvisP4_.E() << ", P = " << genInvisP4_.P() << "," 
		  << " eta = " << genInvisP4_.eta() << ", phi = " << genInvisP4_.phi() 
		  << " (mass = " << genInvisP4_.mass() << ")" << std::endl;
	std::cout << " angle(vis,invis) = " << angle(genVisP4_, genInvisP4_) << std::endl;
	std::cout << " decay vertex: x = " << genTauDecayVertex_(0) << ", y = " << genTauDecayVertex_(1) << ", z = " << genTauDecayVertex_(2) << std::endl;
	AlgebraicVector3 genTauDecayVertex_wrt_ProdVertex = genTauDecayVertex_ - genTauProdVertex_;
	std::cout << " decay-production vertex:" 
		  << " x = " << genTauDecayVertex_wrt_ProdVertex(0) << "," 
		  << " y = " << genTauDecayVertex_wrt_ProdVertex(1) << "," 
		  << " z = " << genTauDecayVertex_wrt_ProdVertex(2) 
		  << " (d = " << TMath::Sqrt(norm2(genTauDecayVertex_wrt_ProdVertex)) << ")" << std::endl;
	reco::Candidate::LorentzVector p4Flight = compFlightP4(genTauDecayVertex_, genTauProdVertex_, genTauP4_.P(), tauLeptonMass);
	std::cout << " flight-path: En = " << p4Flight.E() << ", P = " << p4Flight.P() << "," 
		  << " eta = " << p4Flight.eta() << ", phi = " << p4Flight.phi() << std::endl;
	int errorFlag = 0;
	std::pair<const reco::Candidate::LorentzVector, const reco::Candidate::LorentzVector> p4Invis_reconstructed = 
	  reconstructTauNeutrino(genTauP4_.theta(), genTauP4_.phi(), tauLeptonMass, genVisP4_, genInvisP4_.mass(), errorFlag);
	if ( !errorFlag ) {
	  std::cout << " invis(rec+): En = " << p4Invis_reconstructed.first.E() << ", P = " << p4Invis_reconstructed.first.P() << "," 
		    << " eta = " << p4Invis_reconstructed.first.eta() << ", phi = " << p4Invis_reconstructed.first.phi() << " "
		    << "(qjAngle = " << gjAngleFromLabMomenta(genTauP4_, genTauP4_ - p4Invis_reconstructed.first) << "," 
		    << " phi_lab = " << phiLabFromLabMomenta(genTauP4_, genVisP4_) << ")" << std::endl;
	  std::cout << " invis(rec-): En = " << p4Invis_reconstructed.second.E() << ", P = " << p4Invis_reconstructed.second.P() << "," 
		    << " eta = " << p4Invis_reconstructed.second.eta() << ", phi = " << p4Invis_reconstructed.second.phi() << " "
		    << "(qjAngle = " << gjAngleFromLabMomenta(genTauP4_, genTauP4_ - p4Invis_reconstructed.second) << "," 
		    << " phi_lab = " << phiLabFromLabMomenta(genTauP4_, genVisP4_) << ")" << std::endl;
	}
	std::cout << "decay-mode = " << genTauDecayMode_ << std::endl;
	unsigned idx_track = 0;
	for ( std::vector<const reco::Track*>::const_iterator recTrack = recTracks_.begin();
	      recTrack != recTracks_.end(); ++recTrack ) {
	  std::cout << "Track #" << idx_track << ": Pt = " << (*recTrack)->pt() << "," 
		    << " eta = " << (*recTrack)->eta() << ", phi = " << (*recTrack)->phi() 
		    << " (charge = " << (*recTrack)->charge() << ", chi2 = " << (*recTrack)->normalizedChi2() << ")" << std::endl;
	  reco::Candidate::Point refPoint = (*recTrack)->referencePoint();
	  std::cout << " reference Point: x = " << refPoint.x() << ", y = " << refPoint.y() << ", z = " << refPoint.z() << std::endl;
	  ++idx_track;
	}
      }
    }
    ~matchedTauDecayType() {}

    const reco::GenParticle* genTau_;
    reco::Candidate::LorentzVector genTauP4_;
    AlgebraicVector3 genTauProdVertex_;
    AlgebraicVector3 genTauDecayVertex_;
    std::string genTauDecayMode_;
    bool isLeptonicDecay_;
    reco::Candidate::LorentzVector genVisP4_;
    reco::Candidate::LorentzVector genInvisP4_;
    
    const reco::Candidate* recCandidate_;
    reco::Candidate::LorentzVector recVisP4_;
    std::vector<const reco::Track*> recTracks_;
    std::vector<const reco::Track*> recLeadTrack_; // is vector for technical reasons, contains one entry only
    
    int verbosity_;
  };
  
  void matchRecToGenTauDecays(const edm::View<reco::Candidate>& candidates, const std::vector<const reco::GenParticle*>& genTaus, double dRmatch,
			      std::vector<matchedTauDecayType*>& matchedTauDecays)
  {
    for ( std::vector<const reco::GenParticle*>::const_iterator genTau = genTaus.begin();
	  genTau != genTaus.end(); ++genTau ) {
      double dRmin = 1.e+3;
      const reco::Candidate* candidate_matched = 0;
      for ( edm::View<reco::Candidate>::const_iterator candidate = candidates.begin();
	    candidate != candidates.end(); ++candidate ) {
	double dR = deltaR(candidate->p4(), (*genTau)->p4());
	if ( dR < dRmin ) {
	  candidate_matched = &(*candidate);
	  dRmin = dR;
	}
      }
      if ( dRmin < dRmatch ) matchedTauDecays.push_back(new matchedTauDecayType(*genTau, candidate_matched));
    }
  }
}

void SVfitLikelihoodDisplay::analyze(const edm::Event& evt, const edm::EventSetup& es)
{
  std::cout << "<SVfitLikelihoodDisplay::analyze>:" << std::endl;
  std::cout << " moduleLabel = " << moduleLabel_ << std::endl;

  edm::ESHandle<TransientTrackBuilder> trackBuilderHandle;
  es.get<TransientTrackRecord>().get("TransientTrackBuilder", trackBuilderHandle);
  const TransientTrackBuilder* trackBuilder = trackBuilderHandle.product();
  if ( !trackBuilder ) {
    throw cms::Exception("SVfitLikelihoodDisplay::analyze")
      << " Failed to access TransientTrackBuilder !!\n";
  }

  vertexFitAlgo_->beginEvent(evt, es);

  edm::Handle<reco::GenParticleCollection> genParticles;
  evt.getByLabel(srcGenParticles_, genParticles);
  
  std::vector<int> pdgIds_tau;
  pdgIds_tau.push_back(-15);
  pdgIds_tau.push_back(+15);

  std::vector<const reco::GenParticle*> genTaus;
  for ( reco::GenParticleCollection::const_iterator genParticle = genParticles->begin();
	genParticle != genParticles->end(); ++genParticle ) {
    bool isTau = false;
    for ( std::vector<int>::const_iterator pdgId_tau = pdgIds_tau.begin();
	  pdgId_tau != pdgIds_tau.end(); ++pdgId_tau ) {
      if ( genParticle->pdgId() == (*pdgId_tau) && genParticle->status() == 2 ) isTau = true;
    }
    if ( isTau ) genTaus.push_back(&(*genParticle));
  }

  std::cout << "#genTaus = " << genTaus.size() << std::endl;

  std::vector<matchedTauDecayType*> matchedTauDecays;

  typedef edm::View<reco::Candidate> CandidateView;

  edm::Handle<CandidateView> electrons;
  if ( srcElectrons_.label() != "" ) {
    evt.getByLabel(srcElectrons_, electrons);
    std::cout << "#electrons = " << electrons->size() << std::endl;
    matchRecToGenTauDecays(*electrons, genTaus, 0.3, matchedTauDecays);
  }

  edm::Handle<CandidateView> muons;
  if ( srcMuons_.label() != "" ) {
    evt.getByLabel(srcMuons_, muons);
    std::cout << "#muons = " << muons->size() << std::endl;
    matchRecToGenTauDecays(*muons, genTaus, 0.3, matchedTauDecays);
  }

  edm::Handle<CandidateView> taus;
  if ( srcTaus_.label() != "" ) {
    evt.getByLabel(srcTaus_, taus);
    std::cout << "#taus = " << taus->size() << std::endl;
    matchRecToGenTauDecays(*taus, genTaus, 0.3, matchedTauDecays);
  }

  std::cout << "#matchedTauDecays = " << matchedTauDecays.size() << std::endl;
  if ( !(matchedTauDecays.size() == 2) ) 
    std::cout << " Failed to find exactly two gen. matched Tau decay products --> skipping !!" << std::endl;

  const matchedTauDecayType* matchedTau1 = matchedTauDecays.at(0);
  const matchedTauDecayType* matchedTau2 = matchedTauDecays.at(1);
  assert(matchedTau1 && matchedTau2);

  reco::Candidate::LorentzVector diTauP4 = matchedTau1->genTauP4_ + matchedTau2->genTauP4_;
  std::cout << " diTau: Mass = " << diTauP4.mass() << " "
	    << "(visMass = " << (matchedTau1->genVisP4_ + matchedTau2->genVisP4_).mass() << ")," 
	    << " Pt = " << diTauP4.pt() << std::endl;

  reco::Candidate::LorentzVector genMEtP4 = matchedTau1->genInvisP4_ + matchedTau2->genInvisP4_;
  std::cout << " MEt(gen): Pt = " << genMEtP4.pt() << ", phi = " << genMEtP4.phi() << " "
	    << "(Px = " << genMEtP4.px() << ", Py = " << genMEtP4.py() << ")" << std::endl;
  
  edm::Handle<reco::PFMETCollection> recMETs;
  evt.getByLabel(srcMEt_, recMETs);
  if ( !(recMETs->size() == 1) )
    throw cms::Exception("SVfitLikelihoodDisplay::analyze") 
      << "Failed to find unique MET object !!\n";
  const reco::PFMET& recMEt = recMETs->front();
  std::cout << " MEt(rec): Pt = " << recMEt.pt() << ", phi = " << recMEt.phi() << " "
	    << "(Px = " << recMEt.px() << ", Py = " << recMEt.py() << ")" << std::endl;

  TMatrixD recMEtCov(2,2);
  if ( srcMEtCov_.label() != "" ) {
    edm::Handle<PFMEtSignCovMatrix> recMEtCovHandle;    
    evt.getByLabel(srcMEtCov_, recMEtCovHandle);
    recMEtCov = (*recMEtCovHandle);
  } else {
    recMEtCov = recMEt.getSignificanceMatrix();
  }
  std::cout << " covMEt(rec):" << std::endl;
  recMEtCov.Print();

  typedef edm::View<reco::Vertex> VertexView;
  edm::Handle<VertexView> vertices;
  evt.getByLabel(srcVertices_, vertices);
  if ( vertices->size() != 1 ) return;
  const reco::Vertex& vertex = vertices->front();
  std::cout << "event vertex(rec): x = " << vertex.x() << ", y = " << vertex.y() << ", z = " << vertex.z() 
	    << " (chi2 = " << vertex.normalizedChi2() << ")" << std::endl;
  
  std::vector<const reco::Track*> tauTracks;
  for ( std::vector<matchedTauDecayType*>::const_iterator matchedTauDecay = matchedTauDecays.begin();
	matchedTauDecay != matchedTauDecays.end(); ++matchedTauDecay ) {
    tauTracks.insert(tauTracks.end(), (*matchedTauDecay)->recTracks_.begin(), (*matchedTauDecay)->recTracks_.end());
  }
  TransientVertex vertex_refitted = vertexFitAlgo_->refit(&vertex, &tauTracks);
  AlgebraicVector3 vertexPos_refitted(vertex_refitted.position().x(), vertex_refitted.position().y(), vertex_refitted.position().z());
  AlgebraicMatrix33 vertexCov_refitted = vertex_refitted.positionError().matrix_new();
  std::cout << "event vertex(rec,refitted): x = " << vertexPos_refitted(0) << ", y = " << vertexPos_refitted(1) << ", z = " << vertexPos_refitted(2) 
	    << " (chi2 = " << vertex_refitted.normalisedChiSquared() << ")" << std::endl;    
/*
  makeLikelihoodPlot(matchedTau1->genVisP4_, matchedTau1->genInvisP4_, matchedTau1->genTauDecayMode_, matchedTau1->genTauProdVertex_, matchedTau1->genTauDecayVertex_, 
		     matchedTau1->recVisP4_, matchedTau1->recTracks_,
		     true, false, matchedTau1->isLeptonicDecay_, 
		     true, false,
		     matchedTau2->genVisP4_, matchedTau2->genInvisP4_, matchedTau2->genTauDecayMode_, matchedTau2->genTauProdVertex_, matchedTau2->genTauDecayVertex_, 
		     matchedTau2->recVisP4_, matchedTau2->recTracks_,
		     false, false, false,
		     false, false, 
		     vertexPos_refitted, vertexCov_refitted,
		     recMEt.p4(), recMEtCov,
		     false,
		     trackBuilder, vertexFitAlgo_,
		     evt, std::string(moduleLabel_).append("_TauDecayKine1_vs_gjAngle1_and_Mnunu1"));
  makeLikelihoodPlot(matchedTau1->genVisP4_, matchedTau1->genInvisP4_, matchedTau1->genTauDecayMode_, matchedTau1->genTauProdVertex_, matchedTau1->genTauDecayVertex_, 
		     matchedTau1->recVisP4_, matchedTau1->recLeadTrack_,
		     true, true, false, 
		     false, true, 
		     matchedTau2->genVisP4_, matchedTau2->genInvisP4_, matchedTau2->genTauDecayMode_, matchedTau2->genTauProdVertex_, matchedTau2->genTauDecayVertex_, 
		     matchedTau2->recVisP4_, matchedTau2->recLeadTrack_,
		     false, false, false,
		     false, false, 
		     vertexPos_refitted, vertexCov_refitted,
		     recMEt.p4(), recMEtCov,
		     false,
		     trackBuilder, vertexFitAlgo_,
		     evt, std::string(moduleLabel_).append("_TrackInfo1dca_vs_gjAngle1_and_phiLab1"));
  makeLikelihoodPlot(matchedTau1->genVisP4_, matchedTau1->genInvisP4_, matchedTau1->genTauDecayMode_, matchedTau1->genTauProdVertex_, matchedTau1->genTauDecayVertex_, 
		     matchedTau1->recVisP4_, matchedTau1->recTracks_,
		     true, true, false, 
		     false, true, 
		     matchedTau2->genVisP4_, matchedTau2->genInvisP4_, matchedTau2->genTauDecayMode_, matchedTau2->genTauProdVertex_, matchedTau2->genTauDecayVertex_, 
		     matchedTau2->recVisP4_, matchedTau2->recTracks_,
		     false, false, false,
		     false, false, 
		     vertexPos_refitted, vertexCov_refitted,
		     recMEt.p4(), recMEtCov,
		     false,
		     trackBuilder, vertexFitAlgo_,
		     evt, std::string(moduleLabel_).append("_TrackInfo1vtx_vs_gjAngle1_and_phiLab1"));
  makeLikelihoodPlot(matchedTau1->genVisP4_, matchedTau1->genInvisP4_, matchedTau1->genTauDecayMode_, matchedTau1->genTauProdVertex_, matchedTau1->genTauDecayVertex_, 
		     matchedTau1->recVisP4_, matchedTau1->recTracks_,
		     false, false, false,
		     false, false, 
		     matchedTau2->genVisP4_, matchedTau2->genInvisP4_, matchedTau2->genTauDecayMode_, matchedTau2->genTauProdVertex_, matchedTau2->genTauDecayVertex_, 
		     matchedTau2->recVisP4_, matchedTau2->recTracks_,
		     true, false, matchedTau2->isLeptonicDecay_, 
		     true, false,		     
		     vertexPos_refitted, vertexCov_refitted,
		     recMEt.p4(), recMEtCov,
		     false,
		     trackBuilder, vertexFitAlgo_,
		     evt, std::string(moduleLabel_).append("_TauDecayKine2_vs_gjAngle2_and_Mnunu2"));
  makeLikelihoodPlot(matchedTau1->genVisP4_, matchedTau1->genInvisP4_, matchedTau1->genTauDecayMode_, matchedTau1->genTauProdVertex_, matchedTau1->genTauDecayVertex_, 
		     matchedTau1->recVisP4_, matchedTau1->recLeadTrack_,
		     false, false, false,
		     false, false, 
		     matchedTau2->genVisP4_, matchedTau2->genInvisP4_, matchedTau2->genTauDecayMode_, matchedTau2->genTauProdVertex_, matchedTau2->genTauDecayVertex_, 
		     matchedTau2->recVisP4_, matchedTau2->recLeadTrack_,
		     true, true, false, 
		     false, true, 
		     vertexPos_refitted, vertexCov_refitted,
		     recMEt.p4(), recMEtCov,
		     false,
		     trackBuilder, vertexFitAlgo_,
		     evt, std::string(moduleLabel_).append("_TrackInfo2dca_vs_gjAngle2_and_phiLab2"));
 */
  makeLikelihoodPlot(matchedTau1->genVisP4_, matchedTau1->genInvisP4_, matchedTau1->genTauDecayMode_, matchedTau1->genTauProdVertex_, matchedTau1->genTauDecayVertex_, 
		     matchedTau1->recVisP4_, matchedTau1->recTracks_,
		     false, false, false,
		     false, false, 
		     matchedTau2->genVisP4_, matchedTau2->genInvisP4_, matchedTau2->genTauDecayMode_, matchedTau2->genTauProdVertex_, matchedTau2->genTauDecayVertex_, 
		     matchedTau2->recVisP4_, matchedTau2->recTracks_,
		     true, true, false, 
		     false, true, 
		     vertexPos_refitted, vertexCov_refitted,
		     recMEt.p4(), recMEtCov,
		     false,
		     trackBuilder, vertexFitAlgo_,
		     evt, std::string(moduleLabel_).append("_TrackInfo2vtx_vs_gjAngle2_and_phiLab2"));
/*
  makeLikelihoodPlot(matchedTau1->genVisP4_, matchedTau1->genInvisP4_, matchedTau1->genTauDecayMode_, matchedTau1->genTauProdVertex_, matchedTau1->genTauDecayVertex_, 
		     matchedTau1->recVisP4_, matchedTau1->recTracks_,
		     true, true, false,
		     false, false, 
		     matchedTau2->genVisP4_, matchedTau2->genInvisP4_, matchedTau2->genTauDecayMode_, matchedTau2->genTauProdVertex_, matchedTau2->genTauDecayVertex_, 
		     matchedTau2->recVisP4_, matchedTau2->recTracks_,
		     false, false, false, 
		     false, false, 
		     vertexPos_refitted, vertexCov_refitted,
		     recMEt.p4(), recMEtCov,
		     true,
		     trackBuilder, vertexFitAlgo_,
		     evt, std::string(moduleLabel_).append("_MEt_vs_gjAngle1_and_phiLab1"));
  makeLikelihoodPlot(matchedTau1->genVisP4_, matchedTau1->genInvisP4_, matchedTau1->genTauDecayMode_, matchedTau1->genTauProdVertex_, matchedTau1->genTauDecayVertex_, 
		     matchedTau1->recVisP4_, matchedTau1->recTracks_,
		     true, false, matchedTau1->isLeptonicDecay_, 
		     false, false, 
		     matchedTau2->genVisP4_, matchedTau2->genInvisP4_, matchedTau2->genTauDecayMode_, matchedTau2->genTauProdVertex_, matchedTau2->genTauDecayVertex_, 
		     matchedTau2->recVisP4_, matchedTau2->recTracks_,
		     false, false, false, 
		     false, false, 
		     vertexPos_refitted, vertexCov_refitted,
		     recMEt.p4(), recMEtCov,
		     true,
		     trackBuilder, vertexFitAlgo_,
		     evt, std::string(moduleLabel_).append("_MEt_vs_gjAngle1_and_Mnunu1"));
  makeLikelihoodPlot(matchedTau1->genVisP4_, matchedTau1->genInvisP4_, matchedTau1->genTauDecayMode_, matchedTau1->genTauProdVertex_, matchedTau1->genTauDecayVertex_, 
		     matchedTau1->recVisP4_, matchedTau1->recTracks_,
		     false, false, false, 
		     false, false, 
		     matchedTau2->genVisP4_, matchedTau2->genInvisP4_, matchedTau2->genTauDecayMode_, matchedTau2->genTauProdVertex_, matchedTau2->genTauDecayVertex_, 
		     matchedTau2->recVisP4_, matchedTau2->recTracks_,
		     true, true, false,
		     false, false, 		     
		     vertexPos_refitted, vertexCov_refitted,
		     recMEt.p4(), recMEtCov,
		     true,
		     trackBuilder, vertexFitAlgo_,
		     evt, std::string(moduleLabel_).append("_MEt_vs_gjAngle2_and_phiLab2"));
  makeLikelihoodPlot(matchedTau1->genVisP4_, matchedTau1->genInvisP4_, matchedTau1->genTauDecayMode_, matchedTau1->genTauProdVertex_, matchedTau1->genTauDecayVertex_, 
		     matchedTau1->recVisP4_, matchedTau1->recTracks_,
		     false, false, false, 
		     false, false, 
		     matchedTau2->genVisP4_, matchedTau2->genInvisP4_, matchedTau2->genTauDecayMode_, matchedTau2->genTauProdVertex_, matchedTau2->genTauDecayVertex_, 
		     matchedTau2->recVisP4_, matchedTau2->recTracks_,
		     true, false, matchedTau2->isLeptonicDecay_, 
		     false, false, 		     
		     vertexPos_refitted, vertexCov_refitted,
		     recMEt.p4(), recMEtCov,
		     true,
		     trackBuilder, vertexFitAlgo_,
		     evt, std::string(moduleLabel_).append("_MEt_vs_gjAngle2_and_Mnunu2"));
  makeLikelihoodPlot(matchedTau1->genVisP4_, matchedTau1->genInvisP4_, matchedTau1->genTauDecayMode_, matchedTau1->genTauProdVertex_, matchedTau1->genTauDecayVertex_, 
		     matchedTau1->recVisP4_, matchedTau1->recTracks_,
		     true, false, false,
		     false, false, 
		     matchedTau2->genVisP4_, matchedTau2->genInvisP4_, matchedTau2->genTauDecayMode_, matchedTau2->genTauProdVertex_, matchedTau2->genTauDecayVertex_, 
		     matchedTau2->recVisP4_, matchedTau2->recTracks_,
		     true, false, false, 
		     false, false, 
		     vertexPos_refitted, vertexCov_refitted,
		     recMEt.p4(), recMEtCov,
		     true,
		     trackBuilder, vertexFitAlgo_,
		     evt, std::string(moduleLabel_).append("_MEt_vs_gjAngle1_and_gjAngle2"));
  makeLikelihoodPlot(matchedTau1->genVisP4_, matchedTau1->genInvisP4_, matchedTau1->genTauDecayMode_, matchedTau1->genTauProdVertex_, matchedTau1->genTauDecayVertex_, 
		     matchedTau1->recVisP4_, matchedTau1->recTracks_,
		     true, false, false,
		     true, false, 
		     matchedTau2->genVisP4_, matchedTau2->genInvisP4_, matchedTau2->genTauDecayMode_, matchedTau2->genTauProdVertex_, matchedTau2->genTauDecayVertex_, 
		     matchedTau2->recVisP4_, matchedTau2->recTracks_,
		     true, false, false, 
		     true, false, 
		     vertexPos_refitted, vertexCov_refitted,
		     recMEt.p4(), recMEtCov,
		     true,
		     trackBuilder, vertexFitAlgo_,
		     evt, std::string(moduleLabel_).append("_TauDecayKine_plus_MEt_vs_gjAngle1_and_gjAngle2"));
  makeLikelihoodPlot(matchedTau1->genVisP4_, matchedTau1->genInvisP4_, matchedTau1->genTauDecayMode_, matchedTau1->genTauProdVertex_, matchedTau1->genTauDecayVertex_, 
		     matchedTau1->recVisP4_, matchedTau1->recTracks_,
		     true, false, false,
		     true, true, 
		     matchedTau2->genVisP4_, matchedTau2->genInvisP4_, matchedTau2->genTauDecayMode_, matchedTau2->genTauProdVertex_, matchedTau2->genTauDecayVertex_, 
		     matchedTau2->recVisP4_, matchedTau2->recTracks_,
		     true, false, false, 
		     true, true, 
		     vertexPos_refitted, vertexCov_refitted,
		     recMEt.p4(), recMEtCov,
		     true,
		     trackBuilder, vertexFitAlgo_,
		     evt, std::string(moduleLabel_).append("_all_vs_gjAngle1_and_gjAngle2"));
 */
}

void SVfitLikelihoodDisplay::endJob()
{
// nothing to be done yet...
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(SVfitLikelihoodDisplay);





