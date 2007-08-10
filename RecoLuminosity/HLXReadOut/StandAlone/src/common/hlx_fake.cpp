#include <vector>
#include <string>
#include <exception>
#include <iostream>
#include <fstream>
#include <sstream>

#include <signal.h>
//#include <unistd.h>

#include "ICUtilityToolbox.hh"
//#include "ICException.hh"

#include "LumiStructures.hh"
#include "ICTypeDefs.hh"

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
using namespace ICCoreUtils;
using namespace HCAL_HLX;

const u32 TIMEOUT_PERIOD = 10;

// Variables (global)
u8 **crcTable;

// Prototype function declarations
u8 ComputeCheckSum(const u8 *data, u32 numBytes);
void InitialiseChecksum();
void CleanupChecksum();
u8 SingleChecksum(u8 a, u8 b);
u8 ChecksumHelper(u8 data, u8 prevCRC);
int main(int argc, char **argv);

void CleanupChecksum() {
  // CRC table
  for ( u32 i = 0 ; i != 256 ; i++ ) {
    if ( crcTable[i] ) {
      delete []crcTable;
      crcTable[i] = 0;
    }
  }
  if ( crcTable ) {
    delete []crcTable;
    crcTable = 0;
  }
}

u8 ComputeChecksum(const u8 *data, u32 numBytes) {
  u8 theCRC = 0xAA;
  for ( u32 i = 0 ; i != numBytes ; i++ ) {
    theCRC=SingleChecksum(data[i],theCRC);
  }
  return theCRC;
}

void InitialiseChecksum() {
  // This has to be dynamically allocated or it won't work
  crcTable = new (u8 *)[256];

  // Initialise the array
  for ( u32 i = 0 ; i != 256 ; i++ ) {
    crcTable[i] = 0;
  }

  // Allocate the circular buffer
  for ( u32 i = 0 ; i != 256 ; i++ ) {
    crcTable[i] = new u8[256];
    for ( u32 j = 0 ; j != 256 ; j++ ) {
      	crcTable[i][j] = ChecksumHelper(i,j);
    }
  }

}

u8 SingleChecksum(u8 a, u8 b) {
  return crcTable[a][b];
}

u8 ChecksumHelper(u8 data, u8 prevCRC) {
  u8 ret = 0;
  bool dataBits[8], prevCRCBits[8], nextCRCBits[8];

  for ( int i = 0 ; i != 8 ; i++ ) {
    dataBits[i] = (data & (0x1 << i)) ? true : false;
    prevCRCBits[i] = (prevCRC & (0x1 << i)) ? true : false;
  }

    nextCRCBits[0] = dataBits[7] ^ dataBits[6] ^ dataBits[0]
      ^ prevCRCBits[0] ^ prevCRCBits[6] ^ prevCRCBits[7];
    nextCRCBits[1] = dataBits[6] ^ dataBits[1] ^ dataBits[0]
      ^ prevCRCBits[0] ^ prevCRCBits[1] ^ prevCRCBits[6];
    nextCRCBits[2] = dataBits[6] ^ dataBits[2] ^ dataBits[1] ^ dataBits[0]
      ^ prevCRCBits[0] ^ prevCRCBits[1] ^ prevCRCBits[2] ^ prevCRCBits[6];
    nextCRCBits[3] = dataBits[7] ^ dataBits[3] ^ dataBits[2] ^ dataBits[1]
      ^ prevCRCBits[1] ^ prevCRCBits[2] ^ prevCRCBits[3] ^ prevCRCBits[7];
    nextCRCBits[4] = dataBits[4] ^ dataBits[3] ^ dataBits[2]
      ^ prevCRCBits[2] ^ prevCRCBits[3] ^ prevCRCBits[4];
    nextCRCBits[5] = dataBits[5] ^ dataBits[4] ^ dataBits[3]
      ^ prevCRCBits[3] ^ prevCRCBits[4] ^ prevCRCBits[5];
    nextCRCBits[6] = dataBits[6] ^ dataBits[5] ^ dataBits[4]
      ^ prevCRCBits[4] ^ prevCRCBits[5] ^ prevCRCBits[6];
    nextCRCBits[7] = dataBits[7] ^ dataBits[6] ^ dataBits[5]
      ^ prevCRCBits[5] ^ prevCRCBits[6] ^ prevCRCBits[7];

    for ( int i = 0 ; i != 8 ; i++ ) {
      ret += nextCRCBits[i]?(1<<i):0;
    }

    return ret;
}

