/*
  Debug cout distributor class
  Copyright John Jones 2007

  Output lumi section data to cout
*/

#ifndef DEBUGCOUTDISTRIBUTOR_HH
#define DEBUGCOUTDISTRIBUTOR_HH

// The string and stream definitions
//#include <fstream>
//#include <string>
//#include <vector>

// Include AbstractDistributor definition
#include "AbstractDistributor.hh"

// Type definitions used by the HAL, etc...
#include "ICTypeDefs.hh"

// Standard high-level data structures for luminosity
#include "LumiStructures.hh"

// Namespace for the HCAL HLX
namespace HCAL_HLX
{

  // We shall be using the IC core utility library
  using namespace ICCoreUtils;
  using namespace std;

  // Prototype class definition
  class DebugCoutDistributor : public AbstractDistributor {

  public:

    // Constructor
    DebugCoutDistributor();

    // Destructor
    ~DebugCoutDistributor();

    // Processing function for ET sum histogram
    bool ProcessSection(const LUMI_SECTION & lumiSection);

  }; //~class DebugCoutDistributor

}//~namespace HCAL_HLX

#endif //~DEBUGCOUTDISTRIBUTOR_HH
