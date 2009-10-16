#ifndef TauAnalysis_RecoTools_FakeRateJetWeightExtractor_h
#define TauAnalysis_RecoTools_FakeRateJetWeightExtractor_h

/** \class FakeRateJetWeightExtractor
 *
 * Auxiliary class to encapsulate the different methods 
 * for accessing weights associated to daughters of diTau candidates
 * (efficiency/fake-rate with which the tau-jet passes the tau id. criteria)
 * 
 * \author Chriatian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: FakeRateJetWeightExtractor.h,v 1.1 2009/08/16 13:57:11 veelken Exp $
 *
 */

#include "DataFormats/PatCandidates/interface/Tau.h"

#include <string>

template <typename T>
class FakeRateJetWeightExtractor
{
 public:
  explicit FakeRateJetWeightExtractor(const std::string&) {}

  double operator()(const T&) const
  {
    return 1.;
  }
};

// add template specialization for pat::Taus,
// returning the efficiency/fake-rate with which the tau-jet passes the tau id. criteria
template <>
class FakeRateJetWeightExtractor<pat::Tau>
{
 public:
  explicit FakeRateJetWeightExtractor(const std::string& effName)
    : effName_(effName) {}

  double operator()(const pat::Tau& tau) const
  {
    return tau.efficiency(effName_).value();
  }

 private:
  std::string effName_;
};

#endif
