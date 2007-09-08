/*
  Debug file distributor class
  Copyright John Jones 2007

  Output lumi section data to a file
*/

#ifndef DEBUGFILEDISTRIBUTOR_HH
#define DEBUGFILEDISTRIBUTOR_HH

// The string and stream definitions
#include <fstream>
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
  class DebugFileDistributor : public AbstractDistributor {

  public:

    // Constructor
    DebugFileDistributor(const char *fileName);

    // Destructor
    ~DebugFileDistributor();

    // Processing function for ET sum histogram
    bool ProcessSection(const LUMI_SECTION & lumiSection);

  private:

    std::ofstream mOutFile;

  }; //~class DebugFileDistributor

}//~namespace HCAL_HLX

#endif //~DEBUGFILEDISTRIBUTOR_HH
