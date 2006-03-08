#ifndef CSCRecHit_CSCRecHit2DFromWiresOnly_h
#define CSCRecHit_CSCRecHit2DFromWiresOnly_h

/** \class CSCRecHit2DFromWiresOnly
 * A concrete algorithm to build 2D space-point RecHits
 * from wire information _alone_ in the Endcap Muon CSCs 
 * (This was once a potential scenario for ME1/A and
 * don't want to throw away functionality...)
 *
 * \author Rick Wilkinson, Tim Cox
 *
 */

#include <RecoLocalMuon/CSCRecHit/src/CSCRecHit2DInALayer.h>
#include <FWCore/ParameterSet/interface/ParameterSet.h>

class CSCLayer;
class CSCDetId;

class CSCRecHit2DFromWiresOnly : public CSCRecHit2DInALayer
{
public:
  explicit CSCRecHit2DFromWiresOnly( const edm::ParameterSet& ps ) : CSCRecHit2DInALayer( ps ) {}
  virtual ~CSCRecHit2DFromWiresOnly(){}

  virtual std::vector<CSCRecHit2D> run( 
                      const CSCDetId& id, const CSCLayer* layer, 
                      const CSCStripDigiCollection::Range& rstripd,
	              const CSCWireDigiCollection::Range& rwired ) const; 

};

#endif

