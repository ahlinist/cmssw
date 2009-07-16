#ifndef __AnalysisDataFormats_TauAnalysis_PATLeptonRecoilEnergy_h__
#define __AnalysisDataFormats_TauAnalysis_PATLeptonRecoilEnergy_h__

/** \class PATLeptonRecoilEnergy
 *
 * Information about objects opposite in transverse plane to pat::Lepton
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: PATLeptonRecoilEnergy.h,v 1.1 2009/07/11 15:35:01 veelken Exp $
 *
 */

#include "DataFormats/Common/interface/Ptr.h"
#include "DataFormats/Common/interface/PtrVector.h"

template<typename T1, typename T2>
class PATLeptonRecoilEnergy
{
  typedef edm::Ptr<T1> T1Ptr;
  typedef edm::PtrVector<T2> T2PtrVector;

 public:

  /// default constructor
  PATLeptonRecoilEnergy() {}

  /// constructor 
  PATLeptonRecoilEnergy(const T1Ptr lepton, const T2PtrVector& recoilEnergyObjects)
    : lepton_(lepton), recoilEnergyObjects_(recoilEnergyObjects) {}

  /// destructor
  ~PATLeptonRecoilEnergy() {}
  
  /// access to pat::Lepton
  const T1Ptr lepton() const { return lepton_; }

  /// access to list of objects opposite in transverse plane to pat::Lepton
  const T2PtrVector& recoilEnergyObjects() const { return recoilEnergyObjects_; }

  /// get Et sum of objects opposite in transverse plane to pat::Lepton
  double etSum() const { return etSum_; }
 
 private:

  /// allow only PATLeptonRecoilEnergyAlgorithm to change values of data-members
  template<typename T1_type, typename T2_type> friend class PATLeptonRecoilEnergyAlgorithm;

  /// set Et sum of objects opposite in transverse plane to pat::Lepton
  void setEtSum(double etSum) { etSum_ = etSum; }

  /// pat::Lepton
  T1Ptr lepton_;

  /// list of objects opposite in transverse plane to pat::Lepton
  T2PtrVector recoilEnergyObjects_;

  /// Et sum of objects opposite in transverse plane to pat::Lepton
  double etSum_;  
};

#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/CaloTowers/interface/CaloTower.h"

typedef PATLeptonRecoilEnergy<pat::Tau, reco::Jet> PATTauRecoilEnergyFromJets;
typedef PATLeptonRecoilEnergy<pat::Tau, CaloTower> PATTauRecoilEnergyFromCaloTowers;

#endif
