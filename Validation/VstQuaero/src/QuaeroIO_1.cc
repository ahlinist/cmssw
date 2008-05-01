/***********************************
Implementation of QuaeroIO namespace
Bruce Knuteson 2003
***********************************/


#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <numeric>
#include "Validation/VstMathUtils/interface/Math.hh"
#include "Validation/VstQuaero/interface/QuaeroIO.hh"
#include "Validation/VstQuaeroUtils/interface/generate.hh"
#include "Validation/VstQuaero/interface/QuaeroSysObject.hh"
#include "Validation/VstQuaero/interface/RCPParameters.hh"
using namespace std;


/*****  Opening: Parsing routines  *****/


void QuaeroIO::parseOptions(int argc, char* argv[])
{
  cout << endl;

  if ((argc<2))
    {
      cout << "Usage: quaero <id> [options]" << endl;
      return;
    }
  GlobalVariables::id = argv[1];

  cout << "Validation/VstQuaero Request # "+GlobalVariables::id << endl;
  system("date");
  system("hostname");  // hostname -f
  cout << "Options: " << endl;

  for(int i=0; i<argc; i++)
    {
      bool optionUnderstood = false;
      if(i<2)
	optionUnderstood = true;
       if( ((string)argv[i]).substr(0,11) == "-debugLevel" )
	{
	  RCPParameters::debugLevel=atoi(&argv[i][11]);
	  cout << "  debugLevel = " << RCPParameters::debugLevel << endl;
	  optionUnderstood = true;
	}
      if(!optionUnderstood)
	{
	  cout << "Error: Option '" << argv[i] << "' not understood.  Aborting." << endl;
	  exit(1);
	}	    
    }
  if(RCPParameters::debugLevel>=2)
    {
      RCPParameters::batchQueue = RCPParameters::batchQueue + " -N";
    }

  makeTmpDirectory();
  return;

}

void QuaeroIO::parseRequestFile()
{
  string requestFilename = getFilename("request");
  ifstream frequest(requestFilename.c_str());
  assert(frequest);
  string blah, cr;
  getline(frequest,blah); // To:, Subject: headers + 1 blank line
  getline(frequest,blah); //assert(blah=="Subject: Quaero Request #"+id);
  getline(frequest,blah); assert(blah=="");
  getline(frequest,blah); assert(blah=="Requestor");
  string RequestorName="", RequestorInstitution="", RequestorEmail;
  /*
  getline(frequest,RequestorName);
  RequestorName = RequestorName.substr(2);
  getline(frequest,RequestorInstitution);
  */
  getline(frequest,RequestorEmail);
  RCPParameters::requestorInfo[0] = RequestorName;
  RCPParameters::requestorInfo[1] = RequestorInstitution;
  RCPParameters::requestorInfo[2] = RequestorEmail;
  getline(frequest,blah); assert(blah=="");
  frequest >> blah; assert(blah=="ColliderRuns");
  int ncr = 0;
  frequest >> blah;
  while(blah!=";")
    {
      ncr++;
      RCPParameters::colliderExperiments[blah] = vector<string>(0);
      RCPParameters::leaveOut[blah] = vector<string>(0);
      RCPParameters::signalFormat[blah] = "";
      frequest >> blah;
    }
  frequest >> blah; assert(blah=="ColliderExperiments");
  for(int i=0; i<ncr; i++)
    {
      frequest >> cr; 
      frequest >> blah;
      while(blah!=";")
	{
	  if(blah=="+")
	    RCPParameters::privilegeLevel[cr+" "+RCPParameters::colliderExperiments[cr].back()]++;
	  else
	    {
	      RCPParameters::colliderExperiments[cr].push_back(blah);
	      RCPParameters::privilegeLevel[cr+" "+RCPParameters::colliderExperiments[cr].back()]=0;
	    }
	  frequest >> blah;
	}
    }
  frequest >> blah; 
  if(blah=="BackgroundsLeftOut")
    {
      cout << "BackgroundsLeftOut:" << endl;
      for(int i=0; i<ncr; i++)
	{
	  frequest >> cr;
	  cout << "  " << cr << "  ";
	  frequest >> blah;
	  while(blah!=";")
	    {
	      cout << blah << " ";
	      RCPParameters::leaveOut[cr].push_back(blah);
	      frequest >> blah;
	    }
	  cout << ";" << endl;
	}
      frequest >> blah; 
    }
  if(blah=="HINT")
    {
      RCPParameters::hintSpec.active = true;
      frequest >> RCPParameters::hintSpec.collider;
      frequest >> RCPParameters::hintSpec.experiment;
      frequest >> RCPParameters::hintSpec.finalState;
      frequest >> RCPParameters::hintSpec.sumPtCut;
      frequest >> blah;
    }
  RCPParameters::signalFileStorage.id="0";
  RCPParameters::signalFileStorage.scaleFactor=0;
  if(blah=="signalFileStorage")
    {
      frequest >> RCPParameters::signalFileStorage.id;
      if(RCPParameters::signalFileStorage.id=="store")
	{
	  RCPParameters::signalFileStorage.id = GlobalVariables::id;
	  RCPParameters::signalFileStorage.scaleFactor = 1;
	}
      else
	{
	  frequest >> RCPParameters::signalFileStorage.scaleFactor;
	}
      frequest >> blah;
    }

  if(blah!="SignalFormat")
    cout << "blah=" << blah;
  assert(blah=="SignalFormat");
  string signalFormat;
  frequest >> signalFormat;
  for(map<string,vector<string> >::iterator i = RCPParameters::colliderExperiments.begin();
      i != RCPParameters::colliderExperiments.end(); i++)
    RCPParameters::signalFormat[i->first] = signalFormat;

  frequest >> blah;
  if(blah=="TargetTime=")
    {
      frequest >> RCPParameters::targetTime;
      frequest >> blah; 
    }

  assert(blah=="ModelDescription");
  getline(frequest,blah); // rest of the line
  getline(frequest,RCPParameters::modelDescription);
  frequest.close();
  system(("cp "+getFilename("request")+" "+getFilename("answer")).c_str());
  system(("mv "+getFilename("request")+" "+getFilename(".request")).c_str());
  return;
}



