#ifndef AnalysisDataFormats_TauAnalysis_NSVfitResonanceHypothesisBase_h
#define AnalysisDataFormats_TauAnalysis_NSVfitResonanceHypothesisBase_h

#include "DataFormats/Common/interface/OwnVector.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitSingleParticleHypothesisBase.h"

#include <string>

class NSVfitResonanceHypothesisBase
{
 public:

  NSVfitResonanceHypothesisBase() {}
  NSVfitResonanceHypothesisBase(const NSVfitResonanceHypothesisBase&);
  virtual ~NSVfitResonanceHypothesisBase() {}

  virtual NSVfitResonanceHypothesisBase* clone() const { return new NSVfitResonanceHypothesisBase(*this); }

  virtual NSVfitResonanceHypothesisBase& operator=(const NSVfitResonanceHypothesisBase&);

  const std::string& name() const { return name_; }
  int barcode() const { return barcode_; }

  /// mean and median reconstructed mass,
  /// -1 sigma and +1 sigma limits on reconstructed mass
  double mass() const { return mass_; }
  double massErrUp() const { return massErrUp_; }
  double massErrDown() const { return massErrDown_; }

  bool isValidSolution() const { return isValidSolution_; }

  /// fit hypotheses of daughter particles
  size_t numDaughters() const { return daughters_.size(); }
  NSVfitSingleParticleHypothesisBase* daughter(size_t idx) { return &daughters_[idx]; }
  const NSVfitSingleParticleHypothesisBase* daughter(size_t idx) const { return &daughters_[idx]; }
  const NSVfitSingleParticleHypothesisBase* daughter(const std::string& name) const
  {
    const NSVfitSingleParticleHypothesisBase* retVal = 0;
    for ( edm::OwnVector<NSVfitSingleParticleHypothesisBase>::const_iterator daughter = daughters_.begin();
	  daughter != daughters_.end(); ++daughter ) {
      if ( daughter->name() == name ) retVal = &(*daughter);
    }
    return retVal;
  }

  virtual void print(std::ostream& stream) const
  {
    stream << "<NSVfitResonanceHypothesisBase::print>:" << std::endl;
    stream << " name = " << name_ << std::endl;
    stream << " barcode = " << barcode_ << std::endl;
    stream << " mass = " << mass_ << " + " << massErrUp_ << " - " << massErrDown_ << std::endl;
    stream << " isValidSolution = " << isValidSolution_ << std::endl;
    for ( edm::OwnVector<NSVfitSingleParticleHypothesisBase>::const_iterator daughter = daughters_.begin();
          daughter != daughters_.end(); ++daughter ) {
      daughter->print(stream);
    }
  }

  friend class NSVfitResonanceBuilderBase;
  friend class NSVfitResonanceBuilderW;
  friend class NSVfitAlgorithmBase;
  friend class NSVfitAlgorithmByLikelihoodMaximization;

 protected:

  /// "human-readable" label for print-out
  std::string name_;

  /// unique identifier to associate hypothesis to likelihood function
  int barcode_; 

  /// fit hypotheses for daughter particles
  edm::OwnVector<NSVfitSingleParticleHypothesisBase> daughters_;

  /// "best fit" reconstructed mass,
  /// -1 sigma and +1 sigma limits on reconstructed mass
  double mass_;
  double massErrUp_;
  double massErrDown_;

  /// flag indicating that computed mass hypotheses are physically "valid" solutions
  bool isValidSolution_;
};

// CV: implementation of operator< needed to store NSVfitResonanceHypothesisBase objects in edm::OwnVector
bool operator<(const NSVfitResonanceHypothesisBase&, const NSVfitResonanceHypothesisBase&);

#endif
