/*
  Fake HLX data generator
  Copyright John Jones 2007

  Abstract base class defining a distributor
*/

#ifndef FAKE_HLX_HH
#define FAKE_HLX_HH

// Type definitions used by the HAL, etc...
#include "ICTypeDefs.hh"

// Standard high-level data structures for luminosity
#include "LumiStructures.hh"

// vector
#include <vector>

#include <netinet/in.h>

// Namespace for the HCAL HLX
namespace HCAL_HLX {

  // We shall be using the IC core utility library
  using namespace ICCoreUtils;
  using namespace std;

  // Prototype class definition
  class FakeHLX {

  public:

    // Constructor
    FakeHLX(u32 numHLXs,     // Number of HLXs
	    u32 numBunches,  // Number of bunches per nibble
	    u32 numOrbits,   // Number of orbits per nibble
	    u32 shortSectionLength, // Length of short section (for normalisation)
	    u16 targetPort,  // Target port
	    bool randomRestarts, // Generate random restarts over random periods of time
	    const std::string & target = "192.168.1.100" );  // Local target address

    // Destructor
    ~FakeHLX();

  private:

    // Worker thread function
    static void WorkerThread(void *theClass);

    // Helper functions
    void GeneratePayloadData(u32 numBunches,
			     u32 numOrbits);
    void CleanUpPayloadData();

    struct Packet {
      u8 *data;
      u32 size;
    };
    std::vector<Packet> mPayloadData;

    int mUDPSocket;
    sockaddr_in mSocketTarget;
    bool mWorkerThreadContinue;
    pthread_t mWorkerThreadId;
    bool mRandomRestarts;

    u32 mNumOrbits;
    u32 mNumHLXs;
    u32 mShortSectionLength;

    float mResetProbability;

  }; //~class FakeHLX
  
} //~namespace HCAL_HLX

#endif //~FAKEHLX_HH
