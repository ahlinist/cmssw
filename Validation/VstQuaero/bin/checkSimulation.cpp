// Compile with:  cd code; gmake bin; g++ -O2 -o $QUAERO/quaero_development/utilities/checkSimulation checkSimulation.cpp -L. -lquaero -lstdhep -lFmcfio -lpacklib -lf2c -lg2c -lm; cd ..;

#include <vector>
#include <cassert>
#include <cmath>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "MathUtils/Math.hh"
#include "QuaeroUtils/generate.hh"
#include "Quaero/RCPParameters.hh"
#include "Quaero/QuaeroIO.hh"
#include "Quaero/simulate.hh"
using namespace std;

void setup(string exp)
{
  GlobalVariables::id="1";
  GlobalVariables::iSystematicShift=-1;
  RCPParameters::colliderExperiments["lep2"].push_back(exp);
  QuaeroIO::removeTmpDirectory();
  QuaeroIO::makeTmpDirectory();
  QuaeroIO::makeTmpDirectory(GlobalVariables::iSystematicShift);
  return;
}

void checkSimulation(string experiment)
{
  system("date");
  string colliderRun = "lep2";
  setup(experiment);
  string examplesDir = GlobalVariables::localDir+"doc/examples/";
  string signalfilesDir = examplesDir+"signalfiles/";
  string tmp1 = signalfilesDir+"tmp1.txt";
  system(("cd "+signalfilesDir+"; ls *-* > "+tmp1).c_str());
  ifstream ftmp1(tmp1.c_str());
  string sig;
  while(ftmp1 >> sig)
    {
      string hepgFilename = signalfilesDir+sig;
      string simulatedFilename = examplesDir+"simulatedfiles/"+experiment+"/"+sig;
      string ecm = sig.substr(sig.length()-3,3); // energy encoded as the last three characters of the filename
      system((QuaeroIO::getFilename("simulate0_executable",colliderRun,experiment)+" "+ecm+" "+hepgFilename+" "+simulatedFilename+" "+Math::ftoa(RCPParameters::generatedLuminosity(colliderRun))+" "+QuaeroIO::getFilename("simulate_directory",colliderRun,experiment)).c_str());
    }
  return;
}


int main(int argc, char* argv[])
{
  assert(argc==2);
  string exp = argv[1];
  checkSimulation(exp);
  return(0);
}

