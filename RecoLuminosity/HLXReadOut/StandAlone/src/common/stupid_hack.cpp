#include <vector>
#include <string>
#include <exception>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

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
using namespace ICCoreUtils;

int main(int argc, char ** argv) {
  signal(SIGINT,CtrlC);

  while ( gContinue ) {

    cout << "Sleeping 30s" << endl;
    Sleep(30000);

    // wget the file
    int get = system("wget http://hcaldaq07.cms:16102/urn:xdaq-application:lid=51 -O /tmp/stupid_hack_short.tmp");
    if ( get != 0 ) continue;
    
    // read the file
    ifstream infile("/tmp/stupid_hack_short.tmp");
    if ( !infile ) continue;

    // get a line
    std::string aLine;
    
    while ( !infile.eof() ) {
      std::getline(infile, aLine,'\n');
      int pos1 = aLine.find("Run Number<td bgcolor=#66CCCC ><b>");
      int pos2 = aLine.find("</b><td bgcolor=#66FFFF><td colspan=2 bgcolor=#66CCCC></tr>");
      if  ( pos1 == aLine.npos ) continue;
      if  ( pos2 == aLine.npos ) continue;
      
      //cout << aLine << endl;
      std::string value;
      
      //cout << pos1 << endl;
      //cout << pos2 << endl;

      std::istringstream(aLine.substr(pos1+34,pos2-pos1-34))>>std::dec>>value;
      
      cout << value << endl;
 
      continue;
    }

  }    

  return 0;
}
