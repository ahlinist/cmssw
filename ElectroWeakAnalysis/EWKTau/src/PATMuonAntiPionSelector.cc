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
#include "ElectroWeakAnalysis/EWKTau/interface/PATMuonAntiPionSelector.h"
#include "DataFormats/PatCandidates/interface/Muon.h"

typedef ObjectSelector<PATMuonAntiPionSelector> PATMuonAntiPionSelection;
DEFINE_ANOTHER_FWK_MODULE(PATMuonAntiPionSelection);


