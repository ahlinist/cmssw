
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "MathUtils/Math.hh"
#include "Quaero/quaero.hh"
#include "Quaero/quaeroIO.hh"
#include "Quaero/QuaeroFSObject.hh"
#include "Quaero/RelevantVariables.hh"
#include "Quaero/Discriminator.hh"
#include "Quaero/GraphicalOutput.hh"
using namespace std;

int main(int argc, char* argv[])
{
  assert(argc==3);
  /*
  assert((argc==1)||(argc==3));
  if(argc==1)
    {
      string sysShiftsToComputeFilename = getFilename("systematicShiftsToCompute");
      ifstream fsys(sysShiftsToComputeFilename.c_str());
      if(!fsys)
	return 0;
      if(fsys >> GlobalVariables::id)
	fsys >> GlobalVariables::iSystematicShift;
      else
	return 0;
      string tmpFilename = getFilename("tmp");
      ofstream ftmp(tmpFilename.c_str());
      if(GlobalVariables::iSystematicShift<RCPParameters::nSystematicShifts)
	ftmp << GlobalVariables::id << " " << GlobalVariables::iSystematicShift+1 << endl;
      int x1, x2;
      while(fsys >> x1)
	{
	  fsys >> x2;
	  ftmp << x1 << " " << x2 << endl;
	}
      fsys.close();
      ftmp.close();
      system(("mv "+tmpFilename+" "+sysShiftsToComputeFilename).c_str());
    }
  */
 if(argc==3)
    {
      GlobalVariables::id = argv[1];
      GlobalVariables::iSystematicShift = atoi(argv[2]);
    }
  RCPParameters::read(getFilename("rcpParameters"));
  //RCPParameters::debugLevel=0; // no output from job
  double hypothesisLikelihood=0., smLikelihood=0.;
  map<vector<string>,RelevantVariables> relevantVariableses;
  map<vector<string>,RelevantVariables> relevantVariables;
  ifstream fvars(getFilename("relevantVariables").c_str());
  vector<string> tag;
  int n=0;
  fvars >> n;
  for(int i=0; i<n; i++)
    {
      read<string>(tag,fvars);
      fvars >> relevantVariables;
      relevantVariableses[tag] = relevantVariables;
    }
  fvars.close();
  map<vector<string>,Discriminator> discriminators;
  Discriminator discriminator;
  ifstream fdiscriminators(getFilename("discriminators").c_str());
  fdiscriminators >> n;
  for(int i=0; i<n; i++)
    {
      read<string>(tag,fvars);
      fdiscriminators >> discriminator;
      discriminators[tag] = discriminator;
    }
  srand48(GlobalVariables::iSystematicShift); drand48();
  map<vector<string>,GraphicalOutput> graphicalOutput;
  computeLikelihoodForSystematicErrorShift(GlobalVariables::iSystematicShift, hypothesisLikelihood, smLikelihood, relevantVariables, discriminators, relevantVariableses);
  system(("echo '"+Math::ftoa(hypothesisLikelihood)+" "+Math::ftoa(smLikelihood)+"' >> "+getFilename("likelihoods")).c_str());
  return(0);
}


