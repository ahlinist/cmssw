//
// $Id: PATLeptonRecoilEnergySelector.h,v 1.1 2009/06/10 09:33:09 veelken Exp $
//

#ifndef TauAnalysis_RecoTools_PATLeptonRecoilEnergySelector_h
#define TauAnalysis_RecoTools_PATLeptonRecoilEnergySelector_h

#include "DataFormats/Common/interface/RefVector.h"

#include "PhysicsTools/UtilAlgos/interface/StringCutObjectSelector.h"
#include "PhysicsTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "PhysicsTools/UtilAlgos/interface/ObjectSelector.h"
#include "PhysicsTools/UtilAlgos/interface/SingleElementCollectionSelector.h"

#include "AnalysisDataFormats/TauAnalysis/interface/PATLeptonRecoilEnergy.h"

#include <vector>

typedef SingleObjectSelector<
            std::vector<PATTauRecoilEnergyFromJets>,
            StringCutObjectSelector<PATTauRecoilEnergyFromJets>
        > PATTauRecoilEnergyFromJetsSelector;
typedef SingleObjectSelector<
            std::vector<PATTauRecoilEnergyFromCaloTowers>,
            StringCutObjectSelector<PATTauRecoilEnergyFromCaloTowers>
        > PATTauRecoilEnergyFromCaloTowersSelector;

typedef SingleObjectSelector<
            std::vector<PATTauRecoilEnergyFromJets>,
            StringCutObjectSelector<PATTauRecoilEnergyFromJets>,
            edm::RefVector<std::vector<PATTauRecoilEnergyFromJets> >
        > PATTauRecoilEnergyFromJetsRefSelector;
typedef SingleObjectSelector<
            std::vector<PATTauRecoilEnergyFromCaloTowers>,
            StringCutObjectSelector<PATTauRecoilEnergyFromCaloTowers>,
            edm::RefVector<std::vector<PATTauRecoilEnergyFromCaloTowers> >
        > PATTauRecoilEnergyFromCaloTowersRefSelector;

#endif
