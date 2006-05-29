#include "RecoTracker/TkDetLayers/interface/PixelForwardLayer.h"
#include "Geometry/Surface/interface/BoundingBox.h"
#include "Geometry/Surface/interface/SimpleDiskBounds.h"

#include "TrackingTools/DetLayers/interface/DetLayerException.h"
#include "TrackingTools/DetLayers/interface/simple_stat.h"
#include "TrackingTools/DetLayers/interface/PhiLess.h"
#include "TrackingTools/GeomPropagators/interface/HelixArbitraryPlaneCrossing2Order.h"
#include "TrackingTools/GeomPropagators/interface/HelixArbitraryPlaneCrossing.h"
#include "TrackingTools/PatternTools/interface/MeasurementEstimator.h"


#include "RecoTracker/TkDetLayers/interface/LayerCrossingSide.h"
#include "RecoTracker/TkDetLayers/interface/DetGroupMerger.h"
#include "RecoTracker/TkDetLayers/interface/CompatibleDetToGroupAdder.h"


typedef GeometricSearchDet::DetWithState DetWithState;

PixelForwardLayer::PixelForwardLayer(vector<const PixelBlade*>& blades):
  theComps(blades.begin(),blades.end())
{
  for(vector<const GeometricSearchDet*>::const_iterator it=theComps.begin();
      it!=theComps.end();it++){  
    theBasicComps.insert(theBasicComps.end(),	
			 (**it).basicComponents().begin(),
			 (**it).basicComponents().end());
  }

  //They should be already phi-ordered. TO BE CHECKED!!
  //sort( theBlades.begin(), theBlades.end(), PhiLess());
  setSurface( computeDisk(blades) );
  
  //Is a "periodic" binFinderInPhi enough?. TO BE CHECKED!!
  theBinFinder = BinFinderType( theComps.front()->surface().position().phi(),
				theComps.size());

  /*--------- DEBUG INFO --------------
  cout << "DEBUG INFO for PixelForwardLayer" << endl;
  cout << "PixelForwardLayer.surfcace.phi(): " 
       << this->surface().position().phi() << endl;
  cout << "PixelForwardLayer.surfcace.z(): " 
       << this->surface().position().z() << endl;
  cout << "PixelForwardLayer.surfcace.innerR(): " 
       << this->specificSurface().innerRadius() << endl;
  cout << "PixelForwardLayer.surfcace.outerR(): " 
       << this->specificSurface().outerRadius() << endl;
  //cout << "PixelForwardLayer.surfcace.thickness(): " << specificSurface().thickness << end

  for(vector<const PixelBlade*>::const_iterator it=theBlades.begin(); 
      it!=theBlades.end(); it++){
    cout << "blades phi,z,r: " 
	 << (*it)->surface().position().phi() << " , "
	 << (*it)->surface().position().z() <<   " , "
	 << (*it)->surface().position().perp() << endl;
  }
  ----------------------------------- */

    
}

PixelForwardLayer::~PixelForwardLayer(){
  vector<const GeometricSearchDet*>::const_iterator i;
  for (i=theComps.begin(); i!=theComps.end(); i++) {
    delete *i;
  }
} 

  

vector<DetWithState> 
PixelForwardLayer::compatibleDets( const TrajectoryStateOnSurface& startingState,
				   const Propagator& prop, 
				   const MeasurementEstimator& est) const{

  // standard implementation of compatibleDets() for class which have 
  // groupedCompatibleDets implemented.
  // This code should be moved in a common place intead of being 
  // copied many times.
  
  vector<DetWithState> result;  
  vector<DetGroup> vectorGroups = groupedCompatibleDets(startingState,prop,est);
  for(vector<DetGroup>::const_iterator itDG=vectorGroups.begin();
      itDG!=vectorGroups.end();itDG++){
    for(vector<DetGroupElement>::const_iterator itDGE=itDG->begin();
	itDGE!=itDG->end();itDGE++){
      result.push_back(DetWithState(itDGE->det(),itDGE->trajectoryState()));
    }
  }
  return result;  
}


