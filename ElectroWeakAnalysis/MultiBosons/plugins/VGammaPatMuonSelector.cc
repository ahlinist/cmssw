#include "FWCore/Framework/interface/MakerMacros.h"
#include "CommonTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "ElectroWeakAnalysis/MultiBosons/interface/VGammaMuonSelector.h"

namespace vgamma {
  namespace plugins {
    typedef SingleObjectSelector<
              pat::MuonCollection,
              VGammaMuonSelector
            > VGammaPatMuonSelector;
  } // end namespace vgamma::plugins
} // end namespace vgamma

using namespace vgamma::plugins;
DEFINE_FWK_MODULE( VGammaPatMuonSelector );
