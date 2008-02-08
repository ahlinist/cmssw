#ifndef ElectronNeuralNet_H
#define ElectronNeuralNet_H

#include "EgammaAnalysis/ElectronIDAlgos/interface/ElectronIDAlgo.h"

#include "TMVA/Reader.h"
#include "TMVA/Factory.h"
#include "TMVA/MethodMLP.h"
#include "TMVA/Tools.h"

class ElectronNeuralNet : ElectronIDAlgo{

 public:

  ElectronNeuralNet(){};

  virtual ~ElectronNeuralNet(){};

  void setup(const edm::ParameterSet& conf);
  double result(const reco::GsfElectron* electron, const edm::Event&);

 private:
  edm::ParameterSet conf_;

  int iClass;
  float deta_in;
  float p_in, E,p_out,e1max;
  float covEtaEta,covEtaPhi,covPhiPhi,HoE,deta_out,dphi_in,dphi_out;
  float isoHad;
  float EoP_inv;
  float retval;

  string mySet_;
  string subDet;
  string weightsDir_;

  TMVA::Reader *readerBarrel, *readerEndcap ;
};

#endif // ElectronNeuralNet_H
