/**********************************************
Implementation of GraphicalOutputSimple class
Bruce Knuteson 2003 
(Migrated to new class GraphicalOutputSimple_paw)
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
#include "Validation/VstQuaeroUtils/interface/GraphicalOutputSimple_paw.hh"
using namespace std;



/******   Constructor   ******/

GraphicalOutputSimple_paw::GraphicalOutputSimple_paw(string _pawCommand)
{
  pawCommand = _pawCommand;
  paneNumber = 0;
}


/*****  Utility Methods  *****/

string GraphicalOutputSimple_paw::addUnits(string distributionName)
{
  //add units
  string units = "";
  if(distributionName=="zvtx")
    units = "cm";
  else if((distributionName.find("runHistory")!=string::npos))
    units = "pb^-1!";
  else if(distributionName=="D")
    units = "";
  else if(distributionName.find("/")!=string::npos)
    units = "";
  else if((distributionName.find("mass")!=string::npos)||
	  (distributionName.find("Mass")!=string::npos))	  
    units = "GeV";
  else if((distributionName.find("_eta")!=string::npos)||
	  (distributionName.find("_detEta")!=string::npos)||
	  (distributionName.find("#-#eta")!=string::npos)||
	  (distributionName.find("#-#detEta")!=string::npos)||
	  (distributionName.find("eltaEta")!=string::npos)||
	  (distributionName.find("eltaR")!=string::npos)
	  )
    units = "";
  else if((distributionName.find("delta")!=string::npos)||
	  (distributionName.find("Delta")!=string::npos)||
	  (distributionName.find("theta")!=string::npos)||
	  (distributionName.find("angle")!=string::npos)||
	  (distributionName.find("phi")!=string::npos))
    units = "rad";
  else
    units = "GeV";
  return(units);
}

void GraphicalOutputSimple_paw::reformatDistributionName(string& distributionName)
{
  bool weHavePassedAnEvenNumberOfPoundSigns = true;
  for(size_t j=0; j<distributionName.length(); j++)
    if(distributionName.substr(j,1)=="#")
      {
	if(weHavePassedAnEvenNumberOfPoundSigns)
	  distributionName = distributionName.substr(0,j)+"\""+distributionName.substr(j+1);
	weHavePassedAnEvenNumberOfPoundSigns = !weHavePassedAnEvenNumberOfPoundSigns;
      }
}


template<class T>
string GraphicalOutputSimple_paw::fillPawVector(string vectorName, const vector<T>& values)
{
  string ans = "";
  int n = values.size();
  ans +=  "v/cr "+vectorName+"("+Math::ftoa(n)+") r \n";  // create vector

  int lineLength = 50;
  int mult = n/lineLength;
  int remainder = n % lineLength;
  
  for(int i = 0; i < mult; i++)
    {
      int ii = i*lineLength +1;
      string irange = Math::ftoa(ii) + ":" + Math::ftoa(ii+lineLength-1);
      ans += "v/in "+vectorName+"("+irange+")"; 
    
      for(int j = i*lineLength; j < (i*lineLength + lineLength); j++)
	ans += " "+Math::ftoa(values[j]);
      ans += "\n";
    }
  if(remainder>0)
    {
      string kk = Math::ftoa(n-remainder+1);
      string krange = kk +":" + Math::ftoa(n);
      ans += "v/in "+vectorName+"("+krange+")";
      for(int m = (n-remainder); m < n; m++)
	ans += " "+Math::ftoa(values[m]);
      ans += "\n";
    }
  return(ans);
}


string GraphicalOutputSimple_paw::setShadedLevels(int nLevels, vector<double> rgb, vector<double> range)
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

void GraphicalOutputSimple_paw::addComment(string comment)
{
  pawCommand += "* "+comment+"\n";
  return;
}



// Likelihood ratio

