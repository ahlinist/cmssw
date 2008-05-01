/**************************************************************
  Usage:  partition_withIdSystematics <partitionRuleName> <fileToBePartitioned>
  Purpose:  To partition <fileToBePartitioned> according to the
            partitioning rule <partitionRuleName>.  Here "partition"
            is synonymous with "divide into exclusive final states",
            where the exact nature of "exclusive final states" is
            determined by <partitionRuleName>, as implemented in
            QuaeroUtils/src/PartitionRule.cc.
***************************************************************/

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include "MathUtils/Math.hh"
#include "QuaeroUtils/QuaeroItemFile.hh"
#include "QuaeroUtils/FinalState.hh"
#include "QuaeroUtils/PartitionRule.hh"
#include "Quaero/QuaeroSysEvent.hh"
#include "Quaero/RCPParameters.hh"
using namespace std;

void combine(string filename)
{
  assert(getenv("RELEASE_DIR")!=0);
  vector<string> files = Math::getFilesInDirectory(".",filename+"_*.txt_*");
  for(int i=0; i<files.size(); i++)
    system(("cat "+files[i].substr(0,files[i].length()-2)+"_* | sort --unique --temporary-directory="+getenv("RELEASE_DIR")+"/tmp/ > "+files[i].substr(0,files[i].length()-2)).c_str());
  for(int i=0; i<files.size(); i++)
    system(("rm "+files[i]).c_str());
  return;
}

void partition(string filename, PartitionRule partitionRule, int ipass)
{

  /*  We make two passes in partition_withIdSystematics, 
      once with systematic shifts all very low (ipass==1), 
      and then again with systematic shifts all very high (ipass==2).  */

  assert((ipass==1)||(ipass==2));
  system(("rm "+filename+"_*.txt 2> /dev/null").c_str());

  vector<double> systematicShifts = vector<double>(GlobalVariables::systematicShift.getNumberOfSystematicErrorSources(),
						   (ipass==1 ? 0.001 : 0.999) );
  GlobalVariables::systematicShift.reShift(systematicShifts);


  QuaeroItemFile<QuaeroSysEvent> fin(filename);
  ifstream fin1(filename.c_str());
  QuaeroSysEvent event;
  QuaeroEvent event1;
  vector<string> finalStates;
  std::map<FinalState,std::strstream*> partitioning;
  vector<ofstream*> fout;
  string eventString;

  cout << "Partitioning with systematics (" << ipass << "):   " << flush;
  int count = 0;
  while(fin.nextEvent(event))
    {
      count++;
      if(count%10000==0)
	cout << count/1000 << "k " << flush;
      getline(fin1,eventString);
      assert(eventString.find(event.getEventType())!=string::npos);
      assert(eventString.find(event.getRunNumber())!=string::npos);
      assert(eventString.find(";")!=string::npos);
      /*
      vector<QuaeroSysObject> o = event.getObjects();
      vector<QuaeroRecoObject> o1;
      for(int i=0; i<o.size(); i++)
	o1.push_back(o[i]);
      event1 = QuaeroEvent(event.getEventType(), event.getRunNumber(), event.getWeight(), o1, event.getRootS(), event.getZVtx());
      */
      event1 = event.returnQuaeroEventWithoutSystematics();
      FinalState fs = partitionRule.getFinalState(event1);
      if(partitioning.find(fs)==partitioning.end())
	partitioning[fs] = new strstream;
      (*partitioning[fs]) << eventString << endl;
    }

  cout << "  Writing " << flush;
  for(map<FinalState,strstream*>::iterator i=partitioning.begin(); i!=partitioning.end(); i++)
    {
      if(i->first.getTextString()!="")
	{
	  string foutFilename = filename+"_"+i->first.getTextString()+".txt_"+(ipass==1 ? "1" : "2");
	  if(i->first.getTextString()=="")
	    foutFilename = filename+".null";
	  ofstream fout(foutFilename.c_str());	  
	  string blah;
	  while(getline(*(i->second),blah))
	    fout << (string)blah << endl;
	  delete i->second;
	  i->second = NULL;
	}
    }

  cout << endl;
  return;
}


int main(int argc, char* argv[])
{
  if(argc==1)
    {
      cout << "Usage:  partition_withIdSystematics <partitionRuleName> <fileToBePartitioned>" << endl;
      exit(0);      
    }
  assert(argc==3);
  string partitionRuleName = argv[1];
  string fileToBePartitioned = argv[2];
  system(("rm "+fileToBePartitioned+"_*.txt 2> /dev/null").c_str());

  PartitionRule partitionRule(partitionRuleName);

  partition(fileToBePartitioned, partitionRule, 1);
  partition(fileToBePartitioned, partitionRule, 2);
  combine(fileToBePartitioned);

  cout << "done!" << endl;

  return 0;
}

