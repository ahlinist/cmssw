#ifndef AnalysisDataFormats_TauAnalysis_NSVfitResonanceHypothesis_h
#define AnalysisDataFormats_TauAnalysis_NSVfitResonanceHypothesis_h

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitSingleParticleHypothesisBase.h"

#include <string>

class NSVfitResonanceHypothesis
{
 public:
  NSVfitResonanceHypothesis() 
    : ownsDaughters_(true)
  {}
  virtual ~NSVfitResonanceHypothesis() 
  {
    if ( ownsDaughters_ ) {
      for ( std::vector<NSVfitSingleParticleHypothesisBase*>::const_iterator it = daughters_.begin();
	    it != daughters_.end(); ++it ) {
	delete (*it);
      }
    }
  }
  
  const std::string& name() const { return name_; }
  int barcode() const { return barcode_; }

  /// momentum of particle before fit, after fit
  /// and difference in momentum (after - before) fit
  reco::Candidate::LorentzVector p4() const { return p4_; }
  reco::Candidate::LorentzVector p4_fitted() const { return (p4_ + dp4_); }
  reco::Candidate::LorentzVector dp4_fitted() const { return dp4_; }

  /// mean and median reconstructed mass, 
  /// -1 sigma and +1 sigma limits on reconstructed mass
  double mass() const { return mass_; }
  double massErrUp() const { return massErrUp_; }
  double massErrDown() const { return massErrDown_; }

  double mass_mean() const { return massMean_; }
  double mass_median() const { return massMedian_; }

  /// fit hypotheses of daughter particles
  const std::vector<NSVfitSingleParticleHypothesisBase*>& daughters() const { return daughters_; }
  const NSVfitSingleParticleHypothesisBase* daughter(const std::string& name)
  {
    const NSVfitSingleParticleHypothesisBase* retVal = 0;
    for ( std::vector<NSVfitSingleParticleHypothesisBase*>::const_iterator daughter = daughters_.begin();
	  daughter != daughters_.end(); ++daughter ) {
      if ( (*daughter)->name() == name ) retVal = (*daughter);
    }
    return retVal;
  }

  virtual void print(std::ostream& stream) const
  {
    stream << "<NSVfitResonanceHypothesis::print>:" << std::endl;
    stream << " name = " << name_ << std::endl;
    stream << " barcode = " << barcode_ << std::endl;
    for ( std::vector<NSVfitSingleParticleHypothesisBase*>::const_iterator daughter = daughters_.begin();
          daughter != daughters_.end(); ++daughter ) {
      (*daughter)->print(stream);
    }
  }

  friend class NSVfitResonanceBuilderBase;
  friend class NSVfitAlgorithmBase;
  friend class NSVfitAlgorithmByIntegration;

 private:

  /// "human-readable" label for print-out
  std::string name_;

  /// unique identifier to associate hypothesis to likelihood function
  int barcode_;

  /// momentum of particle before fit
  reco::Candidate::LorentzVector p4_;

  /// difference in momentum (after - before) fit
  reco::Candidate::LorentzVector dp4_;

  /// fit hypotheses for daughter particles
  std::vector<NSVfitSingleParticleHypothesisBase*> daughters_;
  bool ownsDaughters_;

  /// mean and median reconstructed mass, 
  /// -1 sigma and +1 sigma limits on reconstructed mass
  double mass_;
  double massErrUp_;
  double massErrDown_;

  double massMean_;
  double massMedian_;
};

#endif
