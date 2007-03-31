#include "RecoTracker/PixelStubs/interface/SeedComparitorWithPixelStubs.h"
#include "RecoTracker/TkSeedingLayers/interface/SeedingHit.h"
#include "RecoTracker/PixelStubs/interface/PixelStub.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHit.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

SeedComparitorWithPixelStubs::SeedComparitorWithPixelStubs(const edm::ParameterSet & cfg)
  : betaCutFactor_(cfg.getParameter<double>("betaCutFactor"))
{}

SeedComparitorWithPixelStubs::SeedComparitorWithPixelStubs(double betaCutFactor)
  : betaCutFactor_(betaCutFactor)
{}

SeedComparitorWithPixelStubs::~SeedComparitorWithPixelStubs()
{}

// Check to see if the vector of hits are compatible based on
// their local angle beta as extracted using the PixelStubs. If one of the 
// hits is not comptible with another, then the whole set is declared incompatible
bool SeedComparitorWithPixelStubs::compatible(const SeedingHitSet &seed) {
  bool good=true;
  for (unsigned int i=0; i<seed.hits().size()-1; ++i) {
    const SiPixelRecHit &pix1 = dynamic_cast<const SiPixelRecHit&>(*seed.hits()[i].RecHit());
    PixelStub stub1(pix1, betaCutFactor_);
    for (unsigned int j=i+1; j<seed.hits().size(); ++j) {
      const SiPixelRecHit &pix2 = dynamic_cast<const SiPixelRecHit&>(*seed.hits()[j].RecHit());
      PixelStub stub2(pix2, betaCutFactor_);
      
      // Now make the comparison
      if (! stub1.compatible(stub2)) good=false;
    }
  }
  return good;
}

