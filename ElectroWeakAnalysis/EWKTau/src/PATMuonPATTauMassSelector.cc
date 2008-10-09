// ********************************************************************************************
// *** The selector chooses only the PAT muons that satisfy a invariant mass requirement    ***
// *** when combined with a PAT tau.                                                        ***
// ***                                                                                      ***
// ***                                                                  October 8, 2008     ***
// ***                                                                  Alfredo Gurrola     ***
// ********************************************************************************************

#include "PluginManager/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "PhysicsTools/UtilAlgos/interface/ObjectSelector.h"
#include "ElectroWeakAnalysis/EWKTau/interface/PATMuonPATTauMassSelector.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

typedef ObjectSelector<PATMuonPATTauMassSelector> PATMuonPATTauMassSelection;
DEFINE_ANOTHER_FWK_MODULE(PATMuonPATTauMassSelection);


