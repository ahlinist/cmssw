#include "HLTrigger/JetMET/interface/AlphaT.h"

double AlphaT::value_(std::vector<bool> * jet_sign) const {

  // Clear pseudo-jet container
  if (jet_sign) {
    jet_sign->clear();
    jet_sign->resize(et_.size());
  }

  // check the size of the input collection
  if (et_.size() == 0)
    // empty jet collection, return AlphaT = 0
    return 0.;

  if (et_.size() > (unsigned int) std::numeric_limits<unsigned int>::digits)
    // too many jets, return AlphaT = a very large number
    return std::numeric_limits<double>::max();

  // Minimum Delta Et for two pseudo-jets - start the computation from an approximate value
  double min_delta_sum_et = sum_et_;

  for (unsigned int i = 0; i < (1U << (et_.size() - 1)); i++) { //@@ iterate through different combinations
    double delta_sum_et = 0.;
    for (unsigned int j = 0; j < et_.size(); ++j) { //@@ iterate through jets
      if (i & (1U << j))
        delta_sum_et -= et_[j];
      else
        delta_sum_et += et_[j];
    }
    delta_sum_et = std::abs(delta_sum_et);
    if (delta_sum_et < min_delta_sum_et) {
      min_delta_sum_et = delta_sum_et;
      if (jet_sign) {
        for (unsigned int j = 0; j < et_.size(); ++j)
          (*jet_sign)[j] = ((i & (1U << j)) == 0);
      }
    }
  }

  // Alpha_T
  return (sum_et_ - min_delta_sum_et) / denominator_;
}


// return an approximate value of AlphaT
// by construction, this shuld be always lower than, or equal to, the actual AlphaT value
double AlphaT::approximate_value_(std::vector<bool> * jet_sign) const {

  // Clear pseudo-jet container
  if (jet_sign) {
    jet_sign->clear();
    jet_sign->resize(et_.size());
  }

  // check the size of the input collection
  if (et_.size() == 0)
    // empty jet collection, return AlphaT = 0
    return 0.;

  // Approximate Delta Et for two pseudo-jets
  double delta_sum_et = et_[0];
  if (jet_sign) (*jet_sign)[0] = true;
  for (unsigned int j = 1; j < et_.size(); ++j) {
    if (delta_sum_et > 0.) {
      delta_sum_et -= et_[j];
      if (jet_sign) (*jet_sign)[j] = false;
    } else {
      delta_sum_et += et_[j];
      if (jet_sign) (*jet_sign)[j] = true;
    }
  }
  delta_sum_et = std::abs(delta_sum_et);

  // Approximate Alpha_T
  return (sum_et_ - delta_sum_et) / denominator_;
}
