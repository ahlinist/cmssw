// user include files
#include "FWCore/Framework/interface/Event.h"
#include "ElectroWeakAnalysis/MultiBosons/interface/ValueMapTypeConverter.h"
#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/EgammaCandidates/interface/PhotonFwd.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"

typedef vgamma::ValueMapTypeConverter<reco::PhotonCollection, double, float> PhotonMapDoubleToFloat;
typedef vgamma::ValueMapTypeConverter<reco::GsfElectronCollection, double, float> ElectronMapDoubleToFloat;

//define this as a plug-in
DEFINE_FWK_MODULE( PhotonMapDoubleToFloat );
DEFINE_FWK_MODULE( ElectronMapDoubleToFloat );

