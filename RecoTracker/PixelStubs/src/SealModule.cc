#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_SEAL_MODULE();

#include "RecoTracker/TkSeedingLayers/interface/SeedComparitorFactory.h"
#include "RecoTracker/TkSeedingLayers/interface/SeedComparitor.h"
#include "RecoTracker/PixelStubs/interface/SeedComparitorWithPixelStubs.h"

DEFINE_EDM_PLUGIN(SeedComparitorFactory, SeedComparitorWithPixelStubs, "SeedComparitorWithPixelStubs");

