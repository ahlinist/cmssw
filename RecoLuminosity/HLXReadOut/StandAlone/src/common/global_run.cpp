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
#include "ROOTDistributor.hh"
#include "DIPDistributor.hh"
#include "TCPDistributor.hh"
#include "DebugCoutDistributor.hh"
#include "GIFDistributor.hh"

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
#define SHORT_LENGTH 3
#define LONG_LENGTH 45
#define NUM_BUNCHES 3564
#define NUM_ORBITS 4096

int main(int argc, char ** argv) {
  signal(SIGINT,CtrlC);
  SectionCollector *lSectionCollectorShort = 0;
  SectionCollector *lSectionCollectorLong = 0;
  NibbleCollector *lNibbleCollector = 0;
  // The distributors
  OracleDistributor *lOracleDistributor = 0;
  DIPDistributor *lDIPDistributor = 0;
  ROOTDistributor *lROOTDistributor = 0;
  TCPDistributor *lTCPDistributor = 0;
  DebugCoutDistributor *lDebugCoutDistributor = 0;
  GIFDistributor *lGIFDistributor = 0;

  // Get the run number
  if ( argc != 2 ) {
    cerr << "Expected 1 argument but found " << argc-1 << endl;
    return 1;
  }
  u32 runNumber = atol(argv[1]);

  try {
    // One second
    lSectionCollectorShort = new SectionCollector(NUM_BUNCHES, // Num bunches
				         	  SHORT_LENGTH,   // Num nibbles per section
					          NUM_ORBITS,   // Num orbits in lumi nibble
					          NUM_HLXS);  // Num HLXs
    // Ten seconds
    lSectionCollectorLong = new SectionCollector(NUM_BUNCHES, // Num bunches
						 LONG_LENGTH,   // Num nibbles per section
						 NUM_ORBITS,   // Num orbits in lumi nibble
						 NUM_HLXS);  // Num HLXs
    // Set the run number
    lSectionCollectorShort->SetRunNumber(runNumber);
    lSectionCollectorLong->SetRunNumber(runNumber);

    lOracleDistributor = new OracleDistributor;
    lSectionCollectorLong->AttachDistributor(lOracleDistributor);
    lROOTDistributor = new ROOTDistributor(runNumber);
    lSectionCollectorLong->AttachDistributor(lROOTDistributor);

    lGIFDistributor = new GIFDistributor;
    lSectionCollectorShort->AttachDistributor(lGIFDistributor);
    lDebugCoutDistributor = new DebugCoutDistributor;
    lSectionCollectorShort->AttachDistributor(lDebugCoutDistributor);
    lDIPDistributor = new DIPDistributor;
    lSectionCollectorShort->AttachDistributor(lDIPDistributor);
    lTCPDistributor = new TCPDistributor;
    lSectionCollectorShort->AttachDistributor(lTCPDistributor);


    lNibbleCollector = new NibbleCollector(NUM_HLXS);
    lNibbleCollector->AttachSectionCollector(lSectionCollectorShort);
    lNibbleCollector->AttachSectionCollector(lSectionCollectorLong);

    int startTime, tempTime, interTime = 0;
    time((time_t*)&startTime);
    tempTime=startTime;
    interTime=tempTime;

    while (gContinue) {
      Sleep(1);
      lNibbleCollector->RunServiceHandler();
      time((time_t*)&tempTime);
      if ( tempTime-interTime >= 5 ) {
	cout << endl << tempTime-startTime << endl;
	cout << "Good packet count: " << lNibbleCollector->GetNumGoodPackets() << endl;
	cout << "Bad packet count: " << lNibbleCollector->GetNumBadPackets() << endl;
	cout << "Good et nibble count: " << lNibbleCollector->GetNumGoodETSumNibbles() << endl;
	cout << "Bad et nibble count: " << lNibbleCollector->GetNumBadETSumNibbles() << endl;
	cout << "Good section count:" << lSectionCollectorLong->GetNumCompleteLumiSections() << endl;
	cout << "Bad section count: " << lSectionCollectorLong->GetNumIncompleteLumiSections() << endl;
	cout << "Lost section count: " << lSectionCollectorLong->GetNumLostLumiSections() << endl;
	cout << "Lost packet count: " << lNibbleCollector->GetNumLostPackets() << endl;
	cout << "Total data volume: " << lNibbleCollector->GetTotalDataVolume() << endl;
	cout << "Average data rate (Mb/s): " << (double)lNibbleCollector->GetTotalDataVolume()*8.0/(1024.0*1024.0*(double)(tempTime-startTime)) << endl;
	interTime = tempTime;
      }
    }
        
  }catch(ICException & aExc){
    cerr << aExc.what() << endl;
  }catch(std::exception & aExc){
    cerr << aExc.what()<<std::endl;
  }catch(...){
    cerr <<"Unknown exception caught."<<std::endl;
  }

  delete lNibbleCollector;
  delete lSectionCollectorShort;
  delete lSectionCollectorLong;
  delete lOracleDistributor;
  /*delete lDIPDistributor;
  delete lROOTDistributor;
  delete lTCPDistributor;
  delete lDebugCoutDistributor;
  delete lGIFDistributor;*/
    
  return 0;
}
