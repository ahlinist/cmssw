/*
  Test section collector class
  Copyright John Jones 2007
*/

// Prototype class definition
#include "TestSectionCollector.hh"

// Various generic and type-specific exception implementations
#include "ICException.hh"
#include "HardwareAccessException.hh"
#include "MemoryAllocationException.hh"
//#include "ArgumentOutOfRangeException.hh"

// HCAL HLX namespace
namespace HCAL_HLX
{

  // Define namespaces in use
  using namespace ICCoreUtils;
  using namespace std;
  
  // Default constructor
  TestSectionCollector::TestSectionCollector(u32 aNumHLXs,
					     u32 aNumOrbitsPerNibble) {
    try {
      mNumHLXs = aNumHLXs;
      mNumOrbitsPerNibble = aNumOrbitsPerNibble;
      this->Reset();
    } catch (ICException & aExc) {
      RETHROW(aExc);
    }
  }

  // Destructor deletes the hardware interface
  TestSectionCollector::~TestSectionCollector() {
  }

  // Counter reset function
  void TestSectionCollector::Reset() {
    try {
      mNumGoodNibbles = 0;
      mNumBadNibbles = 0;
    } catch (ICException & aExc) {
      RETHROW(aExc);
    }
  }

  u32 TestSectionCollector::GetNumGoodNibbles() {
    return mNumGoodNibbles;
  }
  u32 TestSectionCollector::GetNumBadNibbles() {
    return mNumBadNibbles;
  }

  // Processing function for ET sum nibbles
  void TestSectionCollector::ProcessETSumNibble(const ET_SUM_NIBBLE & etSumNibble,
						u8 hlxID) {
    try {
      u32 i, value;
      for ( i = 0 ; i != etSumNibble.hdr.numBunches ; i++ ) {
	value = i * mNumOrbitsPerNibble;
	if ( etSumNibble.data[i] != value ) {
	  //cout << i << "\t" << value << "\t" << etSumNibble.data[i] << endl;
	  break;
	}
      }
      if ( i != etSumNibble.hdr.numBunches ) {
	mNumBadNibbles++;
      } else {
	mNumGoodNibbles++;
      }
    } catch (ICException & aExc) {
      RETHROW(aExc);
    }
  }

  void TestSectionCollector::ProcessOccupancyNibble(const OCCUPANCY_NIBBLE & occupancyNibble,
						    u8 hlxID) {
    u32 i, j, value;
    for ( j = 0 ; j != 6 ; j++ ) {
      for ( i = 0 ; i != occupancyNibble.hdr.numBunches ; i++ ) {
	value = i * mNumOrbitsPerNibble;
	if ( value > 0xFFFF ) value = 0xFFFF;
	if ( occupancyNibble.data[j][i] != value ) {
	  break;
	}
      }
      if ( i != occupancyNibble.hdr.numBunches ) {
	mNumBadNibbles++;
	return;
      } else {
	mNumGoodNibbles++;
	return;
      }
    }
  }

  void TestSectionCollector::ProcessLHCNibble(const LHC_NIBBLE & lhcNibble,
						    u8 hlxID) {
    u32 i, value;
    for ( i = 0 ; i != lhcNibble.hdr.numBunches ; i++ ) {
      value = i * mNumOrbitsPerNibble;
      if ( value > 0xFFFF ) value = 0xFFFF;
      if ( lhcNibble.data[i] != value ) {
	break;
      }
    }
    if ( i != lhcNibble.hdr.numBunches ) {
      mNumBadNibbles++;
    } else {
      mNumGoodNibbles++;
    }
  }


} // namespace HCAL_HLX
