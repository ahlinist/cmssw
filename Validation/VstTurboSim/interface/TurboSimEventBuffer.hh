#ifndef __TURBOSIMEVENTBUFFER_H
#define __TURBOSIMEVENTBUFFER_H

#include <queue>
#include "Validation/VstTurboSim/interface/TurboSimEvent.hh"

class TurboSimEventBuffer : public std::queue< TurboSimEvent > {
			    
public:

   // Constructors
   TurboSimEventBuffer( ) : std::queue< TurboSimEvent >( ) { };

   // Store into buffer
   TurboSimEventBuffer &operator<<( const TurboSimEvent &data ) {
      std::queue< TurboSimEvent >::push( data );
      _successfulWrite = true;
      return *this;
   }

   // Read from buffer
   TurboSimEventBuffer &operator>>( TurboSimEvent &dest ) {
      if ( !std::queue< TurboSimEvent >::empty() ) {
	 dest = std::queue< TurboSimEvent >::front();
	 std::queue< TurboSimEvent >::pop( );
	 _successfulRead = true;
      }
      else
	 _successfulRead = false;
      return *this;
   }

   // Clear buffer
   TurboSimEventBuffer &clear();

   // Accessors provided by queue include:
   // size_type size() const;
   // bool empty() const;
   // const TurboSimEventBuffer &front() const;
   // TurboSimEventBuffer &front();
   // push, pop

private:

  bool _successfulRead, _successfulWrite;

};

#endif
