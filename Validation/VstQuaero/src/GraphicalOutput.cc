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
#include "Validation/VstQuaero/interface/QuaeroIO.hh"
#include "Validation/VstQuaero/interface/Discriminator.hh"
#include "Validation/VstQuaero/interface/GraphicalOutput.hh"
#include "Validation/VstQuaero/interface/RCPParameters.hh"
using namespace std;


/*****  Constructor  *****/


GraphicalOutput::GraphicalOutput(string _pawCommand): GraphicalOutputSimple_paw(_pawCommand)
{
  return;
}


/*****  Plot Methods  *****/


void GraphicalOutput::addDiscriminatorPlots(vector<string> distributionNames,
					    string tag,
					    Discriminator& discriminator,
					    vector<vector<double> > hVariableValues, 
					    vector<vector<double> > smVariableValues, 
					    vector<double> hWeights, 
					    vector<double> smWeights,
					    vector<vector<double> > dataEvents,
					    bool showKernelEstimates)
{
  
  int d = distributionNames.size();
	    
  //pawCommand += "zone 2 2 \n";
  pawCommand += "opt nsta \n";
  int n1d = 500;
  int n2d = 50;

  double h = discriminator.discriminants[tag].hWt;
  double b = discriminator.discriminants[tag].bWt;

  // One-dimensional distributions
  //cout << "working on 1d" << endl;
  for(int k=0; k<d; k++)
    {
      vector<bool> integrateOutThisDirection = vector<bool>(d,true);
      integrateOutThisDirection[k] = false;
      FewKDE sigEstimate_k = discriminator.discriminants[tag].h.collapse(integrateOutThisDirection);
      FewKDE bkgEstimate_k = discriminator.discriminants[tag].b.collapse(integrateOutThisDirection);
      // cout << "sigEstimate_k = " << sigEstimate_k << endl;
      // cout << "bkgEstimate_k = " << bkgEstimate_k << endl;

      vector<double> xrange = range[k];
      int nx = n1d;
      double dx = (xrange[1]-xrange[0])/nx;
      vector<double> fs, fb;
      vector<int> fd = vector<int>(nx,0);
      for(int i=0; i<nx; i++)
	{
	  double x = dx*(i+.5)+xrange[0];
	  fs.push_back(h*sigEstimate_k.evaluate(vector<double>(1,x)));
	  fb.push_back(b*bkgEstimate_k.evaluate(vector<double>(1,x)));
	}
      vector<bool> showBkgSigData = vector<bool>(3,true);
      showBkgSigData[2] = false;
      if(showKernelEstimates)
	add1dPlot(distributionNames[k], 
		  deriveEvenlySpacedBinEdges(xrange[0], xrange[1], nx), 
		  fs, fb, fd, showBkgSigData);
    }

  if(false) // don't show 2d distributions
    {
  // Two-dimensional distributions
  if(d==3)
    pawCommand += "zone 2 3 \n";
  for(int k=0; k<d; k++)
    for(int l=k+1; l<d; l++)
      {
	vector<bool> integrateOutThisDirection = vector<bool>(d,true);
	integrateOutThisDirection[k] = integrateOutThisDirection[l] = false;
	FewKDE sigEstimate_k = discriminator.discriminants[tag].h.collapse(integrateOutThisDirection);
	FewKDE bkgEstimate_k = discriminator.discriminants[tag].b.collapse(integrateOutThisDirection);
	
	int nx = n2d, ny = n2d;
	double dx = (range[k][1]-range[k][0])/nx;
	double dy = (range[l][1]-range[l][0])/ny;
	vector<vector<double> > fs(nx,vector<double>(ny)), fb(nx,vector<double>(ny));
	vector<vector<int> > fd = vector<vector<int> >(nx,vector<int>(ny,0));
	vector<double> x(2);
	for(int i=0; i<nx; i++)
	  for(int j=0; j<ny; j++)
	    {
	      x[0] = dx*(i+.5)+range[k][0];
	      x[1] = dy*(j+.5)+range[l][0];
	      fs[i][j] = h*sigEstimate_k.evaluate(x);
	      fb[i][j] = b*bkgEstimate_k.evaluate(x);
	    }
      vector<bool> showBkgSigData = vector<bool>(3,true);
      showBkgSigData[2] = false;
      if(showKernelEstimates)
	add2dPlot(distributionNames[k], distributionNames[l], range[k], range[l], nx, ny, fs, fb, fd, showBkgSigData);
      }
    }
  if(d==3)
    pawCommand += "zone 2 2 \n";

  // The discriminant
  vector<double> dataD = vector<double>(dataEvents.size());
  for(size_t i=0; i<dataEvents.size(); i++)
    dataD[i] = discriminator.discriminants[tag].compute(dataEvents[i]);
  //vector<double> hD = discriminator.hD;
  //vector<double> smD = discriminator.bD;
  vector<double> hD, smD;
  for(size_t i=0; i<hVariableValues.size(); i++)
    hD.push_back(discriminator.discriminants[tag].compute(hVariableValues[i]));
  for(size_t i=0; i<smVariableValues.size(); i++)
    smD.push_back(discriminator.discriminants[tag].compute(smVariableValues[i]));
  
    vector<double> binEdges = discriminator.binPoints;
    binEdges.insert(binEdges.begin(), -0.01);
    binEdges.insert(binEdges.end(), 1.01);
    /*
      vector<double> binEdges(13);
      for(int i=0; i<binEdges.size(); i++)
      binEdges[i] = i/10.-0.1;
    */

  vector<int> dk = Math::putIntoBins(discriminator.binPoints, dataD);
  vector<double> hk = Math::putIntoBins(discriminator.binPoints, hD, hWeights); 
  vector<double> bk = Math::putIntoBins(discriminator.binPoints, smD, smWeights); 

  add1dPlot((string)"D", binEdges, hk, bk, dk, vector<bool>(3,true));

  int nLevels = discriminator.binPoints.size()+1;
  int levelStep = (int)((nLevels/20)+1); // 20 is the maximum number of levels we want to show (50 is the maximum number that PAW can show)
  nLevels /= levelStep;
  for(int i=0; i<nLevels; i++)
    {
      string greyLevel = Math::ftoa(1.*(i+0.5)/nLevels);
      pawCommand += "gr/att/color_table "+Math::ftoa(21+i)+" "+greyLevel+" "+greyLevel+" "+greyLevel+"\n";
    }
  pawCommand += "palette 1 "+Math::ftoa(nLevels)+" ";
  for(int i=0; i<nLevels; i++)
    pawCommand += Math::ftoa(21+i)+" ";
  pawCommand += "\n";
  pawCommand += "set lwid 20. \n";
  for(size_t i=0; i<binEdges.size()-1; i++)
    {
      double x1 = binEdges[i]*100;
      double x2 = binEdges[i+1]*100;      
      pawCommand += "set plci "+Math::ftoa(21+((int)(i/levelStep)))+" \n";
      pawCommand += "jline "+Math::ftoa(x1)+" 100 "+Math::ftoa(x2)+" 100 \n";
    }
  pawCommand += "set lwid 2. \n";

  for(size_t i=0; i<distributionNames.size(); i++)
    reformatDistributionName(distributionNames[i]);

  // Bins in the original variable space
  if(d==1)
    {
      string plotCommand = "";
      int nx = n2d, ny = n2d;
      double dx = (range[0][1]-range[0][0])/nx;
      int nbins = nx*ny;
      plotCommand += "2dh 101 '' "+Math::ftoa(nx)+" "+Math::ftoa(range[0][0])+" "+Math::ftoa(range[0][1])+" "+Math::ftoa(ny)+" "+Math::ftoa(0)+" "+Math::ftoa(1)+"\n";

      vector<double> dd(nbins);
      for(int j=0; j<ny; j++)
	for(int i=0; i<nx; i++)
	  {
	    double x = range[0][0] + dx*(i+0.5);
	    dd[ny*j+i] = (int)(( upper_bound(discriminator.binPoints.begin(),
					     discriminator.binPoints.end(),
					     discriminator.discriminants[tag].compute(vector<double>(1,x)))
				 -discriminator.binPoints.begin()
				 ) / levelStep);
	  }
      plotCommand += fillPawVector("DD", dd);

      plotCommand += "h/put/cont 101 DD\n";
      plotCommand += "h/2d/cont 101 "+Math::ftoa(nLevels)+" 3 \n";
      plotCommand += "jtext 50 -15 "+distributionNames[0]+" 0.3 0 c \n";
      pawCommand += plotCommand;
    }
  if(d==2)
    {
      string plotCommand = "";
      int nx = n2d, ny = n2d;
      vector<double> xrange = range[1];
      vector<double> yrange = range[0];
      string xDistributionName = distributionNames[1];
      string yDistributionName = distributionNames[0];
      double dx = (xrange[1]-xrange[0])/nx;
      double dy = (yrange[1]-yrange[0])/ny;
      int nbins = nx*ny;
      plotCommand += "2dh 101 '' "+Math::ftoa(nx)+" "+Math::ftoa(xrange[0])+" "+Math::ftoa(xrange[1])+" "+Math::ftoa(ny)+" "+Math::ftoa(yrange[0])+" "+Math::ftoa(yrange[1])+"\n";
      plotCommand += "v/cre DD("+Math::ftoa(nbins)+") \n";

      vector<double> dd(nbins);
      for(int j=0; j<ny; j++)
	for(int i=0; i<nx; i++)
	  {
	    vector<double> x(2);
	    x[1] = xrange[0] + dx*(i+0.5);
	    x[0] = yrange[0] + dy*(j+0.5);
	    if(nLevels==1)
	      dd[ny*j+i] = 0;
	    else
	      dd[ny*j+i] = (int)(( upper_bound(discriminator.binPoints.begin(),discriminator.binPoints.end(),discriminator.discriminants[tag].compute(x))-discriminator.binPoints.begin() ) / levelStep);
	  }
      plotCommand += fillPawVector("DD",dd);
 
      plotCommand += "h/put/cont 101 DD\n";
      plotCommand += "h/2d/cont 101 "+Math::ftoa(nLevels)+" 3 \n";
      plotCommand += "jtext 50 -15 "+xDistributionName+" 0.3 0 c \n";
      plotCommand += "jtext -22 50 "+yDistributionName+" 0.3 90 c \n";
      pawCommand += plotCommand;

      pawCommand += "zone 2 2 \n";
    }
      
}


