#ifndef AnalysisDataFormats_TauAnalysis_NSVfitTauDecayHypothesisBaseT_h
#define AnalysisDataFormats_TauAnalysis_NSVfitTauDecayHypothesisBaseT_h

/*
 * NSVfitTauDecayHypothesisBaseT
 *
 * Base class representing tau lepton decay to electrons, muons or into hadrons.
 *
 * NOTE: this base-class is used to make persistent solutions of NSVfitAlgorithmByIntegration;
 *       solutions of NSVfitAlgorithmByLikelihoodMaximization are of type NSVfitTauDecayHypothesis
 *
 * Authors: Evan K. Friis, Christian Veelken, UC Davis
 *
 */

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Common/interface/Ptr.h"

template <typename T>
class NSVfitTauDecayHypothesisBaseT : public T
{
 public:

  NSVfitTauDecayHypothesisBaseT() {}
  NSVfitTauDecayHypothesisBaseT(const edm::Ptr<reco::Candidate>& particle, const std::string& name, int barcode)
    : T(particle, name, barcode) 
  {
    //std::cout << "<NSVfitTauDecayHypothesisBaseT::NSVfitTauDecayHypothesisBaseT>:" << std::endl;
    //std::cout << " constructor(const edm::Ptr<reco::Candidate>&, std::string, int)" << std::endl;
  }
  NSVfitTauDecayHypothesisBaseT(const NSVfitTauDecayHypothesisBaseT& bluePrint)
    : T(bluePrint),
      decayMode_(bluePrint.decayMode_)
  {}

  ~NSVfitTauDecayHypothesisBaseT() {}

  NSVfitTauDecayHypothesisBaseT& operator=(const NSVfitTauDecayHypothesisBaseT& bluePrint)
  {
    T::operator=(bluePrint);
    decayMode_ = bluePrint.decayMode_;
    return (*this);
  }

  /// tau lepton hadronic decay mode
  /// (as defined in DataFormats/TauReco/interface/PFTauDecayMode.h)
  int decayMode() const { return decayMode_; }

 protected:

  /// tau lepton hadronic decay mode
  int decayMode_;
};

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitSingleParticleHypothesisBase.h"

typedef NSVfitTauDecayHypothesisBaseT<NSVfitSingleParticleHypothesisBase> NSVfitTauDecayHypothesisBase;

#endif /* end of include guard: AnalysisDataFormats_TauAnalysis_NSVfitTauDecayHypothesisBaseT_h */
