/*****************************************
normalizeProperly_d0
Purpose:  This file properly normalizes an input file produced with
          given Tevatron Run I luminosity,
          producing an output file corresponding to the actual 
          luminosity recorded at Tevatron Run I.
*****************************************/

#include <iostream>
#include "QuaeroUtils/Luminosity.hh"
#include "QuaeroUtils/QuaeroItemFile.hh"
#include "QuaeroUtils/QuaeroEvent.hh"
using namespace std;


void normalizeProperly_d0(string infilename, Luminosity inLumi, string outfilename, Luminosity outLumi)
{
  cout << "      normalizeProperly_d0    " << endl;
  cout << "          ppbar at 1800: " << inLumi.getLuminosity("pb^-1") << " -> " << outLumi.getLuminosity("pb^-1") << "pb^-1" << endl;

  QuaeroItemFile<QuaeroEvent> eventFile(infilename);
  ofstream fout(outfilename.c_str());
  QuaeroEvent event;
  string eventType;
  double z = outLumi.getLuminosity("pb^-1")/inLumi.getLuminosity("pb^-1");
  while(eventFile.nextEvent(event))
    {
      if((event.getCollisionType()=="ppbar")&&
	 (fabs(event.getRootS()-1800)<1))
	{
	  event.reWeight(event.getWeight()*z);
	  if(event.getWeight()>0)
	    fout << event << endl;
	}      
      else
	{
	  cout << "ERROR!  Event " << event << " does not have allowed center-of-mass energy in normalizeProperly routine." << endl;
	  exit(1);
	}
     }
  fout.close();
  return;
}


int main(int argc, char* argv[])
{
  if((argc!=5))
    {
      cout << "normalizeProperly_d0 <inputFile> <inlumi> <outputFile> <outlumi>" << endl;
      exit(1);
    }
  string infilename = argv[1];
  string outfilename = argv[3];
  Luminosity inLumi(argv[2]), outLumi(argv[4]);
  normalizeProperly_d0(infilename, inLumi, outfilename, outLumi);
  return 0;
}



