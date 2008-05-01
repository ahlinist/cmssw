// Bruce Knuteson  October 20, 2000

#include "Validation/VstMathUtils/interface/Math.hh"
#include "Validation/VstQuaero/interface/RCPParameters.hh"
#include "Validation/VstQuaeroUtils/interface/FinalState.hh"
#include <cmath>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

namespace RCPParameters
{
  void EpsilonWt::store(string colliderRun, string experiment, double epsilonWt)
  {
    string colliderRunExperiment = colliderRun+"-"+experiment;
    int k = find(colliderRunExperiments.begin(), colliderRunExperiments.end(), colliderRunExperiment) - colliderRunExperiments.begin();
    if(k==colliderRunExperiments.size())
      {
	colliderRunExperiments.push_back(colliderRunExperiment);
	epsilonWeights.push_back(epsilonWt);
      }
    else
      {
	epsilonWeights[k] = epsilonWt;
      }
  }
  double EpsilonWt::get(string colliderRun, string experiment)
  {
    string colliderRunExperiment = colliderRun+"-"+experiment;
    int k = find(colliderRunExperiments.begin(), colliderRunExperiments.end(), colliderRunExperiment) - colliderRunExperiments.begin();
    if(k<colliderRunExperiments.size())
      return(epsilonWeights[k]);
    else
      assert(false);
  }
  double EpsilonWt::get()
  {
    return(*(max_element(epsilonWeights.begin(), epsilonWeights.end())));
  }

  EpsilonWt epsilonWt;
  double weightQuantumForDesiredSmoothnessOfLogL = 0.001;
  int debugLevel = 0;
  HintSpec hintSpec;
  SignalFileStorage signalFileStorage;
  string discriminatingAlgorithm = "FewKDE";
  bool useSlowMethodForHandlingErrorDueToNumberOfMonteCarloEvents = false;
  vector<string> requestorInfo = vector<string>(3);
  string modelDescription = "";
  map<string,vector<string> > colliderExperiments;
  map<string, int > privilegeLevel; 
  map<string,vector<string> > leaveOut;
  map<string,string> signalFormat;
  string batchQueue = "1nh";

  double collectedLuminosity(string cr)
  {
    double collectedLuminosity = 0.; // units are pb^-1
    if(cr=="lep1")
      collectedLuminosity = 5000;
    if(cr=="lep2")
      collectedLuminosity = 100; // at each cm point
    if(cr=="tev1")
      collectedLuminosity = 100;
    if(cr=="tev2")
      collectedLuminosity = 500;
    if(cr=="hera")
      collectedLuminosity = 100;
    if(cr=="lhc")
      collectedLuminosity = 500;
      
    if(collectedLuminosity==0)
      assert(false);

    return(collectedLuminosity);
  }

  vector<double> desiredGeneratedLuminosity(string cr)
  {
    vector<double> ans;
    double x = collectedLuminosity(cr)/weightQuantumForDesiredSmoothnessOfLogL;
    if(cr=="hera")
      ans = vector<double>(3,x);
    else if(cr=="lep2")
      ans = vector<double>(8,x);
    else if(cr=="tev1")
      ans = vector<double>(1,x);
    else if(cr=="tev2")
      ans = vector<double>(1,x);
    else if(cr=="lhc")
      ans = vector<double>(1,x);
    else
      {
	cout << "ERROR:  cr = " << cr << endl;
	assert(false);
      }
    return(ans);
  }
  map<string,vector<double> > actualGeneratedLuminosity;
  map<string,vector<double> > signalXsec;


  string credit(string colliderRun, string experiment, string finalState)
  {
    string ans = "";
    if(colliderRun=="lep2")
      {
	if(experiment=="aleph")
	  {
	    ans = "";
	  }
	if(experiment=="l3")
	  {
	    ans = "";
	  }
      }
    if(colliderRun=="tev2")
      {
	if(experiment=="cdf")
	  {
	    ans = "";
	  }
	if(experiment=="d0")
	  {
	    if((FinalState(finalState).getNumberOfThisObject("e")>1)&&
	       (FinalState(finalState).getNumberOfThisObject("mu")>1))
	      {
		ans = "";
	      }
	  }
      }    
    return(ans);
  }
  
  int targetTime = 1000; // units are seconds
  int nSignalEvents = 1000;
  int nTurboSimTableLines = 100000;
  int nBkgEvents = 100000;
  int nFewKdeTrials = 1;
  int nSystematicShifts = 5;
}


namespace GlobalVariables
{
  string id = "0";
  string localDir = ( ((getenv("QUAERO_CC_DIR")==NULL)||(getenv("QUAERO_EDITION")==NULL)) ? "./" : (string)getenv("QUAERO_CC_DIR")+"/"+getenv("QUAERO_EDITION")+"/" );
  ChatterTab chatterTab;
  SystematicShift systematicShift;
  int iSystematicShift=0;
  std::map<std::string,GraphicalOutput> graphicalOutput;
  string colliderType = "ppbar"; //bktemp
  // ifdef QUAERO_CC_PLATFORM_clued0 string silencerString = " 2> /dev/null "; else
  string silencerString = " >& /dev/null ";
}

namespace LEPParameters
{
  vector<double> ecm()
  {
    vector<double> ans;
    ans.push_back(183);  
    ans.push_back(189);  
    ans.push_back(192);  
    ans.push_back(196);  
    ans.push_back(200);  
    ans.push_back(202);  
    ans.push_back(205);  
    ans.push_back(207);  
    ans.push_back(208);  
    return(ans);
  }
  vector<double> nominalLuminosity()
  {
    vector<double> ans;
    ans.push_back(50);
    ans.push_back(170);
    ans.push_back(30);
    ans.push_back(80);
    ans.push_back(80);
    ans.push_back(40);
    ans.push_back(60);
    ans.push_back(30);
    ans.push_back(5);
    return(ans);
  }

}


namespace TevatronParameters
{
}
