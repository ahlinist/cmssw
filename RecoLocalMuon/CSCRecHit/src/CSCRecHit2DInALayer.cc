// This is  CSCRecHit2DInALayer.cc

#include <RecoLocalMuon/CSCRecHit/src/CSCRecHit2DAlgo.h>
#include <DataFormats/MuonDetId/interface/CSCDetId.h>
#include <DataFormats/CSCDigi/interface/CSCStripDigi.h>
#include <DataFormats/CSCDigi/interface/CSCWireDigi.h>
#include <DataFormats/CSCRecHit/interface/CSCRecHit2D.h>
#include <DataFormats/CSCRecHit/interface/CSCRecHit2DCollection.h>

#include <RecoLocalMuon/CSCRecHit/src/CSCRecHit2DInALayer.h>
#include <RecoLocalMuon/CSCRecHit/src/CSCFindPositionOfStripCluster.h>
#include <RecoLocalMuon/CSCRecHit/src/CSCFitData.h>
#include <RecoLocalMuon/CSCRecHit/src/CSCFixedBinOfSCAPulse.h>
#include <RecoLocalMuon/CSCRecHit/src/CSCFitSCAPulse.h>
#include <RecoLocalMuon/CSCRecHit/src/CSCPeakBinOfSCAPulse.h>
#include <RecoLocalMuon/CSCRecHit/src/CSCTotalSCACounts.h>
#include <RecoLocalMuon/CSCRecHit/src/CSCStripClusterFitter.h>
#include <RecoLocalMuon/CSCRecHit/src/CSCStripClusterCentroid.h>
#include <RecoLocalMuon/CSCRecHit/src/CSCHalfStripClusterer.h>

#include <Geometry/CSCGeometry/interface/CSCLayer.h>
#include <Geometry/CSCGeometry/interface/CSCChamberSpecs.h>
#include <Geometry/CSCGeometry/interface/CSCLayerGeometry.h>

#include <FWCore/MessageLogger/interface/MessageLogger.h>
#include <FWCore/Utilities/interface/Exception.h>

#include <algorithm>
#include <cmath>
#include <string>


CSCRecHit2DInALayer::CSCRecHit2DInALayer(  const edm::ParameterSet& ps ) : CSCRecHit2DAlgo( ps ),
    stripClusterPositionFinder_( 0 ),
    infoV( false ), myName( "CSCRecHit2DInALayer" ), pulseheightOnStripFinder_( 0 ) {

  infoV               = ps.getUntrackedParameter<bool>("verboseInfo");

  theClusterSize      = ps.getParameter<int>("clusterSize");
  theClusterChargeCut = static_cast<float>( ps.getParameter<double>("clusterChargeCut") );
  theClusterProbCut   = static_cast<float>( ps.getParameter<double>("clusterProbCut") );

  theThresholdForAPeak = static_cast<float>( ps.getParameter<double>("thresholdForAPeak") );

  // require that the strip and wire digi BX match
  theBunchMatching = ps.getParameter<int>("recHitBunchMatching");
  useAverageErrors = ps.getParameter<bool>("useAverageErrors");

//  doCrosstalkCorrection = ps.getParameter<bool>("doCrossTalkCorrection");
//  theCrosstalkLevel     = ps.getParameter<float>("crossTalkCorrection");

  // Which algorithms to use?

  std::string pulseheightOnStripFinder = ps.getParameter<std::string>("pulseheightOnStripFinder");

  if ( pulseheightOnStripFinder=="CSCFixedBinOfSCAPulse" ) {
    pulseheightOnStripFinder_ = new CSCFixedBinOfSCAPulse();
  }
  else if ( pulseheightOnStripFinder=="CSCPeakBinOfSCAPulse" ) {
    pulseheightOnStripFinder_ = new CSCPeakBinOfSCAPulse();
  }
  else if ( pulseheightOnStripFinder=="CSCFitSCAPulse" ) {
    pulseheightOnStripFinder_ = new CSCFitSCAPulse();
  }
  else if ( pulseheightOnStripFinder=="CSCTotalSCACounts" ) {
    pulseheightOnStripFinder_ = new CSCTotalSCACounts();
  }
  else {
    throw cms::Exception("BadSCAPeakFinder") << 
      "Unexpected value = " << pulseheightOnStripFinder << "\n";
  }

  std::string stripClusterPositionFinder = ps.getParameter<std::string>("stripClusterPositionFinder");

  if ( stripClusterPositionFinder=="CSCStripClusterFitter" ) {
    stripClusterPositionFinder_ = new CSCStripClusterFitter();
  }
  else if ( stripClusterPositionFinder=="CSCStripClusterCentroid" ) {
    stripClusterPositionFinder_ = new CSCStripClusterCentroid();
  }
  else if ( stripClusterPositionFinder=="CSCHalfStripClusterer" ) {
    stripClusterPositionFinder_ = new CSCHalfStripClusterer();
  }
  else {
    throw cms::Exception("BadClusterer") << 
      "Unexpected value = " << stripClusterPositionFinder << "\n";
  }
}

