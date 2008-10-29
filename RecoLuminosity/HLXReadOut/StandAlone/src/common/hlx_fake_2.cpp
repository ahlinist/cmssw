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
#include "FakeHLX.hh"

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

    FakeHLX tester(36, // num hlxs
		   3564, // num bunches 
		   4096, // num orbits
		   4,
		   21308,
		   false);
		   //true); // random restarts

    while (gContinue) { Sleep(100); }

  }catch(ICException & aExc){
    cerr<<aExc.what()<<endl;
  }catch(std::exception & aExc){
    std::cerr<<aExc.what()<<std::endl;
  }catch(...){
    std::cerr<<"Unknown exception caught."<<std::endl;
  }

  return 0;
}
