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
    //std::vector<ClientConnectionData> connectionList;
    unsigned char *rData = new unsigned char[1000000];

    // Open a TCP socket
    int tcp_socket = socket(PF_INET,SOCK_STREAM,0);
    if ( tcp_socket == -1 ) {
      cerr << "Socket could not be opened" << endl;
      return 1;
    }

    // Create local port and ip structure
    sockaddr_in sa_local;
    sa_local.sin_port = port;
    sa_local.sin_addr.s_addr = htonl(INADDR_ANY);
    sa_local.sin_family = AF_INET;

    // Bind to a local port to receive data
    int ret = bind(tcp_socket,(struct sockaddr *)&sa_local,sizeof(sa_local));
    if ( ret == -1 ) {
      cerr << "Socket could not be bound" << endl;
      return 1;
    }

    // Assign to listening port
    listen(tcp_socket,1);

    // Wait for connection to client
    cout << "Waiting for client connection" << endl;

    socklen_t clientAddressLength;
    struct sockaddr_in clientAddress;
    int connectSocket;

    fd_set fdsRead, fdsWrite, fdsExcept;
    struct timeval tv;
    
    // sock is an intialized socket handle
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    int outputcode;

  restart_connection:
    while (gContinue) {
      SetupFDSets(fdsRead,
		  fdsWrite,
		  fdsExcept,
		  tcp_socket);

      // tcp_socket+1
      if ( (outputcode = select(1000000,&fdsRead,&fdsWrite,&fdsExcept,0)) > 0 ) {
	//cout << "Something happened" << endl;
	if ( FD_ISSET(tcp_socket, &fdsRead) ) {
	  cout << "Client connecting" << endl;
	  
	  clientAddressLength = sizeof(clientAddress);
	  connectSocket = accept(tcp_socket,
				 (struct sockaddr *) & clientAddress,
				 &clientAddressLength); 
	  if ( connectSocket < 0 ) {
	    cerr << "Couldn't accept incoming connection" << endl;
	    //return 1;
	  } else {
	    cout << "Client connected" << endl;  
	    // Go into data readout loop
	    break;
	  }
	}
      }
      //cout << dec << outputcode << endl;
      //usleep(1000);
    }
    
    int z=0, localCount=0;
    time_t tempTime, curTime;
    time(&curTime);

    // Temporary storage of a lumi section
    LUMI_SECTION localSection;

    while (gContinue) {
      SetupFDSets(fdsRead,
		  fdsWrite,
		  fdsExcept,
		  tcp_socket,
		  connectSocket);

      //cout << connectSocket << endl;      
      if ((outputcode = select(1000000, &fdsRead, 0, &fdsExcept, 0)) > 0) {
	//cout << "Something happened" << endl;
	if (FD_ISSET(connectSocket, &fdsRead)) {
	  //cout << "Socket became readable" << endl;
	  ret = recv(connectSocket,rData,10000000,0);
	  if ( ret < 0 ) {
	    cerr << "Unable to get data" << endl;
	    return 1;
	  } else if ( ret == 0 ) {
	    cout << "Socket shutdown" << endl;
	  } else {
	    //cout << "Client shutdown" << endl;
	    //break;
	  //}

	  //cout << ret << endl;
	  //cout << sizeof(HCAL_HLX::LUMI_SECTION) << endl;

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
	      }
	    }
	  }
	} else if (FD_ISSET(connectSocket, &fdsExcept)) {
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
	goto restart_connection;
      }
      //usleep(1000);
    }

    delete []rData;

  }catch(std::exception & aExc){
    std::cerr<<aExc.what()<<std::endl;
  }catch(...){
    std::cerr<<"Unknown exception caught."<<std::endl;
  }




  return 0;
}
