#include <vector>
#include <string>
#include <exception>
#include <iostream>
#include <fstream>
#include <sstream>

#include <signal.h>
#include <unistd.h>
#include <errno.h>

//#include "ICUtilityToolbox.hh"
//#include "ICException.hh"
//#include "EmulatorInterface.hh"
//#include "ICTypeDefs.hh"

#include "LumiStructures.hh"

// networking
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

int gContinue=1;

void CtrlC(int aSigNum) {
  std::cout << "Ctrl-c detected, stopping run" << std::endl;
  gContinue=0;
}

using namespace std;
using namespace HCAL_HLX; 

const unsigned short port = 0x52C3; // port 50002 (reverse-endian)

void InterpretSection(const LUMI_SECTION & lumiSection) {

  //cout << "HLX ID: " << lumiSection.hdr.hlxID << endl;
  cout << "Run number: " << lumiSection.hdr.runNumber << endl;
  cout << "Num orbits: " << lumiSection.hdr.numOrbits << endl;
  cout << "Num bunches: " << lumiSection.hdr.numBunches << endl;
  cout << "Start orbit: " << lumiSection.hdr.startOrbit << endl;
  cout << "CMS live: " << lumiSection.hdr.bCMSLive << endl;

}

struct ClientConnectionData {
  // Socket identifier
  int sd;

  // Lumi section buffer
  u8 lumiSection[sizeof(LUMI_SECTION)];

  // Amount of data left to write
  u32 leftToWrite;
};

void SetupFDSets(fd_set& ReadFDs, fd_set& WriteFDs, 
		 fd_set& ExceptFDs, int ListeningSocket = -1,
		 int connectSocket = -1) { //std::vector & gConnections) {
    FD_ZERO(&ReadFDs);
    FD_ZERO(&WriteFDs);
    FD_ZERO(&ExceptFDs);

    // Add the listener socket to the read and except FD sets, if there
    // is one.
    if (ListeningSocket != -1) {
        FD_SET(ListeningSocket, &ReadFDs);
        FD_SET(ListeningSocket, &ExceptFDs);
    }

    // Add client connections
    /*    std::vector<ClientConnectionData>::iterator it = gConnections.begin();
    while (it != gConnections.end()) {
      if (it->nCharsInBuffer < kBufferSize) {
	// There's space in the read buffer, so pay attention to
	// incoming data.
	FD_SET(it->sd, &ReadFDs);
      }
      
      //if (it->nCharsInBuffer > 0) {
      // There's data still to be sent on this socket, so we need
      // to be signalled when it becomes writable.
      //FD_SET(it->sd, &WriteFDs);
      //  }
      
      FD_SET(it->sd, &ExceptFDs);
      
      ++it;
      }*/
    if (connectSocket != -1) {
      FD_SET(connectSocket, &ReadFDs);
      FD_SET(connectSocket, &ExceptFDs);
    }
}

int main(int argc, char ** argv)
{
  signal(SIGINT,CtrlC);
  try{
    const char serverName[] = "127.0.0.1";
    unsigned char *rData = new unsigned char[1000000];
    unsigned short port = 50002;

//restart_client:

    // Constructor connects to the TCP/IP loopback
    // Server must be active for this to run
    struct sockaddr_in serverAddress;
    struct hostent * hostInfo = gethostbyname(serverName);
    
    // Create the client socket
    int tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
    if ( tcp_socket < 0 ) {
      cerr << "Unable to create client socket" << endl;
      return 1;
    }
    
    // Connect to the server (if it's up)
    serverAddress.sin_family = hostInfo->h_addrtype;
    memcpy((char *) &serverAddress.sin_addr.s_addr,
	   hostInfo->h_addr_list[0],
	   hostInfo->h_length);
    serverAddress.sin_port = htons(port);
    
    cout << "Connecting to " << serverName << " on port " << port << endl;
    if ( connect(tcp_socket,
		 (struct sockaddr *) &serverAddress,
		 sizeof(serverAddress)) < 0 ) {
      cerr << "Unable to connect to server" << endl;
      return 1;
    }

    fd_set fdsRead, fdsWrite, fdsExcept;
    struct timeval tv;
    
    // sock is an intialized socket handle
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    int outputcode;
    int z=0, localCount=0;
    time_t tempTime, curTime;
    int ret;

    time(&curTime);

    // Temporary storage of a lumi section
    LUMI_SECTION localSection;

    while (gContinue) {
      SetupFDSets(fdsRead,
		  fdsWrite,
		  fdsExcept,
		  -1,
		  tcp_socket);

      //cout << connectSocket << endl;      
      if ((outputcode = select(1000000, &fdsRead, 0, &fdsExcept, 0)) > 0) {
	//cout << "Something happened" << endl;
	if (FD_ISSET(tcp_socket, &fdsRead)) {
	  //cout << "Socket became readable" << endl;
	  ret = recv(tcp_socket,rData,10000000,0);
	  if ( ret < 0 ) {
	    cerr << "Unable to get data" << endl;
	    return 1;
	  } else if ( ret == 0 ) {
	    cout << "Socket shutdown" << endl;
	  } else {
	    
	    if ( ret+localCount > sizeof(HCAL_HLX::LUMI_SECTION) ) {
	      memcpy(reinterpret_cast<char *>(&localSection)+localCount,
		     rData,
		     sizeof(HCAL_HLX::LUMI_SECTION)-localCount);
	      //cout << "A: Lumi section completed" << endl;
	      z++;
	      // DO SOMETHING WITH DATA
	      InterpretSection(localSection);
	      localCount = ret - sizeof(HCAL_HLX::LUMI_SECTION) + localCount;
	      memcpy(reinterpret_cast<char *>(&localSection),
		     rData+ret-localCount,
		     localCount);
	    } else {
	      memcpy(reinterpret_cast<char *>(&localSection)+localCount,rData,ret);
	      localCount += ret;
	      if ( localCount == sizeof(HCAL_HLX::LUMI_SECTION) ) {
		cout << "B: Lumi section completed" << endl;	    
		// DO SOMETHING WITH DATA
		InterpretSection(localSection);
		z++;
		localCount = 0;
                //shutdown(tcp_socket,SHUT_RDWR);
                //goto restart_client;
	      }
	    }
	  }
	} else if (FD_ISSET(tcp_socket, &fdsExcept)) {
	  cout << "Exception occurred on socket" << endl;
	}
      }
      time(&tempTime);
      if ( tempTime-curTime != 0 ) {
	cout << z << endl;
	curTime=tempTime;
      }
      //cout << "Output code: " << outputcode << endl;
      if ( (ret == 0) && (outputcode > 0) ) {
	//cout << "Error number: " << dec << errno << endl;
	//cout << "Error string: " << strerror(errno) << endl;
	//closesocket(connectSocket);
	break;
      }
      //usleep(1000);
    }

//    shutdown(tcp_socket,SHUT_RDWR);
    delete []rData;

  }catch(std::exception & aExc){
    std::cerr<<aExc.what()<<std::endl;
  }catch(...){
    std::cerr<<"Unknown exception caught."<<std::endl;
  }




  return 0;
}