void GraphicalOutput::addDiscriminatorPlots(Discriminator& discriminator,
			   std::string colliderRun,
			   std::string experiment,
			   double likelihoodRatio)
{
  vector<double> binEdges = discriminator.binPoints;
  if(binEdges.empty())
    {
      assert(discriminator.hk.size()==1);
      assert(discriminator.bk.size()==1);
      if((discriminator.hk[0]==0)&&
	 (discriminator.bk[0]==0)&&
	 ((discriminator.dk.empty())||
	  ((discriminator.dk.size()==1)&&
	   (discriminator.dk[0]==0))))
	return;
    }
  pawCommand += "zone 1 1\n";
  pawCommand += "opt nsta\n";
  binEdges.insert(binEdges.begin(), -0.01);
  binEdges.insert(binEdges.end(), 1.01);
  add1dPlot((string)"D", binEdges, discriminator.hk, discriminator.bk, discriminator.dk, vector<bool>(3,true), "large");
  int nLevels = binEdges.size()-1;
  int levelStep = (int)(nLevels/20 + 1); // 20 is the maximum number of levels we wish to show
  nLevels /= levelStep;
  for(int i=0; i<nLevels; i++)
    {
      string greyLevel = Math::ftoa(1.*(i+0.5)/nLevels);
      pawCommand += "gr/att/color_table "+Math::ftoa(21+i)+" "+greyLevel+" "+greyLevel+" "+greyLevel+"\n";
    }
  pawCommand += "palette 1 "+Math::ftoa(nLevels)+" ";
  for(int i=0; i<nLevels; i++)
    pawCommand += Math::ftoa(21+i)+" ";
  pawCommand += "\n";
  pawCommand += "set lwid 40. \n";
  for(size_t i=0; i<binEdges.size()-1; i++)
    {      
      double x1 = binEdges[i]*100;
      double x2 = binEdges[i+1]*100;   
      pawCommand += "set plci "+Math::ftoa(21+((int)(i/levelStep)))+" \n";
      pawCommand += "jline "+Math::ftoa(x1)+" 100 "+Math::ftoa(x2)+" 100 \n";
    }
  pawCommand += "set lwid 2. \n";
  addHeaderAndFooter(colliderRun, experiment, "", likelihoodRatio, discriminator.getExpectedEvidence());
  return;
}


