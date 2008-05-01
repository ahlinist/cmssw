/*****************************************
normalizeProperly_h1
Purpose:  This file properly normalizes an input file produced with
          given luminosities at each HERA collider configuration, 
          producing an output file corresponding to the actual 
          luminosities recorded at each HERA collider configuration.
*****************************************/

#include <iostream>
#include "QuaeroUtils/Luminosity.hh"
#include "QuaeroUtils/QuaeroItemFile.hh"
#include "QuaeroUtils/QuaeroEvent.hh"
using namespace std;


void normalizeProperly_h1(string infilename, Luminosity inLumi301plus, Luminosity inLumi319plus, Luminosity inLumi319minus,
			  string outfilename, Luminosity outLumi301plus, Luminosity outLumi319plus, Luminosity outLumi319minus)
{

  //cout << "  file names:  " << infilename << " -> " << outfilename << endl;
  cout << "      normalizeProperly_h1    " << endl;
  cout << "          e+p at 301: " << inLumi301plus.getLuminosity("pb^-1") << " -> " << outLumi301plus.getLuminosity("pb^-1") << "pb^-1" << endl;
  cout << "          e+p at 319: " << inLumi319plus.getLuminosity("pb^-1") << " -> " << outLumi319plus.getLuminosity("pb^-1") << "pb^-1" << endl;
  cout << "          e-p at 319: " << inLumi319minus.getLuminosity("pb^-1") << " -> " << outLumi319minus.getLuminosity("pb^-1") << "pb^-1" << endl;

  QuaeroItemFile<QuaeroEvent> eventFile(infilename);
  ofstream fout(outfilename.c_str());
  QuaeroEvent event;
  string eventType;
  while(eventFile.nextEvent(event))
    {
      if((event.getCollisionType()=="e+p")&&
	 (fabs(event.getRootS()-301)<1))
	{
	  if(inLumi301plus.getLuminosity("pb^-1")>0)
	    event.reWeight(event.getWeight()*outLumi301plus.getLuminosity("pb^-1")/inLumi301plus.getLuminosity("pb^-1"));
	  else
	    {
	      event.reWeight(0);
	      cout << "ERROR: event at 301 GeV but no luminosity" << endl;
	    }
	}      
      else if((event.getCollisionType()=="e+p")&&
	      (fabs(event.getRootS()-319)<1))
	{
	  if(inLumi319plus.getLuminosity("pb^-1")>0)
	    event.reWeight(event.getWeight()*outLumi319plus.getLuminosity("pb^-1")/inLumi319plus.getLuminosity("pb^-1"));
	  else
	    {
	      event.reWeight(0);
	      cout << "ERROR: event at 319 GeV (e+p) but no luminosity" << endl;
	    }
	}
      else if((event.getCollisionType()=="e-p")&&
	      (fabs(event.getRootS()-319)<1))
	{
	  if(inLumi319minus.getLuminosity("pb^-1")>0)
	    event.reWeight(event.getWeight()*outLumi319minus.getLuminosity("pb^-1")/inLumi319minus.getLuminosity("pb^-1"));
	  else
	    {
	      event.reWeight(0);
	      cout << "ERROR: event at 319 GeV (e-p) but no luminosity" << endl;
	    }
	}
      else
	{
	  cout << "ERROR!  Event " << event << " does not have allowed center-of-mass energy in normalizeProperly routine." << endl;
	  exit(1);
	}
      fout << event << endl;
    }
  fout.close();
  return;
}


int main(int argc, char* argv[])
{
  if((argc!=9))
    {
      cout << "normalizeProperly_h1 <inputFile> <inLumi_301eplus> <inLumi_319eplus> <inLumi_319eminus> <outputFile> <outlumi_301eplus> <outlumi_319eplus> <outLumi_319eminus>" << endl;
      exit(1);
    }

  string infilename = argv[1];
  string outfilename = argv[5];
  Luminosity inLumi301plus(argv[2]), inLumi319plus(argv[3]), inLumi319minus(argv[4]);
  Luminosity outLumi301plus(argv[6]), outLumi319plus(argv[7]), outLumi319minus(argv[8]);
  normalizeProperly_h1(infilename, inLumi301plus, inLumi319plus, inLumi319minus, 
		       outfilename, outLumi301plus, outLumi319plus, outLumi319minus);
  return 0;
}



