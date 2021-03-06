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
//#include "ROOTDistributor.hh"
#include "DIPDistributor.hh"
#include "TCPDistributor.hh"
#include "DebugCoutDistributor.hh"
#include "GIFDistributor.hh"
#include "WedgeGIFDistributor.hh"

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

#define NUM_HLXS 36
#define SHORT_LENGTH 4
#define LONG_LENGTH 256
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
  //ROOTDistributor *lROOTDistributor = 0;
  TCPDistributor *lTCPDistributor = 0;
  DebugCoutDistributor *lDebugCoutDistributor = 0;
  GIFDistributor *lGIFDistributor = 0;
  WedgeGIFDistributor *lWedgeGIFDistributor[NUM_HLXS] = {0};

  u32 runNumber = 0 ;//atol(argv[1]);

  try {
    // One second
    lSectionCollectorShort = new SectionCollector(NUM_BUNCHES, // Num bunches
				         	  SHORT_LENGTH,   // Num nibbles per section
					          NUM_ORBITS,   // Num orbits in lumi nibble
					          NUM_HLXS,
						  runNumber);  // Num HLXs
    // Ten seconds
    lSectionCollectorLong = new SectionCollector(NUM_BUNCHES, // Num bunches
						 LONG_LENGTH,   // Num nibbles per section
						 NUM_ORBITS,   // Num orbits in lumi nibble
						 NUM_HLXS,
						 runNumber);  // Num HLXs
    // Set the run number
    lSectionCollectorShort->SetNextRunNumber(runNumber);
    lSectionCollectorLong->SetNextRunNumber(runNumber);

    //lOracleDistributor = new OracleDistributor;
    //lSectionCollectorLong->AttachDistributor(lOracleDistributor);
    //lROOTDistributor = new ROOTDistributor;
    //lSectionCollectorLong->AttachDistributor(lROOTDistributor);
    //lDebugCoutDistributor = new DebugCoutDistributor;
    //lSectionCollectorLong->AttachDistributor(lDebugCoutDistributor);

    lGIFDistributor = new GIFDistributor;
    lSectionCollectorLong->AttachDistributor(lGIFDistributor);
    lDIPDistributor = new DIPDistributor;
    lSectionCollectorShort->AttachDistributor(lDIPDistributor);
    lTCPDistributor = new TCPDistributor;
    lSectionCollectorShort->AttachDistributor(lTCPDistributor);
    //for ( u32 i = 0 ; i != NUM_HLXS ; i++ ) {
    //lWedgeGIFDistributor[i] = new WedgeGIFDistributor(i);
      //lSectionCollectorLong->AttachDistributor(lWedgeGIFDistributor[i]);
    //}

    lNibbleCollector = new NibbleCollector(NUM_HLXS,
					   NUM_BUNCHES,
					   NUM_ORBITS,
					   0x533A,
					   "BROADCAST");
    lNibbleCollector->AttachSectionCollector(lSectionCollectorShort);
    lNibbleCollector->AttachSectionCollector(lSectionCollectorLong);

    lNibbleCollector->Start();

    int startTime, tempTime, interTime = 0;
    time((time_t*)&startTime);
    tempTime=startTime;
    interTime=tempTime;

    while (gContinue) {
      Sleep(100);
      //lNibbleCollector->RunServiceHandler();
      time((time_t*)&tempTime);
      if ( tempTime-interTime >= 5 ) {
	cout << endl << endl;
	cout << "-------------------------------------------------------" << endl;
	cout << "Debug Information (time = " << dec << tempTime-startTime << "s)" << endl;
	cout << "-------------------------------------------------------" << endl;
	cout << "Good packet count: " << lNibbleCollector->GetNumGoodPackets() << endl;
	cout << "Bad packet count: " << lNibbleCollector->GetNumBadPackets() << endl;
	cout << "Good et nibble count: " << lNibbleCollector->GetNumGoodETSumNibbles() << endl;
	cout << "Bad et nibble count: " << lNibbleCollector->GetNumBadETSumNibbles() << endl;
	cout << "Good occupancy nibble count: " << lNibbleCollector->GetNumGoodOccupancyNibbles() << endl;
	cout << "Bad occupancy nibble count: " << lNibbleCollector->GetNumBadOccupancyNibbles() << endl;
	cout << "Good LHC nibble count: " << lNibbleCollector->GetNumGoodLHCNibbles() << endl;
	cout << "Bad LHC nibble count: " << lNibbleCollector->GetNumBadLHCNibbles() << endl;
	cout << "Good section count:" << lSectionCollectorShort->GetNumCompleteLumiSections() << endl;
	cout << "Bad section count: " << lSectionCollectorShort->GetNumIncompleteLumiSections() << endl;
	cout << "Lost packet count: " << lNibbleCollector->GetNumLostPackets() << endl;
	cout << "Total data volume: " << lNibbleCollector->GetTotalDataVolume() << endl;
	cout << "Average data rate (Mb/s): " << (double)lNibbleCollector->GetTotalDataVolume()*8.0/(1024.0*1024.0*(double)(tempTime-startTime)) << endl;
	cout << "Nibble last error: " << lNibbleCollector->GetLastError() << endl;
	cout << "Short section last error: " << lSectionCollectorShort->GetLastError() << endl;
	cout << "Long section last error: " << lSectionCollectorLong->GetLastError() << endl;
	cout << "Short section resync count: " << lSectionCollectorShort->GetResyncCount() << endl;
	cout << "Long section resync count: " << lSectionCollectorLong->GetResyncCount() << endl;
	interTime = tempTime;
      }
    }
        
  }catch(ICException & aExc){
	cout << "IC exception caught" << endl;
    cerr << aExc.what() << endl;
  }catch(std::exception & aExc){
	cout <<"std exception caught" << endl; 
    cerr << aExc.what()<<std::endl;
  }catch(...){
    cerr <<"Unknown exception caught."<<std::endl;
  }

  lNibbleCollector->Stop();
  delete lNibbleCollector;
  delete lSectionCollectorShort;
  delete lSectionCollectorLong;
  //delete lDIPDistributor;
  //delete lDebugCoutDistributor;
  //delete lTCPDistributor;
  //delete lROOTDistributor;
  //delete lOracleDistributor;
  //delete lGIFDistributor;
  //for ( u32 i = 0 ; i != NUM_HLXS ; i++ ) {
  //  delete lWedgeGIFDistributor[i];
 // }
    
  return 0;
}
