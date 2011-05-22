#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/EgammaReco/interface/ElectronSeed.h"
#include "DataFormats/EgammaReco/interface/ElectronSeedFwd.h"
#include "ElectroWeakAnalysis/MultiBosons/plugins/ObjectMultiplicityProducer.h"
#include "FWCore/Framework/interface/MakerMacros.h"

namespace vgamma {
  namespace plugins {
    typedef ObjectMultiplicityProducer<reco::CandidateView> CandidateCounter;
    typedef ObjectMultiplicityProducer<reco::TrackCollection> TrackCounter;

    typedef ObjectMultiplicityProducer<
              reco::GsfTrackCollection
            > GsfTrackCounter;

    typedef ObjectMultiplicityProducer<
              reco::ElectronSeedCollection 
            > ElectronSeedCounter;

  } // end of namespace plugins
} // end of namespace vgamma

using namespace vgamma::plugins;

DEFINE_FWK_MODULE( CandidateCounter );
DEFINE_FWK_MODULE( GsfTrackCounter );
DEFINE_FWK_MODULE( TrackCounter );
DEFINE_FWK_MODULE( ElectronSeedCounter );
