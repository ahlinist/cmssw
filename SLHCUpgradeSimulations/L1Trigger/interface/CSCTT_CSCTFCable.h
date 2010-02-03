#ifndef __CSCTT_CSCTFCABLE_H
#define __CSCTT_CSCTFCABLE_H (1)

////////////////////////////////////////////////////////////////
// Class: CSCTT_CSCTFCable
// Authors: Ivan Furic, Bobby Scurlock, University of Florida
// Description: Stores L1 CSC Track-Finder tracks
////////////////////////////////////////////////////////////////

#include <vector>
#include <DataFormats/L1GlobalMuonTrigger/interface/L1MuRegionalCand.h>
#include <iostream>

class CSCTT_CSCTFCable {

 public:

  CSCTT_CSCTFCable( int phiSector = -99, int station = -99);

  typedef L1MuRegionalCand Hit;
  
  typedef std::vector<CSCTT_CSCTFCable::Hit> Storage;
  typedef std::vector<CSCTT_CSCTFCable::Hit>::iterator Iterator;
  typedef std::vector<CSCTT_CSCTFCable::Hit>::const_iterator ConstIterator;

  const CSCTT_CSCTFCable::Hit* ReadHit();
  void       WriteHit( const Hit* ahit );
  bool       AtEnd(){_readIter = _storage.begin(); for(int i=0;i<readtimes;i++) _readIter++; 
  		     if(_readIter == _storage.end()) return true; else return false;}
  int        Size(){return _storage.size();}
  void       Reset(){ _readIter = _storage.begin();readtimes=0;}
  void       Clear(){ _storage.clear(); Reset();}
  int readtimes;
private:

  CSCTT_CSCTFCable::Storage _storage;
  
  int _phiSector;
  int _station;

  CSCTT_CSCTFCable::ConstIterator _readIter;

};

#endif

