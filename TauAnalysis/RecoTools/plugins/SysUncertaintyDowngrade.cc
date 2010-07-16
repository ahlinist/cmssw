#include "TauAnalysis/RecoTools/plugins/SysUncertaintyDowngrade.h"

#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"

#include "FWCore/Framework/interface/MakerMacros.h"

typedef SysUncertaintyDowngrade<reco::Track> TrackDowngrade;
 
DEFINE_FWK_MODULE(TrackDowngrade);
