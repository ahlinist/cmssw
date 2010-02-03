#include "../interface/CSCTT_TrackerCable.h"

CSCTT_TrackerCable::CSCTT_TrackerCable( int phiSector, int station ):
  _phiSector(phiSector),
  _station(station)
{

  Clear();
  Reset();
  readtimes=0;
}


const CSCTT_TrackerCable::Hit* CSCTT_TrackerCable::ReadHit(){
const CSCTT_TrackerCable::Hit *retVal = 0;
  
 if (_readIter != _storage.end()){
   retVal = &*_readIter;
    _readIter++;
 }
 
 return retVal;

}

void CSCTT_TrackerCable::WriteHit( const Hit* ahit ){
  
  _storage.push_back ( *ahit );
  
  
}

