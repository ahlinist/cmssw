#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "ElectroWeakAnalysis/MultiBosons/plugins/GenMatchUserDataProducer.h"

using namespace vgamma;

namespace vgamma {
    typedef GenMatchUserDataProducer<reco::Photon> PhotonGenMatchUserDataProducer;
} // namespace::vgamma

DEFINE_FWK_MODULE(PhotonGenMatchUserDataProducer);

