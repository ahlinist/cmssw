#include "TauAnalysis/CandidateTools/interface/NSVfitTauDecayBuilder.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitAlgorithmBase.h"
#include "TauAnalysis/CandidateTools/interface/NSVfitParameter.h"
#include "TauAnalysis/CandidateTools/interface/SVfitTrackTools.h"
#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitTauDecayHypothesis.h"

using namespace nSVfit_namespace;
using namespace SVfit::track;

using namespace SVfit_namespace;

void NSVfitTauDecayBuilder::beginJob(NSVfitAlgorithmBase* algorithm)
{
  algorithm_ = algorithm;

  // Map the fit parameters to indices.
  idxFitParameter_visEnFracX_ = getFitParameterIdx(algorithm, prodParticleLabel_, nSVfit_namespace::kTau_visEnFracX);
  idxFitParameter_phi_lab_    = getFitParameterIdx(algorithm, prodParticleLabel_, nSVfit_namespace::kTau_phi_lab);
  idxFitParameter_deltaR_     = getFitParameterIdx(algorithm, prodParticleLabel_, nSVfit_namespace::kTau_decayDistance_lab, true);
}

void NSVfitTauDecayBuilder::initialize(NSVfitTauDecayHypothesis* hypothesis, const reco::Candidate* visCandidate) const
{
  hypothesis->p3Vis_unit_ = visCandidate->p4().Vect().Unit();
  hypothesis->visMass_ = visCandidate->mass();

  // Add protection against zero mass.  If lower than the electron mass, set it
  // to the electron mass.
  if (hypothesis->visMass_ < 5.1e-4) {
    hypothesis->visMass_ = 5.1e-4;
  }

  // Set tau lepton decay mode
  hypothesis->decayMode_ = getDecayMode(visCandidate);

  // If this is a leptonic tau decay, we need to setup the limits on the
  // neutrino system invariant mass parameter.
  // In case reconstructed mass of visible decay products exceeds 1.5 GeV,
  // assume measurement error and "truncate" @ 1.5 GeV.
  if ( !nuSystemIsMassless() ) {
    NSVfitParameter* fitParameter = algorithm_->getFitParameter(idxFitParameter_nuInvMass_);
    assert(fitParameter);
    fitParameter->setUpperLimit(SVfit_namespace::tauLeptonMass - TMath::Min(hypothesis->visMass_, 1.5));
  }

  // Extract the associated tracks, and fit a vertex if possible.
  hypothesis->tracks_ = extractTracks(visCandidate);
}

