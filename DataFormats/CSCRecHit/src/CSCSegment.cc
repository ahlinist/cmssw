/** \file CSCSegment.cc
 *
 *  $Date: 2006/04/02 10:39:53 $
 *  \author Matteo Sani
 */

#include <DataFormats/CSCRecHit/interface/CSCSegment.h>
#include <DataFormats/CSCRecHit/interface/CSCRecHit2DCollection.h>
#include <iostream>

CSCSegment::CSCSegment(std::vector<CSCRecHit2D> proto_segment, LocalPoint origin, 
	LocalVector direction, AlgebraicSymMatrix errors, double chi2) : theCSCRecHits(proto_segment),
	theOrigin(origin), theLocalDirection(direction), theCovMatrix(errors), theChi2(chi2) {

	  // Get CSCDetId from one of the rechits, but then remove the layer part
	CSCDetId id = theCSCRecHits.begin()->cscDetId();
	theDetId = CSCDetId(id.endcap(),id.station(),id.ring(),id.chamber(), 0);
}

CSCSegment::~CSCSegment() {}

std::vector<const TrackingRecHit*> CSCSegment::recHits() const{
  std::vector<const TrackingRecHit*> pointersOfRecHits;
  std::vector<CSCRecHit2D> cscRHs = specificRecHits();
  for (std::vector<CSCRecHit2D>::const_iterator irh = cscRHs.begin(); irh!=cscRHs.end(); ++irh) {
    pointersOfRecHits.push_back(&(*irh));
  }
  return pointersOfRecHits;
}

AlgebraicVector CSCSegment::parameters() const {
  AlgebraicVector result(4);
  
  if (dimension()==4) 
  	return CSCSegment::parameters();
  else {
      result[0] = theLocalDirection.x();
      result[1] = theLocalDirection.y();
      result[2] = theOrigin.x();
      result[3] = theOrigin.y();
  }
  
  return result;
}

AlgebraicSymMatrix CSCSegment::parametersError() const { 
	return theCovMatrix;
} 
LocalError CSCSegment::localPositionError() const {
  return LocalError(theCovMatrix[2][2], theCovMatrix[2][3], theCovMatrix[3][3]);
}

LocalError CSCSegment::localDirectionError() const {
  return LocalError(theCovMatrix[0][0], theCovMatrix[0][1], theCovMatrix[1][1]); 
}

void CSCSegment::print() const {
  std::cout << *this << std::endl;
}

std::ostream& operator<<(std::ostream& os, const CSCSegment& seg) {
  os << "CSCSegment: local pos = " << seg.localPosition() << 
    " dir = " << seg.localDirection() <<
    " chi2 = " << seg.chi2() << " #rechits = " << seg.specificRecHits().size();
  return os;  
}

/*
const CSCChamber* CSCSegment::chamber() const { return theChamber; }
*/
