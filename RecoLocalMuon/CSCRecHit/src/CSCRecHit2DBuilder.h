#ifndef CSCRecHit_CSCRecHit2DBuilder_h
#define CSCRecHit_CSCRecHit2DBuilder_h

/** \class CSCRecHit2DBuilder 
 * Algorithm to build CSCRecHit2D's from wire and strip digis
 * in endcap muon CSCs by implementing a 'build' function 
 * required by CSCRecHit2DProducer.
 *
 * \author Tim Cox
 *
 * Implementation notes: <BR>
 * Configured via the Producer's ParameterSet. <BR>
 * Presume this might become an abstract base class one day. <BR>
 *
 */

#include <DataFormats/CSCRecHit/interface/CSCRecHit2DCollection.h>
#include <DataFormats/CSCDigi/interface/CSCStripDigiCollection.h>
#include <DataFormats/CSCDigi/interface/CSCWireDigiCollection.h>

#include <FWCore/ParameterSet/interface/ParameterSet.h>

//class CSCRecHit2DCollection;
//class CSCStripDigiCollection;
//class CSCWireDigiCollection;
class CSCLayer;
class CSCGeometry;
class CSCRecHit2DAlgo;
class CSCDetId;

class CSCRecHit2DBuilder
 {
   public:
   
      /** Configure the algorithm via ctor.
	   * Receives ParameterSet percolated down from EDProducer
	   * which owns this Builder.
       */
      explicit CSCRecHit2DBuilder( const edm::ParameterSet& );

      ~CSCRecHit2DBuilder();

      /** Find digis in each CSCLayer, build RecHit2D's in each layer,
       *  and fill into output collection.
       *
       * I expect this function to identify the actual local reco algorithm
       * appropriate for a given chamber from that chamber's associated chamber type.
       * These algorithms will register themselves to their base class, and
       * know to which chamber-type(s) each is to be applied. 
       * This allows the existing ORCA local reco to be ported relatively
       * straightforwardly, at least for a first prototype in CMSSW.
       */
      void build( const CSCStripDigiCollection* stripds,
                  const CSCWireDigiCollection* wireds,
		          CSCRecHit2DCollection& oc );

      /** Cache pointer to geometry _for current event_
       */
      void setGeometry( const CSCGeometry* geom );

   private:

      /** Get layer corresponding to given detid
       */
      const CSCLayer* getLayer( const CSCDetId& detId ) const;

      /** Get algo registered for given chamber type
       */
      CSCRecHit2DAlgo* getAlgo( int iChamberType );

      /** Cache geometry for current event
       */
      const CSCGeometry* geom_;

      /** Map chamber type to algorithm by chamber type index
       */
      std::vector<CSCRecHit2DAlgo*> algos_;

      /** Store algorithms we build
       */
      std::vector<CSCRecHit2DAlgo*> algobuf_;
};

#endif
