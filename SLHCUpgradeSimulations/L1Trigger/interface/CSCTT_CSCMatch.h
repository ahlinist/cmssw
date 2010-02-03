#ifndef __CSCTT_CSCMATCH_H
#define __CSCTT_CSCMATCH_H (1)

////////////////////////////////////////////////////////////////
// Class: CSCTT_CSCMatch
// Authors: Ivan Furic, Bobby Scurlock, University of Florida
// Description: Stores CSCTF track and its corresponding matched Tracker stubs
////////////////////////////////////////////////////////////////


#include <vector>
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h"
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuRegionalCand.h"

#include "SLHCUpgradeSimulations/L1Trigger/interface/CSCTT_MyStub.h"
#include <iostream>
class CSCTT_CSCMatch {

 public:

  CSCTT_CSCMatch();

  typedef L1MuRegionalCand Stub;

  //typedef TrajectoryStateOnSurface Hit;
  typedef CSCTT_MyStub Hit;
  
  
  typedef std::vector<CSCTT_CSCMatch::Hit> Storage;
  typedef std::vector<CSCTT_CSCMatch::Hit>::iterator Iterator;
  typedef std::vector<CSCTT_CSCMatch::Hit>::const_iterator ConstIterator;

  const CSCTT_CSCMatch::Storage* GetHits();
  const CSCTT_CSCMatch::Hit* ReadHit();
  const CSCTT_CSCMatch::Stub GetStub() const { return _stub; }
  //
  //vector<CSCTT_CSCMatch::Hit> GetMatchedTracklet() { return _storage;}
  //
  
  void       AddHit( const Hit* ahit ){ _storage.push_back( *ahit );}
  bool       AtEnd(){_readIter = _storage.begin(); for(int i=0;i<readtimes;i++) _readIter++; 
  		     if(_readIter == _storage.end()) return true; else return false;}
  void       SetStub( const Stub astub ) {_stub = astub;}
  int	     GetNumHits() const {return(_storage.size());}
  void       Clear(){_storage.clear();readtimes=0;}
  int readtimes;
  
  
  //std::vector<CSCTT_CSCMatch::Hit> GetMatchedTracklet() const {
  std::vector<CSCTT_CSCMatch::Hit> GetMatchedTracklet() const {
  	//std::vector<CSCTT_CSCMatch::Hit*> v;
	//v = _storage;
	
	return _storage;}
  
private:

  CSCTT_CSCMatch::Storage _storage;

  CSCTT_CSCMatch::Stub    _stub;
  CSCTT_CSCMatch::ConstIterator _readIter;

};

#endif

