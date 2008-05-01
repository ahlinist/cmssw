/********************************
FinalState implements Sleuth and Quaero exclusive final states
Bruce Knuteson 2003
********************************/

#include <string>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <cmath>
#include <fstream>
#include <algorithm>
#include "Validation/VstQuaeroUtils/interface/FinalState.hh"
#include "Validation/VstMathUtils/interface/Math.hh"
using namespace std;
 
/***  Constructors  ***/

FinalState::FinalState(std::vector<std::string> _objects, std::vector<double> _sumPtRange)
{
  if(_sumPtRange.empty())
    sumPtRange = vector<double>(0);
  else
    {
      assert(_sumPtRange.size()==2);
      assert((_sumPtRange[0]>=0)&&(_sumPtRange[1]>=0)&&
	     (_sumPtRange[0]<_sumPtRange[1]));
      sumPtRange = _sumPtRange;
    }
  
  if(_objects.empty())
    return;
  sort(_objects.begin(), _objects.end());  // put objects in standard order
  objects = vector<string>(0);
  numberOfObjects = vector<int>(0);

  // Take input like { "e+", "e+", "b", "b", "j" }
  // and store it as objects = { "e+", "b", "j" }, numberOfObjects = { 2, 2, 1 }
  for(size_t i=0; i<_objects.size(); i++)
    {
      if((i==0)||(_objects[i]!=_objects[i-1]))
	{
	  numberOfObjects.push_back(1);
	  objects.push_back(_objects[i]);
	}
      else
	numberOfObjects.back()++;
    }
  return;
}


FinalState::FinalState(std::vector<std::string> _objects, std::vector<int> _numberOfObjects, std::vector<double> _sumPtRange)
{
  assert(_objects.size()==_numberOfObjects.size());
  objects = _objects;
  numberOfObjects = _numberOfObjects;
  Math::parallelBubbleSort(objects, numberOfObjects);
  if(_sumPtRange.empty())
    sumPtRange = vector<double>(0);
  else
    {
      assert(_sumPtRange.size()==2);
      assert((_sumPtRange[0]>=0)&&(_sumPtRange[1]>=0)&&
	     (_sumPtRange[0]<_sumPtRange[1]));
      sumPtRange = _sumPtRange;
    }
  
  return;
}

/// Take an input like "2e+2b11j_sumPt40-100" and store it as
/// objects = { "e+", "b", "j" }
/// numberOfObjects = { 2, 2, 11 }
/// sumPtRange = { 40, 100 }

FinalState::FinalState(std::string fs)
{
  objects = vector<string>(0);
  numberOfObjects = vector<int>(0);
  sumPtRange = vector<double>(0);
  
  if(fs.find("_sumPt")!=string::npos)
    {
      sumPtRange = vector<double>(2,0);
      sumPtRange[1] = FLT_MAX;

      string s = fs.substr(fs.find("_sumPt")+6);
      if(s.substr(s.length()-1)=="+")  // final state is in the form "2e+2b11j_sumPt100+"
	sumPtRange[0] = atof(s.substr(0,s.length()-1).c_str());
      else
	{
	  assert(s.find("-")!=string::npos); // else must be in the form "2e+2b11j_sumPt40-100", with a definite range specified
	  sumPtRange[0] = atof(s.substr(0,s.find("-")).c_str());
	  sumPtRange[1] = atof(s.substr(s.find("-")+1).c_str());
	}
      assert((sumPtRange[0]>=0)&&(sumPtRange[1]>=0)&&
	     (sumPtRange[0]<sumPtRange[1]));
      fs = fs.substr(0,fs.find("_sumPt"));
    }

  for(size_t i=0; i<fs.length(); i++)
    for(size_t j=i+1; j<=fs.length(); j++)
      if(
	 (j==fs.length()) ||
	 (atoi(fs.substr(j,1).c_str())>0)
	 )
	{
	  if(((fs.substr(i+1,1)=="0")||
	      (atoi(fs.substr(i+1,1).c_str())>0))) // number of objects > 9, i.e. double digits
	    {
	      numberOfObjects.push_back(atoi(fs.substr(i,2).c_str()));
	      objects.push_back(fs.substr(i+2,j-i-2));
	    }
	  else
	    {
	      numberOfObjects.push_back(atoi(fs.substr(i,1).c_str()));
	      objects.push_back(fs.substr(i+1,j-i-1));
	    }
	  i=j;
	}
  Math::parallelBubbleSort(objects, numberOfObjects);
  return;
}

