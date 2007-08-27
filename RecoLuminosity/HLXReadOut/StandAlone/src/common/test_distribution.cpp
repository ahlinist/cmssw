#include <vector>
#include <string>
#include <exception>
#include <iostream>
#include <fstream>
#include <sstream>
#include <signal.h>

#include "ICException.hh"
#include "ICUtilityToolbox.hh"

// NibbleCollector class
#include "TestDistributor.hh"
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
  TestDistributor *lTestDistributor = 0;
  try {

cout << "NUM HLXS " << dec << NUM_HLXS << endl;
    lSectionCollector = new SectionCollector(3564, //3564, // Num bunches
					     3,    // Num nibbles per section
					     4096,  //280,  // Num orbits in lumi nibble
					     NUM_HLXS);   // Num HLXs
    lNibbleCollector = new NibbleCollector(NUM_HLXS);
    lTestDistributor = new TestDistributor;
    lNibbleCollector->AttachSectionCollector(lSectionCollector);
    lSectionCollector->AttachDistributor(lTestDistributor);

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
	//cout << "Good nibble count: " << lNibbleCollector.GetNumGoodNibbles() << endl;
	//cout << "Bad nibble count: " << lNibbleCollector.GetNumBadNibbles() << endl;
	cout << "Good section count:" << lSectionCollector->GetNumCompleteLumiSections() << endl;
	cout << "Bad section count: " << lSectionCollector->GetNumIncompleteLumiSections() << endl;
	cout << "Good section data count: " << lTestDistributor->GetNumGoodSections() << endl;
	cout << "Bad section data count: " << lTestDistributor->GetNumBadSections() << endl;
	cout << "Lost packet count: " << lNibbleCollector->GetNumLostPackets() << endl;
	cout << "Total data volume: " << lNibbleCollector->GetTotalDataVolume() << endl;
	cout << "Average data rate (Mb/s): " << (double)lNibbleCollector->GetTotalDataVolume()*8.0/(1024.0*1024.0*(double)(tempTime-startTime)) << endl;
	interTime = tempTime;
      }
      Sleep(1);
    }

  }catch(ICException & aExc){
    cerr<<aExc.what()<<endl;
  }catch(std::exception & aExc){
    std::cerr<<aExc.what()<<std::endl;
  }catch(...){
    std::cerr<<"Unknown exception caught."<<std::endl;
  }

  delete lNibbleCollector;
  delete lSectionCollector;
  delete lTestDistributor;
  return 0;
}
