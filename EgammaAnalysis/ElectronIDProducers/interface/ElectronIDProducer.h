#ifndef ElectronIDProducer_h
#define ElectronIDProducer_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include "EgammaAnalysis/ElectronIDAlgos/interface/CutBasedElectronID.h"
#include "EgammaAnalysis/ElectronIDAlgos/interface/ElectronLikelihood.h"
#include "EgammaAnalysis/ElectronIDAlgos/interface/ElectronNeuralNet.h"

class ElectronIDProducer : public edm::EDProducer
{
 public:

  explicit ElectronIDProducer(const edm::ParameterSet& conf);
  virtual ~ElectronIDProducer();

  virtual void beginJob(edm::EventSetup const& iSetup);
  virtual void produce(edm::Event& e, const edm::EventSetup& c);
   
 private:

  CutBasedElectronID* cutBasedAlgo_;
  ElectronLikelihood* likelihoodAlgo_;
  ElectronNeuralNet* neuralNetAlgo_;

  edm::ParameterSet conf_;

  std::string electronProducer_;
  std::string electronLabel_;
  std::string electronIDLabel_;
  std::string electronIDAssociation_; //association map

  bool doCutBased_;
  bool doLikelihood_;
  bool doNeuralNet_;

};

#endif