CSCRecHit2DInALayer::~CSCRecHit2DInALayer() {
  delete pulseheightOnStripFinder_;
  delete stripClusterPositionFinder_;
}

std::vector<CSCRecHit2D> CSCRecHit2DInALayer::run( 
          const CSCDetId& id, const CSCLayer* layer,
          const CSCStripDigiCollection::Range& rstripd,
          const CSCWireDigiCollection::Range& rwired ) {
	
  std::vector<CSCRecHit2D> hitsInLayer;

  // cache layer info for ease of access
  id_ = id;
  layer_ = layer;
  layergeom_ = layer_->geometry();

  // find clusters of wire groups and skip if we find none
  std::vector<CSCWireCluster>  wireClusters  = findWireClusters( rwired );
  if (wireClusters.empty()) return hitsInLayer;

  // cache CSCChamberSpecs so the stripClusterPositionFinder can access type-dependent parameters
  //  specs_ = layer_->getSpecs();
  //  specs_ = &( dynamic_cast<const CSCChamberSpecs&>(layer->type()) );
  specs_ = layer->chamber()->specs();
  stripClusterPositionFinder_->initChamberSpecs(*specs_);

  // find clusters of strips
  fillPulseHeights( rstripd );
  findMaxima();    

  // Make a RecHit out of each overlap of a wiregroup and a strip maximum
  for( std::vector<CSCWireCluster>::iterator wireClusterItr = wireClusters.begin();
      wireClusterItr != wireClusters.end(); ++wireClusterItr) {
    for(size_t imax = 0; imax < theMaxima.size(); ++imax) {
      // We no longer have a beam crossing tag in strip digis so cannot make a match!

      // make sure they match bunch crossings
      //@@ also make sure they actually cross in ME1/1!
      //      int wireBX = wireClusterItr->getBeamCrossingTag();
      //      int stripBX = thePulseHeightMap[theMaxima[imax]].bx();
      //      if(abs(wireBX-stripBX) <= theBunchMatching) {
        CSCRecHit2D rechit = makeCluster(*wireClusterItr, theMaxima[imax]);
        
        if ( isInFiducial(rechit) ) hitsInLayer.push_back( rechit ); //@@ copy it 
      //      }
    }
  }
  return hitsInLayer;
}


std::vector<CSCWireCluster> CSCRecHit2DInALayer::findWireClusters( const CSCWireDigiCollection::Range& rwired ) {
  std::vector<CSCWireCluster> wcls;
  
    // start with a dummy
    CSCWireCluster clus(-100,-100,-100,10.);
    bool any_digis = false;
    for ( CSCWireDigiCollection::const_iterator it = rwired.first; it != rwired.second; ++it ) {
      if ( !any_digis ) {
        any_digis = true;
        clus = CSCWireCluster(*it, layergeom_);
      } 
      else {
        if( !clus.add(*it, layergeom_) ) {
          // save the old one and start a new one
          wcls.push_back(clus);
          clus = CSCWireCluster(*it, layergeom_);
        }
      }
    } // end loop over digis
    if ( any_digis ) wcls.push_back(clus);

  return wcls;
}


