#include "AnalysisDataFormats/ElectronEfficiency/interface/EmObject.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"

namespace EgEff
{
   EmObject *EmObject::clone() const
   {
      return new EmObject(*this);
   }

   reco::TrackRef EmObject::genericTrack() const
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

  bool EmObject::isBarrel() const
  {
    bool retVal = false;
    if(hasSuperCluster())
      {
	retVal = (superCluster()->seed()->getHitsByDetId()[0].subdetId() == EcalBarrel);
      }
    else if(hasTrack())
      {
	reco::TrackRef track = genericTrack();
	retVal = (fabs(track->eta()) < 1.479);
      }
    else
      {
	throw new cms::Exception("No supercluster or track in EmObject");
      }

    return retVal;
  }
}
