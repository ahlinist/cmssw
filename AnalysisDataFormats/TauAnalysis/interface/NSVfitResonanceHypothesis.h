#ifndef AnalysisDataFormats_TauAnalysis_NSVfitResonanceHypothesis_h
#define AnalysisDataFormats_TauAnalysis_NSVfitResonanceHypothesis_h

#include "DataFormats/Common/interface/OwnVector.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitResonanceHypothesisBase.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitSingleParticleHypothesis.h"

#include <string>

class NSVfitEventHypothesis;

class NSVfitResonanceHypothesis : public NSVfitResonanceHypothesisBase
{
 public:
  NSVfitResonanceHypothesis()
    : eventHyp_(NULL)
  {}
  NSVfitResonanceHypothesis(const NSVfitResonanceHypothesis&);
  virtual ~NSVfitResonanceHypothesis() {}

  virtual NSVfitResonanceHypothesis* clone() const { return new NSVfitResonanceHypothesis(*this); }

  virtual NSVfitResonanceHypothesis& operator=(const NSVfitResonanceHypothesis&);

  const NSVfitEventHypothesis* eventHypothesis() const { return eventHyp_; }
  void setEventHypothesis(const NSVfitEventHypothesis* input) 
  {
    eventHyp_ = input;
  }

  /// fit hypotheses of daughter particles
  NSVfitSingleParticleHypothesis* daughter(size_t idx) { return dynamic_cast<NSVfitSingleParticleHypothesis*>(&daughters_[idx]); }
  const NSVfitSingleParticleHypothesis* daughter(size_t idx) const { 
    return dynamic_cast<const NSVfitSingleParticleHypothesis*>(&daughters_[idx]); 
  }
  
  /// momentum of particle before fit, after fit
  /// and difference in momentum (after - before) fit
  reco::Candidate::LorentzVector p4() const { return p4_; }
  reco::Candidate::LorentzVector p4_fitted() const { return (p4_ + dp4_); }
  reco::Candidate::LorentzVector dp4_fitted() const { return dp4_; }

  /// mean and median reconstructed mass
  double mass_mean() const { return massMean_; }
  double mass_median() const { return massMedian_; }
  double mass_maximum() const { return massMaximum_; }
  double mass_maxInterpol() const { return massMaxInterpol_; }

  virtual void print(std::ostream& stream) const
  {
    stream << "<NSVfitResonanceHypothesis::print>:" << std::endl;
    stream << " name = " << name_ << std::endl;
    stream << " barcode = " << barcode_ << std::endl;
    stream << " p4: Pt = " << p4_.pt() << ","
	   << " eta = " << p4_.eta() << ", phi = " << p4_.phi() << std::endl;
    stream << " p4_fitted: Pt = " << p4_fitted().pt() << ","
	   << " eta = " << p4_fitted().eta() << ", phi = " << p4_fitted().phi() << std::endl;
    stream << "--> mass = " << mass_ << " + " << massErrUp_ << " - " << massErrDown_ << std::endl;
    stream << " isValidSolution = " << isValidSolution_ << std::endl;
    for ( edm::OwnVector<NSVfitSingleParticleHypothesisBase>::const_iterator daughter = daughters_.begin();
          daughter != daughters_.end(); ++daughter ) {
      daughter->print(stream);
    }
  }

  friend class NSVfitResonanceBuilderBase;
  friend class NSVfitAlgorithmByLikelihoodMaximization;
  template<typename T1, typename T2> friend class CompositePtrCandidateT1T2MEt;

 private:

  /// pointer to event hypothesis object
  const NSVfitEventHypothesis* eventHyp_;

  /// momentum of particle before fit
  reco::Candidate::LorentzVector p4_;

  /// difference in momentum (after - before) fit
  reco::Candidate::LorentzVector dp4_;

  /// mean and median reconstructed mass
  double massMean_;
  double massMedian_;
  double massMaximum_;
  double massMaxInterpol_;
};

#endif
