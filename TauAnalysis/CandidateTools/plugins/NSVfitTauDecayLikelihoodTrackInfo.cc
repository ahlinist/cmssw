#include "TauAnalysis/CandidateTools/plugins/NSVfitTauDecayLikelihoodTrackInfo.h"

#include "FWCore/Utilities/interface/Exception.h"

#include "TrackingTools/Records/interface/TransientTrackRecord.h"

#include "DataFormats/CLHEP/interface/AlgebraicObjects.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitAlgorithmBase.h"
#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitTauDecayHypothesis.h"

using namespace SVfit_namespace;

NSVfitTauDecayLikelihoodTrackInfo::NSVfitTauDecayLikelihoodTrackInfo(const edm::ParameterSet& cfg)
  : NSVfitSingleParticleLikelihood(cfg)
{
  //std::cout << "<NSVfitTauDecayLikelihoodTrackInfo::NSVfitTauDecayLikelihoodTrackInfo>:" << std::endl;

  useLifetimeConstraint_ = cfg.getParameter<bool>("useLifetimeConstraint");
  useBackwardsPenaltyTerm_ = ( cfg.exists("useBackwardsPenaltyTerm") ) ? 
    cfg.getParameter<bool>("useBackwardsPenaltyTerm") : false;

  ignore3Prongs_ = ( cfg.exists("ignore3Prongs") ) ? 
    cfg.getParameter<bool>("ignore3Prongs") : false;
  ignore1Prongs_ = ( cfg.exists("ignore1Prongs") ) ? 
    cfg.getParameter<bool>("ignore1Prongs") : false;

  // CV: fugde factors to account for the fact that distribution of pulls
  //       (genVertexPos - recVertexPos)^T * recVertexCov^-1 * (genVertexPos - recVertexPos)
  //     do not peak at 1.0, but close to 1.4
  sfProdVertexCov_  = cfg.getParameter<double>("sfProdVertexCov");
  sfDecayVertexCov_ = cfg.getParameter<double>("sfDecayVertexCov");
}

NSVfitTauDecayLikelihoodTrackInfo::~NSVfitTauDecayLikelihoodTrackInfo()
{
// nothing to be done yet...
}

void NSVfitTauDecayLikelihoodTrackInfo::beginJob(NSVfitAlgorithmBase* algorithm)
{
  algorithm_ = algorithm;

  algorithm->requestFitParameter(prodParticleLabel_, nSVfit_namespace::kTau_phi_lab,                 pluginName_);
  algorithm->requestFitParameter(prodParticleLabel_, nSVfit_namespace::kTau_decayDistance_lab_shift, pluginName_);
}

void NSVfitTauDecayLikelihoodTrackInfo::beginCandidate(const NSVfitSingleParticleHypothesis*)
{
  NSVfitParameter* fitParameter = algorithm_->getFitParameter(prodParticleLabel_, nSVfit_namespace::kTau_decayDistance_lab_shift);
  assert(fitParameter);
  decayDistance_lab_shift_lowerLimit_ = fitParameter->LowerLimit();
  decayDistance_lab_shift_upperLimit_ = fitParameter->UpperLimit();
}

