#ifndef __TurboSimMultiBuffer_h
#define __TurboSimMultiBuffer_h

#include <map>
#include "Validation/VstTurboSim/interface/TurboSimBuffer.hh"

template< typename keyType >
class TurboSimMultiBuffer : protected std::map< keyType, TurboSimBuffer *> {

public:

   ~TurboSimMultiBuffer() {
     for ( typename std::map< keyType, TurboSimBuffer *>::iterator i = 
	     std::map< keyType, TurboSimBuffer *>::begin();
	   i != std::map< keyType, TurboSimBuffer *>::end();
	   ++i )
       delete i->second;
   }

   keyType activeBuffer() const {
      return _currentBuffer; }

   keyType lastBuffer() const {
      return _lastBuffer; }
   
   void setActiveBuffer( keyType buf ) const {
      _lastBuffer = _currentBuffer;
      _currentBuffer = buf;
   }


protected:

   TurboSimMultiBuffer( )  {}

private:

   keyType _lastBuffer; // index of last used buffer
   keyType _currentBuffer; // current buffer

};

#endif
