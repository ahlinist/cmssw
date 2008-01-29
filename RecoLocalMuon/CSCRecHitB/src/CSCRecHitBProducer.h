#ifndef CSCRecHitB_CSCRecHitBProducer_h
#define CSCRecHitB_CSCRecHitBProducer_h

/** \class CSCRecHitBProducer 
 *
 * Produces a collection of CSCRecHit2D's (2-dim space-point RecHits)
 * in endcap muon CSCs. 
 * It starts from collections of CSC wire and strip digis.
 * The treatment here is differently than from CSCRecHit2Producer 
 * existing in RecoLocalMuon/CSCRecHit as pseudo-segments are built 
 * from wire hits only and strip only hits. 
 *
 * \author Dominique Fortin
 *
 */

#include <FWCore/Framework/interface/Frameworkfwd.h>
#include <FWCore/Framework/interface/EDProducer.h>
#include <FWCore/Framework/interface/Event.h>
#include <FWCore/ParameterSet/interface/ParameterSet.h>
#include "FWCore/ParameterSet/interface/InputTag.h"

class CSCRecHitBBuilder; 
class CSCStripGainAvg;

class CSCRecHitBProducer : public edm::EDProducer {

public:
  explicit CSCRecHitBProducer( const edm::ParameterSet& ps);
  ~CSCRecHitBProducer();

  virtual void produce( edm::Event&, const edm::EventSetup& );

 private:

  // Counting events processed
  unsigned iRun; 
  bool useCalib;
  bool isData;
  bool debug;
  bool useCleanWireCollection;
  bool useCleanStripCollection;

  float CSCstripGainAvg;

  edm::InputTag stripDigiTag;
  edm::InputTag wireDigiTag;
  edm::InputTag CSCDigiTag;

  CSCRecHitBBuilder* recHitBuilder_;
  CSCStripGainAvg* stripGainAvg_;
};

#endif
