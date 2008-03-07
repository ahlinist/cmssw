#ifndef ElectronIDSelectorCutBased_h
#define ElectronIDSelectorCutBased_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "EgammaAnalysis/ElectronIDAlgos/interface/PTDRElectronID.h"
#include "EgammaAnalysis/ElectronIDAlgos/interface/CutBasedElectronID.h"

class ElectronIDSelectorCutBased
{
 public:

  explicit ElectronIDSelectorCutBased (const edm::ParameterSet& conf) ;
  virtual ~ElectronIDSelectorCutBased () ;

  void newEvent (const edm::Event& e, const edm::EventSetup& c) ;
  double operator() (const reco::GsfElectron & electron, const edm::Event & event) ;
   
 private:

  PTDRElectronID* ptdrAlgo_;
  CutBasedElectronID* cutBasedAlgo_; 	   

  edm::ParameterSet conf_;
  
  bool doPtdrId_;
  bool doCutBased_;

};

#endif
