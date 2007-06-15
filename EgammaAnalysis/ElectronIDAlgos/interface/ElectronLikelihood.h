#ifndef ElectronLikelihood_H
#define ElectronLikelihood_H

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include "EgammaAnalysis/ElectronIDAlgos/interface/ElectronIDAlgo.h"

class ElectronLikelihood : ElectronIDAlgo {

public:

  ElectronLikelihood(){};

  virtual ~ElectronLikelihood(){};

  void setup(const edm::ParameterSet& conf){};
  double result(const reco::PixelMatchGsfElectron* electron, const edm::Event&);

 private:

};

#endif // ElectronLikelihood_H
