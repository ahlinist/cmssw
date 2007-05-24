#include "AnalysisDataFormats/ElectronEfficiency/interface/EmObject.h"

namespace EgEff
{
   EmObject *EmObject::clone() const
   {
      return new EmObject(*this);
   }

   reco::TrackRef EmObject::genericTrack()
   {
      reco::TrackRef trackRef;
      if(hasGsfTrack_)
      {
         edm::RefToBase<reco::Track> trackBaseRef = edm::RefToBase<reco::Track>(gsfTrack());
         trackRef = trackBaseRef.castTo<reco::TrackRef>();
      }
      else if(hasTrack_)
      {
         trackRef = track();
      }
      return trackRef;
   }

   reco::TrackRef EmObject::track() const
   {
      return track_;
   }

   reco::GsfTrackRef EmObject::gsfTrack() const
   {
      return gsfTrack_;
   }

   reco::SuperClusterRef EmObject::superCluster() const
   {
      return superCluster_;
   }

   bool EmObject::overlap(const Candidate &c) const
   {
      const RecoCandidate *o = dynamic_cast<const RecoCandidate*>(&c);
      return (o != 0 &&
         (checkOverlap(track(), o->track()) ||
         checkOverlap(superCluster(), o->superCluster())));
      return false;
   }
}
