/*
  Nibble collector class
  Copyright John Jones 2007
*/

// Prototype class definition
#include "NibbleCollector.hh"

// Networking
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// Threading (ah hell now we're in trouble...)
#include <pthread.h>

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
  NibbleCollector::NibbleCollector(u32 aNumHLXs,
				   u16 localPort,
				   char sourceAddress[]) {
    try {
      mUdpSocket=-1;
      mThreadId=0;
      mNumHLXs = aNumHLXs;

      // Initialise the data structures
      this->Init();

      // Reset the data structures
      this->Reset();

      // Assign the UDP socket
      mUdpSocket = socket(PF_INET,SOCK_DGRAM,0);
      if ( mUdpSocket == -1 ) {
	HardwareAccessException lExc("Could not open socket");
	RAISE(lExc);
      }

      // Bind the socket to the local port / address
      sockaddr_in sa_local;
      sa_local.sin_port = ((localPort&0xFF)<<8)|((localPort&0xFF00)>>8); // endian swap...
      sa_local.sin_addr.s_addr = inet_addr(sourceAddress);
      sa_local.sin_family = AF_INET;

      // Has to be done before bind!!!
      unsigned long tempNum = 1000000;
      socklen_t txa = 4;
      setsockopt(mUdpSocket,SOL_SOCKET,SO_RCVBUF,
		 &tempNum,txa);
      getsockopt(mUdpSocket,SOL_SOCKET,SO_RCVBUF,
		 &tempNum,&txa);
      cout << dec << "SO_RCVBUF set to " << tempNum << endl;

      // Bind to a local port to receive data
      int ret = bind(mUdpSocket,(struct sockaddr *)&sa_local,sizeof(sa_local));
      if ( ret == -1 ) {
	HardwareAccessException lExc("Socket could not be bound to local port");
	RAISE(lExc);
      }

      // Initialise the worker thread
      mWorkerThreadContinue=true;
      mWriteBufferPointer = 0;
      mReadBufferPointer = 0;
      pthread_mutex_init(&mDataMutex,0);

      // Thread attribute initialisation
      pthread_attr_t attr;
      ret = pthread_attr_init(&attr);
      if ( ret != 0 ) {
      	MemoryAllocationException lExc("Cannot initialise worker thread attributes");
      	RAISE(lExc);
      }

      // Thread priority initialisation

      //schedparam.sched_priority = sched_get_priority_max(SCHED_FIFO);

      //pthread_attr_setschedpolicy(&attr,SCHED_FIFO);
      //pthread_attr_setinheritsched(&attr,PTHREAD_EXPLICIT_SCHED);
      //pthread_attr_setschedparam(&attr,&schedparam);


      //ret = pthread_attr_setstacksize(&attr,stacksize);

      // Create the worker thread
      ret = pthread_create(&mThreadId,
			   NULL,
			   (void*(*)(void*))NibbleCollector::WorkerThread,
			   reinterpret_cast<void *>(this));
      if ( ret != 0 ) {
	MemoryAllocationException lExc("Cannot create worker thread");
	RAISE(lExc);
      }

      int policy;
      sched_param schedparam;
      policy = SCHED_FIFO;
      cout << "Thread policy set to SCHED_FIFO" << endl;
      schedparam.sched_priority = sched_get_priority_max(SCHED_FIFO);
      cout << "Thread priority set to " << schedparam.sched_priority << endl;
      pthread_setschedparam(mThreadId,
			    policy,
			    &schedparam);
      pthread_getschedparam(mThreadId,
			    &policy,
			    &schedparam);
      cout << "Policy is now: " << policy << endl;
      cout << "Priority is now: " << schedparam.sched_priority << endl;

    } catch (ICException & aExc) {
      RETHROW(aExc);
    }
  }

  // Destructor deletes the hardware interface
  NibbleCollector::~NibbleCollector() {
    try {
      mWorkerThreadContinue=false;
      if ( mUdpSocket != -1 ) {
	// Release the socket
	shutdown(mUdpSocket,SHUT_RDWR);
	mUdpSocket=-1;
      }
      pthread_join(mThreadId,NULL);
      //cout << "rejoined" << endl;a
      mThreadId=0;
      pthread_mutex_destroy(&mDataMutex);
      this->CleanUp();
      //cout << "cleanup" << endl;
    } catch (ICException & aExc) {
      RETHROW(aExc);
    }
  }

  // Initialisation function
  void NibbleCollector::Init() {
    try {
      mETSumNibbles = 0;
      mETSumDataCounters = 0;
      mLHCNibbles = 0;
      mLHCDataCounters = 0;
      mOccupancyNibbles = 0;
      mOccupancyDataCounters = 0;

      // Initialise the nibble data structures
      mETSumNibbles = new ET_SUM_NIBBLE[mNumHLXs];
      if ( mETSumNibbles == 0 ) {
	MemoryAllocationException lExc("ET sum nibble");
	RAISE(lExc);
      }
      mETSumDataCounters = new u32[mNumHLXs];
      if ( mETSumDataCounters == 0 ) {
	MemoryAllocationException lExc("ET sum data counters");
	RAISE(lExc);
      }
      mLHCNibbles = new LHC_NIBBLE[mNumHLXs];
      if ( mLHCNibbles == 0 ) {
	MemoryAllocationException lExc("LHC nibble");
	RAISE(lExc);
      }
      mLHCDataCounters = new u32[mNumHLXs];
      if ( mLHCDataCounters == 0 ) {
	MemoryAllocationException lExc("LHC data counters");
	RAISE(lExc);
      }
      mOccupancyDataCounters = new (u32 *)[mNumHLXs];
      if ( mOccupancyDataCounters == 0 ) {
	MemoryAllocationException lExc("Occupancy data counters");
	RAISE(lExc);
      }
      mOccupancyNibbles = new OCCUPANCY_NIBBLE[mNumHLXs];
      if ( mOccupancyNibbles == 0 ) {
	MemoryAllocationException lExc("Occupancy nibbles");
      }
      for ( u32 i = 0 ; i != mNumHLXs ; i++ ) {
	mOccupancyDataCounters[i] = new u32[6];
	if ( mOccupancyDataCounters == 0 ) {
	  MemoryAllocationException lExc("Occupancy data counters");
	  RAISE(lExc);
	}
      }

#ifdef HCAL_HLX_U8_BUFFER
      // This has to be dynamically allocated or it won't work
      circularBuffer = new HLX_CB_TYPE[256];
      if ( circularBuffer == 0 ) {
	MemoryAllocationException lExc("Circular buffer");
	RAISE(lExc);
      }

      // Initialise the array
      for ( u32 i = 0 ; i != 256 ; i++ ) {
	circularBuffer[i].data = 0;
      }

      // Allocate the circular buffer
      for ( u32 i = 0 ; i != 256 ; i++ ) {
	circularBuffer[i].data = new u8[1500];
	if ( circularBuffer[i].data == 0 ) {
	  MemoryAllocationException lExc("Circular buffer");
	  RAISE(lExc);
	}
      }
#else
     // This has to be dynamically allocated or it won't work
      circularBuffer = new HLX_CB_TYPE[65536];
      if ( circularBuffer == 0 ) {
	MemoryAllocationException lExc("Circular buffer");
	RAISE(lExc);
      }

      // Initialise the array
      for ( u32 i = 0 ; i != 65536 ; i++ ) {
	circularBuffer[i].data = 0;
      }

      // Allocate the circular buffer
      for ( u32 i = 0 ; i != 65536 ; i++ ) {
	circularBuffer[i].data = new u8[1500];
	if ( circularBuffer[i].data == 0 ) {
	  MemoryAllocationException lExc("Circular buffer");
	  RAISE(lExc);
	}
      }
#endif

      // Initialise the CRC table
      this->InitialiseChecksum();

    } catch (ICException & aExc) {
      RETHROW(aExc);
    }
  }
  
  // Clean up function
  void NibbleCollector::CleanUp() {
    try {

      // Nibbles
      for ( u32 i = 0 ; i != mNumHLXs ; i++ ) {
	delete []mOccupancyDataCounters[i];
      }
      delete []mOccupancyNibbles;
      mOccupancyNibbles = 0;
      delete []mOccupancyDataCounters;
      mOccupancyDataCounters = 0;
      delete []mETSumDataCounters;
      mETSumDataCounters = 0;
      delete []mETSumNibbles;
      mETSumNibbles = 0;
      delete []mLHCDataCounters;
      mLHCDataCounters = 0;
      delete []mLHCNibbles;
      mLHCNibbles = 0;

#ifdef HCAL_HLX_U8_BUFFER
      // Circular buffer
      for ( u32 i = 0 ; i != 256 ; i++ ) {
	if ( circularBuffer[i].data ) {
	  delete []circularBuffer[i].data;
	  circularBuffer[i].data = 0;
	}
      }
      if ( circularBuffer ) {
	delete []circularBuffer;
	circularBuffer = 0;
      }
#else
      // Circular buffer
      for ( u32 i = 0 ; i != 65536 ; i++ ) {
	if ( circularBuffer[i].data ) {
	  delete []circularBuffer[i].data;
	  circularBuffer[i].data = 0;
	}
      }
      if ( circularBuffer ) {
	delete []circularBuffer;
	circularBuffer = 0;
      }
#endif

      // CRC table
      for ( u32 i = 0 ; i != 256 ; i++ ) {
	if ( crcTable[i] ) {
	  delete []crcTable;
	  crcTable[i] = 0;
	}
      }
      if ( crcTable ) {
	delete []crcTable;
	crcTable = 0;
      }

    } catch (ICException & aExc) {
      RETHROW(aExc);
    }
  }

  void NibbleCollector::WorkerThread(void *thisPtr) {
    // Poll for data here    
    // No exceptions, just plain-vanilla C++
    NibbleCollector *theClass = reinterpret_cast<NibbleCollector *>(thisPtr);
    fd_set fds;
    struct timeval tv;
    u8 rData[1500];
    int mUdpSocket=theClass->mUdpSocket;
    int ret;

    while (theClass->mWorkerThreadContinue) {
  
      // Sock is an intialized socket handle
      tv.tv_sec = 0;
      tv.tv_usec = 0;
      
      // Reset the descriptors
      FD_ZERO(&fds);
      FD_SET(mUdpSocket, &fds); // adds sock to the file descriptor set
      
      // See if a packet is available
      select(mUdpSocket+1, &fds, NULL, NULL, &tv);

      // Check to see if a packet is ready
      if (FD_ISSET(mUdpSocket, &fds)) {
	//pthread_mutex_lock(&theClass->mDataMutex);

	// Grab a packet if one is available
	// Need to hack this as a static u8 otherwise it treats it as a u32...
#ifdef HCAL_HLX_U8_BUFFER
	if ( static_cast<u8>(theClass->mWriteBufferPointer+1) == theClass->mReadBufferPointer ) {
#else
	if ( static_cast<u16>(theClass->mWriteBufferPointer+1) == theClass->mReadBufferPointer ) {
#endif
	  // About to overflow
	  // Read the packet and dump it and flag the counter
	  ret = recv(mUdpSocket,rData,1500,0);
	  if ( ret == -1 ) {
	    //HardwareAccessException lExc("Unable to get data from UDP socket");
	    //RAISE(lExc);
	    cout << "ERROR in " << __PRETTY_FUNCTION__ << endl;
	  } else {
	    theClass->mNumLostPackets++;
	  }
	} else {
	  ret = recv(mUdpSocket,theClass->circularBuffer[theClass->mWriteBufferPointer].data,1500,0);
	  if ( ret == -1 ) {
	    //HardwareAccessException lExc("Unable to get data from UDP socket");
	    //RAISE(lExc);
	    cout << "ERROR in " << __PRETTY_FUNCTION__ << endl;
	  } else {

	    // Update the packet length
	    theClass->circularBuffer[theClass->mWriteBufferPointer].len = ret;
	    // Increment the data counter
	    theClass->mTotalDataVolume+=ret;
	    // Increment the circular buffer pointer
	    theClass->mWriteBufferPointer++;

	  }
	}

	//pthread_mutex_unlock(&theClass->mDataMutex);
      }
    }

    //cout << "Worker thread complete" << endl;
  }

  // Reset function
  void NibbleCollector::Reset() {
    try {
      mNumGoodPackets=0;
      mNumBadPackets=0;
      mNumLostPackets=0;
      mTotalDataVolume=0.0;
      mNumGoodETSumNibbles=0;
      mNumBadETSumNibbles=0;
      mNumGoodOccupancyNibbles=0;
      mNumBadOccupancyNibbles=0;
      mNumGoodLHCNibbles=0;
      mNumBadLHCNibbles=0;
      for ( u32 j = 0 ; j != mNumHLXs ; j++ ) {
	mETSumDataCounters[j] = 0;
	memset(&mETSumNibbles[j],0,sizeof(ET_SUM_NIBBLE));
	mLHCDataCounters[j] = 0;
	memset(&mLHCNibbles[j],0,sizeof(LHC_NIBBLE));
	for ( u32 i = 0 ; i != 6 ; i++ ) {
	  mOccupancyDataCounters[j][i] = 0;
	}
	memset(&mOccupancyNibbles[j],0,sizeof(OCCUPANCY_NIBBLE));
      }
    } catch (ICException & aExc) {
      RETHROW(aExc);
    }
  }

  // Statistics registers
  u16 NibbleCollector::GetWriteBufferPointer() {
    return static_cast<u16>(mWriteBufferPointer);
  }
  u16 NibbleCollector::GetReadBufferPointer() {
    return static_cast<u16>(mReadBufferPointer);
  }
  u32 NibbleCollector::GetNumGoodPackets() {
    return mNumGoodPackets;
  }
  u32 NibbleCollector::GetNumBadPackets() {
    return mNumBadPackets;
  }
  u32 NibbleCollector::GetNumLostPackets() {
    return mNumLostPackets;
  }
  double NibbleCollector::GetTotalDataVolume() {
    return mTotalDataVolume;
  }
  u32 NibbleCollector::GetNumGoodETSumNibbles() {
    return mNumGoodETSumNibbles;
  }
  u32 NibbleCollector::GetNumBadETSumNibbles() {
    return mNumBadETSumNibbles;
  }
  u32 NibbleCollector::GetNumGoodLHCNibbles() {
    return mNumGoodLHCNibbles;
  }
  u32 NibbleCollector::GetNumBadLHCNibbles() {
    return mNumBadLHCNibbles;
  }
  u32 NibbleCollector::GetNumGoodOccupancyNibbles() {
    return mNumGoodOccupancyNibbles;
  }
  u32 NibbleCollector::GetNumBadOccupancyNibbles() {
    return mNumBadOccupancyNibbles;
  }

  // LumiSection collector attachment
  void NibbleCollector::AttachSectionCollector(AbstractSectionCollector *sectionCollector) {
    try {
      // Add the section collector to the list
      this->mSectionCollectors.push_back(sectionCollector);
    } catch (ICException & aExc) {
      RETHROW(aExc);
    }
  }

  // Service handler
  // This could be an interlock with a service thread, if needed
  void NibbleCollector::RunServiceHandler() {
    try {
      //pthread_mutex_lock(&mDataMutex);
      while (mWriteBufferPointer!=mReadBufferPointer) {
	// Validate the checksum
	if ( this->ValidateChecksum(circularBuffer[mReadBufferPointer].data,
				    circularBuffer[mReadBufferPointer].len-1) ) {
	  // Increment the good packet counter
	  mNumGoodPackets++;
	  // Map the lumi header
	  LUMI_RAW_HEADER *lumiHdr = reinterpret_cast<LUMI_RAW_HEADER *>(circularBuffer[mReadBufferPointer].data);
	  
	  // If the checksum is valid, process the packet
	  this->ProcessPacket(lumiHdr,
			      circularBuffer[mReadBufferPointer].data+sizeof(LUMI_RAW_HEADER),
			      circularBuffer[mReadBufferPointer].len-1-sizeof(LUMI_RAW_HEADER));
	  } else {
	  // Increment the bad packet counter
	  //cout << "bad checksum" << endl;
	    mNumBadPackets++;
	  }
      	mReadBufferPointer++;
      }
      //pthread_mutex_unlock(&mDataMutex);
    } catch (ICException & aExc) {
      //pthread_mutex_unlock(&mDataMutex);
      RETHROW(aExc);
    }
  }

  void NibbleCollector::ProcessPacket(const LUMI_RAW_HEADER *lumiHdr,
				      const u8 *data,
				      u32 nBytes) {
    //	return;
    try {
      u8 hlxID = lumiHdr->hlxID;

      if ( hlxID < mNumHLXs ) {
	if ( lumiHdr->histogramSet == 7 ) {
	  // ET sum histogram

	  // First check to see if the start orbit matches the
	  // previous one, otherwise we discard
	  if ( lumiHdr->startOrbit != mETSumNibbles[hlxID].hdr.startOrbit ) {
	    // Check for incomplete histogram
	    if ( mETSumNibbles[hlxID].hdr.startOrbit != 0 ) {
	      if ( mETSumNibbles[hlxID].bIsComplete == false ) { 
		cout << "Incomplete ET sum nibble" << endl;
		cout << "HLX ID: " << static_cast<u16>(hlxID) << endl;
		mNumBadETSumNibbles++;
	      } else {
		// LHC nibble is good, ship it...
		mNumGoodETSumNibbles++;
		// Push the nibble into the section collector whether good or bad
		//cout << "Before process et nibble, HLX " << static_cast<u16>(hlxID) << endl;
		for ( u32 i = 0 ; i != mSectionCollectors.size() ; i++ ) {
		  mSectionCollectors[i]->ProcessETSumNibble(mETSumNibbles[hlxID],
							    hlxID);
		}
		//cout << "After process et nibble" << endl;
	      }
	    }
	    
	    // Clear data counter and reset startOrbit marker
	    mETSumDataCounters[hlxID] = 0;
	    mETSumNibbles[hlxID].hdr.startOrbit = lumiHdr->startOrbit;
	    mETSumNibbles[hlxID].hdr.numBunches = lumiHdr->numBunches+1;
	    mETSumNibbles[hlxID].hdr.numOrbits = lumiHdr->numOrbits+1;
	    mETSumNibbles[hlxID].bIsComplete = false;
	    // TODO: bcmslive
	  }
	  
	  // Copy the data into the local buffer
	  // TODO - modify to allow unordered packets (use startBunch)
	  memcpy(mETSumNibbles[hlxID].data + mETSumDataCounters[hlxID],
		 data,
		 nBytes);
	  
	  // Update the data counter and check for completion
	  mETSumDataCounters[hlxID] += nBytes >> 2; // Divide by 4 for longs
	  if ( mETSumDataCounters[hlxID] == mETSumNibbles[hlxID].hdr.numBunches ) {
	    mETSumNibbles[hlxID].bIsComplete = true;
	  }
	  
	} else if ( lumiHdr->histogramSet == 6 ) {

	  // Occupancy histograms
	  // First check to see if the start orbit matches the previous one, otherwise we discard
	  if ( lumiHdr->startOrbit != mLHCNibbles[hlxID].hdr.startOrbit ) {
	    
	    // Check for incomplete histogram
	    if ( mLHCNibbles[hlxID].hdr.startOrbit != 0 ) {
	      if ( mLHCNibbles[hlxID].bIsComplete == false ) {
		cout << "Incomplete LHC nibble" << endl;
		cout << "HLX ID: " << static_cast<u16>(hlxID) << endl;
		mNumBadLHCNibbles++;
	      } else {
		// LHC nibble is good, ship it...
		mNumGoodLHCNibbles++;
		// Push the nibble into the section collector whether good or bad
		for ( u32 i = 0 ; i != mSectionCollectors.size() ; i++ ) {
		  mSectionCollectors[i]->ProcessLHCNibble(mLHCNibbles[hlxID],
							  hlxID);
		}
	      }
	    }
	    
	    // Clear data counter and reset startOrbit marker
	    mLHCDataCounters[hlxID] = 0;
	    mLHCNibbles[hlxID].hdr.startOrbit = lumiHdr->startOrbit;
	    mLHCNibbles[hlxID].hdr.numBunches = lumiHdr->numBunches+1;
	    mLHCNibbles[hlxID].hdr.numOrbits = lumiHdr->numOrbits+1;
	    mLHCNibbles[hlxID].bIsComplete = false;
	  }
	  
	  // Copy the data into the local buffer
	  memcpy(mLHCNibbles[hlxID].data + mLHCDataCounters[hlxID],
		 data,
		 nBytes);
	  
	  // Update the data counter and check for completion
	  mLHCDataCounters[hlxID] += nBytes >> 1; // Divide by 2 for shorts
	  if ( mLHCDataCounters[hlxID] == mLHCNibbles[hlxID].hdr.numBunches ) {
	    mLHCNibbles[hlxID].bIsComplete = true;
	  }

	} else if ( lumiHdr->histogramSet < 6 ) {
	  // Occupancy histograms
	  // First check to see if the start orbit matches the previous one, otherwise we discard
	  if ( lumiHdr->startOrbit != mOccupancyNibbles[hlxID].hdr.startOrbit ) {
	    
	    // Check for incomplete histogram
	    if ( mOccupancyNibbles[hlxID].hdr.startOrbit != 0 ) {
	      bool bAllComplete = true;
	      for ( u32 i = 0 ; i != 6 ; i++ ) {
		if ( !mOccupancyNibbles[hlxID].bIsComplete[i] ) {
		  cout << "Incomplete occupancy nibble" << endl;
		  cout << "HLX ID: " << dec << static_cast<u16>(hlxID) << endl;
		  cout << "Occupancy component: " << i << endl;
		  bAllComplete = false;
		  break;
		}
	      }
	      if ( bAllComplete ) {
		// Push the nibble into the section collector whether good or bad
		for ( u32 i = 0 ; i != mSectionCollectors.size() ; i++ ) {
		  mSectionCollectors[i]->ProcessOccupancyNibble(mOccupancyNibbles[hlxID],
								hlxID);
		}
		mNumGoodOccupancyNibbles++;
	      } else {
		mNumBadOccupancyNibbles++;
	      }
	    }
	    
	    mOccupancyNibbles[hlxID].hdr.startOrbit = lumiHdr->startOrbit;
	    mOccupancyNibbles[hlxID].hdr.numBunches = lumiHdr->numBunches+1;
	    mOccupancyNibbles[hlxID].hdr.numOrbits = lumiHdr->numOrbits+1;	    

	    // Clear data counter and reset startOrbit marker
	    for ( u32 i = 0 ; i != 6 ; i++ ) {
	      mOccupancyDataCounters[hlxID][i] = 0;
	      mOccupancyNibbles[hlxID].bIsComplete[i] = false;
	    }
	  }
	  
	  // Copy the data into the local buffer
	  memcpy(mOccupancyNibbles[hlxID].data[lumiHdr->histogramSet] + mOccupancyDataCounters[hlxID][lumiHdr->histogramSet],
		 data,
		 nBytes);
	  
	  // Update the data counter and check for completion
	  mOccupancyDataCounters[hlxID][lumiHdr->histogramSet] += nBytes >> 1; // Divide by 2 for shorts
	  if ( mOccupancyDataCounters[hlxID][lumiHdr->histogramSet] == mOccupancyNibbles[hlxID].hdr.numBunches ) {
	    mOccupancyNibbles[hlxID].bIsComplete[lumiHdr->histogramSet] = true;
	  }
	  
	} else {
	  cout << "Bad histogram set" << endl;
	  mNumBadPackets++;	
	}
      } else {
	cout << "Bad HLX ID" << dec << static_cast<u16>(lumiHdr->hlxID) << "\t" << mNumHLXs << endl;
	mNumBadPackets++;
      }

    } catch (ICException & aExc) {
      RETHROW(aExc);
    }
  }

  bool NibbleCollector::ValidateChecksum(const u8 *data, u32 numBytes) {
    u8 computedChecksum = ComputeChecksum(data,numBytes);
    return (computedChecksum == data[numBytes]);
  }

  u8 NibbleCollector::ComputeChecksum(const u8 *data, u32 numBytes) {
    u8 theCRC = 0xAA;
    for ( u32 i = 0 ; i != numBytes ; i++ ) {
      theCRC=SingleChecksum(data[i],theCRC);
    }
    return theCRC;
  }

  void NibbleCollector::InitialiseChecksum() {
    // This has to be dynamically allocated or it won't work
    crcTable = new (u8 *)[256];
    if ( crcTable == 0 ) {
      MemoryAllocationException lExc("CRC table");
      RAISE(lExc);
    }
    
    // Initialise the array
    for ( u32 i = 0 ; i != 256 ; i++ ) {
      crcTable[i] = 0;
    }
    
    // Allocate the circular buffer
    for ( u32 i = 0 ; i != 256 ; i++ ) {
      crcTable[i] = new u8[256];
      if ( crcTable[i] == 0 ) {
	this->CleanUp();
	MemoryAllocationException lExc("CRC table");
	RAISE(lExc);
      }
    }
    
    for ( u32 i = 0 ; i != 256 ; i++ ) {
      for ( u32 j = 0 ; j != 256 ; j++ ) {
	crcTable[i][j] = ChecksumHelper(i,j);
      }
    }
  }

  u8 NibbleCollector::SingleChecksum(u8 a, u8 b) {
    return crcTable[a][b];
  }

  u8 NibbleCollector::ChecksumHelper(u8 data, u8 prevCRC) {
    u8 ret = 0;
    bool dataBits[8], prevCRCBits[8], nextCRCBits[8];
    
    for ( int i = 0 ; i != 8 ; i++ ) {
      dataBits[i] = (data & (0x1 << i)) ? true : false;
      prevCRCBits[i] = (prevCRC & (0x1 << i)) ? true : false; 
    }
    
    nextCRCBits[0] = dataBits[7] ^ dataBits[6] ^ dataBits[0] 
      ^ prevCRCBits[0] ^ prevCRCBits[6] ^ prevCRCBits[7];
    nextCRCBits[1] = dataBits[6] ^ dataBits[1] ^ dataBits[0] 
      ^ prevCRCBits[0] ^ prevCRCBits[1] ^ prevCRCBits[6];
    nextCRCBits[2] = dataBits[6] ^ dataBits[2] ^ dataBits[1] ^ dataBits[0] 
      ^ prevCRCBits[0] ^ prevCRCBits[1] ^ prevCRCBits[2] ^ prevCRCBits[6];
    nextCRCBits[3] = dataBits[7] ^ dataBits[3] ^ dataBits[2] ^ dataBits[1] 
      ^ prevCRCBits[1] ^ prevCRCBits[2] ^ prevCRCBits[3] ^ prevCRCBits[7];
    nextCRCBits[4] = dataBits[4] ^ dataBits[3] ^ dataBits[2]
      ^ prevCRCBits[2] ^ prevCRCBits[3] ^ prevCRCBits[4];
    nextCRCBits[5] = dataBits[5] ^ dataBits[4] ^ dataBits[3]
      ^ prevCRCBits[3] ^ prevCRCBits[4] ^ prevCRCBits[5];
    nextCRCBits[6] = dataBits[6] ^ dataBits[5] ^ dataBits[4]
      ^ prevCRCBits[4] ^ prevCRCBits[5] ^ prevCRCBits[6];
    nextCRCBits[7] = dataBits[7] ^ dataBits[6] ^ dataBits[5]
      ^ prevCRCBits[5] ^ prevCRCBits[6] ^ prevCRCBits[7];
    
    for ( int i = 0 ; i != 8 ; i++ ) {
      ret += nextCRCBits[i]?(1<<i):0;
    }
    
    return ret;
  }

} // namespace HCAL_HLX
