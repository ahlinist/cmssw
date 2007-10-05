#ifndef PTDRElectronID_H
#define PTDRElectronID_H

#include "EgammaAnalysis/ElectronIDAlgos/interface/ElectronIDAlgo.h"

class PTDRElectronID : ElectronIDAlgo {

public:

  PTDRElectronID(){};

  virtual ~PTDRElectronID(){};

  void setup(const edm::ParameterSet& conf);
  bool result(const reco::PixelMatchGsfElectron*, const edm::Event&);

 private:

  std::string quality_;
  edm::ParameterSet cuts_;

};

#endif // PTDRElectronID_H