void GraphicalOutputSimple_paw::addLikelihoodRatio(
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

void GraphicalOutputSimple_paw::addHeaderAndFooter(
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


/**  New Page  **/

void GraphicalOutputSimple_paw::newPage(int numberOfPlotsOnPage)
{
  assert((numberOfPlotsOnPage==1)||(numberOfPlotsOnPage==4));
  if(numberOfPlotsOnPage==1)
    pawCommand += "zone 1 1 \n";
  if(numberOfPlotsOnPage==4)
    pawCommand += "zone 2 2 \n";
  paneNumber = 0;
  return;
}


/**  Credit  **/

void GraphicalOutputSimple_paw::addCredit(string credit)
{
  pawCommand += "selnt 1 \n";
  pawCommand += "jtext 95 2 '"+credit+"' 0.2 0 r \n";
  return;
}


/*****  Plot Methods  *****/

/**  1d  **/


void GraphicalOutputSimple_paw::add1dPlot(string distributionName,
				      vector<double> binEdges,
				      vector<double> fs, vector<double> fb, vector<int> fd,
				      vector<bool> showBkgSigData,
				      string plotSize)
{
  add1dPlot(distributionName, binEdges,
	    vector<vector<double> >(1,fs), 
	    vector<vector<double> >(1,fb), 
	    fd, showBkgSigData, plotSize);
}



void GraphicalOutputSimple_paw::add1dPlot(
		string distributionName,
		vector<double> binEdges,
		vector<vector<double> > fs, vector<vector<double> > fb, vector<int> fd,
		vector<bool> showBkgSigData,
		string plotSize
		)
{
  string plotCommand = "";
  plotCommand += "* begin distribution "+distributionName+"\n";
  string distributionName1 = distributionName;
  size_t nbins = binEdges.size()-1;
  assert(fb.size()<10);
  assert(fs.size()<10);


  for(size_t i=0; i<fb.size(); i++)
    if(fb[i].empty())
      fb[i] = vector<double>(nbins,0);
  for(size_t i=0; i<fs.size(); i++)
    if(fs[i].empty())
      fs[i] = vector<double>(nbins,0);
  if(fd.empty())
    fd = vector<int>(nbins,0);
  for(size_t i=1; i<fb.size(); i++)
    {
      assert(fb[i].size()==nbins);
      for(size_t j=0; j<nbins; j++)
	fb[i][j] += fb[i-1][j];
    }
  for(size_t i=1; i<fs.size(); i++)
    {
      assert(fs[i].size()==nbins);
      for(size_t j=0; j<nbins; j++)
	fs[i][j] += fs[i-1][j];
    }

  double histoYMAX = 0.;
  int nx = nbins;
 
  Math::makeDifferent(binEdges);
  plotCommand += fillPawVector("vx",binEdges);

  if( (showBkgSigData[0]) &&
      (!fb.empty()) &&
      (histoYMAX < *max_element(fb.back().begin(), fb.back().end())))
    histoYMAX = *max_element(fb.back().begin(), fb.back().end());
  if( (showBkgSigData[1]) &&
      (!fs.empty()) &&
      (histoYMAX < *max_element(fs.back().begin(), fs.back().end())))
    histoYMAX = *max_element(fs.back().begin(), fs.back().end());
  if( showBkgSigData[2] &&
   (histoYMAX < *max_element(fd.begin(), fd.end())))

  
  histoYMAX *= 1.05;

  string units = addUnits(distributionName);
  reformatDistributionName(distributionName);
  
  plotCommand += fillPawVector("vfd", fd);
  plotCommand += "h/cre/bins 110 '' "+Math::ftoa(nx)+" vx\n";
  plotCommand += "h/put/cont 110 vfd\n";
  plotCommand += "max 110 "+Math::ftoa(histoYMAX)+"\n";
  plotCommand += "set dmod 1\n";

  string opt = "";

  if(showBkgSigData[0]) // if showBkg
    {
      int hbNumber = 121+fb.size();
	//mrenna      for(size_t j=fb.size()-1; j>=0; --j)
      int j=fb.size()-1;
      for(std::vector< std::vector<double> >::reverse_iterator iterFb = fb.rbegin(); iterFb != fb.rend(); ++iterFb)
	{
	  hbNumber--;
	  double r = 0.8*j/fb.size();
	  --j;
	  plotCommand += "color 9 1.0 "+Math::ftoa(1-r)+" "+Math::ftoa(1-r)+"\n";
	  plotCommand += "set hcol 1909\n";
	  //	  if(j==fb.size()-1)
	  if(iterFb==fb.rbegin())
	    plotCommand += "set hcol 1209\n";
	  plotCommand += fillPawVector("vfb", *iterFb);
	  plotCommand += "h/cre/bins "+Math::ftoa(hbNumber)+" '' "+Math::ftoa(nx)+" vx\n";
	  plotCommand += "h/put/cont "+Math::ftoa(hbNumber)+" vfb\n";
	  plotCommand += "max "+Math::ftoa(hbNumber)+" "+Math::ftoa(histoYMAX)+"\n";
	  plotCommand += "h/pl "+Math::ftoa(hbNumber)+" "+opt+"\n";
	  opt = "S";
	}
      plotCommand += "set hcol 2\n";
      plotCommand += "h/pl "+Math::ftoa(121+fb.size()-1)+" S\n";
    }
  if(showBkgSigData[1]) // if showSig
    {
      int hsNumber = 131+fs.size();
      //mrenna
      int j=fs.size()-1;
      for(std::vector< std::vector<double> >::reverse_iterator iterFs = fs.rbegin(); iterFs != fs.rend(); ++iterFs)
	//      for(size_t j=fs.size()-1; j>=0; j--)
	{
	  hsNumber--;
 	  double g = 0.8*j/fs.size();
	  j--;
	  plotCommand += "color 9 "+Math::ftoa(1-g)+" 1.0 "+Math::ftoa(1-g)+"\n";
	  plotCommand += "set hcol 1909\n";
	  //	  if(j==fs.size()-1)
	  if( iterFs==fs.rbegin() )
	    plotCommand += "set hcol 1309\n";
	  //	  plotCommand += fillPawVector("vfs", fs[j]);
	  plotCommand += fillPawVector("vfs", *iterFs);
	  plotCommand += "h/cre/bins "+Math::ftoa(hsNumber)+" '' "+Math::ftoa(nx)+" vx\n";
	  plotCommand += "h/put/cont "+Math::ftoa(hsNumber)+" vfs\n";
	  plotCommand += "max "+Math::ftoa(hsNumber)+" "+Math::ftoa(histoYMAX)+"\n";
	  //mrenna	  plotCommand += "h/pl "+Math::ftoa(hsNumber)+" "+opt+"\n";
	  opt = "S";
	}
      plotCommand += "set hcol 3\n";
      plotCommand += "h/pl "+Math::ftoa(131+fs.size()-1)+" eS\n";
    }
  if(showBkgSigData[0]) // if showBkg
    {
      plotCommand += "set dmod 2\n";
      plotCommand += "set hcol 2\n";
      plotCommand += "h/pl "+Math::ftoa(121+fb.size()-1)+" S\n";
      plotCommand += "set dmod 1\n";
    }

  if(showBkgSigData[2]) // if showData
    {
      plotCommand += "set hcol 1\n";
      plotCommand += "h/pl 110 e"+opt+"\n";
      opt = "S"; // "Same"
    }

  assert((plotSize=="small")||(plotSize=="large"));

  if(plotSize=="small")
    {
      plotCommand += "jtext 50 -15 "+distributionName+" 0.3 0 c \n";
      if(units!="")
	plotCommand += "jtext 107 -7 "+units+" 0.32 0 l \n";
    }
  if(plotSize=="large")
    {
      plotCommand += "jtext 50 -10 "+distributionName+" 0.5 0 c \n";
      if(units!="")
	plotCommand += "jtext 107 -3 "+units+" 0.43 0 l \n";
    }

  plotCommand += "* end distribution "+distributionName1+"\n";

  pawCommand += plotCommand;
}


void GraphicalOutputSimple_paw::add1dPlots(vector<string> distributionNames, 
		  vector<bool> showBkgSigData,
		  string colliderRun,
		  string experiment, 
		  string finalstate,
		  double logLikelihood
		  )
{
  cout << "This is not yet implemented..." <<endl;
  return;
}


void GraphicalOutputSimple_paw::add1dPlots(vector<string> distributionNames, 
		const vector<double>& weightsBkgSM,
		const vector<double>& weightsSigBkg,
		const vector<vector<double> >& variableValuesBkgSM, 
		const vector<vector<double> >& variableValuesSigBkg, 
		const vector<vector<double> >& variableValuesData,
		vector<bool> showBkgSigData,
		string colliderRun, 
		string experiment, 
		string finalstate, 
		double logLikelihood,
		bool justShowOne)
{
  vector<string> sources;
  vector<double> sourceWeights;
  add1dPlots(distributionNames, 
	     vector<vector<double> >(1,weightsBkgSM),
	     vector<vector<double> >(1,weightsSigBkg),
	     vector<vector<vector<double> > >(1,variableValuesBkgSM), 
	     vector<vector<vector<double> > >(1,variableValuesSigBkg), 
	     variableValuesData, sources, sourceWeights,
	     showBkgSigData, colliderRun, experiment, finalstate, logLikelihood, justShowOne);
}



void GraphicalOutputSimple_paw::add1dPlots(
		vector<string> distributionNames,
		const vector<vector<double> >& bkgWeights,  // [iSubComponent][iEvent]
		const vector<vector<double> >& sigWeights,  // [iSubComponent][iEvent]
		const vector<vector<vector<double> > >& bkgEvents, // [iSubComponent][iEvent][iVariable]
		const vector<vector<vector<double> > >& sigEvents, // [iSubComponent][iEvent][iVariable]
		const vector<vector<double> >& dataEvents, // [iEvent][iVariable]
		vector<string> sources, vector<double> sourcesWeight,
		vector<bool> showBkgSigData,
		string colliderRun, 
		string experiment, 
		string fs, 
		double logLikelihood,
		bool justShowOne,
		string specificDistribution,
		vector<double> windowLimitsLeft,
		vector<double> windowLimitsRight
		)
{
  // sources are currently ignored in paw implementation...



  logLikelihood = max(-100.,logLikelihood);
  double s=0, b=0;
  for(size_t i=0; i<sigWeights.size(); i++)
    s += accumulate(sigWeights[i].begin(),sigWeights[i].end(),0.);
  for(size_t i=0; i<bkgWeights.size(); i++)    
    b += accumulate(bkgWeights[i].begin(),bkgWeights[i].end(),0.);
  int N = dataEvents.size();


  vector<double> combinedBkgWeights, combinedSigWeights;
  vector<vector<double> > combinedBkgEvents, combinedSigEvents;
  for(size_t i=0; i<sigWeights.size(); i++)
    combinedSigWeights.insert(combinedSigWeights.end(),sigWeights[i].begin(),sigWeights[i].end());
  for(size_t i=0; i<bkgWeights.size(); i++)
    combinedBkgWeights.insert(combinedBkgWeights.end(),bkgWeights[i].begin(),bkgWeights[i].end());
  for(size_t i=0; i<sigEvents.size(); i++)
    combinedSigEvents.insert(combinedSigEvents.end(),sigEvents[i].begin(),sigEvents[i].end());
  for(size_t i=0; i<bkgEvents.size(); i++)
    combinedBkgEvents.insert(combinedBkgEvents.end(),bkgEvents[i].begin(),bkgEvents[i].end());


  //assert(comparableQ(sigEvents,dataEvents));
  //assert(comparableQ(sigEvents,bkgEvents));

  double sigMultiplicativeFactor = 1.;
  //sigMultiplicativeFactor = (int)pow(10.,(int)log10(b/s));

  string finalstate=fs;

  size_t d = distributionNames.size();
  for(size_t j=0; j<dataEvents.size(); j++)
    assert(dataEvents[j].size()==d);
  for(size_t i=0; i<sigEvents.size(); i++)
    for(size_t j=0; j<sigEvents[i].size(); j++)
    assert(sigEvents[i][j].size()==d);
  for(size_t i=0; i<bkgEvents.size(); i++)
    for(size_t j=0; j<bkgEvents[i].size(); j++)
      assert(bkgEvents[i][j].size()==d);
  if(justShowOne)
    d = min(1,(int) d);
	    
  pawCommand += "* begin finalState "+fs+"\n";

  pawCommand += "set xwin 3 \n";
  pawCommand += "set vsiz 0.35 \n";
  pawCommand += "set xval .2 \n";
  pawCommand += "set yval 0 \n";
  pawCommand += "set ndvz 0 \n";
  //pawCommand += "zone 2 2\n";

  if(d==0)
    {
      pawCommand += "selnt 10\n";
      pawCommand += "opt ntic\n null 0 1 0 1 AB\n opt tic\n";
      pawCommand += "gr/set txci 3\n jtext 50 50 'H = "+Math::ftoa(Math::nice(s))+"' 0.4 0 c \n";
      pawCommand += "gr/set txci 2\n jtext 50 40 'SM = "+Math::ftoa(Math::nice(b))+"' 0.4 0 c \n";
      pawCommand += "gr/set txci 1\n jtext 50 30 'D = "+Math::ftoa(N)+"' 0.4 0 c \n";    
      addHeaderAndFooter(colliderRun, experiment, finalstate, logLikelihood, 0, 1, justShowOne);
    }  
  else
    {
      // loop over variables
      vector<int> nbins;
      Math::makeNiceHistogramRange(combinedBkgWeights, combinedSigWeights, 
				   combinedBkgEvents, combinedSigEvents, 
				   dataEvents, range, nbins);
      
      if(nbins!=vector<int>(0))
	{
	  for(size_t k=0; k<d; k++)
	    {
	      
	      vector<double> xrange = range[k];
	      int nx = nbins[k];
	      
	      vector<double> v(1);
	      double dx = (xrange[1]-xrange[0])/nx;
	      string vv1="", vfs="", vfb="", vfd="";
	      vector<vector<double> > fs(sigEvents.size(),vector<double>(nx));
	      vector<vector<double> > fb(bkgEvents.size(),vector<double>(nx));
	      vector<int> fd(nx);
	      for(size_t i=0; i<sigEvents.size(); i++)
		{
		  for(size_t j=0; j<sigEvents[i].size(); j++)
		    {
		      int bin = (int)((sigEvents[i][j][k]-xrange[0])/dx);
		      if((bin>=0)&&(bin<nx)) 
			fs[i][bin] += sigMultiplicativeFactor*sigWeights[i][j];
		    }
		}
	      for(size_t i=0; i<bkgEvents.size(); i++)
		{
		  for(size_t j=0; j<bkgEvents[i].size(); j++)
		    {
		      int bin = (int)((bkgEvents[i][j][k]-xrange[0])/dx);
		      if((bin>=0)&&(bin<nx))
			fb[i][bin] += bkgWeights[i][j];
		    }
		}
	      for(size_t i=0; i<dataEvents.size(); i++)
		{
		  int bin = (int)((dataEvents[i][k]-xrange[0])/dx);
		  if((bin>=0)&&(bin<nx))
		    fd[bin]++;
		} 
	      add1dPlot(distributionNames[k], 
			deriveEvenlySpacedBinEdges(xrange[0], xrange[1], nx), 
			fs, fb, fd, 
			showBkgSigData); 
	      //if(k % 4==0) // if starting paw page
	      int pageNumber = 0;
	      if(d>4)
		pageNumber = k/4+1;
	      addHeaderAndFooter(colliderRun, experiment, finalstate, logLikelihood, 0, pageNumber, justShowOne);
	    }
	}      
    }
  pawCommand += "* end finalState "+fs+"\n";
}


/**  2d  **/

void GraphicalOutputSimple_paw::add2dPlot(string distributionName1, string distributionName2, 
				vector<double> range1, vector<double> range2, 
				int nbins1, int nbins2, 
				vector<vector<double> > fs, vector<vector<double> > fb, vector<vector<int> > fd, 
				vector<bool> showBkgSigData)
{
  string plotCommand = "";
  int nbins = nbins1*nbins2;

  vector<double> flat_fs(nbins), flat_fb(nbins), flat_fd(nbins);
  for(int i=0; i < nbins; i++)
    {
      flat_fs[i] = fs[i%nbins1][i/nbins1];
      flat_fb[i] = fb[i%nbins1][i/nbins1];
      flat_fd[i] = fd[i%nbins1][i/nbins1];
    }

  plotCommand += fillPawVector("vfs",flat_fs);
  plotCommand += fillPawVector("vfb",flat_fb);
  plotCommand += fillPawVector("vfd",flat_fd);

  reformatDistributionName(distributionName1);
  reformatDistributionName(distributionName2);
  
  plotCommand += 
    "2dh 101 '' "+
    Math::ftoa(nbins1)+" "+Math::ftoa(range1[0])+" "+Math::ftoa(range1[1])+" "+
    Math::ftoa(nbins2)+" "+Math::ftoa(range2[0])+" "+Math::ftoa(range2[1])+"\n";
  plotCommand += 
    "2dh 102 '' "+
    Math::ftoa(nbins1)+" "+Math::ftoa(range1[0])+" "+Math::ftoa(range1[1])+" "+
    Math::ftoa(nbins2)+" "+Math::ftoa(range2[0])+" "+Math::ftoa(range2[1])+"\n";

  plotCommand += "h/put/cont 101 vfb\n";
  plotCommand += "h/put/cont 102 vfs\n";

  plotCommand += "palette 1 2 0 4 \n";
  plotCommand += "set hcol 0.0 \n";
  plotCommand += "set hcol 2 \n";
  plotCommand += "set vsiz 0.25 \n";
  plotCommand += "set xwin 2. \n";
  string angle = "-60";
  string a2x = "95";
  string a2y = "-25";
  string a1x = "-15";
  string a1y = "-5";
  string textsize = "0.30";
  plotCommand += "set xval 0.25 \n";
  plotCommand += "set yval 0.1 \n";
  plotCommand += "angle 40 "+angle+" \n";
  plotCommand += "set ndvz 110 \n";
  plotCommand += "h/pl 101 SURF4BBFB \n";
  plotCommand += "jtext "+a2x+" "+a2y+" "+distributionName2+" "+textsize+" 0 c \n";
  plotCommand += "jtext "+a1x+" "+a1y+" "+distributionName1+" "+textsize+" 0 c \n";
  plotCommand += "gr/att/color_table 41 0.25 1.0 0.25\n";
  plotCommand += "palette 1 1 41\n";
  plotCommand += "set hcol 41 \n";
  plotCommand += "angle 40 "+angle+" \n";
  plotCommand += "h/pl 102 SURF4BBFB \n";
  plotCommand += "jtext "+a2x+" "+a2y+" "+distributionName2+" "+textsize+" 0 c \n";
  plotCommand += "jtext "+a1x+" "+a1y+" "+distributionName1+" "+textsize+" 0 c \n";
  plotCommand += "set vsiz 0.35 \n";
  plotCommand += "set xwin 3. \n";

  pawCommand += plotCommand;
}


/*****  Draw  *****/

void GraphicalOutputSimple_paw::draw(string plotFileName, string pawCommandFileName, string localDir)
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

  string executionCommand = "/cern/pro/bin/paw -b "+pawCommandFileName;   
  executionCommand += " >& /dev/null";
  string tmpFileName = Math::getTmpFilename();
  ofstream ftmp(tmpFileName.c_str());
  ftmp << "#!/bin/sh" << endl;
  ftmp << "source /etc/bashrc" << endl;
  //  ftmp << "setup cern 2002" << endl;
  ftmp << "/cern/pro/bin/paw -b $1" << endl;
  ftmp.close();
  system(("chmod u+x "+tmpFileName).c_str());
  //#ifdef QUAERO_CC_PLATFORM_clued0
  executionCommand = "cd "+localDir+"; source "+tmpFileName+" "+pawCommandFileName+" 2>&1 > /dev/null";
  //#endif

  // cout << executionCommand << endl;
  system(executionCommand.c_str());
  system(("rm "+tmpFileName).c_str());

  return;
}

/*****  Addition operator  *****/

GraphicalOutputSimple_paw GraphicalOutputSimple_paw::operator+(const GraphicalOutputSimple_paw& rhs) const
{
  return(GraphicalOutputSimple_paw(this->pawCommand+rhs.pawCommand));
}





