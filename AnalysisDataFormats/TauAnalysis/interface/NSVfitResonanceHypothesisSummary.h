#ifndef AnalysisDataFormats_TauAnalysis_NSVfitResonanceHypothesisSummary_h
#define AnalysisDataFormats_TauAnalysis_NSVfitResonanceHypothesisSummary_h

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitResonanceHypothesisBase.h"

#include <string>

class NSVfitResonanceHypothesisSummary
{
 public:

  NSVfitResonanceHypothesisSummary() {}
  NSVfitResonanceHypothesisSummary(const NSVfitResonanceHypothesisSummary&);
  NSVfitResonanceHypothesisSummary(const NSVfitResonanceHypothesisBase&);
  virtual ~NSVfitResonanceHypothesisSummary() {}

  virtual NSVfitResonanceHypothesisSummary* clone() const { return new NSVfitResonanceHypothesisSummary(*this); }

  virtual NSVfitResonanceHypothesisSummary& operator=(const NSVfitResonanceHypothesisSummary&);

  void setName(const std::string& name) { name_ = name; }

  const std::string& name() const { return name_; }

  /// mean and median reconstructed mass,
  /// -1 sigma and +1 sigma limits on reconstructed mass
  double mass() const { return mass_; }
  double massErrUp() const { return massErrUp_; }
  double massErrDown() const { return massErrDown_; }

  bool isValidSolution() const { return isValidSolution_; }

  virtual void print(std::ostream& stream) const
  {
    stream << "<NSVfitResonanceHypothesisSummary::print>:" << std::endl;
    stream << " name = " << name_ << std::endl;
    stream << " mass = " << mass_ << " + " << massErrUp_ << " - " << massErrDown_ << std::endl;
    stream << " isValidSolution = " << isValidSolution_ << std::endl;
  }

 protected:

  /// "human-readable" label for print-out
  std::string name_;

  /// "best fit" reconstructed mass,
  /// -1 sigma and +1 sigma limits on reconstructed mass
  double mass_;
  double massErrUp_;
  double massErrDown_;

  /// flag indicating that computed mass hypotheses are physically "valid" solutions
  bool isValidSolution_;
};

#endif
