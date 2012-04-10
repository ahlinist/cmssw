#include "TauAnalysis/CandidateTools/plugins/NSVfitTauDecayLikelihoodTrackInfo.h"

#include "FWCore/Utilities/interface/Exception.h"

#include "TrackingTools/Records/interface/TransientTrackRecord.h"

#include "DataFormats/CLHEP/interface/AlgebraicObjects.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitAlgorithmBase.h"
#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitTauDecayHypothesis.h"

using namespace SVfit_namespace;

unsigned defaultMinNumHits      =  5;
unsigned defaultMinNumPixelHits =  1;
double   defaultMaxChi2DoF      = 10.;
double   defaultMaxDeltaPoverP  =  1.e+3;
double   defaultMinPt           =  5.;

NSVfitTauDecayLikelihoodTrackInfo::NSVfitTauDecayLikelihoodTrackInfo(const edm::ParameterSet& cfg)
  : NSVfitSingleParticleLikelihood(cfg),
    trackBuilder_(0)
{
  //std::cout << "<NSVfitTauDecayLikelihoodTrackInfo::NSVfitTauDecayLikelihoodTrackInfo>:" << std::endl;

  minNumHits_      = ( cfg.exists("minNumHits")      ) ? cfg.getParameter<unsigned>("minNumHits")      : defaultMinNumHits;
  minNumPixelHits_ = ( cfg.exists("minNumPixelHits") ) ? cfg.getParameter<unsigned>("minNumPixelHits") : defaultMinNumPixelHits;
  maxChi2DoF_      = ( cfg.exists("maxChi2DoF")      ) ? cfg.getParameter<double>("maxChi2DoF")        : defaultMaxChi2DoF;
  maxDeltaPoverP_  = ( cfg.exists("maxDeltaPoverP")  ) ? cfg.getParameter<double>("maxDeltaPoverP")    : defaultMaxDeltaPoverP;
  minPt_           = ( cfg.exists("minPt")           ) ? cfg.getParameter<double>("minPt")             : defaultMinPt;

  useLinearApprox_ = ( cfg.exists("useLinearApprox") ) ? cfg.getParameter<bool>("useLinearApprox")     : true;
  //std::cout << " useLinearApprox = " << useLinearApprox_ << std::endl;

  useLifetimeConstraint_ = cfg.getParameter<bool>("useLifetimeConstraint");
}

NSVfitTauDecayLikelihoodTrackInfo::~NSVfitTauDecayLikelihoodTrackInfo()
{
// nothing to be done yet...
}

void NSVfitTauDecayLikelihoodTrackInfo::beginJob(NSVfitAlgorithmBase* algorithm)
{
  algorithm->requestFitParameter(prodParticleLabel_, nSVfit_namespace::kTau_phi_lab,           pluginName_);
  algorithm->requestFitParameter(prodParticleLabel_, nSVfit_namespace::kTau_decayDistance_lab, pluginName_);
}

void NSVfitTauDecayLikelihoodTrackInfo::beginEvent(const edm::Event& evt, const edm::EventSetup& es)
{
//--- get pointer to TransientTrackBuilder
  edm::ESHandle<TransientTrackBuilder> trackBuilderHandle;
  es.get<TransientTrackRecord>().get("TransientTrackBuilder", trackBuilderHandle);
  trackBuilder_ = trackBuilderHandle.product();
  if ( !trackBuilder_ ) {
    throw cms::Exception("NSVfitResonanceLikelihoodPtBalance::operator()")
      << " Failed to access TransientTrackBuilder !!\n";
  }
}