void
NSVfitTauDecayBuilder::applyFitParameter(NSVfitSingleParticleHypothesis* hypothesis, const double* param) const
{
  // Cast to the concrete tau decay hypothesis
  NSVfitTauDecayHypothesis* hypothesis_T = dynamic_cast<NSVfitTauDecayHypothesis*>(hypothesis);
  assert(hypothesis_T);

  double visEnFracX = param[idxFitParameter_visEnFracX_];
  double phi_lab    = param[idxFitParameter_phi_lab_];
  double pVis_lab   = hypothesis_T->p4().P();
  double enVis_lab  = hypothesis_T->p4().energy();
  double visMass    = hypothesis_T->visMass();
  double nuInvMass  = ( nuSystemIsMassless() ) ? 0. : param[idxFitParameter_nuInvMass_];

  const reco::Candidate::Vector& p3Vis_unit = hypothesis_T->p3Vis_unit();

//--- compute momentum of visible decay products in tau lepton rest frame
  double pVis_rf = pVisRestFrame(visMass, nuInvMass, tauLeptonMass);

//--- decay angle in tau lepton rest frame as function of X
//    (= energy ratio of visible decay products/tau lepton energy)
  double gjAngle = gjAngleFromX(visEnFracX, visMass, pVis_rf, enVis_lab, tauLeptonMass);

//--- compute tau lepton decay angle in laboratory frame
  double angleVis_lab = gjAngleToLabFrame(pVis_rf, gjAngle, pVis_lab);

//--- compute tau lepton momentum in laboratory frame
  double pTau_lab = motherMomentumLabFrame(visMass, pVis_rf, gjAngle, pVis_lab, tauLeptonMass);

//--- compute tau lepton direction in laboratory frame
  reco::Candidate::Vector tauFlight;
  const std::vector<const reco::Track*>& tracks = hypothesis_T->tracks();
  //std::stringstream decayVertexLog;
  // If we are not using track likelihoods, the tau direction is just a unit vector.
  if ( idxFitParameter_deltaR_ == -1 || tracks.size() == 0 ) {
    tauFlight = motherDirection(p3Vis_unit, angleVis_lab, phi_lab);
    if ( this->verbosity_ )
      std::cout << " Build non-track based vertex @ " << tauFlight << std::endl;
  } else {
    if ( this->verbosity_ )
      std::cout << "Finding decay vertex with tracks" << std::endl;

    /*************************************************
     *Begin track parameterization of secondary vertex
     **************************************************/

    // Get the reconstructed primary vertex from the event.
    const NSVfitResonanceHypothesis* mother = hypothesis_T->mother();
    assert(mother);
    const NSVfitEventHypothesis* eventHyp = mother->eventHypothesis();
    assert(eventHyp);

    if ( !eventHyp->eventVertexSVrefittedIsValid() )
      throw cms::Exception("NSVfitTauDecayBuilderBase::NoEventPV") <<
        "Couldn't get the refitted primary vertex!" << std::endl;

    GlobalPoint eventVertex(
        eventHyp->eventVertexPosSVrefitted()(0),
        eventHyp->eventVertexPosSVrefitted()(1),
        eventHyp->eventVertexPosSVrefitted()(2)
        );

    if (this->verbosity_)
      std::cout << "Event vertex is @ " << eventVertex << std::endl;

    GlobalVector visDirection = convert<GlobalVector>(
        hypothesis_T->p4().Vect());

    const reco::Track* leadTrack = tracks[0];
    assert(leadTrack);

    // Get the track extrapolation
    const SVfit::track::TrackExtrapolation& linearizedTrack =
      trackService_->linearizedTrack(leadTrack);

    GlobalPoint trackOrigin(
        linearizedTrack.dcaPosition()(0),
        linearizedTrack.dcaPosition()(1),
        linearizedTrack.dcaPosition()(2));
    GlobalVector trackDirection(
        linearizedTrack.tangent()(0),
        linearizedTrack.tangent()(1),
        linearizedTrack.tangent()(2));

    // Check if the linearized track intersects with the cone
    int status = -999;
    GlobalPoint svReferencePoint;
    GlobalPoint pcaOnTrackClosestToSVReferencePoint;
    svReferencePoint = intersectionOfLineAndCone(
        trackOrigin, trackDirection,
        eventVertex, visDirection, angleVis_lab, status);
    // If there is an intersection the PCAs are the same
    pcaOnTrackClosestToSVReferencePoint = svReferencePoint;

    if (this->verbosity_)
      std::cout << "Trying to find intersection - success = "
        << status << " vertex @ " << svReferencePoint << std::endl;

    // If there is no intersection point, try and find the point of closest
    // approach of the cone to the track
    if ( !status ) {
      // Get the track to cone PCA
      pcaOnTrackClosestToSVReferencePoint = pcaOfLineToCone(
        trackOrigin, trackDirection,
        eventVertex, visDirection, angleVis_lab, status);
      if (this->verbosity_)
        std::cout << "Trying to find pca - success = "
          << status << " track pca @ " << pcaOnTrackClosestToSVReferencePoint
          << std::endl;
      // Use this to get the cone to track PcA
      if (status) {
        svReferencePoint = pcaOfConeToPoint(
            pcaOnTrackClosestToSVReferencePoint,
            eventVertex, visDirection, angleVis_lab, status);
        if (this->verbosity_)
          std::cout << "Trying to find pca - success = "
            << status << " cone pca @ " << svReferencePoint
            << std::endl;
      }
    }

    // Find the point of max displacment, corresponding to 5 sigmas in c-tau
    // along the track
    int propagationStatus = 0;
    GlobalPoint maxDisplacementOnTrack = propagateTrackToDistanceWrtConeVertex(
        trackOrigin, trackDirection,
        eventVertex, visDirection,
        10*hypothesis_T->p4().energy()*cTauLifetime,
        propagationStatus);
    // This can only fail if the track and visible momenta are perpendicular
    // to each other.
    assert(propagationStatus);

    double displacement2 = SVfit::track::vectorSubtract(
        pcaOnTrackClosestToSVReferencePoint, eventVertex).mag2();

    double displacementAtMax2 = SVfit::track::vectorSubtract(
        maxDisplacementOnTrack, eventVertex).mag2();

    // If we couldn't find an intersection or PCA, or the found point was
    // farther than the max, the reference point is point on the cone that is
    // closest to the max displacement point
    if (!status || displacementAtMax2 < displacement2) {
      svReferencePoint = pcaOfConeToPoint(
          maxDisplacementOnTrack,
            eventVertex, visDirection, angleVis_lab, status);
      if (this->verbosity_)
        std::cout << "Using PCA of cone to max displacement point. success="
          << status << " cone pca @ " << svReferencePoint
          << std::endl;
    }
    // We should always have found a successful status by now!
    assert(status);
    // Now apply the phi and deltaR corrections
    GlobalVector referenceFlight = SVfit::track::vectorSubtract(
        svReferencePoint, eventVertex);
    if (this->verbosity_)
      std::cout << "Flight path is: " << referenceFlight << std::endl;

    // We scale the radius correction such that deltaR = 1
    // corresonds to approximately 1 simga of track error in transverse
    // displacement.
    double radialCorrection =
      param[idxFitParameter_deltaR_]*linearizedTrack.approximateTrackError() /
      std::max(1e-6, TMath::Sin(angleVis_lab));

    GlobalVector correctedFlight = applyPhiAndRadiusCorrections(
        visDirection,
        referenceFlight,
        param[idxFitParameter_phi_lab_], radialCorrection);

    if (this->verbosity_) {
      std::cout << "Applying dPhi = " << param[idxFitParameter_phi_lab_]
        << " and unscaled/scaled DR = " << param[idxFitParameter_deltaR_]
        << " / " << radialCorrection << std::endl;
      std::cout << "Corrected flight path is: " << correctedFlight << std::endl;
    }

    // Convert all to the stupid correct types
    AlgebraicVector3 finalSecondaryVertex(
        eventVertex.x() + correctedFlight.x(),
        eventVertex.y() + correctedFlight.y(),
        eventVertex.z() + correctedFlight.z());
    if (this->verbosity_) {
      std::cout << "Final SV: " << finalSecondaryVertex << std::endl;
    }
    tauFlight = reco::Candidate::Vector(
        correctedFlight.x(),
        correctedFlight.y(),
        correctedFlight.z());
    hypothesis_T->decayVertexPos_ = finalSecondaryVertex;
    hypothesis_T->flightPath_ = tauFlight;
    hypothesis_T->decayDistance_ = tauFlight.r();

    // Check if our radial correction flipped the sign of the tau direction.
    // If so, flip it back, so the calculation of the tau direction
    // (and subsequent boosting) is correct.
    if (radialCorrection + correctedFlight.mag() < 0) {
      tauFlight *= -1;
    }
  }

//--- compute tau lepton four-vector in laboratory frame
  reco::Candidate::LorentzVector p4Tau = motherP4(tauFlight.Unit(), pTau_lab, tauLeptonMass);

  hypothesis_T->p4_fitted_      = p4Tau;
  hypothesis_T->dp4_            = (p4Tau - hypothesis_T->p4_);

  hypothesis_T->p4invis_rf_     = boostToCOM(p4Tau, hypothesis_T->dp4_);
  hypothesis_T->p4vis_rf_       = boostToCOM(p4Tau, hypothesis_T->p4());

  hypothesis_T->visEnFracX_     = visEnFracX;
  hypothesis_T->decay_angle_rf_ = gjAngle;

  if ( verbosity_ ) {
    std::cout << "<NSVfitTauDecayBuilderBase::applyFitParameter>:" << std::endl;
    std::cout << " visEnFracX = " << visEnFracX << std::endl;
    std::cout << " phi_lab = " << phi_lab << std::endl;
    std::cout << " enVis_lab = " << enVis_lab << std::endl;
    std::cout << " visMass = " << visMass << std::endl;
    std::cout << " nuInvMass = " << hypothesis_T->p4invis_rf_.mass() << std::endl;
    std::cout << " gjAngle = " << gjAngle << std::endl;
    std::cout << " angleVis_lab = " << angleVis_lab << std::endl;
    std::cout << " pTau_lab = " << pTau_lab << std::endl;
    std::cout << "p4Vis: E = " << hypothesis_T->p4_.energy() << ","
	      << " px = " << hypothesis_T->p4_.px() << ", py = " << hypothesis_T->p4_.py() << ","
	      << " pz = " << hypothesis_T->p4_.pz() << std::endl;
    std::cout << "p4Tau: E = " << p4Tau.energy() << ","
	      << " px = " << p4Tau.px() << ", py = " << p4Tau.py() << ","
	      << " pz = " << p4Tau.pz() << std::endl;
    std::cout << "p4Vis (rest frame): E = " << hypothesis_T->p4vis_rf_.energy() << ","
              << " mass = " << hypothesis_T->p4vis_rf_.mass()
	      << " px = " << hypothesis_T->p4vis_rf_.px() << ", py = " << hypothesis_T->p4vis_rf_.py() << ","
	      << " pz = " << hypothesis_T->p4vis_rf_.pz() << std::endl;
    std::cout << "p4Invis (rest frame): E = " << hypothesis_T->p4invis_rf_.energy() << ","
              << " mass = " << hypothesis_T->p4invis_rf_.mass()
	      << " px = " << hypothesis_T->p4invis_rf_.px() << ", py = " << hypothesis_T->p4invis_rf_.py() << ","
	      << " pz = " << hypothesis_T->p4invis_rf_.pz() << std::endl;
  }
}

void NSVfitTauDecayBuilder::print(std::ostream& stream) const
{
  stream << "<NSVfitTauDecayBuilderBase::print>:" << std::endl;
  stream << " pluginName = " << pluginName_ << std::endl;
  stream << " pluginType = " << pluginType_ << std::endl;
  stream << " prodParticleLabel = " << prodParticleLabel_ << std::endl;
  stream << " idxFitParameter_visEnFracX = " << idxFitParameter_visEnFracX_ << std::endl;
  stream << " idxFitParameter_phi_lab = " << idxFitParameter_phi_lab_ << std::endl;
  stream << " idxFitParameter_nuInvMass = " << idxFitParameter_nuInvMass_ << std::endl;
}

//
//-------------------------------------------------------------------------------
//

void applyOptionalFitParameter(const double* param, int idxFitParameter, double& value)
{
  if   ( idxFitParameter != -1 ) value = param[idxFitParameter];
  else                           value = 0.;
}

