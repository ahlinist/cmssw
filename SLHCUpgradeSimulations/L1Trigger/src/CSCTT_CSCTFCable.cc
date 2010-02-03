#include "../interface/CSCTT_CSCTFCable.h"

CSCTT_CSCTFCable::CSCTT_CSCTFCable( int phiSector, int station ):
  _phiSector(phiSector),
  _station(station)
{

  Clear();
  Reset();
  readtimes=0; 
}


const CSCTT_CSCTFCable::Hit* CSCTT_CSCTFCable::ReadHit(){
const CSCTT_CSCTFCable::Hit *retVal = 0;

 
 if (_readIter != _storage.end()){
   retVal = &*_readIter;
   _readIter++;
 }
 
 //std::cout << "BLEGH " << _readIter - _storage.begin() << " OF " << _storage.size() <<  std::endl;
 
 return retVal;

}

void CSCTT_CSCTFCable::WriteHit( const Hit* ahit ){
 
  _storage.push_back ( *ahit );  
}

