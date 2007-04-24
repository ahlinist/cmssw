#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_SEAL_MODULE();

using cms::SiStripOfflinePedNoiseToDb;

#include "CalibTracker/SiStripPedestals/interface/SiStripPedDB.h"
DEFINE_ANOTHER_FWK_MODULE(SiStripPedDB);

#include "CalibTracker/SiStripPedestals/interface/SiStripNoiseDB.h"
DEFINE_ANOTHER_FWK_MODULE(SiStripNoiseDB);

#include "CalibTracker/SiStripPedestals/interface/SiStripOfflinePedNoiseToDb.h"
DEFINE_ANOTHER_FWK_MODULE(SiStripOfflinePedNoiseToDb);
