// This is  CSCHitFromStripOnly.cc

#include <DataFormats/MuonDetId/interface/CSCDetId.h>
#include <DataFormats/CSCDigi/interface/CSCStripDigi.h>
#include <DataFormats/CSCRecHit/interface/CSCStripHit.h>
#include <DataFormats/CSCRecHit/interface/CSCStripHitCollection.h>

#include <RecoLocalMuon/CSCRecHitB/src/CSCHitFromStripOnly.h>
#include <RecoLocalMuon/CSCRecHitB/src/CSCPeakBinOfStripPulse.h>
#include <RecoLocalMuon/CSCRecHitB/src/CSCStripData.h>

#include <Geometry/CSCGeometry/interface/CSCLayer.h>
#include <Geometry/CSCGeometry/interface/CSCChamberSpecs.h>
#include <Geometry/CSCGeometry/interface/CSCLayerGeometry.h>

#include <FWCore/MessageLogger/interface/MessageLogger.h>
#include <FWCore/Utilities/interface/Exception.h>

#include <algorithm>
#include <cmath>
#include <string>

#include <iostream>


CSCHitFromStripOnly::CSCHitFromStripOnly( const edm::ParameterSet& ps ) {
  
  theClusterSize         = ps.getUntrackedParameter<int>("CSCStripClusterSize");
  theClusterChargeCut    = ps.getUntrackedParameter<double>("CSCStripClusterChargeCut");
  theThresholdForAPeak   = ps.getUntrackedParameter<double>("CSCStripPeakThreshold");
  stripHituse3timeBin    = ps.getUntrackedParameter<bool>("CSCstripHituse3timeBin");
  
  pulseheightOnStripFinder_   = new CSCPeakBinOfStripPulse();
}


CSCHitFromStripOnly::~CSCHitFromStripOnly() {
  delete pulseheightOnStripFinder_;
}


/* runStrip
 *
 * Search for strip with ADC output exceeding theThresholdForAPeak.  For each of these strips,
 * build a cluster of strip of size theClusterSize (typically 5 strips).  Finally, make
 * a Strip Hit out of these clusters by finding the center-of-mass position of the hit
 */
std::vector<CSCStripHit> CSCHitFromStripOnly::runStrip( const CSCDetId& id, const CSCLayer* layer,
                                                        const CSCStripDigiCollection::Range& rstripd ) {	
  
  std::vector<CSCStripHit> hitsInLayer;
  
  // cache layer info for ease of access
  id_ = id;
  layer_ = layer;
  layergeom_ = layer_->geometry();
  
  specs_ = &( dynamic_cast<const CSCChamberSpecs&>(layer->type()) );
  
  // find clusters of strips
  fillPulseHeights( rstripd );
  findMaxima();    
  
  // Make a Strip Hit out of each strip local maximum
  for ( size_t imax = 0; imax < theMaxima.size(); ++imax ) {
    tmax_of_cluster = 5;
    float strippos = makeCluster( theMaxima[imax] );  // tmax_of_cluster is also found here
    
    if ( strippos < 0 ) continue;
    
    CSCStripHit striphit( id, strippos, tmax_of_cluster );
    hitsInLayer.push_back( striphit ); 
  }
  return hitsInLayer;
}


/* makeCluster
 *
 */
float CSCHitFromStripOnly::makeCluster( int centerStrip ) {
  
  float strippos = -1.;
  int ClusterSize = theClusterSize;
  std::vector<CSCStripData> stripDataV;
  
  // We only want to use strip position in terms of strip # for the strip hit.
  
  // If Maximum on edge of detector, can only use the center of the strip...
  if ( centerStrip == 1 || centerStrip == specs_->nStrips() ) ClusterSize = 1;
  
  // If the cluster size is such that you go beyond the edge of detector, shrink cluster appropriatly
  for ( int i = 1; i < 9; i++) {
    if ( centerStrip - ClusterSize/2 < 1 || centerStrip + ClusterSize/2 > specs_->nStrips() ) {
      ClusterSize = theClusterSize - 2*i;  // Shrink cluster size by 2 to keep it and odd number.
    } else {
      continue;
    }
  }
  
  for ( int i = -ClusterSize/2; i <= ClusterSize/2; ++i ) {
    CSCStripData data = makeStripData(centerStrip, i, ClusterSize);
    stripDataV.push_back( data );
  }
  
  strippos = findHitOnStripPosition( stripDataV );
  tmax_of_cluster = findTmaxofCluster( stripDataV );
  
  return strippos;
}


