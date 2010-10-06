#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "ElectroWeakAnalysis/MultiBosons/plugins/Cleaner.h"
#include "FWCore/Framework/interface/MakerMacros.h"

namespace vgamma {
    typedef vgamma::Cleaner<reco::Photon> PhotonCleaner;
}
using namespace vgamma;
DEFINE_FWK_MODULE(PhotonCleaner);
