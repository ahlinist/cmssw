/**********************************************
Implementation of GraphicalOutputSimple class
Bruce Knuteson 2003
**********************************************/


#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <numeric>
#include "Validation/VstMathUtils/interface/Math.hh"
#include "Validation/VstQuaeroUtils/interface/FinalState.hh"
#include "Validation/VstQuaeroUtils/interface/GraphicalOutputSimple.hh"
using namespace std;



/******   Constructor   ******/

GraphicalOutputSimple::GraphicalOutputSimple(string _pawCommand)
{
  pawCommand = _pawCommand;
  paneNumber = 0;
}


/*****  Utility Methods  *****/


bool comparableQ(const vector<vector<double> > & m1, const vector<vector<double> > & m2)
{
  return( (m1.size()==0) ||
	  (m2.size()==0) ||
	  (m1[0].size()==m2[0].size())
	  );
}



string GraphicalOutputSimple::setShadedLevels(int nLevels, vector<double> rgb, vector<double> range)
{
  string ans;
  assert(nLevels<=50);
  if(range.empty())
    {
      range=vector<double>(2);
      range[1] = 1.;
    }
  assert(range.size()==2);
  assert(range[1]>range[0]);
  vector<double> rgbShaded(3);
  for(int i=0; i<nLevels; i++)
    {
      double intensity = range[0]+(range[1]-range[0])*(i+0.5)/nLevels;
      if(intensity<0.5)
	for(int j=0; j<3; j++)	
	  rgbShaded[j] = rgb[j]*2*intensity;
      else
	for(int j=0; j<3; j++)	
	  rgbShaded[j] = rgb[j]+(1-rgb[j])*2*(intensity-0.5);
	
      ans += "gr/att/color_table "+Math::ftoa(21+i)+" "+Math::ftoa(rgbShaded[0])+" "+Math::ftoa(rgbShaded[1])+" "+Math::ftoa(rgbShaded[2])+"\n";
    }
  ans += "palette 1 "+Math::ftoa(nLevels)+" ";
  for(int i=0; i<nLevels; i++)
    ans += Math::ftoa(21+i)+" ";
  ans += "\n";
  return(ans);
}


/*****  Adornment Methods  *****/


// Likelihood ratio

void GraphicalOutputSimple::addLikelihoodRatio(
		std::string colliderRun, 
		std::string experiment, 
		std::string fs, 
		double logLikelihood,
		double expectedEvidence)
{
  logLikelihood = max(-100.,logLikelihood);
  for(int i=1; i<=2; i++)
    {
      while(true)  // replace a previously inserted logL comment with an actual PAW command
	{
	  string keyString = "*"+Math::ftoa(i)+" logL "+colliderRun+" "+experiment+" "+fs;
	  int k = pawCommand.find(keyString);
	  if(k<0)
	    break;
	  pawCommand.replace(k,keyString.length(), 
			     (i==1 ? "" : 
			      (Math::toleranceRound(logLikelihood,0.1) > 0 ? "+" : "") +
			      Math::ftoa(Math::toleranceRound(logLikelihood,0.1))));
	}
      while(true)  // replace a previously inserted expectedEvidence comment with an actual PAW command
	{
	  string keyString = "*"+Math::ftoa(i)+" expectedEvidence "+colliderRun+" "+experiment+" "+fs;
	  int k = pawCommand.find(keyString);
	  if(k<0)
	    break;
	  pawCommand.replace(k,keyString.length(),
			     (i==1 ? "" :
			      Math::ftoa(min(0.,Math::toleranceRound(-expectedEvidence,0.1)))));
	}
    }
}


/** Header and Footer **/

