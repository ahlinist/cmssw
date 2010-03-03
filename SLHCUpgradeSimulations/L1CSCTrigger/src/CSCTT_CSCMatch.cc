#include "../interface/CSCTT_CSCMatch.h"


CSCTT_CSCMatch::CSCTT_CSCMatch(){
  Clear();
  readtimes=0;
}

const CSCTT_CSCMatch::Hit* CSCTT_CSCMatch::ReadHit(){
const CSCTT_CSCMatch::Hit *retVal = 0;
  
  _readIter = _storage.begin();
  
  for(int i=0;i<readtimes;i++) _readIter++;
  readtimes++;
  
  if (_readIter == _storage.end())
    return retVal;
  else {
    return (&*_readIter);
  //  _readIter++;
  }

}

