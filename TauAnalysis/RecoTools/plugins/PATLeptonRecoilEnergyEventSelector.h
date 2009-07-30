//
// $Id: PATLeptonRecoilEnergyEventSelector.h,v 1.1 2009/07/29 13:03:38 veelken Exp $
//

#ifndef TauAnalysis_RecoTools_PATLeptonRecoilEnergyEventSelector_h
#define TauAnalysis_RecoTools_PATLeptonRecoilEnergyEventSelector_h

#include "PhysicsTools/UtilAlgos/interface/AnySelector.h"
#include "PhysicsTools/UtilAlgos/interface/ObjectCountEventSelector.h"
#include "PhysicsTools/UtilAlgos/interface/MinNumberSelector.h"
#include "PhysicsTools/PatUtils/interface/MaxNumberSelector.h"

#include "AnalysisDataFormats/TauAnalysis/interface/PATLeptonRecoilEnergy.h"

#include <vector>

namespace reco {

  typedef ObjectCountEventSelector<std::vector<PATTauRecoilEnergyFromJets>, AnySelector, MinNumberSelector> PATTauRecoilEnergyFromJetsMinEventSelector;
  typedef ObjectCountEventSelector<std::vector<PATTauRecoilEnergyFromCaloTowers>, AnySelector, MinNumberSelector> PATTauRecoilEnergyFromCaloTowersMinEventSelector;

  typedef ObjectCountEventSelector<std::vector<PATTauRecoilEnergyFromJets>, AnySelector, MaxNumberSelector> PATTauRecoilEnergyFromJetsMaxEventSelector;
  typedef ObjectCountEventSelector<std::vector<PATTauRecoilEnergyFromCaloTowers>, AnySelector, MaxNumberSelector> PATTauRecoilEnergyFromCaloTowersMaxEventSelector;

}

#endif
