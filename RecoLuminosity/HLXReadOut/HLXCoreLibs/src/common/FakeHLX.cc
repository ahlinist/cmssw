/*
  Fake HLX class
  John Jones, Princeton U. 2008
*/

// Prototype class definition
#include "FakeHLX.hh"

// Various generic and type-specific exception implementations
#include "ICException.hh"
#include "MemoryAllocationException.hh"

// Networking
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// Threading (ah hell now we're in trouble...)
#include <pthread.h>

#define FAKE_HLX_MTU 1400

// HCAL HLX namespace
namespace HCAL_HLX {

  // Define namespaces in use
  using namespace ICCoreUtils;
  using namespace std;
  
  // Default constructor
  FakeHLX::FakeHLX(u32 numHLXs,
		   u32 numBunches,
		   u32 numOrbits,
		   u32 shortSectionLength, // used to normalise rate
		   u16 targetPort,
		   bool randomRestarts,
		   const std::string & target) {
    mWorkerThreadId = 0;
    mWorkerThreadContinue = false;
    mNumOrbits = numOrbits;
    mNumHLXs = numHLXs;
    mShortSectionLength = shortSectionLength;
    mRandomRestarts = randomRestarts;

    // Check on payload size
    if ( numBunches == 0 || numBunches > 3564 ) {
      MemoryAllocationException lExc("numBunches is an invalid value");
      RAISE(lExc);
    }

    //  First we try and setup the network connection

    // Open a UDP socket
    mUDPSocket = socket(PF_INET,SOCK_DGRAM,0);
    if ( mUDPSocket == -1 ) {
      cerr << "Socket could not be opened" << endl;
      return;
    }

    // Set up the socket descriptor
    mSocketTarget.sin_port = htons(targetPort);
    mSocketTarget.sin_addr.s_addr = inet_addr(target.c_str());
    mSocketTarget.sin_family = AF_INET;

    // Now we generate the payload data
    // Which forms a circular loop
    // To do this we need to know how many packet we're sending
    GeneratePayloadData(numBunches,
			numOrbits);

    // Start the random number generator
    srand(time(NULL));

    // Calculate the probability required to get a reset every 60 seconds
    mResetProbability = 1.0 / ((float)shortSectionLength * 60.0 * (float)mPayloadData.size() * (float)mNumHLXs);

    // Start the worker thread
    mWorkerThreadContinue = true;
    int ret = pthread_create(&mWorkerThreadId,
			     NULL,
			     (void*(*)(void*))FakeHLX::WorkerThread,
			     reinterpret_cast<void *>(this));
    if ( ret != 0 ) {
      MemoryAllocationException lExc("Cannot create worker thread");
      RAISE(lExc);
    }

  }

  // Destructor deletes the hardware interface
  FakeHLX::~FakeHLX() {

    // Shut down the service thread
    if ( mWorkerThreadId != 0 ) {
      mWorkerThreadContinue = false;
      pthread_join(mWorkerThreadId,NULL);
      mWorkerThreadId = 0;
    }

    // UDP socket shutdown
    if ( mUDPSocket != -1 ) {
      shutdown(mUDPSocket,SHUT_RDWR);
      mUDPSocket=-1;
    }

    // Clean up the payload data
    CleanUpPayloadData();
  }

  void FakeHLX::WorkerThread(void *thisPtr) {
    FakeHLX *me = reinterpret_cast<FakeHLX *>(thisPtr);
    int ret;
    std::vector<Packet>::const_iterator i;
    u32 orbitNumber = 0;
    time_t last_time,next_time;
    next_time = last_time = time(NULL);
    u32 nibbleCount = 0;

    // Orbit reset on first orbit
    //u8 packetID = 1; // 1=orbit reset

    // Main service loop
    while ( me->mWorkerThreadContinue ) {

    restart_loop:

      // Loop through all HLXs
      for ( u8 j = 0 ; j != me->mNumHLXs ; j++ ) {
	
	i = me->mPayloadData.begin();
	while ( i != me->mPayloadData.end() ) {

	  // Set the variable numbers
	  reinterpret_cast<LUMI_RAW_HEADER *>(i->data)->startOrbit = orbitNumber;
	  reinterpret_cast<LUMI_RAW_HEADER *>(i->data)->hlxID = j;
	  //reinterpret_cast<LUMI_RAW_HEADER *>(i->data)->packetID = packetID;

	  if ( nibbleCount == me->mShortSectionLength ) {
	    // Stall until next second
	    // Normalises rate to 1 short section per second
	    while ( true ) {
	      next_time = time(NULL);
	      if ( next_time == last_time ) Sleep(1);
	      else {
		cout << "HLX: "
		     << static_cast<u16>(j)
		     << "\tOrbit number: "
		     << orbitNumber
		     << endl;
		last_time = next_time;
		nibbleCount = 0;
		break;
	      }
	    }
	  }

	  if ( me->mRandomRestarts ) {
	    // Generate a randomised restart
	    // The HLX can produce this anywhere in a payload from and orbit counter reset
	    // Throw a dice and see whether to screw up
	    
	    int failureRand = rand();
	    float chance = (float)failureRand/(float)RAND_MAX;
	    if ( chance < me->mResetProbability ) {
	      cout << "Random restart" << endl;

	      // Randomly mess up the packet and reset
	      float chance = (float)failureRand/(float)RAND_MAX;
	      u32 ssize = (u32)((float)i->size * chance);
	      // Send it
	      ret = sendto(me->mUDPSocket,
			   i->data,
			   ssize,
			   0,
			   (struct sockaddr *)&me->mSocketTarget,
			   sizeof(me->mSocketTarget)); 
	      if ( ret == -1 ) {
		cerr << "Unable to send data" << endl;
	      }
	      orbitNumber = 0;
	      goto restart_loop;
	    } 
	  }

	  // Send it
	  ret = sendto(me->mUDPSocket,
		       i->data,
		       i->size,
		       0,
		       (struct sockaddr *)&me->mSocketTarget,
		       sizeof(me->mSocketTarget)); 
	  if ( ret == -1 ) {
	    cerr << "Unable to send data" << endl;
	    ++i;
	    continue;
	  }

	  ++i;
	}

	// Slow things down
	Sleep(1);

      }

      // Increment the base orbit counter
      orbitNumber += me->mNumOrbits;
      
      // Update nibble count
      nibbleCount++;
      
      // Clear the packet ID
      //packetID = 0;
      
    }

  }

