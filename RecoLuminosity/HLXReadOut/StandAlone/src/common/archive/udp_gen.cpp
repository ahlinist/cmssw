#include <vector>
#include <string>
#include <exception>
#include <iostream>
#include <fstream>
#include <sstream>

#include <signal.h>

//#include "ICUtilityToolbox.hh"
//#include "ICException.hh"
//#include "EmulatorInterface.hh"
//#include "ICTypeDefs.hh"

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

const char sourceAddress[] = "192.168.1.100";
const char targetAddress[] = "192.168.1.103";
const unsigned short sourcePort = 0x3B53;
const unsigned short destPort = 0x3B53;

int main(int argc, char ** argv)
{
  signal(SIGINT,CtrlC);
  try{
    
    // Open a UDP socket
    int udp_socket = socket(PF_INET,SOCK_DGRAM,0);
    if ( udp_socket == -1 ) {
      cerr << "Socket could not be opened" << endl;
      return 1;
    }

    // Create local port and ip structure
    sockaddr_in sa_local;
    sa_local.sin_port = sourcePort;
    sa_local.sin_addr.s_addr = inet_addr(sourceAddress);
    sa_local.sin_family = AF_INET;

    // Bind to a local port to receive data
    int ret = bind(udp_socket,(struct sockaddr *)&sa_local,sizeof(sa_local));
    if ( ret == -1 ) {
      cerr << "Socket could not be bound" << endl;
      return 1;
    }
    
    int j=1;
    int goodPacketCount = 0, missingPacketCount = 0;
    int startTime, tempTime, interTime = 0;
    time((time_t*)&startTime);

    while (gContinue) {

      time((time_t*)&tempTime);
      j++;

      if ( tempTime != interTime ) {
        cout << j << "\tGood: " << goodPacketCount << "\tBad: " << 
missingPacketCount << endl;
        cout << tempTime-startTime << "s (" << 
(static_cast<double>(j)*1400.0/1024.0)/static_cast<double>(tempTime-startTime) << "kB/s)" << endl;      
interTime = tempTime;
}

      const int dataSize = 1400;
      unsigned char data[dataSize];
      for ( int i = 0 ; i != dataSize ; i++ ) {
	data[i] = static_cast<unsigned char>(i);
      }

      sockaddr_in sa_target;
      sa_target.sin_port = destPort;
      sa_target.sin_addr.s_addr = inet_addr(targetAddress);
      sa_target.sin_family = AF_INET;
      
      //    while (gContinue) {
      ret = sendto(udp_socket,data,dataSize,0,(struct sockaddr *)&sa_target,sizeof(sa_target)); 
      //}
      if ( ret == -1 ) {
	cerr << "Unable to send data" << endl;
	return 1;
      }
      /*ret = sendto(udp_socket,data,dataSize,0,(struct sockaddr *)&sa_target,sizeof(sa_target)); 
      //}
      if ( ret == -1 ) {
	cerr << "Unable to send data" << endl;
	return 1;
      }
      ret = sendto(udp_socket,data,dataSize,0,(struct sockaddr *)&sa_target,sizeof(sa_target)); 
      //}
      if ( ret == -1 ) {
	cerr << "Unable to send data" << endl;
	return 1;
      }
      ret = sendto(udp_socket,data,dataSize,0,(struct sockaddr *)&sa_target,sizeof(sa_target)); 
      //}
      if ( ret == -1 ) {
	cerr << "Unable to send data" << endl;
	return 1;
	}*/

      fd_set fds;
      struct timeval tv;

      // sock is an intialized socket handle
      tv.tv_sec = 1;//2;
      tv.tv_usec = 100000; //500000;
      // tv now represents 2.5 seconds

      FD_ZERO(&fds);
      FD_SET(udp_socket, &fds); // adds sock to the file descriptor set

      /* wait 2.5 seconds for any data to be read from any single socket */
      select(udp_socket+1, &fds, NULL, NULL, &tv);

      if (FD_ISSET(udp_socket, &fds)&&gContinue) {
	unsigned char rData[dataSize];
	socklen_t fromLen = sizeof(sa_target);
	ret = recvfrom(udp_socket,rData,dataSize,0,(struct sockaddr *)&sa_target,&fromLen);
	if ( ret == -1 ) {
	  cerr << "Unable to get data" << endl;
	  return 1;
	}
	
	int i;
	for ( i = 0 ; i != dataSize ; i++ ) {
	  if ( data[i] != rData[i] ) break;
	}
	if ( i != dataSize ) {
	  cerr << "Bad data set" << endl;
	  return 1;
	}

	goodPacketCount++;
      } else {
	missingPacketCount++;
      }
      //      cout << "a" << endl;

    }
    //cout << "b" <<endl;
/*
    ret = shutdown(udp_socket,0);
    if ( ret == -1 ) {
      cerr << "Socket could not be shut down" << endl;
      return 1;
    }
    */

  }catch(std::exception & aExc){
    std::cerr<<aExc.what()<<std::endl;
  }catch(...){
    std::cerr<<"Unknown exception caught."<<std::endl;
  }




  return 0;
}
