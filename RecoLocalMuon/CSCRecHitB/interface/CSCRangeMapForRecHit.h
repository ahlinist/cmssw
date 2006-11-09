#ifndef CSCRecHitB_CSCRangeMapForRecHit_H
#define CSCRecHitB_CSCRangeMapForRecHit_H

/** \class CSCRangeMapForRecHit
 *  Comparator to retrieve CSCrechits by chamber or by layer. 
 *
 *  \author Dominique Fortin
 */

#include <DataFormats/MuonDetId/interface/CSCDetId.h>

class CSCDetIdSameDetLayerComparator {
 public:
  bool operator() (CSCDetId i1, CSCDetId i2) const;
};

class CSCDetIdSameChamberComparator {
 public:
  bool operator()(CSCDetId i1, CSCDetId i2) const;
};

class CSCRangeMapForRecHit {
 public:
  
  /// Constructor
  CSCRangeMapForRecHit();

  /// Destructor
  virtual ~CSCRangeMapForRecHit();
  
  ///  Returns a valid DetId + a valid comparator for the RangeMap.
  static std::pair<CSCDetId,CSCDetIdSameChamberComparator> cscChamber(CSCDetId id);
  static std::pair<CSCDetId,CSCDetIdSameDetLayerComparator> cscDetLayer(CSCDetId id);

 private:
   
};
 
#endif

