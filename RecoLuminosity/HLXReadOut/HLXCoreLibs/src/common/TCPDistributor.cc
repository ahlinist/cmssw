/*
  TCP/IP distributor class
  Copyright John Jones 2007
*/

// Prototype class definition
#include "TCPDistributor.hh"

// Various generic and type-specific exception implementations
#include "ICException.hh"
#include "HardwareAccessException.hh"
#include "MemoryAllocationException.hh"
//#include "ArgumentOutOfRangeException.hh"

// networking
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

// HCAL HLX namespace
namespace HCAL_HLX
{

  // Define namespaces in use
  using namespace ICCoreUtils;
  using namespace std;
  
  // Default constructor
  TCPDistributor::TCPDistributor(const char * serverName, unsigned short port) {
    try {
      // Constructor connects to the TCP/IP loopback
      // Server must be active for this to run
      struct sockaddr_in serverAddress;
      struct hostent * hostInfo = gethostbyname(serverName);
      
      // Create the client socket
      mSocket = socket(AF_INET, SOCK_STREAM, 0);
      if ( mSocket < 0 ) {
	MemoryAllocationException lExc("Unable to create client socket");
	RAISE(lExc);
      }

      // Connect to the server (if it's up)
      serverAddress.sin_family = hostInfo->h_addrtype;
      memcpy((char *) &serverAddress.sin_addr.s_addr,
	     hostInfo->h_addr_list[0],
	     hostInfo->h_length);
      serverAddress.sin_port = htons(port);
      
      cout << "connecting to " << serverName << " on port " << port << endl;
      if ( connect(mSocket,
		   (struct sockaddr *) &serverAddress,
		   sizeof(serverAddress)) < 0 ) {
	HardwareAccessException lExc("Unable to connect to server");
	RAISE(lExc);
      }

      cout << "connected" << endl;

      // Initialisation completed

    } catch (ICException & aExc) {
      RETHROW(aExc);
    }
  }

  // Destructor deletes the hardware interface
  TCPDistributor::~TCPDistributor() {
    try {
      // Just close the socket
      cout << "shutting down" << endl;
      shutdown(mSocket, SHUT_RDWR);
    } catch (ICException & aExc) {
      RETHROW(aExc);
    }
  }

  void TCPDistributor::ProcessSection(const LUMI_SECTION & lumiSection) {
    try {
      //char rubbish[8] = "Maggots";
      //      if ( send(mSocket,rubbish,8,0) < 0 ) {
      if ( send(mSocket,reinterpret_cast<const char *>(&lumiSection), sizeof(LUMI_SECTION), 0) < 0 ) {
	//sizeof(LUMI_SECTION)*numHLXs,0) < 0 ) {
	HardwareAccessException lExc("Unable to send data");
	RAISE(lExc);
      }
    } catch (ICException & aExc) {
      RETHROW(aExc);
    }
  }


} // namespace HCAL_HLX
