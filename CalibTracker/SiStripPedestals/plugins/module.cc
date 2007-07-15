#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/SourceFactory.h"
DEFINE_SEAL_MODULE();

#include "CalibTracker/SiStripPedestals/interface/SiStripPedDB.h"
DEFINE_ANOTHER_FWK_MODULE(SiStripPedDB);

#include "CalibTracker/SiStripPedestals/interface/SiStripNoiseDB.h"
DEFINE_ANOTHER_FWK_MODULE(SiStripNoiseDB);

#include "CalibTracker/SiStripPedestals/interface/SiStripOfflinePedNoiseToDb.h"
using cms::SiStripOfflinePedNoiseToDb;
DEFINE_ANOTHER_FWK_MODULE(SiStripOfflinePedNoiseToDb);

#include "CalibTracker/SiStripPedestals/interface/SiStripNoiseDummyCalculator.h"
DEFINE_ANOTHER_FWK_MODULE(SiStripNoiseDummyCalculator);

#include "CalibTracker/SiStripPedestals/interface/SiStripNoiseFakeESSource.h"
DEFINE_ANOTHER_FWK_EVENTSETUP_SOURCE(SiStripNoiseFakeESSource);

#include "CalibTracker/SiStripPedestals/interface/SiStripPedestalsFakeESSource.h"
DEFINE_ANOTHER_FWK_EVENTSETUP_SOURCE(SiStripPedestalsFakeESSource);

