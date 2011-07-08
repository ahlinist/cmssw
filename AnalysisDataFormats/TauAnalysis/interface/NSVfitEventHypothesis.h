#ifndef AnalysisDataFormats_TauAnalysis_NSVfitEventHypothesis_h
#define AnalysisDataFormats_TauAnalysis_NSVfitEventHypothesis_h

#include "DataFormats/Candidate/interface/Candidate.h" 

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitEventHypothesisBase.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitResonanceHypothesis.h"

#include <string>

class NSVfitEventHypothesis : public NSVfitEventHypothesisBase
{
 public:

  NSVfitEventHypothesis() {}
  NSVfitEventHypothesis(const edm::Ptr<reco::Candidate>& met) 
    : NSVfitEventHypothesisBase(met),
      p4MEt_(met->p4())
  {}
  NSVfitEventHypothesis(const NSVfitEventHypothesis&);
  virtual ~NSVfitEventHypothesis() {}

  virtual NSVfitEventHypothesis* clone() const { return new NSVfitEventHypothesis(*this); }

  virtual NSVfitEventHypothesis& operator=(const NSVfitEventHypothesis&);

  /// fit hypotheses of lepton resonances
  NSVfitResonanceHypothesis* resonance(size_t idx) { 
    return dynamic_cast<NSVfitResonanceHypothesis*>(&resonances_[idx]); 
  }
  const NSVfitResonanceHypothesis* resonance(size_t idx) const { 
    return dynamic_cast<const NSVfitResonanceHypothesis*>(&resonances_[idx]); 
  }
  
  /// momentum of multi-lepton system before fit, after fit
  /// and difference in momentum (after - before) fit
  reco::Candidate::LorentzVector p4() const { return p4_; }
  reco::Candidate::LorentzVector p4_fitted() const { return (p4_ + dp4_); }
  reco::Candidate::LorentzVector dp4fitted() const { return dp4_; }

  /// missing transverse momentum before fit
  /// and fitted value of missing transverse momentum
  reco::Candidate::LorentzVector p4MEt() const { return p4MEt_; }
  reco::Candidate::LorentzVector dp4MEt_fitted() const { return (p4MEt_ - dp4_); } 

  friend class NSVfitEventBuilderBase;

 private:

  /// momentum of particle before fit
  reco::Candidate::LorentzVector p4_;

  /// difference in momentum (after - before) fit
  reco::Candidate::LorentzVector dp4_;

  /// missing transverse momentum before fit
  reco::Candidate::LorentzVector p4MEt_;
};

#endif


