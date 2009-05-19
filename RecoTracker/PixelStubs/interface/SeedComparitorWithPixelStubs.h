#ifndef PixelStubs_SeedComparitorWithPixelStubs_h
#define PixelStubs_SeedComparitorWithPixelStubs_h
/** \class SeedComparitorWithPixelStubs SeedComparitorWithPixelStubs.h RecoTracker/PixelStubs/interface/SeedComparitorWithPixelStubs.h 
 * Inherits from abstract base class to compare pixel seeds using the the Pixel Template CPE.
 *  \author Dave Fehling
 */
#include "RecoTracker/TkSeedingLayers/interface/SeedingHitSet.h"
#include "RecoTracker/TkSeedingLayers/interface/SeedComparitor.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class PixelCPEBase;

class SeedComparitorWithPixelStubs : public SeedComparitor {
 public:
  SeedComparitorWithPixelStubs(const edm::ParameterSet & cfg);

  virtual ~SeedComparitorWithPixelStubs();
  
  virtual bool compatible(const SeedingHitSet &hits, const edm::EventSetup &es);
 private:
	double tempCutFactor_;
	mutable const PixelCPEBase *theCPE;
};

#endif
