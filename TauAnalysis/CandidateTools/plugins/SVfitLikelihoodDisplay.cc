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
#include "TauAnalysis/CandidateTools/interface/NSVfitEventVertexRefitter.h"
#include "TauAnalysis/CandidateTools/interface/NSVfitDecayVertexFitter.h"
#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"
#include "TauAnalysis/CandidateTools/interface/svFitLikelihoodDisplayAuxFunctions.h"

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

const int minNumTracksRefit = 2;
const int minNumTracksFit = 2;

const int verbosity = 0;

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

  edm::ParameterSet cfgEventVertexFitAlgo(cfg);
  cfgEventVertexFitAlgo.addParameter<unsigned>("minNumTracksRefit", minNumTracksRefit);
  eventVertexFitAlgorithm_ = new NSVfitEventVertexRefitter(cfgEventVertexFitAlgo);

  edm::ParameterSet cfgDecayVertexFitAlgorithm(cfg);
  cfgDecayVertexFitAlgorithm.addParameter<unsigned>("minNumTracksFit", minNumTracksFit);
  decayVertexFitAlgorithm_ = new NSVfitDecayVertexFitter(cfgDecayVertexFitAlgorithm);
}

SVfitLikelihoodDisplay::~SVfitLikelihoodDisplay()
{
  delete eventVertexFitAlgorithm_;
  delete decayVertexFitAlgorithm_;
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
// auxiliary functions for kinematic reconstruction

namespace
{
  reco::Candidate::LorentzVector compTauP4(double gjAngle_or_X, bool isX, double phi_lab, const reco::Candidate::LorentzVector& p4Vis, double invisMass, double motherMass, 
					   double& pVis_rf, double& gjAngle_rf, reco::Candidate::Vector& p3Tau_unit, bool& isValidSolution)
  {
    // CV: code taken from TauAnalysis/CandidateTools/src/NSVfitTauDecayBuilder.cc
    
    double visMass = p4Vis.mass();
    double pVis_lab = p4Vis.P();
    double enVis_lab = p4Vis.E();
    double motherMass2 = square(motherMass);

    reco::Candidate::Vector p3Vis_unit = normalize(reco::Candidate::Vector(p4Vis.px(), p4Vis.py(), p4Vis.pz()));
  
//--- compute energy and momentum of visible decay products in tau lepton rest-frame
    pVis_rf = TMath::Sqrt((motherMass2 - square(visMass + invisMass))*(motherMass2 - square(visMass - invisMass)))/(2.*motherMass);
    double enVis_rf = TMath::Sqrt(square(pVis_rf) + square(visMass));

//--- compute Gottfried-Jackson angle
//   (angle of visible decay products wrt. tau lepton flight direction)
    if ( isX ) {
      double visEnFracX = gjAngle_or_X;
      
      double beta2 = 1. - square(visEnFracX*motherMass/enVis_lab);
      double beta = TMath::Sqrt(beta2);
      
      double cosGjAngle_rf = (visEnFracX*motherMass - enVis_rf)/(beta*pVis_rf);
      //std::cout << " cosGjAngle_rf = " << cosGjAngle_rf;
      isValidSolution = true;
      if        ( cosGjAngle_rf < -1. ) {
	cosGjAngle_rf = -1.;
	isValidSolution = false;
      } else if ( cosGjAngle_rf > +1. ) {
	cosGjAngle_rf = +1.;
	isValidSolution = false;
      }
      //std::cout << " (isValidSolution = " << isValidSolution << ")" << std::endl;
      gjAngle_rf = TMath::ACos(cosGjAngle_rf);
    } else {
      isValidSolution = true;
      gjAngle_rf = gjAngle_or_X;
    }

//--- compute component of visible momentum perpendicular to tau flight direction
//   (NB: the perpendicular component of the visible momentum vector
//        is invariant under Lorentz boost in tau direction, 
//        i.e. is identical in tau rest-frame and laboratory frame)
    double pVis_perp = pVis_rf*TMath::Sin(gjAngle_rf);

    double gjAngle_lab = TMath::ASin(pVis_perp/pVis_lab);

    double pVis_parl_lab = TMath::Sqrt(square(pVis_lab) - square(pVis_perp));
    double pVis_parl_rf = pVis_rf*TMath::Cos(gjAngle_rf);

    double gamma = (enVis_rf*TMath::Sqrt(square(enVis_rf) + square(pVis_parl_lab) - square(pVis_parl_rf))
		  - pVis_parl_rf*pVis_parl_lab)/(square(enVis_rf) - square(pVis_parl_rf));

//--- compute tau lepton energy and momentum in laboratory frame  
    double pTau_lab = TMath::Sqrt(square(gamma) - 1.)*tauLeptonMass;
    double enTau_lab = TMath::Sqrt(square(pTau_lab) + tauLeptonMass2);
    p3Tau_unit = motherDirection(p3Vis_unit, gjAngle_lab, phi_lab);
    reco::Candidate::LorentzVector p4Tau_lab = reco::Candidate::LorentzVector(
      pTau_lab*p3Tau_unit.x(), pTau_lab*p3Tau_unit.y(), pTau_lab*p3Tau_unit.z(), enTau_lab);

    return p4Tau_lab;
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
    double nll = TMath::Log(a) + tauFlightPath/a;
    if ( verbosity >= 3 ) {
      std::cout << "<negLogLikelihoodExponentialDecay>:" << std::endl;
      std::cout << " -log(P) = " << nll << std::endl;
    }
    return nll;
  }

  double backwardsPenaltyTerm(const AlgebraicVector3& flight_direction, double fligt_distance, const reco::Candidate::LorentzVector& visP4)
  {
//--- add penalty term to -log(likelihood)
//    in case the flight correction moves the SV 'behind' the primary vertex.

    return 0.;

    //double penalty = 0.;
    //if ( (flight_direction(0)*visP4.px() + flight_direction(1)*visP4.py() + flight_direction(2)*visP4.pz()) < 0. ) {
    //  penalty += 1.e+4*fligt_distance*fligt_distance;
    //}
    //
    //return penalty;
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
    
    double nll;
    if ( prob > 0. ) nll = -TMath::Log(prob);  
    else nll = 1.e+37;

    if ( verbosity >= 2 ) {
      std::cout << "<negLogLikelihoodTauToLepDecay>:" << std::endl;
      std::cout << " -log(P) = " << nll << std::endl;
    }
    
    return nll;
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
    
    double nll;
    if ( prob > 0. ) nll = -TMath::Log(prob);  
    else nll = 1.e+37;

    if ( verbosity >= 2 ) {
      std::cout << "<negLogLikelihoodTauToHadDecay>:" << std::endl;
      std::cout << " -log(P) = " << nll << std::endl;
    }
    
    return nll;
  }
  
  double negLogLikelihoodTrackInfo1Prong(const AlgebraicVector3& eventVertexPos, const AlgebraicMatrix33& eventVertexCov,
					 double pTau_lab, const reco::Candidate::LorentzVector& p4Vis, const AlgebraicVector3& tauFlightPath_unit, 
					 const reco::TransientTrack& track_trajectory, 
					 const AlgebraicVector3& track_refPoint, const AlgebraicVector3& track_direction)
  {
    if ( verbosity >= 2 ) {
      std::cout << "<negLogLikelihoodTrackInfo1Prong>:" << std::endl;
      printVector("tauFlightPath_unit", tauFlightPath_unit);
    }

    long numWarnings = 0;
    AlgebraicVector3 pcaPos0 = compIntersection_of_lines(eventVertexPos, tauFlightPath_unit, track_refPoint, track_direction, numWarnings, verbosity);
    if ( verbosity >= 2 ) {
      AlgebraicVector3 pcaPos_wrt_vertex = pcaPos0 - eventVertexPos;
      std::cout << "pcaPos(initial):" 
    	        << " x = " << pcaPos0(0) << ", y = " << pcaPos0(1) << ", z = " << pcaPos0(2) 
    	        << " (d = " << TMath::Sqrt(norm2(pcaPos_wrt_vertex)) << ")" << std::endl;      
    }
        
    AlgebraicVector3 pcaPos = pcaPos0;
    int iteration = 0;
    const int maxIterations = 10;
    int errorFlag = 0;
    bool hasConverged = false;     
    do {
      if ( norm2(pcaPos) < 1.e+4 ) {
	SVfitTrackExtrapolation track_extrapolation_update(track_trajectory, pcaPos);
	errorFlag |= track_extrapolation_update.errorFlag();
	if ( !errorFlag ) {
	  const AlgebraicVector3& track_direction_update = track_extrapolation_update.tangent();
	  if ( verbosity >= 2 ) printVector("track_direction_update", track_direction_update);
	  const AlgebraicVector3& track_refPoint_update = track_extrapolation_update.point_of_closest_approach();
	  if ( verbosity >= 2 ) printVector("track_refPoint_update", track_refPoint_update);
	  long numWarnings = 0;
	  AlgebraicVector3 pcaPos_update = compIntersection_of_lines(
	    eventVertexPos, tauFlightPath_unit, track_refPoint_update, track_direction_update, numWarnings, verbosity);
	  if ( verbosity >= 2 ) {
	    AlgebraicVector3 pcaPos_wrt_vertex_update = pcaPos_update - eventVertexPos;
	    std::cout << "pcaPos(iteration #" << iteration << "):" 
		      << " x = " << pcaPos_update(0) << ", y = " << pcaPos_update(1) << ", z = " << pcaPos_update(2) 
		      << " (d = " << TMath::Sqrt(norm2(pcaPos_wrt_vertex_update)) << ")" << std::endl;    
	  }
	  double diff_pca_update2 = norm2(pcaPos_update - pcaPos);
	  if ( verbosity >= 2 ) std::cout << "diff_pca_update2 = " << diff_pca_update2 << std::endl;
	  if ( diff_pca_update2 < 1.e-8 ) {
	    hasConverged = true;	 
	  } else {
	    pcaPos = pcaPos_update;
	    // CV: do not spend computing time running up to 10 iterations for events
	    //     for which no convergence is likely to be reached at the end.
	    if ( diff_pca_update2 > 1. ) iteration = maxIterations;
	  } 
	} 
      } else {
	errorFlag = 1;
      }
      ++iteration;
    } while ( !errorFlag && !hasConverged && iteration < maxIterations ); 
    
    if ( errorFlag || !hasConverged ) pcaPos = pcaPos0;
    if ( verbosity >= 2 ) printVector("pcaPos(final)", pcaPos);
    
    double prob = 0.;
    double decayDistance = TMath::Sqrt(norm2(pcaPos - eventVertexPos));
    if ( verbosity >= 2 ) std::cout << "decayDistance = " << decayDistance << std::endl;
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
      if ( verbosity >= 2 ) {
	//if ( (TMath::Nint(decayDistance_shift/dStep) % 10) == 0 ) {
	if ( TMath::Abs(decayDistance_shift) < (0.5*dStep) ) {
	  std::cout << "decayDistance_shift = " << decayDistance_shift << std::endl;
	  if ( verbosity >= 3 ) {
	    printVector("expectedDecayVertexPos", expectedDecayVertexPos);
	    printMatrix("expectedDecayVertexCov", expectedDecayVertexCov);
	    printVector("reconstructedDecayVertexPos", reconstructedDecayVertexPos);
	    printMatrix("reconstructedDecayVertexCov", reconstructedDecayVertexCov);
	    printMatrix("reconstructed_wrt_expectedDecayVertexCov", reconstructed_wrt_expectedDecayVertexCov);
	    std::cout << "residual: x = " << residual(0) << ", y = " << residual(1) << ", z = " << residual(2) 
		      << " ( |residual| = " << TMath::Sqrt(norm2(residual)) << ")" << std::endl;
	    printMatrix("reconstructed_wrt_expectedDecayVertexCov", reconstructed_wrt_expectedDecayVertexCov);
	    std::cout << "sigma = " << TMath::Sqrt(sigma2) << std::endl;
	  }
	  std::cout << "--> -log(P) = " << nll
		    << " ( -log(P_dca) = " << nll_dca << ", -log(P_lifetime) = " << nll_lifetime << ", -log(P_penalty) = " << nll_penalty << ")" << std::endl;
	}
      }
      prob += (TMath::Exp(-nll)*dStep);
    }
    
    double nll;
    if ( prob > 0. ) nll = -TMath::Log(prob);  
    else nll = 1.e+37;

    if ( verbosity >= 2 ) {
      std::cout << "<negLogLikelihoodTrackInfo1Prong>:" << std::endl;
      std::cout << " -log(P) = " << nll << std::endl;
    }
    
    return nll;
  }
  
  double negLogLikelihoodTrackInfo3Prong(const AlgebraicVector3& eventVertexPos, const AlgebraicMatrix33& eventVertexCov,
					 double pTau_lab, const reco::Candidate::LorentzVector& p4Vis, const AlgebraicVector3& tauFlightPath_unit,
					 const AlgebraicVector3& reconstructedDecayVertexPos, const AlgebraicMatrix33& reconstructedDecayVertexCov)
  {
    if ( verbosity >= 2 ) std::cout << "<negLogLikelihoodTrackInfo3Prong>:" << std::endl;
    AlgebraicVector3 decayVertex_wrt_eventVertexPos = reconstructedDecayVertexPos - eventVertexPos;
    if ( verbosity >= 2 ) printVector("decayVertex_wrt_eventVertexPos", decayVertex_wrt_eventVertexPos);
    double decayDistance_projection = compScalarProduct(decayVertex_wrt_eventVertexPos, tauFlightPath_unit);
    if ( verbosity >= 2 ) std::cout << "decayDistance_projection = " << decayDistance_projection << std::endl;
    AlgebraicVector3 expectedDecayVertexPos = compDecayPosition(eventVertexPos, decayDistance_projection, tauFlightPath_unit);
    if ( verbosity >= 2 ) printVector("expectedDecayVertexPos", expectedDecayVertexPos);
    AlgebraicMatrix33 expectedDecayVertexCov = eventVertexCov;
    if ( verbosity >= 2 ) printMatrix("expectedDecayVertexCov", expectedDecayVertexCov);
    
    AlgebraicVector3 residual = reconstructedDecayVertexPos - expectedDecayVertexPos;
    if ( verbosity >= 2 ) printVector("residual", residual);
    double residual2 = norm2(residual);
    if ( verbosity >= 2 ) std::cout << "residual2 = " << residual2 << std::endl;
    AlgebraicMatrix33 reconstructed_wrt_expectedDecayVertexCov = reconstructedDecayVertexCov;
    for ( unsigned iRow = 0; iRow < 3; ++iRow ) {
      for ( unsigned iColumn = 0; iColumn < 3; ++iColumn ) {
	reconstructed_wrt_expectedDecayVertexCov(iRow, iColumn) += expectedDecayVertexCov(iRow, iColumn);
      }
    }
    if ( verbosity >= 2 ) printMatrix("reconstructed_wrt_expectedDecayVertexCov", reconstructed_wrt_expectedDecayVertexCov);
    double sigma2 = ROOT::Math::Similarity(residual, reconstructed_wrt_expectedDecayVertexCov)/residual2;
    if ( verbosity >= 2 ) std::cout << "sigma2 = " << sigma2 << std::endl;
    double nll_dca = -logGaussian(TMath::Sqrt(residual2), TMath::Sqrt(sigma2));
    double nll_lifetime = negLogLikelihoodExponentialDecay(decayDistance_projection, pTau_lab);	 
    double nll_penalty = backwardsPenaltyTerm(tauFlightPath_unit, decayDistance_projection, p4Vis);
    double nll = nll_dca + nll_lifetime + nll_penalty;
    if ( verbosity >= 2 ) {
      std::cout << "residual: x = " << residual(0) << ", y = " << residual(1) << ", z = " << residual(2) 
    	        << " ( |residual| = " << TMath::Sqrt(norm2(residual)) << ", sigma = " << TMath::Sqrt(sigma2) << ")" << std::endl;
      std::cout << "--> -log(P) = " << nll
    	        << " ( -log(P_dca) = " << nll_dca << ", -log(P_lifetime) = " << nll_lifetime << ", -log(P_penalty) = " << nll_penalty << ")" << std::endl;
    }

    if ( verbosity >= 2 ) {
      std::cout << "<negLogLikelihoodTrackInfo3Prong>:" << std::endl;
      std::cout << " -log(P) = " << nll << std::endl;
    }

    return nll;
  }
  
  double negLogLikelihoodMEt(const reco::Candidate::LorentzVector& genMEt, const reco::Candidate::LorentzVector& recMEt, 
			     double recMEtCovDet, const TMatrixD& recMEtCovInverse)
  {
    // code copied from TauAnalysis/CandidateTools/plugins/NSVfitEventLikelihoodMEt2.cc
    
    double nll;
    if ( recMEtCovDet != 0. ) {
      double residualPx = recMEt.px() - genMEt.px();
      double residualPy = recMEt.py() - genMEt.py();
      
      double term1 = TMath::Log(2.*TMath::Pi()) + 0.5*TMath::Log(TMath::Abs(recMEtCovDet));
      double term2 = 0.5*(residualPx*(recMEtCovInverse(0,0)*residualPx + recMEtCovInverse(0,1)*residualPy)
			  + residualPy*(recMEtCovInverse(1,0)*residualPx + recMEtCovInverse(1,1)*residualPy));
      nll = term1 + term2;
    } else {
      nll = 0.;
    }

    if ( verbosity >= 2 ) {
      std::cout << "<negLogLikelihoodMEt>:" << std::endl;
      std::cout << " -log(P) = " << nll << std::endl;
    }

    return nll;
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
    canvas->Print(std::string(outputFileName_plot).append(".png").data());
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
    canvas->Print(std::string(outputFileName_plot).append(".png").data());
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
  
  void updateParametersToVary(unsigned idx, bool doVaryTheta, bool doVaryX, bool doVaryPhi, bool doVaryInvisMass,
			      double* theta_or_X, int& theta_or_XNumBins, double& theta_or_XMin, double& theta_or_XMax, double theta_or_X_true, 
			      double* phi, int& phiNumBins, double& phiMin, double& phiMax, double phi_true, 
			      double* invisMass, int& invisMassNumBins, double& invisMassMin, double& invisMassMax, double invisMass_true, 
			      double*& parameter1, int& parameter1NumBins, double& parameter1Min, double& parameter1Max, double& parameter1_true, std::string& xAxisLabel,
			      double*& parameter2, int& parameter2NumBins, double& parameter2Min, double& parameter2Max, double& parameter2_true, std::string& yAxisLabel,
			      unsigned& numParametersToVary)
  {
    if ( doVaryTheta || doVaryX ) {
      std::string theta_or_XaxisLabel = "";
      if ( doVaryTheta ) {
	theta_or_XMin = 0.;
	theta_or_XMax = TMath::Pi();
	theta_or_XaxisLabel = "#theta_{%u} / Rad";
      } else {
	theta_or_XMin = 0.;
	theta_or_XMax = 1.;
	theta_or_XaxisLabel = "X_{%u}";
      }
      addParameterReference(idx, theta_or_X, theta_or_XNumBins, theta_or_XMin, theta_or_XMax, theta_or_X_true, theta_or_XaxisLabel.data(),
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
			  const reco::Candidate::LorentzVector& recP4Vis1, const reco::TransientTrack* recLeadTrack1_trajectory, 
			  bool hasRecDecayVertex1, const AlgebraicVector3& recDecayVertexPos1, const AlgebraicMatrix33& recDecayVertexCov1, 
			  bool doVaryTheta1, bool doVaryX1, bool doVaryPhi1, bool doVaryInvisMass1,
			  bool addLikelihoodTauDecayKine1, bool addLikelihoodTrackInfo1, 
			  const reco::Candidate::LorentzVector& genP4Vis2, const reco::Candidate::LorentzVector& genP4Invis2, const std::string& genTauDecayMode2,
			  const AlgebraicVector3& genProdVertexPos2, const AlgebraicVector3& genDecayVertexPos2, 
			  const reco::Candidate::LorentzVector& recP4Vis2, const reco::TransientTrack* recLeadTrack2_trajectory, 
			  bool hasRecDecayVertex2, const AlgebraicVector3& recDecayVertexPos2, const AlgebraicMatrix33& recDecayVertexCov2, 
			  bool doVaryTheta2, bool doVaryX2, bool doVaryPhi2, bool doVaryInvisMass2,
			  bool addLikelihoodTauDecayKine2, bool addLikelihoodTrackInfo2, 
			  const AlgebraicVector3& eventVertexPos, const AlgebraicMatrix33& eventVertexCov,
			  const reco::Candidate::LorentzVector& recMEt, const TMatrixD& recMEtCov,
			  bool addLikelihoodMEt,			  
			  const edm::Event& evt, const std::string& outputFileName)
  {
    std::cout << "<makeLikelihoodPlot>:" << std::endl;
        
    if ( (doVaryTheta1 && doVaryX1) ||
	 (doVaryTheta2 && doVaryX2) )
      throw cms::Exception("makeLikelihoodPlot")
	<< " Cannot vary gjAngle and X simultaneously !!\n";

    SVfitTrackExtrapolation recLeadTrack1_extrapolation(*recLeadTrack1_trajectory, eventVertexPos);
    const AlgebraicVector3& recLeadTrackDirection1 = recLeadTrack1_extrapolation.tangent();
    const AlgebraicVector3& recLeadTrackRefPoint1 = eventVertexPos;
        
    SVfitTrackExtrapolation recLeadTrack2_extrapolation(*recLeadTrack2_trajectory, eventVertexPos);
    const AlgebraicVector3& recLeadTrackDirection2 = recLeadTrack2_extrapolation.tangent();
    const AlgebraicVector3& recLeadTrackRefPoint2 = eventVertexPos;
    
    double recMEtCovDet = recMEtCov.Determinant();
    TMatrixD recMEtCovInverse = recMEtCov;
    if ( recMEtCovDet != 0. ) recMEtCovInverse.Invert();
    
    reco::Candidate::LorentzVector genP4Tau1 = genP4Vis1 + genP4Invis1;
    double gjAngle_or_X1_true;
    bool isX1;
    if ( doVaryX1 ) {
      gjAngle_or_X1_true    = genP4Vis1.E()/genP4Tau1.E();
      isX1                  = true;
    } else {
      gjAngle_or_X1_true    = gjAngleFromLabMomenta(genP4Tau1, genP4Vis1);
      isX1                  = false;
    }
    double gjAngle_or_X1Min = gjAngle_or_X1_true;
    double gjAngle_or_X1Max = gjAngle_or_X1_true;
    double phi_lab1_true    = phiLabFromLabMomenta(genP4Tau1, genP4Vis1);
    double phi_lab1Min      = phi_lab1_true;
    double phi_lab1Max      = phi_lab1_true;
    double invisMass1_true  = genP4Invis1.mass();
    double invisMass1Min    = invisMass1_true;
    double invisMass1Max    = invisMass1_true;
    
    reco::Candidate::LorentzVector genP4Tau2 = genP4Vis2 + genP4Invis2;
    double gjAngle_or_X2_true;
    bool isX2;
    if ( doVaryX2 ) {
      gjAngle_or_X2_true    = genP4Vis2.E()/genP4Tau2.E();
      isX2                  = true;
    } else {
      gjAngle_or_X2_true    = gjAngleFromLabMomenta(genP4Tau2, genP4Vis2);
      isX2                  = false;
    }
    double gjAngle_or_X2Min = gjAngle_or_X2_true;
    double gjAngle_or_X2Max = gjAngle_or_X2_true;
    double phi_lab2_true    = phiLabFromLabMomenta(genP4Tau2, genP4Vis2);
    double phi_lab2Min      = phi_lab2_true;
    double phi_lab2Max      = phi_lab2_true;
    double invisMass2_true  = genP4Invis2.mass();
    double invisMass2Min    = invisMass2_true;
    double invisMass2Max    = invisMass2_true;
    
    int gjAngle_or_XNumBins = 100;
    int phi_labNumBins      = 100;
    int invisMassNumBins    = 100;
    
    double gjAngle_or_X1, phi_lab1, invisMass1;
    double gjAngle_or_X2, phi_lab2, invisMass2;
    
    double* parameter1 = 0;
    int parameter1NumBins;
    double parameter1Min, parameter1Max, parameter1_true; 
    std::string xAxisLabel;
    double* parameter2 = 0;
    int parameter2NumBins;
    double parameter2Min, parameter2Max, parameter2_true; 
    std::string yAxisLabel;
    unsigned numParametersToVary = 0;
    updateParametersToVary(1, doVaryTheta1, doVaryX1, doVaryPhi1, doVaryInvisMass1, 
			   &gjAngle_or_X1, gjAngle_or_XNumBins, gjAngle_or_X1Min, gjAngle_or_X1Max, gjAngle_or_X1_true,
			   &phi_lab1, phi_labNumBins, phi_lab1Min, phi_lab1Max, phi_lab1_true, 
			   &invisMass1, invisMassNumBins, invisMass1Min, invisMass1Max, invisMass1_true, 
			   parameter1, parameter1NumBins, parameter1Min, parameter1Max, parameter1_true, xAxisLabel, 
			   parameter2, parameter2NumBins, parameter2Min, parameter2Max, parameter2_true, yAxisLabel,
			   numParametersToVary);
    updateParametersToVary(2, doVaryTheta2, doVaryX2, doVaryPhi2, doVaryInvisMass2, 
			   &gjAngle_or_X2, gjAngle_or_XNumBins, gjAngle_or_X2Min, gjAngle_or_X2Max, gjAngle_or_X2_true,
			   &phi_lab2, phi_labNumBins, phi_lab2Min, phi_lab2Max, phi_lab2_true, 
			   &invisMass2, invisMassNumBins, invisMass2Min, invisMass2Max, invisMass2_true, 
			   parameter1, parameter1NumBins, parameter1Min, parameter1Max, parameter1_true, xAxisLabel, 
			   parameter2, parameter2NumBins, parameter2Min, parameter2Max, parameter2_true, yAxisLabel,
			   numParametersToVary);
    
    int gjAngle_or_XNumBins1 = ( gjAngle_or_X1Max > gjAngle_or_X1Min ) ? gjAngle_or_XNumBins : 1;
    int phi_labNumBins1      = ( phi_lab1Max      > phi_lab1Min      ) ? phi_labNumBins      : 1;
    int invisMassNumBins1    = ( invisMass1Max    > invisMass1Min    ) ? invisMassNumBins    : 1;
    std::cout << " numBins(1): gjAngle/X = " << gjAngle_or_XNumBins1 << ", phi_lab = " << phi_labNumBins1 << ", invisMass = " << invisMassNumBins1 << std::endl;
    
    int gjAngle_or_XNumBins2 = ( gjAngle_or_X2Max > gjAngle_or_X2Min ) ? gjAngle_or_XNumBins : 1;
    int phi_labNumBins2      = ( phi_lab2Max      > phi_lab2Min      ) ? phi_labNumBins      : 1;
    int invisMassNumBins2    = ( invisMass2Max    > invisMass2Min    ) ? invisMassNumBins    : 1;
    std::cout << " numBins(2): gjAngle/X = " << gjAngle_or_XNumBins2 << ", phi_lab = " << phi_labNumBins2 << ", invisMass = " << invisMassNumBins2 << std::endl;
    
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
    for ( int gjAngle_or_XBin1 = 0; gjAngle_or_XBin1 < gjAngle_or_XNumBins1; ++gjAngle_or_XBin1 ) { 
    //for ( double gjAngle_or_X1 = 0.494759; gjAngle_or_X1 <= 0.5; gjAngle_or_X1 += 0.5 ) {
      for ( int phi_labBin1 = 0; phi_labBin1 < phi_labNumBins1; ++phi_labBin1 ) {
	for ( int invisMassBin1 = 0; invisMassBin1 < invisMassNumBins1; ++invisMassBin1 ) {
	  for ( int gjAngle_or_XBin2 = 0; gjAngle_or_XBin2 < gjAngle_or_XNumBins2; ++gjAngle_or_XBin2 ) {    
	  //for ( double gjAngle_or_X2 = 0.527036; gjAngle_or_X2 <= 0.6; gjAngle_or_X2 += 0.5 ) {
	    for ( int phi_labBin2 = 0; phi_labBin2 < phi_labNumBins2; ++phi_labBin2 ) {
	      for ( int invisMassBin2 = 0; invisMassBin2 < invisMassNumBins2; ++invisMassBin2 ) {
		
		if ( numPoints > 0 && (numPoints % 1000) == 0 ) std::cout << " computing point = " << numPoints << std::endl;
		
		gjAngle_or_X1 = gjAngle_or_X1Min + (gjAngle_or_XBin1 + 0.5)*(gjAngle_or_X1Max - gjAngle_or_X1Min)/gjAngle_or_XNumBins1;
		phi_lab1      = phi_lab1Min      + (phi_labBin1      + 0.5)*(phi_lab1Max      - phi_lab1Min     )/phi_labNumBins1;
		invisMass1    = invisMass1Min    + (invisMassBin1    + 0.5)*(invisMass1Max    - invisMass1Min   )/invisMassNumBins1;
		double pVis_rf1, gjAngle1;
		reco::Candidate::Vector tauFlight1;
		bool isPhysicalSolution1;
		//if ( isX1 ) std::cout << "X1 = " << gjAngle_or_X1 << std::endl;
		reco::Candidate::LorentzVector p4Tau1 = compTauP4(gjAngle_or_X1, isX1, phi_lab1, recP4Vis1, invisMass1, tauLeptonMass, pVis_rf1, gjAngle1, tauFlight1, isPhysicalSolution1);
		double X1 = recP4Vis1.E()/p4Tau1.E();
		double tauFlight1_mag = TMath::Sqrt(tauFlight1.mag2());
		AlgebraicVector3 tauFlightPath1_unit(tauFlight1.x()/tauFlight1_mag, tauFlight1.y()/tauFlight1_mag, tauFlight1.z()/tauFlight1_mag);
		
		gjAngle_or_X2 = gjAngle_or_X2Min + (gjAngle_or_XBin2 + 0.5)*(gjAngle_or_X2Max - gjAngle_or_X2Min)/gjAngle_or_XNumBins2;
		phi_lab2      = phi_lab2Min      + (phi_labBin2      + 0.5)*(phi_lab2Max      - phi_lab2Min     )/phi_labNumBins2;
		invisMass2    = invisMass2Min    + (invisMassBin2    + 0.5)*(invisMass2Max    - invisMass2Min   )/invisMassNumBins2;
		double pVis_rf2, gjAngle2;
		reco::Candidate::Vector tauFlight2;
		bool isPhysicalSolution2;
		//if ( isX2 ) std::cout << "X2 = " << gjAngle_or_X2 << std::endl;
		reco::Candidate::LorentzVector p4Tau2 = compTauP4(gjAngle_or_X2, isX2, phi_lab2, recP4Vis2, invisMass2, tauLeptonMass, pVis_rf2, gjAngle2, tauFlight2, isPhysicalSolution2);		
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
		  if ( hasRecDecayVertex1 ) negLogP += negLogLikelihoodTrackInfo3Prong(
		    eventVertexPos, eventVertexCov, p4Tau1.P(), recP4Vis1, tauFlightPath1_unit, recDecayVertexPos1, recDecayVertexCov1);
		  else negLogP += negLogLikelihoodTrackInfo1Prong(
		    eventVertexPos, eventVertexCov, p4Tau1.P(), recP4Vis1, tauFlightPath1_unit, *recLeadTrack1_trajectory, recLeadTrackRefPoint1, recLeadTrackDirection1);
		}
		
		if ( addLikelihoodTauDecayKine2 ) {
		  if ( isLeptonicDecay2 ) negLogP += negLogLikelihoodTauToLepDecay(gjAngle2, X2, invisMass2);
		  else negLogP += negLogLikelihoodTauToHadDecay(gjAngle2, pVis_rf2, X2, recP4Vis2.mass());
		}
		if ( addLikelihoodTrackInfo2 ) {
		  if ( hasRecDecayVertex2 ) negLogP += negLogLikelihoodTrackInfo3Prong(
		    eventVertexPos, eventVertexCov, p4Tau2.P(), recP4Vis2, tauFlightPath2_unit, recDecayVertexPos2, recDecayVertexCov2);
		  else negLogP += negLogLikelihoodTrackInfo1Prong(
		    eventVertexPos, eventVertexCov, p4Tau2.P(), recP4Vis2, tauFlightPath2_unit, *recLeadTrack2_trajectory, recLeadTrackRefPoint2, recLeadTrackDirection2);
		}
		
		if ( addLikelihoodMEt ) {		
		  negLogP += negLogLikelihoodMEt(genMEt, recMEt, recMEtCovDet, recMEtCovInverse);
		}

		if ( !(isPhysicalSolution1 && isPhysicalSolution2) ) negLogP += 1.e+37;
		
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
	//std::cout << "X1 = " << xAxis->GetBinCenter(iBinX) << ", X2 = " << yAxis->GetBinCenter(iBinY) << ":"
	//	    << " binContent = " << binContent << std::endl;
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

  eventVertexFitAlgorithm_->beginEvent(evt, es);
  decayVertexFitAlgorithm_->beginEvent(evt, es);

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
    matchRecToGenTauDecays(*electrons, genTaus, 0.3, matchedTauDecays, trackBuilder, decayVertexFitAlgorithm_);
  }

  edm::Handle<CandidateView> muons;
  if ( srcMuons_.label() != "" ) {
    evt.getByLabel(srcMuons_, muons);
    std::cout << "#muons = " << muons->size() << std::endl;
    matchRecToGenTauDecays(*muons, genTaus, 0.3, matchedTauDecays, trackBuilder, decayVertexFitAlgorithm_);
  }

  edm::Handle<CandidateView> taus;
  if ( srcTaus_.label() != "" ) {
    evt.getByLabel(srcTaus_, taus);
    std::cout << "#taus = " << taus->size() << std::endl;
    matchRecToGenTauDecays(*taus, genTaus, 0.3, matchedTauDecays, trackBuilder, decayVertexFitAlgorithm_);
  }

  std::cout << "#matchedTauDecays = " << matchedTauDecays.size() << std::endl;
  if ( !(matchedTauDecays.size() == 2) ) 
    std::cout << " Failed to find exactly two gen. matched Tau decay products --> skipping !!" << std::endl;

  const matchedTauDecayType* matchedTau1 = matchedTauDecays.at(0);
  const matchedTauDecayType* matchedTau2 = matchedTauDecays.at(1);
  assert(matchedTau1 && matchedTau2);
  matchedTau1->print(std::cout);
  matchedTau2->print(std::cout);

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
  TransientVertex vertex_refitted = eventVertexFitAlgorithm_->refit(&vertex, &tauTracks);
  AlgebraicVector3 vertexPos_refitted;
  AlgebraicMatrix33 vertexCov_refitted;
  if ( vertex_refitted.isValid() ) {
    vertexPos_refitted = AlgebraicVector3(vertex_refitted.position().x(), vertex_refitted.position().y(), vertex_refitted.position().z());
    vertexCov_refitted = vertex_refitted.positionError().matrix_new();
    std::cout << "event vertex(rec,refitted): x = " << vertexPos_refitted(0) << ", y = " << vertexPos_refitted(1) << ", z = " << vertexPos_refitted(2) 
	      << " (chi2 = " << vertex_refitted.normalisedChiSquared() << ")" << std::endl;    
  } else {
    std::cout << "event vertex(rec,refitted): NA (vertex fit failed)" << std::endl;
  }
/*
  makeLikelihoodPlot(matchedTau1->genVisP4_, matchedTau1->genInvisP4_, matchedTau1->genTauDecayMode_, matchedTau1->genTauProdVertexPos_, matchedTau1->genTauDecayVertexPos_, 
		     matchedTau1->recVisP4_, matchedTau1->recLeadTrackTrajectory_, matchedTau1->hasRecTauDecayVertex_, matchedTau1->recTauDecayVertexPos_, matchedTau1->recTauDecayVertexCov_, 
		     true, false, false, matchedTau1->isLeptonicDecay_, 
		     true, false,
		     matchedTau2->genVisP4_, matchedTau2->genInvisP4_, matchedTau2->genTauDecayMode_, matchedTau2->genTauProdVertexPos_, matchedTau2->genTauDecayVertexPos_, 
		     matchedTau2->recVisP4_, matchedTau2->recLeadTrackTrajectory_, matchedTau2->hasRecTauDecayVertex_, matchedTau2->recTauDecayVertexPos_, matchedTau2->recTauDecayVertexCov_, 
		     false, false, false, false,
		     false, false, 
		     vertexPos_refitted, vertexCov_refitted,
		     recMEt.p4(), recMEtCov,
		     false,
		     evt, std::string(moduleLabel_).append("_TauDecayKine1_vs_gjAngle1_and_Mnunu1"));
  makeLikelihoodPlot(matchedTau1->genVisP4_, matchedTau1->genInvisP4_, matchedTau1->genTauDecayMode_, matchedTau1->genTauProdVertexPos_, matchedTau1->genTauDecayVertexPos_, 
		     matchedTau1->recVisP4_, matchedTau1->recLeadTrackTrajectory_, matchedTau1->hasRecTauDecayVertex_, matchedTau1->recTauDecayVertexPos_, matchedTau1->recTauDecayVertexCov_, 
		     false, true, false, matchedTau1->isLeptonicDecay_, 
		     true, false,
		     matchedTau2->genVisP4_, matchedTau2->genInvisP4_, matchedTau2->genTauDecayMode_, matchedTau2->genTauProdVertexPos_, matchedTau2->genTauDecayVertexPos_, 
		     matchedTau2->recVisP4_, matchedTau2->recLeadTrackTrajectory_, matchedTau2->hasRecTauDecayVertex_, matchedTau2->recTauDecayVertexPos_, matchedTau2->recTauDecayVertexCov_, 
		     false, false, false, false,
		     false, false, 
		     vertexPos_refitted, vertexCov_refitted,
		     recMEt.p4(), recMEtCov,
		     false,
		     evt, std::string(moduleLabel_).append("_TauDecayKine1_vs_X1_and_Mnunu1"));
  makeLikelihoodPlot(matchedTau1->genVisP4_, matchedTau1->genInvisP4_, matchedTau1->genTauDecayMode_, matchedTau1->genTauProdVertexPos_, matchedTau1->genTauDecayVertexPos_, 
		     matchedTau1->recVisP4_, matchedTau1->recLeadTrackTrajectory_, false, matchedTau1->recTauDecayVertexPos_, matchedTau1->recTauDecayVertexCov_, 
		     true, false, true, false, 
		     false, true, 
		     matchedTau2->genVisP4_, matchedTau2->genInvisP4_, matchedTau2->genTauDecayMode_, matchedTau2->genTauProdVertexPos_, matchedTau2->genTauDecayVertexPos_, 
		     matchedTau2->recVisP4_, matchedTau2->recLeadTrackTrajectory_, matchedTau2->hasRecTauDecayVertex_, matchedTau2->recTauDecayVertexPos_, matchedTau2->recTauDecayVertexCov_, 
		     false, false, false, false,
		     false, false, 
		     vertexPos_refitted, vertexCov_refitted,
		     recMEt.p4(), recMEtCov,
		     false,
		     evt, std::string(moduleLabel_).append("_TrackInfo1dca_vs_gjAngle1_and_phiLab1"));
  makeLikelihoodPlot(matchedTau1->genVisP4_, matchedTau1->genInvisP4_, matchedTau1->genTauDecayMode_, matchedTau1->genTauProdVertexPos_, matchedTau1->genTauDecayVertexPos_, 
		     matchedTau1->recVisP4_, matchedTau1->recLeadTrackTrajectory_, false, matchedTau1->recTauDecayVertexPos_, matchedTau1->recTauDecayVertexCov_, 
		     false, true, true, false, 
		     false, true, 
		     matchedTau2->genVisP4_, matchedTau2->genInvisP4_, matchedTau2->genTauDecayMode_, matchedTau2->genTauProdVertexPos_, matchedTau2->genTauDecayVertexPos_, 
		     matchedTau2->recVisP4_, matchedTau2->recLeadTrackTrajectory_, matchedTau2->hasRecTauDecayVertex_, matchedTau2->recTauDecayVertexPos_, matchedTau2->recTauDecayVertexCov_, 
		     false, false, false, false,
		     false, false, 
		     vertexPos_refitted, vertexCov_refitted,
		     recMEt.p4(), recMEtCov,
		     false,
		     evt, std::string(moduleLabel_).append("_TrackInfo1dca_vs_X1_and_phiLab1"));
  makeLikelihoodPlot(matchedTau1->genVisP4_, matchedTau1->genInvisP4_, matchedTau1->genTauDecayMode_, matchedTau1->genTauProdVertexPos_, matchedTau1->genTauDecayVertexPos_, 
		     matchedTau1->recVisP4_, matchedTau1->recLeadTrackTrajectory_, matchedTau1->hasRecTauDecayVertex_, matchedTau1->recTauDecayVertexPos_, matchedTau1->recTauDecayVertexCov_, 
		     true, false, true, false, 
		     false, true, 
		     matchedTau2->genVisP4_, matchedTau2->genInvisP4_, matchedTau2->genTauDecayMode_, matchedTau2->genTauProdVertexPos_, matchedTau2->genTauDecayVertexPos_, 
		     matchedTau2->recVisP4_, matchedTau2->recLeadTrackTrajectory_, matchedTau2->hasRecTauDecayVertex_, matchedTau2->recTauDecayVertexPos_, matchedTau2->recTauDecayVertexCov_, 
		     false, false, false, false,
		     false, false, 
		     vertexPos_refitted, vertexCov_refitted,
		     recMEt.p4(), recMEtCov,
		     false,
		     evt, std::string(moduleLabel_).append("_TrackInfo1vtx_vs_gjAngle1_and_phiLab1"));
  makeLikelihoodPlot(matchedTau1->genVisP4_, matchedTau1->genInvisP4_, matchedTau1->genTauDecayMode_, matchedTau1->genTauProdVertexPos_, matchedTau1->genTauDecayVertexPos_, 
		     matchedTau1->recVisP4_, matchedTau1->recLeadTrackTrajectory_, matchedTau1->hasRecTauDecayVertex_, matchedTau1->recTauDecayVertexPos_, matchedTau1->recTauDecayVertexCov_, 
		     false, true, true, false, 
		     false, true, 
		     matchedTau2->genVisP4_, matchedTau2->genInvisP4_, matchedTau2->genTauDecayMode_, matchedTau2->genTauProdVertexPos_, matchedTau2->genTauDecayVertexPos_, 
		     matchedTau2->recVisP4_, matchedTau2->recLeadTrackTrajectory_, matchedTau2->hasRecTauDecayVertex_, matchedTau2->recTauDecayVertexPos_, matchedTau2->recTauDecayVertexCov_, 
		     false, false, false, false,
		     false, false, 
		     vertexPos_refitted, vertexCov_refitted,
		     recMEt.p4(), recMEtCov,
		     false,
		     evt, std::string(moduleLabel_).append("_TrackInfo1vtx_vs_X1_and_phiLab1"));
  makeLikelihoodPlot(matchedTau1->genVisP4_, matchedTau1->genInvisP4_, matchedTau1->genTauDecayMode_, matchedTau1->genTauProdVertexPos_, matchedTau1->genTauDecayVertexPos_, 
		     matchedTau1->recVisP4_, matchedTau1->recLeadTrackTrajectory_, matchedTau1->hasRecTauDecayVertex_, matchedTau1->recTauDecayVertexPos_, matchedTau1->recTauDecayVertexCov_, 
		     false, false, false, false,
		     false, false, 
		     matchedTau2->genVisP4_, matchedTau2->genInvisP4_, matchedTau2->genTauDecayMode_, matchedTau2->genTauProdVertexPos_, matchedTau2->genTauDecayVertexPos_, 
		     matchedTau2->recVisP4_, matchedTau2->recLeadTrackTrajectory_, matchedTau2->hasRecTauDecayVertex_, matchedTau2->recTauDecayVertexPos_, matchedTau2->recTauDecayVertexCov_, 
		     true, false, false, matchedTau2->isLeptonicDecay_, 
		     true, false,		     
		     vertexPos_refitted, vertexCov_refitted,
		     recMEt.p4(), recMEtCov,
		     false,
		     evt, std::string(moduleLabel_).append("_TauDecayKine2_vs_gjAngle2_and_Mnunu2"));
  makeLikelihoodPlot(matchedTau1->genVisP4_, matchedTau1->genInvisP4_, matchedTau1->genTauDecayMode_, matchedTau1->genTauProdVertexPos_, matchedTau1->genTauDecayVertexPos_, 
		     matchedTau1->recVisP4_, matchedTau1->recLeadTrackTrajectory_, matchedTau1->hasRecTauDecayVertex_, matchedTau1->recTauDecayVertexPos_, matchedTau1->recTauDecayVertexCov_, 
		     false, false, false, false,
		     false, false, 
		     matchedTau2->genVisP4_, matchedTau2->genInvisP4_, matchedTau2->genTauDecayMode_, matchedTau2->genTauProdVertexPos_, matchedTau2->genTauDecayVertexPos_, 
		     matchedTau2->recVisP4_, matchedTau2->recLeadTrackTrajectory_, matchedTau2->hasRecTauDecayVertex_, matchedTau2->recTauDecayVertexPos_, matchedTau2->recTauDecayVertexCov_, 
		     false, true, false, matchedTau2->isLeptonicDecay_, 
		     true, false,		     
		     vertexPos_refitted, vertexCov_refitted,
		     recMEt.p4(), recMEtCov,
		     false,
		     evt, std::string(moduleLabel_).append("_TauDecayKine2_vs_X2_and_Mnunu2"));
  makeLikelihoodPlot(matchedTau1->genVisP4_, matchedTau1->genInvisP4_, matchedTau1->genTauDecayMode_, matchedTau1->genTauProdVertexPos_, matchedTau1->genTauDecayVertexPos_, 
		     matchedTau1->recVisP4_, matchedTau1->recLeadTrackTrajectory_, matchedTau1->hasRecTauDecayVertex_, matchedTau1->recTauDecayVertexPos_, matchedTau1->recTauDecayVertexCov_, 
		     false, false, false, false,
		     false, false, 
		     matchedTau2->genVisP4_, matchedTau2->genInvisP4_, matchedTau2->genTauDecayMode_, matchedTau2->genTauProdVertexPos_, matchedTau2->genTauDecayVertexPos_, 
		     matchedTau2->recVisP4_, matchedTau2->recLeadTrackTrajectory_, false, matchedTau2->recTauDecayVertexPos_, matchedTau2->recTauDecayVertexCov_, 
		     true, false, true, false, 
		     false, true, 
		     vertexPos_refitted, vertexCov_refitted,
		     recMEt.p4(), recMEtCov,
		     false,
		     evt, std::string(moduleLabel_).append("_TrackInfo2dca_vs_gjAngle2_and_phiLab2"));
  makeLikelihoodPlot(matchedTau1->genVisP4_, matchedTau1->genInvisP4_, matchedTau1->genTauDecayMode_, matchedTau1->genTauProdVertexPos_, matchedTau1->genTauDecayVertexPos_, 
		     matchedTau1->recVisP4_, matchedTau1->recLeadTrackTrajectory_, matchedTau1->hasRecTauDecayVertex_, matchedTau1->recTauDecayVertexPos_, matchedTau1->recTauDecayVertexCov_, 
		     false, false, false, false,
		     false, false, 
		     matchedTau2->genVisP4_, matchedTau2->genInvisP4_, matchedTau2->genTauDecayMode_, matchedTau2->genTauProdVertexPos_, matchedTau2->genTauDecayVertexPos_, 
		     matchedTau2->recVisP4_, matchedTau2->recLeadTrackTrajectory_, false, matchedTau2->recTauDecayVertexPos_, matchedTau2->recTauDecayVertexCov_, 
		     false, true, true, false, 
		     false, true, 
		     vertexPos_refitted, vertexCov_refitted,
		     recMEt.p4(), recMEtCov,
		     false,
		     evt, std::string(moduleLabel_).append("_TrackInfo2dca_vs_X2_and_phiLab2"));
  makeLikelihoodPlot(matchedTau1->genVisP4_, matchedTau1->genInvisP4_, matchedTau1->genTauDecayMode_, matchedTau1->genTauProdVertexPos_, matchedTau1->genTauDecayVertexPos_, 
		     matchedTau1->recVisP4_, matchedTau1->recLeadTrackTrajectory_, matchedTau1->hasRecTauDecayVertex_, matchedTau1->recTauDecayVertexPos_, matchedTau1->recTauDecayVertexCov_, 
		     false, false, false, false,
		     false, false, 
		     matchedTau2->genVisP4_, matchedTau2->genInvisP4_, matchedTau2->genTauDecayMode_, matchedTau2->genTauProdVertexPos_, matchedTau2->genTauDecayVertexPos_, 
		     matchedTau2->recVisP4_, matchedTau2->recLeadTrackTrajectory_, matchedTau2->hasRecTauDecayVertex_, matchedTau2->recTauDecayVertexPos_, matchedTau2->recTauDecayVertexCov_, 
		     true, false, true, false, 
		     false, true, 
		     vertexPos_refitted, vertexCov_refitted,
		     recMEt.p4(), recMEtCov,
		     false,
		     evt, std::string(moduleLabel_).append("_TrackInfo2vtx_vs_gjAngle2_and_phiLab2"));
  makeLikelihoodPlot(matchedTau1->genVisP4_, matchedTau1->genInvisP4_, matchedTau1->genTauDecayMode_, matchedTau1->genTauProdVertexPos_, matchedTau1->genTauDecayVertexPos_, 
		     matchedTau1->recVisP4_, matchedTau1->recLeadTrackTrajectory_, matchedTau1->hasRecTauDecayVertex_, matchedTau1->recTauDecayVertexPos_, matchedTau1->recTauDecayVertexCov_, 
		     false, false, false, false,
		     false, false, 
		     matchedTau2->genVisP4_, matchedTau2->genInvisP4_, matchedTau2->genTauDecayMode_, matchedTau2->genTauProdVertexPos_, matchedTau2->genTauDecayVertexPos_, 
		     matchedTau2->recVisP4_, matchedTau2->recLeadTrackTrajectory_, matchedTau2->hasRecTauDecayVertex_, matchedTau2->recTauDecayVertexPos_, matchedTau2->recTauDecayVertexCov_, 
		     false, true, true, false, 
		     false, true, 
		     vertexPos_refitted, vertexCov_refitted,
		     recMEt.p4(), recMEtCov,
		     false,
		     evt, std::string(moduleLabel_).append("_TrackInfo2vtx_vs_X2_and_phiLab2"));
  makeLikelihoodPlot(matchedTau1->genVisP4_, matchedTau1->genInvisP4_, matchedTau1->genTauDecayMode_, matchedTau1->genTauProdVertexPos_, matchedTau1->genTauDecayVertexPos_, 
		     matchedTau1->recVisP4_, matchedTau1->recLeadTrackTrajectory_, matchedTau1->hasRecTauDecayVertex_, matchedTau1->recTauDecayVertexPos_, matchedTau1->recTauDecayVertexCov_, 
		     true, false, true, false,
		     false, false, 
		     matchedTau2->genVisP4_, matchedTau2->genInvisP4_, matchedTau2->genTauDecayMode_, matchedTau2->genTauProdVertexPos_, matchedTau2->genTauDecayVertexPos_, 
		     matchedTau2->recVisP4_, matchedTau2->recLeadTrackTrajectory_, matchedTau2->hasRecTauDecayVertex_, matchedTau2->recTauDecayVertexPos_, matchedTau2->recTauDecayVertexCov_, 
		     false, false, false, false, 
		     false, false, 
		     vertexPos_refitted, vertexCov_refitted,
		     recMEt.p4(), recMEtCov,
		     true,
		     evt, std::string(moduleLabel_).append("_MEt_vs_gjAngle1_and_phiLab1"));
  makeLikelihoodPlot(matchedTau1->genVisP4_, matchedTau1->genInvisP4_, matchedTau1->genTauDecayMode_, matchedTau1->genTauProdVertexPos_, matchedTau1->genTauDecayVertexPos_, 
		     matchedTau1->recVisP4_, matchedTau1->recLeadTrackTrajectory_, matchedTau1->hasRecTauDecayVertex_, matchedTau1->recTauDecayVertexPos_, matchedTau1->recTauDecayVertexCov_, 
		     false, true, true, false,
		     false, false, 
		     matchedTau2->genVisP4_, matchedTau2->genInvisP4_, matchedTau2->genTauDecayMode_, matchedTau2->genTauProdVertexPos_, matchedTau2->genTauDecayVertexPos_, 
		     matchedTau2->recVisP4_, matchedTau2->recLeadTrackTrajectory_, matchedTau2->hasRecTauDecayVertex_, matchedTau2->recTauDecayVertexPos_, matchedTau2->recTauDecayVertexCov_, 
		     false, false, false, false, 
		     false, false, 
		     vertexPos_refitted, vertexCov_refitted,
		     recMEt.p4(), recMEtCov,
		     true,
		     evt, std::string(moduleLabel_).append("_MEt_vs_X1_and_phiLab1"));
  makeLikelihoodPlot(matchedTau1->genVisP4_, matchedTau1->genInvisP4_, matchedTau1->genTauDecayMode_, matchedTau1->genTauProdVertexPos_, matchedTau1->genTauDecayVertexPos_, 
		     matchedTau1->recVisP4_, matchedTau1->recLeadTrackTrajectory_, matchedTau1->hasRecTauDecayVertex_, matchedTau1->recTauDecayVertexPos_, matchedTau1->recTauDecayVertexCov_, 
		     true, false, false, matchedTau1->isLeptonicDecay_, 
		     false, false, 
		     matchedTau2->genVisP4_, matchedTau2->genInvisP4_, matchedTau2->genTauDecayMode_, matchedTau2->genTauProdVertexPos_, matchedTau2->genTauDecayVertexPos_, 
		     matchedTau2->recVisP4_, matchedTau2->recLeadTrackTrajectory_, matchedTau2->hasRecTauDecayVertex_, matchedTau2->recTauDecayVertexPos_, matchedTau2->recTauDecayVertexCov_, 
		     false, false, false, false, 
		     false, false, 
		     vertexPos_refitted, vertexCov_refitted,
		     recMEt.p4(), recMEtCov,
		     true,
		     evt, std::string(moduleLabel_).append("_MEt_vs_gjAngle1_and_Mnunu1"));
  makeLikelihoodPlot(matchedTau1->genVisP4_, matchedTau1->genInvisP4_, matchedTau1->genTauDecayMode_, matchedTau1->genTauProdVertexPos_, matchedTau1->genTauDecayVertexPos_, 
		     matchedTau1->recVisP4_, matchedTau1->recLeadTrackTrajectory_, matchedTau1->hasRecTauDecayVertex_, matchedTau1->recTauDecayVertexPos_, matchedTau1->recTauDecayVertexCov_, 
		     false, true, false, matchedTau1->isLeptonicDecay_, 
		     false, false, 
		     matchedTau2->genVisP4_, matchedTau2->genInvisP4_, matchedTau2->genTauDecayMode_, matchedTau2->genTauProdVertexPos_, matchedTau2->genTauDecayVertexPos_, 
		     matchedTau2->recVisP4_, matchedTau2->recLeadTrackTrajectory_, matchedTau2->hasRecTauDecayVertex_, matchedTau2->recTauDecayVertexPos_, matchedTau2->recTauDecayVertexCov_, 
		     false, false, false, false, 
		     false, false, 
		     vertexPos_refitted, vertexCov_refitted,
		     recMEt.p4(), recMEtCov,
		     true,
		     evt, std::string(moduleLabel_).append("_MEt_vs_X1_and_Mnunu1"));
  makeLikelihoodPlot(matchedTau1->genVisP4_, matchedTau1->genInvisP4_, matchedTau1->genTauDecayMode_, matchedTau1->genTauProdVertexPos_, matchedTau1->genTauDecayVertexPos_, 
		     matchedTau1->recVisP4_, matchedTau1->recLeadTrackTrajectory_, matchedTau1->hasRecTauDecayVertex_, matchedTau1->recTauDecayVertexPos_, matchedTau1->recTauDecayVertexCov_, 
		     false, false, false, false, 
		     false, false, 
		     matchedTau2->genVisP4_, matchedTau2->genInvisP4_, matchedTau2->genTauDecayMode_, matchedTau2->genTauProdVertexPos_, matchedTau2->genTauDecayVertexPos_, 
		     matchedTau2->recVisP4_, matchedTau2->recLeadTrackTrajectory_, matchedTau2->hasRecTauDecayVertex_, matchedTau2->recTauDecayVertexPos_, matchedTau2->recTauDecayVertexCov_, 
		     true, false, true, false,
		     false, false, 		     
		     vertexPos_refitted, vertexCov_refitted,
		     recMEt.p4(), recMEtCov,
		     true,
		     evt, std::string(moduleLabel_).append("_MEt_vs_gjAngle2_and_phiLab2"));
  makeLikelihoodPlot(matchedTau1->genVisP4_, matchedTau1->genInvisP4_, matchedTau1->genTauDecayMode_, matchedTau1->genTauProdVertexPos_, matchedTau1->genTauDecayVertexPos_, 
		     matchedTau1->recVisP4_, matchedTau1->recLeadTrackTrajectory_, matchedTau1->hasRecTauDecayVertex_, matchedTau1->recTauDecayVertexPos_, matchedTau1->recTauDecayVertexCov_, 
		     false, false, false, false, 
		     false, false, 
		     matchedTau2->genVisP4_, matchedTau2->genInvisP4_, matchedTau2->genTauDecayMode_, matchedTau2->genTauProdVertexPos_, matchedTau2->genTauDecayVertexPos_, 
		     matchedTau2->recVisP4_, matchedTau2->recLeadTrackTrajectory_, matchedTau2->hasRecTauDecayVertex_, matchedTau2->recTauDecayVertexPos_, matchedTau2->recTauDecayVertexCov_, 
		     false, true, true, false,
		     false, false, 		     
		     vertexPos_refitted, vertexCov_refitted,
		     recMEt.p4(), recMEtCov,
		     true,
		     evt, std::string(moduleLabel_).append("_MEt_vs_X2_and_phiLab2"));
  makeLikelihoodPlot(matchedTau1->genVisP4_, matchedTau1->genInvisP4_, matchedTau1->genTauDecayMode_, matchedTau1->genTauProdVertexPos_, matchedTau1->genTauDecayVertexPos_, 
		     matchedTau1->recVisP4_, matchedTau1->recLeadTrackTrajectory_, matchedTau1->hasRecTauDecayVertex_, matchedTau1->recTauDecayVertexPos_, matchedTau1->recTauDecayVertexCov_, 
		     false, false, false, false, 
		     false, false, 
		     matchedTau2->genVisP4_, matchedTau2->genInvisP4_, matchedTau2->genTauDecayMode_, matchedTau2->genTauProdVertexPos_, matchedTau2->genTauDecayVertexPos_, 
		     matchedTau2->recVisP4_, matchedTau2->recLeadTrackTrajectory_, matchedTau2->hasRecTauDecayVertex_, matchedTau2->recTauDecayVertexPos_, matchedTau2->recTauDecayVertexCov_, 
		     true, false, false, matchedTau2->isLeptonicDecay_, 
		     false, false, 		     
		     vertexPos_refitted, vertexCov_refitted,
		     recMEt.p4(), recMEtCov,
		     true,
		     evt, std::string(moduleLabel_).append("_MEt_vs_gjAngle2_and_Mnunu2"));
  makeLikelihoodPlot(matchedTau1->genVisP4_, matchedTau1->genInvisP4_, matchedTau1->genTauDecayMode_, matchedTau1->genTauProdVertexPos_, matchedTau1->genTauDecayVertexPos_, 
		     matchedTau1->recVisP4_, matchedTau1->recLeadTrackTrajectory_, matchedTau1->hasRecTauDecayVertex_, matchedTau1->recTauDecayVertexPos_, matchedTau1->recTauDecayVertexCov_, 
		     false, false, false, false, 
		     false, false, 
		     matchedTau2->genVisP4_, matchedTau2->genInvisP4_, matchedTau2->genTauDecayMode_, matchedTau2->genTauProdVertexPos_, matchedTau2->genTauDecayVertexPos_, 
		     matchedTau2->recVisP4_, matchedTau2->recLeadTrackTrajectory_, matchedTau2->hasRecTauDecayVertex_, matchedTau2->recTauDecayVertexPos_, matchedTau2->recTauDecayVertexCov_, 
		     false, true, false, matchedTau2->isLeptonicDecay_, 
		     false, false, 		     
		     vertexPos_refitted, vertexCov_refitted,
		     recMEt.p4(), recMEtCov,
		     true,
		     evt, std::string(moduleLabel_).append("_MEt_vs_X2_and_Mnunu2"));

  makeLikelihoodPlot(matchedTau1->genVisP4_, matchedTau1->genInvisP4_, matchedTau1->genTauDecayMode_, matchedTau1->genTauProdVertexPos_, matchedTau1->genTauDecayVertexPos_, 
		     matchedTau1->recVisP4_, matchedTau1->recLeadTrackTrajectory_, matchedTau1->hasRecTauDecayVertex_, matchedTau1->recTauDecayVertexPos_, matchedTau1->recTauDecayVertexCov_, 
		     true, false, false, false,
		     false, false, 
		     matchedTau2->genVisP4_, matchedTau2->genInvisP4_, matchedTau2->genTauDecayMode_, matchedTau2->genTauProdVertexPos_, matchedTau2->genTauDecayVertexPos_, 
		     matchedTau2->recVisP4_, matchedTau2->recLeadTrackTrajectory_, matchedTau2->hasRecTauDecayVertex_, matchedTau2->recTauDecayVertexPos_, matchedTau2->recTauDecayVertexCov_, 
		     true, false, false, false, 
		     false, false, 
		     vertexPos_refitted, vertexCov_refitted,
		     recMEt.p4(), recMEtCov,
		     true,
		     evt, std::string(moduleLabel_).append("_MEt_vs_gjAngle1_and_gjAngle2"));
  makeLikelihoodPlot(matchedTau1->genVisP4_, matchedTau1->genInvisP4_, matchedTau1->genTauDecayMode_, matchedTau1->genTauProdVertexPos_, matchedTau1->genTauDecayVertexPos_, 
		     matchedTau1->recVisP4_, matchedTau1->recLeadTrackTrajectory_, matchedTau1->hasRecTauDecayVertex_, matchedTau1->recTauDecayVertexPos_, matchedTau1->recTauDecayVertexCov_, 
		     false, true, false, false,
		     false, false, 
		     matchedTau2->genVisP4_, matchedTau2->genInvisP4_, matchedTau2->genTauDecayMode_, matchedTau2->genTauProdVertexPos_, matchedTau2->genTauDecayVertexPos_, 
		     matchedTau2->recVisP4_, matchedTau2->recLeadTrackTrajectory_, matchedTau2->hasRecTauDecayVertex_, matchedTau2->recTauDecayVertexPos_, matchedTau2->recTauDecayVertexCov_, 
		     false, true, false, false, 
		     false, false, 
		     vertexPos_refitted, vertexCov_refitted,
		     recMEt.p4(), recMEtCov,
		     true,
		     evt, std::string(moduleLabel_).append("_MEt_vs_X1_and_X2"));
  makeLikelihoodPlot(matchedTau1->genVisP4_, matchedTau1->genInvisP4_, matchedTau1->genTauDecayMode_, matchedTau1->genTauProdVertexPos_, matchedTau1->genTauDecayVertexPos_, 
		     matchedTau1->recVisP4_, matchedTau1->recLeadTrackTrajectory_, matchedTau1->hasRecTauDecayVertex_, matchedTau1->recTauDecayVertexPos_, matchedTau1->recTauDecayVertexCov_, 
		     true, false, false, false,
		     true, false, 
		     matchedTau2->genVisP4_, matchedTau2->genInvisP4_, matchedTau2->genTauDecayMode_, matchedTau2->genTauProdVertexPos_, matchedTau2->genTauDecayVertexPos_, 
		     matchedTau2->recVisP4_, matchedTau2->recLeadTrackTrajectory_, matchedTau2->hasRecTauDecayVertex_, matchedTau2->recTauDecayVertexPos_, matchedTau2->recTauDecayVertexCov_, 
		     true, false, false, false, 
		     true, false, 
		     vertexPos_refitted, vertexCov_refitted,
		     recMEt.p4(), recMEtCov,
		     true,
		     evt, std::string(moduleLabel_).append("_TauDecayKine_plus_MEt_vs_gjAngle1_and_gjAngle2"));
  makeLikelihoodPlot(matchedTau1->genVisP4_, matchedTau1->genInvisP4_, matchedTau1->genTauDecayMode_, matchedTau1->genTauProdVertexPos_, matchedTau1->genTauDecayVertexPos_, 
		     matchedTau1->recVisP4_, matchedTau1->recLeadTrackTrajectory_, matchedTau1->hasRecTauDecayVertex_, matchedTau1->recTauDecayVertexPos_, matchedTau1->recTauDecayVertexCov_, 
		     false, true, false, false,
		     true, false, 
		     matchedTau2->genVisP4_, matchedTau2->genInvisP4_, matchedTau2->genTauDecayMode_, matchedTau2->genTauProdVertexPos_, matchedTau2->genTauDecayVertexPos_, 
		     matchedTau2->recVisP4_, matchedTau2->recLeadTrackTrajectory_, matchedTau2->hasRecTauDecayVertex_, matchedTau2->recTauDecayVertexPos_, matchedTau2->recTauDecayVertexCov_, 
		     false, true, false, false, 
		     true, false, 
		     vertexPos_refitted, vertexCov_refitted,
		     recMEt.p4(), recMEtCov,
		     true,
		     evt, std::string(moduleLabel_).append("_TauDecayKine_plus_MEt_vs_X1_and_X2"));
  makeLikelihoodPlot(matchedTau1->genVisP4_, matchedTau1->genInvisP4_, matchedTau1->genTauDecayMode_, matchedTau1->genTauProdVertexPos_, matchedTau1->genTauDecayVertexPos_, 
		     matchedTau1->recVisP4_, matchedTau1->recLeadTrackTrajectory_, matchedTau1->hasRecTauDecayVertex_, matchedTau1->recTauDecayVertexPos_, matchedTau1->recTauDecayVertexCov_, 
		     true, false, false, false,
		     true, true, 
		     matchedTau2->genVisP4_, matchedTau2->genInvisP4_, matchedTau2->genTauDecayMode_, matchedTau2->genTauProdVertexPos_, matchedTau2->genTauDecayVertexPos_, 
		     matchedTau2->recVisP4_, matchedTau2->recLeadTrackTrajectory_, matchedTau2->hasRecTauDecayVertex_, matchedTau2->recTauDecayVertexPos_, matchedTau2->recTauDecayVertexCov_, 
		     true, false, false, false, 
		     true, true, 
		     vertexPos_refitted, vertexCov_refitted,
		     recMEt.p4(), recMEtCov,
		     true,
		     evt, std::string(moduleLabel_).append("_all_vs_gjAngle1_and_gjAngle2"));
 */
  makeLikelihoodPlot(matchedTau1->genVisP4_, matchedTau1->genInvisP4_, matchedTau1->genTauDecayMode_, matchedTau1->genTauProdVertexPos_, matchedTau1->genTauDecayVertexPos_, 
		     matchedTau1->recVisP4_, matchedTau1->recLeadTrackTrajectory_, matchedTau1->hasRecTauDecayVertex_, matchedTau1->recTauDecayVertexPos_, matchedTau1->recTauDecayVertexCov_, 
		     false, true, false, false,
		     true, true, 
		     matchedTau2->genVisP4_, matchedTau2->genInvisP4_, matchedTau2->genTauDecayMode_, matchedTau2->genTauProdVertexPos_, matchedTau2->genTauDecayVertexPos_, 
		     matchedTau2->recVisP4_, matchedTau2->recLeadTrackTrajectory_, matchedTau2->hasRecTauDecayVertex_, matchedTau2->recTauDecayVertexPos_, matchedTau2->recTauDecayVertexCov_, 
		     false, true, false, false, 
		     true, true, 
		     vertexPos_refitted, vertexCov_refitted,
		     recMEt.p4(), recMEtCov,
		     true,
		     evt, std::string(moduleLabel_).append("_all_vs_X1_and_X2"));
}

void SVfitLikelihoodDisplay::endJob()
{
// nothing to be done yet...
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(SVfitLikelihoodDisplay);





