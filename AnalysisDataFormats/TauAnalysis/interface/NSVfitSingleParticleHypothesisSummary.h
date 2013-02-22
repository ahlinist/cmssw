#ifndef AnalysisDataFormats_TauAnalysis_NSVfitSingleParticleHypothesisSummary_h
#define AnalysisDataFormats_TauAnalysis_NSVfitSingleParticleHypothesisSummary_h

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitSingleParticleHypothesis.h"

#include <string>

class NSVfitSingleParticleHypothesisSummary
{
 public:

  NSVfitSingleParticleHypothesisSummary() {}
  NSVfitSingleParticleHypothesisSummary(const NSVfitSingleParticleHypothesisSummary&);
  NSVfitSingleParticleHypothesisSummary(const NSVfitSingleParticleHypothesis&);
  virtual ~NSVfitSingleParticleHypothesisSummary() {}

  virtual NSVfitSingleParticleHypothesisSummary* clone() const { return new NSVfitSingleParticleHypothesisSummary(*this); }

  virtual NSVfitSingleParticleHypothesisSummary& operator=(const NSVfitSingleParticleHypothesisSummary&);

  void setName(const std::string& name) { name_ = name; }

  const std::string& name() const { return name_; }
  
  /// mass of visible decay products
  /// (no uncertainty)
  double mass() const { return mass_; }

  /// mean and median plus -1 sigma and +1 sigma limits on 
  /// reconstructed Pt, eta, phi
  ///
  /// NOTE: Pt, eta, phi will be valid only in case
  ///       NSVfitAlgorithmByIntegration2 algorithm (Markov Chain integration) is used
  ///  
  double pt() const { return pt_; }
  double ptErrUp() const { return ptErrUp_; }
  double ptErrDown() const { return ptErrDown_; }
  bool pt_isValid() const { return pt_isValid_; }
  double eta() const { return eta_; }
  double etaErrUp() const { return etaErrUp_; }
  double etaErrDown() const { return etaErrDown_; }
  bool eta_isValid() const { return eta_isValid_; }
  double phi() const { return phi_; }
  double phiErrUp() const { return phiErrUp_; }
  double phiErrDown() const { return phiErrDown_; }
  bool phi_isValid() const { return phi_isValid_; }

  virtual void print(std::ostream& stream) const
  {
    stream << "<NSVfitSingleParticleHypothesisSummary::print>:" << std::endl;
    stream << " name = " << name_ << std::endl;
    if ( pt_isValid_   ) stream << " Pt   = " << pt_   << " + " << ptErrUp_   << " - " << ptErrDown_   << std::endl;
    if ( eta_isValid_  ) stream << " eta  = " << eta_  << " + " << etaErrUp_  << " - " << etaErrDown_  << std::endl;
    if ( phi_isValid_  ) stream << " phi  = " << phi_  << " + " << phiErrUp_  << " - " << phiErrDown_  << std::endl;
    stream << " mass = " << mass_ << std::endl;
  }

 protected:

  /// "human-readable" label for print-out
  std::string name_;

  /// mass of visible decay products
  /// (no uncertainty)
  double mass_;

  /// "best fit" reconstructed Pt, eta, phi 
  /// plus -1 sigma and +1 sigma limits  
  double pt_;
  double ptErrUp_;
  double ptErrDown_;
  bool pt_isValid_;
  double eta_;
  double etaErrUp_;
  double etaErrDown_;
  bool eta_isValid_;
  double phi_;
  double phiErrUp_;
  double phiErrDown_;
  bool phi_isValid_;
};

// CV: implementation of operator< needed to store NSVfitSingleParticleHypothesisSummary objects in edm::OwnVector
bool operator<(const NSVfitSingleParticleHypothesisSummary&, const NSVfitSingleParticleHypothesisSummary&);

#endif
