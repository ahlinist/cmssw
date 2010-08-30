// user include files
#include "FWCore/Framework/interface/Event.h"
#include "ElectroWeakAnalysis/MultiBosons/interface/EgammaUserDataProducer.h"
#include "DataFormats/EgammaCandidates/interface/Photon.h"

typedef vgamma::EgammaUserDataProducer<reco::Photon> PhotonUserDataProducer;
//define this as a plug-in
DEFINE_FWK_MODULE( PhotonUserDataProducer );

