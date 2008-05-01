#include <iostream>
#include "QuaeroUtils/Luminosity.hh"
#include "QuaeroUtils/QuaeroItemFile.hh"
#include "QuaeroUtils/QuaeroEvent.hh"
using namespace std;


void normalizeProperly(string infilename, Luminosity inLumi, string outfilename, Luminosity outLumi)
{

  // muck up
  cout << "Normalizing properly..." << endl;
  cout << "  in:  " << infilename << "  " << inLumi.getLuminosity("pb^-1") << "pb^-1" << endl;
  cout << "  out: " << outfilename << "  " << outLumi.getLuminosity("pb^-1") << "pb^-1" << endl;
  cout << "  ..." << flush;

  QuaeroItemFile<QuaeroEvent> eventFile(infilename);
  ofstream fout(outfilename.c_str());
  QuaeroEvent event;
  string eventType;
  while(eventFile.nextEvent(event))
    {
      event.reWeight(event.getWeight()*outLumi.getLuminosity("fb^-1")/inLumi.getLuminosity("fb^-1"));
      fout << event << endl;
    }
  fout.close();
  cout << "done!" << endl;
  return;
}


void normalizeProperly(string infilename, double xsecpb, string outfilename, Luminosity outLumi)
{
  QuaeroItemFile<QuaeroEvent> eventFile(infilename);
  QuaeroEvent e;
  double totalWeight = 0.;
  while(eventFile.nextEvent(e))
    totalWeight += e.getWeight();
  Luminosity inLumi(totalWeight/xsecpb,"pb^-1");
  normalizeProperly(infilename, inLumi, outfilename, outLumi);
  return;
}


int main(int argc, char* argv[])
{
  if((argc!=6))
    {
      cout << "normalizeProperly <inputFile> <'xsec'/'lumi'> <input xsec/lumi> <outputFile> <outlumi>" << endl;
      exit(1);
    }

  string infilename = argv[1];
  string type = argv[2];
  string eitherXsecOrLumi = argv[3];
  string outfilename = argv[4];
  cout << argv[5] << endl;
  Luminosity outLumi = Luminosity(argv[5]);
  assert((type=="xsec")||(type=="lumi"));
  if(type=="xsec")
    {
      assert(eitherXsecOrLumi.substr(eitherXsecOrLumi.length()-2)=="pb");
      double xsec = atof(eitherXsecOrLumi.substr(0,eitherXsecOrLumi.length()-2).c_str());
      cout << "xsec = " << xsec << endl;
      normalizeProperly(infilename, xsec, outfilename, outLumi);
    }
  if(type=="lumi")
    normalizeProperly(infilename, Luminosity(eitherXsecOrLumi), outfilename, outLumi);
  return 0;
}



