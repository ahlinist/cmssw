/*
  Abstract Section Collector class
  Copyright John Jones 2007

  Abstract base class defining the section collector
  for processing lumi nibble data into sections
*/

#ifndef ABSTRACTSECTIONCOLLECTOR_HH
#define ABSTRACTSECTIONCOLLECTOR_HH

// The string and stream definitions
//#include <iostream>
//#include <string>
//#include <vector>

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
  class AbstractSectionCollector {

  public:

    // Constructor
    AbstractSectionCollector() {}

    // Destructor
    virtual ~AbstractSectionCollector() {};

    // Processing function for ET sum histogram
    virtual void ProcessETSumNibble(const ET_SUM_NIBBLE & etSumNibble,
				    u8 hlxID) = 0;

    // Processing function for LHC histogram
    virtual void ProcessLHCNibble(const LHC_NIBBLE & lhcNibble,
				  u8 hlxID) = 0;

    // Processing function for occupancy histograms
    virtual void ProcessOccupancyNibble(const OCCUPANCY_NIBBLE & occupancyNibble,
					 u8 hlxID) = 0;

  }; //~class AbstractSectionCollector

}//~namespace HCAL_HLX

#endif //~ABSTRACTSECTIONCOLLECTOR_HH
