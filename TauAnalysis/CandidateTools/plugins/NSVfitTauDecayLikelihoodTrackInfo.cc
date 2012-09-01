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

namespace{

  double norm2(const AlgebraicVector3& p)
  {
    return square(p(0)) + square(p(1)) + square(p(2));
  }

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

  double nll = 0.;
  if ( hypothesis_T->expectedDecayDistance() > 0. ) {
    if ( !ignore3Prongs_ && hypothesis_T->hasDecayVertexFit() ) {
      AlgebraicVector3 residual = hypothesis_T->reconstructedDecayVertexPos() - hypothesis_T->expectedDecayVertexPos();
      double residual2 = norm2(residual);
      if ( residual2 > 0. ) {
	double sigma2 = ROOT::Math::Similarity(residual, hypothesis_T->reconstructedDecayVertexCov())/residual2;
	if ( sigma2 > 0. ) nll -= logGaussian(TMath::Sqrt(residual2), TMath::Sqrt(sigma2));
      }
    } else if ( !ignore1Prongs_ && hypothesis_T->leadTrackTrajectory() ) {
      if ( !hypothesis_T->leadTrackExtrapolationError() ) {
	AlgebraicVector3 residual = hypothesis_T->reconstructedDecayVertexPos() - hypothesis_T->expectedDecayVertexPos();
	double residual2 = norm2(residual);
	if ( residual2 > 0. ) {
	  AlgebraicMatrix33 reconstructed_wrt_expectedDecayVertexCov = hypothesis_T->reconstructedDecayVertexCov();
	  const AlgebraicMatrix33& expectedDecayVertexCov = hypothesis_T->expectedDecayVertexCov();
	  for ( unsigned iRow = 0; iRow < 3; ++iRow ) {
	    for ( unsigned iColumn = 0; iColumn < 3; ++iColumn ) {
	      reconstructed_wrt_expectedDecayVertexCov(iRow, iColumn) += expectedDecayVertexCov(iRow, iColumn);
	    }
	  }
	  double sigma2 = ROOT::Math::Similarity(residual, reconstructed_wrt_expectedDecayVertexCov)/residual2;
	  if ( sigma2 > 0. ) nll -= logGaussian(TMath::Sqrt(residual2), TMath::Sqrt(sigma2));
	}
      } else {
	nll += hypothesis_T->leadTrackExtrapolationNLL();
      }
      //if ( useLifetimeConstraint_ ) { 
      // CV: add likelihood correction term to account for the fact 
      //     that integration of decay distance is truncated to reduce computing times
      //  double d0 = TMath::Max(0., hypothesis_T->expectedDecayDistance() + decayDistance_lab_shift_lowerLimit_);
      //  double d1 = hypothesis_T->expectedDecayDistance() + decayDistance_lab_shift_upperLimit_;
      //  double p = hypothesis_T->p4_fitted().P();
      //  if ( p < 1. ) p = 1.;
      //  double a = (p/tauLeptonMass)*cTauLifetime;
      //  double tauFlightPath = hypothesis_T->expectedDecayDistance();
      //  if ( tauFlightPath < 0. ) tauFlightPath = 0.;
      //  double norm = 1./(TMath::Exp(-d0/a) - TMath::Exp(-d1/a));
      //  nll -= TMath::Log(norm);
      //}    
    }
  }

  if ( useLifetimeConstraint_ ) { 
    nll += negLogLikelihoodExponentialDecay(hypothesis_T->expectedDecayDistance(), hypothesis_T->p4_fitted().P());
  }    

  if ( useBackwardsPenaltyTerm_ ) {
    nll += backwardsPenaltyTerm(hypothesis_T->expectedFlightPath_unit(), hypothesis_T->expectedDecayDistance(), hypothesis_T->p4());
  }
    
  double prob = TMath::Exp(-nll);
  if ( this->verbosity_ ) std::cout << "--> prob = " << prob << std::endl;

  return prob;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(NSVfitSingleParticleLikelihoodPluginFactory, NSVfitTauDecayLikelihoodTrackInfo, "NSVfitTauDecayLikelihoodTrackInfo");
