/***************************************************************
Purpose:  hepevt2partonConverter converts a  
          hepevt ascii file to a Quaero parton file.
Usage:    hepevt2partonConverter <inputHepevtFile> <outputPartonFile>
Comments: This file is simply a wrapper for the fortran code
          in hepevt2parton.cc, for use by Andre Holzner at L3.
***************************************************************/

#include <iostream>
#include <string>
#include "VistaTools/TurboSim/interface/hepevt2parton.hh"
#include <unistd.h>
using namespace std;

int main(int argc, char* argv[])
{
  assert(argc==3);
  string hepevtFileName = argv[1];
  string partonFileName = argv[2];
  hepevt2parton(hepevtFileName, partonFileName);
  return(0);
}      
