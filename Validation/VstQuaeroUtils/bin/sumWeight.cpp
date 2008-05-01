/****************************************************************
Usage:    sumWeight <file>
Purpose:  Provide a summary of the event weights in a background file
****************************************************************/


#include <vector>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <unistd.h>
#include "Validation/VstQuaeroUtils/interface/QuaeroEvent.hh"
#include "Validation/VstQuaeroUtils/interface/QuaeroItemFile.hh"
#include "Validation/VstQuaeroUtils/interface/JargonReduction.hh"
using namespace std;

void sumFiles(vector<string> filenames, vector<string> & sources, vector<double> & weight, vector<int>& numberOfEvents, vector<double>& effectiveNumberOfEvents)
{
  //Trigger trigger("cdf");
  //FiducialArea fiducialArea("cdf");

  sources = vector<string>(0);
  weight = vector<double>(0);
  numberOfEvents = vector<int>(0);
  effectiveNumberOfEvents = vector<double>(0);
  vector<vector<double> > allWeights;
  string experiment = "cdf";
  if((getenv("experiment")!=NULL))
    experiment = getenv("experiment");

  for(int q=0; q<filenames.size(); q++)
    {
      string filename = filenames[q];
      if(access(filename.c_str(),F_OK)!=0)
	{
	  filename="/home/highpt/cvs/current/Vista_"+experiment+"/files/bkg/bkg_"+filename+".txt"; // shortcut
	  cout << filename << endl;
	}
      QuaeroItemFile<QuaeroEvent> ef(filename);
      QuaeroEvent e;
      int i;

  while(ef.nextEvent(e))
    {  
      //fiducialArea.mask(e);
      //if(trigger.doesThisEventPass(e))
	{
	  string eventType = e.getEventType();
	  int nUnderscores = 0;
	  for(int j=0; j<eventType.length(); j++)
	    if(eventType.substr(j,1)=="_")
	      {
		nUnderscores++;
		if(false) // bktemp (nUnderscores==2)
		  {
		    eventType = eventType.substr(0,j);
		    break;
		  }
	      }
	  i = find(sources.begin(),sources.end(),eventType)-sources.begin();
	  if(i==sources.size())
	    {
	      sources.push_back(eventType);
	      allWeights.push_back(vector<double>(0));
	    }
	  allWeights[i].push_back(e.getWeight());
	}
    }
    }
  for(int i=0; i<allWeights.size(); i++)
    {
      weight.push_back(Math::computeSum(allWeights[i]));
      effectiveNumberOfEvents.push_back((int)Math::effectiveNumberOfEvents(allWeights[i]));
      numberOfEvents.push_back(allWeights[i].size());
    }
  return;
}

void sumSingleFile(string filename, double& totalWeight, int& numberOfEvents, double& effectiveNumberOfEvents)
{
  QuaeroEvent e;
  QuaeroItemFile<QuaeroEvent> ef(filename);
  vector<double> wt;
  while(ef.nextEvent(e))
    wt.push_back(e.getWeight());
  totalWeight = Math::computeSum(wt);
  effectiveNumberOfEvents = Math::effectiveNumberOfEvents(wt);
  numberOfEvents = wt.size();
  return;
}


