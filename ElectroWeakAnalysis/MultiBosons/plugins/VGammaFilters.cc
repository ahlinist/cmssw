#include "ElectroWeakAnalysis/MultiBosons/interface/VerboseEDFilterObjectWrapper.h"
#include "ElectroWeakAnalysis/MultiBosons/interface/VGammaMuonSelector.h"
#include "ElectroWeakAnalysis/MultiBosons/interface/VGammaDiLeptonSelector.h"
#include "ElectroWeakAnalysis/MultiBosons/interface/VGammaPhotonSelector.h"
#include "ElectroWeakAnalysis/MultiBosons/interface/ZMuMuGammaSelector.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "FWCore/Framework/interface/MakerMacros.h"

namespace vgamma {
  namespace plugins {

    typedef ::vgamma::VerboseFilterObjectWrapper<
                        VGammaMuonSelector,
                        pat::MuonCollection
                      > VGammaMuonFilter;

    typedef ::vgamma::VerboseFilterObjectWrapper<
                        VGammaDiLeptonSelector,
                        reco::CompositeCandidateCollection
                      > VGammaDiLeptonFilter;

    typedef ::vgamma::VerboseFilterObjectWrapper<
                        VGammaPhotonSelector,
                        pat::PhotonCollection
                      > VGammaPhotonFilter;

    typedef ::vgamma::VerboseFilterObjectWrapper<
                        ZMuMuGammaSelector,
                        reco::CompositeCandidateCollection
                      > ZMuMuGammaFilter;

  } // end of namespace vgamma::plugins
} // end of namespace vgamma

using namespace vgamma::plugins;

DEFINE_FWK_MODULE( VGammaMuonFilter );
DEFINE_FWK_MODULE( VGammaDiLeptonFilter );
DEFINE_FWK_MODULE( VGammaPhotonFilter );
DEFINE_FWK_MODULE( ZMuMuGammaFilter );