  void FakeHLX::GeneratePayloadData(u32 numBunches,
				    u32 numOrbits) {
    // Basic payload header
    LUMI_RAW_HEADER lumiHeader = {
      0xFFFF, // marker (always ffff)
      0,      // hlx ID (always zero as only one for fake)
      0,      // packet ID
      0,      // start orbit
      numOrbits - 1,      // num orbits
      0,      // start bunch (always zero for short orbit)
      numBunches - 1,      // num bunches
      0,      // histogram set
      0,      // histogram sequence
      0xAAAA,
      0xFFFF
    };
    
    // Remaining payload data space
    // MTU minus the packet header and the orbit counter sync bit
    u32 payloadDataSpace = FAKE_HLX_MTU - sizeof(LUMI_RAW_HEADER) - 1;

    // Calculate maximum number of 32-bit entries
    u32 max32BitBunches = payloadDataSpace / sizeof(u32);
    // Calculate maximum number of 16-bit entries
    u32 max16BitBunches = payloadDataSpace / sizeof(u16);

    // Calculate number of packets required
    u32 num32BitPackets = numBunches / max32BitBunches;
    u32 last32BitBunches = numBunches % max32BitBunches;
    //if ( last32BitBunches != 0 ) num32BitPackets++;
    u16 num16BitPackets = numBunches / max16BitBunches;
    u32 last16BitBunches = numBunches % max16BitBunches;
    //if ( last32BitBunches != 0 ) num16BitPackets++;

    // Generate the payload data
    for ( lumiHeader.histogramSet = 0 ;
	  lumiHeader.histogramSet != 7 ;
	  lumiHeader.histogramSet++ ) {
      lumiHeader.histogramSequence = 0;
      lumiHeader.startBunch = 0;

      for ( ; lumiHeader.histogramSequence != num16BitPackets ;
	    lumiHeader.histogramSequence++ ) {
	  
	// Packet data
	Packet tempPacket;
	tempPacket.size = sizeof(LUMI_RAW_HEADER)+(max16BitBunches*sizeof(u16))+1;
	tempPacket.data = new u8[tempPacket.size];
	
	// Copy in the lumi raw header
	memcpy(tempPacket.data,
	       &lumiHeader,
	       sizeof(LUMI_RAW_HEADER));
	
	// Add the straight line data
	u16 *dpoints = reinterpret_cast<u16 *>(tempPacket.data+sizeof(LUMI_RAW_HEADER));
	for ( u32 i = 0 ;
	      i != max16BitBunches ;
	      i++ ) {
	  u32 value = (lumiHeader.startBunch+i) * numOrbits;
	  if ( value > 0xFFFF ) {
	    dpoints[i] = 0xFFFF; // cap value
	  } else {
	    dpoints[i] = value;
	  }
	}
	
	// No CRC anymore so just blank
	tempPacket.data[tempPacket.size-1] = 0;
	
	// Append to the payload data
	mPayloadData.push_back(tempPacket);
	
	// Increment the bunch marker
	lumiHeader.startBunch += max16BitBunches;
      }

      // Last entry if it exists
      if ( last16BitBunches != 0 ) {

	// Packet data
	Packet tempPacket;
	tempPacket.size = sizeof(LUMI_RAW_HEADER)+(last16BitBunches*sizeof(u16))+1;
	tempPacket.data = new u8[tempPacket.size];
	
	// Copy in the lumi raw header
	memcpy(tempPacket.data,
	       &lumiHeader,
	       sizeof(LUMI_RAW_HEADER));
	
	// Add the straight line data
	u16 *dpoints = reinterpret_cast<u16 *>(tempPacket.data+sizeof(LUMI_RAW_HEADER));
	for ( u32 i = 0 ;
	      i != last16BitBunches ;
	      i++ ) {
	  u32 value = (lumiHeader.startBunch+i) * numOrbits;
	  if ( value > 0xFFFF ) {
	    dpoints[i] = 0xFFFF; // cap value
	  } else {
	    dpoints[i] = value;
	  }
	}
	
	// No CRC anymore so just blank
	tempPacket.data[tempPacket.size-1] = 0;
	
	// Append to the payload data
	mPayloadData.push_back(tempPacket);
	
      }

    }

    // Last histogram (et = 32bit)
    lumiHeader.histogramSequence = 0;
    lumiHeader.startBunch = 0;

    for ( ; lumiHeader.histogramSequence != num32BitPackets ;
	  lumiHeader.histogramSequence++ ) {
      
      // Packet data
      Packet tempPacket;
      tempPacket.size = sizeof(LUMI_RAW_HEADER)+(max32BitBunches*sizeof(u32))+1;
      tempPacket.data = new u8[tempPacket.size];
      
      // Copy in the lumi raw header
      memcpy(tempPacket.data,
	     &lumiHeader,
	     sizeof(LUMI_RAW_HEADER));
      
      // Add the straight line data
      u32 *dpoints = reinterpret_cast<u32 *>(tempPacket.data+sizeof(LUMI_RAW_HEADER));
      for ( u32 i = 0 ;
	    i != max32BitBunches ;
	    i++ ) {
	dpoints[i] = (lumiHeader.startBunch+i) * numOrbits;
      }
      
      // No CRC anymore so just blank
      tempPacket.data[tempPacket.size-1] = 0;
      
      // Append to the payload data
      mPayloadData.push_back(tempPacket);
      
      // Increment the bunch marker
      lumiHeader.startBunch += max32BitBunches;
    }
  
    // Last entry if it exists
    if ( last32BitBunches != 0 ) {
      
      // Packet data
      Packet tempPacket;
      tempPacket.size = sizeof(LUMI_RAW_HEADER)+(last32BitBunches*sizeof(u32))+1;
      tempPacket.data = new u8[tempPacket.size];
      
      // Copy in the lumi raw header
      memcpy(tempPacket.data,
	     &lumiHeader,
	     sizeof(LUMI_RAW_HEADER));
    
      // Add the straight line data
      u32 *dpoints = reinterpret_cast<u32 *>(tempPacket.data+sizeof(LUMI_RAW_HEADER));
      for ( u32 i = 0 ;
	    i != last32BitBunches ;
	    i++ ) {
	dpoints[i] = (lumiHeader.startBunch+i) * numOrbits;
      }
      
      // No CRC anymore so just blank
      tempPacket.data[tempPacket.size-1] = 0;
      
      // Append to the payload data
      mPayloadData.push_back(tempPacket);
     
    }
    
  }

