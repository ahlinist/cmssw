#include "../interface/CSCTT_TrackerSimCrude.h"
#include "TrackingTools/GeomPropagators/interface/Propagator.h"

const double CSCTT_TrackerSimCrude::_radii[5] =  {34.0, 50.0, 68.0, 86.0, 104.0};

const double CSCTT_TrackerSimCrude::_abszlimits[5][2] = { 
  {  0.0, 183.6},
  {  0.0, 270.0},
  {213.5, 270.0},
  {213.5, 270.0},
  {  0.0, 270.0}
};


CSCTT_TrackerSimCrude::CSCTT_TrackerSimCrude():
  _propagator      (0),
  _rand            (13),
  _npu             (0),
  _nEvePiled       (0),
  _nEveStop        (0),
  _nResets         (0),
  _resetEveryEvent (0)
{

  // create the cylinders and clear hit containers ...

  std::cout << " --- CSCTT_TrackerSimCrude creating cylinders --- " << std::endl;

  for (int i = 0; i <5; i++){

    Cylinder::PositionType pos(0,0,0);
    Cylinder::RotationType rot;
    
    _layerCylinders[i] = Cylinder::build(pos, rot, _radii[i]);

    _hitContainers[i].clear();

  }

  std::cout << " --- CSCTT_TrackerSimCrude cylinders created --- " << std::endl;

}


//TrajectoryStateOnSurface CSCTT_TrackerSimCrude::HitLayer ( int i, 
//						     FreeTrajectoryState *fts )
/*
MyStub CSCTT_TrackerSimCrude::HitLayer ( int i, 
						     FreeTrajectoryState *fts )
{

  if ((i < 0) || ( i>=5)){
    std::cout << " ... illegal layer number: " << i 
	      << " .. bombing " << std::endl; 
    //return false;
  }
  
  if (_propagator == 0){
    std::cout << " ... null propagator .. bombing " << std::endl;

    //return false;
  }

  //TrajectoryStateOnSurface out = _propagator -> propagate ( *fts , 
//							    *_layerCylinders[i] );

MyStub out = _propagator -> propagate ( *fts , 
							    *_layerCylinders[i] );
 
 // if(!out.isValid()){ return out; }

  //  std::cout << "HEPPART PROPAGATED OK!" << std::endl;

 // GlobalPoint outPoint = out.globalPosition();

  double hitZ = out.z; //outPoint.z();
  
  //  std::cout << "propagated radius: " << outPoint.perp() << std::endl;
  //  std::cout << "propagated phi:    " << outPoint.phi()  << std::endl;
  //  std::cout << "propagated z:      " << outPoint.z()    << std::endl;

  bool madeHit = true;

  madeHit &= ( fabs(hitZ) >= _abszlimits[i][0] );
  madeHit &= ( fabs(hitZ) <= _abszlimits[i][1] );

  if (madeHit){
    _hitContainers[i].push_back( out );
    return out;
  }

  //else return _invalid; i did this

}*/

void CSCTT_TrackerSimCrude::ResetStorage(){

  _nResets++;

  _nEvePiled = 0;

  _nEveStop  = _rand.Poisson( _npu );

  // reset actual storage

  for (int i = 0; i < 5; i++)
    _hitContainers[i].clear();


}

void CSCTT_TrackerSimCrude::NewEvent(){

  if (_resetEveryEvent){
    ResetStorage();
  } else {
    _nEvePiled++;
  }
  
}

void CSCTT_TrackerSimCrude::Print(){

  std::cout << "CSCTT_TrackerSimCrude status: " << std::endl;

  std::cout << std::endl;

  std::cout << "Simulating    " << _npu << " mean pileup events." << std::endl;
  std::cout << "Currently     " << _nEvePiled << " events piled up." << std::endl;
  std::cout << "Need to reach " << _nEveStop << " for new reset. " << std::endl;
  std::cout << "Resets:    " << _nResets << " so far." << std::endl;

  std::cout << std::endl;

  for (int i = 0; i < 5; i++){
    std::cout << "   Layer " << i << " hit container contains " << _hitContainers[i].size() << " hits " << std::endl;
  }
  
  std::cout << std::endl;

  if ( _resetEveryEvent )
    std::cout << "CSCTT_TrackerSimCrude *IS RESETTING STORAGE EVERY EVENT*" << std::endl;

}

