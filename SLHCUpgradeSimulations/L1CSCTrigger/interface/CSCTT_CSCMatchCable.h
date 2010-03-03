#ifndef __CSCTT_CSCMATCHCABLE_H
#define __CSCTT_CSCMATCHCABLE_H (1)

////////////////////////////////////////////////////////////////
// Class: CSCTT_CSCMatchCable
// Authors: Ivan Furic, Bobby Scurlock, University of Florida
// Description: Stores all CSCTT_CSCMatch objects for event
////////////////////////////////////////////////////////////////

#include <vector>
#include "../interface/CSCTT_CSCMatch.h"

class CSCTT_CSCMatchCable {

 public:

  CSCTT_CSCMatchCable( int phiSector = -99, int station = -99);

 // typedef TrajectoryStateOnSurface Match;
  
  typedef CSCTT_CSCMatch Match;
  typedef std::vector<CSCTT_CSCMatchCable::Match> Storage;
  typedef std::vector<CSCTT_CSCMatchCable::Match>::iterator Iterator;
  typedef std::vector<CSCTT_CSCMatchCable::Match>::const_iterator ConstIterator;

  const CSCTT_CSCMatchCable::Match *ReadMatch();
  void       WriteMatch( const Match ahit );
  bool       AtEnd(){_readIter = _storage.begin(); for(int i=0;i<readtimes;i++) _readIter++; 
  		     if(_readIter == _storage.end()) return true; else return false;}
  void       Reset(){ _readIter = _storage.begin();readtimes=0;}
  void       Clear(){ _storage.clear(); Reset();}
  int        NumMatchs(){ return(_storage.size());}
  int readtimes;
private:

  CSCTT_CSCMatchCable::Storage _storage;
  
  int _phiSector;
  int _station;

  CSCTT_CSCMatchCable::ConstIterator _readIter;

};

#endif

