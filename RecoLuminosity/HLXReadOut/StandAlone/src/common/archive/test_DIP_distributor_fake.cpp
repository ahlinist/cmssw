#include <vector>
#include <string>
#include <exception>
#include <iostream>
#include <fstream>
#include <sstream>
#include <signal.h>
#include <unistd.h>

// Exception classes for HLX code
#include "ICException.hh"
#include "HardwareAccessException.hh"
#include "MemoryAllocationException.hh"

// Classes for HLX data distribution
#include "DIPDistributor.hh"
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

    SectionCollector lSectionCollector(300, // Num bunches
				       2, // Num nibbles per section
				       1, // Num orbits in lumi nibble
				       1);  // Num HLXs
    NibbleCollector lNibbleCollector(1);
    lNibbleCollector.AttachSectionCollector(reinterpret_cast<AbstractSectionCollector *>(&lSectionCollector));
    DIPDistributor lDIPDistributor;
    lSectionCollector.AttachDistributor(&lDIPDistributor);

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
	cout << "Lost section count: " << lSectionCollector.GetNumLostLumiSections() << endl;
	cout << "Lost packet count: " << lNibbleCollector.GetNumLostPackets() << endl;
	cout << "Total data volume: " << lNibbleCollector.GetTotalDataVolume() << endl;
	cout << "Average data rate (Mb/s): " << (double)lNibbleCollector.GetTotalDataVolume()*8.0/(1024.0*1024.0*(double)(tempTime-startTime)) << endl;
	interTime = tempTime;
      }
      usleep(1000);
    }

  }catch(ICException & aExc){
    cout << "bollocks" << endl;
    // doesn't work if you print the exception!
    //cout << aExc.what() << endl;
    //std::cerr<<aExc.what()<<std::endl;
    cout << "bollocks2" << endl;
  }catch(std::exception & aExc){
    std::cerr<<aExc.what()<<std::endl;
  }catch(...){
    std::cerr<<"Unknown exception caught."<<std::endl;
  }

  return 0;
}
