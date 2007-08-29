/*
  Section collector class
  Copyright John Jones 2007
*/

// Prototype class definition
#include "SectionCollector.hh"

// Threading (ah hell now we're in trouble...)
#include <pthread.h>

// For usleep function
#include <unistd.h>

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
  SectionCollector::SectionCollector(u32 aNumBunches,
				     u32 aNumNibblesPerSection,
				     u32 aNumOrbitsPerNibble,
				     u32 aNumHLXs) {
    try {
      // Distributor thread
      mThreadId=0;
      mNumHLXs = aNumHLXs;
      mNumNibblesPerSection = aNumNibblesPerSection;
      mNumOrbitsPerNibble = aNumOrbitsPerNibble;
      mNumOrbits = aNumOrbitsPerNibble * aNumNibblesPerSection;
      mNumBunches = aNumBunches;
      mRunNumber = 0;
      this->Init();
      // Initialise the lumi section structures
      /*for ( u32 i = 0 ; i != mNumHLXs ; i++ ) {
	memset(mLumiSections+i,0,sizeof(LUMI_SECTION));
	mLumiSections[i].hdr.numOrbits = mNumOrbits;
	mLumiSections[i].hdr.numBunches = mNumBunches;
	}*/

      // Reset the luminosity section collector
      this->Reset();

      // Initialise the worker thread
      mWorkerThreadContinue=true;
      mBufferTransmit=false;
      mTransmitComplete=false;

      // Create the worker thread
      int ret = pthread_create(&mThreadId,
			   NULL,
			   (void*(*)(void*))SectionCollector::WorkerThread,
			   reinterpret_cast<void *>(this));
      if ( ret != 0 ) {
	MemoryAllocationException lExc("Cannot create worker thread");
	RAISE(lExc);
      }

    } catch (ICException & aExc) {
      RETHROW(aExc);
    }
  }

  // Destructor deletes the hardware interface
  SectionCollector::~SectionCollector() {
    try {

      // Rejoin the original thread
      mWorkerThreadContinue=false;
      pthread_join(mThreadId,NULL);
      mThreadId=0;
      mBufferTransmit=false;
      mTransmitComplete=false;

      // Clean up afterwards or we can have thread collisions
      this->CleanUp();

    } catch (ICException & aExc) {
      RETHROW(aExc);
    }
  }

  // Initialisation function
  void SectionCollector::Init() {
    try {
      mLumiSection = new LUMI_SECTION;
      if ( !mLumiSection ) {
	MemoryAllocationException lExc("Unable to allocate lumi section memory");
	RAISE(lExc);
      }
      mLumiSectionBuffer = new LUMI_SECTION;
      if ( !mLumiSectionBuffer ) {
	MemoryAllocationException lExc("Unable to allocate lumi section buffer");
	RAISE(lExc);
      }
    } catch (ICException & aExc) {
      RETHROW(aExc);
    }
  }

  // Clean up function
  void SectionCollector::CleanUp() {
    try {
      delete mLumiSection;
      mLumiSection = 0;
      delete mLumiSectionBuffer;
      mLumiSectionBuffer = 0;
    } catch (ICException & aExc) {
      RETHROW(aExc);
    }
  }

  // Counter reset function
  void SectionCollector::Reset() {
    try {
      mNumCompleteLumiSections = 0;
      mNumIncompleteLumiSections = 0;
      mNumLostLumiSections = 0;
      mSectionNumber = 0;
    } catch (ICException & aExc) {
      RETHROW(aExc);
    }
  }

  // Set the run number from RCMS
  void SectionCollector::SetRunNumber(u32 aRunNumber) {
    mRunNumber = aRunNumber;
  }

  // Statistics
  u32 SectionCollector::GetNumIncompleteLumiSections() {
    return mNumIncompleteLumiSections;
  }
  u32 SectionCollector::GetNumCompleteLumiSections() {
    return mNumCompleteLumiSections;
  }
  u32 SectionCollector::GetNumLostLumiSections() {
    return mNumLostLumiSections;
  }

  // Distributor attachment function
  void SectionCollector::AttachDistributor(AbstractDistributor *aDistributor) {
    try {
      // Add the distributor to the list
      this->mDistributors.push_back(aDistributor);
    } catch (ICException & aExc) {
      RETHROW(aExc);
    }
  }

  // Processing function for ET sum nibbles
  void SectionCollector::ProcessETSumNibble(const ET_SUM_NIBBLE & etSumNibble,
					    u8 hlxID) {
    if ( mTransmitComplete ) mBufferTransmit = false;

    // Check for incomplete previous lumi section
    if ( etSumNibble.hdr.numOrbits == mNumOrbitsPerNibble ) {
      if ( mLumiSection->hdr.startOrbit
	   + mLumiSection->hdr.numOrbits <= etSumNibble.hdr.startOrbit ) {
	// Move on to next lumi section as the previous one has to be complete by now...
	
	if ( mLumiSection->hdr.startOrbit != 0 ) {
	  mSectionNumber++;
	  mNumCompleteLumiSections++;
	  if ( !mTransmitComplete ) {
	    // Copy the data into the buffer
	    memcpy(mLumiSectionBuffer,mLumiSection,sizeof(LUMI_SECTION));
	    // Flag the new data
	    mBufferTransmit = true;
	  } else {
	    mNumLostLumiSections++;
	  }
	}

	// Initialise the lumi section
	memset(mLumiSection,0,sizeof(LUMI_SECTION));	
	mLumiSection->hdr.numHLXs = mNumHLXs;
	mLumiSection->hdr.sectionNumber = mSectionNumber;
	mLumiSection->hdr.runNumber = mRunNumber;
	mLumiSection->hdr.numOrbits = mNumOrbits;
	mLumiSection->hdr.numBunches = mNumBunches;
	mLumiSection->hdr.startOrbit = etSumNibble.hdr.startOrbit;
        mLumiSection->hdr.bCMSLive = etSumNibble.hdr.bCMSLive;

	// Load the first nibble
	mLumiSection->etSum[hlxID].hdr.numNibbles = 1;
	// As this is the first nibble of the set, copy into array
	for ( u32 i = 0 ; i != mNumBunches ; i++ ) {
	  // Accumulate the data
	  mLumiSection->etSum[hlxID].data[i] = etSumNibble.data[i];
	}
	
      } else {
	
	// Part of recording sequence, accumulate data
	// Check for missing nibbles - we assume that nibbles cannot arrive out of order
	// However the packets comprising them can - this is already handled by the nibble collector
	// This amounts to assuming that all the data is transmitted and received
	// during a single histogramming period. If not, we'd end up with all sort of problems
	// such as packet collisions etc. which would break the system anyway...
	
	// Add to the nibble count
	mLumiSection->etSum[hlxID].hdr.numNibbles++;
	
	// TODO, add etsumdatalow, high
	for ( u32 i = 0 ; i != mNumBunches ; i++ ) {
	  // Accumulate the data
	  mLumiSection->etSum[hlxID].data[i] += etSumNibble.data[i];
	}
	
      }
    }
    
  }

  void SectionCollector::WorkerThread(void *thisPtr) {
    // Poll for data here    
    // No exceptions, just plain-vanilla C++
    SectionCollector *theClass = reinterpret_cast<SectionCollector *>(thisPtr);

    while (theClass->mWorkerThreadContinue) {

      if ( !theClass->mTransmitComplete ) {
	if ( theClass->mBufferTransmit ) {
	
	  for ( u32 i = 0 ; i != theClass->mDistributors.size() ; i++ ) {
	    theClass->mDistributors[i]->ProcessSection(*theClass->mLumiSectionBuffer);
	    Sleep(1);
	  }
	  theClass->mTransmitComplete = true;

	}
      } else {
	if ( !theClass->mBufferTransmit ) {
	  theClass->mTransmitComplete = false;
	}
      }

      usleep(1000);

    }

    cout << "Worker thread complete" << endl;
  }

  void SectionCollector::ProcessLHCNibble(const LHC_NIBBLE & lhcNibble,
					  u8 hlxID) {
    if ( mTransmitComplete ) mBufferTransmit = false;

    // Check for incomplete previous lumi section
    if ( lhcNibble.hdr.numOrbits == mNumOrbitsPerNibble ) {
      if ( mLumiSection->hdr.startOrbit
	   + mLumiSection->hdr.numOrbits <= lhcNibble.hdr.startOrbit ) {

	// Move on to next lumi section as the previous one has to be complete by now...	
	if ( mLumiSection->hdr.startOrbit != 0 ) {
	  mSectionNumber++;
	  mNumCompleteLumiSections++;
	  if ( !mTransmitComplete ) {
	    // Copy the data into the buffer
	    memcpy(mLumiSectionBuffer,mLumiSection,sizeof(LUMI_SECTION));
	    // Flag the new data
	    mBufferTransmit = true;
	  } else {
	    mNumLostLumiSections++;
	  }
	}

	memset(mLumiSection,0,sizeof(LUMI_SECTION));
	mLumiSection->hdr.numHLXs = mNumHLXs;
	mLumiSection->hdr.sectionNumber = mSectionNumber;
	mLumiSection->hdr.runNumber = mRunNumber;
	mLumiSection->hdr.numOrbits = mNumOrbits;
	mLumiSection->hdr.numBunches = mNumBunches;
	mLumiSection->hdr.startOrbit = lhcNibble.hdr.startOrbit;
	mLumiSection->hdr.bCMSLive = lhcNibble.hdr.bCMSLive;
	  
	// Load the first nibble
	mLumiSection->lhc[hlxID].hdr.numNibbles = 1;
	// As this is the first nibble of the set, copy into array
	for ( u32 i = 0 ; i != mNumBunches ; i++ ) {
	  mLumiSection->lhc[hlxID].data[i] = lhcNibble.data[i];
	}

      } else {
	
	// Part of recording sequence, accumulate data
	// Check for missing nibbles - we assume that nibbles cannot arrive out of order
	// However the packets comprising them can - this is already handled by the nibble collector
	// This amounts to assuming that all the data is transmitted and received
	// during a single histogramming period. If not, we'd end up with all sort of problems
	// such as packet collisions etc. which would break the system anyway...
	
	// Add to the nibble count
	mLumiSection->lhc[hlxID].hdr.numNibbles++;
	
	for ( u32 i = 0 ; i != mNumBunches ; i++ ) {
	  // Accumulate the data
	  mLumiSection->lhc[hlxID].data[i] += lhcNibble.data[i];
	}
	
      }
    }
  }

  void SectionCollector::ProcessOccupancyNibble(const OCCUPANCY_NIBBLE & occupancyNibble,
						u8 hlxID) {
    if ( mTransmitComplete ) mBufferTransmit = false;

    // Check for incomplete previous lumi section
    if ( occupancyNibble.hdr.numOrbits == mNumOrbitsPerNibble ) {
      if ( mLumiSection->hdr.startOrbit
	   + mLumiSection->hdr.numOrbits <= occupancyNibble.hdr.startOrbit ) {

	// Move on to next lumi section as the previous one has to be complete by now...	
	if ( mLumiSection->hdr.startOrbit != 0 ) {
	  mSectionNumber++;
	  mNumCompleteLumiSections++;
	  if ( !mTransmitComplete ) {
	    // Copy the data into the buffer
	    memcpy(mLumiSectionBuffer,mLumiSection,sizeof(LUMI_SECTION));
	    // Flag the new data
	    mBufferTransmit = true;
	  } else {
	    mNumLostLumiSections++;
	  }
	}

	memset(mLumiSection,0,sizeof(LUMI_SECTION));
	mLumiSection->hdr.numHLXs = mNumHLXs;
	mLumiSection->hdr.sectionNumber = mSectionNumber;
	mLumiSection->hdr.runNumber = mRunNumber;
	mLumiSection->hdr.numOrbits = mNumOrbits;
	mLumiSection->hdr.numBunches = mNumBunches;
	mLumiSection->hdr.startOrbit = occupancyNibble.hdr.startOrbit;
	mLumiSection->hdr.bCMSLive = occupancyNibble.hdr.bCMSLive;

	// Load the first nibble
	mLumiSection->occupancy[hlxID].hdr.numNibbles = 1;

	for ( u32 j = 0 ; j != 6 ; j++ ) {
	  // As this is the first nibble of the set, copy into array
	  for ( u32 i = 0 ; i != mNumBunches ; i++ ) {
	    mLumiSection->occupancy[hlxID].data[j][i] = occupancyNibble.data[j][i];
	  }
	}

      } else {
	
	// Part of recording sequence, accumulate data
	// Check for missing nibbles - we assume that nibbles cannot arrive out of order
	// However the packets comprising them can - this is already handled by the nibble collector
	// This amounts to assuming that all the data is transmitted and received
	// during a single histogramming period. If not, we'd end up with all sort of problems
	// such as packet collisions etc. which would break the system anyway...
	
	// Add to the nibble count
	mLumiSection->occupancy[hlxID].hdr.numNibbles++;

	for ( u32 j = 0 ; j != 6 ; j++ ) {
	  for ( u32 i = 0 ; i != mNumBunches ; i++ ) {
	    // Accumulate the data
	    mLumiSection->occupancy[hlxID].data[j][i] += occupancyNibble.data[j][i];
	  }
	}
	
      }
    }
  }


} // namespace HCAL_HLX
