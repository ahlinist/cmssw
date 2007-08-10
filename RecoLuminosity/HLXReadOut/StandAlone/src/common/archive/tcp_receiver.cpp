#include <vector>
#include <string>
#include <exception>
#include <iostream>
#include <fstream>
#include <sstream>

#include <signal.h>
#include <unistd.h>

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

int main(int argc, char ** argv)
{
  signal(SIGINT,CtrlC);
  try{
    
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

    fd_set fds;
    struct timeval tv;
    
    // sock is an intialized socket handle
    tv.tv_sec = 1;
    tv.tv_usec = 100000;
      
    while (gContinue) {
      
      FD_ZERO(&fds);
      FD_SET(tcp_socket, &fds);

      if ( select(tcp_socket+1,&fds,0,0,&tv) > 0 ) {
	cout << "something happened" << endl;
	if ( FD_ISSET(tcp_socket, &fds) ) {
	  
	  cout << "Client connecting" << endl;
	  
	  clientAddressLength = sizeof(clientAddress);
	  connectSocket = accept(tcp_socket,
				 (struct sockaddr *) & clientAddress,
				 &clientAddressLength); 
	  if ( connectSocket < 0 ) {
	    cerr << "Couldn't accept incoming connection" << endl;
	    return 1;
	  }

	  cout << "Client connected" << endl;  
	  
	  // Go into data readout loop
	  break;
	  
	}
      }
      usleep(1000000);

    }
    
    int z=0, localCount=0;
    time_t tempTime, curTime;
    time(&curTime);

    // Temporary storage of a lumi section
    LUMI_SECTION localSection;

    while (gContinue) {

      FD_ZERO(&fds);
      FD_SET(connectSocket, &fds); // adds sock to the file descriptor set

      select(connectSocket+1, &fds, NULL, NULL, 0);

      if (FD_ISSET(connectSocket, &fds)&&gContinue) {

	//cout << "Receiving data" << endl;
	ret = recv(connectSocket,rData,1000000,0);
	if ( ret == -1 ) {
	  cerr << "Unable to get data" << endl;
	  return 1;
	} else if ( ret < 0 ) {
	  cout << "Client shutdown" << endl;
	  break;
	} //else {
	  //cout << rData << endl;
	//}

	if ( ret+localCount > sizeof(HCAL_HLX::LUMI_SECTION) ) {
	  memcpy(reinterpret_cast<char *>(&localSection)+localCount,
		 rData,
		 sizeof(HCAL_HLX::LUMI_SECTION)-localCount);
	  cout << "A: Lumi section completed" << endl;
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

	time(&tempTime);
	if ( tempTime-curTime != 0 ) {
	  cout << z << endl;
	  curTime=tempTime;
	}

      }

      //usleep(1000000);

    }

    delete []rData;

  }catch(std::exception & aExc){
    std::cerr<<aExc.what()<<std::endl;
  }catch(...){
    std::cerr<<"Unknown exception caught."<<std::endl;
  }




  return 0;
}
