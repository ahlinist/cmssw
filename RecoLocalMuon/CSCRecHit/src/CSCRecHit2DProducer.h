#ifndef CSCRecHit_CSCRecHit2DProducer_h
#define CSCRecHit_CSCRecHit2DProducer_h

/** \class CSCRecHit2DProducer 
 * Produces a collection of CSCRecHit2D's (2-dim space-point RecHits)
 * in endcap muon CSCs. 
 * It starts from collections of the CSC wire and strip digis.
 *
 * \author Tim Cox
 *
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class CSCRecHit2DBuilder; 

class CSCRecHit2DProducer : public edm::EDProducer {
   public:
      explicit CSCRecHit2DProducer( const edm::ParameterSet& );
      ~CSCRecHit2DProducer();

      virtual void produce( edm::Event&, const edm::EventSetup& );

   private:
      int iev; // events through
      std::string stripDigiProducer_;
      std::string wireDigiProducer_;
      CSCRecHit2DBuilder* recHitBuilder_;
};

#endif
