#ifndef CutBasedElectronID_H
#define CutBasedElectronID_H

#include "EgammaAnalysis/ElectronIDAlgos/interface/ElectronIDAlgo.h"

class CutBasedElectronID : ElectronIDAlgo {

public:

  CutBasedElectronID(){};

  virtual ~CutBasedElectronID(){};

  void setup(const edm::ParameterSet& conf);
  bool result(const reco::PixelMatchGsfElectron*, const edm::Event&);
  int classify(const reco::PixelMatchGsfElectron*);
  
 private:
  
  std::string quality_;
  edm::ParameterSet cuts_;
  
};

#endif // PTDRElectronID_H
