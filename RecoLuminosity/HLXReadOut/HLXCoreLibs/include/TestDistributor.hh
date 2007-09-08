/*
  Test distributor class
  Copyright John Jones
  For validation of test lumi section data
*/

#ifndef TESTDISTRIBUTOR_HH
#define TESTDISTRIBUTOR_HH

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
  class TestDistributor : public AbstractDistributor {

  public:

    // Constructor
    TestDistributor();

    // Destructor
    ~TestDistributor();

    // Section stats
    u32 GetNumBadSections() {
      return mNumBadSections;
    }
    u32 GetNumGoodSections() {
      return mNumGoodSections;
    }

    // Processing function for ET sum histogram
    bool ProcessSection(const LUMI_SECTION & lumiSection);

  private:

    u32 mNumBadSections;
    u32 mNumGoodSections;

  }; //~class TestDistributor

}//~namespace HCAL_HLX

#endif //~DEBUGCOUTDISTRIBUTOR_HH
