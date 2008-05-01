#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include "Validation/VstQuaeroUtils/interface/PartitionRule.hh"
using namespace std;


void partition(string partitionRuleName, string fileToBePartitioned)
{
  if((fileToBePartitioned.length()>=2)&&
     (fileToBePartitioned.substr(0,1)=="-"))
    system(("rm "+fileToBePartitioned.substr(1)+"_*.txt 2> /dev/null").c_str());
  else     
    system(("rm "+fileToBePartitioned+"_*.txt 2> /dev/null").c_str());

  cout << "Partitioning ..." << flush;
  PartitionRule partitionRule(partitionRuleName);
  partitionRule.partition(fileToBePartitioned);
  cout << "done!" << endl;

  return;
}


int main(int argc, char* argv[])
{
  if(argc==1)
    {
      cout << "Usage:  partition <partitionRuleName> <fileToBePartitioned>" << endl;
      exit(0);      
    }
  assert(argc==3);
  string partitionRuleName = argv[1];
  string fileToBePartitioned = argv[2];
  partition(partitionRuleName, fileToBePartitioned);
  return 0;
}

