#include "../interface/CSCTT_CSCMatchCable.h"

CSCTT_CSCMatchCable::CSCTT_CSCMatchCable( int phiSector, int station ):
  _phiSector(phiSector),
  _station(station)
{

  Clear();
  Reset();
  readtimes=0;
}


const CSCTT_CSCMatchCable::Match *CSCTT_CSCMatchCable::ReadMatch(){
  const CSCTT_CSCMatchCable::Match *retVal = 0;
  
  if (_readIter != _storage.end()){
    retVal = &*_readIter;
    _readIter++;
  }

  return retVal;

}

void CSCTT_CSCMatchCable::WriteMatch( const Match ahit ){
  _storage.push_back ( ahit );
 
}

