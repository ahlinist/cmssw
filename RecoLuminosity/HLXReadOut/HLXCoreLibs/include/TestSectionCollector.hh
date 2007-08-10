/*
  Test section collector class
  Copyright John Jones 2007
*/

#ifndef TESTSECTIONCOLLECTOR_HH
#define TESTSECTIONCOLLECTOR_HH

// The string and stream definitions
//#include <iostream>
//#include <string>
//#include <vector>

// Abstract distributor definition
//#include "AbstractDistributor.hh"

// Abstract section collector definition
#include "AbstractSectionCollector.hh"

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
  class TestSectionCollector : public AbstractSectionCollector {

  public:

    // Constructor
    TestSectionCollector(u32 aNumHLXs,
			 u32 aNumOrbitsPerNibble);

    // Destructor
    ~TestSectionCollector();

    // Counter reset function
    void Reset();

    // Statistics
    u32 GetNumGoodNibbles();
    u32 GetNumBadNibbles();

    // Processing function for ET sum histogram
    void ProcessETSumNibble(const ET_SUM_NIBBLE & etSumNibble,
			    u8 hlxID);

    // Processing function for occupancy histograms
    void ProcessOccupancyNibble(const OCCUPANCY_NIBBLE & occupancyNibble,
				u8 hlxID);

    // Processing function for LHC histogram
    void ProcessLHCNibble(const LHC_NIBBLE & lhcNibble,
			  u8 hlxID);

  protected:

    // Number of HLXs in system;
    u32 mNumHLXs;

    // Number of orbits per nibble
    u32 mNumOrbitsPerNibble;

    // Statistics counters
    u32 mNumGoodNibbles;
    u32 mNumBadNibbles;

  }; //~class TestSectionCollector

}//~namespace HCAL_HLX

#endif //~TESTSECTIONCOLLECTOR_HH
