/*
  Nibble Collector class
  Copyright John Jones 2007

  Implements the UDP collector for HLX data packets
*/

#ifndef NIBBLECOLLECTOR_HH
#define NIBBLECOLLECTOR_HH

// The string and stream definitions
//#include <iostream>
//#include <string>
#include <vector>

// Type definitions used by the HAL, etc...
#include "ICTypeDefs.hh"

// Standard high-level data structures for luminosity
#include "LumiStructures.hh"

// Abstract base class for Lumi Section
#include "AbstractSectionCollector.hh"

// Namespace for the HCAL HLX
namespace HCAL_HLX
{

  // We shall be using the IC core utility library and the HAL
  using namespace ICCoreUtils;
  using namespace std;

  // Data structure for the circular buffer
  struct HLX_CB_TYPE {
    u8 *data;
    u32 len;
  };
  
  // Prototype class definition
  class NibbleCollector {

  public:

    // Constructor takes port and local IP as arguments
    NibbleCollector(u32 aNumHLXs,
		    u16 aNumBunches,
		    u16 aNumOrbits,
		    u16 localPort = 0x533A,
		    const std::string & sourceAddress = "192.168.1.100");
    ~NibbleCollector();
    
    // Reset function
    void Reset();
    // Thread start function
    void Start();
    // Thread stop function
    void Stop();

    // Statistics
    u32 GetNumGoodPackets();
    u32 GetNumBadPackets();
    u32 GetNumLostPackets();
    double GetTotalDataVolume();
    u32 GetNumGoodETSumNibbles();
    u32 GetNumBadETSumNibbles();
    u32 GetNumGoodOccupancyNibbles();
    u32 GetNumBadOccupancyNibbles();
    u32 GetNumGoodLHCNibbles();
    u32 GetNumBadLHCNibbles();
    
    // Pointer debug
    u16 GetWriteBufferPointer();
    u16 GetReadBufferPointer();

    // Lumi section collector
    void AttachSectionCollector(AbstractSectionCollector *sectionCollector);

    // Yes this is by-value to be thread-safe (invokes copy constructor)
    const std::string GetLastError();

  protected:

    // Helper functions
    bool CheckDataValidity(const LUMI_RAW_HEADER *lumiHdr,
			   u32 nBytes);
    void InitialiseNibble(u8 hlxID,
			  u32 startOrbit);

    // Error logging
    pthread_mutex_t mErrorMutex;
    std::string mErrorMsg;
    void SetError(const std::string & errorMsg);

    // Packet processor
    void ProcessPacket(const LUMI_RAW_HEADER *lumiHdr,
		       const u8 *data,
		       u32 nBytes);

    // Initialisation function
    void Init();
    
    // Cleanup function
    void CleanUp();

    // Thread worker function
    static void WorkerThread(void *thisPtr);
    void WorkerThreadInt();

    // Thread service functions
    static void ServiceThread(void *thisPtr);
    void ServiceThreadInt();

    // Checksum calculation helper functions
    bool ValidateChecksum(const u8 *data, u32 numBytes);
    u8 ComputeChecksum(const u8 *data, u32 numBytes);
    u8 ChecksumHelper(u8 data, u8 prevCRC);
    u8 SingleChecksum(u8 a, u8 b);
    void InitialiseChecksum();
    
    // Statistics registers
    u32 mNumGoodPackets;
    u32 mNumBadPackets;
    u32 mNumLostPackets;
    double mTotalDataVolume;
    u32 mNumGoodOccupancyNibbles;
    u32 mNumBadOccupancyNibbles;
    u32 mNumGoodLHCNibbles;
    u32 mNumBadLHCNibbles;
    u32 mNumGoodETSumNibbles;
    u32 mNumBadETSumNibbles;

    // Lumi nibble data
    ET_SUM_NIBBLE *mETSumNibbles;
    u32 *mETSumDataCounters;
    LHC_NIBBLE *mLHCNibbles;
    u32 *mLHCDataCounters;
    OCCUPANCY_NIBBLE *mOccupancyNibbles;
    u32 **mOccupancyDataCounters;

    // Vector of attached lumi section generators
    // Finished nibbles (complete or otherwise) are pushed into these classes
    std::vector<AbstractSectionCollector *> mSectionCollectors;

    // Socket handle
    int mUdpSocket;

    // Worker thread continuation flag
    bool mWorkerThreadContinue;
    bool mServiceThreadContinue;

    // Number of HLXs in system
    u32 mNumHLXs;
    // Expected number of orbits in a packet
    u16 mNumOrbits;
    // Expected number of bunches in a packet
    u16 mNumBunches;
    // Payload data volume
    u32 m32BitNibbleSize;
    u32 m16BitNibbleSize;

    // Circular buffer pointers
    u16 mWriteBufferPointer;
    u16 mReadBufferPointer;

    // Circular buffers
    HLX_CB_TYPE *circularBuffer;

    // CRC checksums
    u8 **crcTable;

    // Worker thread ID
    pthread_t mWorkerThreadId, mServiceThreadId;

    // Hardware abstraction layer
    //HLX_HAL::HLXVMEILInterface * mHardwareInterface;

  }; //~class HLXInterface

}//~namespace HCAL_HLX

#endif //~HLXCARDINTERFACE_HH
