// user include files
#include "FWCore/Framework/interface/Event.h"
#include "ElectroWeakAnalysis/MultiBosons/interface/EgammaUserDataProducer.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"

typedef
vgamma::EgammaUserDataProducer<
  reco::GsfElectron
> ElectronUserDataProducer;

//define this as a plug-in
DEFINE_FWK_MODULE( ElectronUserDataProducer );

