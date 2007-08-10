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

void InterpretSection(const LUMI_SECTION & lumiSection) {

  //cout << "HLX ID: " << lumiSection.hdr.hlxID << endl;
  cout << "Run number: " << lumiSection.hdr.runNumber << endl;
  cout << "Num orbits: " << lumiSection.hdr.numOrbits << endl;
  cout << "Num bunches: " << lumiSection.hdr.numBunches << endl;
  cout << "Start orbit: " << lumiSection.hdr.startOrbit << endl;
  cout << "CMS live: " << lumiSection.hdr.bCMSLive << endl;

}

void InitializePort(int &tcp_socket, unsigned short port)
{
  // Open a TCP socket
  tcp_socket = socket(AF_INET,SOCK_STREAM,0);
  if ( tcp_socket == -1 ) {
    cerr << "Socket could not be opened" << endl;
  }else{
    
    // Create local port and ip structure
    sockaddr_in sa_local;
    sa_local.sin_port = htons(port);
    sa_local.sin_addr.s_addr = htonl(INADDR_ANY);
    sa_local.sin_family = AF_INET;

    // Bind to a local port to receive data
    int ret = bind(tcp_socket,(struct sockaddr *)&sa_local,sizeof(sa_local));
    if ( ret == -1 ) {
      cerr << "Socket could not be bound" << endl;
    }
  }
}

int ConnectToClient(int &tcp_socket, int &connectSocket, fd_set &fds)
{
  struct timeval tv;
  socklen_t clientAddressLength;
  struct sockaddr_in clientAddress;

  // Assign to listening port
  listen(tcp_socket,1);

  // Wait for connection to client
  cout << "Waiting for client connection" << endl;
    
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
	  connectSocket = accept(tcp_socket, (struct sockaddr *) & clientAddress, &clientAddressLength); 
	  if ( connectSocket < 0 ) {
	    cerr << "Couldn't accept incoming connection" << endl;
	    return 1;
	  }
	  cout << "Client connected" << endl;  
	  return 0;
	}
      }
      usleep(1000000);
    }  
  return 0;
}


int AquireSection(LUMI_SECTION &localSection, int &connectSocket, fd_set & fds)
{
  // Temporary storage of a lumi section

  int localCount=0, ret;
  unsigned char *rData = new unsigned char[1000000];


  FD_ZERO(&fds);
  FD_SET(connectSocket, &fds); // adds sock to the file descriptor set

  select(connectSocket+1, &fds, NULL, NULL, 0);

  if (FD_ISSET(connectSocket, &fds)&&gContinue) {
    ret = recv(connectSocket,rData,1000000,0);
    if ( ret == -1 ) {
      cerr << "Unable to get data" << endl;
      return 55;
    } else if ( ret < 0 ) {
      cout << "Client shutdown" << endl;
      return 56;
    }

    if ( (unsigned int)ret+localCount > sizeof(HCAL_HLX::LUMI_SECTION) ) {
      memcpy(reinterpret_cast<char *>(&localSection)+localCount, rData, sizeof(HCAL_HLX::LUMI_SECTION)-localCount);
      cout << "A: Lumi section completed" << endl;
      localCount = ret - sizeof(HCAL_HLX::LUMI_SECTION) + localCount;
      memcpy(reinterpret_cast<char *>(&localSection), rData+ret-localCount, localCount);
    } else {
      memcpy(reinterpret_cast<char *>(&localSection)+localCount,rData,ret);
      localCount += ret;
      if ( localCount == sizeof(HCAL_HLX::LUMI_SECTION) ) {
	cout << "B: Lumi section completed" << endl;
	localCount = 0;
      }
    }
  }
  delete []rData;
  return 0;      
}

int main(int argc, char ** argv)
{
  int Tsocket;
  int Csocket;
  fd_set fds;

  LUMI_SECTION section;

  signal(SIGINT,CtrlC);

  try{
    InitializePort(Tsocket, 50002);
  
    ConnectToClient(Tsocket,Csocket, fds);

    while(gContinue) {
      AquireSection(section, Csocket, fds);
      if(section.hdr.numOrbits!=0)
	InterpretSection(section);
    }

  }catch(std::exception & aExc){
    std::cerr<<aExc.what()<<std::endl;
  }catch(...){
    std::cerr<<"Unknown exception caught."<<std::endl;}

  return 0;
}