/***  Accessors  ***/

std::vector<std::string> FinalState::getObjects() const
{
  vector<string> ans;
  for(size_t i=0; i<objects.size(); i++)
    for(int j=0; j<numberOfObjects[i]; j++)
      ans.push_back(objects[i]);
  return(ans);
}


// Turn the final state into a normal human-readable text string

std::string FinalState::getTextString() const
{
  string ans = "";
  for(size_t i=0; i<objects.size(); i++)
    ans += Math::ftoa(numberOfObjects[i]) + objects[i];
  if(!sumPtRange.empty())
    {
      ans += "_sumPt";
      ans += Math::ftoa(sumPtRange[0]);
      if(sumPtRange[1]==FLT_MAX)
	ans += "+";
      else
	ans += "-"+Math::ftoa(sumPtRange[1]);
    }
  return(ans);
}

// Turn the final state into a string appropriate for MadGraph input

std::string FinalState::getMadGraphTextString() const
{
  map<string,string> madgraphObjects;
  madgraphObjects["e+"] = "e+";
  madgraphObjects["e-"] = "e-";
  madgraphObjects["mu+"] = "mu+";
  madgraphObjects["mu-"] = "mu-";
  madgraphObjects["tau+"] = "ta+";
  madgraphObjects["tau-"] = "ta-";
  madgraphObjects["j"] = "j";
  madgraphObjects["jj"] = "jj";
  madgraphObjects["bb"] = "b b~";
  madgraphObjects["pmiss"] = "pmiss";
  madgraphObjects["w+"] = "w+";
  madgraphObjects["w-"] = "w-";
  madgraphObjects["z"] = "z";

  string ans = "";
  for(size_t i=0; i<objects.size(); i++)
    for(int j=0; j<numberOfObjects[i]; j++)
      ans += madgraphObjects[objects[i]]+" ";

  return(ans);
}

// Turn the final state into a string appropriate for PAW input

std::string FinalState::getPawString() const
{
  string pawString="";

  for(size_t i=0; i<objects.size(); i++)
    {
      int n = numberOfObjects[i];
      string o = objects[i];
      if(o.substr(o.length()-1)=="+")
	o=o.substr(0,o.length()-1)+"^+!";  // superscript a positive charge
      if(o.substr(o.length()-1)=="-")
	o=o.substr(0,o.length()-1)+"^-!";  // superscript a negative charge
      if(o.substr(0,2)=="mu")
	o="[m]"+o.substr(2);  // use Greek letter "mu"
      if(o.substr(0,3)=="tau")
	o="[t]"+o.substr(3);  // use Greek letter "tau"
      if(o.substr(0,2)=="ph")
	o="[g]"+o.substr(2);  // use Green letter "gamma"
      if(o.substr(0,5)=="pmiss")
	o="p&/"+o.substr(5);  // use "p" with a slash through it
      if(n==0)
	continue;
      if(n==1)
	pawString = pawString+" "+o;
      if(n>1)
	pawString = pawString+" "+Math::ftoa(n)+o;
    }
  if(pawString!="")
    pawString=pawString.substr(1);
  return(pawString);
}

int FinalState::getNumberOfThisObject(std::string _object) const
{
  if(_object=="e")
    return(getNumberOfThisObject("e+") + getNumberOfThisObject("e-"));
  if(_object=="mu")
    return(getNumberOfThisObject("mu+") + getNumberOfThisObject("mu-"));
  if(_object=="tau")
    return(getNumberOfThisObject("tau+") + getNumberOfThisObject("tau-"));
  if(_object=="l")
    return(getNumberOfThisObject("e")+getNumberOfThisObject("mu")+getNumberOfThisObject("tau"));

  int ans = 0;
  for(size_t i=0; i<objects.size(); i++)
    if(objects[i]==_object)
      ans = numberOfObjects[i];
  return(ans);
}

vector<double> FinalState::getSumPtRange() const
{
  return(sumPtRange);
}

bool FinalState::operator==(const FinalState& rhs) const
{
  double ans = 
    ( (objects == rhs.objects) &&
      (numberOfObjects == rhs.numberOfObjects) );
  return(ans);
}
  
bool FinalState::operator<(const FinalState& rhs) const
{
  return(getTextString()<rhs.getTextString());
}
