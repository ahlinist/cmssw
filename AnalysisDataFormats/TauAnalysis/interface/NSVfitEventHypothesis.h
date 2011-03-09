#ifndef AnalysisDataFormats_TauAnalysis_NSVfitEventHypothesis_h
#define AnalysisDataFormats_TauAnalysis_NSVfitEventHypothesis_h

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitResonanceHypothesis.h"

#include <TH1.h>

#include <string>

class NSVfitEventHypothesis
{
 public:
  NSVfitEventHypothesis() 
    : ownsResonances_(true)
  {}
  virtual ~NSVfitEventHypothesis() 
  {
    if ( ownsResonances_ ) {
      for ( std::vector<NSVfitResonanceHypothesis*>::const_iterator it = resonances_.begin();
	    it != resonances_.end(); ++it ) {
	delete (*it);
      }
    }
  }

  const std::string& name() const { return name_; }
  int barcode() const { return barcode_; }

  /// momentum of multi-lepton system before fit, after fit
  /// and difference in momentum (after - before) fit
  reco::Candidate::LorentzVector p4() const { return p4_; }
  reco::Candidate::LorentzVector p4_fitted() const { return (p4_ + dp4_); }
  reco::Candidate::LorentzVector dp4fitted() const { return dp4_; }

  /// missing transverse momentum before fit
  /// and fitted value of missing transverse momentum 
  reco::Candidate::LorentzVector p4MEt() const { return p4MEt_; }
  reco::Candidate::LorentzVector p4MEt_fitted() const { return (p4MEt_ - dp4_); }

  /// fit hypotheses of lepton resonances
  const std::vector<NSVfitResonanceHypothesis*>& resonances() const { return resonances_; }
  const NSVfitResonanceHypothesis* resonance(const std::string& name)
  {
    const NSVfitResonanceHypothesis* retVal = 0;
    for ( std::vector<NSVfitResonanceHypothesis*>::const_iterator resonance = resonances_.begin();
	  resonance != resonances_.end(); ++resonance ) {
      if ( (*resonance)->name() == name ) retVal = (*resonance);
    }
    return retVal;
  }

  const TH1* histMassResults() const { return histMassResults_; }

  virtual void print(std::ostream& stream) const
  {
    stream << "<NSVfitEventHypothesis::print>:" << std::endl;
    stream << " name = " << name_ << std::endl;
    stream << " barcode = " << barcode_ << std::endl;
    for ( std::vector<NSVfitResonanceHypothesis*>::const_iterator resonance = resonances_.begin();
          resonance != resonances_.end(); ++resonance ) {
      (*resonance)->print(stream);
    }
  }

  friend class NSVfitEventBuilderBase;
  template<typename T_type> friend class NSVfitProducerT;
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

  /// missing transverse momentum before fit
  reco::Candidate::LorentzVector p4MEt_;

  /// fit hypotheses for daughter particles
  std::vector<NSVfitResonanceHypothesis*> resonances_;
  bool ownsResonances_;
  
  TH1* histMassResults_;
};

#endif


