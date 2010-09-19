// user include files
#include "FWCore/Framework/interface/Event.h"
#include "ElectroWeakAnalysis/MultiBosons/interface/AsscToValueToValueMapConverter.h"
#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/EgammaCandidates/interface/PhotonFwd.h"

typedef vgamma::AsscToValueToValueMapConverter<reco::PhotonCollection, float> PhotonAsscToValMap;
//define this as a plug-in
DEFINE_FWK_MODULE( PhotonAsscToValMap );

