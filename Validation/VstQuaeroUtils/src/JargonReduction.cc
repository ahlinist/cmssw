/*************************************************
JargonReduction
*************************************************/


#include "Validation/VstQuaeroUtils/interface/JargonReduction.hh"
#include <string>
#include <cassert>
#include <fstream>
#include <iostream>
using namespace std;

/***  Constructor  ***/

JargonReduction::JargonReduction()
{
}

/***  Methods  ***/

void JargonReduction::load(string jargonReductionFileName)
{
  jargonReductionMap.clear();
  if(jargonReductionFileName!="")
    {
      ifstream fin(jargonReductionFileName.c_str());
      string jargon, s;
      if(fin)
	while(fin >> jargon)
	  {
	    getline(fin,s);
	    if(jargon.substr(0,1)=="#")
	      continue;
	    while((!s.empty())&&
		  ((s.substr(0,1)==" ")||
		   (s.substr(0,1)=="\t")))
	      s = s.substr(1);
	    while((!s.empty())&&
		  ((s.substr(s.length()-1)==" ")||
		   (s.substr(s.length()-1)=="\t")))
	      s = s.substr(0,s.length()-1);
	    
	    jargonReductionMap[jargon]=s;
	}
      fin.close();
    }
  return;
}

string JargonReduction::interpret(string jargon)
{
  if(jargonReductionMap.find(jargon)==jargonReductionMap.end())
    return(jargon);

  return(jargonReductionMap[jargon]);
}