CSCRecHit2D CSCRecHit2DInALayer::makeCluster(
        const CSCWireCluster & wireCluster, int centerStrip) {
  // fit for the local precise coordinate
  // fit an rhit for every wire group that was hit
  float centerWire = wireCluster.centerWire();

  double u, sigma, chisq, prob;
  CSCRecHit2D::ChannelContainer channels;

  if( centerStrip == 1 || centerStrip == specs_->nStrips() ) {
    // edge strip, so just give it full-strip resolution
    // maybe can improve to half-strip by looking at neighbor
    LocalPoint lp = layergeom_->stripWireIntersection(centerStrip, centerWire);
    channels.push_back(centerStrip);
    u = lp.x();
    sigma =  layergeom_->stripPitch(lp)/sqrt(12.);
    chisq = 0.;
    prob = 1.;
 
  } else {
    // fill the x, y, and dy to be sent to fitter
    std::vector<CSCFitData> fitDataV;
    // this routine will share strips if they're both going into
    // a cluster
    LogDebug("CSC") << "check channels centred on " << centerStrip << "\n";
    for(int i = -theClusterSize/2; i <= theClusterSize/2; ++i) {
      LogDebug("CSC") << "try channel " << i << " in fit" << "\n";
      if(centerStrip+i > 0 && centerStrip+i <= specs_->nStrips()) { 
        LogDebug("CSC") << "use channel " << i << " in fit" << "\n";
        CSCFitData data = makeFitData(centerStrip, centerWire, i);

        fitDataV.push_back( data );
        channels.push_back(centerStrip+i);
      }
    }

    stripClusterPositionFinder_->findPosition(fitDataV, u, sigma, chisq, prob);
  }

  float v = layergeom_->yOfWire(centerWire, u);
    LocalPoint localPoint(u, v); 

  float strangle = layergeom_->stripAngle(centerStrip);
  float du = sigma/sin(strangle); // since sigma is in local x units

  if(useAverageErrors) {
    int stat = id_.station();
    int ring = id_.ring();
    if ( stat == 1 ) {
      if (ring == 2 || ring==3 ) {  
        du = 0.0199;
      }
      else {
        du = 0.0125;
      }
    }
    else {
      if(ring == 1) {
        du = 0.0234;
      }
      else {
        du = 0.0216;
      }
    }
  }
  
  // Now calculate approximate x and y resolutions, and xy correlation,
  // from strip measurement resolution (above) and wire measurement resolution

  //	  du = du * sin(layerGeometry->stripAngle(centerStrip)); 
  //      float dv = layerGeometry->yResolution(); 
  //      float duv = du * dv * sin(layerGeometry->wireAngle());

  //  float strangle = layergeom_->stripAngle(centerStrip);
  float wangle   = layergeom_->wireAngle();

  float dv = wireCluster.width()/sqrt(12.);

  float sinangdif = sin(strangle-wangle);
  float sin2angdif = sinangdif * sinangdif;
  
  // The notation is
  // wsins = wire resol  * sin(strip angle)
  // wcoss = wire resol  * cos(strip angle)
  // ssinw = strip resol * sin(wire angle)
  // scosw = strip resol * cos(wire angle)

  float wsins = dv * sin(strangle);
  float wcoss = dv * cos(strangle);
  float ssinw = du * sin(wangle);
  float scosw = du * cos(wangle);

  float dx2 = (scosw*scosw + wcoss*wcoss)/sin2angdif;
  float dy2 = (ssinw*ssinw + wsins*wsins)/sin2angdif;
  float dxy = (scosw*ssinw + wcoss*wsins)/sin2angdif;
          
  LocalError localError(dx2, dxy, dy2);

  CSCRecHit2D rechit( id_, localPoint, localError, channels, chisq, prob );

  if( infoV ) {
    edm::LogInfo("CSC") << "new CSCRecHit2D ME" <<
        id_.station() << "/" << id_.ring() << " C " << 
        id_.chamber() << " L " << id_.layer() << " local point: " << localPoint << "\n";
    edm::LogInfo("CSC") << " errors:" << 
         " sqrt(dx2)=" << sqrt(dx2) <<
         " sqrt(dy2)=" << sqrt(dy2) << " dxy=" << dxy << 
         " strip du=" << du << "\n";
  }
  return rechit;
}


CSCFitData 
CSCRecHit2DInALayer::makeFitData(int centerStrip, float centerWire, int offset) {
  CSCFitData prelimData(0.,0.,0.,0);
  int thisStrip = centerStrip+offset;
  if(offset == 0) {
    prelimData = thePulseHeightMap[centerStrip];
  } else {
    int sign = offset>0 ? 1 : -1;
    // if there's another maximum that would like to use this cluster,
    // it gets shared in proportion to the height of the maxima
    for(int i = 1; i <= theClusterSize/2; ++i) {
      // go in the direction of the offset
      int testStrip = thisStrip + sign*i;
      std::vector<int>::iterator otherMax = find(theMaxima.begin(), theMaxima.end(), testStrip);
      if(otherMax == theMaxima.end()) {
        prelimData = thePulseHeightMap[thisStrip];
      } else {
        // uh oh.  We have to share
        float heightCenter = thePulseHeightMap[centerStrip].y();
        float heightNeighbor = thePulseHeightMap[testStrip].y();
        float ratio = heightCenter / (heightNeighbor+heightCenter);
        float thisHeight = thePulseHeightMap[thisStrip].y();
        float dy = thePulseHeightMap[thisStrip].dy();
	//@@ Shouldn't dy get scaled by 'ratio' too?
        prelimData = CSCFitData(0., thisHeight * ratio, dy);
      }
    }
  }
  LocalPoint lp = layergeom_->stripWireIntersection(thisStrip, centerWire);
  return CSCFitData(lp.x(), prelimData.y(), prelimData.dy());
}