/* makeStripData
 *
 */
CSCStripData CSCHitFromStripOnly::makeStripData(int centerStrip, int offset, int ClusterSize) {
  
  CSCStripData prelimData(-1.,0.,0.,0.,0);
  int thisStrip = centerStrip+offset;
  
  if ( offset == 0 ) {
    prelimData = thePulseHeightMap[centerStrip];
  } else {
    int sign = offset>0 ? 1 : -1;
    
    // if there's another maximum that would like to use part of this cluster, it gets shared in proportion to the height of the maxima
    for ( int i = 1; i <= ClusterSize/2; ++i ) {
      // go in the direction of the offset
      int testStrip = thisStrip + sign*i;
      std::vector<int>::iterator otherMax = find(theMaxima.begin(), theMaxima.end(), testStrip);
      if ( otherMax == theMaxima.end() ) {
	prelimData = thePulseHeightMap[thisStrip];
      } else {
	float heightCenter0   = thePulseHeightMap[centerStrip].y0();
	float heightCenter    = thePulseHeightMap[centerStrip].y();
	float heightCenter2   = thePulseHeightMap[centerStrip].y2();
	float heightNeighbor0 = thePulseHeightMap[testStrip].y0();
	float heightNeighbor  = thePulseHeightMap[testStrip].y();
	float heightNeighbor2 = thePulseHeightMap[testStrip].y2();
	float ratio0          = heightCenter0/ (heightNeighbor0+heightCenter0);
	float ratio           = heightCenter / (heightNeighbor +heightCenter );
	float ratio2          = heightCenter2/ (heightNeighbor2+heightCenter2);
	float thisHeight0     = thePulseHeightMap[thisStrip].y0() * ratio0;
	float thisHeight      = thePulseHeightMap[thisStrip].y()  * ratio; 
	float thisHeight2     = thePulseHeightMap[thisStrip].y2() * ratio2;
	int thisTmax          = thePulseHeightMap[thisStrip].t();
	prelimData = CSCStripData(thisStrip, thisHeight0, thisHeight, thisHeight2, thisTmax);
      }
    }
  }
  return prelimData;
}


/* fillPulseHeights
 *
 */
void CSCHitFromStripOnly::fillPulseHeights( const CSCStripDigiCollection::Range& rstripd ) {
  
  // Loop over strip digis and fill the pulseheight map
  
  thePulseHeightMap.clear();
  thePulseHeightMap.resize(100);
  
  int lastChannel = -1;
  int lastFirstSCA = -999;
  
  for ( CSCStripDigiCollection::const_iterator it = rstripd.first; it != rstripd.second; ++it ) {
    bool fill = true;
    int thisChannel = (*it).getStrip(); 
    std::vector<int> sca = (*it).getADCCounts();
    int thisFirstSCA = sca[0];
    if ( thisChannel == lastChannel ) {
      if ( thisFirstSCA - lastFirstSCA > 5. ) {
	fill = false; // use last one: prevent overwrite of last with this one
      }
    }
    lastChannel = thisChannel;
    lastFirstSCA = thisFirstSCA;
    
    if ( fill ) {
      double height[3], sigma;
      int tmax;
      pulseheightOnStripFinder_->peakAboveBaseline( (*it), *specs_, height[1], tmax, sigma);
      pulseheightOnStripFinder_->signalAboveBaseline( (*it), *specs_, height[0], tmax-1);
      pulseheightOnStripFinder_->signalAboveBaseline( (*it), *specs_, height[2], tmax+1);
      thePulseHeightMap[thisChannel] = CSCStripData( float(thisChannel), height[0], height[1], height[2], tmax);
    }
  }
  //  correctForCrosstalk( rstripd );
}


/* findMaxima
 *
 * fills vector theMaxima with the local maxima in the pulseheight distribution
 * of the strips. The threshold defining a maximum is a configurable parameter.
 * A typical value is 30.
 */
void CSCHitFromStripOnly::findMaxima() {
  
  theMaxima.clear();
  for ( size_t i = 1; i < thePulseHeightMap.size(); ++i ) {
    // sum 3 strips so that hits between strips are not suppressed
    float height = thePulseHeightMap[i-1].y() +
      thePulseHeightMap[i].y()   +
      thePulseHeightMap[i+1].y();
    if ( height > theThresholdForAPeak ) {
      // check neighbors of strip i  Don't worry about bounds;
      // should be OK at 0 and beyond last strip
      if ( thePulseHeightMap[i].y() > thePulseHeightMap[i-1].y() &&
	   thePulseHeightMap[i].y() >= thePulseHeightMap[i+1].y()) {
	theMaxima.push_back(i);
      }
    }
  }
}


