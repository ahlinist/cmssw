#include <RecoLocalMuon/CSCRecHit/src/CSCRecHit2DFromWiresOnly.h>
#include <Geometry/CSCGeometry/interface/CSCLayer.h>
#include <Geometry/CSCGeometry/interface/CSCLayerGeometry.h>
#include <DataFormats/CSCRecHit/interface/CSCRecHit2D.h>
#include <DataFormats/CSCDigi/interface/CSCWireDigi.h>
#include <FWCore/MessageLogger/interface/MessageLogger.h>
#include <cmath>

std::vector<CSCRecHit2D> CSCRecHit2DFromWiresOnly::run( 
  const CSCDetId& id, const CSCLayer* layer,
  const CSCStripDigiCollection::Range& rstripd,
  const CSCWireDigiCollection::Range& rwired ) const {

  std::vector<CSCRecHit2D> hitsInLayer;

  const CSCLayerGeometry * lg = layer->geometry();

  for ( CSCWireDigiCollection::const_iterator it = rwired.first; it != rwired.second; ++it ) {
    CSCWireDigi wdigi = *it;

    int channel = wdigi.getWireGroup();
    LocalPoint lp = lg->localCenterOfWireGroup(channel);
    float lengthOfWireGroup = lg->lengthOfWireGroup(channel);
    float wireAngle = lg->wireAngle();
    float du = lengthOfWireGroup / sqrt(12.);
    float dv = lg->yResolution();
    float c = cos(wireAngle);
    float s = sin(wireAngle);
    float dx2 = du*du*c*c + dv*dv*s*s;
    float dy2 = du*du*s*s + dv*dv*c*c;
    float dxdy = c*s*(du*du - dv*dv);
    LocalError error(dx2, dxdy, dy2);

    CSCRecHit2D rechit( id, lp, error, CSCRecHit2D::ChannelContainer(), 0., 0. );    
    
    hitsInLayer.push_back( rechit ); //@@ copy it
	
    LogDebug("CSC") << "ME1/A rechit "<< channel << " " << lp <<  " " 
	      << dx2 << " " << dy2 << " " << dxdy << "\n";
  }
  return hitsInLayer;
}

