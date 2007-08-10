/*
  Section collector class
  Copyright John Jones 2007
*/

#ifndef SECTIONCOLLECTOR_HH
#define SECTIONCOLLECTOR_HH

// The string and stream definitions
//#include <iostream>
//#include <string>
#include <vector>

// Abstract distributor definition
#include "AbstractDistributor.hh"

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
  class SectionCollector : public AbstractSectionCollector {

  public:

    // Constructor
    SectionCollector(u32 aNumBunches,
		     u32 aNumNibblesPerSection,
		     u32 aNumOrbitsPerNibble,
		     u32 aNumHLXs);

    // Destructor
    ~SectionCollector();

    // Counter reset function
    void Reset();

    // Set the run number added to the data before shipping
    void SetRunNumber(u32 aRunNumber);

    // Statistics
    u32 GetNumIncompleteLumiSections();
    u32 GetNumCompleteLumiSections();
    u32 GetNumLostLumiSections();

    // Processing function for ET sum histogram
    void ProcessETSumNibble(const ET_SUM_NIBBLE & etSumNibble,
			    u8 hlxID);

    // Processing function for occupancy histograms
    void ProcessOccupancyNibble(const OCCUPANCY_NIBBLE & occupancyNibble,
				u8 hlxID);

    // Processing function for LHC histogram
    void ProcessLHCNibble(const LHC_NIBBLE & lhcNibble,
			  u8 hlxID);

    // Attach function for distributors
    void AttachDistributor(AbstractDistributor *aDistributor);

  protected:

    // Init/cleanup
    void Init();
    void CleanUp();

    // Thread worker function
    static void WorkerThread(void *thisPtr);
    pthread_t mThreadId;

    // Holds all lumi section data
    LUMI_SECTION *mLumiSection;
    // Buffer for distribution
    LUMI_SECTION *mLumiSectionBuffer;
    // Thread interlocks
    bool mBufferTransmit, mTransmitComplete, mWorkerThreadContinue;

    // Number of nibbles per lumi section
    u32 mNumNibblesPerSection;

    // Run number
    u32 mRunNumber;

    // Number of HLXs in system;
    u32 mNumHLXs;

    // Num orbits, num bunches
    u32 mNumOrbits;
    u32 mNumOrbitsPerNibble;
    u32 mNumBunches;

    // Statistics counters
    u32 mNumCompleteLumiSections;
    u32 mNumIncompleteLumiSections;
    u32 mNumLostLumiSections;

    // Distributor list
    std::vector<AbstractDistributor *> mDistributors;

  }; //~class SectionCollector

}//~namespace HCAL_HLX

#endif //~SECTIONCOLLECTOR_HH
