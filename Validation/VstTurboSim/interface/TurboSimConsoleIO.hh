#ifndef __TurboSimConsoleIO_h
#define __TurboSimConsoleIO_h

//#include "Validation/VstTurboSim/interface/atox.hh"
#include <string>
#include <iostream>

inline int getVerbosity() {
   return 1;

     //   ( ( getenv( "turboSimVerbosity" ) == NULL ) ?
     //	   1 :
     //	   atox( getenv( "turboSimVerbosity" ) ) );
}

inline void printMsg( const std::string &msg, int level = 1 ) {
   return;
  if ( level >= getVerbosity() ) {
    cout << "\t[" << flush;
    system( "date +%T" );
    cout << "] " << msg << endl;
  }
}

inline void printWarning( const std::string &msg ) {
  if ( getVerbosity() > 1 ) {
    cout << "\t[" << flush;
    system( "date +%T" );
    cout << "] " << msg << endl;
  }
}

inline void printError( const std::string &msg ) {
  cerr << "\t[" << flush;
  system( "date +%T" );
  cerr << "] " << msg << endl;
}

#endif