const char sourceAddress[] = "192.168.1.100";
const char targetAddress[] = "192.168.1.100";
const unsigned short sourcePort = 0x3B53;
const unsigned short destPort = 0x3A53;

// Data to be transmitted
LUMI_RAW_HEADER lumiHeader = {
  0xFFFF, // marker (always ffff)
  0,      // hlx ID (always zero as only one for fake)
  0,      // packet ID
  1,      // start orbit
  0,      // num orbits
  0,      // start bunch (always zero for short orbit)
  299,    // num bunches
  0,      // histogram set
  0,      // histogram sequence
  0xAAAA,
  0xFFFF
};
u16 lumiOccData[300];
u32 lumiEtData[300];
u8 payloadData[2000];
u32 payloadVolume = 0;

void GeneratePacket() {
  memcpy(payloadData,&lumiHeader,sizeof(LUMI_RAW_HEADER));
  if ( lumiHeader.histogramSet == 7 ) {
    memcpy(payloadData+sizeof(LUMI_RAW_HEADER),lumiEtData,sizeof(u32)*300);   
    payloadData[sizeof(LUMI_RAW_HEADER)+sizeof(u32)*300] = ComputeChecksum(payloadData,sizeof(LUMI_RAW_HEADER)+sizeof(u32)*300);
    payloadVolume = sizeof(LUMI_RAW_HEADER)+sizeof(u32)*300+1;
  } else {
    memcpy(payloadData+sizeof(LUMI_RAW_HEADER),lumiOccData,sizeof(u16)*300);   
    payloadData[sizeof(LUMI_RAW_HEADER)+sizeof(u16)*300] = ComputeChecksum(payloadData,sizeof(LUMI_RAW_HEADER)+sizeof(u16)*300);
    payloadVolume = sizeof(LUMI_RAW_HEADER)+sizeof(u16)*300+1;
  }
}

int main(int argc, char ** argv)
{
  signal(SIGINT,CtrlC);
  try{
    u32 timeoutCount = TIMEOUT_PERIOD;
    InitialiseChecksum();

    // Initialise the data structures to a counter
    for ( u32 i = 0 ; i != lumiHeader.numBunches+1 ; i++ ) {
      lumiEtData[i] = i;
      lumiOccData[i] = i;
    }

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
    
    int j=0;
    int startTime, tempTime, interTime = 0;
    time((time_t*)&startTime);

    while (gContinue) {

      time((time_t*)&tempTime);
      j++;

      if ( tempTime != interTime ) {
        cout << dec << j << endl;
        cout << tempTime-startTime << "s" << endl;
	interTime = tempTime;
      }

      sockaddr_in sa_target;
      sa_target.sin_port = destPort;
      sa_target.sin_addr.s_addr = inet_addr(targetAddress);
      sa_target.sin_family = AF_INET;

      if ( lumiHeader.histogramSet == 7 ) {
	lumiHeader.histogramSet = 0;
	lumiHeader.startOrbit += lumiHeader.numOrbits+1;
	timeoutCount--;
	if ( timeoutCount == 0 ) {
	  Sleep(1);
          timeoutCount = TIMEOUT_PERIOD;
	}
      }	else {
	lumiHeader.histogramSet++;
      }
      GeneratePacket();

      ret = sendto(udp_socket,payloadData,payloadVolume,0,(struct sockaddr *)&sa_target,sizeof(sa_target)); 
      if ( ret == -1 ) {
	cerr << "Unable to send data" << endl;
	return 1;
      }

      // Sleep for 1ms between packets
      //usleep(10);

    }

  }catch(std::exception & aExc){
    std::cerr<<aExc.what()<<std::endl;
  }catch(...){
    std::cerr<<"Unknown exception caught."<<std::endl;
  }
  CleanupChecksum();
  return 0;
}
