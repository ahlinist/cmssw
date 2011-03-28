#ifndef AnalysisDataFormats_TauAnalysis_NSVfitResonanceHypothesis_h
#define AnalysisDataFormats_TauAnalysis_NSVfitResonanceHypothesis_h

#include "DataFormats/Common/interface/OwnVector.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitSingleParticleHypothesisBase.h"

#include <string>

class NSVfitResonanceHypothesis
{
 public:
  NSVfitResonanceHypothesis() {}
  NSVfitResonanceHypothesis(const NSVfitResonanceHypothesis&);
  virtual ~NSVfitResonanceHypothesis() {}

  virtual NSVfitResonanceHypothesis* clone() const { return new NSVfitResonanceHypothesis(*this); }
  
  virtual NSVfitResonanceHypothesis& operator=(const NSVfitResonanceHypothesis&);

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
  double mass_maximum() const { return massMaximum_; }
  double mass_maxInterpol() const { return massMaxInterpol_; }

  bool isValidSolution() const { return isValidSolution_; }

  /// fit hypotheses of daughter particles
  const edm::OwnVector<NSVfitSingleParticleHypothesisBase>& daughters() const { return daughters_; }
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
    stream << "<NSVfitResonanceHypothesis::print>:" << std::endl;
    stream << " name = " << name_ << std::endl;
    stream << " barcode = " << barcode_ << std::endl;
    for ( edm::OwnVector<NSVfitSingleParticleHypothesisBase>::const_iterator daughter = daughters_.begin();
          daughter != daughters_.end(); ++daughter ) {
      daughter->print(stream);
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
  edm::OwnVector<NSVfitSingleParticleHypothesisBase> daughters_;

  /// mean and median reconstructed mass, 
  /// -1 sigma and +1 sigma limits on reconstructed mass
  double mass_;
  double massErrUp_;
  double massErrDown_;

  double massMean_;
  double massMedian_;
  double massMaximum_;
  double massMaxInterpol_;

  /// flag indicating that computed mass hypotheses are physically "valid" solutions
  bool isValidSolution_;
};

bool operator<(const NSVfitResonanceHypothesis&, const NSVfitResonanceHypothesis&);

#endif
