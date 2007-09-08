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

// HLX interface classes
#include "OracleDistributor.hh"
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

#define NUM_HLXS 13

int main(int argc, char ** argv) {
  signal(SIGINT,CtrlC);
  SectionCollector *lSectionCollector = 0;
  NibbleCollector *lNibbleCollector = 0;
  OracleDistributor *lOracleDistributor = 0;
  try {
    // send approximately every 10 seconds
    lSectionCollector = new SectionCollector(3564,    // Num bunches
					     3,       // Num nibbles per section
					     4096,    // Num orbits in lumi nibble
					     NUM_HLXS);  // Num HLXs
    lNibbleCollector = new NibbleCollector(NUM_HLXS);
    lNibbleCollector->AttachSectionCollector(lSectionCollector);
    lOracleDistributor = new OracleDistributor;
    lSectionCollector->AttachDistributor(lOracleDistributor);

    int startTime, tempTime, interTime = 0;
    time((time_t*)&startTime);
    tempTime=startTime;

    while (gContinue) {
      Sleep(1);
      lNibbleCollector->RunServiceHandler();
      time((time_t*)&tempTime);
      if ( tempTime != interTime ) {
	cout << endl << tempTime-startTime << endl;
	cout << "Good packet count: " << lNibbleCollector->GetNumGoodPackets() << endl;
	cout << "Bad packet count: " << lNibbleCollector->GetNumBadPackets() << endl;
	cout << "Good et nibble count: " << lNibbleCollector->GetNumGoodETSumNibbles() << endl;
	cout << "Bad et nibble count: " << lNibbleCollector->GetNumBadETSumNibbles() << endl;
	cout << "Good section count:" << lSectionCollector->GetNumCompleteLumiSections() << endl;
	cout << "Bad section count: " << lSectionCollector->GetNumIncompleteLumiSections() << endl;
	//cout << "Lost section count: " << lSectionCollector->GetNumLostLumiSections() << endl;
	cout << "Lost packet count: " << lNibbleCollector->GetNumLostPackets() << endl;
	cout << "Total data volume: " << lNibbleCollector->GetTotalDataVolume() << endl;
	cout << "Average data rate (Mb/s): " << (double)lNibbleCollector->GetTotalDataVolume()*8.0/(1024.0*1024.0*(double)(tempTime-startTime)) << endl;
	interTime = tempTime;
      }
      usleep(1000);
    }
        
  }catch(ICException & aExc){
    cerr << aExc.what() << endl;
  }catch(std::exception & aExc){
    cerr << aExc.what()<<std::endl;
  }catch(...){
    cerr <<"Unknown exception caught."<<std::endl;
  }

  delete lNibbleCollector;
  delete lSectionCollector;
  delete lOracleDistributor;
    
  return 0;
}