void NSVfitTauDecayLikelihoodTrackInfo::beginCandidate(const NSVfitSingleParticleHypothesis* hypothesis)
{
  //std::cout << "<NSVfitTauDecayLikelihoodTrackInfo::beginCandidate>:" << std::endl;
  //std::cout << " trackBuilder = " << trackBuilder_ << std::endl;

  const NSVfitTauDecayHypothesis* hypothesis_T = dynamic_cast<const NSVfitTauDecayHypothesis*>(hypothesis);
  assert(hypothesis_T != 0);

  selectedTracks_.clear();

  if ( this->verbosity_ )
    std::cout << "<NSVfitTauDecayLikelihoodTrackInfo>::beginCandidate"
      << std::endl;

  const std::vector<const reco::Track*>& tracks = hypothesis_T->tracks();
  for ( std::vector<const reco::Track*>::const_iterator track = tracks.begin();
	track != tracks.end(); ++track ) {
    if (this->verbosity_)
      std::cout << "Examining track with pt: " << (*track)->pt() << " ... ";
    const reco::HitPattern& trackHitPattern = (*track)->hitPattern();
    if ( trackHitPattern.numberOfValidTrackerHits() >= (int)minNumHits_ &&
	 trackHitPattern.numberOfValidPixelHits() >= (int)minNumPixelHits_ &&
	 (*track)->normalizedChi2() < maxChi2DoF_ &&
	 ((*track)->ptError()/(*track)->pt()) < maxDeltaPoverP_ &&
	 (*track)->pt() > minPt_ ) {
      reco::TransientTrack transientTrack = trackBuilder_->build(*track);
      selectedTracks_.push_back(transientTrack);
      if (this->verbosity_)
        std::cout << "passes quality cuts." << std::endl;
    } else {
      if (this->verbosity_)
        std::cout << "FAILS quality cuts." << std::endl;
    }
  }

  isNewCandidate_ = true;
}

double logExponentialDecay(double tauFlightPath, double p)
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
  return -TMath::Log(a) - tauFlightPath/a;
}

// Compute a penalty incase the flight correction moves the SV 'behind' the
// primary vertex.
double backwardsPenaltyTerm(const reco::Candidate::Vector& flight, const reco::Candidate::LorentzVector& visP4, bool verbosity)
{
  double penalty = 0.;

  if ( flight.Unit().Dot(visP4.Vect().Unit()) < 0 ) {
    double distance = flight.r();
    penalty += 1.e+4*distance*distance;
    if ( verbosity ) {
      std::cout << "<backwardsPenaltyTerm>:" << std::endl;
      std::cout << " backwards decay distance = " << distance
		<< " --> penalty = " << penalty << std::endl;
    }
  }

  return penalty;
}

