// ********************************************************************************************
// *** The selector chooses only the PAT muons that satisfy a invariant mass requirement    ***
// *** when combined with a PAT tau.                                                        ***
// ***                                                                                      ***
// ***                                                                  October 8, 2008     ***
// ***                                                                  Alfredo Gurrola     ***
// ********************************************************************************************

#include "TauAnalysis/RecoTools/plugins/PATMuonAntiPionSelector.h"

#include "PhysicsTools/UtilAlgos/interface/ObjectSelector.h"

#include "FWCore/Framework/interface/MakerMacros.h"

typedef ObjectSelector<PATMuonAntiPionSelector> PATMuonAntiPionSelection;
DEFINE_ANOTHER_FWK_MODULE(PATMuonAntiPionSelection);


