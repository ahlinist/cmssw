#ifndef RecoEcal_EgammaMaterialDetProducers_Pi0ConversionProducer_h_
#define RecoEcal_EgammaMaterialDetProducers_Pi0ConversionProducer_h_

#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "RecoEcal/EgammaMaterialDetAlgos/Pi0ConversionAlgo.h"

//


class Pi0ConversionProducer : public edm::EDProducer 
{
  
 public:
  
  Pi0ConversionProducer(const edm::ParameterSet& ps);
  
  ~Pi0ConversionProducer();
  
  virtual void produce(edm::Event&, const edm::EventSetup&);
  
 private:
  
  int nMaxPrintout_; // max # of printouts
  int nEvt_;         // internal counter of events
  Pi0ConversionAlgo* pi0conv_p;
  std::string basicclusterCollection_;
  std::string basicclusterProducer_;
 
  edm::InputTag matchedStripRecHitsInputTag_;
  edm::InputTag rphiStripRecHitsInputTag_;
  edm::InputTag stereoStripRecHitsInputTag_;
  edm::InputTag pixelRecHitsInputTag_;
  float clusterdEtaMax_;
  float clusterdRMax_;
  float clusterMinPT_;
  float clusterLabel_;
  int stubminimumhits_;
  
  Pi0ConversionAlgo::DebugLevel debugL;  
  
  bool counterExceeded() const { return ((nEvt_ > nMaxPrintout_) || (nMaxPrintout_ < 0));}
};


#endif


