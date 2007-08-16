// This is CSCMake2DRecHit

#include <RecoLocalMuon/CSCRecHitB/src/CSCMake2DRecHit.h>
#include <RecoLocalMuon/CSCRecHitB/src/CSCFitXonStripWithGatti.h>
#include <RecoLocalMuon/CSCRecHitB/src/probab.h>
#include <RecoLocalMuon/CSCRecHitB/interface/CSCStripHit.h>
#include <RecoLocalMuon/CSCRecHitB/interface/CSCWireHit.h>

#include <DataFormats/CSCRecHit/interface/CSCRecHit2D.h>
#include <DataFormats/MuonDetId/interface/CSCDetId.h>

#include <Geometry/CSCGeometry/interface/CSCLayer.h>
#include <Geometry/CSCGeometry/interface/CSCChamberSpecs.h>
#include <Geometry/CSCGeometry/interface/CSCLayerGeometry.h>

#include <FWCore/MessageLogger/interface/MessageLogger.h>
#include <FWCore/Utilities/interface/Exception.h>

#include <iostream>
#include <algorithm>
#include <cmath>
#include <string>


/* Constructor
 *
 */
CSCMake2DRecHit::CSCMake2DRecHit(const edm::ParameterSet& ps){
    
  debug                      = ps.getUntrackedParameter<bool>("CSCDebug");
  useCalib                   = ps.getUntrackedParameter<bool>("CSCUseCalibrations");
  stripWireDeltaTime         = ps.getUntrackedParameter<int>("CSCstripWireDeltaTime");
  useGatti                   = ps.getUntrackedParameter<bool>("CSCUseGattiFit");
  maxGattiChi2               = ps.getUntrackedParameter<double>("CSCMaxGattiChi2");

  xFitWithGatti_             = new CSCFitXonStripWithGatti( ps );
}   


/* Destructor
 *
 */
CSCMake2DRecHit::~CSCMake2DRecHit() {
  delete xFitWithGatti_;
}


/* hitFromStripAndWire
 *
 */
