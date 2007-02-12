#include "PluginManager/ModuleDef.h"
#include "FWCore/Framework/interface/InputSourceMacros.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "IOPool/Streamer/interface/StreamerOutputModule.h"
#include "EventFilter/FUSender/src/FUStreamerI2OWriter.h"
#include "FWCore/ServiceRegistry/interface/ServiceMaker.h"
#include "EventFilter/FUSender/interface/DQMSenderService.h"

typedef edm::StreamerOutputModule<edm::FUStreamerI2OWriter> I2OStreamConsumer;

using edm::FUStreamerI2OWriter;
using namespace edm::serviceregistry;

typedef AllArgsMaker<DQMSenderService> dssMaker;

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(I2OStreamConsumer);
DEFINE_ANOTHER_FWK_SERVICE_MAKER(DQMSenderService,dssMaker);