void CSCRecHit2DInALayer::fillPulseHeights( const CSCStripDigiCollection::Range& rstripd ) {
  
// Loop over strip digis and fill the pulseheight map

  thePulseHeightMap.clear();
  thePulseHeightMap.resize(100);
  
  //@@ But add hack to recuperate bad duplicate strip digis
  //@@ There's a magic number too (5.)

  int lastChannel = -1;
  int lastFirstSCA = -999;

  for ( CSCStripDigiCollection::const_iterator it = rstripd.first; it != rstripd.second; ++it ) {
    bool fill = true;
    int thisChannel = (*it).getStrip(); //@@ New strip digi has 'strip number' not channel
    std::vector<int> sca = (*it).getADCCounts();
    int thisFirstSCA = sca[0];
    if ( thisChannel == lastChannel ) {
      edm::LogWarning("CSC|BadNews") << myName << " WARNING! DIGI ON SAME CHANNEL." 
	   << " Please inform Muon developers.\n"
	   << "channel=" << thisChannel 
	   << ", sca(0) this=" << thisFirstSCA << ", last=" << lastFirstSCA  << "\n";
      
      if ( thisFirstSCA - lastFirstSCA > 5. ) {
        fill = false; // use last one: prevent overwrite of last with this one
      }
    }
    lastChannel = thisChannel;
    lastFirstSCA = thisFirstSCA;

    if ( fill ) {
      double height, sigma;
      pulseheightOnStripFinder_->peakAboveBaseline( (*it), *specs_, height, sigma);
      // We no longer have a beam crossing tag in strip digis
      //      int bx = (*it).getBeamCrossingTag();
      int bx = 0; //@@ just a dummy
      thePulseHeightMap[thisChannel] = CSCFitData( 0., height, sigma, bx );
    }
  }

//@@ Always do crosstalk correction
//  if(doCrosstalkCorrection) {
  correctForCrosstalk( rstripd );
//  }
}


void CSCRecHit2DInALayer::correctForCrosstalk( const CSCStripDigiCollection::Range& rstripd ) {
  // loop over the map, and do a correction for each entry
  for( size_t istrip = 1; istrip < thePulseHeightMap.size(); ++istrip ) {
    if( thePulseHeightMap[istrip].y() > 0 ) {
      // find the digis corresponding to this strip and nearest neighbors.
	  for ( CSCStripDigiCollection::const_iterator it = rstripd.first; it != rstripd.second; ++it ) {
        CSCStripDigi digi = *it;
        unsigned int jstrip = digi.getStrip();
        if( jstrip == istrip ) {
          // add back in what got removed
          thePulseHeightMap[istrip] += 2.*crosstalkLevel( digi );
        } 
        else if( abs(jstrip - istrip) == 1 ) {
          // subtract off what came from that neighbor
          thePulseHeightMap[istrip] += -1.*crosstalkLevel( digi );
        }
      }
    }
  }
}


float  CSCRecHit2DInALayer::crosstalkLevel(const CSCStripDigi & digi) {
  // Don't even bother for small signals - 
  //@@ check 5th bin, which is where peak is supposed to fall
  std::vector<int> sca = digi.getADCCounts();
  if( sca[4] < 10.) return 0.; 

  // Make crosstalk proportional to the slope in the digi
  // our time-variable will be the ratio of the SCA values.
  float slope =  sca[4]/( sca[3]+0.1 ) - 1.;
  if( slope < 0. ) return 0.;
  if( slope > 2.5 ) slope = 2.5;
  
  // Correction factor is 0 for ratio of 1, and 2.6% for ratio of 2...
  //@@ Need theCrossTalkLevel to be configurable
  float theCrosstalkLevel = 0.026; 
 
  // Smaller chambers get half the crosstalk
  //@@ What happened to ME1a ??
  if(id_.station() == 1 || id_.ring() == 1) {
    theCrosstalkLevel /= 2.;
  }

  return theCrosstalkLevel * sca[4] * slope;
}


void CSCRecHit2DInALayer::findMaxima() {
  // fills vector theMaxima with the local maxima in the pulseheight distribution
  // of the strips. The threshold defining a maximum is a configurable parameter.
  // A typical value is 30.

  theMaxima.clear();

  for(size_t i = 1; i < thePulseHeightMap.size(); ++i) {
    // sum 3 strips so that hits between strips are not suppressed
    float height = thePulseHeightMap[i-1].y() +
                   thePulseHeightMap[i].y() +
                   thePulseHeightMap[i+1].y();
    if( height > theThresholdForAPeak ) {
      // check neighbors of strip i  Don't worry about bounds;
      // should be OK at 0 and beyond last strip
      if( thePulseHeightMap[i].y() > thePulseHeightMap[i-1].y() &&
          thePulseHeightMap[i].y() >= thePulseHeightMap[i+1].y()) {
        theMaxima.push_back(i);
      }
    }
  }
}
    
bool CSCRecHit2DInALayer::isInFiducial( const CSCRecHit2D& rh ) {

  const float marginAtEdge = 0.1; // Allow extra margin for future tuning etc. For now 0.1 cm.

  float y = rh.localPosition().y();
  float apothem = layergeom_->length()/2.;
  return ( fabs(y) < (apothem+marginAtEdge) );
}



