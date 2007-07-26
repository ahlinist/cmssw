// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 05/31/07
// License: GPL

#include <ostream>

#include "interface/Flag.h"

bool Flag::setState( const State &reSTATE) {
  // reSTATE may be any value of: 0, 1, 2, 3... e.g. 
  //
  //   oHackObj.setState( Flag::State( 3));
  //
  return ( 3 > static_cast<unsigned char>( reSTATE) ?
           ( eState_ = reSTATE, true) :
           false);
}

std::ostream &operator <<( std::ostream &roOut,
                           const Flag &roFLAG) {

  roOut << static_cast<int>( roFLAG.getState()) << " : "
        << roFLAG.getComment();

  return roOut;
}
