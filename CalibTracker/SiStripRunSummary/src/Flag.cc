// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 05/31/07
// License: GPL

#include <ostream>

#include "CalibTracker/SiStripRunSummary/interface/Flag.h"

// --[ FLAG ]------------------------------------------------------------------
//                                                  --[ PUBLIC ]--
bool Flag::setState( const State &reSTATE) {
  // 1. reSTATE may be any value of: 0, 1, 2, 3...(2^n-1) where n is the nearest
  //    larger binary power.
  //
  //      oHackObj.setState( Flag::State( 3));
  //
  // 2. Change state of flag only in case a new value was submitted
  return ( 3 > static_cast<unsigned char>( reSTATE) &&
             reSTATE != eState_ ?
           ( eState_ = reSTATE, true) :
           false);
}

// --[ HELPER FUNCTIONS ]------------------------------------------------------
std::ostream &operator <<( std::ostream &roOut, const Flag &roFLAG) {
  roOut << static_cast<int>( roFLAG.getState()) << " : "
        << roFLAG.getComment();

  return roOut;
}
