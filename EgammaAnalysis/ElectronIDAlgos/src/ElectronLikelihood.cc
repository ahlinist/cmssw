#include "EgammaAnalysis/ElectronIDAlgos/interface/ElectronLikelihood.h"

double ElectronLikelihood::result(const reco::PixelMatchGsfElectron* electron,
				  const edm::Event& e) {
  return 1.;
}
