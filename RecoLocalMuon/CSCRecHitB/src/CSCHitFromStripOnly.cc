// This is  CSCHitFromStripOnly.cc

#include <RecoLocalMuon/CSCRecHitB/src/CSCHitFromStripOnly.h>
#include <RecoLocalMuon/CSCRecHitB/src/CSCPeakBinOfStripPulse.h>
#include <RecoLocalMuon/CSCRecHitB/src/CSCStripGain.h>

#include <RecoLocalMuon/CSCRecHitB/src/CSCStripData.h>

#include <DataFormats/MuonDetId/interface/CSCDetId.h>
#include <DataFormats/CSCDigi/interface/CSCStripDigi.h>
#include <DataFormats/CSCRecHit/interface/CSCStripHit.h>
#include <DataFormats/CSCRecHit/interface/CSCStripHitCollection.h>

#include <Geometry/CSCGeometry/interface/CSCLayer.h>
#include <Geometry/CSCGeometry/interface/CSCChamberSpecs.h>
#include <Geometry/CSCGeometry/interface/CSCLayerGeometry.h>

#include "CondFormats/CSCObjects/interface/CSCGains.h"
#include "CondFormats/DataRecord/interface/CSCGainsRcd.h"
//#include "CondFormats/CSCObjects/interface/CSCcrosstalk.h"
//#include "CondFormats/DataRecord/interface/CSCcrosstalkRcd.h"
//#include "CondFormats/CSCObjects/interface/CSCNoiseMatrix.h"
//#include "CondFormats/DataRecord/interface/CSCNoiseMatrixRcd.h"

#include <FWCore/MessageLogger/interface/MessageLogger.h>
#include <FWCore/Utilities/interface/Exception.h>

#include <cmath>
#include <string>
#include <vector>
#include <iostream>


CSCHitFromStripOnly::CSCHitFromStripOnly( const edm::ParameterSet& ps ) {
  
  debug                      = ps.getUntrackedParameter<bool>("CSCDebug");
  isData                     = ps.getUntrackedParameter<bool>("CSCIsRunningOnData");
  theClusterSize             = ps.getUntrackedParameter<int>("CSCStripClusterSize");
  theThresholdForAPeak       = ps.getUntrackedParameter<double>("CSCStripPeakThreshold");
  theThresholdForCluster     = ps.getUntrackedParameter<double>("CSCStripClusterChargeCut");

  pulseheightOnStripFinder_  = new CSCPeakBinOfStripPulse( ps );
  stripGain_                 = new CSCStripGain( ps );
  
}