/* findHitOnStripPosition
 *
 * 
 */
float CSCHitFromStripOnly::findHitOnStripPosition( const std::vector<CSCStripData>& data ) {
  
  float strippos = -1.;
  
  int nstrips = data.size();
  if ( nstrips == 0 ) return strippos;
  
  // biggestStrip is strip with largest pulse height --> use pointer subtraction
  int biggestStrip = max_element(data.begin(), data.end()) - data.begin();
  
  // If more than one strip:  use ClusterSize x 3 centroid to find center of cluster ( N strips x 3 time bins )
  float sum=0.;
  float sum_w=0.;
  
  if ( !stripHituse3timeBin ) {  
    // This uses only 1 time bin
    for ( unsigned i = 0; i != data.size(); i++ ) {
      float w = data[i].y();
      if (w < 0.) w = 0.;
      sum_w += w;
      sum  += w * data[i].x();
    }
  } else {  
    //    This uses 3 time bins, but ignore for now.
    for ( unsigned i = 0; i != data.size(); i++ ) {
      float w0 = data[i].y0();
      float w = data[i].y();
      float w2 = data[i].y2();
      if (w0 < 0.) w0 = 0.;
      if (w < 0.) w = 0.;
      if (w2 < 0.) w2 = 0.;
      float tot_w = w0 + w + w2;
      sum_w += tot_w;
      sum += tot_w * data[i].x();
    }
  }
  
  if ( sum_w > 0. ) {
    strippos = sum / sum_w;
  } else {
    strippos = data[biggestStrip].x();
  } 
  return strippos;
}


/* findTmaxofCluster
 * 
 * Simply find time bin when maximu hit occurs
 */
int CSCHitFromStripOnly::findTmaxofCluster( const std::vector<CSCStripData>& data ) {
  
  int tmax = -1;
  int nstrips = data.size();
  if ( nstrips == 0 ) return tmax;
  
  tmax = data[0].t();
  return tmax;
}


/* This is not adequate, ignore for now
 
void CSCHitFromStripOnly::correctForCrosstalk( const CSCStripDigiCollection::Range& rstripd ) {
  // loop over the map, and do a correction for each entry
  for ( size_t istrip = 1; istrip < thePulseHeightMap.size(); ++istrip ) {
    if ( thePulseHeightMap[istrip].y() > 0 ) {
      
      int theTbin = thePulseHeightMap[istrip].t();
      
      // find the digis corresponding to this strip and nearest neighbors.
      for ( CSCStripDigiCollection::const_iterator it = rstripd.first; it != rstripd.second; ++it ) {
	CSCStripDigi digi = *it;
	unsigned int jstrip = digi.getStrip();
	if ( jstrip == istrip ) {
	  // add back in what got removed
	  thePulseHeightMap[istrip] += 2.*crosstalkLevel( digi, theTbin );
	} else if ( abs(jstrip - istrip) == 1 ) {
	  // subtract off what came from that neighbor
	  thePulseHeightMap[istrip] += -1.*crosstalkLevel( digi, theTbin );
	}
      }
    }
  }
}


float CSCHitFromStripOnly::crosstalkLevel(const CSCStripDigi & digi, int& theTbin) {
  // Don't even bother for small signals -
  //@@ check 5th bin, which is where peak is supposed to fall
  std::vector<int> sca = digi.getADCCounts();
  if ( sca[theTbin] < theClusterChargeCut) return 0.;
  
  // Make crosstalk proportional to the slope in the digi
  // our time-variable will be the ratio of the SCA values.
  float slope =  sca[theTbin]/( sca[theTbin-1]+0.1 ) - 1.;
  if ( slope < 0. ) return 0.;
  if ( slope > 2.5 ) slope = 2.5;
  
  // Correction factor is 0 for ratio of 1, and 2.6% for ratio of 2...
  //@@ Need theCrossTalkLevel to be configurable
  float theCrosstalkLevel = 0.026;
  
  // Smaller chambers get half the crosstalk
  //@@ What happened to ME1a ??
  if ( id_.station() == 1 || id_.ring() == 1) {
    theCrosstalkLevel /= 2.;
  }
  return theCrosstalkLevel * sca[theTbin] * slope;
}


*/
