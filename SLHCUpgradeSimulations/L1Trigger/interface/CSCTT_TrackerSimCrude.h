#ifndef __CSCTT_TRACKERSIMCRUDE_H
#define __CSCTT_TRACKERSIMCRUDE_H (1)

////////////////////////////////////////////////////////////////
// Class: CSCTT_TrackerSimCrude
// Authors: Ivan Furic, Bobby Scurlock, University of Florida
// Description: Runs an ideal tracker model
////////////////////////////////////////////////////////////////

#include "DataFormats/GeometrySurface/interface/Cylinder.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h"

#include "SLHCUpgradeSimulations/L1Trigger/interface/CSCTT_MyStub.h"
#include "TRandom.h"

class FreeTrajectoryState;
class Propagator;

class CSCTT_TrackerSimCrude {

 public:

  //typedef TrajectoryStateOnSurface Hit;
  
  //
  typedef CSCTT_MyStub Hit;
  //
  typedef std::vector<CSCTT_TrackerSimCrude::Hit> Storage;
  typedef std::vector<CSCTT_TrackerSimCrude::Hit>::iterator Iterator;
  typedef std::vector<CSCTT_TrackerSimCrude::Hit>::const_iterator ConstIterator;

  CSCTT_TrackerSimCrude();

  //TrajectoryStateOnSurface HitLayer ( int i, 
//				      FreeTrajectoryState *fts );
CSCTT_MyStub HitLayer ( int i, 
				      FreeTrajectoryState *fts );				      
				      

  void SetPropagator( Propagator *p ){ _propagator = p; }

  void Print();

  void ResetStorage();
  void NewEvent();

  int GetNPU     () const { return _npu; }
  int GetEvePiled() const { return _nEvePiled; }
  int GetEveStop () const { return _nEveStop;  }

  void SetNPU(int i) { _npu = i; _nEveStop = _rand.Poisson(_npu); _nResets = 0; }

  void ResetEveryEvent( bool b = true ){ _resetEveryEvent = b; }

  const CSCTT_TrackerSimCrude::Storage *GetHitContainer( int i ){
    if (i < 0) return 0;
    if (i >= 5) return 0;
    return &_hitContainers[i];
  }

private:

  Cylinder::CylinderPointer _layerCylinders[5];  

  static const double _radii[5];

  static const double _abszlimits[5][2]; 
  
  Propagator *_propagator;

  TrajectoryStateOnSurface _invalid;

  TRandom _rand;
  
  int     _npu;

  int     _nEvePiled;

  int     _nEveStop;

  int     _nResets;

  bool    _resetEveryEvent;

  CSCTT_TrackerSimCrude::Storage _hitContainers[5];

};

#endif