void GraphicalOutputSimple::addHeaderAndFooter(
		std::string colliderRun, 
		std::string experiment, 
		std::string fs, 
		double logLikelihood,
		double expectedEvidence,
		double pageNumber,
		bool singlePane
		)
{
  logLikelihood = max(-100.,logLikelihood);
  if(!singlePane)
    pawCommand += "selnt 1\n";
    
  pawCommand += "gr/set txci 1 \n";  // text color to black
  if((!singlePane)&&(pageNumber>0))
    pawCommand += "text 10 0 '"+Math::ftoa(pageNumber)+"' 0.3 0 c \n";  // write page number

  pawCommand += "gr/set txci 4\n";  // text color to blue
  double xmid=10, xleft=2, xright=18, y=19, dx=0.2, dy=0.6;
  string textSize1 = "0.4", textSize2 = "0.3";
  string text = "text";
  if(singlePane)
    {
      text = "jtext";
      xmid = 50;
      xleft = -10;
      xright = 100;
      y = 105;
      dx = 0;
      dy = 0;
      textSize1 = "0.3";
      textSize2 = "0.2";
    }
  if(fs!="")
    pawCommand += text+" "+Math::ftoa(xmid)+" "+Math::ftoa(y)+" '"+FinalState(fs).getPawString()+"' "+textSize1+" 0 c \n";  // write final state, 10 19
  
  pawCommand += "gr/set txci 1 \n";  // text color to black
  pawCommand += text+" "+Math::ftoa(xleft)+" "+Math::ftoa(y)+" '"+colliderRun+" "+experiment+"' "+textSize1+" 0 l \n";  // write collider run and experiment
  string commentString = "";
  if(logLikelihood==0)
    commentString = "*1 logL "+colliderRun+" "+experiment+" "+fs+" ";  // insert paw comment 
  if(!singlePane)
    {
      pawCommand += commentString + (string)text+" "+Math::ftoa(xright)+" "+Math::ftoa(y+dy/2)+" ' log?10! L    = '  "+textSize2+" 0 r \n ";
    }
  pawCommand += commentString + text+" "+Math::ftoa(xright+dx)+" "+Math::ftoa(y+dy/2)+" '"+
    (logLikelihood==0 ? "*2"+commentString.substr(2) :
     (Math::toleranceRound(logLikelihood,0.1) > 0 ? "+" : "") +
     Math::ftoa(Math::toleranceRound(logLikelihood,0.1)))
    +"' "+textSize2+" 0 l \n";  // write log likelihood

  pawCommand += "gr/att/color_table 31 0.5 0.5 0.5\n";
  pawCommand += "palette 1 1 31\n";
  pawCommand += "gr/set txci 31\n";
  commentString = "";
  if(!singlePane)
    {
      if(expectedEvidence==0)
	commentString = "*1 expectedEvidence "+colliderRun+" "+experiment+" "+fs+" "; // insert paw comment
      pawCommand += commentString + (string)text+" "+Math::ftoa(xright)+" "+Math::ftoa(y-dy/2)+" '\"l#log?10! L\"g# = '  "+textSize2+" 0 r \n ";
      pawCommand += commentString + text+" "+Math::ftoa(xright+dx)+" "+Math::ftoa(y-dy/2)+" '"+ (expectedEvidence==0 ? "*2"+commentString.substr(2) : Math::ftoa(max(-10.,min(0.,Math::toleranceRound(-expectedEvidence,0.1))))) + "' "+textSize2+" 0 l \n";  // write expected evidence
    }
  pawCommand += "gr/set txci 1\n";

  paneNumber = (paneNumber+1)%4;

  return;
}

/**  Credit  **/

void GraphicalOutputSimple::addCredit(string credit)
{
  pawCommand += "selnt 1 \n";
  pawCommand += "jtext 95 2 '"+credit+"' 0.2 0 r \n";
  return;
}


/*****  Plot Methods  *****/

/**  1d  **/

std::vector<double> GraphicalOutputSimple::deriveEvenlySpacedBinEdges(double lo, double hi, int nbins)
{
  vector<double> binEdges = vector<double>(nbins+1);
  double dx = (hi-lo)/nbins;
  for(int i=0; i<nbins+1; i++)
    binEdges[i] = lo + i*dx;
  return(binEdges);
}



/*****  Draw  *****/