void GraphicalOutput::addDiscriminatorPlots( map<vector<string>,vector<vector<double> > > contributionsToD,
					     string colliderRun,
					     string experiment,
					     double expectedEvidence,
					     double likelihoodRatio)
{

  if(contributionsToD.empty())
    return;
  int nBins = 0;

  for(map<vector<string>,vector<vector<double> > >::iterator i = contributionsToD.begin(); i!= contributionsToD.end(); i++)
    {
      assert(i->first.size()==3);
      assert(i->second.size()==3);
      if((colliderRun==i->first[0])&&
	 (experiment==i->first[1]))
	nBins = i->second[2].size();
    }
  if(nBins==0)
    return;

  pawCommand += "zone 1 1 \n";
  pawCommand += "opt ntic\n null 0 1 0 1 A\n opt tic\n";

  vector<vector<string> > finalStates(nBins); // first index labels discriminator bin, second index labels relative contribution in the bin
  vector<vector<vector<double> > > contributionFromThisFinalStateToThisBin(nBins, vector<vector<double> >(3)); // first index labels discriminator bin; second index labels h+b, h, b, or d; third index labels relative contribution in the bin
  vector<vector<double> > hbDelta(nBins);

  for(map<vector<string>,vector<vector<double> > >::iterator i = contributionsToD.begin(); i!= contributionsToD.end(); i++)
    {
      if((colliderRun != i->first[0]) || (experiment != i->first[1]))
	continue;
      for(int j=0; j<nBins; j++)
	{
	  finalStates[j].push_back(i->first[2]);
	  hbDelta[j].push_back(fabs(i->second[0][j]-i->second[1][j]));
	  for(int k=0; k<3; k++)
	    contributionFromThisFinalStateToThisBin[j][k].push_back(i->second[k][j]);
	}
    }
  for(int i=0; i<nBins; i++)
    {
      vector<double> tmp = hbDelta[i];
      Math::parallelBubbleSort(tmp, finalStates[i]);
      for(int k=0; k<3; k++)
	{
	  vector<double> tmp = hbDelta[i];
	  Math::parallelQuickSort(tmp, contributionFromThisFinalStateToThisBin[i][k]);
	}
    }
  
  assert(nBins>0);
  vector<double> totalHeightPerBin(nBins);
  for(int i=0; i<nBins; i++)
    {
      vector<double> totalHeightPerhbd(3);
      for(int k=0; k<3; k++)
	totalHeightPerhbd[k] = accumulate(contributionFromThisFinalStateToThisBin[i][k].begin(), contributionFromThisFinalStateToThisBin[i][k].end(), 0.);      
      totalHeightPerBin[i] = *max_element(totalHeightPerhbd.begin(), totalHeightPerhbd.end());
    }
  double maxTotalHeight = *max_element(totalHeightPerBin.begin(), totalHeightPerBin.end());

  pawCommand += "set fais 1 \n";
  vector<double> range(2);
  for(int i=0; i<nBins; i++)
    {      
      if(totalHeightPerBin[i]>0)
	{
      range[0] = 0.35;
      range[1] = 0.90;
     size_t nFinalStates = 0;
      while( (nFinalStates<finalStates[i].size())&&
	     ( fabs(contributionFromThisFinalStateToThisBin[i][0][finalStates[i].size()-1-nFinalStates] - contributionFromThisFinalStateToThisBin[i][1][finalStates[i].size()-1-nFinalStates])/2 > 0.1 * sqrt(totalHeightPerBin[i]) ) &&
	     (nFinalStates<40) )
	nFinalStates++;
      double top = 0;
      if(maxTotalHeight>0)
	top = sqrt(totalHeightPerBin[i]/maxTotalHeight)-0.1;
      // draw h, b, d
      double x1, x2, y1, y2;
      for(int k=0; k<3; k++)
	{
	  vector<double> rgb(3);
	  if(k==0)
	    rgb[1] = 1.; // green
	  if(k==1)
	    rgb[0] = 1.; // red
	  if(k==2)
	    rgb = vector<double>(3,0.5); // grey
	  pawCommand += setShadedLevels(nFinalStates, rgb, range);
	  double totalSoFar = 0;
	  for(size_t j=0; j<finalStates[i].size()-nFinalStates; j++)
	    totalSoFar += contributionFromThisFinalStateToThisBin[i][k][j]/totalHeightPerBin[i];
	  pawCommand += "set bord 1 \n";
	  int color = 1;
	  if(k==0)
	    color = 3;
	  if(k==1)
	    color = 2;
	  pawCommand += "set plci "+Math::ftoa(color)+" \n";
	  pawCommand += "set faci 0 \n";
	  x1 = (i+(k/6.))/nBins;
	  x2 = (i+((k+1)/6.))/nBins;
	  y1 = 0;
	  y2 = totalSoFar*top;
	  pawCommand += "box "+Math::ftoa(x1)+" "+Math::ftoa(x2)+" "+Math::ftoa(y1)+" "+Math::ftoa(y2)+" \n";
	  pawCommand += "set bord 0 \n";	  
	  for(size_t jj=finalStates[i].size()-nFinalStates; jj<finalStates[i].size(); jj++)
	      {
		int j = jj-(finalStates[i].size()-nFinalStates);
		pawCommand += "set faci "+Math::ftoa(20+nFinalStates-j)+" \n";
		 x1 = (i+(k/6.))/nBins;
		 x2 = (i+((k+1)/6.))/nBins;
		 y1 = totalSoFar*top;
		   totalSoFar += contributionFromThisFinalStateToThisBin[i][k][jj]/totalHeightPerBin[i];
		 y2 = totalSoFar*top;
		pawCommand += "box "+Math::ftoa(x1)+" "+Math::ftoa(x2)+" "+Math::ftoa(y1)+" "+Math::ftoa(y2)+" \n";
	      }
	  if(k==0)
	    pawCommand += "set txci 3 \n";
	  if(k==1)
	    pawCommand += "set txci 2 \n";
	  if(k==2)
	    pawCommand += "set txci 1 \n";
	  if(k==2)
	    pawCommand += 
	      "text "+Math::ftoa((x1+x2)/2)+" "+Math::ftoa(y2)+" "+
	      Math::ftoa((int)accumulate(contributionFromThisFinalStateToThisBin[i][2].begin(), contributionFromThisFinalStateToThisBin[i][2].end(), 0.))+
	      " 0.15 0 r \n";     
	}
       pawCommand += "set plci 0 \n";
      pawCommand += 
	"line "+Math::ftoa((x1+x2)/2)+" "+
	Math::ftoa(y2-sqrt(accumulate(contributionFromThisFinalStateToThisBin[i][2].begin(), contributionFromThisFinalStateToThisBin[i][2].end(), 0.))/totalHeightPerBin[i]*top)+" "+
	Math::ftoa((x1+x2)/2)+" "+Math::ftoa(y2)+" \n";
      pawCommand += "set plci 1 \n";
      pawCommand += 
	"line "+Math::ftoa((x1+x2)/2)+" "+
	Math::ftoa(y2)+" "+Math::ftoa((x1+x2)/2)+" "+
	Math::ftoa(y2+sqrt(accumulate(contributionFromThisFinalStateToThisBin[i][2].begin(), contributionFromThisFinalStateToThisBin[i][2].end(), 0.))/totalHeightPerBin[i]*top)+" \n";
      vector<double> rgb = vector<double>(3); 
      rgb[2] = 1; // blue
      pawCommand += setShadedLevels(nFinalStates, rgb, range);
      double totalSoFar = 0;
      for(size_t j=0; j<finalStates[i].size()-nFinalStates; j++)
	totalSoFar += 
	  (contributionFromThisFinalStateToThisBin[i][0][j] + contributionFromThisFinalStateToThisBin[i][1][j]) / 
	  (2. * totalHeightPerBin[i]);
    for(size_t jj=finalStates[i].size()-nFinalStates; jj<finalStates[i].size(); jj++)
	{
	  int j = jj-(finalStates[i].size()-nFinalStates);
	  double y = totalSoFar*top+.02;
	  pawCommand += "set txci "+Math::ftoa(20+nFinalStates-j)+" \n";
	  pawCommand += "text "+Math::ftoa((i+0.5)/nBins)+" "+Math::ftoa(y)+" '"+FinalState(finalStates[i][jj]).getPawString()+"' "+Math::ftoa(0.8/nBins)+" 0 l \n";
	  totalSoFar += (contributionFromThisFinalStateToThisBin[i][0][jj] + contributionFromThisFinalStateToThisBin[i][1][jj])/(2*totalHeightPerBin[i]);
	}

	}
    }
  pawCommand += "set txci 1 \n";
  pawCommand += "jtext 50 -5 'D' 0.4 0 c \n";
  pawCommand += "jtext -3 100 '\"F# events' 0.35 90 r \n";
 
  addHeaderAndFooter(colliderRun, experiment,"",likelihoodRatio,expectedEvidence);
  return;
}



