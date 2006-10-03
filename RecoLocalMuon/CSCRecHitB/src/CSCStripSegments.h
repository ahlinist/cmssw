#ifndef CSCRecHitB_CSCStripSegments_h
#define CSCRecHitB_CSCStripSegments_h

/** \class CSCStripSegments
 *
 * Strip hits are used to build strip segments.  The strip hits are then
 * compared to these segments and removed from the strip list if they lay
 * to far off pseudo segment.  To do so flag hits have followed.
 * UsedHits = 0  --> unused
 *          = 1  --> near existing segment
 *          = 2  --> very far from existing segment
 *          = 3  --> used
 * And store hits which fit in category 0, 1, 3.
 *
 * \author: Dominique Fortin - UCR
 *
 */

#include <DataFormats/CSCRecHit/interface/CSCStripHitCollection.h>
#include <DataFormats/CSCRecHit/interface/CSCStripHit.h>
#include <FWCore/ParameterSet/interface/ParameterSet.h>

#include <map>  
#include <vector>

class CSCGeometry;
class CSCLayer;
class CSCDetId;

class CSCStripSegments
{
 public:
  
  /// Typedefs
  typedef std::vector<CSCStripHit> ChamberHitContainer;
  typedef ChamberHitContainer::const_iterator ChamberHitContainerCIt;
  
  explicit CSCStripSegments(const edm::ParameterSet& ps);
  
  ~CSCStripSegments();
  
  CSCStripHitCollection cleanStripHits(const CSCStripHitCollection& raw_soc);
  void setGeometry( const CSCGeometry* geom );
  
  
 private:
  
  /** Member functions:
   *  See .cc file for description.
   */
  void findStripSegments( const ChamberHitContainer& cscStripHit );
  bool addHit( const CSCStripHit& aHit, int layer );
  void updateParameters( void );
  void fitSlope( void );
  void fillChiSquared( void );
  void tryAddingHitsToSegment( const ChamberHitContainer& striphit,
			       const ChamberHitContainerCIt i1, 
			       const ChamberHitContainerCIt i2 ); 
  bool isHitNearSegment( const CSCStripHit& h ) const;
  void flagHitNearSegment( const CSCStripHit& h, const ChamberHitContainerCIt id1, const ChamberHitContainerCIt id2 ); 
  bool hasHitOnLayer( int layer ) const;
  void compareProtoSegment( const CSCStripHit& h, int layer );
  bool replaceHit( const CSCStripHit& h, int layer );
  void increaseProtoSegment( const CSCStripHit& h, int layer );
  bool isSegmentGood( const ChamberHitContainer& StripHitsInChamber ) const;
  void flagHitsAsUsed( const ChamberHitContainer& StripHitsInChamber );
  void storeChamberHits( void );
  void storeLeftOverHits( const ChamberHitContainer& striphit );
  const CSCLayer* getLayer( const CSCDetId& detId ); 
  float getStripOffset( const CSCStripHit& aHit );
  
  /*
   * Collection of hits satisfying the segment building which will be sent back to builder
   */
  CSCStripHitCollection clean_shit;
  
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
  
  
  
  /// Cuts used in the code:  see .cfi file for description
  int minLayersApart;
  double proto_poca;
  double isFarFromSegment;
  double proto_deltaT;
  int minStripHitsPerSegment;
  bool useHitsFromFits;
  bool debug;
  bool storeLeftOvers;
  int muonsPerChamberMax;
  
  /*
   * Cache geometry for current event
   */
  const CSCGeometry* geom_;
  

};

#endif