CSCHitFromStripOnly::~CSCHitFromStripOnly() {
  delete pulseheightOnStripFinder_;
  delete stripGain_;
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
  Nstrips = specs_->nStrips();

  TmaxOfCluster = 5;

  // Get gains for cluster and compute correction weights:

  // Fill all the gain weights at once, so save calling this function many times.
  // N.B. in database, strip_id starts at 0, whereas it starts at 1 in detId
  // Initialize weights to 1. and crosstalk to 0., in case database isn't populated or using MC

  if (isData) {
    stripGain_->setCalibration( gains_ );
    float globalGainAvg = stripGain_->getStripGainAvg();
    stripGain_->getStripGain( id_, globalGainAvg, gainWeight );
  } else {
    for ( int i = 0; i < 100; i++) gainWeight[i] = 1.;     
  }
  
  // Fill adc map and find maxima (potential hits)
  fillPulseHeights( rstripd );
  findMaxima();      

  // Make a Strip Hit out of each strip local maximum
  for ( unsigned imax = 0; imax < theMaxima.size(); imax++ ) {

    // Initialize parameters entering the CSCStripHit
    ClusterSize = theClusterSize;
    strips_adc.clear();

    // This is where centroid position is determined
    // The strips_adc vector is also filled here
    // Remember, the array starts at 0, but the stripId starts at 1...
    float strippos = makeCluster( theMaxima[imax]+1 );    
    
    if ( strippos < 0 || TmaxOfCluster < 3 ) continue;
    
    CSCStripHit striphit( id, strippos, TmaxOfCluster, ClusterSize, strips_adc );
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
    
  // If the cluster size is such that you go beyond the edge of detector, shrink cluster appropriatly
  for ( int i = theClusterSize/2; i > 0; i--) {
    if ( centerStrip - i < 1 || centerStrip + i > specs_->nStrips() ) {
      // Shrink cluster size, but keep it an odd number of strips.
      ClusterSize = 2*i - 1;  
    } else {
      break;
    }
  }

  for ( int i = -ClusterSize/2; i <= ClusterSize/2; i++ ) {
    CSCStripData data = makeStripData(centerStrip, i);
    stripDataV.push_back( data );
  }
  
  strippos = findHitOnStripPosition( stripDataV, centerStrip );
  
  return strippos;
}


/* makeStripData
 *
 * Gain corrections are applied here
 */
CSCStripData CSCHitFromStripOnly::makeStripData(int centerStrip, int offset) {
  
  CSCStripData prelimData(-1.,0.,0.,0.,0.,0);
  int thisStrip = centerStrip+offset;

  float adc[8];
  adc[0] = thePulseHeightMap[thisStrip-1].y0();
  adc[1] = thePulseHeightMap[thisStrip-1].y();
  adc[2] = thePulseHeightMap[thisStrip-1].y2();
  adc[3] = thePulseHeightMap[thisStrip-1].y3();

  TmaxOfCluster = thePulseHeightMap[centerStrip-1].t();
  if ( debug ) std::cout << "tmax is : " << TmaxOfCluster << std::endl;  
  
  if ( offset == 0 ) {
    prelimData = CSCStripData(thisStrip, adc[0], adc[1], adc[2], adc[3], TmaxOfCluster);
  } else {
    int sign = offset>0 ? 1 : -1;
    
    // If there's another maximum that would like to use part of this cluster, 
    // it gets shared in proportion to the height of the maxima
    for ( int i = 1; i <= ClusterSize/2; i++ ) {

      // Find the direction of the offset
      int testStrip = thisStrip + sign*i;

      std::vector<int>::iterator otherMax = find(theMaxima.begin(), theMaxima.end(), testStrip);

      // No other maxima found, so just store
      if ( otherMax == theMaxima.end() ) {
        prelimData = CSCStripData(thisStrip, adc[0], adc[1], adc[2], adc[3], TmaxOfCluster);

      } else {
        adc[4] = thePulseHeightMap[testStrip-1].y0();
	adc[5] = thePulseHeightMap[testStrip-1].y();
	adc[6] = thePulseHeightMap[testStrip-1].y2();
	adc[7] = thePulseHeightMap[testStrip-1].y3();
        float ratio[4]; 
        for (int k = 0; k < 4; k++) ratio[k] = adc[0] / (adc[0]+adc[k+4]);
        for (int k = 0; k < 4; k++) adc[k]   = adc[k] * ratio[k];

	prelimData = CSCStripData(thisStrip, adc[0], adc[1], adc[2], adc[3], TmaxOfCluster);
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
  
  for ( CSCStripDigiCollection::const_iterator it = rstripd.first; it != rstripd.second; ++it ) {
    bool fill = true;
    int thisChannel = (*it).getStrip(); 
    std::vector<int> sca = (*it).getADCCounts();

    float height[4];
    int tmax = -1;
    
    fill = pulseheightOnStripFinder_->peakAboveBaseline( (*it), height, tmax );

    // Don't forget that the ME_11/a strips are ganged !!!
    // Have to loop 2 more times to populate strips 17-48.
    
    if ( id_.station() == 1 && id_.ring() == 4 ) {
      for ( int j = 0; j < 3; j++ ) {
        thePulseHeightMap[thisChannel+16*j-1] = CSCStripData( float(thisChannel+16*j), height[0], height[1], height[2], height[3], tmax);
        thePulseHeightMap[thisChannel+16*j-1] *= gainWeight[thisChannel-1];
      }
    } else {
      thePulseHeightMap[thisChannel-1] = CSCStripData( float(thisChannel), height[0], height[1], height[2], height[3], tmax);
      thePulseHeightMap[thisChannel-1] *= gainWeight[thisChannel-1];
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
  for ( size_t i = 0; i < thePulseHeightMap.size(); i++ ) {

    float heightPeak    = thePulseHeightMap[i].y();
    // sum 3 strips so that hits between strips are not suppressed
    float heightCluster;
    if ( i == 0 ) {
      heightCluster = thePulseHeightMap[i].y()+thePulseHeightMap[i+1].y();
    } else if ( i == thePulseHeightMap.size()-1) {  
      heightCluster = thePulseHeightMap[i-1].y()+thePulseHeightMap[i].y();
    } else {
      heightCluster = thePulseHeightMap[i-1].y()+thePulseHeightMap[i].y()+thePulseHeightMap[i+1].y();
    }
   
    // Have found a strip Hit if...
    if (( heightPeak               > theThresholdForAPeak       ) &&  
        ( heightCluster            > theThresholdForCluster     ) && 
        ( thePulseHeightMap[i].t() > 2                          ) && 
        ( thePulseHeightMap[i].y() > thePulseHeightMap[i-1].y() ) &&
	( thePulseHeightMap[i].y() >= thePulseHeightMap[i+1].y())) {
      theMaxima.push_back(i);
    }
  }
}



/* findHitOnStripPosition
 *
 */
float CSCHitFromStripOnly::findHitOnStripPosition( const std::vector<CSCStripData>& data, const int& centerStrip ) {
  
  float strippos = -1.;
  
  int nstrips = data.size();
  if ( nstrips == 0 ) return strippos;
  
  // biggestStrip is strip with largest pulse height --> use pointer subtraction
  int biggestStrip = max_element(data.begin(), data.end()) - data.begin();
  
  // If more than one strip:  use centroid to find center of cluster
  // but only use time bin == tmax (otherwise, bias centroid).
  float sum  = 0.;
  float sum_w= 0.;
  
  for ( unsigned i = 0; i != data.size(); i++ ) {
    float w0 = data[i].y0();
    float w  = data[i].y();
    float w2 = data[i].y2();

    if (w0 < 0.) w0 = 0.;
    if (w  < 0.) w  = 0.;
    if (w2 < 0.) w2 = 0.;

     // Fill the adcs to the strip hit --> needed for Gatti fitter
    strips_adc.push_back( w0 );
    strips_adc.push_back( w );
    strips_adc.push_back( w2 );
 
    sum_w += w;
    sum   += w * data[i].x();
  }

  if ( sum_w > 0. ) {
    strippos = sum / sum_w;
  } else {
    strippos = data[biggestStrip].x();
  } 

  return strippos;
}


/*  Moved x-Talk stuff into Gatti fitter for now !!!
 *
 * correctForCrosstalk
 *
 *
 * void CSCHitFromStripOnly::correctForCrosstalk( const CSCStripDigiCollection::Range& rstripd, const unsigned& theChannel ) {
 *
 * if ( thePulseHeightMap[theChannel].y() < 5. ) return;
 *
 * int theTmax = thePulseHeightMap[theChannel].t();
 *
 * // find the digis corresponding to this strip and nearest neighbors
 * for ( CSCStripDigiCollection::const_iterator it = rstripd.first; it != rstripd.second; ++it ) {
 *   CSCStripDigi digi = *it;
 *   unsigned int jstrip = digi.getStrip();
 *   std::vector<int> sca = digi.getADCCounts();
 *
 *   float pedestal = (sca[0] + sca[1]) /2.;
 *
 *   float xtalks[4];
 *    
 *   // Dealing with MC first     
 *   if ( !isData ) {
 *
 *     // add back what leaked out to adjacent strips
 *     if ( jstrip == theChannel ) {
 *       for ( int t = 0; t < 4; t++ ) xtalks[t] = 2.*crosstalkLevel( digi, theTmax + t - 1);
 *       thePulseHeightMap[theChannel] += xtalks;
 *
 *     // subtract off what came from that neighbor
 *     } else if ( abs(jstrip - theChannel) == 1 ) {
 *       for ( int t = 0; t < 4; t++ ) xtalks[t] = -1.*crosstalkLevel( digi, theTmax + t - 1);
 *       thePulseHeightMap[theChannel] += xtalks;
 *     }
 *   }
 *
 *   // Now, dealing with data
 *   if ( isData ) {
 *
 *     // add back what leaked out to adjacent strips
 *     if ( jstrip == theChannel ) {
 *       for ( int t = 0; t < 4; t++ ) {
 *         int tbin = theTmax + t - 1;
 *         if ( theChannel == 1 ) {
 *           xtalks[t] = (sca[tbin]-pedestal) 
 *                     * ( slopeRight[theChannel-1] * 50. * (t-1) + interRight[theChannel-1]);
 *         } else if ( theChannel ==  Nstrips ) {
 *           xtalks[t] = (sca[tbin]-pedestal)
 *                     * ( slopeLeft[theChannel-1]  * 50. * (t-1) + interLeft[theChannel-1] );
 *         } else {
 *           xtalks[t] = (sca[tbin]-pedestal)  
 *                     * ( slopeRight[theChannel-1] * 50. * (t-1) + interRight[theChannel-1]
 *                       + slopeLeft[theChannel-1]  * 50. * (t-1) + interLeft[theChannel-1] );
 *         }
 *       }
 *       thePulseHeightMap[theChannel] += xtalks;
 *       
 *     // subtract off what came from the neighbor  --> look for strip j on Left of strip i
 *     } else if ( int(theChannel - jstrip) == 1 ) {
 *       for ( int t = 0; t < 4; t++ ) {
 *         int tbin = theTmax + t - 1;
 *         xtalks[t] = -(sca[tbin]-pedestal)* 
 *                   ( slopeLeft[theChannel-1]  * 50. * (t-1) + interLeft[theChannel-1] );
 *       } 
 *       thePulseHeightMap[theChannel] += xtalks;
 *   
 *     // subtract off what came from the neighbor  --> look for strip j on Right of strip i
 *     } else if ( int(theChannel - jstrip) == -1 ) {
 *       for ( int t = 0; t < 4; t++ ) {
 *         int tbin = theTmax + t - 1;
 *         xtalks[t] = -(sca[tbin]-pedestal)* 
 *                   ( slopeRight[theChannel-1] * 50. * (t-1) + interRight[theChannel-1]);
 *       }
 *       thePulseHeightMap[theChannel] += xtalks;
 *     } 
 *   }
 *  }
 *}
 *
 *
 * crosstalkLevel
 *
 * This is how crosstalk was implemented in MC (digi)
 *
 * float CSCHitFromStripOnly::crosstalkLevel( const CSCStripDigi& digi, const int& tbin ) {
 * // Don't even bother for small signals
 * std::vector<int> sca = digi.getADCCounts();
 *
 * float pedestal = (sca[0] + sca[1]) /2.;
 *
 * if ( sca[tbin]-pedestal < 5.) return 0.; 
 *
 * // Make crosstalk proportional to the slope in the digi
 * // our time-variable will be the ratio of the SCA values.
 * float slope =  sca[tbin]/( sca[tbin-1]+0.1 ) - 1.;
 * if( slope < 0. ) return 0.;
 * if( slope > 2.5 ) slope = 2.5;
 * 
 * // Correction factor is 0 for ratio of 1, and 2.6% for ratio of 2...
 * //@@ Need theCrossTalkLevel to be configurable
 * float theCrosstalkLevel = 0.026; 
 * 
 * // Smaller chambers get half the crosstalk
 * //@@ What happened to ME1a ??
 * if(id_.station() == 1 || id_.ring() == 1) {
 *   theCrosstalkLevel /= 2.;
 * }
 *
 * return theCrosstalkLevel * (sca[tbin]-pedestal) * slope;
 * }
 *
 */
