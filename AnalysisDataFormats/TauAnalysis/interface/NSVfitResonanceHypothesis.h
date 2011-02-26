#ifndef AnalysisDataFormats_TauAnalysis_NSVfitResonanceHypothesis_h
#define AnalysisDataFormats_TauAnalysis_NSVfitResonanceHypothesis_h

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitSingleParticleHypothesisBase.h"

#include <string>

class NSVfitResonanceHypothesis
{
 public:
  NSVfitResonanceHypothesis();
  virtual ~NSVfitResonanceHypothesis() 
  {
    for ( std::vector<NSVfitSingleParticleHypothesisBase*>::const_iterator it = daughters_.begin();
          it != daughters_.end(); ++it ) {
      delete (*it);
    }
  }

  /// momentum of particle before fit, after fit
  /// and difference in momentum (after - before) fit
  reco::Candidate::LorentzVector p4() const { return p4_; }
  reco::Candidate::LorentzVector p4_fitted() const { return (p4_ + dp4_); }
  reco::Candidate::LorentzVector dp4_fitted() const { return dp4_; }

  /// fit hypotheses of daughter particles
  const std::vector<NSVfitSingleParticleHypothesisBase*>& daughters() const { return daughters_; }

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
};

#endif
