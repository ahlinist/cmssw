#include <iostream>
#include "QuaeroUtils/Luminosity.hh"
#include "QuaeroUtils/QuaeroItemFile.hh"
#include "QuaeroUtils/QuaeroEvent.hh"
using namespace std;


void normalizeProperly_h1(string infilename, Luminosity inLumi, string outfilename, Luminosity outLumi_301, Luminosity outLumi_319)
{

  // muck up
  cout << "Normalizing properly..." << endl;
  cout << "  in:  " << infilename << "  " << inLumi.getLuminosity("pb^-1") << "pb^-1" << endl;
  cout << "  out at 301: " << outfilename << "  " << outLumi_301.getLuminosity("pb^-1") << "pb^-1" << endl;
  cout << "  out at 319: " << outfilename << "  " << outLumi_319.getLuminosity("pb^-1") << "pb^-1" << endl;
  cout << "  ..." << flush;

  QuaeroItemFile<QuaeroEvent> eventFile(infilename);
  ofstream fout(outfilename.c_str());
  QuaeroEvent event;
  string eventType;
  double generatedLuminosityAtEachCmEnergy = inLumi.getLuminosity("fb^-1")/2; // divide by two for 301, 319
  while(eventFile.nextEvent(event))
    {
      if(fabs(event.getRootS()-301)<1)
	event.reWeight(event.getWeight()*outLumi_301.getLuminosity("fb^-1")/generatedLuminosityAtEachCmEnergy);
      else if(fabs(event.getRootS()-319)<1)
	event.reWeight(event.getWeight()*outLumi_319.getLuminosity("fb^-1")/generatedLuminosityAtEachCmEnergy);
      else
	{
	  cout << "ERROR!  Event " << event << " does not have allowed center-of-mass energy in normalizeProperly routine." << endl;
	  exit(1);
	}
      fout << event << endl;
    }
  fout.close();
  cout << "done!" << endl;
  return;
}


int main(int argc, char* argv[])
{
  if((argc!=6))
    {
      cout << "normalizeProperly_h1 <inputFile> <input xsec/lumi> <outputFile> <outlumi_301> <outlumi_319>" << endl;
      exit(1);
    }

  string infilename = argv[1];
  string outfilename = argv[3];
  cout << "at 301 GeV:  " << argv[4] << endl;
  cout << "at 319 GeV:  " << argv[5] << endl;
  Luminosity inLumi(argv[2]), outLumi_301(argv[4]), outLumi_319(argv[5]);
  normalizeProperly_h1(infilename, inLumi, outfilename, outLumi_301, outLumi_319);
  return 0;
}



