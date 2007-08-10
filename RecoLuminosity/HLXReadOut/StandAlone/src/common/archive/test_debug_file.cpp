#include <vector>
#include <string>
#include <exception>
#include <iostream>
#include <fstream>
#include <sstream>
#include <signal.h>

// NibbleCollector class
#include "DebugFileDistributor.hh"
#include "DebugCoutDistributor.hh"
#include "NibbleCollector.hh"
#include "SectionCollector.hh"

int gContinue=1;

void CtrlC(int aSigNum) {
  std::cout << "Ctrl-c detected, stopping run" << std::endl;
  gContinue=0;
}

using namespace std;
using namespace HCAL_HLX;

int main(int argc, char ** argv) {
  signal(SIGINT,CtrlC);
  try {

    SectionCollector lSectionCollector(200,  // Num bunches
				       200,  // Num nibbles per section
				       300,  // Num orbits in lumi nibble
				       1);   // Num HLXs
    NibbleCollector lNibbleCollector(1);
    lNibbleCollector.AttachSectionCollector(reinterpret_cast<AbstractSectionCollector *>(&lSectionCollector));
    DebugFileDistributor lDebugFileDistributor("debug.txt");
    DebugCoutDistributor lCouts[2];
    lSectionCollector.AttachDistributor(reinterpret_cast<AbstractDistributor *>(&lDebugFileDistributor));
    lSectionCollector.AttachDistributor(lCouts);
    lSectionCollector.AttachDistributor(lCouts+1);

    int startTime, tempTime, interTime = 0;
    time((time_t*)&startTime);
    tempTime=startTime;

    while (gContinue) {
      lNibbleCollector.RunServiceHandler();
      time((time_t*)&tempTime);
      if ( tempTime != interTime ) {
	cout << endl << tempTime-startTime << endl;
	cout << "Good packet count: " << lNibbleCollector.GetNumGoodPackets() << endl;
	cout << "Bad packet count: " << lNibbleCollector.GetNumBadPackets() << endl;
	cout << "Good nibble count: " << lNibbleCollector.GetNumGoodNibbles() << endl;
	cout << "Bad nibble count: " << lNibbleCollector.GetNumBadNibbles() << endl;
	cout << "Good section count:" << lSectionCollector.GetNumCompleteLumiSections() << endl;
	cout << "Bad section count: " << lSectionCollector.GetNumIncompleteLumiSections() << endl;
	cout << "Lost packet count: " << lNibbleCollector.GetNumLostPackets() << endl;
	cout << "Total data volume: " << lNibbleCollector.GetTotalDataVolume() << endl;
	cout << "Average data rate (Mb/s): " << (double)lNibbleCollector.GetTotalDataVolume()*8.0/(1024.0*1024.0*(double)(tempTime-startTime)) << endl;
	interTime = tempTime;
      }
    }

    return 0;

    /*

    // Two sets of nibble buffers
    ET_SUM_LUMI_NIBBLE etSumNibble;
    OCCUPANCY_LUMI_NIBBLE occNibbles[7];

    // Initialise the markers
    etSumNibble.hdr.startOrbit=0;
    for ( int j = 0 ; j != 6 ; j++ ) occNibbles[j].hdr.startOrbit=0;

    // Data counters
    unsigned int etSumDataCounter=0;
    unsigned int occDataCounters[7]={0};
    unsigned int dataVolume=0;
    int j=0;
    int goodNibbleCount = 0, badNibbleCount = 0;
    int startTime, tempTime, interTime = 0;
    time((time_t*)&startTime);

    while (gContinue) {

      //time((time_t*)&tempTime);
      //j++;


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
      //ret = sendto(udp_socket,data,dataSize,0,(struct sockaddr 
      // *)&sa_target,sizeof(sa_target)); 
      //}
      if ( ret == -1 ) {
	cerr << "Unable to send data" << endl;
	return 1;
      }

      fd_set fds;
      struct timeval tv;

      // sock is an intialized socket handle
      tv.tv_sec = 1;//2;
      tv.tv_usec = 0; //500000;
      // tv now represents 2.5 seconds

      FD_ZERO(&fds);
      FD_SET(udp_socket, &fds); // adds sock to the file descriptor set

      // wait 2.5 seconds for any data to be read from any single socket
      select(udp_socket+1, &fds, NULL, NULL, &tv);

      if (FD_ISSET(udp_socket, &fds)&&gContinue) {
	unsigned char rData[dataSize];
	socklen_t fromLen = sizeof(sa_target);
	ret = recvfrom(udp_socket,rData,dataSize,0,(struct sockaddr *)&sa_target,&fromLen);
	if ( ret == -1 ) {
	  cerr << "Unable to get data" << endl;
	  return 1;
	}
	
	//int i;
	//cout << endl << endl << "New packet:" << endl << endl;

	LUMI_RAW_HEADER *lumiHdr;
	lumiHdr = reinterpret_cast<LUMI_RAW_HEADER *>(rData);

	//cout << "Marker: 0x" << hex << static_cast<unsigned short>(lumiHdr.marker) << endl;
 	//cout << "HLX ID: " << dec << static_cast<unsigned short>(lumiHdr.hlxID) << endl;
 	//cout << "Packet ID " << dec << static_cast<unsigned short>(lumiHdr.packetID) << endl;
 	//cout << "Start orbit " << dec << lumiHdr.startOrbit << endl;
 	//cout << "Finish orbit " << dec << lumiHdr.finishOrbit << endl;
 	//cout << "Histogram set " << dec << static_cast<unsigned short>(lumiHdr.histogramSet) << endl;
 	//cout << "Histogram sequence " << dec << static_cast<unsigned short>(lumiHdr.histogramSequence) << endl;
 	//cout << "All As: 0x" << hex << static_cast<unsigned short>(lumiHdr.allA) << endl;
 	//cout << "All 5s: 0x" << hex << static_cast<unsigned short>(lumiHdr.all5) << endl;
 	//cout << "All Fs: 0x" << hex << static_cast<unsigned short>(lumiHdr.allF) << endl;
	//dataVolume += ret;

	//unsigned char checkSum = DoChecksum(rData,ret-1);
	//cout << hex << static_cast<unsigned short>(checkSum) << "\t"
	//     << static_cast<unsigned short>(rData[ret-1]) << endl;

	// Check for valid checksum
	//if ( checkSum == rData[ret-1] ) {
	  if ( lumiHdr->histogramSet == 7 ) {

	    // Et sum histogram
	    // First check to see if the start orbit matches the previous one, otherwise we discard
	    if ( lumiHdr->startOrbit != etSumNibble.hdr.startOrbit ) {
	      // Check for incomplete histogram
	      if ( etSumNibble.hdr.startOrbit != 0
		   && etSumNibble.hdr.bIsComplete == false ) { 
		badNibbleCount++;
		//cout << lumiHdr->startOrbit << "\t" << etSumNibble.hdr.startOrbit << endl;
	      }
	      
	      // Clear data counter and reset startOrbit marker
	      etSumDataCounter=0;
	      etSumNibble.hdr.startOrbit = lumiHdr->startOrbit;
	      etSumNibble.hdr.numBunches = lumiHdr->numBunches+1;
	      etSumNibble.hdr.numOrbits = lumiHdr->numOrbits+1;
	      etSumNibble.hdr.hlxID = lumiHdr->hlxID;
	      etSumNibble.hdr.bIsComplete = false;
	    }
	    
	    // Copy the data into the local buffer
	    memcpy(etSumNibble.data+lumiHdr->startBunch,rData+sizeof(LUMI_RAW_HEADER),ret-1-sizeof(LUMI_RAW_HEADER));
	    
	    // Update the data counter and check for completion
	    etSumDataCounter += (ret-1-sizeof(LUMI_RAW_HEADER))>>2; // Divide by 4 for longs
	    if ( etSumDataCounter == etSumNibble.hdr.numBunches ) {
	      etSumNibble.hdr.bIsComplete = true;
	      //cout << "ET sum nibble completed" << endl;
	      goodNibbleCount++;
	    }
	    
	  } else if ( lumiHdr->histogramSet < 7 ) {
	    
	    // Occupancy histograms
	    // First check to see if the start orbit matches the previous one, otherwise we discard
	    if ( lumiHdr->startOrbit != occNibbles[lumiHdr->histogramSet].hdr.startOrbit ) {

	      // Check for incomplete histogram
	      if ( occNibbles[lumiHdr->histogramSet].hdr.startOrbit != 0
		   && occNibbles[lumiHdr->histogramSet].hdr.bIsComplete == false ) { 
		badNibbleCount++;
	      }
	      
	      // Clear data counter and reset startOrbit marker
	      occDataCounters[lumiHdr->histogramSet]=0;
	      occNibbles[lumiHdr->histogramSet].hdr.startOrbit = lumiHdr->startOrbit;
	      occNibbles[lumiHdr->histogramSet].hdr.numBunches = lumiHdr->numBunches+1;
	      occNibbles[lumiHdr->histogramSet].hdr.numOrbits = lumiHdr->numOrbits+1;
	      occNibbles[lumiHdr->histogramSet].hdr.hlxID = lumiHdr->hlxID;
	      occNibbles[lumiHdr->histogramSet].hdr.bIsComplete = false;
	    }
	    
	    // Copy the data into the local buffer
	    memcpy(occNibbles[lumiHdr->histogramSet].data+lumiHdr->startBunch,rData+sizeof(LUMI_RAW_HEADER),ret-1-sizeof(LUMI_RAW_HEADER));
	    
	    // Update the data counter and check for completion
	    occDataCounters[lumiHdr->histogramSet] += (ret-1-sizeof(LUMI_RAW_HEADER))>>1; // Divide by 2 for shorts
	    //cout << occDataCounters[lumiHdr->histogramSet] << "\t" << 
	    if ( occDataCounters[lumiHdr->histogramSet] == occNibbles[lumiHdr->histogramSet].hdr.numBunches ) {
	      occNibbles[lumiHdr->histogramSet].hdr.bIsComplete = true;
	      //cout << "Occupancy nibble " << lumiHdr->histogramSet << " completed" << endl;
	      goodNibbleCount++;
	    }

	  } else {
	    cout << "Bad packet" << endl;
	  }

	time((time_t *)&tempTime);
	j++;
	if ( tempTime != interTime ) {
	  cout << tempTime-startTime << endl;
	  cout << endl << "Packet: " << dec << j << endl;
	  cout << "Data volume (MB): " << dec << (double)dataVolume/(1024.0*1024.0) << endl;
	  cout << "Rate (Mb/s): " << dec << (double)dataVolume*8.0/(1000.0*1000.0*(double)(tempTime-startTime)) << endl;
	  cout << "Good nibble count: " << dec << goodNibbleCount << endl;
	  cout << "Bad nibble count: " << dec << badNibbleCount << endl;
	  interTime = tempTime;
	}

	//for ( i = 0 ; i != ret ; i++ ) {
	//  cout << dec << i << "\t" << hex << static_cast<unsigned short>(rData[i]) << endl;
	//}


	// goodPacketCount++;
	//      } 
// else {
      //	missingPacketCount++;
      //}
      //      cout << "a" << endl;

      }
    }*/

  }catch(std::exception & aExc){
    std::cerr<<aExc.what()<<std::endl;
  }catch(...){
    std::cerr<<"Unknown exception caught."<<std::endl;
  }




  return 0;
}
