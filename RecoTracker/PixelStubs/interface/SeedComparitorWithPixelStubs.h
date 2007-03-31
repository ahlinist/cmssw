#ifndef PixelStubs_SeedComparitorWithPixelStubs_h
#define PixelStubs_SeedComparitorWithPixelStubs_h
/** \class SeedComparitorWithPixelStubs SeedComparitorWithPixelStubs.h RecoTracker/PixelStubs/interface/SeedComparitorWithPixelStubs.h 
 * Inherits from abstract base class to compare pixel seeds using the PixelStub class.
 *  \author Aaron Dominguez (UNL)
 */
#include "RecoTracker/TkSeedingLayers/interface/SeedingHitSet.h"
#include "RecoTracker/TkSeedingLayers/interface/SeedComparitor.h"

namespace edm { class ParameterSet; }

class SeedComparitorWithPixelStubs : public SeedComparitor {
 public:
  SeedComparitorWithPixelStubs(const edm::ParameterSet & cfg);
  SeedComparitorWithPixelStubs(double betaCutFactor);
  virtual ~SeedComparitorWithPixelStubs();
  
  virtual bool compatible(const SeedingHitSet &hits);

 private:
  double betaCutFactor_;
};

#endif