namespace
{
  double negLogLikelihoodExponentialDecay(double tauFlightPath, double p)
  {
//--- compute negative log-likelihood for tau flight path
//   (distance between primary event (tau production) vertex and secondary (tau decay) vertex)
//    to be compatible with tau lepton lifetime,
//    taking into account time dilation factor of Lorentz boost by tau momentum
//
//    NOTE: the probability for a particle of lifetime c*tau and momentum p
//          to decay after a distance x0 is taken from the PDG:
//          K. Nakamura et al. (Particle Data Group), J. Phys. G 37, 075021 (2010);
//          formula 38.14, differentiated by d/dx0 in order to get the probability
//          for the tau lepton to decay **at** distance x0
//         (rather than the probability to decay at distance x0 or greater)
//
    double a = (p/tauLeptonMass)*cTauLifetime;
    if ( tauFlightPath < 0. ) tauFlightPath = 0.;
    return TMath::Log(a) + tauFlightPath/a;
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
}

double NSVfitTauDecayLikelihoodTrackInfo::operator()(const NSVfitSingleParticleHypothesis* hypothesis, int polHandedness) const
{
//--- compute negative log-likelihood for tracks of tau lepton decay "leg"
//    to be compatible with originating from hypothetic secondary (tau lepton decay) vertex
//
//    The likelihood is computed as the product of probabilities for the tracks
//    to be compatible with the hypothetic secondary vertex of the tau lepton decay
//   (distance of closest approach of track to secondary vertex divided by estimated uncertainties of track extrapolation)

  if ( this->verbosity_ ) std::cout << "<NSVfitTauDecayLikelihoodTrackInfo::operator()>:" << std::endl;

  const NSVfitTauDecayHypothesis* hypothesis_T = dynamic_cast<const NSVfitTauDecayHypothesis*>(hypothesis);
  assert(hypothesis_T != 0);
  if ( verbosity_ >= 2 ) {
    std::cout << "decayDistance0 = " << hypothesis_T->expectedDecayDistance() << std::endl;
    std::cout << " a = " << hypothesis_T->expectedDecayDistanceJacobiFactor() << std::endl;
  }

  double nll_dca = 0.;  
  if ( hypothesis_T->expectedDecayDistance() > 0. ) {
    if ( !ignore3Prongs_ && hypothesis_T->hasDecayVertexFit() ) {
      AlgebraicVector3 residual = hypothesis_T->reconstructedDecayVertexPos() - hypothesis_T->expectedDecayVertexPos();
      AlgebraicMatrix33 reconstructed_wrt_expectedDecayVertexCov = hypothesis_T->reconstructedDecayVertexCov();
      const AlgebraicMatrix33& expectedDecayVertexCov = hypothesis_T->expectedDecayVertexCov();
      for ( unsigned iRow = 0; iRow < 3; ++iRow ) {
	for ( unsigned iColumn = 0; iColumn < 3; ++iColumn ) {
	  reconstructed_wrt_expectedDecayVertexCov(iRow, iColumn) *= sfDecayVertexCov_;
	  reconstructed_wrt_expectedDecayVertexCov(iRow, iColumn) += (sfProdVertexCov_*expectedDecayVertexCov(iRow, iColumn));
	}
      }
      nll_dca = -logGaussian3d(residual, reconstructed_wrt_expectedDecayVertexCov);      
      if ( verbosity_ >= 2 ) {
	std::cout << "processing 3-Prong case:" << std::endl;
	printVector("reconstructedDecayVertexPos", hypothesis_T->reconstructedDecayVertexPos());
	printVector("expectedDecayVertexPos", hypothesis_T->expectedDecayVertexPos());
	std::cout << "residual: x = " << residual(0) << ", y = " << residual(1) << ", z = " << residual(2) 
		  << " ( |residual| = " << TMath::Sqrt(norm2(residual)) << ")" << std::endl;
	printMatrix("reconstructed_wrt_expectedDecayVertexCov", reconstructed_wrt_expectedDecayVertexCov);
	double sigma2 = ROOT::Math::Similarity(residual, reconstructed_wrt_expectedDecayVertexCov)/norm2(residual);
	std::cout << "sigma(approx.) = " << TMath::Sqrt(sigma2) << std::endl;
	std::cout << " -log(P_dca) = " << nll_dca << std::endl;
      }
    } else if ( !ignore1Prongs_ && hypothesis_T->leadTrackTrajectory() ) {
      if ( !hypothesis_T->leadTrackExtrapolationError() ) {
	AlgebraicVector3 u1, u2, u3;
	compLocalCoordinates(hypothesis_T->expectedFlightPath_unit(), u1, u2, u3); 
	AlgebraicVector3 residual = hypothesis_T->reconstructedDecayVertexPos() - hypothesis_T->expectedDecayVertexPos();
	AlgebraicVector3 residual_U = transformToLocalCoordinatesPos(residual, u1, u2, u3);	
	AlgebraicMatrix33 reconstructed_wrt_expectedDecayVertexCov = hypothesis_T->reconstructedDecayVertexCov();
	const AlgebraicMatrix33& expectedDecayVertexCov = hypothesis_T->expectedDecayVertexCov();
	for ( unsigned iRow = 0; iRow < 3; ++iRow ) {
	  for ( unsigned iColumn = 0; iColumn < 3; ++iColumn ) {
	    reconstructed_wrt_expectedDecayVertexCov(iRow, iColumn) *= sfDecayVertexCov_;
	    reconstructed_wrt_expectedDecayVertexCov(iRow, iColumn) += (sfProdVertexCov_*expectedDecayVertexCov(iRow, iColumn));
	  }
	}
	AlgebraicMatrix33 reconstructed_wrt_expectedDecayVertexCov_U = transformToLocalCoordinatesCov(reconstructed_wrt_expectedDecayVertexCov, u1, u2, u3);       
	AlgebraicVector2 residual2d;
	residual2d(0) = residual_U(0);
	residual2d(1) = residual_U(1);
	AlgebraicMatrix22 reconstructed_wrt_expectedDecayVertexCov2d;
	for ( int iRow = 0; iRow < 2; ++iRow ) {
	  for ( int iColumn = 0; iColumn < 2; ++iColumn ) {
	    reconstructed_wrt_expectedDecayVertexCov2d(iRow, iColumn) = reconstructed_wrt_expectedDecayVertexCov_U(iRow, iColumn);
	  }
	}
	nll_dca = -logGaussian2d(residual2d, reconstructed_wrt_expectedDecayVertexCov2d);
	if ( verbosity_ >= 2 ) {
	  std::cout << "processing 1-Prong case:" << std::endl;
	  printVector("reconstructedDecayVertexPos", hypothesis_T->reconstructedDecayVertexPos());
	  printVector("expectedDecayVertexPos", hypothesis_T->expectedDecayVertexPos());
	  printMatrix("reconstructed_wrt_expectedDecayVertexCov", reconstructed_wrt_expectedDecayVertexCov);
	  printVector("u1", u1);
	  printVector("u2", u2);
	  printVector("u3", u3);
	  std::cout << "residual: u1 = " << residual2d(0) << ", u2 = " << residual2d(1) 
		    << " ( |residual| = " << TMath::Sqrt(norm2(residual2d)) << ")" << std::endl;	  
	  printMatrix("reconstructed_wrt_expectedDecayVertexCov", reconstructed_wrt_expectedDecayVertexCov2d);
	  double sigma2 = ROOT::Math::Similarity(residual2d, reconstructed_wrt_expectedDecayVertexCov2d)/norm2(residual2d);
	  std::cout << "sigma(approx.) = " << TMath::Sqrt(sigma2) << std::endl;
	  std::cout << " -log(P_dca) = " << nll_dca << std::endl;
	}
      } else {
	const NSVfitEventHypothesis* event = hypothesis_T->mother()->eventHypothesis();
	nll_dca = -logGaussian(TMath::Sqrt(norm2(event->eventVertexPos() - hypothesis_T->expectedDecayVertexPos())), 1.e-6);
	if ( verbosity_ >= 2 ) {
	  std::cout << "processing 1-Prong case:" << std::endl;
	  std::cout << " lead. Track extrapolation failed --> setting -log(P_dca) = " << nll_dca << std::endl;
	}
      }
    }
  }

  double nll_lifetime = 0.;
  if ( useLifetimeConstraint_ ) { 
    nll_lifetime = negLogLikelihoodExponentialDecay(hypothesis_T->expectedDecayDistance(), hypothesis_T->p4_fitted().P());
  }    

  double nll_penalty = 0.;
  if ( useBackwardsPenaltyTerm_ ) {
    nll_penalty = backwardsPenaltyTerm(hypothesis_T->expectedFlightPath_unit(), hypothesis_T->expectedDecayDistance(), hypothesis_T->p4());
  }
   
  double nll = nll_dca + nll_lifetime + nll_penalty;
  if ( verbosity_ >= 2 ) {
    std::cout << "--> -log(P) = " << nll
	      << " ( -log(P_dca) = " << nll_dca << ", -log(P_lifetime) = " << nll_lifetime << ", -log(P_penalty) = " << nll_penalty << ")" << std::endl;
  }
  double nll_JacobiFactor = -TMath::Log(hypothesis_T->expectedDecayDistanceJacobiFactor());
  nll += nll_JacobiFactor;

  double prob = TMath::Exp(-nll);
  if ( this->verbosity_ ) std::cout << "--> prob = " << prob << std::endl;

  return prob;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(NSVfitSingleParticleLikelihoodPluginFactory, NSVfitTauDecayLikelihoodTrackInfo, "NSVfitTauDecayLikelihoodTrackInfo");