  void FakeHLX::CleanUpPayloadData() {

    // Loop and delete data
    std::vector<Packet>::iterator i = mPayloadData.begin();
    while ( i != mPayloadData.end() ) {
      /*      cout << i->size<< endl;
      cout << static_cast<u32>(reinterpret_cast<LUMI_RAW_HEADER *>(i->data)->marker) << endl;
      cout << static_cast<u32>(reinterpret_cast<LUMI_RAW_HEADER *>(i->data)->hlxID)<< endl;
      cout << static_cast<u32>(reinterpret_cast<LUMI_RAW_HEADER *>(i->data)->packetID)<< endl;
      cout << static_cast<u32>(reinterpret_cast<LUMI_RAW_HEADER *>(i->data)->startOrbit)<< endl;
      cout << static_cast<u32>(reinterpret_cast<LUMI_RAW_HEADER *>(i->data)->numOrbits)<< endl;
      cout << static_cast<u32>(reinterpret_cast<LUMI_RAW_HEADER *>(i->data)->startBunch)<< endl;
      cout << static_cast<u32>(reinterpret_cast<LUMI_RAW_HEADER *>(i->data)->numBunches)<< endl;
      cout << static_cast<u32>(reinterpret_cast<LUMI_RAW_HEADER *>(i->data)->histogramSet)<< endl;
      cout << static_cast<u32>(reinterpret_cast<LUMI_RAW_HEADER *>(i->data)->histogramSequence)<< endl;
      cout << static_cast<u32>(reinterpret_cast<LUMI_RAW_HEADER *>(i->data)->allA)<< endl;
      cout << static_cast<u32>(reinterpret_cast<LUMI_RAW_HEADER *>(i->data)->allF)<< endl;
      cout << endl;*/

      delete []i->data;
      ++i;
    }
    mPayloadData.clear();

  }


} // namespace HCAL_HLX
