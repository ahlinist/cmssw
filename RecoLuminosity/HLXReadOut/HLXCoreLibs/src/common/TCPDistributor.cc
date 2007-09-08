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

//#define HCAL_HLX_TCP_DISTRIBUTOR_DEBUG

// HCAL HLX namespace
namespace HCAL_HLX
{

  // Define namespaces in use
  using namespace ICCoreUtils;
  using namespace std;
  
  // Default constructor
  TCPDistributor::TCPDistributor(const char * serverName, u16 port) {
    try {
      mThreadId = 0;

      // Constructor connects to the TCP/IP loopback
      // Server must be active for this to run
      struct sockaddr_in sa_local;
      struct hostent * hostInfo = gethostbyname(serverName);

      // Create the client socket
      mSocket = socket(AF_INET, SOCK_STREAM, 0);
      if ( mSocket < 0 ) {
	MemoryAllocationException lExc("Unable to create server socket");
	RAISE(lExc);
      }

      // Connect to the server (if it's up)
      sa_local.sin_family = hostInfo->h_addrtype;
      memcpy((char *) &sa_local.sin_addr.s_addr,
	     hostInfo->h_addr_list[0],
	     hostInfo->h_length);
      sa_local.sin_port = htons(port);

      int on = 1;
      setsockopt(mSocket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

      // Bind to a local port to receive data
      int ret = bind(mSocket,(struct sockaddr *)&sa_local,sizeof(sa_local));
      if ( ret == -1 ) {
	HardwareAccessException lExc("Server listening socket could not be bound");
	RAISE(lExc);
      }
      
      // Assign to listening port
      // Allow only one queued connection
      listen(mSocket,1);

      // Create the interlock data structure
      mInterlockData = new TCPThreadInterlockData;
      if ( mInterlockData == 0 ) {
	MemoryAllocationException lExc("Unable to allocate memory for TCPThreadInterlockData structure");
	RAISE(lExc);
      }

      // Construct the server thread, and interlock mutex
      // Now it's REALLY getting complicated
      pthread_mutex_init(&mDataMutex,0);
      mWorkerThreadContinue = true;

      // Create the worker thread
      ret = pthread_create(&mThreadId,
			   NULL,
			   (void*(*)(void*))TCPDistributor::WorkerThread,
			   reinterpret_cast<void *>(this));
      if ( ret != 0 ) {
	MemoryAllocationException lExc("Cannot create server thread");
	RAISE(lExc);
      }

    } catch (ICException & aExc) {
      RETHROW(aExc);
    }
  }

  // Destructor deletes the hardware interface
  TCPDistributor::~TCPDistributor() {
    try {
      if ( mThreadId != 0 ) {
	mWorkerThreadContinue=false;
	pthread_join(mThreadId,NULL);
	mThreadId=0;
	pthread_mutex_destroy(&mDataMutex);
      }
      if ( mInterlockData != 0 ) {
	delete mInterlockData;
	mInterlockData = 0;
      }
      std::list<ClientConnectionData>::iterator it = clientList.begin();
      while (it != clientList.end()) {
	shutdown(it->socket, SHUT_RDWR);
	++it;
      }
      clientList.clear();
      // Just close the socket
      if ( mSocket >= 0 ) {
	shutdown(mSocket, SHUT_RDWR);
	mSocket = -1;
      }
    } catch (ICException & aExc) {
      RETHROW(aExc);
    }
  }

  void TCPDistributor::SetupFDs() {		   
    FD_ZERO(&fdsRead);
    FD_ZERO(&fdsWrite);
    FD_ZERO(&fdsExcept);
    
    // Add the listener socket to the read and except FD sets
    // Note we presume the socket is always present
    FD_SET(mSocket, &fdsRead);
    FD_SET(mSocket, &fdsExcept);

    // Add client connections
    std::list<ClientConnectionData>::iterator it = clientList.begin();
    while (it != clientList.end()) {
      // We just read to clear...
      FD_SET(it->socket, &fdsRead);
      
      //cout << "Inside client list fdset loop" << endl;
      if (it->nBytesLeft > 0) {
	// There's data still to be sent on this socket, so we need
	// to be signalled when it becomes writable.
	//cout << "FDset write" << endl;
	FD_SET(it->socket, &fdsWrite);
      }

      //cout << "FDset except" << endl;
      FD_SET(it->socket, &fdsExcept);
      ++it;
    }
}

  void TCPDistributor::WorkerThread(void *thisPtr) {
    // Manager the server in this thread...
    TCPDistributor *theClass = reinterpret_cast<TCPDistributor *>(thisPtr);
    struct timeval tv;
    int fdChangeCount;
    u8 readDataBuffer[100000];
    int ret;

    // TODO - replace max socket number here with a proper one
    while (theClass->mWorkerThreadContinue) {
      // DON'T REMOVE THIS
      pthread_mutex_lock(&theClass->mDataMutex);
      // Initialise the FDs
      theClass->SetupFDs();
      // DON'T REMOVE THIS
      pthread_mutex_unlock(&theClass->mDataMutex);

      // Reinitialise timer
      tv.tv_sec = 1;
      tv.tv_usec = 0;

      // Check for updates
      fdChangeCount = select(1000000,
			     &theClass->fdsRead,
			     &theClass->fdsWrite,
			     &theClass->fdsExcept,
			     &tv);
      if ( fdChangeCount > 0 ) {
	//cout << "Something happened" << endl;
	//cout << "Pre-lock mutex" << endl;
	// DON'T REMOVE THIS
	pthread_mutex_lock(&theClass->mDataMutex);
	if ( FD_ISSET(theClass->mSocket, &theClass->fdsRead) ) {
#ifdef HCAL_HLX_TCP_DISTRIBUTOR_DEBUG
	  cout << "Client connecting" << endl;
#endif
	  struct sockaddr_in clientAddress;
	  socklen_t clientAddressLength = sizeof(clientAddress);
	  ClientConnectionData tempConnData;
	  tempConnData.nBytesLeft = 0;
	  tempConnData.socket = accept(theClass->mSocket,
				       (struct sockaddr *) & clientAddress,
				       &clientAddressLength); 
	  if ( tempConnData.socket < 0 ) {
#ifdef HCAL_HLX_TCP_DISTRIBUTOR_DEBUG
	    cout << "Couldn't accept incoming connection" << endl;
#endif
	  } else {
#ifdef HCAL_HLX_TCP_DISTRIBUTOR_DEBUG
	    cout << "Client connected" << endl;  
#endif
	    theClass->clientList.push_back(tempConnData);
	    //cout << "Post-push_back" << endl;
	  }
	}
	//cout << "On to client list" << endl;
	// TODO - handle except fd on mSocket...
	std::list<ClientConnectionData>::iterator it = theClass->clientList.begin();
	while ( it != theClass->clientList.end() ) {
	  if ( FD_ISSET(it->socket, &theClass->fdsExcept) ) {
#ifdef HCAL_HLX_TCP_DISTRIBUTOR_DEBUG
	    cout << "Error on socket " << it->socket << endl;
#endif
	    // Shut down the socket
	    shutdown(it->socket, SHUT_RDWR);
	    // Remove this client from the list
	    std::list<ClientConnectionData>::iterator itold = it++;
	    theClass->clientList.erase(itold);
	    continue;
	  } else if ( FD_ISSET(it->socket, &theClass->fdsRead) ) {
	    //cout << "Client socket became readable" << endl;
	    ret = recv(it->socket, readDataBuffer, 100000, 0);
	    if ( ret < 0 ) {
#ifdef HCAL_HLX_TCP_DISTRIBUTOR_DEBUG
	      cout << "Error on socket " << it->socket << endl;
#endif
	      // Shut down the socket
	      shutdown(it->socket, SHUT_RDWR);
	      // Remove this client from the list
	      std::list<ClientConnectionData>::iterator itold = it++;
	      theClass->clientList.erase(itold);
	      continue;
	    } else if ( ret == 0 ) {
#ifdef HCAL_HLX_TCP_DISTRIBUTOR_DEBUG
	      cout << "Socket " << it->socket 
		   << " closed at client side" << endl;
#endif
	      // Shut down the socket
	      shutdown(it->socket, SHUT_RDWR);
	      // Remove this client from the list
	      std::list<ClientConnectionData>::iterator itold = it++;
	      theClass->clientList.erase(itold);
	      continue;
	    } else {
	      //cout << "Read (discarded) " << dec << ret << " bytes of data" << endl;
	    } 
	  } else if ( FD_ISSET(it->socket, &theClass->fdsWrite) ) {
	    //cout << "Client socket became writable" << endl;
	    ret = send(it->socket,
		      it->lumiSection + sizeof(LUMI_SECTION) - it->nBytesLeft,
		      it->nBytesLeft,
		      0);
	    if ( ret < 0 ) {
#ifdef HCAL_HLX_TCP_DISTRIBUTOR_DEBUG
	      cout << "ERROR - write to client socket " << it->socket << " failed" << endl;
#endif
	      // Shut down the socket
	      shutdown(it->socket, SHUT_RDWR);
	      // Remove this client from the list
	      std::list<ClientConnectionData>::iterator itold = it++;
	      theClass->clientList.erase(itold);
	      continue;
	    } else {
	      //cout << "Sent " << dec << ret
	      //   << " bytes to client on socket "
	      //   << it->socket << endl;
	      it->nBytesLeft -= ret;
	    }
	  }
	  ++it;
	}
	//cout << "Pre-unlock mutex" << endl;
	// DON'T REMOVE THIS
	pthread_mutex_unlock(&theClass->mDataMutex);
      }
      //Sleep(1);
    }

    //cout << "TCPDistributor: INFO - Worker thread complete" << endl;
  }

  bool TCPDistributor::ProcessSection(const LUMI_SECTION & lumiSection) {
#ifdef HCAL_HLX_TCP_DISTRIBUTOR_DEBUG
    cout << "Begin " << __PRETTY_FUNCTION__ << endl;
#endif
    // TODO - modify so list held by section collector
    // Stalls are then also flagged in the section collector *not* here
//    try {
      // DON'T REMOVE THIS
      //cout << "Before mutex lock" << endl;
      pthread_mutex_lock(&mDataMutex);

      // Data structure copy
      std::list<ClientConnectionData>::iterator it = clientList.begin();
      while (it != clientList.end()) {
	if ( it->nBytesLeft != 0 ) {
	  // Flag an error (not done properly yet)
	  cout << "TCPDistributor: ERROR - server interlock copy failed" << endl;
	  cout << "                Client socket " << dec << it->socket << endl;
	  pthread_mutex_unlock(&mDataMutex);
          return false;
	} else {
	  // Copy the data into the interlock buffer
	  it->nBytesLeft = sizeof(LUMI_SECTION);
	  memcpy(&it->lumiSection,&lumiSection,sizeof(LUMI_SECTION));
	}
	++it;
      }

      // DON'T REMOVE THIS
      //cout << "Before mutex unlock" << endl;
      pthread_mutex_unlock(&mDataMutex);
    //} catch (ICException & aExc) {
    //  RETHROW(aExc);
   // }

   return true;
#ifdef HCAL_HLX_TCP_DISTRIBUTOR_DEBUG
    cout << "End " << __PRETTY_FUNCTION__ << endl;
#endif
  }


} // namespace HCAL_HLX
