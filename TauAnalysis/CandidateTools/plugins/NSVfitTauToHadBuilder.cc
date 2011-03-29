#include "TauAnalysis/CandidateTools/interface/NSVfitTauDecayBuilderBase.h"
#include "TauAnalysis/CandidateTools/interface/SVfitLegTrackExtractor.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitTauToHadHypothesis.h"
#include "TauAnalysis/CandidateTools/interface/nSVfitParameter.h"

/** \class NSVfitTauToHadBuilder
 *
 * Auxiliary class reconstructing tau --> had decays and
 * building NSVfitTauToHadHypothesis objects;
 * used by NSVfit algorithm
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.6 $
 *
 * $Id: NSVfitTauToHadBuilder.h,v 1.6 2011/03/27 14:22:35 friis Exp $
 *
 */

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "TauAnalysis/CandidateTools/interface/NSVfitTrackService.h"

#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h"
#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"

using namespace nSVfit_namespace;

class NSVfitTauToHadBuilder : public NSVfitTauDecayBuilderBase
{
 public:
  NSVfitTauToHadBuilder(const edm::ParameterSet& cfg)
    : NSVfitTauDecayBuilderBase(cfg)
  {}
  
  void beginJob(NSVfitAlgorithmBase* algorithm) 
  {
    NSVfitTauDecayBuilderBase::beginJob(algorithm);

    idxFitParameter_thetaVMrho_ = getFitParameterIdx(algorithm, prodParticleLabel_, nSVfit_namespace::kTauVM_theta_rho, true);
    idxFitParameter_mass2VMrho_ = getFitParameterIdx(algorithm, prodParticleLabel_, nSVfit_namespace::kTauVM_mass2_rho, true);
    idxFitParameter_thetaVMa1_  = getFitParameterIdx(algorithm, prodParticleLabel_, nSVfit_namespace::kTauVM_theta_a1,  true);
    idxFitParameter_thetaVMa1r_ = getFitParameterIdx(algorithm, prodParticleLabel_, nSVfit_namespace::kTauVM_theta_a1r, true);
    idxFitParameter_phiVMa1r_   = getFitParameterIdx(algorithm, prodParticleLabel_, nSVfit_namespace::kTauVM_phi_a1r,   true);
    idxFitParameter_mass2VMa1_  = getFitParameterIdx(algorithm, prodParticleLabel_, nSVfit_namespace::kTauVM_mass2_a1,  true);

    if ( verbosity_ ) print(std::cout);
  }

  NSVfitSingleParticleHypothesisBase* build(const NSVfitTauDecayBuilderBase::inputParticleMap& inputParticles) const 
  {
    inputParticleMap::const_iterator particlePtr = inputParticles.find(prodParticleLabel_);
    assert(particlePtr != inputParticles.end());

    NSVfitTauToHadHypothesis* hypothesis = new NSVfitTauToHadHypothesis(particlePtr->second, prodParticleLabel_, barcodeCounter_);
    ++barcodeCounter_;

    NSVfitTauDecayBuilderBase::initialize(hypothesis, particlePtr->second.get());

    // Three prong case: check if we can fit a reconstructed vertex.
    std::vector<reco::TrackBaseRef> tracks = extractTracks(particlePtr->second.get());
    if ( tracks.size() > 1 ) {
      KalmanVertexFitter kvf(true);
      // Get the associated transient tracks
      std::vector<reco::TransientTrack> transTracks = trackService_->transientTracks(tracks.begin(), tracks.end());
      TransientVertex vertex = kvf.vertex(transTracks);
      hypothesis->fittedVertex_ = vertex;
    }

    return hypothesis;
  }

  void applyFitParameter(NSVfitTauDecayHypothesis* hypothesis, double* param) const 
  {
    std::cout << "<NSVfitTauToHadBuilder::applyFitParameter>:" << std::endl;
    NSVfitTauDecayBuilderBase::applyFitParameter(hypothesis, param);
    std::cout << "hypothesis = " << hypothesis << std::endl;
    std::cout << "param = " << param << std::endl;
    NSVfitTauToHadHypothesis* hypothesis_T = dynamic_cast<NSVfitTauToHadHypothesis*>(hypothesis);
    std::cout << "hypothesis_T = " << hypothesis_T << std::endl;
    assert(hypothesis_T);
    std::cout << "break-point A.1 reached" << std::endl;
    applyOptionalFitParameter(param, idxFitParameter_thetaVMrho_, hypothesis_T->decay_angle_VMrho_);
    std::cout << "break-point A.2 reached" << std::endl;
    applyOptionalFitParameter(param, idxFitParameter_mass2VMrho_, hypothesis_T->mass2_VMrho_);
    std::cout << "break-point A.3 reached" << std::endl;
    applyOptionalFitParameter(param, idxFitParameter_thetaVMa1_,  hypothesis_T->decay_angle_VMa1_);
    std::cout << "break-point A.4 reached" << std::endl;
    applyOptionalFitParameter(param, idxFitParameter_thetaVMa1r_, hypothesis_T->decay_angle_VMa1r_theta_);
    std::cout << "break-point A.5 reached" << std::endl;
    applyOptionalFitParameter(param, idxFitParameter_phiVMa1r_,   hypothesis_T->decay_angle_VMa1r_phi_);
    std::cout << "break-point A.6 reached" << std::endl;
    applyOptionalFitParameter(param, idxFitParameter_mass2VMa1_,  hypothesis_T->mass2_VMa1_);
    std::cout << "break-point A.7 reached" << std::endl;
  }

  // The two neutrion system in a leptonic decay can have mass.
  bool nuSystemIsMassless() const { return true; }

  virtual int getDecayMode(const reco::Candidate* candidate) const
  {
    const pat::Tau* tauPtr = dynamic_cast<const pat::Tau*>(candidate);
    assert(tauPtr);
    return tauPtr->decayMode();
  }

  virtual std::vector<reco::TrackBaseRef> extractTracks(const reco::Candidate* candidate) const
  {
    const pat::Tau* tauPtr = dynamic_cast<const pat::Tau*>(candidate);
    assert(tauPtr);
    return trackExtractor_(*tauPtr);
  }

  private:
    edm::Service<NSVfitTrackService> trackService_;
    SVfitLegTrackExtractor<pat::Tau> trackExtractor_;

    int idxFitParameter_thetaVMrho_;
    int idxFitParameter_mass2VMrho_;
    int idxFitParameter_thetaVMa1_;
    int idxFitParameter_thetaVMa1r_;
    int idxFitParameter_phiVMa1r_;
    int idxFitParameter_mass2VMa1_;
};

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(NSVfitSingleParticleBuilderPluginFactory, NSVfitTauToHadBuilder, "NSVfitTauToHadBuilder");


