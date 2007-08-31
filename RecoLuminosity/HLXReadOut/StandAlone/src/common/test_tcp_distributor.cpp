#include <vector>
#include <string>
#include <exception>
#include <iostream>
#include <fstream>
#include <sstream>
#include <signal.h>
#include <unistd.h>

#include "ICException.hh"
#include "HardwareAccessException.hh"
#include "MemoryAllocationException.hh"

// NibbleCollector class
#include "TCPDistributor.hh"
#include "NibbleCollector.hh"
#include "SectionCollector.hh"

int gContinue=1;

void CtrlC(int aSigNum) {
  std::cout << "Ctrl-c detected, stopping run" << std::endl;
  gContinue=0;
}

using namespace std;
using namespace HCAL_HLX;
using namespace ICCoreUtils;

int main(int argc, char ** argv) {
  signal(SIGINT,CtrlC);
  try {
    SectionCollector *lSectionCollector = new SectionCollector(300, // Num bunches
							       5,   // Num nibbles per section
							       1,   // Num orbits in lumi nibble
							       1);  // Num HLXs
    NibbleCollector *lNibbleCollector = new NibbleCollector(1);
    lNibbleCollector->AttachSectionCollector(lSectionCollector);
    TCPDistributor *lTCPDistributor = new TCPDistributor("127.0.0.1",50002);
    lSectionCollector->AttachDistributor(lTCPDistributor);

    int startTime, tempTime, interTime = 0;
    time((time_t*)&startTime);
    tempTime=startTime;

    while (gContinue) {
      lNibbleCollector->RunServiceHandler();
      time((time_t*)&tempTime);
      if ( tempTime != interTime ) {
	cout << endl << tempTime-startTime << endl;
	cout << "Good packet count: " << lNibbleCollector->GetNumGoodPackets() << endl;
	cout << "Bad packet count: " << lNibbleCollector->GetNumBadPackets() << endl;
	cout << "Good nibble count: " << lNibbleCollector->GetNumGoodETSumNibbles() << endl;
	cout << "Bad nibble count: " << lNibbleCollector->GetNumBadETSumNibbles() << endl;
	cout << "Good section count:" << lSectionCollector->GetNumCompleteLumiSections() << endl;
	cout << "Bad section count: " << lSectionCollector->GetNumIncompleteLumiSections() << endl;
	cout << "Lost section count: " << lSectionCollector->GetNumLostLumiSections() << endl;
	cout << "Lost packet count: " << lNibbleCollector->GetNumLostPackets() << endl;
	cout << "Total data volume: " << lNibbleCollector->GetTotalDataVolume() << endl;
	cout << "Average data rate (Mb/s): " << (double)lNibbleCollector->GetTotalDataVolume()*8.0/(1024.0*1024.0*(double)(tempTime-startTime)) << endl;
	interTime = tempTime;
      }
      usleep(1000);
    }

  }catch(ICException & aExc){
    // doesn't work if you print the exception!
    cerr << aExc.what() << endl;
  }catch(std::exception & aExc){
    std::cerr<<aExc.what()<<std::endl;
  }catch(...){
    std::cerr<<"Unknown exception caught."<<std::endl;
  }

  return 0;
}
