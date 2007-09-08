/*
  TCP distributor class
  Copyright John Jones 2007

  Output lumi section data to cout
*/

#ifndef TCPDISTRIBUTOR_HH
#define TCPDISTRIBUTOR_HH

// The string and stream definitions
//#include <fstream>
//#include <string>
#include <list>

// Include AbstractDistributor definition
#include "AbstractDistributor.hh"

// Type definitions used by the HAL, etc...
#include "ICTypeDefs.hh"

// Standard high-level data structures for luminosity
#include "LumiStructures.hh"

// pthreads
#include <pthread.h>

// networking
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>

// Namespace for the HCAL HLX
namespace HCAL_HLX {

  // We shall be using the IC core utility library
  using namespace ICCoreUtils;
  using namespace std;

  struct ClientConnectionData {
    // Client connection socket
    int socket;

    // Always a lumi section data structure for this
    u8 lumiSection[sizeof(LUMI_SECTION)];

    // Number of bytes left in the send
    u32 nBytesLeft;
  };

  struct TCPThreadInterlockData {
    // Flag for update
    bool bDataUpdate;

    // Data structure to copy data into
    LUMI_SECTION lumiSection;
  };

  // Prototype class definition
  class TCPDistributor : public AbstractDistributor {

  public:

    // Constructor
    TCPDistributor(const char * serverName = "127.0.0.1",
		   u16 port = 50002);

    // Destructor
    ~TCPDistributor();

    // Processing function for ET sum histogram
    bool ProcessSection(const LUMI_SECTION & lumiSection);

  private:
    // File descriptor setup
    void SetupFDs();

    // Thread worker function stuff
    static void WorkerThread(void *thisPtr);
    pthread_t mThreadId;
    pthread_mutex_t mDataMutex;
    bool mWorkerThreadContinue;

    // Thread interlock data structure
    TCPThreadInterlockData *mInterlockData;

    // Listener socket
    int mSocket;

    // FD sets
    fd_set fdsRead;
    fd_set fdsWrite;
    fd_set fdsExcept;

    // Client connection information
    std::list<HCAL_HLX::ClientConnectionData> clientList;

  }; //~class TCPDistributor

}//~namespace HCAL_HLX

#endif //~TCPDISTRIBUTOR_HH
