#ifndef TkSeedingLayers_SeedingLayer_H
#define TkSeedingLayers_SeedingLayer_H

#include <string>
#include <vector>
#include <memory>

#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHit.h"

class DetLayer;
class TransientTrackingRecHitBuilder;

namespace edm { class Event; class EventSetup; }
namespace ctfseeding {class HitExtractor; }

namespace ctfseeding {

class SeedingLayer {
public:
  enum Side { Barrel = 0, NegEndcap =1,  PosEndcap = 2 }; 
public:
  typedef  std::vector<TransientTrackingRecHit::ConstRecHitPointer> Hits;
  
  ~SeedingLayer();

  /*
  SeedingLayer(SeedingLayer const & rh) noexcept;
  SeedingLayer(SeedingLayer && rh) noexcept;
  */

  SeedingLayer( const std::string & name,
                const DetLayer* layer,
                const TransientTrackingRecHitBuilder * hitBuilder,
                const HitExtractor * hitExtractor,  
                bool usePredefinedErrors = false, float hitErrorRZ = 0., float hitErrorRPhi=0.);

  // void hits(const edm::Event& ev, const edm::EventSetup& es, Hits &) const;
  Hits hits(const edm::Event& ev, const edm::EventSetup& es) const;

  bool operator==(const SeedingLayer &s) const { return name()==s.name(); }

  std::string const & name() const { return theName; }

  const DetLayer*  detLayer() const { return theLayer; }
  const TransientTrackingRecHitBuilder * hitBuilder() const { return theTTRHBuilder; }

  bool  hasPredefinedHitErrors() const { return theHasPredefinedHitErrors; }
  float predefinedHitErrorRZ() const { return thePredefinedHitErrorRZ; }
  float predefinedHitErrorRPhi() const { return thePredefinedHitErrorRPhi; }

private:
  std::string theName;
  const DetLayer* theLayer;
  const TransientTrackingRecHitBuilder *theTTRHBuilder;
  std::shared_ptr<const HitExtractor> theHitExtractor;
  bool theHasPredefinedHitErrors;
  float thePredefinedHitErrorRZ, thePredefinedHitErrorRPhi;

};

}
#endif
