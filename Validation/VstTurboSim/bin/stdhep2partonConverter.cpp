/***************************************************************
Purpose:  stdhep2partonConverter converts a stdhep 
          (or a hepevt ascii file) to a Quaero parton file.
Usage:    stdhep2partonConverter <inputStdhepFile> <outputPartonFile>
Comments: This file is simply a wrapper for the fortran code
          in stdhep2parton.f, for use by Andre Holzner at L3.
***************************************************************/

#include <string>
#include "Validation/VstTurboSim/interface/stdhep2partonWrap.hh"
#include <unistd.h>
#include <iostream>
#include <cassert>
#include <cstdlib>
using namespace std;

int main(int argc, char* argv[])
{
  if(argc!=3)
    {
      std::cout << "Usage:  stdhep2partonConverter <inputStdhepFile> <outputTextFile>" << std::endl;
      exit(0);
    }
  assert(argc==3);
  std::string stdhepFileName = argv[1];
  std::string txtFileName = argv[2];
  stdhep2partonWrap(stdhepFileName, txtFileName);
  return(0);
}      
