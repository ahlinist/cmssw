#include "PluginManager/ModuleDef.h"
#include "FWCore/Framework/interface/InputSourceMacros.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "IOPool/Streamer/interface/StreamerOutputModule.h"
#include "EventFilter/FUSender/src/FUStreamerI2OWriter.h"

typedef edm::StreamerOutputModule<edm::FUStreamerI2OWriter> I2OStreamConsumer;

using edm::FUStreamerI2OWriter;

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(I2OStreamConsumer);

