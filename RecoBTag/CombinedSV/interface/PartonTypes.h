#ifndef BTauReco_PartonTypes_h
#define BTauReco_PartonTypes_h

#include <string>

namespace combsv {
  namespace Partons {
    /** Type of parton from which the jet originated
     */
    enum PartonType {B=0, C=1, UDSG=2, UndefParton=99 };

    /**
     *  convenience functions that return descriptive strings, rather than
     *  integral types
     */
    std::string name ( PartonType );
    PartonType type ( const std::string & );
    PartonType type ( char );
  }
}

#endif
