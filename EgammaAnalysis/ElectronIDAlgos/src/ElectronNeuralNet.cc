#include "EgammaAnalysis/ElectronIDAlgos/interface/ElectronNeuralNet.h"

double ElectronNeuralNet::result(const reco::PixelMatchGsfElectron* electron,
				 const edm::Event& e) {
  return 1.;
}
