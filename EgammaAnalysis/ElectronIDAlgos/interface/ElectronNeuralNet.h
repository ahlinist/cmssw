#ifndef ElectronNeuralNet_H
#define ElectronNeuralNet_H

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include "EgammaAnalysis/ElectronIDAlgos/interface/ElectronIDAlgo.h"

class ElectronNeuralNet : ElectronIDAlgo{

public:

  ElectronNeuralNet(){};

  virtual ~ElectronNeuralNet(){};

  void setup(const edm::ParameterSet& conf){};
  double result(const reco::PixelMatchGsfElectron* electron, const edm::Event&);

 private:

};

#endif // ElectronNeuralNet_H