void GraphicalOutput::addSummary(map<string,double> logLikelihoods)
{
  /*
    //system(("/bin/sh -c '. /usr/local/etc/setups.sh 2>&1; setup d0paw 2>&1; setup paw 2>&1; setup pawX11 2>&1'"));
    string pawCommandFilename = GlobalVariables::localDir+"tmp/pawcommand_"+GlobalVariables::id;
    ofstream fpaw(pawCommandFilename.c_str());
    //string executionCommand = "/bin/sh -c '. /usr/local/etc/setups.sh 2>&1; setup cern 2>&1; cd "+GlobalVariables::localDir+"; paw < "+ pawCommandFilename+"; ' 2>&1";
     //string executionCommand = "/bin/sh -c '. /D0/ups/etc/setups.sh 2>&1; setup cern 2>&1; cd "+GlobalVariables::localDir+"; paw < "+ pawCommandFilename+"; ' 2>&1";
     string executionCommand = "cd "+GlobalVariables::localDir+"; paw < "+ pawCommandFilename+"; 2>&1";
     //string executionCommand = "/bin/sh -c 'export HOME="+localDir+"; cd "+localDir+"; "+localDir+"paw < "+pawCommandFilename+"' 2>&1 ";
    if(RCPParameters::debugLevel<1)
    executionCommand = executionCommand + " > /dev/null";
    */
  //command = command + "fort/file 66 "+localDir+"requests/plots/"+id+".ps \n";
  //command = command + "meta 66 -111 \n";
  // make title page
  string command = "";
  command = command + "zone 1 1\n";
  command = command + "opt ntic\n null 0 1 0 1 AB\n opt tic\n";
  command = command + "gr/set txci 1\n";
  size_t ipos;
  string requestorName = RCPParameters::requestorInfo[0];
  while((ipos = requestorName.find("'")) != string::npos)
    {
      requestorName.erase(ipos,1);
      requestorName.insert(ipos,"");
    }
  string requestorInstitution = RCPParameters::requestorInfo[1];
  while((ipos = requestorInstitution.find("'"))!= string::npos)
    {
      requestorInstitution.erase(ipos,1);
      requestorInstitution.insert(ipos,"");
    }

  command = command + "jtext 50 70 '"+requestorName+"' 0.6 0 c \n";
  command = command + "jtext 50 63 '"+requestorInstitution+"' 0.5 0 c \n";
  command = command + "gr/set txci 4\n";
  command = command + "jtext 50 45 'Quaero Request \"f#"+GlobalVariables::id+"' 0.4 0 c \n";
  command = command + "gr/set txci 1\n";
  command = command + "jtext 35 35 'Experiment' 0.4 0 c \n";
  command = command + "jtext 65 35 '[S] log?10! L' 0.4 0 c \n";
  command = command + "jline 25 32 75 32 \n";
  string ypos = "";
  double summedLogLikelihood = 0.;
  int k=0;
  for(map<string,double>::iterator i=logLikelihoods.begin(); i != logLikelihoods.end(); i++)
    {
      ypos = Math::ftoa((int)(28-5*k));
      string experiment = i->first;
      double logL = max(-100.,i->second);
      command = command + "jtext 35 "+ypos+" '"+experiment+"' 0.4 0 c \n";
      command = command + "jtext 70 "+ypos+" '"+Math::ftoa(Math::toleranceRound(logL,0.1))+"' 0.4 0 r \n";
      summedLogLikelihood += logL;
      k++;
    }
  ypos = Math::ftoa((int)(30-5*logLikelihoods.size()));
  command = command + "jline 25 "+ypos+" 75 "+ypos+" \n";
  ypos = Math::ftoa((int)(30-5*logLikelihoods.size())-5);
  command = command + "jtext 35 "+ypos+" 'Combined' 0.4 0 c \n";
  command = command + "jtext 70 "+ypos+" '"+Math::ftoa(((int)(10*summedLogLikelihood))/10.)+"' 0.4 0 r \n";
  command = command + "gr/set txci 4\n";
  command = command + "gr/set txci 1\n";
  //command = command + "jtext 80 4 'Bruce Knuteson' 0.2 0 c \n";
  //command = command + "jtext 80 2 'knuteson\"U#mit.edu' 0.2 0 c \n";

  /*
  //command = command + "close 66 \n";
   fpaw << command << endl; fpaw.close();
  system(executionCommand.c_str());
  system(("rm "+pawCommandFilename).c_str());
  //system(("echo 'rm "+localDir+"requests/plots/"+id+".ps' | at now + 1 day").c_str());
  */
  
  pawCommand = command + pawCommand;
  
}


