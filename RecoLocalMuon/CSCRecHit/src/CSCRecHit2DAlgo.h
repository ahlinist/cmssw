
#ifndef CSCRecHit_CSCRecHit2DAlgo_h
#define CSCRecHit_CSCRecHit2DAlgo_h

/** \class CSCRecHit2DAlgo
 * An abstract base class for algorithmic classes used to
 * build first-level RecHits in one layer of an Endcap Muon CSC.
 *
 * \author Tim Cox
 *
 * Implementation notes: <BR>
 * For example, CSCRecHit2DInALayer inherits from this class,
 * and classes ported from ORCA local reco inherit from that.
 */

#include <DataFormats/CSCRecHit/interface/CSCRecHit2D.h>
#include <DataFormats/CSCDigi/interface/CSCStripDigiCollection.h>
#include <DataFormats/CSCDigi/interface/CSCWireDigiCollection.h>

#include <FWCore/Framework/interface/Frameworkfwd.h>
#include <vector>

class CSCDetId;
class CSCLayer;

class CSCRecHit2DAlgo {
  public:
    explicit CSCRecHit2DAlgo( const edm::ParameterSet& ){};
    virtual ~CSCRecHit2DAlgo(){};

    /** Run the algorithm = build the rechits in this layer
    */
    virtual std::vector<CSCRecHit2D> run( const CSCDetId& id, const CSCLayer* layer, 
                      const CSCStripDigiCollection::Range& rstripd,
 	                  const CSCWireDigiCollection::Range& rwired ) = 0;  

 private:
 
};

#endif

