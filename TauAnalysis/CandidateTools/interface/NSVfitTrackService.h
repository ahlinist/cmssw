/*
 * NSVfitTrackService
 *
 * Central service for interacting with tracks in the NSVfit.
 * Provides memoized functions for building track extrapolations and Transient
 * track objects.
 *
 * NB that the service must be passed the edm::EventSetup via the method
 * setEventSetup(es) before any of the methods are called.
 *
 * The caches are flushed automatically at the end of each event.
 *
 * Author: Evan K. Friis (UC Davis)
 *
 */

#ifndef TauAnalysis_CandidateTools_NSVfitTrackService_h
#define TauAnalysis_CandidateTools_NSVfitTrackService_h

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Event.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "DataFormats/Candidate/interface/Candidate.h"

#include "TauAnalysis/CandidateTools/interface/SVfitTrackExtrapolation.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"

// Forward declarations needed to register as edm::Service
namespace edm {
  class ActivityRegistry;
  class ParameterSet;
  class ModuleDescription;
}

class NSVfitTrackService {
public:
  NSVfitTrackService (const edm::ParameterSet&, edm::ActivityRegistry&);
  virtual ~NSVfitTrackService() {};

  /// Linearize a track about a reference point
  const SVfit::track::TrackExtrapolation& linearizedTrack(const reco::Track*) const;

  /// *Must* be called before any other functions are called.  The [point]
  /// argument is the spatial point about which to linearize the tracks
  /// (i.e. the nominal PV).
  void setup(const edm::EventSetup&, const reco::Candidate::Point&);

  /// Build a transient track from a TrackRef
  reco::TransientTrack transientTrack(const reco::Track*) const;

  /// Helper function to convert a collection of tracks to a vector of
  /// TransientTracks (for use in vertex fits)
  template<typename Iter> std::vector<reco::TransientTrack>
  transientTracks(const Iter& begin, const Iter& end) const 
  {
    std::vector<reco::TransientTrack> output;
    for ( Iter i = begin; i < end; i++ ) {
      output.push_back(transientTrack(*i));
    }
    return output;
  }

private:
  /// Clear all caches and reset pointer to TransientTrackBuilder. Called
  /// automatically by the servic eregisterat end of event.
  void reset(const edm::Event& evt, const edm::EventSetup&);
  edm::ESHandle<TransientTrackBuilder> builder_;
  typedef std::map<const reco::Track*, reco::TransientTrack> TransTrackCache;
  mutable TransTrackCache cacheTransientTrack_;
  typedef std::map<const reco::Track*, SVfit::track::TrackExtrapolation> TrackExtrapolationCache;
  mutable TrackExtrapolationCache cacheTrackExtrapolations_;
  bool isValid_;
  AlgebraicVector3 refPoint_;
};

#endif /* end of include guard: TauAnalysis_CandidateTools_NSVfitTrackService_h */
