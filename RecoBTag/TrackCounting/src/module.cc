#include "FWCore/Framework/interface/ModuleFactory.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "RecoBTau/JetTagComputer/interface/JetTagComputerESProducer.h"
#include "RecoBTag/TrackCounting/interface/NegativeTrackCountingComputer.h"
#include "RecoBTag/TrackCounting/interface/TrackCountingComputer.h"
#include "RecoBTag/TrackCounting/interface/TrackCounting.h"

DEFINE_FWK_MODULE(TrackCounting);

typedef JetTagComputerESProducer<TrackCountingComputer>         TrackCountingESProducer;
DEFINE_FWK_EVENTSETUP_MODULE(TrackCountingESProducer);

typedef JetTagComputerESProducer<NegativeTrackCountingComputer> NegativeTrackCountingESProducer;
DEFINE_FWK_EVENTSETUP_MODULE(NegativeTrackCountingESProducer);
