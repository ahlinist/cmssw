#ifndef AnalysisDataFormats_TauAnalysis_NSVfitTauToDaughtersHypothesisBaseT1T2_h
#define AnalysisDataFormats_TauAnalysis_NSVfitTauToDaughtersHypothesisBaseT1T2_h

/*
 * NSVfitTauToDaughtersHypothesisBaseT1T2
 *
 *  Class to store information about tau lepton decay into electrons, muons or hadrons.
 *
 * NOTE: 
 *      (1) this class is used to make persistent 
 *           o tau decays into electrons and muons,
 *             reconstructed by NSVfitAlgorithmByLikelihoodMaximization or NSVfitAlgorithmByIntegration
 *           o tau decays into hadrons,
 *             reconstructed by NSVfitAlgorithmByIntegration
 *      (2) information about tau lepton decays into hadrons reconstructed by NSVfitAlgorithmByLikelihoodMaximization
 *          are stored in objects of type NSVfitTauToHadHypothesis
 *        
 * Authors: Evan K. Friis, Christian Veelken, UC Davis
 *
 */

template <typename T1, typename T2>
class NSVfitTauToDaughtersHypothesisBaseT1T2 : public T1
{
 public:
  NSVfitTauToDaughtersHypothesisBaseT1T2() {}
  NSVfitTauToDaughtersHypothesisBaseT1T2(const edm::Ptr<reco::Candidate>& particle, const std::string& name, int barcode)
    : T1(particle, name, barcode)
  {
    //std::cout << "<NSVfitTauToDaughtersHypothesisBaseT1T2::NSVfitTauToDaughtersHypothesisBaseT1T2>:" << std::endl;
    //std::cout << " constructor(const edm::Ptr<reco::Candidate>&, std::string, int)" << std::endl;
  }
  NSVfitTauToDaughtersHypothesisBaseT1T2(const NSVfitTauToDaughtersHypothesisBaseT1T2& bluePrint)
    : T1(bluePrint)
  {}
  ~NSVfitTauToDaughtersHypothesisBaseT1T2() {}

  NSVfitTauToDaughtersHypothesisBaseT1T2* clone() const { return new NSVfitTauToDaughtersHypothesisBaseT1T2(*this); }
  NSVfitSingleParticleHypothesisBase* reduceToBase() const { return this->clone(); }

  NSVfitTauToDaughtersHypothesisBaseT1T2& operator=(const NSVfitTauToDaughtersHypothesisBaseT1T2& bluePrint)
  {
    T1::operator=(bluePrint);
    return (*this);
  }

  template <typename T> friend class NSVfitTauToLepBuilder;
};

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitTauDecayHypothesisBaseT.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitSingleParticleHypothesisBase.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

typedef NSVfitTauToDaughtersHypothesisBaseT1T2<NSVfitTauDecayHypothesisBase, pat::Electron> NSVfitTauToElecHypothesisBase;
typedef NSVfitTauToDaughtersHypothesisBaseT1T2<NSVfitTauDecayHypothesisBase, pat::Muon> NSVfitTauToMuHypothesisBase;
typedef NSVfitTauToDaughtersHypothesisBaseT1T2<NSVfitTauDecayHypothesisBase, pat::Tau> NSVfitTauToHadHypothesisBase;

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitTauDecayHypothesis.h"

typedef NSVfitTauToDaughtersHypothesisBaseT1T2<NSVfitTauDecayHypothesis, pat::Electron> NSVfitTauToElecHypothesis;
typedef NSVfitTauToDaughtersHypothesisBaseT1T2<NSVfitTauDecayHypothesis, pat::Muon> NSVfitTauToMuHypothesis;

#endif