vector<DetGroup> 
PixelForwardLayer::groupedCompatibleDets( const TrajectoryStateOnSurface& tsos,
					  const Propagator& prop,
					  const MeasurementEstimator& est) const
{  
  vector<DetGroup> result;  //to clean out
  vector<DetGroup> closestResult;
  SubTurbineCrossings  crossings; 
  try{
    crossings = computeCrossings( tsos, prop.propagationDirection());
  }
  catch(DetLayerException& err){
    //cout << "Aie, got a DetLayerException in PixelForwardLayer::groupedCompatibleDets:" 
    //	 << err.what() << endl;
    return closestResult;
  }
  CompatibleDetToGroupAdder adder;
  adder.add( *theComps[theBinFinder.binIndex(crossings.closestIndex)], 
	     tsos, prop, est, closestResult);

  if(closestResult.empty()){
    vector<DetGroup> nextResult;
    adder.add( *theComps[theBinFinder.binIndex(crossings.nextIndex)], 
	       tsos, prop, est, nextResult);
    return nextResult;
  }      

  DetGroupElement closestGel( closestResult.front().front());
  float window = computeWindowSize( closestGel.det(), closestGel.trajectoryState(), est);

  //vector<DetGroup> result;
  float detWidth = closestGel.det()->surface().bounds().width();
  if (crossings.nextDistance < detWidth + window) {
    vector<DetGroup> nextResult;
    if (adder.add( *theComps[theBinFinder.binIndex(crossings.nextIndex)], 
		   tsos, prop, est, nextResult)) {
      int crossingSide = LayerCrossingSide().endcapSide( tsos, prop);
      DetGroupMerger merger;
      int theHelicity = computeHelicity(theComps[theBinFinder.binIndex(crossings.closestIndex)],
					theComps[theBinFinder.binIndex(crossings.nextIndex)] );
      result = merger.orderAndMergeTwoLevels( closestResult, nextResult, 
					      theHelicity, crossingSide);
    }
    else {
      result = closestResult;
    }
  }

  // only loop over neighbors (other than closest and next) if window is BIG
  if (window > 0.5*detWidth) {
    searchNeighbors( tsos, prop, est, crossings, window, result);
  } 
  return result;  
}



void 
PixelForwardLayer::searchNeighbors( const TrajectoryStateOnSurface& tsos,
				    const Propagator& prop,
				    const MeasurementEstimator& est,
				    const SubTurbineCrossings& crossings,
				    float window, 
				    vector<DetGroup>& result) const
{
  CompatibleDetToGroupAdder adder;
  int crossingSide = LayerCrossingSide().endcapSide( tsos, prop);
  DetGroupMerger merger;

  int negStart = min( crossings.closestIndex, crossings.nextIndex) - 1;
  int posStart = max( crossings.closestIndex, crossings.nextIndex) + 1;

  int quarter = theComps.size()/4;
  for (int idet=negStart; idet >= negStart - quarter+1; idet--) {
    const GeometricSearchDet* neighbor = theComps[theBinFinder.binIndex(idet)];
    // if (!overlap( gCrossingPos, *neighbor, window)) break; // mybe not needed?
    // maybe also add shallow crossing angle test here???
    vector<DetGroup> tmp;
    if (!adder.add( *neighbor, tsos, prop, est, tmp)) break;
    int theHelicity = computeHelicity(theComps[theBinFinder.binIndex(idet)],
				      theComps[theBinFinder.binIndex(idet+1)] );
    result = merger.orderAndMergeTwoLevels( tmp, result, theHelicity, crossingSide);
  }
  for (int idet=posStart; idet < posStart + quarter-1; idet++) {
    const GeometricSearchDet* neighbor = theComps[theBinFinder.binIndex(idet)];
    // if (!overlap( gCrossingPos, *neighbor, window)) break; // mybe not needed?
    // maybe also add shallow crossing angle test here???
    vector<DetGroup> tmp;
    if (!adder.add( *neighbor, tsos, prop, est, tmp)) break;
    int theHelicity = computeHelicity(theComps[theBinFinder.binIndex(idet-1)],
				      theComps[theBinFinder.binIndex(idet)] );
    result = merger.orderAndMergeTwoLevels( result, tmp, theHelicity, crossingSide);
  }
}

int 
PixelForwardLayer::computeHelicity(const GeometricSearchDet* firstBlade,const GeometricSearchDet* secondBlade) const
{  
  if( fabs(firstBlade->position().z()) < fabs(secondBlade->position().z()) ) return 0;
  return 1;
}

