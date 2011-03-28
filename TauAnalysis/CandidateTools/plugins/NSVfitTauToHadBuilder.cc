#include "TauAnalysis/CandidateTools/interface/NSVfitTauDecayBuilderBase.h"
#include "TauAnalysis/CandidateTools/interface/SVfitLegTrackExtractor.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitTauToHadHypothesis.h"
#include "TauAnalysis/CandidateTools/interface/nSVfitParameter.h"

class NSVfitTauToHadBuilder : public NSVfitTauDecayBuilderBase 
{
 public:
  NSVfitTauToHadBuilder(const edm::ParameterSet& cfg)
    : NSVfitTauDecayBuilderBase(cfg)
  {}
  
  NSVfitTauDecayHypothesis* buildSpecific(const edm::Ptr<reco::Candidate> particle, const std::string& label, int barcode) const 
  {
    NSVfitTauToHadHypothesis* hypothesis = new NSVfitTauToHadHypothesis(particle, label, barcode);
    return hypothesis;
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
  
  virtual void beginJobSpecific(NSVfitAlgorithmBase* algorithm) 
  {
    idxFitParameter_thetaVMrho_ = getFitParameterIdx(algorithm, prodParticleLabel_, nSVfit_namespace::kTauVM_theta_rho, true);
    idxFitParameter_mass2VMrho_ = getFitParameterIdx(algorithm, prodParticleLabel_, nSVfit_namespace::kTauVM_mass2_rho, true);
    idxFitParameter_thetaVMa1_  = getFitParameterIdx(algorithm, prodParticleLabel_, nSVfit_namespace::kTauVM_theta_a1,  true);
    idxFitParameter_thetaVMa1r_ = getFitParameterIdx(algorithm, prodParticleLabel_, nSVfit_namespace::kTauVM_theta_a1r, true);
    idxFitParameter_phiVMa1r_   = getFitParameterIdx(algorithm, prodParticleLabel_, nSVfit_namespace::kTauVM_phi_a1r,   true);
    idxFitParameter_mass2VMa1_  = getFitParameterIdx(algorithm, prodParticleLabel_, nSVfit_namespace::kTauVM_mass2_a1,  true);
    // Check if we can fit a reconstructed vertex.
  }
  
  virtual void applyFitParameterSpecific(NSVfitTauDecayHypothesis* hypothesis, double *param) const 
  {
    NSVfitTauToHadHypothesis* hypothesis_T = dynamic_cast<NSVfitTauToHadHypothesis*>(hypothesis);
    applyOptionalFitParameter(param, idxFitParameter_thetaVMrho_, hypothesis_T->decay_angle_VMrho_);
    applyOptionalFitParameter(param, idxFitParameter_mass2VMrho_, hypothesis_T->mass2_VMrho_);
    applyOptionalFitParameter(param, idxFitParameter_thetaVMa1_,  hypothesis_T->decay_angle_VMa1_);
    applyOptionalFitParameter(param, idxFitParameter_thetaVMa1r_, hypothesis_T->decay_angle_VMa1r_theta_);
    applyOptionalFitParameter(param, idxFitParameter_phiVMa1r_,   hypothesis_T->decay_angle_VMa1r_phi_);
    applyOptionalFitParameter(param, idxFitParameter_mass2VMa1_,  hypothesis_T->mass2_VMa1_);
  }
  
  private:
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


