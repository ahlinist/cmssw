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
  specs_ = layer->chamber()->specs();  
  
  // find clusters of strips
  fillPulseHeights( rstripd );
  findMaxima();    
  
  // Make a Strip Hit out of each strip local maximum
  for ( size_t imax = 0; imax < theMaxima.size(); ++imax ) {

    // Initialize parameters entering the CSCStripHit
    tmax_cluster = 5;
    t_peak = -999.;
    ClusterSize = theClusterSize;
    strips_adc.clear();

    // tmax_of_cluster is also found here as well as size of cluster used, and adc outputs for each strips * 3 time bins    
    float strippos = makeCluster( theMaxima[imax] );  
    
    if ( strippos < 0 ) continue;
    
    CSCStripHit striphit( id, strippos, tmax_cluster, t_peak, ClusterSize, strips_adc );
    hitsInLayer.push_back( striphit ); 
  }
  return hitsInLayer;
}


/* makeCluster
 *
 */
float CSCHitFromStripOnly::makeCluster( int centerStrip ) {
  
  float strippos = -1.;
  ClusterSize = theClusterSize;
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
    CSCStripData data = makeStripData(centerStrip, i);
    stripDataV.push_back( data );
  }
  
  strippos = findHitOnStripPosition( stripDataV );
  tmax_cluster = findTmaxofCluster( stripDataV );
  
  return strippos;
}


/* makeStripData
 *
 */
CSCStripData CSCHitFromStripOnly::makeStripData(int centerStrip, int offset ) {
  
  CSCStripData prelimData(-1.,0.,0.,0.,0,-1.);
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
	float thisTpeak       = thePulseHeightMap[thisStrip].tpk();

	prelimData = CSCStripData(thisStrip, thisHeight0, thisHeight, thisHeight2, thisTmax, thisTpeak);
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
  
//  std::vector<float> strip_adcs;

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
      float tpeak;
      pulseheightOnStripFinder_->peakAboveBaseline( (*it), height[1], tmax, tpeak, sigma);    // time bin for maximum
      pulseheightOnStripFinder_->signalAboveBaseline( (*it), height[0], tmax-1);              // previous time bin
      pulseheightOnStripFinder_->signalAboveBaseline( (*it), height[2], tmax+1);              // next time bin


      // Don't forget that the ME_11/a strips are ganged !!!
      // Have to loop 2 more times to populate strips 17-48.
    
      if ( id_.station() == 1 && id_.ring() == 4 ) {
        for ( int j = 0; j < 3; j++ ) {
          thePulseHeightMap[thisChannel+16*j] = CSCStripData( float(thisChannel+16*j), height[0], height[1], height[2], tmax, tpeak);
        }
      } else {
        thePulseHeightMap[thisChannel] = CSCStripData( float(thisChannel), height[0], height[1], height[2], tmax, tpeak);
      }

    }
  }
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
      std::cout << " you have chosen not to use 3 time bins --> Gatti will fail " << std::endl;
    }
  } else {  
    //    This uses 3 time bins
    for ( unsigned i = 0; i != data.size(); i++ ) {
      float w0 = data[i].y0();
      float w = data[i].y();
      float w2 = data[i].y2();

      // Fill the adcs to the strip hit --> needed for Gatti fitter
      strips_adc.push_back( w0 );
      strips_adc.push_back(  w );
      strips_adc.push_back( w2 );
 
      if (w0 < 0.) w0 = 0.;
      if (w  < 0.)  w = 0.;
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
 * Simply find time bin when maximum hit occurs
 * Also want to eventually find what this corresponds to for t_peak
 */
int CSCHitFromStripOnly::findTmaxofCluster( const std::vector<CSCStripData>& data ) {
  
  int tmax = -1;
  int nstrips = data.size();
  if ( nstrips == 0 ) return tmax;
  
  tmax = data[0].t();
  return tmax;
}


