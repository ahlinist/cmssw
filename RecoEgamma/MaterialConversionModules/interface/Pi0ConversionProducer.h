#ifndef RecoEgamma_MaterialConversionModules_Pi0ConversionProducer_h_
#define RecoEgamma_MaterialConversionModules_Pi0ConversionProducer_h_

#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "RecoEgamma/MaterialConversionTools/interface/Pi0ConversionAlgo.h"

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
  std::string conversioncollection_;
  std::string basicclusterCollection_;
  std::string basicclusterProducer_;
 
  edm::InputTag matchedStripRecHitsInputTag_;
  edm::InputTag rphiStripRecHitsInputTag_;
  edm::InputTag stereoStripRecHitsInputTag_;
  edm::InputTag pixelRecHitsInputTag_;
  double clusterdEtaMax_;
  double clusterdRMax_;
  double clusterMinPT_;
  double cluster_pt_thresh_;
  std::string clusterLabel_;
  int stubminimumhits_;
  std::string builderName_;
  std::string TTRHbuilderName_;  
  Pi0ConversionAlgo::DebugLevel debugL;  
  Float_t GetSep(float eta1, float phi1, float eta2, float phi2);
  float pimass(float eta_g, float phi_g, float e_g, float eta_e, float phi_e, float e_e, float eta_p, float phi_p, float e_p);
  bool counterExceeded() const { return ((nEvt_ > nMaxPrintout_) || (nMaxPrintout_ < 0));}
};


#endif


