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
				   u16 aNumBunches,
				   u16 aNumOrbits,
				   u16 localPort,
				   const std::string & sourceAddress) {
    mUdpSocket=-1;
    mWorkerThreadId = 0;
    mServiceThreadId=0;
    mNumHLXs = aNumHLXs;
    mNumBunches = aNumBunches;
    mNumOrbits = aNumOrbits;
    m32BitNibbleSize = aNumBunches * sizeof(u32);
    m16BitNibbleSize = aNumBunches * sizeof(u16);
    
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
    if ( sourceAddress == "BROADCAST" ) {
      sa_local.sin_addr.s_addr = INADDR_BROADCAST;
    } else {
      sa_local.sin_addr.s_addr = inet_addr(sourceAddress.c_str());
    }
    sa_local.sin_family = AF_INET;
    
    // Has to be done before bind!!!
    unsigned long tempNum = 1000000;
    socklen_t txa = 4;
    setsockopt(mUdpSocket,SOL_SOCKET,SO_RCVBUF,
	       &tempNum,txa);
    getsockopt(mUdpSocket,SOL_SOCKET,SO_RCVBUF,
	       &tempNum,&txa);
    //2Acout << dec << "SO_RCVBUF set to " << tempNum << endl;
    
    // Allow socket to be re-bound
    int val=1;
    setsockopt(mUdpSocket,SOL_SOCKET,SO_REUSEADDR,&val,sizeof(val));
    
    // Enable broadcast packet decoding
    // Should really check if this fails
    if ( sourceAddress == "BROADCAST" ) {
      //int reaa = 
      setsockopt(mUdpSocket,SOL_SOCKET,SO_BROADCAST,&val,sizeof(val));
      getsockopt(mUdpSocket,SOL_SOCKET,SO_BROADCAST,&val,&txa);
      //cout << "socket broadcast " << reaa << "\t" <<  val << endl;
    }
    
    // Bind to a local port to receive data
    int ret = bind(mUdpSocket,(struct sockaddr *)&sa_local,sizeof(sa_local));
    if ( ret == -1 ) {
      HardwareAccessException lExc("Socket could not be bound to local port");
      RAISE(lExc);
    }

    // Error mutex initialisation
    pthread_mutex_init(&mErrorMutex,
		       NULL);

    // Set initialisation flag
    SetError("System initialised");
    
  }

  void NibbleCollector::Start() {
    // Don't start if already started
    // Consider exception? Perhaps not
    if ( mWorkerThreadId != 0 ) return;

    // Reset the data structures
    Reset();

    // Initialise the worker thread
    mWorkerThreadContinue=true;
    mWriteBufferPointer = 0;
    mReadBufferPointer = 0;
    
    // Thread attribute initialisation
    pthread_attr_t attr;
    int ret = pthread_attr_init(&attr);
    if ( ret != 0 ) {
      MemoryAllocationException lExc("Cannot initialise worker thread attributes");
      RAISE(lExc);
    }
    
    // Create the worker thread
    ret = pthread_create(&mWorkerThreadId,
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
    pthread_setschedparam(mWorkerThreadId,
			  policy,
			  &schedparam);
    pthread_getschedparam(mWorkerThreadId,
			  &policy,
			  &schedparam);
    cout << "Policy is now: " << policy << endl;
    cout << "Priority is now: " << schedparam.sched_priority << endl;

    // Service thread
    cout << "Starting service thread" << endl;
    mServiceThreadContinue=true;
    ret = pthread_create(&mServiceThreadId,
			 NULL,
			 (void*(*)(void*))NibbleCollector::ServiceThread,
			 reinterpret_cast<void *>(this));
    if ( ret != 0 ) {
      MemoryAllocationException lExc("Cannot create service thread");
      RAISE(lExc);
    }

  }
  
  void NibbleCollector::Stop() {
    
    if ( mWorkerThreadId != 0 ) {
      // Worker thread shutdown
      mWorkerThreadContinue=false;
      pthread_join(mWorkerThreadId,NULL);
      mWorkerThreadId = 0;
    }

    if ( mServiceThreadId != 0 ) {
      // Service thread shutdown
      mServiceThreadContinue=false;
      pthread_join(mServiceThreadId,NULL);
      mServiceThreadId = 0;
    }

  }

  // Destructor deletes the hardware interface
  NibbleCollector::~NibbleCollector() {
    try {

      if ( mWorkerThreadContinue ) Stop();

      if ( mUdpSocket != -1 ) {
	// Release the socket
	shutdown(mUdpSocket,SHUT_RDWR);
	//close(mUdpSocket);
	mUdpSocket=-1;
      }

      // Clean the data structures
      CleanUp();

      // Destroy the error mutex
      pthread_mutex_destroy(&mErrorMutex);

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
      mOccupancyNibbles = new OCCUPANCY_NIBBLE[mNumHLXs];
      if ( mOccupancyNibbles == 0 ) {
	MemoryAllocationException lExc("Occupancy nibbles");
      }
      mOccupancyDataCounters = new u32 *[mNumHLXs];
      if ( mOccupancyDataCounters == 0 ) {
	MemoryAllocationException lExc("Occupancy data counters");
	RAISE(lExc);
      }
      for ( u32 i = 0 ; i != mNumHLXs ; i++ ) {
	mOccupancyDataCounters[i] = new u32[6];
	if ( mOccupancyDataCounters == 0 ) {
	  MemoryAllocationException lExc("Occupancy data counters");
	  RAISE(lExc);
	}
      }

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

      // Initialise the CRC table
      this->InitialiseChecksum();

    } catch (ICException & aExc) {
      RETHROW(aExc);
    }
  }
  
  // Clean up function
  void NibbleCollector::CleanUp() {

    // Nibbles
    for ( u32 i = 0 ; i != mNumHLXs ; i++ ) {
      delete []mOccupancyDataCounters[i];
    }
    delete []mOccupancyDataCounters;
    mOccupancyDataCounters = 0;

    delete []mOccupancyNibbles;
    mOccupancyNibbles = 0;

    delete []mETSumDataCounters;
    mETSumDataCounters = 0;
    delete []mETSumNibbles;
    mETSumNibbles = 0;

    delete []mLHCDataCounters;
    mLHCDataCounters = 0;
    delete []mLHCNibbles;
    mLHCNibbles = 0;

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

    // CRC table
    for ( u32 i = 0 ; i != 256 ; i++ ) {
      if ( crcTable[i] ) {
	delete []crcTable[i];
	crcTable[i] = 0;
      }
    }
    if ( crcTable ) {
      delete []crcTable;
      crcTable = 0;
    }
    
  }

  void NibbleCollector::WorkerThread(void *thisPtr) {
    // Redirect to class-instance-specific implementation
    reinterpret_cast<NibbleCollector *>(thisPtr)->WorkerThreadInt();
  }
  
  void NibbleCollector::WorkerThreadInt() {
    // Poll for data here    
    // No exceptions, just plain-vanilla C++
    fd_set fds;
    struct timeval tv;
    u8 rData[2000];
    int ret;
    
    while (mWorkerThreadContinue) {
      
      // Sock is an intialized socket handle
      tv.tv_sec = 0;
      tv.tv_usec = 1000;
      
      // Reset the descriptors
      FD_ZERO(&fds);
      FD_SET(mUdpSocket, &fds); // adds sock to the file descriptor set
      
      // See if a packet is available
      select(mUdpSocket+1, &fds, NULL, NULL, &tv);
      
      // Check to see if a packet is ready
      if (FD_ISSET(mUdpSocket, &fds)) {
	
	// Grab a packet if one is available
	// Need to define this as a static u8/u16 otherwise it treats it as a u32...
	if ( static_cast<u16>(mWriteBufferPointer+1) == mReadBufferPointer ) {
	  
	  // About to overflow
	  // Read the packet and dump it and flag the counter
	  ret = recv(mUdpSocket,rData,2000,0);
	  if ( ret == -1 ) {
	    SetError("Unable to get data from UDP socket");
	  } else {
	    SetError("Nibble collector input buffer overflow");
	    mNumLostPackets++;
	  }
	  
	} else {
	  
	  // Get a packet from the socket
	  ret = recv(mUdpSocket,circularBuffer[mWriteBufferPointer].data,1500,0);
	  
	  // Check for errors
	  if ( ret == -1 ) {
	    SetError("Unable to get data from UDP socket");
	  } else {
	    
	    // Update the packet length
	    circularBuffer[mWriteBufferPointer].len = ret;
	    // Increment the data counter
	    mTotalDataVolume+=ret;
	    // Increment the circular buffer pointer
	    mWriteBufferPointer++;
	    
	  }
	}
      }
    }
  }
  
  const std::string NibbleCollector::GetLastError() {
    // Locked to make it thread safe
    pthread_mutex_lock(&mErrorMutex);
    std::string localError = mErrorMsg;
    pthread_mutex_unlock(&mErrorMutex);
    return localError;
  }
  
  void NibbleCollector::SetError(const std::string & errorMsg) {
    // Locked to make it thread safe
    pthread_mutex_lock(&mErrorMutex);
    mErrorMsg = errorMsg;
    pthread_mutex_unlock(&mErrorMutex);
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
    // Add the section collector to the list
    this->mSectionCollectors.push_back(sectionCollector);
  }

  // Service handler
  void NibbleCollector::ServiceThread(void *thisPtr) {
    // Redirect to class-instance-specific implementation
    reinterpret_cast<NibbleCollector *>(thisPtr)->ServiceThreadInt();
  }

  void NibbleCollector::ServiceThreadInt() {
    while (mServiceThreadContinue) {
      while (mWriteBufferPointer != mReadBufferPointer) {
	// Validate the checksum
	//if ( ValidateChecksum(circularBuffer[mReadBufferPointer].data,
	//circularBuffer[mReadBufferPointer].len-1) ) {


	// Map the lumi header
	LUMI_RAW_HEADER *lumiHdr = reinterpret_cast<LUMI_RAW_HEADER *>(circularBuffer[mReadBufferPointer].data);
	
	// If the header data is large enough, process the packet
	if ( circularBuffer[mReadBufferPointer].len > sizeof(LUMI_RAW_HEADER) + 1 ) {
	  ProcessPacket(lumiHdr,
			circularBuffer[mReadBufferPointer].data+sizeof(LUMI_RAW_HEADER),
			circularBuffer[mReadBufferPointer].len-1-sizeof(LUMI_RAW_HEADER));
	}

	//} else {
	// Increment the bad packet counter
	//cout << "bad checksum" << endl;
	//mNumBadPackets++;
	//}
      	mReadBufferPointer++;
      }
      Sleep(1);
    }
  }

  bool NibbleCollector::CheckDataValidity(const LUMI_RAW_HEADER *lumiHdr, u32 nBytes) {
    if ( lumiHdr->hlxID >= mNumHLXs ){
      SetError("Invalid HLX ID");
      return false;
    }
    if ( lumiHdr->numOrbits+1 != mNumOrbits ) {
      SetError("Invalid number of orbits"); 
      return false;
    }
    if ( lumiHdr->numBunches+1 != mNumBunches ){
      SetError("Invalid number of bunches");
      return false;
    }
    // Payload size check
    switch ( lumiHdr->histogramSet ) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
      // Occ. and LHC
      // Check data volume against dimensions of nibble
      if ( (lumiHdr->startBunch << 1) + nBytes > m16BitNibbleSize ) {
	SetError("Boundary check failure on 16-bit nibble");
	return false;
      }
      break;

    case 7:
      // Check data volume against dimensions of nibble
      if ( (lumiHdr->startBunch << 2) + nBytes > m32BitNibbleSize ) {
	SetError("Boundary check failure on 32-bit nibble");
	return false;
      }
      break;

    default:
      SetError("Invalid histogram set");
      return false;
    }
    return true;
  }

  void NibbleCollector::ProcessPacket(const LUMI_RAW_HEADER *lumiHdr,
				      const u8 *data,
				      u32 nBytes) {
    
    // Data integrity checks
    if ( !CheckDataValidity(lumiHdr, nBytes) ) {
      mNumBadPackets++;
      return;
    }
    mNumGoodPackets++;

    // cache the hlx id
    u8 hlxID = lumiHdr->hlxID;
    bool bOC0 = (lumiHdr->packetID & 0x1)?true:false;
    bool bCMSLive = (lumiHdr->packetID & 0x2)?true:false;
    
    if ( lumiHdr->histogramSet == 7 ) {
      // ET sum histogram
      
      // First check to see if the start orbit matches the
      // previous one, in which case we are reconstructing the nibble
      // Otherwise we commit the nibble if it's complete
      if ( lumiHdr->startOrbit != mETSumNibbles[hlxID].hdr.startOrbit ) {
	
	// Don't commit if the nibble is empty
	//if ( mETSumNibbles[hlxID].hdr.startOrbit != 0 ) {
	if ( mETSumDataCounters[hlxID] != 0 ) {
	  
	  if ( mETSumDataCounters[hlxID] != m32BitNibbleSize ) { 

	    // Set error
	    SetError("ET sum nibble was incomplete");

	    // Nibble is incomplete - don't ship and count
	    mNumBadETSumNibbles++;

	  } else {

	    // LHC nibble is good, ship it...
	    mNumGoodETSumNibbles++;

	    // Push the nibble into the section collector whether good or bad
	    std::vector<AbstractSectionCollector *>::const_iterator i = mSectionCollectors.begin();
	    while ( i != mSectionCollectors.end() ) {
	      (*i)->ProcessETSumNibble(mETSumNibbles[hlxID],
				       hlxID);
	      ++i;
	    }

	  }
	  
	}
	
	// Clear data counter and reset startOrbit marker
	mETSumDataCounters[hlxID] = 0;
	mETSumNibbles[hlxID].hdr.startOrbit = lumiHdr->startOrbit;
	mETSumNibbles[hlxID].hdr.numBunches = mNumBunches;
	mETSumNibbles[hlxID].hdr.numOrbits = mNumOrbits;
	mETSumNibbles[hlxID].hdr.bCMSLive = bCMSLive;
	mETSumNibbles[hlxID].hdr.bOC0 = bOC0;
	
      }
      
      // Copy the data into the local buffer
      // Modified to index using the bunch number (allows out-of-order) packet arrival
      // Doesn't check for overlapping data however
      memcpy(mETSumNibbles[hlxID].data + lumiHdr->startBunch, //+ (mETSumDataCounters[hlxID]>>2)
	     data,
	     nBytes);
      
      // Update the data counter and check for completion
      mETSumDataCounters[hlxID] += nBytes;

      // Note: complete flag no longer needed - data length monitoring is used
      
    } else if ( lumiHdr->histogramSet == 6 ) {
      
      // Occupancy histograms
      // First check to see if the start orbit matches the previous one, otherwise we discard
      if ( lumiHdr->startOrbit != mLHCNibbles[hlxID].hdr.startOrbit ) {
	
	// Check for incomplete histogram

	// Don't commit if the nibble is empty
	//if ( mLHCNibbles[hlxID].hdr.startOrbit != 0 ) {
	if ( mLHCDataCounters[hlxID] != 0 ) {
	  
	  if ( mLHCDataCounters[hlxID] != m16BitNibbleSize ) { 

	    SetError("LHC nibble was incomplete");

	    mNumBadLHCNibbles++;

	  } else {

	    // LHC nibble is good, ship it...
	    mNumGoodLHCNibbles++;

	    // Push the nibble into the section collector if good
 	    std::vector<AbstractSectionCollector *>::const_iterator i = mSectionCollectors.begin();
	    while ( i != mSectionCollectors.end() ) {
	      (*i)->ProcessLHCNibble(mLHCNibbles[hlxID],
				     hlxID);
	      ++i;
	    }
	  }
	}
	
	// Clear data counter and reset startOrbit marker
	mLHCDataCounters[hlxID] = 0;
	mLHCNibbles[hlxID].hdr.startOrbit = lumiHdr->startOrbit;
	mLHCNibbles[hlxID].hdr.numBunches = mNumBunches;
	mLHCNibbles[hlxID].hdr.numOrbits = mNumOrbits;
	mLHCNibbles[hlxID].hdr.bCMSLive = bCMSLive;
	mLHCNibbles[hlxID].hdr.bOC0 = bOC0;

      }
      
      // Copy the data into the local buffer
      memcpy(mLHCNibbles[hlxID].data + lumiHdr->startBunch,
	     data,
	     nBytes);
      
      // Update the data counter and check for completion
      mLHCDataCounters[hlxID] += nBytes;
      
    } else if ( lumiHdr->histogramSet < 6 ) {

      // Occupancy histograms
      // First check to see if the start orbit matches the previous one, otherwise we discard
      if ( lumiHdr->startOrbit != mOccupancyNibbles[hlxID].hdr.startOrbit ) {

	// Check if nibble contains data
	//if ( mOccupancyNibbles[hlxID].hdr.startOrbit != 0 ) {
	if ( mOccupancyDataCounters[hlxID][0] +
	     mOccupancyDataCounters[hlxID][1] + 
	     mOccupancyDataCounters[hlxID][2] + 
	     mOccupancyDataCounters[hlxID][3] + 
	     mOccupancyDataCounters[hlxID][4] + 
	     mOccupancyDataCounters[hlxID][5] != 0 ) {
	  
	  // Generate the completion check
	  bool bAllComplete = true;
	  for ( u32 i = 0 ; i != 6 ; i++ ) {
	    if ( mOccupancyDataCounters[hlxID][i] != m16BitNibbleSize ) {
	      
	      SetError("Occupancy nibble was incomplete");
	      
	      //cout << "Incomplete occupancy nibble" << endl;
	      //cout << "HLX ID: " << dec << static_cast<u16>(hlxID) << endl;
	      //cout << "Occupancy component: " << i << endl;
	      
	      bAllComplete = false;
	      break;
	    }
	  }
	  
	  if ( bAllComplete ) {

	    mNumGoodOccupancyNibbles++;
	    
	    // Push the nibble into the section collector if good
	    std::vector<AbstractSectionCollector *>::const_iterator i = mSectionCollectors.begin();
	    while ( i != mSectionCollectors.end() ) {
	      (*i)->ProcessOccupancyNibble(mOccupancyNibbles[hlxID],
					hlxID);
	      ++i;
	    }

	  } else {
	    mNumBadOccupancyNibbles++;
	  }
	}
	
	mOccupancyNibbles[hlxID].hdr.startOrbit = lumiHdr->startOrbit;
	mOccupancyNibbles[hlxID].hdr.numBunches = mNumBunches;
	mOccupancyNibbles[hlxID].hdr.numOrbits = mNumOrbits;
	mOccupancyNibbles[hlxID].hdr.bCMSLive = bCMSLive;
	mOccupancyNibbles[hlxID].hdr.bOC0 = bOC0;
	
	// Clear data counter and reset startOrbit marker
	for ( u32 i = 0 ; i != 6 ; i++ ) {
	  mOccupancyDataCounters[hlxID][i] = 0;
	}
	
      }
      
      // Copy the data into the local buffer
      memcpy(mOccupancyNibbles[hlxID].data[lumiHdr->histogramSet] + lumiHdr->startBunch,
	     data,
	     nBytes);
      
      // Update data counters
      mOccupancyDataCounters[hlxID][lumiHdr->histogramSet] += nBytes;
      
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
    crcTable = new u8 *[256];
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
