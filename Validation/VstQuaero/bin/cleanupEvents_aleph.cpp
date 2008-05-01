// Compile with:  cd code; g++ -O2 -o ../lepcode/aleph/cleanupEvents_aleph cleanupEvents_aleph.cpp -L. -lquaero -lstdhep -lFmcfio -lpacklib -lf2c -lg2c -lm; cd ..;

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <cassert>
#include "Quaero/QuaeroFSObject.hh"
#include "Quaero/triggers_lep.hh"
using namespace std;

string localDir = "/afs/cern.ch/aleph/project/quaero/www/quaero_development/lepcode/aleph/maggi/../vol3/quaero_prod/";
string strangeEventFilename = localDir+"strangeEvents.txt";

void cleanupEvents(string infilename, string outfilename)
{
  ifstream fin(infilename.c_str());
  ofstream fout(outfilename.c_str());
  QuaeroEvent event;
  int count = 0;
  while(fin >> event)
    {
      count++;
      if(trigger_aleph(event))
	{
	  fout << event << endl;
	  if(fabs(event.getThisObject("uncl").e)>100)
	    ;//system(("echo '"+infilename+" "+event.print()+"' >> "+strangeEventFilename).c_str());
	}
    }
  fin.close(); 
  fout.close();
  return;
}

void cleanup1(string filename)
{
  cout << "Working on " << filename << endl;
  string infilename = localDir+filename+".bruceout";
  string outfilename = localDir+filename+".txt";
  //string infilename = localDir+"maggiOutput/"+filename+"_1998.bruceout";
  //string outfilename = localDir+"bruceOutput/"+filename+".txt";
  cout << "  unzipping file" << endl;
  system(("gunzip "+infilename+".gz").c_str());
  cout << "  cleaning events" << endl;

  cleanupEvents(infilename,outfilename);

  cout << "  zipping file" << endl;
  //system(("gzip "+infilename).c_str());

  return;
}

void cleanup2(string filename)
{
  string tmpfile = "tmp/tmp.txt";
  cleanupEvents(filename,tmpfile);
  system(("mv "+tmpfile+" "+filename).c_str());
  return;
}

int main(int argc, char* argv[])
{
  system("date");
  for(int i=1; i<argc; i++)
    {
      cleanup1(argv[i]);
    }
  system("date");
  return(0);
}


