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

//For time stamp
#include <time.h>
#include <sys/time.h>
// Various generic and type-specific exception implementations
#include "ICException.hh"
#include "HardwareAccessException.hh"
#include "MemoryAllocationException.hh"
//#include "ArgumentOutOfRangeException.hh"

#include <sstream>
#include <fstream>
#include <iostream>

//#define HCAL_HLX_SECTION_COLLECTOR_DEBUG

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
				     u32 aNumHLXs,
				     u32 aInitialRunNumber) {
    try {
      // Distributor thread
      mNumHLXs = aNumHLXs;
      mNumNibblesPerSection = aNumNibblesPerSection;
      mNumOrbitsPerNibble = aNumOrbitsPerNibble;
      mNumOrbits = aNumOrbitsPerNibble * aNumNibblesPerSection;
      mNumBunches = aNumBunches;
      mNextRunNumber = aInitialRunNumber;
      mCurrentRunNumber = aInitialRunNumber;
      mResyncCount = 0;

      mLumiCalculator = new LumiCalc;
      if ( !mLumiCalculator ) {
 	MemoryAllocationException lExc("Cannot allocate LumiCalc module");
 	RAISE(lExc);
      }

      // Initialise data structures
      this->Init();

      // Initialise the worker thread
      mWorkerThreadContinue=true;

      // Initialise the error message mutex
      pthread_mutex_init(&mErrorMutex,
			 NULL);

    } catch (ICException & aExc) {
      RETHROW(aExc);
    }
  }

  // Destructor deletes the hardware interface
  SectionCollector::~SectionCollector() {
    try {

      // Rejoin the original thread
      mWorkerThreadContinue=false;
      for ( u32 i = 0 ; i != mDistributors.size() ; i++ ) {
	pthread_join(mDistributors[i]->threadId,NULL);
	mDistributors[i]->threadId = 0;
      }

      // Clean up afterwards or we can have thread collisions
      CleanUp();

      // Delete the lumi calculator
      if ( mLumiCalculator != 0 ) {
 	delete mLumiCalculator;
 	mLumiCalculator = 0;
      }

      // Destory the error message mutex
      pthread_mutex_destroy(&mErrorMutex);
      
    } catch (ICException & aExc) {
      RETHROW(aExc);
    }
  }

  // Initialisation function
  void SectionCollector::Init() {
    try {
      mLumiSection = new LUMI_SECTION;
      if ( mLumiSection == 0 ) {
	MemoryAllocationException lExc("Unable to allocate lumi section memory");
	RAISE(lExc);
      }

      mNumCompleteLumiSections = 0;
      mNumIncompleteLumiSections = 0;
      mSectionNumber = 1;
      mCurNumNibbles = 0;

      InitialiseLumiSection();

    } catch (ICException & aExc) {
      RETHROW(aExc);
    }
  }

  // Clean up function
  void SectionCollector::CleanUp() {
    try {
      for ( u32 i = 0 ; i != mDistributors.size() ; i++ ) {
	delete mDistributors[i];
      }
      mDistributors.clear();

      if ( mLumiSection != 0 ) {
	delete mLumiSection;
	mLumiSection = 0;
      }

    } catch (ICException & aExc) {
      RETHROW(aExc);
    }
  }

  // Set the next run number from RCMS
  void SectionCollector::SetNextRunNumber(u32 aRunNumber) {
    mNextRunNumber = aRunNumber;
  }

  // Statistics
  u32 SectionCollector::GetNumIncompleteLumiSections() {
    return mNumIncompleteLumiSections;
  }
  u32 SectionCollector::GetNumCompleteLumiSections() {
    return mNumCompleteLumiSections;
  }
  //u32 SectionCollector::GetNumLostLumiSections() {
  // return mNumLostLumiSections;
  //}

  // Distributor attachment function
  void SectionCollector::AttachDistributor(AbstractDistributor *aDistributor) {
    try {
      WorkerPlayground *tempPlayground = new WorkerPlayground;
      if ( tempPlayground == 0 ) {
	MemoryAllocationException lExc("Unable to allocate playground for new distributor");
	RAISE(lExc);
      }

      tempPlayground->distributor = aDistributor;
      tempPlayground->writeIndex = 0;
      tempPlayground->readIndex = 0;
      tempPlayground->thisPtr = this;

      // Create the worker thread
      int ret = pthread_create(&tempPlayground->threadId,
			   NULL,
			   (void*(*)(void*))SectionCollector::WorkerThread,
			   reinterpret_cast<void *>(tempPlayground));
      if ( ret != 0 ) {
	MemoryAllocationException lExc("Cannot create worker thread");
	RAISE(lExc);
      }

      // Add the distributor to the list
      mDistributors.push_back(tempPlayground);
    } catch (ICException & aExc) {
      RETHROW(aExc);
    }
  }

  void SectionCollector::WorkerThread(void *ptr) {
    // Poll for data here    
    // No exceptions, just plain-vanilla C++
    WorkerPlayground *thePlayground = reinterpret_cast<WorkerPlayground *>(ptr);
    SectionCollector *theClass = thePlayground->thisPtr;
    AbstractDistributor *theDistributor = thePlayground->distributor;
    u32 nextIndex;
    bool bRet;

    while (theClass->mWorkerThreadContinue) {

      if ( thePlayground->readIndex != thePlayground->writeIndex ) {
	bRet = theDistributor->ProcessSection(*(thePlayground->sectionBuffer+thePlayground->readIndex));
	if ( bRet ) {
	  nextIndex = thePlayground->readIndex;
#ifdef HCAL_HLX_SECTION_COLLECTOR_DEBUG
	  cout << "Worker thread: Submitting packet to distributor" << endl;
#endif
	  if ( nextIndex == HCAL_HLX_DISTRIBUTOR_BUFFER_DEPTH-1 ) {
	    nextIndex = 0;
	  } else {
	    nextIndex++;
	  }
	  thePlayground->readIndex = nextIndex;
	}
      }

      Sleep(1);

    }

  }

  const std::string SectionCollector::GetLastError() {
    // Locked to make it thread safe
    pthread_mutex_lock(&mErrorMutex);
    std::string localError = mErrorMsg;
    pthread_mutex_unlock(&mErrorMutex);
    return localError;
  }

  void SectionCollector::SetError(const std::string & errorMsg) {
    // Locked to make it thread safe
    pthread_mutex_lock(&mErrorMutex);
    mErrorMsg = errorMsg;
    pthread_mutex_unlock(&mErrorMutex);
  } 

  void SectionCollector::InitialiseLumiSection() {
      memset(mLumiSection,0,sizeof(LUMI_SECTION));	
      timeval now_time;
      gettimeofday(&now_time, NULL);
      mLumiSection->hdr.timestamp = now_time.tv_sec;
      mLumiSection->hdr.timestamp_micros = now_time.tv_usec;
      mLumiSection->hdr.numHLXs = mNumHLXs;
      mLumiSection->hdr.sectionNumber = mSectionNumber;
      mLumiSection->hdr.runNumber = mCurrentRunNumber;
      mLumiSection->hdr.numOrbits = mNumOrbits;
      mLumiSection->hdr.numBunches = mNumBunches;
  }

  // Processing function for ET sum nibbles
  void SectionCollector::ProcessETSumNibble(const ET_SUM_NIBBLE & etSumNibble,
					    u8 hlxID) {

    // Changed this to detect changes going backwards in time as well 

    // Conditions for synchronisation are either:
    // 1) TTC OC0 restart:
    //    a) Orbit start of nibble is less than or equal to previous lumi section (i.e. back in time)
    //    b) New nibble start orbit is zero
    //    c) HLX detected OC0
    //    Condition (a) is needed to prevent re-initialisation after the first OC0 from an HLX
    // 2) Normal operation:
    //    a) Orbit start of next nibble is greater than previous lumi section
    //    b) Orbit start of next nibble is an exact multiple of the lumi section 

    // The only caveat of this is that if an HLX glitches during running and jumps
    // it can knock out the rest of the DAQ during running, although an OC0 will resynchronise it

    // Transition condition checking
    // Either section boundary or resync
    /*
    bool resyncCondition = ((etSumNibble.hdr.startOrbit == 0) && // resync condition
			    (hlxID == 0) && // we need to choose a master to stop multiple resyncs
			    etSumNibble.hdr.bOC0);
    bool newLSCondition = ((mLumiSection->hdr.startOrbit < etSumNibble.hdr.startOrbit) &&
			    (etSumNibble.hdr.startOrbit % mNumOrbits == 0));
    */

    /*
    // Section commit management
    if (  newLSCondition ) {
    
      // Move on to next lumi section as the previous one has to be complete by now...
      // But only commit if there is at least one nibble in the section for one HLX
      if ( mCurNumNibbles != 0 ) CommitLumiSection();
      
      // If resync condition, update the run number
      if ( resyncCondition ) {
	mCurrentRunNumber = mNextRunNumber;
	mResyncCount++;
      }

      // Initialise the lumi section
      InitialiseLumiSection();
      mLumiSection->hdr.startOrbit = etSumNibble.hdr.startOrbit;
      mLumiSection->hdr.bCMSLive = etSumNibble.hdr.bCMSLive;
      mLumiSection->hdr.bOC0 = false;
      
      // Load the first nibble
      mLumiSection->etSum[hlxID].hdr.numNibbles = 1;
      mCurNumNibbles = 1;
      
      // As this is the first nibble of the set, copy into array
      for ( u32 i = 0 ; i != mNumBunches ; i++ ) mLumiSection->etSum[hlxID].data[i] = etSumNibble.data[i];
      
      // Copy the condition in for the nibble
      mLumiSection->hdr.bOC0 |= etSumNibble.hdr.bOC0;
      
      return;

    }
    */

    // Check we are on a nibble boundary and not a section boundary
    bool normalCondition = ((mLumiSection->hdr.startOrbit <= etSumNibble.hdr.startOrbit) &&
			    (etSumNibble.hdr.startOrbit % mNumOrbitsPerNibble == 0));

    // In normal running (accumulation)
    if ( normalCondition ) {
      
      // Part of recording sequence, accumulate data
      // Check for missing nibbles - we assume that nibbles cannot arrive out of order
      // However the packets comprising them can - this is already handled by the nibble collector
      // This amounts to assuming that all the data is transmitted and received
      // during a single histogramming period. If not, we'd end up with all sort of problems
      // such as packet collisions etc. which would break the system anyway...
      
      // Add to the nibble count
      mLumiSection->etSum[hlxID].hdr.numNibbles++;
      mCurNumNibbles++;

      // Accumulate the data
      for ( u32 i = 0 ; i != mNumBunches ; i++ ) mLumiSection->etSum[hlxID].data[i] += etSumNibble.data[i];

      // Cache OC0
      mLumiSection->hdr.bOC0 |= etSumNibble.hdr.bOC0;
      
      return;
      
    }

    // Report the timing error
    SetError("HLX data glitch detected for ET nibble - ignoring");
    cout << mLumiSection->hdr.startOrbit << endl;
    cout << etSumNibble.hdr.startOrbit << endl;
    cout << mNumOrbitsPerNibble << endl;

  }

  void SectionCollector::CommitLumiSection() {
    mSectionNumber++;

    // Do a quick verification on the number of nibbles
    if ( mCurNumNibbles == mNumNibblesPerSection * mNumHLXs * 3 ) { // x3 because of occ,lhc,et
      mNumCompleteLumiSections++;
    } else {
      mNumIncompleteLumiSections++;
    }
    
    // Do the luminosity calculation
#ifdef HCAL_HLX_SECTION_COLLECTOR_DEBUG
    cout << "Running LumiCalc" << endl;
#endif
    mLumiCalculator->DoCalc(*mLumiSection);
    
    for ( u32 i = 0 ; i != mDistributors.size() ; i++ ) {
      u32 nextIndex = mDistributors[i]->writeIndex;
      if ( nextIndex == HCAL_HLX_DISTRIBUTOR_BUFFER_DEPTH-1 ) {
	nextIndex = 0;
      } else {
	nextIndex++;
      }
      
#ifdef HCAL_HLX_SECTION_COLLECTOR_DEBUG
      cout << "Write index is: " << dec << mDistributors[i]->writeIndex << endl;
      cout << "Read index is: " << dec << mDistributors[i]->readIndex << endl;
      cout << "Next index is: " << dec << nextIndex << endl;
#endif
      if ( nextIndex == mDistributors[i]->readIndex ) {
#ifdef HCAL_HLX_SECTION_COLLECTOR_DEBUG
	cout << "Section dropped" << endl;
#endif
	mDistributors[i]->distributor->mNumLostLumiSections++;
      } else {
	// Copy the data into the buffer
#ifdef HCAL_HLX_SECTION_COLLECTOR_DEBUG
	cout << "Copying data into circular buffer" << endl;
#endif

	memcpy(mDistributors[i]->sectionBuffer+mDistributors[i]->writeIndex,
	       mLumiSection,
	       sizeof(LUMI_SECTION));  
	// Set the new write pipeline address
#ifdef HCAL_HLX_SECTION_COLLECTOR_DEBUG
	cout << "Submit" << endl;
#endif
	mDistributors[i]->writeIndex = nextIndex;
      }
    }
    
  }

  void SectionCollector::ProcessLHCNibble(const LHC_NIBBLE & lhcNibble,
					  u8 hlxID) {

    // Changed this to detect changes going backwards in time as well 

    // Conditions for synchronisation are either:
    // 1) TTC OC0 restart:
    //    a) Orbit start of nibble is less than or equal to previous lumi section (i.e. back in time)
    //    b) New nibble start orbit is zero
    //    c) HLX detected OC0
    //    Condition (a) is needed to prevent re-initialisation after the first OC0 from an HLX
    // 2) Normal operation:
    //    a) Orbit start of next nibble is greater than previous lumi section
    //    b) Orbit start of next nibble is an exact multiple of the lumi section 

    // The only caveat of this is that if an HLX glitches during running and jumps
    // it can knock out the rest of the DAQ during running, although an OC0 will resynchronise it

    // Transition condition checking
    // Glitch condition checking
    /*
    bool resyncCondition = ((lhcNibble.hdr.startOrbit == 0) && // resync condition
			    (mLumiSection->hdr.startOrbit != 0) &&
			    lhcNibble.hdr.bOC0);
    bool newLSCondition = ((mLumiSection->hdr.startOrbit < lhcNibble.hdr.startOrbit ) &&
			    (lhcNibble.hdr.startOrbit % mNumOrbits == 0));

    if ( resyncCondition || newLSCondition ) {
    
      // Move on to next lumi section as the previous one has to be complete by now...
      // But only commit if there is at least one nibble in the section for one HLX
      if ( mCurNumNibbles != 0 ) CommitLumiSection();
      
      // If resync condition, update the run number
      if ( resyncCondition ) {
	mCurrentRunNumber = mNextRunNumber;
	mResyncCount++;
      }
      
      // Initialise the lumi section
      InitialiseLumiSection();
      mLumiSection->hdr.startOrbit = lhcNibble.hdr.startOrbit;
      mLumiSection->hdr.bCMSLive = lhcNibble.hdr.bCMSLive;
      mLumiSection->hdr.bOC0 = false;
      
      // Load the first nibble
      mLumiSection->lhc[hlxID].hdr.numNibbles = 1;
      mCurNumNibbles = 1;
      
      // As this is the first nibble of the set, copy into array
      for ( u32 i = 0 ; i != mNumBunches ; i++ ) {
	// Accumulate the data
	mLumiSection->lhc[hlxID].data[i] = lhcNibble.data[i];
      }

      // Cache OC0
      mLumiSection->hdr.bOC0 |= lhcNibble.hdr.bOC0;
      
      return;
      
    }
    */

    bool normalCondition = ((mLumiSection->hdr.startOrbit <= lhcNibble.hdr.startOrbit) &&
			    (lhcNibble.hdr.startOrbit % mNumOrbitsPerNibble == 0));

    // In normal running (accumulation)
    if ( normalCondition ) {

      // Part of recording sequence, accumulate data
      // Check for missing nibbles - we assume that nibbles cannot arrive out of order
      // However the packets comprising them can - this is already handled by the nibble collector
      // This amounts to assuming that all the data is transmitted and received
      // during a single histogramming period. If not, we'd end up with all sort of problems
      // such as packet collisions etc. which would break the system anyway...
      
      // Add to the nibble count
      mLumiSection->lhc[hlxID].hdr.numNibbles++;
      mCurNumNibbles++;

      // Accumulate the data
      for ( u32 i = 0 ; i != mNumBunches ; i++ ) {
	// Accumulate the data
	mLumiSection->lhc[hlxID].data[i] += lhcNibble.data[i];
      }

      // Cache OC0
      mLumiSection->hdr.bOC0 |= lhcNibble.hdr.bOC0;

      return;

    }
      
    // To get here implies an error in the data pattern
    SetError("HLX data glitch detected for LHC nibble - ignoring");

  }

  u32 SectionCollector::GetResyncCount() {
    return mResyncCount;
  }

  u32 SectionCollector::GetCurrentRunNumber() {
    return mCurrentRunNumber;
  }

  void SectionCollector::ProcessOccupancyNibble(const OCCUPANCY_NIBBLE & occupancyNibble,
						u8 hlxID) {


    // Changed this to detect changes going backwards in time as well 

    // Conditions for synchronisation are either:
    // 1) TTC OC0 restart:
    //    a) Orbit start of nibble is less than or equal to previous lumi section (i.e. back in time)
    //    b) New nibble start orbit is zero
    //    c) HLX detected OC0
    //    Condition (a) is needed to prevent re-initialisation after the first OC0 from an HLX
    // 2) Normal operation:
    //    a) Orbit start of next nibble is greater than previous lumi section
    //    b) Orbit start of next nibble is an exact multiple of the lumi section 

    // The only caveat of this is that if an HLX glitches during running and jumps
    // it can knock out the rest of the DAQ during running, although an OC0 will resynchronise it

    // Section synchronisation
    bool resyncCondition = ((occupancyNibble.hdr.startOrbit == 0) && // resync condition
			    (hlxID == 0));// && // needed to stop multiple resyncs - keyed to 1st HLX 
    //occupancyNibble.hdr.bOC0);
    bool newLSCondition = ((mLumiSection->hdr.startOrbit < occupancyNibble.hdr.startOrbit) &&
			   (occupancyNibble.hdr.startOrbit % mNumOrbits == 0));

    if ( resyncCondition || newLSCondition ) {

      // Move on to next lumi section as the previous one has to be complete by now...
      // But only commit if there is at least one nibble in the section for one HLX
      if ( mCurNumNibbles != 0 ) {
	CommitLumiSection();
      }

      // If resync condition, update the run number
      if ( resyncCondition ) {
	if ( mCurrentRunNumber != mNextRunNumber ) {
	  mCurrentRunNumber = mNextRunNumber;
	  mSectionNumber = 1;
	}
	mResyncCount++;
      }

      // Initialise the lumi section
      InitialiseLumiSection();
      mLumiSection->hdr.startOrbit = occupancyNibble.hdr.startOrbit;
      mLumiSection->hdr.bCMSLive = occupancyNibble.hdr.bCMSLive;
      mLumiSection->hdr.bOC0 = false;

      // Load the first nibble
      mLumiSection->occupancy[hlxID].hdr.numNibbles = 1;
      mCurNumNibbles = 1;

      // As this is the first nibble of the set, copy into array
      for ( u32 j = 0 ; j != 6 ; j++ ) {
	for ( u32 i = 0 ; i != mNumBunches ; i++ ) {
	  // Accumulate the data
	  mLumiSection->occupancy[hlxID].data[j][i] = occupancyNibble.data[j][i];
	}
      }

      // Cache OC0
      mLumiSection->hdr.bOC0 |= occupancyNibble.hdr.bOC0;
      
      return;

    }

    // Transition condition checking
    bool normalCondition = ((mLumiSection->hdr.startOrbit <= occupancyNibble.hdr.startOrbit) &&
			   (occupancyNibble.hdr.startOrbit % mNumOrbitsPerNibble == 0));

    // In normal running (accumulation)
    if ( normalCondition ) {

      // Part of recording sequence, accumulate data
      // Check for missing nibbles - we assume that nibbles cannot arrive out of order
      // However the packets comprising them can - this is already handled by the nibble collector
      // This amounts to assuming that all the data is transmitted and received
      // during a single histogramming period. If not, we'd end up with all sort of problems
      // such as packet collisions etc. which would break the system anyway...
      
      // Add to the nibble count
      mLumiSection->occupancy[hlxID].hdr.numNibbles++;
      mCurNumNibbles++;

      // Accumulate the data
      for ( u32 j = 0 ; j != 6 ; j++ ) {
	for ( u32 i = 0 ; i != mNumBunches ; i++ ) {
	  // Accumulate the data
	  mLumiSection->occupancy[hlxID].data[j][i] += occupancyNibble.data[j][i];
	}
      }

      // Cache OC0
      mLumiSection->hdr.bOC0 |= occupancyNibble.hdr.bOC0;

      return;
    }

    // To get here implies an error in the data pattern
    SetError("HLX data glitch detected for occupancy nibble - ignoring");

  }


} // namespace HCAL_HLX
