/*********************************************************
This executable implements "refine", a process of final 
massaging to data and background events before their comparison.

Bruce Knuteson   Nov 2004
**********************************************************/


#include <vector>
#include <string>
#include <fstream>
#include "Validation/VstQuaeroUtils/interface/QuaeroItemFile.hh"
#include "Validation/VstQuaeroUtils/interface/QuaeroEvent.hh"
#include "Validation/VstQuaeroUtils/interface/Refine.hh"
using namespace std;


void refine(int level, double minWt, string collider, string experiment, vector<string> inputFilenames)
{
  string partitionRuleName = collider+"-Vista";
  Refine refine(collider, experiment, partitionRuleName, level);
  refine.setMinWt(fabs(minWt));
  double runningWeight = 0;
  for(size_t i=0; i<inputFilenames.size(); i++)
    {
      QuaeroItemFile<QuaeroEvent> ef(inputFilenames[i]);
      QuaeroEvent e;
      while(ef.nextEvent(e))
	{
	  if(refine.satisfiesCriteriaQ(e))
	    {
	      if(refine.passesMinWt(e,runningWeight))
		{
		  if(minWt>=0)
		    cout << e << endl;
		  else
		    {
		      runningWeight += e.getWeight();
		      e.reWeight(1);
		      e.reType("data");
		      while(runningWeight>1)
			{
			  cout << e << endl;
			  runningWeight -= 1;
			}
		    }
		}
	    }
	}
    }
  return;
}

int main(int argc, char* argv[])
{
  if(argc<5)
    {
      cout << "Usage:  refine [-level=<level>] <minWt> <collider> <experiment> <inputFilenames...>" << endl;
      exit(1);
    }

  assert(argc>=5);
  int level=10;
  int k=1;
  if(((string)argv[1]).substr(0,7)=="-level=")
    {
      level=atoi(((string)argv[1]).substr(7).c_str());
      k++;
    }
  double minWt=-1;
  if(((string)argv[k])=="pseudoData")
    k++;
  else
    minWt = atof(argv[k++]);
  string collider = argv[k++];
  string experiment = argv[k++];
  vector<string> inputFilenames;
  for(int i=k; i<argc; i++)
    inputFilenames.push_back(argv[i]);
  refine(level, minWt, collider, experiment, inputFilenames);
  return(0);
}
