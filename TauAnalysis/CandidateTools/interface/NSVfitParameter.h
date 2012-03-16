#ifndef TauAnalysis_CandidateTools_SVfitVariable_h
#define TauAnalysis_CandidateTools_SVfitVariable_h

#include <TMath.h>

#include <vector>
#include <string>
#include <cmath>

namespace nSVfit_namespace
{
  enum fitParameter {
    // fit parameters related to shifts of primary event vertex
    kPV_shiftX, kPV_shiftY, kPV_shiftZ,
    // fit parameters specific to tau decays
    kTau_visEnFracX, kTau_phi_lab, kTau_decayDistance_lab, kTau_nuInvMass, kTau_pol,
    kTauVM_theta_rho, kTauVM_mass2_rho,
    kTauVM_theta_a1, kTauVM_theta_a1r, kTauVM_phi_a1r, kTauVM_mass2_a1,
    // fit parameters specific to electrons, muons not originating from tau decays
    kLep_shiftEn,
    // fit parameters specific to neutrinos (not originating from tau decays)
    kNu_energy_lab, kNu_phi_lab,
    // fit parameters specific to W -> e nu and W -> mu nu decays
    kW_theta_lab, kW_phi_lab, kW_mass
  };
}

namespace ROOT { namespace Math {
  class Minimizer;
}}

std::string get_name_incl_type(const std::string&, int);

class NSVfitParameter
{
 public:
  NSVfitParameter(int, const std::string&, int, double, double, double, double, bool = false);
  NSVfitParameter(int, const std::string&, int, bool = false);

  void regUsedBy(const std::string& requester) { usedBy_.push_back(requester); }

  void setInitialValue(double initialValue) { initialValue_ = initialValue; }
  void setValue(double value) { value_ = value; }
  void setErrUp(double errUp) { errUp_ = errUp; }
  void setErrDown(double errDown) { errDown_ = errDown; }
  void setStepSize(double stepSize) { stepSize_ = stepSize; }
  void setLowerLimit(double lowerLimit) { lowerLimit_ = lowerLimit; }
  void setUpperLimit(double upperLimit) { upperLimit_ = upperLimit; }

  void reset() { value_ = initialValue_; }

  const std::string& Name() const { return name_; }
  std::string UniqueName() const { return get_name_incl_type(Name(), Type()); }

  int Type() const { return type_; }

  double Value() const { return value_; }
  double ErrUp() const { return errUp_; }
  double ErrDown() const { return errDown_; }

  double StepSize() const { return stepSize_; }
  double LowerLimit() const { return lowerLimit_; }
  double UpperLimit() const { return upperLimit_; }

  bool IsFixed() const { return isFixed_; }
  bool IsDoubleBound() const { return (!isnan(lowerLimit_) && !isnan(upperLimit_)); } // CMSSW_4_1_x version
  bool HasLowerLimit() const { return (!isnan(lowerLimit_)); }
  bool HasUpperLimit() const { return (!isnan(upperLimit_)); }
  //bool IsDoubleBound() const { return (!TMath::IsNaN(lowerLimit_) && !TMath::IsNaN(upperLimit_)); } // CMSSW_4_2_x version
  //bool HasLowerLimit() const { return (!TMath::IsNaN(lowerLimit_)); }
  //bool HasUpperLimit() const { return (!TMath::IsNaN(upperLimit_)); }

  int index() const { return idx_; }

  // Add predicate to sort by index, to ensure the variables get added
  // correctly.
  bool operator<(const NSVfitParameter& other) const
  {
    return index() < other.index();
  }

  // Print to a stream
  void dump(std::ostream& stream) const;

 protected:
  static void initializeDefaultValues();
  static std::vector<double> defaultInitialValues_;
  typedef std::pair<double, double> pdouble;
  static std::vector<pdouble> defaultLimits_;
  static std::vector<double> defaultStepSizes_;
  static bool defaultValues_initialized_;

  unsigned int idx_;

  std::string name_;
  int type_;

  double value_;
  double errUp_;
  double errDown_;

  double initialValue_;
  double lowerLimit_;
  double upperLimit_;
  double stepSize_;
  bool isFixed_;

  std::vector<std::string> usedBy_; // list of SingleParticle likelihoods depending on this fitParameter
};

std::ostream& operator<<(std::ostream&, const NSVfitParameter&);


#endif
