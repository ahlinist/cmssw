//
// $Id: PATLeptonRecoilEnergyEventSelector.h,v 1.1.2.1 2009/08/04 10:04:55 mbluj Exp $
//

#ifndef TauAnalysis_RecoTools_PATLeptonRecoilEnergyEventSelector_h
#define TauAnalysis_RecoTools_PATLeptonRecoilEnergyEventSelector_h

#include "CommonTools/Utils/interface/AnySelector.h"
#include "CommonTools/UtilAlgos/interface/ObjectCountEventSelector.h"
#include "PhysicsTools/UtilAlgos/interface/MinNumberSelector.h"
#include "PhysicsTools/UtilAlgos/interface/MaxNumberSelector.h"

#include "AnalysisDataFormats/TauAnalysis/interface/PATLeptonRecoilEnergy.h"

#include <vector>

namespace reco {

  typedef ObjectCountEventSelector<std::vector<PATTauRecoilEnergyFromJets>, AnySelector, MinNumberSelector> PATTauRecoilEnergyFromJetsMinEventSelector;
  typedef ObjectCountEventSelector<std::vector<PATTauRecoilEnergyFromCaloTowers>, AnySelector, MinNumberSelector> PATTauRecoilEnergyFromCaloTowersMinEventSelector;

  typedef ObjectCountEventSelector<std::vector<PATTauRecoilEnergyFromJets>, AnySelector, MaxNumberSelector> PATTauRecoilEnergyFromJetsMaxEventSelector;
  typedef ObjectCountEventSelector<std::vector<PATTauRecoilEnergyFromCaloTowers>, AnySelector, MaxNumberSelector> PATTauRecoilEnergyFromCaloTowersMaxEventSelector;

}

#endif