double NSVfitTauDecayLikelihoodTrackInfo::operator()(const NSVfitSingleParticleHypothesis* hypothesis, int polHandedness) const
{
//--- compute negative log-likelihood for tracks of tau lepton decay "leg"
//    to be compatible with originating from hypothetic secondary (tau lepton decay) vertex
//
//    The likelihood is computed as the product of probabilities for the tracks
//    to be compatible with the hypothetic secondary vertex of the tau lepton decay
//   (distance of closest approach of track to secondary vertex divided by estimated uncertainties of track extrapolation)

  if ( this->verbosity_ )
    std::cout << "<NSVfitTauDecayLikelihoodTrackInfo::operator()>:" << std::endl;

  const NSVfitTauDecayHypothesis* hypothesis_T = dynamic_cast<const NSVfitTauDecayHypothesis*>(hypothesis);
  assert(hypothesis_T != 0);

  double nll = 0.;

  if ( useLinearApprox_ ) {

//--- "linearize" computation of point of closest approach of track to hypothetic seconday (tau decay) vertex,
//    in order to improve numerical convergence of Minuit fit;
//    compute track direction and covariance matrix for one reference point
//   ("original" primary event (tau production) vertex position
//    plus expected "mean" tau lepton flight path, given by (visibleEnergy/tauMass)*c*tauLifetime)
//    instead of doing a "full" helix track extrapolation for each hypothetic seconday (tau decay) vertex position;
//    given the "linearized" track direction vector, approximate point of closest approach by:
//
//     primaryVertex + trackVector*scalarProduct(trackVector, secondaryVertex)/(|trackVector|*|secondaryVertex|)
//
    if ( isNewCandidate_ ) {
      if ( this->verbosity_ )
	std::cout << "--> computing linear approximation of helix track extrapolation..." << std::endl;

      AlgebraicVector3 direction;
      direction(0) = hypothesis_T->p4().px()/hypothesis_T->p4().P();
      direction(1) = hypothesis_T->p4().py()/hypothesis_T->p4().P();
      direction(2) = hypothesis_T->p4().pz()/hypothesis_T->p4().P();

      AlgebraicVector3 refPoint = pvPosition_;

      selectedTrackInfo_.clear();
      for ( std::vector<reco::TransientTrack>::const_iterator selectedTrack = selectedTracks_.begin();
	    selectedTrack != selectedTracks_.end(); ++selectedTrack ) {
        selectedTrackInfo_.push_back(SVfit::track::TrackExtrapolation(*selectedTrack, refPoint));
      }

      isNewCandidate_ = false;
    }

    if ( this->verbosity_ )
      std::cout << "--> computing distance between extrapolated track and (hypothetic) tau decay vertex..." << std::endl;
    for ( std::vector<SVfit::track::TrackExtrapolation>::const_iterator selectedTrackInfo = selectedTrackInfo_.begin();
	  selectedTrackInfo != selectedTrackInfo_.end(); ++selectedTrackInfo ) {

      double trackLL = selectedTrackInfo->logLikelihood(hypothesis_T->decayVertexPos());
      if ( this->verbosity_ ) {
        AlgebraicVector3 displacement =
          selectedTrackInfo->displacementFromTrack(
              hypothesis_T->decayVertexPos());
	std::cout << " SV is: " << hypothesis_T->decayVertexPos() << std::endl;
	std::cout << " displacement is: "
          << displacement << " mag: " << ROOT::Math::Mag(displacement)
          << " approx track error: "
          << selectedTrackInfo->approximateTrackError() << std::endl;
        std::cout << *selectedTrackInfo << std::endl;
        std::cout << " tracking NLL: " << -trackLL << std::endl;
      }
      nll -= trackLL;
    }
  } else {
    if ( this->verbosity_ )
      std::cout << "--> computing distance to (hypothetic) tau decay vertex using full helix extrapolation of track..." << std::endl;
    for ( std::vector<reco::TransientTrack>::const_iterator selectedTrack = selectedTracks_.begin();
	  selectedTrack != selectedTracks_.end(); ++selectedTrack ) {
      SVfit::track::TrackExtrapolation selectedTrackInfo(*selectedTrack, hypothesis_T->decayVertexPos());

      AlgebraicVector3 displacement = hypothesis_T->decayVertexPos() - selectedTrackInfo.dcaPosition();

      nll -= selectedTrackInfo.logLikelihoodFromDisplacement(displacement);
    }
  }

  if ( useLifetimeConstraint_ ) {
    double lifeTimeConstraintLL = logExponentialDecay(
        hypothesis_T->decayDistance(), hypothesis_T->p4().P());
    if (this->verbosity_) {
      std::cout << " flight distance: " << hypothesis_T->decayDistance()
        << " p: " << hypothesis_T->p4().P()
        << " flight NLL " << -lifeTimeConstraintLL << std::endl;
    }
    nll -= lifeTimeConstraintLL;
  }

//--- add a penalty term in case the SV is 'behind' the PV
  nll += backwardsPenaltyTerm(hypothesis_T->flightPath(), hypothesis_T->p4(), this->verbosity_);

  double prob = TMath::Exp(-nll);

  //if ( this->verbosity_ ) std::cout << "--> prob = " << prob << std::endl;

  return prob;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(NSVfitSingleParticleLikelihoodPluginFactory, NSVfitTauDecayLikelihoodTrackInfo, "NSVfitTauDecayLikelihoodTrackInfo");
