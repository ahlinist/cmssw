#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"


#include "EgammaAnalysis/EgammaIsolationProducers/interface/EgammaHcalIsolationProducer.h"
#include "EgammaAnalysis/EgammaIsolationProducers/interface/EgammaEcalIsolationProducer.h"
//#include "EgammaAnalysis/EgammaIsolationProducers/interface/EgammaElectronTkIsolationProducer.h"
//#include "EgammaAnalysis/EgammaIsolationProducers/interface/EgammaElectronTkNumIsolationProducer.h"
//#include "EgammaAnalysis/EgammaIsolationProducers/interface/EgammaPhotonTkIsolationProducer.h"
//#include "EgammaAnalysis/EgammaIsolationProducers/interface/EgammaPhotonTkNumIsolationProducer.h"
//#include "EgammaAnalysis/EgammaIsolationProducers/interface/EgammaTowerIsolationProducer.h"
//#include "EgammaAnalysis/EgammaIsolationProducers/interface/EgammaEcalRecHitIsolationProducer.h"


DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(EgammaHcalIsolationProducer);
DEFINE_ANOTHER_FWK_MODULE(EgammaEcalIsolationProducer);
//DEFINE_ANOTHER_FWK_MODULE(EgammaElectronTkIsolationProducer);
//DEFINE_ANOTHER_FWK_MODULE(EgammaElectronTkNumIsolationProducer);
//DEFINE_ANOTHER_FWK_MODULE(EgammaPhotonTkIsolationProducer);
//DEFINE_ANOTHER_FWK_MODULE(EgammaPhotonTkNumIsolationProducer);
//DEFINE_ANOTHER_FWK_MODULE(EgammaTowerIsolationProducer);
//DEFINE_ANOTHER_FWK_MODULE(EgammaEcalRecHitIsolationProducer);

