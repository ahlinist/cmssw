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
  useGatti                   = ps.getUntrackedParameter<bool>("CSCUseGattiFit");

  xFitWithGatti_             = new CSCFitXonStripWithGatti( ps );
}   


/* hitFromStripAndWire
 *
 */
CSCRecHit2D CSCMake2DRecHit::hitFromStripAndWire(const CSCDetId& id, const CSCLayer* layer,
                                                 const CSCWireHit& wHit, const CSCStripHit& sHit){
  
  // Cache layer info for ease of access
  layer_         = layer;
  layergeom_     = layer_->geometry();
  specs_         = layer->chamber()->specs();
  int this_layer = id.layer();
  
  double sigma, chisq, prob;
  chisq = 0.00;
  prob  = 1.00;
  
  CSCRecHit2D::ChannelContainer channels;
  
  // Find wire hit position and wire properties
  float wireg = wHit.wHitPos();                            // Position in terms of wire group #
  int wireg1 = int( wireg );
  int wireg2 = wireg1;
  if ( (wireg - wireg1) != 0. ) {
    wireg2++;
  }  
  float wgoffset   = wireg - wireg1;
  float the_wire1  = layergeom_->middleWireOfGroup(wireg1);
  float the_wire2  = layergeom_->middleWireOfGroup(wireg2);
  int n_wires      = layergeom_->numberOfWiresPerGroup(wireg1);   
  double wSpace    = specs_->wireSpacing();
  float wAngle     = layergeom_->wireAngle();
  float dy         = wSpace * (n_wires - 1) / sqrt(12.);       // Error on y'  (on y if wAngle = 0)


  // Now, find strip position and properties
  int stripClusterSize = sHit.clusterSize();
  float strip_pos = sHit.sHitPos();
  int ch = int(strip_pos);
  float strip_offset = float(strip_pos) - ch;
  if ( strip_offset > 0.5 ) ch++;
  int centerStrip = ch;
  double sAngle   = layergeom_->stripAngle(ch);


  // If at the edge, then used 1 strip cluster only :
  if ( ch <= 1 || ch >= specs_->nStrips() ) {

    channels.push_back(centerStrip);
    LocalPoint lp1 = layergeom_->stripWireIntersection( centerStrip, the_wire1);
    LocalPoint lp2 = layergeom_->stripWireIntersection( centerStrip, the_wire2);
    float x1 = lp1.x();
    float x2 = lp2.x();
    float y1 = lp1.y();
    float y2 = lp2.y();

    float x = ( x1 + wgoffset * (x2 - x1) );
    float y = ( y1 + wgoffset * (y2 - y1) );

    LocalPoint lp3(x, y);

    // Ensure that y position is within active area (ME_11 chambers):
    // If not, use upper (ME_1b)/lower (ME_1a) edge
    LocalPoint lp0;
    keepHitInFiducial( lp3, lp0 );

    sigma =  layergeom_->stripPitch(lp0)/sqrt(12);  
    double dx = sigma/sin(sAngle);
    
    float sinangdif = sin(sAngle - wAngle);
    float sin2angdif = sinangdif * sinangdif;   
    
    float wsins = dy * sin(sAngle);
    float wcoss = dy * cos(sAngle);
    float ssinw = dx * sin(wAngle);
    float scosw = dx * cos(wAngle);
    
    float dx2 = (scosw*scosw + wcoss*wcoss)/sin2angdif;
    float dy2 = (ssinw*ssinw + wsins*wsins)/sin2angdif;
    float dxy = (scosw*ssinw + wcoss*wsins)/sin2angdif;
    
    LocalError localerr(dx2, dxy, dy2);
    
    CSCRecHit2D rechit( id, lp0, localerr, channels, chisq, prob );
    if (debug) std::cout << "Found rechit in layer " << this_layer << " with local position:  x = " << x << "  y = " << y << std::endl; 

    return rechit;  
  } 


  // If not at the edge, used cluster of size ClusterSize:

  // Store channels used
  for ( int i = (centerStrip-stripClusterSize/2); i <= (centerStrip+stripClusterSize/2); i++ ) {
    channels.push_back( i );
  }

  int ch0 = int(strip_pos);
  LocalPoint lp11  = layergeom_->stripWireIntersection( ch0,   the_wire1);
  LocalPoint lp111 = layergeom_->stripWireIntersection( ch0,   the_wire2);
  LocalPoint lp22  = layergeom_->stripWireIntersection( ch0+1, the_wire1);
  LocalPoint lp222 = layergeom_->stripWireIntersection( ch0+1, the_wire2);

  float x11  = lp11.x();
  float x111 = lp111.x();
  float x1   = (x11 + wgoffset * (x111 - x11) );
  float y11  = lp11.y();
  float y111 = lp111.y();
  float y1   = (y11 + wgoffset * (y111 - y11) );
  LocalPoint lp1(x1, y1);

  float x22  = lp22.x();
  float x222 = lp222.x();
  float x2   = (x22 + wgoffset * (x222 - x22) );
  float y22  = lp22.y();
  float y222 = lp222.y();
  float y2   = (y22 + wgoffset * (y222 - y22) );
  LocalPoint lp2(x2, y2);
  

  // Ensure that y position is within active area (ME_11 chambers):
  // If not, use upper (ME_1b)/lower (ME_1a) edge
  LocalPoint lp3;
  keepHitInFiducial( lp1, lp3 );
  LocalPoint lp4;
  keepHitInFiducial( lp2, lp4 );


  // Use center of gravity to determine local x and y position:
  x1 = lp3.x();
  x2 = lp4.x();
  float x  = (1. - strip_offset) * x1 + strip_offset * x2;
  y1 = lp3.y();
  y2 = lp4.y();
  float y  = (1. - strip_offset) * y1 + strip_offset * y2;

  if (debug) std::cout <<  "Output from simple centroid:" << std::endl;
  if (debug) std::cout <<  "x = " << x << std::endl;

  // Build local point
  float stripWidth = fabs(x2 - x1);  
  sigma =  stripWidth/sqrt(12);              

  // Here try to improve the strip position by applying Gatti fitter

  if ( useGatti ) {   
    LocalPoint lp55  = layergeom_->stripWireIntersection( centerStrip, the_wire1);
    LocalPoint lp555 = layergeom_->stripWireIntersection( centerStrip, the_wire2);
    x1 = lp55.x();
    y1 = lp55.y();
    x2 = lp555.x();
    y2 = lp555.y();

    x1   = (x1 + wgoffset * (x2 - x1) );
    y1   = (y1 + wgoffset * (y2 - y1) );
  
    LocalPoint lp5(x1, y1);
                                   
    LocalPoint lp6;
    keepHitInFiducial( lp5, lp6 );

    // Local position at center of strip
    float x_to_gatti = lp6.x();   
    y = lp6.y();
    float x_fit;
    xFitWithGatti_->findXOnStrip( layer_, sHit, x_to_gatti, stripWidth, x_fit, sigma, chisq, prob );

    if (debug) std::cout << "Centroid x, Gatti x, diff " << x << " " << x_fit << " " << x-x_fit << std::endl;

    x = x_fit;
  }
  LocalPoint lp0(x, y);


  // Now compute the errors properly on local x and y

  double dx = sigma / sin(sAngle);         
  
  float sinangdif = sin(sAngle - wAngle);
  float sin2angdif = sinangdif * sinangdif;
  
  float wsins = dy * sin(sAngle); 
  float wcoss = dy * cos(sAngle);
  float ssinw = dx * sin(wAngle);
  float scosw = dx * cos(wAngle);
  
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
CSCRecHit2D CSCMake2DRecHit::hitFromWireOnly(const CSCDetId& id, const CSCLayer* layer, const CSCWireHit& wHit){

  // Cache layer info for ease of access
  layer_        = layer;
  layergeom_    = layer_->geometry();
  specs_        = layer->chamber()->specs();
  float wAngle  = layergeom_->wireAngle();
  
  int this_layer= id.layer();

  double sigma, chisq, prob;
  chisq = 0.00;
  prob  = 1.00;
  
  CSCRecHit2D::ChannelContainer channels;
  
  // Find wire hit position and wire properties
  float wireg = wHit.wHitPos();                            // Position in terms of wire group #
  int wireg1 = int( wireg );
  int wireg2 = wireg1;
  if ( (wireg - wireg1) != 0. ) {
    wireg2++;
  }  
  float wgoffset   = wireg - wireg1;

  LocalPoint lp1 = layergeom_->localCenterOfWireGroup( wireg1 ); 
  LocalPoint lp2 = layergeom_->localCenterOfWireGroup( wireg2 );

  float x = lp1.x() + wgoffset * ( lp2.x() - lp1.x() ); 
  float y = lp1.y() + wgoffset * ( lp2.y() - lp1.y() ); 

  LocalPoint lp0(x, y);

  // Now compute errors properly:

  float width1 = layergeom_->lengthOfWireGroup( wireg1 );
  float width2 = layergeom_->lengthOfWireGroup( wireg1 );
  float width  = width1 + wgoffset * (width2 - width1 );

  sigma = width * cos(wAngle) / sqrt(12.);

  // What are the proper error on these ?????

  float dx2 = sigma * sigma;
  float dy2 = ( lp1.y()-lp2.y() )*( lp1.y()-lp2.y() ) /12.; 
  float dxy = dx2 + dy2;

  LocalError localerr(dx2, dxy, dy2);

  CSCRecHit2D rechit( id, lp0, localerr, channels, chisq, prob );

  if (debug) std::cout << "Found wire hit in layer " << this_layer << " with local position:  x = " << x << "  y = " << y << std::endl; 

  return rechit;
}



/* hitFromStripOnly
 *
 * Don't bother applying Gatti fit here since we don't know the "y", i.e. the width of the strip.
 * Centroid will do.  Use y = 0 to find the "x", then assume the strip width at top/sqrt(12) is error (maximum deviation)
 */
CSCRecHit2D CSCMake2DRecHit::hitFromStripOnly(const CSCDetId& id, const CSCLayer* layer, const CSCStripHit& sHit){

  // Cache layer info for ease of access
  layer_        = layer;
  layergeom_    = layer_->geometry();
  specs_        = layer->chamber()->specs();

  int this_layer= id.layer();

  double sigma, chisq, prob;
  chisq = 0.00;
  prob  = 1.00;

  CSCRecHit2D::ChannelContainer channels;

  // Strip position and properties
  int stripClusterSize = sHit.clusterSize();
  float strip_pos = sHit.sHitPos();
  int ch = int(strip_pos);
  float strip_offset = float(strip_pos) - ch;
  if ( strip_offset > 0.5 ) ch++;
  int centerStrip = ch;
  float chamberLength = layergeom_->length();
  float stripWidth = layergeom_->stripPitch(); 

  float x, xAtTop, xAtBottom;

  // If at the edge, then used 1 strip cluster only :
  if ( ch <= 1 || ch >= specs_->nStrips() ) {
    
    // Store channel used
    channels.push_back(centerStrip);

    x         = layergeom_->xOfStrip( ch, 0.);
    xAtTop    = layergeom_->xOfStrip( ch, chamberLength/2.);
    xAtBottom = layergeom_->xOfStrip( ch, -chamberLength/2.);

  // If not at the edge, used cluster of size ClusterSize:
  } else {
 
    // Store channels used
    for ( int i = (centerStrip-stripClusterSize/2); i <= (centerStrip+stripClusterSize/2); i++ ) {
      channels.push_back( i ); 
    }
    int ch0 = int(strip_pos);

    float x1, x2;
    x1 = layergeom_->xOfStrip( ch0,   0.); 
    x2 = layergeom_->xOfStrip( ch0+1, 0.);
    x  = (1. - strip_offset) * x1 + strip_offset * x2;

    x1     = layergeom_->xOfStrip( ch0,   chamberLength/2.); 
    x2     = layergeom_->xOfStrip( ch0+1, chamberLength/2.);
    xAtTop = (1. - strip_offset) * x1 + strip_offset * x2;

    x1        = layergeom_->xOfStrip( ch0,   -chamberLength/2.); 
    x2        = layergeom_->xOfStrip( ch0+1, -chamberLength/2.);
    xAtBottom = (1. - strip_offset) * x1 + strip_offset * x2;
  }

  float y=0.;

  LocalPoint lp0(x, y);

  sigma = (xAtTop - xAtBottom + stripWidth)/sqrt(12.);  // Be on the conservative side...


  // Now compute the errors, but again, what are they ???

  double dx2 = sigma * sigma;
  double dy2 = chamberLength*chamberLength/12.;   // i.e. sigma_y = chamberLength/sqrt(12);
  double dxy = dx2 + dy2;

  LocalError localerr(dx2, dxy, dy2);

  CSCRecHit2D rechit( id, lp0, localerr, channels, chisq, prob );

  if (debug) std::cout << "Found strip hit in layer " << this_layer << " with local position:  x = " << x << "  y = " << y << std::endl; 

  return rechit;
}



void CSCMake2DRecHit::keepHitInFiducial( LocalPoint& lp1, LocalPoint& lp0 ) {

  // Allow extra margin for future tuning etc. For now use 0.2 cm.
  const float marginAtEdge = 0.2;  

  // Initialize parameters needed for correction:
  float x1 = lp1.x();
  float y1 = lp1.y();
  int centralStrip = layergeom_->nearestStrip( lp0 );
  float wAngle     = layergeom_->wireAngle();      
  float apothem    = layergeom_->length()/2.;

  // Test if beyond edge of chamber
  if ( fabs(y1) > apothem )  {                                

    // beyond bottom of chamber
    if ( y1 < 0. ) {       

      float yNearWire     = apothem + marginAtEdge;           
      float xNearWire     = x1 - yNearWire * sin( wAngle );
      LocalPoint lpGetWire( xNearWire, yNearWire );
      float closestWire   = layergeom_->nearestWire( lpGetWire );
      lp0                 = layergeom_->stripWireIntersection( centralStrip, closestWire );

   // beyond top of chamber      
    } else {            

      float yNearWire     = apothem - marginAtEdge;           
      float xNearWire     = x1 + yNearWire * sin( wAngle );
      LocalPoint lpGetwire( xNearWire, yNearWire );
      float closestWire   = layergeom_->nearestWire( lpGetwire );
      lp0                 = layergeom_->stripWireIntersection( centralStrip, closestWire );
    }

  // Within chamber, so keep local point as is
  } else {              
    lp0 = lp1;
  } 
 
}