void GraphicalOutput::showModelParameterMeasurement(vector<double> xValue, vector<double> xlogLikelihood, vector<double> xlogLikelihoodError)
{
  string id=GlobalVariables::id;
  vector<string> requestorInfo = RCPParameters::requestorInfo;
  string localDir=GlobalVariables::localDir;
  if(xlogLikelihoodError.empty())
    xlogLikelihoodError = vector<double>(xlogLikelihood.size(),0);

  /*
  string pawCommandFilename = localDir+"tmp/pawcommand_"+id;
  ofstream fpaw(pawCommandFilename.c_str());
  string executionCommand = "/bin/sh -c '. /usr/local/etc/setups.sh 2>&1; setup cern 2>&1; cd "+localDir+"; paw < "+ pawCommandFilename+"; ' 2>&1";
  //string executionCommand = "/bin/sh -c 'export HOME="+localDir+"; cd "+localDir+"; "+localDir+"paw < "+pawCommandFilename+"' 2>&1 ";
  if(debugLevel<1)
    executionCommand = executionCommand + " > /dev/null";
  string command = "1 \n";
  */
  string command = "";
  /*
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
  command = command + "set xval .6 \n";
  command = command + "set xlab 2.1 \n";
  command = command + "set xmgl 3.0 \n";
  command = command + "set pass 3 \n";
  command = command + "set cshi .03 \n";
  command = command + "set csiz 0.28 \n";
  command = command + "set pwid 1. \n";
  command = command + "igset lwid 2. \n";
  command = command + "igset mtyp 20 \n";
  command = command + "igset mscf .2 \n";
  command = command + "DEFAULTS -AUTO \n";
  command = command + "opt nsta \n";
  command = command + "alias/create jtext 'exec percent#percent_text' \n";
  command = command + "alias/create jline 'exec percent#percent_line' \n";
  //command = command + "fort/file 66 "+localDir+"requests/plots/"+id+"_.ps \n";
  //command = command + "meta 66 -111 \n";
  */
  // make title page
  command = command + "zone 1 1\n";
  command = command + "opt ntic\n null 0 1 0 1 AB\n opt tic\n";
  command = command + "gr/set txci 1\n";
  command = command + "jtext 50 70 '"+requestorInfo[0]+"' 0.6 0 c \n";
  command = command + "jtext 50 63 '"+requestorInfo[1]+"' 0.5 0 c \n";
  command = command + "gr/set txci 4\n";
  command = command + "jtext 50 45 'Quaero Request \"f#"+id+"' 0.4 0 c \n";

  /*
  double xEstimate = -abc[1]/(2.*abc[0]);
  double yxEstimate = abc[0]*xEstimate*xEstimate+abc[1]*xEstimate+abc[2];
  double xEstimateError = (-abc[1]+sqrt(abc[1]*abc[1]-4*abc[0]*(abc[2]-(yxEstimate-.22))))/(2.*abc[0]) - xEstimate;
  */
  
  command = command + "gr/set txci 1\n";
  //command = command + "jtext 50 35 'x = "+Math::ftoa(Math::nice(xEstimate))+"+-"+Math::ftoa(Math::nice(xEstimateError))+"' 0.4 0 c \n";

  vector<double> sortedxValue = xValue; sort(sortedxValue.begin(),sortedxValue.end());
  vector<double> sortedyValue = xlogLikelihood; sort(sortedyValue.begin(),sortedyValue.end());

  string strxValue = "", stryValue="", stryError="";
  for(size_t i=0; i<xValue.size(); i++)
    strxValue = strxValue + Math::ftoa(xValue[i]) + " ";
  for(size_t i=0; i<xlogLikelihood.size(); i++)
    stryValue = stryValue + Math::ftoa(xlogLikelihood[i]) + " ";
  for(size_t i=0; i<xlogLikelihood.size(); i++)
    stryError = stryError + Math::ftoa(xlogLikelihoodError[i]) + " ";
  
  command = command + "v/cr x("+Math::ftoa(xValue.size())+") r "+strxValue+" \n";
  command = command + "v/cr y("+Math::ftoa(xValue.size())+") r "+stryValue+" \n";
  command = command + "v/cr e("+Math::ftoa(xValue.size())+") r "+stryError+" \n";
  double x0 = sortedxValue[0];
  double x1 = sortedxValue[sortedxValue.size()-1];
  double xrange = x1-x0+(x0==x1)*(.1*x0+(x0==0));
  int nScans = xValue.size();
  double x0edge = x0-xrange/(Math::max(1,nScans-1))/2.;
  double x1edge = x1+xrange/(Math::max(1,nScans-1))/2.;
  double y0 = sortedyValue[0];
  double y1 = sortedyValue[sortedyValue.size()-1];
  double yrange = y1-y0+(y0==y1)*(.1*y0+(y0==0));
  double y0edge = y0-yrange*.1;
  double y1edge = y1+yrange*.1;
  //command = command + "2dh 101 '' 100 "+Math::ftoa(x0edge)+" "+Math::ftoa(x1edge)+" "+Math::ftoa(y0edge)+" "+Math::ftoa(y1edge)+" \n";
  command = command + "1dh 101 '' "+Math::ftoa(nScans)+" "+Math::ftoa(x0edge)+" "+Math::ftoa(x1edge)+" \n";
  command = command + "set mtyp 8 \n";
  command = command + "set mscf .8 \n";
  command = command + "set pmci 1 \n";
  command = command + "set hcol 1 \n";
  command = command + "h/put/cont 101 y \n";
  command = command + "h/put/err 101 e \n";
  command = command + "min 101 "+Math::ftoa(y0edge)+" \n";
  command = command + "max 101 "+Math::ftoa(y1edge)+" \n";
  command = command + "set xval .2 \n";
  command = command + "h/pl 101 pce \n";
  command = command + "jtext 50 -8 'x' 0.5 0 c \n";
  command = command + "jtext -11 50 'log?10! L' 0.5 90 c \n";
  double maxx = -1.e8;
  for(size_t i=0; i<xValue.size(); i++)
    if(xlogLikelihood[i]==sortedyValue[sortedyValue.size()-1])
      maxx = xValue[i];
  vector<double> standardDeviation(4);
  for(size_t i=0; i<standardDeviation.size(); i++)
    standardDeviation[i] = abs(log10(exp(-pow((float)i,(float)2)/2.)));
  command = command + "line "+Math::ftoa(Math::max(x0edge,maxx-.15*xrange))+" "+Math::ftoa(y1)+" "+Math::ftoa(Math::min(x1edge,maxx+.15*xrange))+" "+Math::ftoa(y1)+" \n";
  command = command + "set dmod 2 \n";
  for(size_t i=1; i<standardDeviation.size(); i++)
    if(y1-standardDeviation[i]>y0edge)
      {
	command = command + "line "+Math::ftoa(x0edge)+" "+Math::ftoa(y1-standardDeviation[i])+" "+Math::ftoa(x1edge)+" "+Math::ftoa(y1-standardDeviation[i])+" \n";
	command = command + "jtext 3 "+Math::ftoa((y1-standardDeviation[i]-y0edge)/(y1edge-y0edge)*100.+1)+" '"+Math::ftoa(i)+"[s]' 0.2 0 l \n";
      }
  command = command + "set dmod 1 \n";
  if((y1edge>0.)&&(y0edge<0.))
    {
      command = command + "set plci 2 \n";
      command = command + "line "+Math::ftoa(x0edge)+" "+Math::ftoa(0)+" "+Math::ftoa(x1edge)+" "+Math::ftoa(0)+" \n";
      command = command + "set plci 1 \n";
    }

  //command = command + "close 66 \n";
  /*
  fpaw << command << endl; fpaw.close();
  system(executionCommand.c_str());
  system(("rm "+pawCommandFilename).c_str());
  */

  pawCommand = command + pawCommand;

}
	    

/*****  Addition operator  *****/

GraphicalOutput GraphicalOutput::operator+(const GraphicalOutput& rhs) const
{
  return(GraphicalOutput(this->pawCommand+rhs.pawCommand));
}

