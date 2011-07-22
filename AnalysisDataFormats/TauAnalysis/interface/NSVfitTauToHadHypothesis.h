#ifndef AnalysisDataFormats_TauAnalysis_NSVfitTauToHadHypothesis_h
#define AnalysisDataFormats_TauAnalysis_NSVfitTauToHadHypothesis_h

/*
 * NSVfitTauToHadHypothesis
 *
 * Class to store information about tau lepton decay into hadrons.
 *
 * NOTE: this class is used to make persistent solutions of NSVfitAlgorithmByLikelihoodMaximization;
 *       solutions of NSVfitAlgorithmByIntegration are of type NSVfitTauDecayHypothesis
 *
 * Authors: Evan K. Friis, Christian Veelken, UC Davis
 *
 */

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Common/interface/Ptr.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitTauDecayHypothesis.h"

class NSVfitTauToHadHypothesis : public NSVfitTauDecayHypothesis
{
 public:

  NSVfitTauToHadHypothesis() {}
  NSVfitTauToHadHypothesis(const edm::Ptr<reco::Candidate>& particle, const std::string& name, int barcode)
    : NSVfitTauDecayHypothesis(particle, name, barcode)
  {
    //std::cout << "<NSVfitTauToHadHypothesis::NSVfitTauToHadHypothesis>:" << std::endl;
    //std::cout << " constructor(const edm::Ptr<reco::Candidate>&, std::string, int)" << std::endl;
    p4_ = particle->p4();
  }
  NSVfitTauToHadHypothesis(const NSVfitTauToHadHypothesis& bluePrint)
    : NSVfitTauDecayHypothesis(bluePrint),
      decay_angle_VMrho_(bluePrint.decay_angle_VMrho_),
      mass2_VMrho_(bluePrint.mass2_VMrho_),
      decay_angle_VMa1_(bluePrint.decay_angle_VMa1_),
      decay_angle_VMa1r_theta_(bluePrint.decay_angle_VMa1r_theta_),
      decay_angle_VMa1r_phi_(bluePrint.decay_angle_VMa1r_phi_),
      mass2_VMa1_(bluePrint.mass2_VMa1_),
      fittedVertex_(bluePrint.fittedVertex_) 
  {}
  ~NSVfitTauToHadHypothesis() {}

  NSVfitTauToHadHypothesis* clone() const { return new NSVfitTauToHadHypothesis(*this); }
  NSVfitSingleParticleHypothesisBase* reduceToBase() const { 
    return new NSVfitTauToHadHypothesis(this->particle_, this->name_, this->barcode_);
  }

  NSVfitTauToHadHypothesis& operator=(const NSVfitTauToHadHypothesis& bluePrint)
  {
    NSVfitTauDecayHypothesis::operator=(bluePrint);
    decay_angle_VMrho_ = bluePrint.decay_angle_VMrho_;
    mass2_VMrho_ = bluePrint.mass2_VMrho_;
    decay_angle_VMa1_ = bluePrint.decay_angle_VMa1_;
    decay_angle_VMa1r_theta_ = bluePrint.decay_angle_VMa1r_theta_;
    decay_angle_VMa1r_phi_ = bluePrint.decay_angle_VMa1r_phi_;
    mass2_VMa1_ = bluePrint.mass2_VMa1_;
    fittedVertex_ = bluePrint.fittedVertex_;
    return (*this);
  }

  /// decay angles and masses of intermediate vector-meson resonances
  double decay_angle_VMrho() const { return decay_angle_VMrho_; }
  double mass2_VMrho() const { return mass2_VMrho_; }
  double decay_angle_VMa1() const { return decay_angle_VMa1_; }
  double decay_angle_VMa1r_theta() const { return decay_angle_VMa1r_theta_; }
  double decay_angle_VMa1r_phi() const { return decay_angle_VMa1r_phi_; }
  double mass2_VMa1() const { return mass2_VMa1_; }

  bool hasFittedVertex() const { return fittedVertex_.isValid(); }
  virtual const reco::Vertex& fittedVertex() const 
  {
    return fittedVertex_;
  }

  friend class NSVfitTauToHadBuilder;

 private:

  /// decay angles and masses of intermediate vector-meson resonances
  double decay_angle_VMrho_;
  double mass2_VMrho_;
  double decay_angle_VMa1_;
  double decay_angle_VMa1r_theta_;
  double decay_angle_VMa1r_phi_;
  double mass2_VMa1_;

  reco::Vertex fittedVertex_;
};

#endif
