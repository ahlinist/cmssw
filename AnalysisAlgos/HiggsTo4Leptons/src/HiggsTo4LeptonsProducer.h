#ifndef HiggsAnalysis_HiggsTo4LeptonsProducer_H
#define HiggsAnalysis_HiggsTo4LeptonsProducer_H

/**\class HiggsTo4LeptonsProducer
 *
 * EDProducer for the H--> ZZ(*) --> 2e2mu analysis which fills collections 
 * (TTrees) with generated and reconstructed quantities
 *
 * This is meant as a first raw analysis.  Further selections will be
 * applied in a secondary EDAnalyser.
 *
 *
 * Author:  Dominique Fortin - UCR
 *
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"


class HiggsTo4LeptonsMCBuilder;


class HiggsTo4LeptonsProducer : public edm::EDProducer {

 public:
 
  explicit HiggsTo4LeptonsProducer( const edm::ParameterSet& ps);
  ~HiggsTo4LeptonsProducer();

  virtual void produce( edm::Event&, const edm::EventSetup& );

 private:

  // Counting events processed
  int iev; 

  // Flag if running on data or MC
  bool isData;
  bool isVtxSmeared;
  bool debug;


  int  ievt;

  HiggsTo4LeptonsMCBuilder* mcBuilder_;

};

#endif