CSCRecHit2D CSCMake2DRecHit::hitFromStripAndWire(const CSCDetId& id, const CSCLayer* layer,
                                                 const CSCWireHit& wHit, const CSCStripHit& sHit){


  // Cache layer info for ease of access
  layer_        = layer;
  layergeom_    = layer_->geometry();
  specs_        = layer->chamber()->specs();
  id_           = id;

  float sqrt_12 = 3.4641;

  double sigma, chisq, prob;
  sigma = 0.00;
  chisq = 0.00;
  prob  = 1.00;
  float tpeak = 0.;
    
  CSCRecHit2D::ChannelContainer channels;
  CSCRecHit2D::ADCContainer adcMap;
  CSCRecHit2D::ChannelContainer wgroups;


  // Find wire hit position and wire properties
  wgroups = wHit.wgroups();
  int nWG = wgroups.size();
  int wireg1 = wgroups[0];
  int wireg2 = wgroups[nWG-1];

  int Nwires1 = layergeom_->numberOfWiresPerGroup( wireg1 );
  int Nwires2 = layergeom_->numberOfWiresPerGroup( wireg2 );

  float Mwire1 = layergeom_->middleWireOfGroup( wireg1 );
  float Mwire2 = layergeom_->middleWireOfGroup( wireg2 );

  int wire1 = (int) (Mwire1 - Nwires1 / 2. + 0.5);
  int wire2 = (int) (Mwire2 + Nwires2 / 2.);

  float centerWire = (wire1 + wire2) / 2.;

  float sigmaWire  = (layergeom_->yResolution( wireg1 ) + layergeom_->yResolution( wireg2 )) / 2.;


  // Find strips position and properties

  CSCRecHit2D::ChannelContainer strips = sHit.strips();
  int tmax = sHit.tmax();
  int nStrip = strips.size();
  int idCenterStrip = nStrip/2;
  int ch = strips[idCenterStrip];
  int centerStrip = ch;
  float strip_pos = sHit.sHitPos(); // centroid, in units of strip #


  // Setup ADCs
  std::vector<float> adcs = sHit.s_adc();
  std::vector<float> adc2;
  for ( int iStrip = 0; iStrip < nStrip; ++iStrip) {
    adc2.clear();
    for ( int t = 0; t < 4; ++t ) adc2.push_back(adcs[t+iStrip*4]);    
    adcMap.put( strips[iStrip], adc2.begin(), adc2.end() ); 
    if (iStrip == nStrip/2 ) 
      tpeak = 50. * ( adc2[0]*(tmax-1) + adc2[1]*tmax + adc2[2]*(tmax+1) ) / (adc2[0]+adc2[1]+adc2[2]);
  }


  // If at the edge, then used 1 strip cluster only :
  if ( ch == 1 || ch == specs_->nStrips() || nStrip < 2 ) {

    LocalPoint lp1 = layergeom_->stripWireIntersection( centerStrip, centerWire);

    float x = lp1.x();
    float y = lp1.y();

    LocalPoint lp0(x, y);

    sigma =  layergeom_->stripPitch(lp0) / sqrt_12; 

    // Now compute the errors properly on local x and y
    LocalError localerr = layergeom_->localError( centerStrip, sigma, sigmaWire );

    CSCRecHit2D rechit( id, lp0, localerr, strips, adcMap, wgroups, tpeak, chisq, prob );

    return rechit;  
  } 


  // If not at the edge, used cluster of size ClusterSize:

  int ch0 = strips[idCenterStrip];
  LocalPoint lp11  = layergeom_->stripWireIntersection( ch0, centerWire);

  float x = lp11.x();
  float y = lp11.y();

  LocalPoint lpTest(x, y);

  float stripWidth = layergeom_->stripPitch( lpTest );  
  sigma =  stripWidth / sqrt_12;


  // Here try to improve the strip position by applying Gatti fitter

  if ( useGatti ) {   

    if (useCalib) xFitWithGatti_->setCalibration( globalGainAvg, gains_, xtalk_, noise_ );

    // Local position at center of strip
    float x_to_gatti = x;   
    float x_fit;
    double sigma_fit, chisq_fit;

    xFitWithGatti_->findXOnStrip( id, layer_, sHit, centerStrip, x_to_gatti, stripWidth, x_fit, tpeak, sigma_fit, chisq_fit );

    // Did fit pass ?
    if ( chisq_fit > 0.00001 ) {
      x     = x_fit;
      sigma = sigma_fit;
      chisq = chisq_fit;
      int ndof = 5;
      prob  = probab(ndof, chisq_fit);

    } else {

      // Position from central strip
      x = x;
      sigma = stripWidth/2.;      // use half-strip resolution 
      chisq = 0.;
      prob = 1.0;
    }

  } else {

    // Position from centroid

    int ch1 = strips[idCenterStrip-1]; // Left strip
    int ch2 = strips[idCenterStrip+1]; // Right strip

    LocalPoint lp22  = layergeom_->stripWireIntersection( ch1, centerWire);
    LocalPoint lp33  = layergeom_->stripWireIntersection( ch2, centerWire);

    float x2 = lp22.x();
    float x3 = lp33.x();

    float w = strip_pos - centerStrip;

    if (w < 0.) {
      w = - w;
      x = x2 * w + x * (1 - w);
    } else {
      x = x * w + x3 * (1 - w);
    }
 
    sigma = stripWidth/sqrt_12;
    chisq = 0.;
    prob = 1.0;

  }   

  y = layergeom_->yOfWire(centerWire, x);

  LocalPoint lp0(x, y);

  // Now compute the errors properly on local x and y
  LocalError localerr = layergeom_->localError( centerStrip, sigma, sigmaWire );

  // store rechit    
  CSCRecHit2D rechit( id, lp0, localerr, strips, adcMap, wgroups, tpeak, chisq, prob );

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

  double sigma, chisq, prob;
  sigma = chisq = 0.00;
  prob  = 1.00;
  float dx2, dy2, dxy;
  dx2 = dy2 = dxy = 0.;

  CSCRecHit2D::ChannelContainer strips;
  CSCRecHit2D::ChannelContainer wiregroups;
  CSCRecHit2D::ADCContainer adcMap;

  // Find wire hit position and wire properties
  float tpeak = wHit.tmax() * 50.;
  float wireg = wHit.wHitPos();                            // Position in terms of wire group #
  CSCRecHit2D::ChannelContainer wgroups = wHit.wgroups();
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

  LocalPoint lp0(x,y);

  // Now compute errors properly:

  float width1 = layergeom_->lengthOfWireGroup( wireg1 );
  float width2 = layergeom_->lengthOfWireGroup( wireg1 );
  float width  = width1 + wgoffset * (width2 - width1 );

  sigma = width * cos(wAngle) / sqrt(12.);

  // What are the proper error on these ?????

  dx2 = sigma * sigma;
  dy2 = ( lp1.y()-lp2.y() )*( lp1.y()-lp2.y() ) /12.; 
  dxy = 0.;

  LocalError localerr(dx2, dxy, dy2);

  CSCRecHit2D rechit( id, lp0, localerr, strips, adcMap, wgroups, tpeak, chisq, prob );

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

  CSCRecHit2D::ChannelContainer wgroups;
  CSCRecHit2D::ADCContainer adcMap;

  // Strip position and properties
  CSCRecHit2D::ChannelContainer strips = sHit.strips();
  int stripClusterSize = strips.size();
  float tpeak = sHit.tmax()*50.;
  float strip_pos = sHit.sHitPos();
  int ch = int(strip_pos);

  float chamberHalfLength = layergeom_->length()/2.;
  float stripWidth = layergeom_->stripPitch(); 

  std::vector<float> adcs = sHit.s_adc();
  std::vector<float> adc2;
  for ( int t = 0; t < stripClusterSize*4; ++t ) {
    if ( t%4 == 0 ) adc2.clear();
    adc2.push_back(adcs[t]);
    if ( (t+1)%4 == 0) adcMap.put( strips[t/4], adc2.begin(), adc2.end() );
  }

  float x, xAtTop, xAtBottom;

  // Use central strip for this case (not worth using centroid)
  x         = layergeom_->xOfStrip( ch, 0.);
  xAtTop    = layergeom_->xOfStrip( ch, chamberHalfLength);
  xAtBottom = layergeom_->xOfStrip( ch, -chamberHalfLength);

  float y=0.;

  LocalPoint lp0(x, y);

  sigma = (0.5*(xAtTop+xAtBottom)+stripWidth)/sqrt(12.);     // Be on the conservative side...

  // Now compute the errors, but again, what are they ???

  double dx2 = sigma * sigma;
  double dy2 = chamberHalfLength*chamberHalfLength/12.;      // i.e. sigma_y = 0.5 * chamberLength/sqrt(12);
  double dxy = 0.;

  LocalError localerr(dx2, dxy, dy2);

  CSCRecHit2D rechit( id, lp0, localerr, strips, adcMap, wgroups, tpeak, chisq, prob );

  if (debug) std::cout << "Found strip hit in layer " << this_layer << " with local position:  x = " << x << "  y = " << y << std::endl; 

  return rechit;
}



/* keepHitInFiducial
 *
 * Only useful for ME11 chambers.
 */
bool CSCMake2DRecHit::keepHitInFiducial( LocalPoint& lp1, LocalPoint& lp0 ) {

  bool isInFiducial = true;

  lp0 = lp1;
  return isInFiducial;

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
 
 

