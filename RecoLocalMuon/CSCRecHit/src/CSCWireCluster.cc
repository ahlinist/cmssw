#include <RecoLocalMuon/CSCRecHit/src/CSCWireCluster.h>
#include <DataFormats/CSCDigi/interface/CSCWireDigi.h>
#include <Geometry/CSCGeometry/interface/CSCLayerGeometry.h>
#include <FWCore/MessageLogger/interface/MessageLogger.h>

#include <iostream>

CSCWireCluster::CSCWireCluster(const CSCWireDigi & digi, const CSCLayerGeometry * geom) {
  int iwg = digi.getWireGroup();
  float middleWire = geom->middleWireOfGroup(iwg);
  int nwires = geom->numberOfWiresPerGroup(iwg);
  theFirstWire = (int) (middleWire - nwires/2. + 0.5);
  theLastWire  = (int) (middleWire + nwires/2.);
  theBeamCrossing = digi.getBeamCrossingTag();
  theWireSpacing = geom->wirePitch();
  theLastChannel = iwg;
  LogDebug("CSC") << "making wire cluster nwires=" << 
     nwires << ", first=" << theFirstWire << ", last=" << theLastWire <<  "\n";
}


bool CSCWireCluster::add(const CSCWireDigi & digi, const CSCLayerGeometry * geom) {
	
  //@@ Need to check wire cluster building makes sense with realistic wire geometry!
	
  bool value = false;
  int iwg = digi.getWireGroup();

  LogDebug("CSC") << "wirecluster add " <<
      iwg << " " << theLastChannel << " " << 
      digi.getBeamCrossingTag() << " " << theBeamCrossing <<  "\n";
  
  if( (iwg == theLastChannel+1)
     && (abs(digi.getBeamCrossingTag()-theBeamCrossing)<= 1) ) {
    value = true;
    theLastChannel = iwg;
    float middleWire = geom->middleWireOfGroup(iwg);
    int nwires = geom->numberOfWiresPerGroup(iwg);
    theLastWire  = (int) (middleWire + nwires/2.);
    LogDebug("CSC") << "adding digi to wirecluster " << 
        theFirstWire << " " << theLastWire <<  "\n";
  }
  return value;
}