PixelForwardLayer::SubTurbineCrossings 
PixelForwardLayer::computeCrossings( const TrajectoryStateOnSurface& startingState,
				     PropagationDirection propDir) const
{  
  typedef MeasurementEstimator::Local2DVector Local2DVector;

  HelixPlaneCrossing::PositionType startPos( startingState.globalPosition());
  HelixPlaneCrossing::DirectionType startDir( startingState.globalMomentum());
  float rho( startingState.transverseCurvature());

  HelixArbitraryPlaneCrossing turbineCrossing( startPos, startDir, rho,
					       propDir);

  pair<bool,double> thePath = turbineCrossing.pathLength( specificSurface() );
  
  if (!thePath.first) {
    //cout << "ERROR in PixelForwardLayer: disk not crossed by track" << endl;
    throw DetLayerException("PixelForwardLayer: disk not crossed by track");
  }

  HelixPlaneCrossing::PositionType  turbinePoint( turbineCrossing.position(thePath.second));
  HelixPlaneCrossing::DirectionType turbineDir( turbineCrossing.direction(thePath.second));
  int closestIndex = theBinFinder.binIndex(turbinePoint.phi());

  const BoundPlane& closestPlane( dynamic_cast<const BoundPlane&>( 
    theComps[closestIndex]->surface()));


  HelixArbitraryPlaneCrossing2Order theBladeCrossing(turbinePoint, turbineDir, rho);

  pair<bool,double> theClosestBladePath = theBladeCrossing.pathLength( closestPlane );
  LocalPoint closestPos = closestPlane.toLocal(GlobalPoint(theBladeCrossing.position(theClosestBladePath.second)) );
    
  float closestDist = closestPos.x(); // use fact that local X perp to global Y

  //int next = turbinePoint.phi() - closestPlane.position().phi() > 0 ? closest+1 : closest-1;
  int nextIndex = PhiLess()( closestPlane.position().phi(), turbinePoint.phi()) ? 
    closestIndex+1 : closestIndex-1;

  const BoundPlane& nextPlane( dynamic_cast<const BoundPlane&>( 
    theComps[ theBinFinder.binIndex(nextIndex)]->surface()));

  pair<bool,double> theNextBladePath    = theBladeCrossing.pathLength( nextPlane );
  LocalPoint nextPos = nextPlane.toLocal(GlobalPoint(theBladeCrossing.position(theNextBladePath.second)) );

  float nextDist = nextPos.x();

  if (fabs(closestDist) < fabs(nextDist)) {
    return SubTurbineCrossings( closestIndex, nextIndex, nextDist);
  }
  else {
    return SubTurbineCrossings( nextIndex, closestIndex, closestDist);
  }
}

float 
PixelForwardLayer::computeWindowSize( const GeomDet* det, 
				      const TrajectoryStateOnSurface& tsos, 
				      const MeasurementEstimator& est) const
{
  return est.maximalLocalDisplacement(tsos, dynamic_cast<const BoundPlane&>(det->surface())).x();
}



BoundDisk* 
PixelForwardLayer::computeDisk(const vector<const PixelBlade*>& blades) const
{
  vector<const PixelBlade*>::const_iterator ifirst = blades.begin();
  vector<const PixelBlade*>::const_iterator ilast  = blades.end();

  // Find extension in R
  // float tolerance = 1.; // cm
  float theRmin = (**ifirst).position().perp(); float theRmax = theRmin;
  float theZmin = (**ifirst).position().z(); float theZmax = theZmin;
  for ( vector<const PixelBlade*>::const_iterator deti = ifirst;
	deti != ilast; deti++) {
    vector<GlobalPoint> corners = 
      BoundingBox().corners( dynamic_cast<const BoundPlane&>((**deti).surface()));
    for (vector<GlobalPoint>::const_iterator ic = corners.begin();
	 ic != corners.end(); ic++) {
      float r = ic->perp();
      float z = ic->z();
      theRmin = min( theRmin, r);
      theRmax = max( theRmax, r);
      theZmin = min( theZmin, z);
      theZmax = max( theZmax, z);
    }

    // in addition to the corners we have to check the middle of the 
    // det +/- length/2
    // , since the min (max) radius for typical fw dets is reached there
    float rdet = (**deti).position().perp();
    float len = (**deti).surface().bounds().length();
    theRmin = min( theRmin, rdet-len/2.F);
    theRmax = max( theRmax, rdet+len/2.F);
  }

#ifdef DEBUG_GEOM
  cout << "creating SimpleDiskBounds with r range" << theRmin << " " 
       << theRmax << " and z range " << theZmin << " " << theZmax << endl;
#endif
  
  // By default the forward layers are positioned around the z axis of the
  // global frame, and the axes of their local frame coincide with 
  // those of the global grame (z along the disk axis)
  float zPos = (theZmax+theZmin)/2.;
  Surface::PositionType pos(0.,0.,zPos);
  Surface::RotationType rot;

  return new BoundDisk( pos, rot, 
			SimpleDiskBounds( theRmin, theRmax, 
					  theZmin-zPos, theZmax-zPos));
}    


