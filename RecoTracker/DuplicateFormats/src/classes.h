#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackCandidate/interface/TrackCandidate.h"
#include "DataFormats/Common/interface/Wrapper.h"
#include <vector>

namespace {
  struct dictionary {
    std::pair<reco::Track,reco::Track> trackPair;
    std::pair<TrackCandidate, std::pair<reco::Track,reco::Track> > ctpair;
    std::vector<std::pair<TrackCandidate, std::pair<reco::Track,reco::Track> > > candidateMap;
    edm::Wrapper<std::pair<reco::Track,reco::Track> > w1;
    edm::Wrapper<std::vector<std::pair<TrackCandidate, std::pair<reco::Track,reco::Track> > > > w2;
    edm::Wrapper<std::pair<TrackCandidate, std::pair<reco::Track,reco::Track> > > w3;
  };
}