void GraphicalOutputSimple::draw(string plotFileName, string pawCommandFileName, string localDir)
{

  string command = "";
#ifdef QUAERO_CC_PLATFORM_rtc
  command = command + "1 \n";
#else
  command = command + "1.cdf13.uchicago.edu:0.0 \n";
#endif
  string plotFileNameWithNoDoubleSlashes = plotFileName.substr(0,1);
  for(size_t i=1; i<plotFileName.length(); i++)
    if(plotFileName.substr(i-1,2)!="//")
      plotFileNameWithNoDoubleSlashes += plotFileName.substr(i,1);
  if(pawCommandFileName=="")
    pawCommandFileName = Math::getTmpFilename();
  command = command + "filecase keep \n";
  command = command + "fort/file 66 "+plotFileNameWithNoDoubleSlashes+" \n";
  command = command + "meta 66 -111 \n";
  command = command + "opt tic \n";
  command = command + "opt zfl1 \n";
  command = command + "graphics/set csiz 0.4 \n";
  command = command + "opt nbox \n";
  command = command + "set hwid 4 \n";
  command = command + "set ywid 4 \n";
  command = command + "set xwin 3 \n";
  command = command + "set xwid 4 \n";
  command = command + "set bwid 4 \n";
  command = command + "set fwid 4 \n";
  command = command + "set vsiz 0.35 \n";
  command = command + "set tsiz 0.5 \n";
  command = command + "set asiz 0.5 \n";
  command = command + "set xval .2 \n";
  command = command + "set xlab 2.1 \n";
  command = command + "set xmgl 3.0 \n";
  command = command + "set pass 3 \n";
  command = command + "set cshi .03 \n";
  command = command + "set csiz 0.28 \n";
  command = command + "set pwid 1. \n";
  command = command + "igset lwid 2. \n";
  command = command + "igset mtyp 20 \n";
  command = command + "igset mscf .2 \n";
  command = command + "opt sta \n";
  command = command + "set stat 10 \n";
  command = command + "alias/create jtext 'exec percent#percent_text' \n";
  command = command + "alias/create jline 'exec percent#percent_line' \n";

  pawCommand = command + pawCommand;
  pawCommand += "close 66 \n";

  ofstream fpaw(pawCommandFileName.c_str());
  fpaw << pawCommand << endl;
  fpaw.close();

  //string executionCommand = "/bin/sh -c '. /usr/local/etc/setups.sh 2>&1; setup cern 2>&1; cd "+GlobalVariables::localDir+"; paw < "+ pawCommandFileName+"; ' 2>&1";
  //string executionCommand = "cd "+GlobalVariables::localDir+"; /cern/pro/bin/paw < "+pawCommandFileName;
  //string executionCommand = "/bin/sh -c 'export HOME="+GlobalVariables::localDir+"; cd "+GlobalVariables::localDir+"; "+GlobalVariables::localDir+"paw < "+pawCommandFileName+"' 2>&1 ";

  string executionCommand = "paw < "+pawCommandFileName;   
  executionCommand += " >& /dev/null";
  string tmpFileName = Math::getTmpFilename();
  ofstream ftmp(tmpFileName.c_str());
  ftmp << "#!/bin/sh" << endl;
  ftmp << "source /etc/bashrc" << endl;
  ftmp << "setup cern 2002" << endl;
  ftmp << "paw < $1" << endl;
  ftmp.close();
  system(("chmod u+x "+tmpFileName).c_str());
#ifdef QUAERO_CC_PLATFORM_clued0
  executionCommand = "cd "+localDir+"; source "+tmpFileName+" "+pawCommandFileName+" 2>&1 > /dev/null";
#endif

  // cout << executionCommand << endl;
  system(executionCommand.c_str());
  system(("rm "+tmpFileName).c_str());

  return;
}

void GraphicalOutputSimple::writePawCommand(std::string pawCommandFileName)
{
  ofstream fout(pawCommandFileName.c_str());
  fout << pawCommand << endl;
  fout.close();
}



