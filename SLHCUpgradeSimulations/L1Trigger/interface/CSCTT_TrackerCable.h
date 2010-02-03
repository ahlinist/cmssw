#ifndef __CSCTT_TRACKERCABLE_H
#define __CSCTT_TRACKERCABLE_H (1)

////////////////////////////////////////////////////////////////
// Class: CSCTT_TrackerCable
// Authors: Ivan Furic, Bobby Scurlock, University of Florida
// Description: Stores stubs from tracker
////////////////////////////////////////////////////////////////

#include <vector>
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h"
#include "SLHCUpgradeSimulations/L1Trigger/interface/CSCTT_MyStub.h"

class CSCTT_TrackerCable {

 public:

  CSCTT_TrackerCable( int phiSector = -99, int station = -99);

  //typedef TrajectoryStateOnSurface Hit;
  //
  typedef CSCTT_MyStub Hit;
  //
  
  typedef std::vector<CSCTT_TrackerCable::Hit> Storage;
  typedef std::vector<CSCTT_TrackerCable::Hit>::iterator Iterator;
  typedef std::vector<CSCTT_TrackerCable::Hit>::const_iterator ConstIterator;

  const CSCTT_TrackerCable::Hit* ReadHit();
  void       WriteHit( const Hit* ahit );
  
  void       Reset(){ _readIter = _storage.begin();readtimes=0;}
  void       Clear(){ _storage.clear(); Reset();}

  int        Size(){ return _storage.size(); }
  int readtimes;
private:

  CSCTT_TrackerCable::Storage _storage;
  
  int _phiSector;
  int _station;

  CSCTT_TrackerCable::ConstIterator _readIter;

};

#endif

