#ifndef CSCRecHitB_CSCWireSegments_h
#define CSCRecHitB_CSCWireSegments_h

/** \class CSCWireSegments
 *
 * Wire hits are used to build wire segments.  The wire hits are then
 * compared to these segments and removed from the wire list if they lay
 * to far off these pseudo segment.  To do so, flag hits which have entered 
 * segment building so we don't reuse them.
 * UsedHits = 0  --> unused
 *          = 1  --> near existing segment
 *          = 2  --> used
 *
 * Hits falling in category 0 and 2 are used
 *
 * \author: Dominique Fortin - UCR
 *
 */

#include <RecoLocalMuon/CSCRecHitB/interface/CSCWireHitCollection.h>
#include <RecoLocalMuon/CSCRecHitB/interface/CSCWireHit.h>

#include <FWCore/ParameterSet/interface/ParameterSet.h>

#include <deque>
#include <map>  
#include <vector>

class CSCGeometry;
class CSCLayer;
class CSCDetId;

class CSCWireSegments
{
 public:
  
  /// Typedefs
  typedef std::vector<CSCWireHit> ChamberHitContainer;
  typedef ChamberHitContainer::const_iterator ChamberHitContainerCIt;
  
  explicit CSCWireSegments(const edm::ParameterSet& ps);
  
  ~CSCWireSegments();
  
  CSCWireHitCollection cleanWireHits(const CSCWireHitCollection& raw_woc);
  void setGeometry( const CSCGeometry* geom ) {geom_ = geom;}
  
  
 private:
  
  /** Member functions:
   *  See .cc file for description.
   */
  void findWireSegments(const ChamberHitContainer& cscWireHit);
  bool addHit(const CSCWireHit& aHit, int layer);
  void updateParameters(const CSCWireHit& aHit);
  void fitSlope(void);
  void fillChiSquared(const CSCWireHit& aHit);
  void tryAddingHitsToSegment( const ChamberHitContainer& wirehit,
			       const ChamberHitContainerCIt i1, 
			       const ChamberHitContainerCIt i2); 
  bool isHitNearSegment(const CSCWireHit& h) const;
  void flagHitNearSegment(const CSCWireHit& h, const ChamberHitContainerCIt id1, const ChamberHitContainerCIt id2); 
  bool hasHitOnLayer(int layer) const;
  void compareProtoSegment(const CSCWireHit& h, int layer);
  bool replaceHit(const CSCWireHit& h, int layer);
  void increaseProtoSegment(const CSCWireHit& h, int layer);
  bool isSegmentGood(const ChamberHitContainer& WireHitsInChamber) const;
  void flagHitsAsUsed(const ChamberHitContainer& WireHitsInChamber);
  void storeChamberHits();
  void storeLeftOverHits(const ChamberHitContainer& wirehit);
  const CSCLayer* getLayer( const CSCDetId& detId ); 
  
  /*
   * Collection of hits satisfying the segment building which will be sent back to builder
   */
  CSCWireHitCollection clean_whit;
  
  /*
   * variables used for proto segment building
   */
  ChamberHitContainer proto_segment;
  ChamberHitContainer hitsInChamber;
  int usedHits[100];
  float proto_Chi2;
  float proto_intercept;
  float proto_slope;
  float proto_y[6];
  
  
  /* 
   * Cuts used in the code:  see .cfi file for description
   */
  int minLayersApart;
  double proto_poca;
  int proto_deltaT;
  bool debug;
  bool useHitsFromFits;
  int minWireHitsPerSegment;
  int muonsPerChamberMax;
  bool storeLeftOvers;
    
  /*
   * Cache geometry for current event
   */
  const CSCGeometry* geom_;
  
};

#endif

