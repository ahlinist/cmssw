/*****************************************
normalizeProperly_aleph
Purpose:  This file properly normalizes an input file produced with
          given luminosities at the nominal LEP2 energies, producing
          an output file corresponding to the actual luminosities
          recorded at the nominal LEP2 energies.
*****************************************/

#include <iostream>
#include "QuaeroUtils/Luminosity.hh"
#include "QuaeroUtils/QuaeroItemFile.hh"
#include "QuaeroUtils/QuaeroEvent.hh"
using namespace std;


void normalizeProperly_aleph(string infilename, vector<Luminosity> inLumi, string outfilename, vector<Luminosity> outLumi)
{
  QuaeroItemFile<QuaeroEvent> eventFile(infilename);
  ofstream fout(outfilename.c_str());
  QuaeroEvent event;
  string eventType;
  const int Necm = 8;
  int ecm[Necm] = { 183, 189, 192, 196, 200, 202, 205, 207 };
  while(eventFile.nextEvent(event))
    {
      bool ecmUnderstood = false;
      for(int i=0; i<8; i++)
	if(fabs(event.getRootS()-ecm[i])<1)
	  {
	    event.reWeight(event.getWeight()*outLumi[i].getLuminosity("pb^-1")/inLumi[i].getLuminosity("pb^-1"));
	    ecmUnderstood = true;
	  }
      if(!ecmUnderstood)
	{
	  cout << "ERROR!  Event " << event << " does not have allowed center-of-mass energy in normalizeProperly routine." << endl;
	  exit(1);
	}
      if(event.getWeight()>0)
	fout << event << endl;
    }
  fout.close();
  return;
}


int main(int argc, char* argv[])
{
  if((argc!=19))
    {
      cout << "normalizeProperly_aleph <inputFile> <inlumi_183,189,192,196,200,202,205,207> <outputFile> <outlumi_183,189,192,196,200,202,205,207>" << endl;
      exit(1);
    }

  string infilename = argv[1];
  string outfilename = argv[10];
  vector<Luminosity> inLumi, outLumi;
  int ecm[8] = { 183, 189, 192, 196, 200, 202, 205, 207 };
  cout << "      normalizeProperly_aleph" << endl;
  for(int i=0; i<8; i++)
    {
      inLumi.push_back(Luminosity(argv[2+i]));
      outLumi.push_back(Luminosity(argv[11+i]));
      cout << "           " << ecm[i] << " GeV:  " 
	   << inLumi.back().getLuminosity("pb^-1")  << " pb^-1  ->  "
	   << outLumi.back().getLuminosity("pb^-1") << " pb^-1 " 
	   << endl;
    }
  normalizeProperly_aleph(infilename, inLumi, outfilename, outLumi);
  return 0;
}



