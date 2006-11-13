#include <RecoLocalMuon/CSCRecHit/src/CSCRecHit2DFromORedStrips.h>
#include <RecoLocalMuon/CSCRecHit/src/CSCFitData.h>
#include <Geometry/CSCGeometry/interface/CSCLayer.h>

CSCRecHit2DFromORedStrips::CSCRecHit2DFromORedStrips( const edm::ParameterSet& ps ) 
  : CSCRecHit2DInALayer(ps), theStaggering( 16 ) {
  //@@ Must fill theStaggering from PS
}

void CSCRecHit2DFromORedStrips::fillPulseHeights( const CSCStripDigiCollection::Range& rstripd ) {
  CSCRecHit2DInALayer::fillPulseHeights( rstripd );
  int nstrips = layergeom_->numberOfStrips();
  // make a copy so I can add to the real one
  PulseHeightMap aPulseHeightMap = thePulseHeightMap;
  for(int i = 1; i <= theStaggering; ++i) {
    int strip = i + theStaggering;
    // add the "image" pulses
    while(strip <= nstrips) {
      thePulseHeightMap[strip] = aPulseHeightMap[i];
      strip += theStaggering;
    }
  }
}        
 
