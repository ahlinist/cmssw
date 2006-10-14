// This is CSCMake2DRecHit

#include <RecoLocalMuon/CSCRecHitB/src/CSCMake2DRecHit.h>
#include <RecoLocalMuon/CSCRecHitB/src/CSCStripCrosstalk.h>
#include <RecoLocalMuon/CSCRecHitB/src/CSCStripNoiseMatrix.h>
#include <RecoLocalMuon/CSCRecHitB/src/CSCFitXonStripWithGatti.h>
#include <RecoLocalMuon/CSCRecHitB/src/probab.h>

#include <DataFormats/MuonDetId/interface/CSCDetId.h>
#include <DataFormats/CSCRecHit/interface/CSCStripHit.h>
#include <DataFormats/CSCRecHit/interface/CSCWireHit.h>
#include <DataFormats/CSCRecHit/interface/CSCRecHit2D.h>

#include <Geometry/CSCGeometry/interface/CSCLayer.h>
#include <Geometry/CSCGeometry/interface/CSCChamberSpecs.h>
#include <Geometry/CSCGeometry/interface/CSCLayerGeometry.h>

#include "CondFormats/CSCObjects/interface/CSCcrosstalk.h"
#include "CondFormats/DataRecord/interface/CSCcrosstalkRcd.h"
#include "CondFormats/CSCObjects/interface/CSCNoiseMatrix.h"
#include "CondFormats/DataRecord/interface/CSCNoiseMatrixRcd.h"

#include <Geometry/Vector/interface/LocalPoint.h>

#include <FWCore/MessageLogger/interface/MessageLogger.h>
#include <FWCore/Utilities/interface/Exception.h>

#include <iostream>
#include <algorithm>
#include <cmath>
#include <string>


CSCMake2DRecHit::CSCMake2DRecHit(const edm::ParameterSet& ps){
    
  debug                      = ps.getUntrackedParameter<bool>("CSCDebug");
  isData                     = ps.getUntrackedParameter<bool>("CSCIsRunningOnData");
  stripWireDeltaTime         = ps.getUntrackedParameter<int>("CSCstripWireDeltaTime");
  useGatti                   = ps.getUntrackedParameter<bool>("CSCUseGattiFit");
  maxGattiChi2               = ps.getUntrackedParameter<double>("CSCMaxGattiChi2");

  stripCrosstalk_            = new CSCStripCrosstalk( ps );
  stripNoiseMatrix_          = new CSCStripNoiseMatrix( ps );
  xFitWithGatti_             = new CSCFitXonStripWithGatti( ps );

}   


CSCMake2DRecHit::~CSCMake2DRecHit() {
  
  delete stripCrosstalk_;
  delete stripNoiseMatrix_;
  delete xFitWithGatti_;
}


/* hitFromStripAndWire
 *
 */
