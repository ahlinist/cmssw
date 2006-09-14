// This is CSCMake2DRecHit

#include <RecoLocalMuon/CSCRecHitB/src/CSCMake2DRecHit.h>
#include <RecoLocalMuon/CSCRecHitB/src/CSCFitXonStripWithGatti.h>


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
    
  debug                      = ps.getUntrackedParameter<bool>("CSCDebug");
  stripWireDeltaTime         = ps.getUntrackedParameter<int>("CSCstripWireDeltaTime");
  useGatti                   = ps.getUntrackedParameter<bool>("CSC2UseGattiFit");

  xFitWithGatti_             = new CSCFitXonStripWithGatti( ps );
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
  

  // Find wire hit position and wire properties
  float wire_pos = wHit.wHitPos();                            // Position in terms of wire #
  int the_wire   = int(wire_pos);
  int w_group    = layergeom_->wireGroup(the_wire);
  int n_wires    = layergeom_->numberOfWiresPerGroup(w_group);   
  double w_space = specs_->wireSpacing();
  float w_angle  = layergeom_->wireAngle();
  float dy       = w_space * (n_wires - 1) / sqrt(12.);       // Error on y'  (on y if w_angle = 0)


  // Now, find strip position and properties
  int stripClusterSize = sHit.clusterSize();
  float strip_pos = sHit.sHitPos();
  int ch = int(strip_pos);
  float strip_offset = float(strip_pos) - ch;
  if ( strip_offset > 0.5 ) ch++;
  int centerStrip = ch;
  double s_angle = layergeom_->stripAngle(ch);



  // If at the edge, then used 1 strip cluster only :
  if ( ch <= 1 || ch >= specs_->nStrips() ) {

    channels.push_back(centerStrip);
    LocalPoint lp1 = layergeom_->stripWireIntersection( centerStrip, wire_pos);
    float x = lp1.x();
    float y = lp1.y();

    // Ensure that y position is within active area (ME_11 chambers):
    // If not, use upper (ME_1b)/lower (ME_1a) edge
    y = keepHitInFiducial( y );
    LocalPoint lp0(x, y);

    sigma =  layergeom_->stripPitch(lp0)/sqrt(12);  
    double dx = sigma/sin(s_angle);
    
    float sinangdif = sin(s_angle - w_angle);
    float sin2angdif = sinangdif * sinangdif;   
    
    float wsins = dy * sin(s_angle);
    float wcoss = dy * cos(s_angle);
    float ssinw = dx * sin(w_angle);
    float scosw = dx * cos(w_angle);
    
    float dx2 = (scosw*scosw + wcoss*wcoss)/sin2angdif;
    float dy2 = (ssinw*ssinw + wsins*wsins)/sin2angdif;
    float dxy = (scosw*ssinw + wcoss*wsins)/sin2angdif;
    
    LocalError localerr(dx2, dxy, dy2);
    
    CSCRecHit2D rechit( id, lp0, localerr, channels, chisq, prob );
    if (debug) std::cout << "Found rechit in layer " << this_layer << " with local position:  x = " << x << "  y = " << y << std::endl; 
    return rechit;  
  }


  // If not at the edge, used stripClusterSize strip cluster:

  // Store channels used
  for ( int i = (centerStrip-stripClusterSize/2); i <= (centerStrip+stripClusterSize/2); i++ ) {
    channels.push_back( i );
  }

  int ch0 = int(strip_pos);
  LocalPoint lp1 = layergeom_->stripWireIntersection( ch0,   wire_pos);
  LocalPoint lp2 = layergeom_->stripWireIntersection( ch0+1, wire_pos);
    
  // Use center of gravity to determine local x and y position:
  float x1 = lp1.x();
  float x2 = lp2.x();
  float x  = (1. - strip_offset) * x1 + strip_offset * x2;
  float y1 = lp1.y();
  float y2 = lp2.y();
  float y  = (1. - strip_offset) * y1 + strip_offset * y2;

  if (debug) std::cout <<  "Output from simple centroid:" << std::endl;
  if (debug) std::cout <<  "x = " << x << std::endl;


  // Build local point
  LocalPoint lp3(x, y);  
  float stripWidth = fabs(x2 - x1);   // layergeom_->stripPitch(lp3);
  sigma =  stripWidth/sqrt(12);              


  // Here try to improve the strip position by applying Gatti fitter

  if ( useGatti ) {   
    LocalPoint lp4 = layergeom_->stripWireIntersection( centerStrip, the_wire);
    float x_to_gatti = lp4.x();   // Position at center of strip
    float x_fit;
//    xFitWithGatti_->initChamberSpecs( *specs_ );
//    xFitWithGatti_->findXOnStrip( sHit, x_to_gatti, stripWidth, x_fit, sigma, chisq, prob );
    xFitWithGatti_->findXOnStrip( layer_, sHit, x_to_gatti, stripWidth, x_fit, sigma, chisq, prob );
    if (debug) std::cout << "x, x_to_gatti, x_fit, and diff " << x << " " << x_to_gatti  << " " << x_fit << " " << x-x_fit << std::endl;
    x = x_fit;
  }

  // Ensure that y position is within active area (ME_11 chambers):
  // If not, use upper (ME_1b)/lower (ME_1a) edge              
  y = keepHitInFiducial( y );
  LocalPoint lp0(x, y);

  // Now compute the errors properly on local x and y

  double dx = sigma / sin(s_angle);         
  
  float sinangdif = sin(s_angle - w_angle);
  float sin2angdif = sinangdif * sinangdif;
  
  float wsins = dy * sin(s_angle); 
  float wcoss = dy * cos(s_angle);
  float ssinw = dx * sin(w_angle);
  float scosw = dx * cos(w_angle);
  
  float dx2 = (scosw*scosw + wcoss*wcoss)/sin2angdif;
  float dy2 = (ssinw*ssinw + wsins*wsins)/sin2angdif;
  float dxy = (scosw*ssinw + wcoss*wsins)/sin2angdif;
  
  LocalError localerr(dx2, dxy, dy2);
    
  CSCRecHit2D rechit( id, lp0, localerr, channels, chisq, prob );
  
  if (debug) std::cout << "Found rechit in layer " << this_layer << " with local position:  x = " << x << "  y = " << y << std::endl; 
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


float CSCMake2DRecHit::keepHitInFiducial( float& y ) {

  const float marginAtEdge = 0.1;  // Allow extra margin for future tuning etc. For now 0.1 cm.
  float y2 = y;

  float apothem = layergeom_->length()/2.;

  if ( fabs(y) > (apothem+marginAtEdge) ) {
    if ( y < 0. ) {
      y2 = -1. * (apothem+marginAtEdge);
    } else {
      y2 =  (apothem+marginAtEdge);
    }
  } 
  return y2;
}

