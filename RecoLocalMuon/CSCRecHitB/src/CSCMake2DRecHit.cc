// This is CSCMake2DRecHit

#include <RecoLocalMuon/CSCRecHitB/src/CSCMake2DRecHit.h>

#include <DataFormats/MuonDetId/interface/CSCDetId.h>
#include <DataFormats/CSCRecHit/interface/CSCStripHit.h>
#include <DataFormats/CSCRecHit/interface/CSCWireHit.h>
#include <DataFormats/CSCRecHit/interface/CSCRecHit2D.h>

#include <Geometry/CSCGeometry/interface/CSCLayer.h>
#include <Geometry/CSCGeometry/interface/CSCChamberSpecs.h>
#include <Geometry/CSCGeometry/interface/CSCLayerGeometry.h>

#include <Geometry/Vector/interface/LocalPoint.h>

#include <FWCore/MessageLogger/interface/MessageLogger.h>
#include <FWCore/Utilities/interface/Exception.h>

#include <iostream>
#include <algorithm>
#include <cmath>
#include <string>


CSCMake2DRecHit::CSCMake2DRecHit(const edm::ParameterSet& ps){
  
  
  Debug                      = ps.getUntrackedParameter<bool>("CSCDebug");
  stripWireDeltaTime         = ps.getUntrackedParameter<int>("CSCstripWireDeltaTime");
  
}   

/* hitFromStripAndWire
 *
 */
CSCRecHit2D CSCMake2DRecHit::hitFromStripAndWire(const CSCDetId& id, const CSCLayer* layer,
                                                 const CSCWireHit& wHit, const CSCStripHit& sHit){
  
  // Cache layer info for ease of access
  layer_ = layer;
  layergeom_ = layer_->geometry();
  specs_ = layer->chamber()->specs();
  int this_layer = id.layer();
  
  double sigma, chisq, prob;
  chisq = 0.00;
  prob = 1.00;
  
  CSCRecHit2D::ChannelContainer channels;
  
  // Find wire hit position in terms of wire #
  float wire_pos = wHit.wgrouppos();           // This is the position of the wire hit in terms of wire #
  int thewire = int(wire_pos);
  int wgroup = layergeom_->wireGroup(thewire);
  
  // Find strip position in terms of strip #
  // Channel 2, or ch2, is the central channel
  float strip_pos = sHit.halfStripPos();
  int ch2 = int(strip_pos);
  float offset = float(strip_pos) - ch2;
  if (offset > 0.5 ) ch2++;
  int ch1 = ch2 - 1;
  int ch3 = ch2 + 1;
  
  // If at the edge, then used 1 strip only :
  if ( ch2 <= 1 || ch2 >= specs_->nStrips() ) {
    channels.push_back(ch2);
    LocalPoint lp0 = layergeom_->stripWireIntersection( ch2, wire_pos);
    float x = lp0.x();
    float y = lp0.y();
    sigma =  layergeom_->stripPitch(lp0)/sqrt(12);  
    double strip_angle = layergeom_->stripAngle(ch2);
    double dx = sigma/sin(strip_angle);
    
    float wangle = layergeom_->wireAngle();
    float dy = layergeom_->yResolution(wgroup);
    
    float sinangdif = sin(strip_angle - wangle);
    float sin2angdif = sinangdif * sinangdif;   
    
    float wsins = dy * sin(strip_angle);
    float wcoss = dy * cos(strip_angle);
    float ssinw = dx * sin(wangle);
    float scosw = dx * cos(wangle);
    
    float dx2 = (scosw*scosw + wcoss*wcoss)/sin2angdif;
    float dy2 = (ssinw*ssinw + wsins*wsins)/sin2angdif;
    float dxy = (scosw*ssinw + wcoss*wsins)/sin2angdif;
    
    LocalError localerr(dx2, dxy, dy2);
    
    CSCRecHit2D rechit( id, lp0, localerr, channels, chisq, prob );
    if (Debug) std::cout << "Found rechit in layer " << this_layer << " with local position:  x = " << x << "  y = " << y << std::endl; 
    return rechit;
  }
  
  // If not at the edge, used 3 strip cluster:
  channels.push_back(ch1);
  channels.push_back(ch2);
  channels.push_back(ch3);
  
  int ch0 = int(strip_pos);
  int ch00 = ch0+1;
  
  LocalPoint lp1 = layergeom_->stripWireIntersection( ch0, wire_pos);
  LocalPoint lp2 = layergeom_->stripWireIntersection( ch00, wire_pos);
  
  float x, y, x1, y1, x2, y2;
  
  // Again, use center of gravity :
  x1 = lp1.x();
  x2 = lp2.x();
  x  = (1.-offset) * x1 + offset * x2;
  y1 = lp1.y();
  y2 = lp2.y();
  y  = (1.-offset) * y1 + offset * y2;
  
  LocalPoint lp0(x, y);
  
  sigma =  layergeom_->stripPitch(lp0)/sqrt(12);     // This is an overestimate 
  double strip_angle = layergeom_->stripAngle(ch2);
  double dx = sigma/sin(strip_angle);         
  
  float wangle = layergeom_->wireAngle();
  float dy = layergeom_->yResolution(wgroup);
  
  float sinangdif = sin(strip_angle - wangle);
  float sin2angdif = sinangdif * sinangdif;
  
  float wsins = dy * sin(strip_angle); 
  float wcoss = dy * cos(strip_angle);
  float ssinw = dx * sin(wangle);
  float scosw = dx * cos(wangle);
  
  float dx2 = (scosw*scosw + wcoss*wcoss)/sin2angdif;
  float dy2 = (ssinw*ssinw + wsins*wsins)/sin2angdif;
  float dxy = (scosw*ssinw + wcoss*wsins)/sin2angdif;
  
  LocalError localerr(dx2, dxy, dy2);
  
  chisq = 0.00;
  prob = 1.00;
  
  CSCRecHit2D rechit( id, lp0, localerr, channels, chisq, prob );
  
  if (Debug) std::cout << "Found rechit in layer " << this_layer << " with local position:  x = " << x << "  y = " << y << std::endl; 
  return rechit;
}


/* hitFromWireOnly
 *
 */
CSCRecHit2D CSCMake2DRecHit::hitFromWireOnly(const CSCDetId& id, const CSCLayer* layer, const CSCWireHit& w_Hit){
  CSCRecHit2D rechit;
  return rechit;
}


/* hitFromStripOnly
 *
 */
CSCRecHit2D CSCMake2DRecHit::hitFromStripOnly(const CSCDetId& id, const CSCLayer* layer, const CSCStripHit& s_Hit){
  CSCRecHit2D rechit;
  return rechit;
}

