// $Id: SealModule.cc,v 1.18 2009/03/27 13:59:24 sharper Exp $

#include "DQMOffline/Trigger/interface/FourVectorHLTClient.h"
#include "DQMOffline/Trigger/interface/FourVectorHLTOffline.h"
#include "DQMOffline/Trigger/interface/EgHLTOfflineSource.h"
#include "DQMOffline/Trigger/interface/EgHLTOfflineClient.h"
#include "DQMOffline/Trigger/interface/EgHLTOfflineSummaryClient.h"
#include "DQMOffline/Trigger/interface/HLTTauRefProducer.h"
#include "DQMOffline/Trigger/interface/HLTTauDQMOfflineSource.h"
#include "DQMOffline/Trigger/interface/HLTTauPostProcessor.h"
#include "DQMOffline/Trigger/interface/JetMETHLTOfflineSource.h"

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(FourVectorHLTClient);
DEFINE_ANOTHER_FWK_MODULE(FourVectorHLTOffline);
DEFINE_ANOTHER_FWK_MODULE(EgHLTOfflineSource);
DEFINE_ANOTHER_FWK_MODULE(EgHLTOfflineClient);
DEFINE_ANOTHER_FWK_MODULE(EgHLTOfflineSummaryClient);
DEFINE_ANOTHER_FWK_MODULE(HLTTauRefProducer);
DEFINE_ANOTHER_FWK_MODULE(HLTTauDQMOfflineSource);
DEFINE_ANOTHER_FWK_MODULE(HLTTauPostProcessor);
DEFINE_ANOTHER_FWK_MODULE(JetMETHLTOfflineSource);