CSCRecHit2D CSCMake2DRecHit::hitFromStripAndWire(const CSCDetId& id, const CSCLayer* layer,
                                                 const CSCWireHit& wHit, const CSCStripHit& sHit){

  if (debug) std::cout <<"[CSCMake2DRecHit::hitFromStripAndWire] creating 2-D hit" << std::endl;  

  // Cache layer info for ease of access
  layer_        = layer;
  layergeom_    = layer_->geometry();
  specs_        = layer->chamber()->specs();
  id_           = id;
  int this_layer = id.layer();

  double sigma, chisq, prob;
  sigma = chisq = 0.00;
  float dx2, dy2, dxy;
  dx2 = dy2 = dxy = 0.;
  prob  = 1.00;
    
  CSCRecHit2D::ChannelContainer channels;

  // In case hit falls outside fiducial volume of chamber:
  bool keepHit = true;
  LocalPoint lpFailed(-999., -999.);
  LocalError localerrFailed(dx2, dxy, dy2);  
  CSCRecHit2D failedHit( id, lpFailed, localerrFailed, channels, chisq, prob );


  // Fill x-talk and noise matrix at once:
  if ( isData ) {
    stripCrosstalk_->setCrossTalk( xtalk_ );
    stripCrosstalk_->getCrossTalk( id, slopeLeft, interLeft, slopeRight, interRight );
    stripNoiseMatrix_->setNoiseMatrix( noise_ );
    stripNoiseMatrix_->getNoiseMatrix( id, nMatrix ); 
  } else {
    for ( int i = 0; i < 1500; i++ ) nMatrix.push_back( 0. );
    for ( int i = 0; i < 100; i++ ) {
      slopeRight[i] = 0.;
      slopeLeft[i]  = 0.;
      interRight[i] = 0.026;  // From MC digi...
      interLeft[i]  = 0.026;
    }
  }

  
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
  if ( strip_offset >= 0.5 ) ch++;
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
    LocalPoint lp0;
    keepHit = keepHitInFiducial( lp3, lp0 );

    if ( !keepHit ) { 
      if (debug) std::cout <<"[CSCMake2DRecHit::hitFromStripAndWire] failedHit" << std::endl;
      return failedHit;
    }
    sigma =  layergeom_->stripPitch(lp0)/sqrt(12);  
    double dx = sigma/sin(sAngle);
    
    float sinangdif = sin(sAngle - wAngle);
    float sin2angdif = sinangdif * sinangdif;   
    
    float wsins = dy * sin(sAngle);
    float wcoss = dy * cos(sAngle);
    float ssinw = dx * sin(wAngle);
    float scosw = dx * cos(wAngle);
    
    dx2 = (scosw*scosw + wcoss*wcoss)/sin2angdif;
    dy2 = (ssinw*ssinw + wsins*wsins)/sin2angdif;
    dxy = (scosw*ssinw + wcoss*wsins)/sin2angdif;
    
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
  LocalPoint lp3;
  keepHit = keepHitInFiducial( lp1, lp3 );
  if ( !keepHit ) { 
    if (debug) std::cout <<"[CSCMake2DRecHit::hitFromStripAndWire] failedHit" << std::endl;
    return failedHit;
  }
  LocalPoint lp4;
  keepHit = keepHitInFiducial( lp2, lp4 );
  if ( !keepHit ) { 
    if (debug) std::cout <<"[CSCMake2DRecHit::hitFromStripAndWire] failedHit" << std::endl;
    return failedHit;
  }

  // Use center of gravity to determine local x and y position:
  float x3 = lp3.x();
  float x4 = lp4.x();
  float x  = (1. - strip_offset) * x3 + strip_offset * x4;
  float y3 = lp3.y();
  float y4 = lp4.y();
  float y  = (1. - strip_offset) * y3 + strip_offset * y4;

  if (debug) std::cout <<  "Output from simple centroid:" << std::endl;
  if (debug) std::cout <<  "x = " << x << std::endl;

  // Build local point
  float stripWidth = fabs(x2 - x1);  
  sigma =  stripWidth/sqrt(12);              

  // Here try to improve the strip position by applying Gatti fitter

  if ( useGatti ) {   
                                     
    // Local position at center of strip  --> this was determined above
    float x_to_gatti = x1;   
    float x_fit;
    double sigma_fit, chisq_fit;

    std::vector<float> xtalks, nmatrix;
    for ( int j = centerStrip -2; j < centerStrip + 1; j++) {  // Index of strip is starts at 1 whereas array 
      xtalks.push_back(slopeLeft[j]);                          // starts at 0 --> make use of this here
      xtalks.push_back(interLeft[j]);
      xtalks.push_back(slopeRight[j]);
      xtalks.push_back(slopeRight[j]);
      for ( int k = 0; k < 15; k++ ) nmatrix.push_back(nMatrix[j*15 + k]);
    }   

    xFitWithGatti_->findXOnStrip( layer_, sHit, x_to_gatti, stripWidth, xtalks, nmatrix, x_fit, sigma_fit, chisq_fit );

    if (debug) std::cout << "Centroid x, Gatti x, diff " << x << " " << x_fit << " " << x-x_fit << std::endl;

    if (chisq_fit < maxGattiChi2 ) {
      x     = x_fit;
      sigma = sigma_fit;
      chisq = chisq_fit;
      int ndof = 5;
      prob  = probab(ndof, chisq_fit);
    }
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
  
  dx2 = (scosw*scosw + wcoss*wcoss)/sin2angdif;
  dy2 = (ssinw*ssinw + wsins*wsins)/sin2angdif;
  dxy = (scosw*ssinw + wcoss*wsins)/sin2angdif;
  
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
  id_           = id;
  float wAngle  = layergeom_->wireAngle();
  int this_layer= id.layer();

  bool keepHit;
  double sigma, chisq, prob;
  sigma = chisq = 0.00;
  prob  = 1.00;
  float dx2, dy2, dxy;
  dx2 = dy2 = dxy = 0.;

  CSCRecHit2D::ChannelContainer channels;
    
  // In case hit falls outside fiducial volume of chamber:
  LocalPoint lpFailed(-999., -999.);
  LocalError localerrFailed(dx2, dxy, dy2);
  CSCRecHit2D failedHit( id, lpFailed, localerrFailed, channels, chisq, prob );

  
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

  LocalPoint lp0;
  keepHit = keepHitInFiducial( lp1, lp0 );
  if ( !keepHit ) { 
    if (debug) std::cout <<"[CSCMake2DRecHit::hitFromWireOnly] failedHit" << std::endl;
    return failedHit;
  }	


  // Now compute errors properly:

  float width1 = layergeom_->lengthOfWireGroup( wireg1 );
  float width2 = layergeom_->lengthOfWireGroup( wireg1 );
  float width  = width1 + wgoffset * (width2 - width1 );

  sigma = width * cos(wAngle) / sqrt(12.);

  // What are the proper error on these ?????

  dx2 = sigma * sigma;
  dy2 = ( lp1.y()-lp2.y() )*( lp1.y()-lp2.y() ) /12.; 
  dxy = dx2 + dy2;

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
  id_           = id;

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



/* keepHitInFiducial
 *
 * Only useful for ME11 chambers.
 */
bool CSCMake2DRecHit::keepHitInFiducial( LocalPoint& lp1, LocalPoint& lp0 ) {

  bool isInFiducial = true;

  // Allow extra margin for future tuning etc. For now 0.1 cm.
  float marginAtEdge = 0.1; 

  // Initialize parameters needed for correction:
  float x1 = lp1.x();
  float y1 = lp1.y();
  int centralStrip = layergeom_->nearestStrip( lp0 );
  float wAngle     = layergeom_->wireAngle();      
  float apothem    = layergeom_->length()/2.;

  // Test what type of chambers we are dealing with

  if (id_.station() == 1 && id_.ring() == 4 ) {
    // Within ME1a chambers, allow hit to be reconstructed below bottom, 
    // but not above top of chamber
    if (y1 > apothem + marginAtEdge) {
      lp0 = lp1;
      isInFiducial = false;
      return isInFiducial;
    } 
  } else if (id_.station() == 1 && id_.ring() == 1 ) {
    // Within ME1b chambers, allow hit to be reconstructed above top, 
    // but not below bottom of chamber
    if (y1 < - (apothem + marginAtEdge)) {
      lp0 = lp1;
      isInFiducial = false;
      return isInFiducial;
    }

  } else {
   // Within all other chambers, wires are parallel to top/bottom of chamber 
   // so hit should always be within fiducial volume
   lp0 = lp1;
   return isInFiducial;
  }

  // Now deal with ME11 chambers:

  // Test if beyond edge of chamber
  if ( fabs(y1) > apothem )  {                                

    // beyond bottom of chamber
    if ( y1 < 0. ) {       

      float yNearWire     = apothem + marginAtEdge/cos( wAngle );           
      float xNearWire     = x1 - yNearWire * sin( wAngle );
      LocalPoint lpGetWire( xNearWire, yNearWire );
      float closestWire   = layergeom_->nearestWire( lpGetWire );
      lp0                 = layergeom_->stripWireIntersection( centralStrip, closestWire );

   // beyond top of chamber      
    } else {            

      float yNearWire     = apothem - marginAtEdge/cos( wAngle );           
      float xNearWire     = x1 + yNearWire * sin( wAngle );
      LocalPoint lpGetwire( xNearWire, yNearWire );
      float closestWire   = layergeom_->nearestWire( lpGetwire );
      lp0                 = layergeom_->stripWireIntersection( centralStrip, closestWire );
    }

  // Within chamber, so keep local point as is
  } else {              
    lp0 = lp1;
  } 

  return isInFiducial;
}



/* isHitInFiducial
 *
 * Only useful for ME11 chambers.
 */
bool CSCMake2DRecHit::isHitInFiducial( const CSCLayer* layer, const CSCRecHit2D& rh ) {

  bool isInFiducial = true;

  // Allow extra margin for future tuning etc.
  float marginAtEdge = 0.1; 

  const CSCLayerGeometry* layergeom = layer->geometry();
  
  float y = rh.localPosition().y();
  float apothem = layergeom->length()/2.;

  if ( fabs(y) > (apothem+marginAtEdge) ) isInFiducial=false;

  return isInFiducial;
}
 
 