void sumArbitraryFiles(int argc, char* argv[])
{
  assert(argc>=2);
  vector<string> filenames;
  JargonReduction jargonReduction;
  string jargonReductionFileName = "";
  for(int i=1; i<argc; i++)
    {
      if(((string)argv[i]).substr(0,21)=="-jargonReductionFile=")
	{
	  jargonReductionFileName = ((string)argv[i]).substr(21);
	  jargonReduction.load(jargonReductionFileName);
	}
      else
	filenames.push_back(argv[i]);
    }

  vector<string> sources;
  vector<double> weight;
  vector<double> effectiveNumberOfEvents;
  vector<int> numberOfEvents;
  sumFiles(filenames, sources, weight, numberOfEvents, effectiveNumberOfEvents);
  vector<double> effectiveWeight, effectiveWeight1;
  for(int j=0; j<sources.size(); j++)
    effectiveWeight.push_back(weight[j]/effectiveNumberOfEvents[j]);
  effectiveWeight1 = effectiveWeight;
  Math::parallelBubbleSort(effectiveWeight1,sources);
  effectiveWeight1 = effectiveWeight;
  Math::parallelBubbleSort(effectiveWeight1,effectiveNumberOfEvents);
  Math::parallelBubbleSort(effectiveWeight,weight);
  reverse(sources.begin(), sources.end());
  reverse(weight.begin(), weight.end());
  reverse(effectiveWeight.begin(), effectiveWeight.end());
  reverse(effectiveNumberOfEvents.begin(), effectiveNumberOfEvents.end());
  
  for(int j=0; j<sources.size(); j++)
    {
      string english = jargonReduction.interpret(sources[j]);
      string x1 = Math::ftoa(Math::sigFigRound(effectiveWeight[j],2));
      string x2 = Math::ftoa((int)(effectiveNumberOfEvents[j]));
      string x3 = Math::ftoa(Math::toleranceRound(weight[j],0.01));
      string a1 = Math::spacePad(7-max(1,1+(int)log10(effectiveWeight[j])));
      string a2 = Math::spacePad(7-max(1,1+(int)log10(effectiveNumberOfEvents[j])));
      string a3 = Math::spacePad(7-max(1,1+(int)log10(weight[j])));
      cout << "  " << sources[j] << Math::spacePad(20-sources[j].length())+" ";
      if(jargonReductionFileName!="")
	cout << english << Math::spacePad(30-english.length())+" ";
      cout << a1   << x1         << Math::spacePad(15-x1.length()-a1.length())+" " 
	   << a2   << x2         << Math::spacePad(12-x2.length()-a2.length())+" "
	   << a3   << x3         << endl;
    }
  string x4 = Math::ftoa(Math::toleranceRound(Math::computeSum(weight),0.01));
  string a4 = Math::spacePad(12-max(1,1+(int)log10(Math::computeSum(weight))));
  cout << "  Total:   " << Math::spacePad(36+30*(jargonReductionFileName!="")) << a4 << x4 << endl << endl;

  return;
}

void compareDataAndBkg(string baseDir)
{
  cout << endl;
  cout << "Final state             Data       Bkg" << endl; 
  vector<string> files = Math::getFilesInDirectory(baseDir+"/data/","data_*.txt");
  vector<string> sources;
  vector<double> subWeights;
  for(int i=0; i<files.size(); i++)
    {
      string fs = files[i].substr(0,files[i].length()-4); // subtract off .txt
      fs = fs.substr(5); // subtract off data_
      cout << " " << fs;
      for(int i=fs.length(); i<20; i++)
	cout << " ";
      double dataWeight;
      int dataNumberOfEvents;
      double effectiveNumberOfEvents;
      sumSingleFile(baseDir+"/data/data_"+fs+".txt", dataWeight, dataNumberOfEvents, effectiveNumberOfEvents);
      for(int i=(int)log10(dataWeight+0.001)+1; i<8; i++)
	cout << " ";
      cout << dataWeight;
      vector<double> n;
      vector<int> N;
      sumFiles(vector<string>(1,baseDir+"/bkg/bkg_"+fs+".txt"),sources,subWeights,N,n);
      double bkgWeight = Math::computeSum(subWeights);
      for(int i=(int)log10(bkgWeight+0.001)+1; i<10; i++)
	cout << " ";
      cout << ((int)(bkgWeight*10))/10. << "    ";
      if(((int)(bkgWeight*10))/10.==(int)bkgWeight)
	cout << "  ";
      cout << "( ";
      for(int i=0; i<sources.size(); i++)
	if(((int)(10*subWeights[i]))/10.>0)
	  cout << sources[i] << " = " << ((int)(10*subWeights[i]))/10. << " , ";
      cout << ")" << endl;
    }
  return;
}

int main(int argc, char* argv[])
{
  if((argc==3)&&(((string)argv[1])=="-bkgListing"))
    {
      double totalWeight, effectiveNumberOfEvents;
      int numberOfEvents;
      sumSingleFile(argv[2], totalWeight, numberOfEvents, effectiveNumberOfEvents);
      double wtSqd = 0;
      if(effectiveNumberOfEvents>0)
	wtSqd = pow(totalWeight,2.)/effectiveNumberOfEvents;
      cout << totalWeight << " " << numberOfEvents << " " << wtSqd << endl;
      return(0);
    }
  sumArbitraryFiles(argc, argv);
  //assert(argc==2); compareDataAndBkg(argv[1]);
  return(0);
}
