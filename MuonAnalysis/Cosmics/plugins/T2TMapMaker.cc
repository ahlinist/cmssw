#include <map>
#include <vector>
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/TrackToTrackMap.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class T2TMapMaker : public edm::EDProducer {
public:
  explicit T2TMapMaker(const edm::ParameterSet&);

private:
  virtual void produce(edm::Event&, const edm::EventSetup&);

  struct entry {
    entry(int i_, int j_, double dR_) : i(i_), j(j_), dR(dR_) {}
    int i;
    int j;
    double dR;

    bool operator<(const entry& other) const {
      return dR < other.dR;
    }
  };
  
  edm::InputTag src;
  edm::InputTag dst;
};

T2TMapMaker::T2TMapMaker(const edm::ParameterSet& cfg)
  : src(cfg.getParameter<edm::InputTag>("src")),
    dst(cfg.getParameter<edm::InputTag>("dst"))
{
  produces<reco::TrackToTrackMap>();
}

void T2TMapMaker::produce(edm::Event& event, const edm::EventSetup&) {
  static const bool debug = false;
  std::ostringstream out;

  edm::Handle<reco::TrackCollection> src_tracks;
  event.getByLabel(src, src_tracks);

  edm::Handle<reco::TrackCollection> dst_tracks;
  event.getByLabel(dst, dst_tracks);

  const int ni = src_tracks->size();
  const int nj = dst_tracks->size();

  if (debug) {
    out << "src " << src.encode() << " has " << ni << " tracks:\n";
    for (int i = 0; i < ni; ++i)
      out << "src tk #" << i << " pt: " << src_tracks->at(i).pt() << " eta: " << src_tracks->at(i).eta() << "\n";
    out << "dst " << dst.encode() << " has " << nj << " tracks:\n";
    for (int j = 0; j < nj; ++j)
      out << "dst tk #" << j << " pt: " << dst_tracks->at(j).pt() << " eta: " << dst_tracks->at(j).eta() << "\n";
  }

  std::vector<entry> entries;
  for (int i = 0; i < ni; ++i)
    for (int j = 0; j < nj; ++j)
      entries.push_back(entry(i, j, reco::deltaR(src_tracks->at(i), dst_tracks->at(j))));

  std::sort(entries.begin(), entries.end());
  
  bool* i_used = new bool[ni];
  bool* j_used = new bool[nj];
  memset(i_used, 0, sizeof(bool)*ni);
  memset(j_used, 0, sizeof(bool)*nj);

  std::auto_ptr<reco::TrackToTrackMap> t2tmap(new reco::TrackToTrackMap);

  for (std::vector<entry>::const_iterator it = entries.begin(); it != entries.end(); ++it) {
    if (!i_used[it->i] && !j_used[it->j]) {
      t2tmap->insert(reco::TrackRef(src_tracks, it->i), reco::TrackRef(dst_tracks, it->j));
      i_used[it->i] = j_used[it->j] = true;
      if (debug) out << "matched src #" << it->i << " -> dst #" << it->j << " with dR = " << it->dR << "\n";
    }
  }

  if (debug) edm::LogInfo("T2TMapMaker") << out.str();

  event.put(t2tmap);

  delete [] i_used;
  delete [] j_used;
}

DEFINE_FWK_MODULE(T2TMapMaker);
