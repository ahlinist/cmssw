#ifndef AnalysisDataFormats_TauAnalysis_PATLeptonRecoilEnergyFwd_h
#define AnalysisDataFormats_TauAnalysis_PATLeptonRecoilEnergyFwd_h

#include "AnalysisDataFormats/TauAnalysis/interface/PATLeptonRecoilEnergy.h"

#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/RefProd.h"
#include "DataFormats/Common/interface/RefVector.h"

#include <vector>

/// collection of PATLeptonRecoilEnergy objects
typedef std::vector<PATTauRecoilEnergyFromJets> PATTauRecoilEnergyFromJetsCollection;
typedef std::vector<PATTauRecoilEnergyFromCaloTowers> PATTauRecoilEnergyFromCaloTowersCollection;

/// persistent reference to a PATLeptonRecoilEnergy object
typedef edm::Ref<PATTauRecoilEnergyFromJetsCollection> PATTauRecoilEnergyFromJetsRef;
typedef edm::Ref<PATTauRecoilEnergyFromCaloTowersCollection> PATTauRecoilEnergyFromCaloTowersRef;

/// references to PATLeptonRecoilEnergy collection
typedef edm::RefProd<PATTauRecoilEnergyFromJetsCollection> PATTauRecoilEnergyFromJetsRefProd;
typedef edm::RefProd<PATTauRecoilEnergyFromCaloTowersCollection> PATTauRecoilEnergyFromCaloTowersRefProd;

/// vector of references to PATLeptonRecoilEnergy objects all in the same collection
typedef edm::RefVector<PATTauRecoilEnergyFromJetsCollection> PATTauRecoilEnergyFromJetsRefVector;
typedef edm::RefVector<PATTauRecoilEnergyFromCaloTowersCollection> PATTauRecoilEnergyFromCaloTowersRefVector;

#endif
