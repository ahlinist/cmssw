/***************************************************************
This file (quaero.cc) contains the guts of the Quaero algorithm.
This algorithm is conveniently thought of in terms of routines, 
rather than classes, and is implemented as such.

Bruce Knuteson 2003
***************************************************************/


/*****  Include Files  ******/

#include <fstream>
#include <strstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cassert>
#include <cmath>
#include <algorithm>
#include <numeric>
//#include "gsl/gsl_math.h"
//#include "gsl/gsl_monte.h"
//#include "gsl/gsl_monte_miser.h"
#include "Validation/VstMathUtils/interface/Math.hh"
#include "Validation/VstQuaeroUtils/interface/QuaeroEvent.hh"
#include "Validation/VstQuaeroUtils/interface/QuaeroItemFile.hh"
#include "Validation/VstQuaeroUtils/interface/FinalState.hh"
#include "Validation/VstQuaeroUtils/interface/PartitionRule.hh"
#include "Validation/VstQuaeroUtils/interface/Kolmogorov.hh"
#include "Validation/VstQuaeroUtils/interface/RelevantVariables.hh"
#include "Validation/VstQuaeroUtils/interface/Refine.hh"
#include "Validation/VstQuaero/interface/QuaeroSysEvent.hh"
#include "Validation/VstQuaero/interface/GraphicalOutput.hh"
#include "Validation/VstQuaero/interface/Discriminator.hh"
#include "Validation/VstQuaero/interface/RCPParameters.hh"
#include "Validation/VstQuaero/interface/QuaeroIO.hh"
#include "Validation/VstQuaero/interface/quaero.hh"
using namespace std;


/*  Print header in the log file, with the corresponding time */

void printHeaderAndDate(string header)
{
  int numberOfSpacesToAdd = 75 - header.length();
  if(numberOfSpacesToAdd<0)
    numberOfSpacesToAdd = 0;

  cout << header << string(numberOfSpacesToAdd,' ') <<flush;
  system("date");
  return;
}



/*  This class implements a figure of merit that is maximized within
    the constrains of a specified allowed execution time for the 
    Quaero algorithm.  */


class TimeCostFigureOfMerit: public Math::FunctionObject
{
public:

  /**  Constructors  **/

  // Default
  TimeCostFigureOfMerit() { targetTime = 0;nColliderRuns =  nColliderExperiments = nMadEventRuns = 0; setLowerLimits(); };

  // Nontrivial constructor
  TimeCostFigureOfMerit(double _targetTime): targetTime(_targetTime) { nColliderRuns = nColliderExperiments = nMadEventRuns = 0; setLowerLimits(); };

  /**  Public Methods  **/

  // Determine global chi^2 with the fudge factors x
  double operator()(const std::vector<double>& _x)
  {
    assert(_x.size()==5);
    vector<double> x = _x;
    double figureOfMerit = 0;

    // Impose a Lagrange multiplier to keep x[i] > lowerLimits[i]

    for(size_t i=0; i<x.size(); i++)
      if(x[i]<lowerLimits[i])
	figureOfMerit += (x[i]-lowerLimits[i])/lowerLimits[i]*1e10;  
    if(figureOfMerit<0)
      return(-figureOfMerit);

    /* Varying weight is given to increasing the four parameters:
       nSignalEvents, nBkgEvents, nFewKdeTrials, and nSystematicShifts.
       How much increasing each of these numbers helps the overall analysis
       is specified by the number in the exponent below. */

    figureOfMerit = 1;
    figureOfMerit *= pow(x[0]/1e6,0.45); // nSignalEvents**()
    figureOfMerit *= pow(x[1]/1e5,0.20); // nTurboSimTableLines**()
    figureOfMerit *= pow(x[2]/1e6,1.00); // nBkgEvents**()
    figureOfMerit *= pow(x[3]/1e1,0.07); // nFewKdeTrials**()
    figureOfMerit *= pow(x[4]/1e3,0.05); // nSystematicShifts**()
    figureOfMerit -= max(0.,timeCost(x[0],x[1],x[2],x[3],x[4])-targetTime);
    // cout << "figureOfMerit = " << figureOfMerit << "    ;   "; print(x);
    return(-figureOfMerit);
  }

  /* Calculate the time cost of running the Quaero algorithm with the specified number of
     nSignalEvents, nBkgEvents, nFewKdeTrials, and nSystematicShifts */

  double timeCost(double nSignalEvents, double nTurboSimTableLines, double nBkgEvents, double nFewKdeTrials, double nSystematicShifts)
  {
    assert(nSignalEvents>=1);
    assert(nTurboSimTableLines>=1);
    assert(nBkgEvents>=1);
    assert(nFewKdeTrials>=1);
    assert(nSystematicShifts>=1);
    
    double timeCostToGenerateSignalEvents = 0.010 * nSignalEvents * nColliderRuns; // 10 milliseconds per event.  Need to generate nSignalEvents for each of nColliderRuns
    timeCostToGenerateSignalEvents += 30 * nMadEventRuns; // additional time cost of 30 seconds for each MadEvent run
    double timeCostToReadInTurboSimTables = 30.0 * (nTurboSimTableLines/1e5) * nColliderRuns; // 30 seconds for each 10^5 lines in the TurboSim lookup table
    double timeCostToEstimateDensities = 0.005 * (nBkgEvents+nSignalEvents) * nColliderExperiments * nFewKdeTrials;
    double timeCostToIntegrateOverSystematicShifts = 0.001 * (nBkgEvents+nSignalEvents) * nColliderExperiments * nSystematicShifts;
    double totalTimeCost = timeCostToGenerateSignalEvents + timeCostToReadInTurboSimTables + timeCostToEstimateDensities + timeCostToIntegrateOverSystematicShifts;
    return(totalTimeCost);
  }

  /*  Set lower limits for these parameters */
  void setLowerLimits(vector<double> _lowerLimits = vector<double>(5)) { assert(_lowerLimits.size()==5); lowerLimits = _lowerLimits; }
  void setAdditionalBoundaryConditions(map<string, vector<string> > colliderExperiments,
				       map<string, string> signalFormat)
  {
    nColliderRuns = nColliderExperiments = nMadEventRuns = 0;
    for(map<string, vector<string> >::iterator i=colliderExperiments.begin(); i!=colliderExperiments.end(); i++)
      nColliderExperiments += i->second.size();
    for(map<string, string>::iterator i=signalFormat.begin(); i!=signalFormat.end(); i++)
      {
	nColliderRuns++;
	if(i->first=="hera")
	  nMadEventRuns += 3;
	else if(i->first=="lep2")
	  nMadEventRuns += 8;
	else
	  nMadEventRuns += 1;
      }
  };

private:
  double targetTime; // units are seconds
  vector<double> lowerLimits;
  int nColliderRuns, nColliderExperiments, nMadEventRuns;
};


/*  Determine Quaero algorithm parameters to give the "best" result 
    while staying within the user-allotted time.  A lot like chess.  */

double getAlgorithmParameters(double targetTime, int& nSignalEvents, int& nTurboSimTableLines, int& nBkgEvents, int& nFewKdeTrials, int& nSystematicShifts)
{
  // Initialize starting conditions
  TimeCostFigureOfMerit timeCostFigureOfMerit(targetTime);
  vector<double> x(5);
  x[0] = 1e4;
  x[1] = 1e5; 
  x[2] = 1e5;
  x[3] = 1;
  x[4] = 3; //bktemp 10
  timeCostFigureOfMerit.setLowerLimits(x);
  timeCostFigureOfMerit.setAdditionalBoundaryConditions(RCPParameters::colliderExperiments,RCPParameters::signalFormat);
  double tol = 1e-4;

  // Perform the maximization of the figure of merit
  Math::minimize(x,&timeCostFigureOfMerit,x,tol);
  nSignalEvents = (int)x[0];
  nTurboSimTableLines = (int)x[1];
  nBkgEvents = (int)x[2];
  nFewKdeTrials =(int)x[3];
  nSystematicShifts = (int)x[4];

  // Recompute the achievable time
  double achievableTime = timeCostFigureOfMerit.timeCost(nSignalEvents, nTurboSimTableLines, nBkgEvents, nFewKdeTrials, nSystematicShifts);

  // Return the achievable time and values of the best algorithm parameters, passed in as arguments
  return(achievableTime);
}


/*  Take the events that have been provided to Quaero, and run them through 
    the detector simulation appropriate for each experiment.
    After this routine, a text file with simulated events in Quaero 4-vector 
    form exists at getFilename("metaSig",colliderRun,experiment). */

void simulateDetectors()
{
  // simulate the detectors
  cout << "Simulating detectors" << endl;
  for(map<string,vector<string> >::iterator iColliderRun=RCPParameters::colliderExperiments.begin();  // loop over collider experiments
      iColliderRun!=RCPParameters::colliderExperiments.end(); 
      iColliderRun++)
    {
      string colliderRun = iColliderRun->first;  // colliderRun = tev1, tev2, hera, lep2, etc.
      for(size_t iExperiment=0; iExperiment<iColliderRun->second.size(); iExperiment++)
	{
	  string experiment = iColliderRun->second[iExperiment];  // experiment = cdf, d0, l3, h1, etc.
	  if(ifstream((QuaeroIO::getFilename("simulate_executable",colliderRun,experiment)).c_str()))
	    {
	      // simulate the signal file
	      printHeaderAndDate("  "+colliderRun+" "+experiment);
	      string actualGeneratedLuminosityString = "";
	      for(size_t i=0; i<RCPParameters::actualGeneratedLuminosity[colliderRun].size(); i++)
		actualGeneratedLuminosityString += Math::ftoa(RCPParameters::actualGeneratedLuminosity[colliderRun][i])+"pb^-1 ";

	      system((QuaeroIO::getFilename("simulate_executable",colliderRun,experiment)+" "+          // simulate
		      QuaeroIO::getFilename("hepg",colliderRun)+" "+                                    // <hepg file>
		      actualGeneratedLuminosityString+                                                  // <lumi(pb^-1)>
		      QuaeroIO::getFilename("metaSig",colliderRun,experiment)+" "+                      // <output text file>
		      QuaeroIO::getFilename("simulate_directory",colliderRun,experiment)+" "+           // <simulate working directory>
		      Math::ftoa(RCPParameters::nTurboSimTableLines)                                    // <number of TurboSim table lines to use
		      ).c_str());     
	      cout << endl;
	    }  
	}
      if(RCPParameters::debugLevel<=3)
	system(("rm "+QuaeroIO::getFilename("hepg",colliderRun)+"* >& /dev/null").c_str()); // remove the hepg file to save disk space
    }
  if((RCPParameters::signalFileStorage.id==GlobalVariables::id)&&
     (RCPParameters::signalFileStorage.scaleFactor==1))
    {
      // Store these signal files in signalFileStorage for possible use by future Quaero requests
      system(("cp -r "+QuaeroIO::getFilename("signalfiles")+" "+QuaeroIO::getFilename("signalFileStorage")).c_str());
    }
  return;
}


vector<QuaeroEvent> getDataEvents(string colliderRun, string experiment, string fs, const HintSpec& hintSpec)
{
  vector<QuaeroEvent> dataEvents = QuaeroItemFile<QuaeroEvent>(QuaeroIO::getFilename("data",colliderRun,experiment,fs)).loadEvents();
  if(hintSpec.active)
    {
      vector<QuaeroEvent> dataEventsPassingSumPtCut;
      for(size_t i=0; i<dataEvents.size(); i++)
	if(dataEvents[i].sumPt() > hintSpec.sumPtCut)
	  dataEventsPassingSumPtCut.push_back(dataEvents[i]);
      dataEvents = dataEventsPassingSumPtCut;
    }
 return(dataEvents);
}


void shiftRefineAndPartition(int iSystematicShift, vector<double> systematicShifts = vector<double>(0))
{
  if(iSystematicShift<=0)
    {
      printHeaderAndDate(GlobalVariables::chatterTab.tab(+1)+"Shift / refine / partition  (iSys = "+Math::ftoa(iSystematicShift)+")");
      if(!systematicShifts.empty())
	print(systematicShifts);
    }

  bool reallyShift = (iSystematicShift>=2);                      // only impose a non-zero shift if reallyShift
  if(reallyShift)
    GlobalVariables::systematicShift.reShift(systematicShifts);  // set the systematic shift in GlobalVariables::systematicShift
  int whichThird = 2;
  if((iSystematicShift==-3)||(iSystematicShift==-2))
    whichThird = 0;
  if((iSystematicShift==-1))
    whichThird = 1;

  for(map<string,vector<string> >::iterator iColliderRun=RCPParameters::colliderExperiments.begin();  // loop over collider runs
      iColliderRun!=RCPParameters::colliderExperiments.end();
      iColliderRun++)
    for(size_t iExperiment=0; iExperiment<iColliderRun->second.size(); iExperiment++)  // loop over experiments in this collider run
      {
	string colliderRun = iColliderRun->first;                // tev1, tev2, lep2, hera, etc.
	string experiment = iColliderRun->second[iExperiment];   // cdf, d0, l3, h1, etc.
	string colliderRunExperiment = colliderRun + " " + experiment;
	if(iSystematicShift<=0) 
	  cout << GlobalVariables::chatterTab.tab() 
	       << colliderRunExperiment 
	       << string(max(1,(int)(20-colliderRunExperiment.length())),' ') << flush;
	string partitionRuleName = colliderRun+"-Quaero";
	PartitionRule partitionRule(partitionRuleName);       // rule for partitioning the events into exclusive final states
	Refine refine(colliderRun, experiment, partitionRuleName, 10, RCPParameters::hintSpec);
	vector<double> weights;

	for(int iType=0; iType<=1; iType++)
	  {
	    string type= ((iType==0) ? "sig" : "bkg"); assert((type=="sig")||(type=="bkg"));
	    if(iSystematicShift<=0) cout << type << "  " << flush;
	    if((iSystematicShift==-3)&&(type=="bkg"))
	      {
		ifstream fbkgListing(QuaeroIO::getFilename("bkgListing",colliderRun,experiment).c_str());
		string s; double x, wt=0, wtSqd=0;
		while(fbkgListing >> s)
		  {
		    fbkgListing >> x; wt += x;
		    fbkgListing >> s >> x; wtSqd += x;
		    fbkgListing >> s; assert(s==";");
		  }
		fbkgListing.close();
		double epsWt = (wtSqd/wt);
		RCPParameters::epsilonWt.store(colliderRun, experiment, epsWt);   // store this result in our Run Control Parameters namespace
		cout << " (epsWt = " << Math::nice(RCPParameters::epsilonWt.get(colliderRun, experiment)) << ")  " << flush;
		continue;
	      }

	    string meta = "";
	    if(type=="sig")
	      meta = QuaeroIO::getFilename("metaSig",colliderRun,experiment);    // "metaSig" is the Quaero 4-vector output of the detector simulation ; 
	    else                                                                 // (type=="bkg")
	      if(iSystematicShift>=1)
		meta = QuaeroIO::getFilename("relevantBkg",colliderRun,experiment);  // use only the relevant background if we have already determined what the relevant background is
	      else
		meta = QuaeroIO::getFilename("metaBkg",colliderRun,experiment);  // "metaBkg" is the Quaero 4-vector output of the detector simulation for the background; this file is provided by each experiment

	    QuaeroSysEvent event;
	    QuaeroEvent event1;
	    int k=0;
	    double weightMultiplicativeFactor = 3;

	    if((type=="sig")&&
	       (atoi(RCPParameters::signalFileStorage.id.c_str())>0))
	      weightMultiplicativeFactor *= RCPParameters::signalFileStorage.scaleFactor;

	    QuaeroItemFile<QuaeroSysEvent> ef(meta); // open Quaero file
	    std::map<FinalState,std::strstream*> partitioning;
	    std::map<FinalState,pair<double,double> > listing;
	    string partitionFilename = QuaeroIO::getFilename(type,colliderRun,experiment);
	    system(("rm "+partitionFilename+"_*.txt 2> /dev/null").c_str());
	    while(ef.nextEvent(event))
	      {
		vector<QuaeroSysObject> o = event.getObjects();
		vector<QuaeroRecoObject> o1;
		for(size_t i=0; i<o.size(); i++)
		  o1.push_back(o[i]);
		event1 = QuaeroEvent(event.getEventType(), event.getRunNumber(),
				     event.getWeight()*weightMultiplicativeFactor, // every event now counts thrice, so multiply the weight by 3
				     o1, event.getCollisionType(), event.getRootS(), event.getZVtx(), event.getGeneratedSumPt());
		if((((k%3)==whichThird)  // use every third event
		    ||((type=="bkg")&&(iSystematicShift>=1))) // use all events
		   &&(event1.getWeight()>0)
		   &&(refine.satisfiesCriteriaQ(event1)))
		  {
		    if(type=="bkg")
		      weights.push_back(event1.getWeight());  // create a vector containing all bkg weights
		    FinalState fs = partitionRule.getFinalState(event1);
		    if(partitioning.find(fs)==partitioning.end())
		      partitioning[fs] = new strstream;
		    if(listing.find(fs)==listing.end())
		      listing[fs] = pair<double,double>(0,0);
		    (*partitioning[fs]) << event1.print() << endl;
		    listing[fs].first += event1.getWeight();
		    listing[fs].second += event1.getWeight()*event1.getWeight();
		  }
		k++;
		if(iSystematicShift<=0)
		  if(k%10000==0)
		    cout << k/10000 << " " << flush;
	      }
	    if((iSystematicShift==-3)&&(type=="sig"))
	      {
		ofstream fsigListing(QuaeroIO::getFilename("sigListing",colliderRun,experiment).c_str());
		for(map<FinalState,pair<double,double> >::iterator l=listing.begin(); l!=listing.end(); l++)
		  if(l->first.getTextString()!="")
		    fsigListing << l->first.getTextString() << " " << l->second.first << "  " << l->second.second << " ;" << endl;
		fsigListing.close();
	      }
	    for(map<FinalState,strstream*>::iterator i=partitioning.begin(); i!=partitioning.end(); i++)
	      {
		if(i->first.getTextString()!="")
		  {
		    ofstream fout((partitionFilename+"_"+i->first.getTextString()+".txt").c_str());
		    string blah;
		    while(getline(*(i->second),blah))
		      fout << (string)blah << endl;
		    delete i->second;
		    i->second = NULL;
		  }
	      }
	  }
	if(iSystematicShift<=0) cout << endl;
      }
  if(iSystematicShift<=0) GlobalVariables::chatterTab.tab(-1);
  GlobalVariables::systematicShift.setDefault();
  return;
}


// Return the list of final states that need to be considered for this experiment
// Output:  a vector of strings labeling these final states

vector<string> getListOfFinalStatesForThisExperiment(string colliderRun, string experiment)
{
  // figure out what final states we need to consider
  string bkgBaseFilename = QuaeroIO::getFilename("bkg",colliderRun,experiment);               // bkgBaseFilename has the form /blah1/blah2/../blahN/bkg
  string bkgBaseDirectory = bkgBaseFilename.substr(0,bkgBaseFilename.length()-3);             // strip off ending "bkg"
  vector<string> finalstates_bkg = Math::getFilesInDirectory(bkgBaseDirectory,"bkg_*.txt");   // partitioned events have filenames of the form /blah1/blah2/../blahN/bkg_<finalState>.txt
  for(size_t i=0; i<finalstates_bkg.size(); i++)
    finalstates_bkg[i] = finalstates_bkg[i].substr(4,finalstates_bkg[i].length()-4-4);        // starting from "bkg_<finalState>.txt", strip off "bkg_" and ".txt"
  string sigBaseFilename = QuaeroIO::getFilename("sig",colliderRun,experiment);               // sigBaseFilename has the form /blah1/blah2/../blahN/sig
  string sigBaseDirectory = sigBaseFilename.substr(0,sigBaseFilename.length()-3);             // strip off ending "sig"
  vector<string> finalstates_sig = Math::getFilesInDirectory(sigBaseDirectory,"sig_*.txt");   // partitioned events have filenames of the form /blah1/blah2/../blahN/sig_<finalState>.txt
  for(size_t i=0; i<finalstates_sig.size(); i++)
    finalstates_sig[i] = finalstates_sig[i].substr(4,finalstates_sig[i].length()-4-4);        // starting from "sig_<finalState>.txt", strip off "sig_" and ".txt"
  vector<string> finalstates = finalstates_bkg;
  assert(!finalstates.empty());
  finalstates.insert(finalstates.end(), finalstates_sig.begin(), finalstates_sig.end());
  sort(finalstates.begin(), finalstates.end());
  vector<string>::iterator lpos = unique(finalstates.begin(), finalstates.end());
  finalstates.erase(lpos, finalstates.end());
  return(finalstates);                                                                       // finalstates = union of finalstates_sig and finalstates_bkg
}


// These global variables are needed for the C-based GNU integration routine "Miser"

// relevantVariables is a map
//     key is:  vector<string>(3) = { colliderRun, experiment, finalState }
//   value is:  RelevantVariables, a class containing the relevant variables for this colliderRun, experiment, and finalState
map<vector<string>,RelevantVariables> relevantVariables;

// discriminators is a map
//     key is:  vector<string>(3) = { colliderRun, experiment, finalState }
//   value is:  Discriminator, which contains 
//                               signal and background kernel density estimates, 
//                               a binning in the resulting discriminant, and 
//                               predictions from H and SM for each of these bins
map<vector<string>,Discriminator> discriminators;

// whichHypothesis is either "h" or "sm"
string whichHypothesis;

// lsm0 = p(D|SM,s=0), where "s=0" indicates that no systematic shifts have been imposed.
// Used to prevent underflows when exponentiating large negative log likelihoods
double lsm0;

//  Return p(Data | H, s) or p(Data | SM, s), depending on whether whichHypothesis equals "h" or "sm"
//  Inputs:
//    systematicShifts:  array of <numberOfSystematicShifts> numbers between 0 and 1
//    void* parameters is not used

double probabilityOfObservingDataGivenHypothesisAndVectorOfSystematicShifts(double* systematicShifts, size_t numberOfSystematicShifts, void* parameters) 
{
  GlobalVariables::iSystematicShift = 2;
  QuaeroIO::makeTmpDirectory(GlobalVariables::iSystematicShift);                             // make temporary directory in which to manipulate files
  vector<double> sysShifts = vector<double>(numberOfSystematicShifts);
  for(size_t i=0; i<numberOfSystematicShifts; i++)
    sysShifts[i] = systematicShifts[i];
  shiftRefineAndPartition(GlobalVariables::iSystematicShift, sysShifts);                     // shift, refine, and partition the signal, background, and data

  double hypothesisLikelihood = 0, smLikelihood = 0;
  map<string,double> likelihoodRatios_exp;                                                   // { string labeling the experiment, likelihood ratio obtained in that experiment }
  map<vector<string>,double> likelihoodRatios;                                               // { { colliderRun, experiment, finalState }, likelihood ratio obtained in that colliderRun, experiment, and final state}
  map<vector<string>,vector<vector<double> > > contributionsToD;                             // {  { colliderRun, experiment, finalState }, ... }

  string colliderRun="", experiment="";
  for(map<vector<string>,Discriminator>::iterator i=discriminators.begin(); i!=discriminators.end(); i++)
    i->second.clearBinContents();                                                            // clear predictions of H and SM from each discriminator
  GlobalVariables::chatterTab.tab(+1);
  for(map<vector<string>,RelevantVariables>::iterator i=relevantVariables.begin(); 
      i!=relevantVariables.end(); i++)                                                       // loop over all colliderRuns, experiments, and final states
    {
      vector<string> tag2 = i->first;                                                        // tag2 = { colliderRun, experiment, finalState }
      assert(tag2.size()==3);
      if((colliderRun!=tag2[0])||(experiment!=tag2[1]))                                      // on to a new colliderRun and/or experiment
	{
	  colliderRun = tag2[0];
	  experiment = tag2[1];
	  if(RCPParameters::debugLevel>=6) cout << GlobalVariables::chatterTab.tab() << colliderRun << " " << experiment << endl;
	}
      string fs = tag2[2];                                                                   // final state

      vector<QuaeroEvent> eventsUniqueToH = QuaeroItemFile<QuaeroEvent>(QuaeroIO::getFilename("sig",colliderRun,experiment,fs)).loadEvents();  
                                                                                             // events predicted by H but not predicted by SM
      vector<QuaeroEvent> eventsUniqueToSM = QuaeroItemFile<QuaeroEvent>(QuaeroIO::getFilename("bkg",colliderRun,experiment,fs)).loadEvents(RCPParameters::leaveOut[colliderRun],"onlyInclude");  
                                                                                             // events predicted by SM but not predicted by H
      vector<QuaeroEvent> commonEvents = QuaeroItemFile<QuaeroEvent>(QuaeroIO::getFilename("bkg",colliderRun,experiment,fs)).loadEvents(RCPParameters::leaveOut[colliderRun]);  
                                                                                             // events predicted by both H and SM
      vector<QuaeroEvent> dataEvents = getDataEvents(colliderRun,experiment,fs,RCPParameters::hintSpec);
                                                                                             // events observed in the data
      vector<string> distributionNames;
      vector<double> weightsCommon, weightsUniqueToH, weightsUniqueToSM, weightsData;
      vector<vector<double> > variableValuesCommon, variableValuesUniqueToH, variableValuesUniqueToSM, variableValuesData;

      /*      bool sufficientlyDifferent = i->second.compute(commonEvents, eventsUniqueToH, eventsUniqueToSM, dataEvents,         // input events
						     colliderRun, experiment, fs, distributionNames,                      // input colliderRun, experiment, final state, output distributionNames
						     weightsCommon, weightsUniqueToH, weightsUniqueToSM, weightsData,     // output vectors of event weights
						     variableValuesCommon, variableValuesUniqueToH, variableValuesUniqueToSM, variableValuesData);  // output vectors of variable values
      */
      vector<vector<double> > hVariableValues = variableValuesCommon, smVariableValues = variableValuesCommon;      
      hVariableValues.insert(hVariableValues.end(), variableValuesUniqueToH.begin(), variableValuesUniqueToH.end());     // union of common and events unique to H
      smVariableValues.insert(smVariableValues.end(), variableValuesUniqueToSM.begin(), variableValuesUniqueToSM.end()); // union of common and events unique to SM
      vector<double> hWeights = weightsCommon, smWeights = weightsCommon;
      hWeights.insert(hWeights.end(), weightsUniqueToH.begin(), weightsUniqueToH.end());                                 // union of common and events unique to H
      smWeights.insert(smWeights.end(), weightsUniqueToSM.begin(), weightsUniqueToSM.end());                             // union of common and events unique to SM

      vector<string> tag1 = vector<string>(tag2.begin(), tag2.begin()+2);                                                // tag1 = { colliderRun, experiment }
      string tag2_string = tag2[0]+" "+tag2[1]+" "+tag2[2];                                                              // e.g. "tev2 cdf 1e+1e-1pmiss"
      
      // add the prediction of H and SM to the bins in the discriminant D in the discriminator
      // return { h: { bin1, bin2, .. }, sm: { bin1, bin2, .. } }
      vector<vector<double> > hbContributionsToD = discriminators[tag1].addToPredictedD(tag2_string, hVariableValues, smVariableValues, hWeights, smWeights);
      assert(hbContributionsToD.size()==2);

      // add the observed data to the bins in the discriminant D in the discriminator
      vector<double> dContributionsToD = discriminators[tag1].addToPredictedD(tag2_string, variableValuesData);
      contributionsToD[tag2] = hbContributionsToD;
      contributionsToD[tag2].push_back(dContributionsToD);

    }
  GlobalVariables::chatterTab.tab(-1);
  
  if(RCPParameters::debugLevel>=5) cout << GlobalVariables::chatterTab.tab(+1) << "" << endl;
  for(map<vector<string>,Discriminator>::iterator i=discriminators.begin(); i!=discriminators.end(); i++)
    {
      double hL = 0, smL = 0;
      string colliderRun = i->first[0];
      string experiment = i->first[1];
      if(RCPParameters::debugLevel>=5) cout << GlobalVariables::chatterTab.tab() << colliderRun << " " << experiment << endl;
      i->second.computeLogLikelihoods(hL,smL,(RCPParameters::debugLevel>=5));  // compute the log likelihood for this final state alone
                                                                               // this will be used for making plots, but not for the final result
      hypothesisLikelihood += hL;
      smLikelihood += smL;
    }
  if(RCPParameters::debugLevel>=5) GlobalVariables::chatterTab.tab(-1);

  double ans = 0.;
  if(whichHypothesis=="sm")
    ans = pow(10.,smLikelihood-lsm0);
  else if(whichHypothesis=="h")
    ans = pow(10.,hypothesisLikelihood-lsm0);
  else
    assert(false);
  
  cout << Math::nice(ans) << "  " << flush;
  QuaeroIO::removeTmpDirectory(GlobalVariables::iSystematicShift);  // remove the temporary directory for this systematic shift to save disk space 

  return(ans);
}


//  Return p(Data | H) or p(Data | SM), depending on whether whichHypothesis equals "h" or "sm"
//  Integrate p(Data | H, s)  [or p(Data | SM)] over the systematic shifts s
//  Inputs:
//    _relevantVariables:  the relevant variables in each final state in each experiment in each collider run
//       _discriminators:  a vector of discriminators
//      _whichHypothesis:  either "h" or "sm"
//                 _lsm0:  p(Data | SM, s=0)

double probabilityOfObservingDataGivenHypothesis(map<vector<string>,RelevantVariables> & _relevantVariables, 
						 map<vector<string>,Discriminator>& _discriminators, 
						 string _whichHypothesis, double _lsm0, double& err)
{
  // Set corresponding global variables
  ::relevantVariables = _relevantVariables;
  ::discriminators = _discriminators;
  ::whichHypothesis = _whichHypothesis;
  ::lsm0 = _lsm0;

  // Make use of GNU Scientific Library (GSL) and its Miser integration routine

  const gsl_rng_type *T;  // random number generator (rng)
  gsl_rng *r;
  size_t n = GlobalVariables::systematicShift.getNumberOfSystematicErrorSources();
  gsl_monte_function p = { &probabilityOfObservingDataGivenHypothesisAndVectorOfSystematicShifts, n, 0 };  // the function to be integrated, number of arguments, and pointer (null, in this case) to a struct containing parameters 
  //  double sl[n], su[n]; 
  double *sl = new double[n];
  double *su = new double[n];
  for(size_t i=0; i<n; i++)
    {
      sl[i] = 0;  // lower bound of integration
      su[i] = 1;  // upper bound of integration
    }
  size_t calls = 2;  // start with only two calls
  gsl_rng_env_setup();  // set up gsl random number generator environment
  T = gsl_rng_default;
  r = gsl_rng_alloc(T);
  double res=0; err=0;  // res = the result, err = estimated error
  gsl_monte_miser_state *s = gsl_monte_miser_alloc(n);
  cout << GlobalVariables::chatterTab.tab(+1) 
       << "Integrating p(D|"
       << (_whichHypothesis=="h" ? "H" : "SM") 
       << ",s) over the vector of systematics shifts s"
       << endl;
  cout << GlobalVariables::chatterTab.tab();
  if(RCPParameters::nSystematicShifts>0)
    gsl_monte_miser_integrate(&p, sl, su, n, RCPParameters::nSystematicShifts, r, s, &res, &err);  // . . . estimate the answer . . .
  else
    {
      do
	{
	  calls *= 2;  // double the number of calls . . .
	  gsl_monte_miser_integrate(&p, sl, su, n, calls, r, s, &res, &err);  // . . . estimate the answer . . .
	  cout << "Miser intermediate (" << calls << ")" << endl << "  result = " << res << "  sigma = " << err << endl;
	  assert(res>=0);
	  assert(err>=0);
	}
      while((res-err<=0) || (fabs(log10(res+err)-log10(res-err))>0.2));  // . . . and repeat until the result is determined to 0.1 units of log10(probability)
    }
  cout << endl << GlobalVariables::chatterTab.tab(-1) << "= " << Math::nice(res) << " +- " << Math::nice(err) << endl;
  gsl_monte_miser_free(s);
  
  return(res);
}


// Compute the likelihood for one systematic shift
// This routine is used only to make the plots that Quaero returns, with iSystematicShift = 0

void computeLikelihoodForSystematicErrorShift(int iSystematicShift, double& hypothesisLikelihood, double& smLikelihood, 
					      map<vector<string>,RelevantVariables> & relevantVariables, 
					      map<vector<string>,Discriminator>& discriminators, 
					      map<string,map<vector<string>,GraphicalOutput> >& graphicalOutput)
{
  bool makePlots = (iSystematicShift==0);
  QuaeroIO::makeTmpDirectory(iSystematicShift);
  shiftRefineAndPartition(iSystematicShift);
  printHeaderAndDate(GlobalVariables::chatterTab.tab(+1)+"Computing likelihood");

  hypothesisLikelihood = smLikelihood = 0;
  double hL=0, smL=0;
  map<string,double> likelihoodRatios_exp;
  map<vector<string>,double> likelihoodRatios;
  map<vector<string>,vector<vector<double> > > contributionsToD;

  string colliderRun="", experiment="";
  for(map<vector<string>,Discriminator>::iterator i=discriminators.begin(); i!=discriminators.end(); i++)
    i->second.clearBinContents();
  GlobalVariables::chatterTab.tab(+1);
  for(map<vector<string>,RelevantVariables>::iterator i=relevantVariables.begin(); i!=relevantVariables.end(); i++)
    {
      vector<string> tag2 = i->first;
      assert(tag2.size()==3);
      if((colliderRun!=tag2[0])||(experiment!=tag2[1]))
	{
	  colliderRun = tag2[0];
	  experiment = tag2[1];
	  GlobalVariables::chatterTab.tab(-1);
	  cout << GlobalVariables::chatterTab.tab(+1) << colliderRun << " " << experiment << endl;
	}
      string fs = tag2[2];

      // Load events
      vector<QuaeroEvent> eventsUniqueToH = QuaeroItemFile<QuaeroEvent>(QuaeroIO::getFilename("sig",colliderRun,experiment,fs)).loadEvents();
      vector<QuaeroEvent> eventsUniqueToSM = QuaeroItemFile<QuaeroEvent>(QuaeroIO::getFilename("bkg",colliderRun,experiment,fs)).loadEvents(RCPParameters::leaveOut[colliderRun],"onlyInclude");
      vector<QuaeroEvent> commonEvents = QuaeroItemFile<QuaeroEvent>(QuaeroIO::getFilename("bkg",colliderRun,experiment,fs)).loadEvents(RCPParameters::leaveOut[colliderRun]);  
      vector<QuaeroEvent> dataEvents = getDataEvents(colliderRun,experiment,fs,RCPParameters::hintSpec);
      vector<string> distributionNames;
      vector<double> weightsCommon, weightsUniqueToH, weightsUniqueToSM, weightsData;
      vector<vector<double> > variableValuesCommon, variableValuesUniqueToH, variableValuesUniqueToSM, variableValuesData;
      
      // Compute the relevant variables
      /*      bool sufficientlyDifferent = i->second.compute(commonEvents, eventsUniqueToH, eventsUniqueToSM, dataEvents,     // input events
						     colliderRun, experiment, fs, distributionNames,                  // input colliderRun, experiment, fs; output names of relevant distributions
						     weightsCommon, weightsUniqueToH, weightsUniqueToSM, weightsData, // output event weights
						     variableValuesCommon, variableValuesUniqueToH, variableValuesUniqueToSM, variableValuesData); // output variable values
      */	 
      vector<vector<double> > hVariableValues = variableValuesCommon, smVariableValues = variableValuesCommon;      
      hVariableValues.insert(hVariableValues.end(), variableValuesUniqueToH.begin(), variableValuesUniqueToH.end());
      smVariableValues.insert(smVariableValues.end(), variableValuesUniqueToSM.begin(), variableValuesUniqueToSM.end());
      vector<double> hWeights = weightsCommon, smWeights = weightsCommon;
      hWeights.insert(hWeights.end(), weightsUniqueToH.begin(), weightsUniqueToH.end());
      smWeights.insert(smWeights.end(), weightsUniqueToSM.begin(), weightsUniqueToSM.end());
      vector<vector<double> > variableValuesAll = hVariableValues;
      variableValuesAll.insert(variableValuesAll.end(), variableValuesUniqueToSM.begin(), variableValuesUniqueToSM.end()); // this combines the data points of common, uniqueToH, and uniqueToSM, and is used for FewKDE::enclose
      
      vector<string> tag1 = vector<string>(tag2.begin(), tag2.begin()+2);
      string tag2_string = tag2[0]+" "+tag2[1]+" "+tag2[2];
      
      vector<vector<double> > hbContributionsToD = discriminators[tag1].addToPredictedD(tag2_string, hVariableValues, smVariableValues, hWeights, smWeights);
      vector<double> dContributionsToD = discriminators[tag1].addToPredictedD(tag2_string, variableValuesData);
      contributionsToD[tag2] = hbContributionsToD;
      contributionsToD[tag2].push_back(dContributionsToD);

      if(makePlots)
	{
	  hL = smL = 0;
	  Discriminator discriminatorForThisFinalStateOnly = discriminators[tag1];
	  discriminatorForThisFinalStateOnly.clearBinContents();
	  discriminatorForThisFinalStateOnly.addToPredictedD(tag2_string, hVariableValues, smVariableValues, hWeights, smWeights);
	  discriminatorForThisFinalStateOnly.addToPredictedD(tag2_string, variableValuesData);
	  //	  assert(getenv("QUAERO_EDITION")!=NULL);
	  //	  bool showFewKDEs = (((string)getenv("QUAERO_EDITION")=="quaero_mit") || ((string)getenv("QUAERO_EDITION")=="quaero_development"));

	  cout << GlobalVariables::chatterTab.tab() << fs << endl;
	  discriminatorForThisFinalStateOnly.computeLogLikelihoods(hL,smL);
	  double likelihoodRatio = hL - smL;
	  cout << "            LogL = " << (likelihoodRatio>0 ? "+" : "") << Math::nice(likelihoodRatio) << endl;
	  likelihoodRatios[tag2] = likelihoodRatio;
	  graphicalOutput["expert"][tag2].addComment(tag2_string+" discriminator");
	  graphicalOutput["expert"][tag2].addDiscriminatorPlots(distributionNames, tag2_string, discriminatorForThisFinalStateOnly, hVariableValues, smVariableValues, 
								hWeights, smWeights, variableValuesData, true);
	  graphicalOutput["expert"][tag2].addCredit(RCPParameters::credit(colliderRun, experiment, fs));
	  graphicalOutput["user"][tag2].addLikelihoodRatio(colliderRun,experiment,fs,likelihoodRatio, discriminatorForThisFinalStateOnly.getExpectedEvidence());
	  graphicalOutput["expert"][tag2].addLikelihoodRatio(colliderRun,experiment,fs,likelihoodRatio, discriminatorForThisFinalStateOnly.getExpectedEvidence());
	}
    }
  GlobalVariables::chatterTab.tab(-1);
  GlobalVariables::chatterTab.tab(-1);
  
  cout << GlobalVariables::chatterTab.tab(+1) << "Total" << endl;
  for(map<vector<string>,Discriminator>::iterator i=discriminators.begin(); i!=discriminators.end(); i++)
    {
      hL = smL = 0;
      string colliderRun = i->first[0];
      string experiment = i->first[1];
      cout << GlobalVariables::chatterTab.tab() << colliderRun << " " << experiment << endl;
      i->second.computeLogLikelihoods(hL,smL);
      hypothesisLikelihood += hL;
      smLikelihood += smL;
      double likelihoodRatio = hL - smL;
      likelihoodRatios_exp[colliderRun+" "+experiment] += likelihoodRatio;
    }
  GlobalVariables::chatterTab.tab(-1);
  
  if(makePlots)
    {
      while(likelihoodRatios.size()>0)
	{
	  //cout << GlobalVariables::chatterTab.tab(+1) << "Final states contributing to logL" << endl;
	  map<vector<string>,double>::iterator maxL = likelihoodRatios.begin();
	  for(map<vector<string>,double>::iterator i = likelihoodRatios.begin(); i!=likelihoodRatios.end(); i++)
	    if(fabs(i->second)>fabs(maxL->second))
	      maxL = i;
	  
	  vector<string> tag2 = maxL->first;
	  //cout << GlobalVariables::chatterTab.tab(); print(tag2);
	  if(fabs(maxL->second) > 0.1)  // only show if it contributes non-negligibly
	    {
	      GlobalVariables::graphicalOutput["user"] = GlobalVariables::graphicalOutput["user"] + graphicalOutput["user"][tag2];
	      GlobalVariables::graphicalOutput["expert"] = GlobalVariables::graphicalOutput["expert"] + graphicalOutput["expert"][tag2];
	    }
	  likelihoodRatios.erase(likelihoodRatios.find(tag2));
	  graphicalOutput["user"].erase(graphicalOutput["user"].find(tag2));
	  graphicalOutput["expert"].erase(graphicalOutput["expert"].find(tag2));
	}
      map<string,GraphicalOutput> graphicalOutput;
      system(("echo 'Evidence' > "+QuaeroIO::getFilename(".answer")).c_str());
      for(map<string,double>::iterator i=likelihoodRatios_exp.begin(); i!=likelihoodRatios_exp.end(); i++)
	system(("echo '  "+i->first+"   log10L = "+Math::ftoa(Math::toleranceRound(i->second,0.01))+
		"' >> "+QuaeroIO::getFilename(".answer")).c_str());
      system(("echo 'Expected Evidence' >> "+QuaeroIO::getFilename(".answer")).c_str());
      for(map<vector<string>,Discriminator>::iterator i=discriminators.begin(); i!=discriminators.end(); i++)
	{
	  string colliderRun = i->first[0];
	  string experiment = i->first[1];
	  graphicalOutput["expert"].addDiscriminatorPlots(i->second, colliderRun, experiment, likelihoodRatios_exp[colliderRun+" "+experiment]);
	  graphicalOutput["expert"].addDiscriminatorPlots(contributionsToD, colliderRun, experiment, i->second.getExpectedEvidence(), likelihoodRatios_exp[colliderRun+" "+experiment]);
	  system(("echo '  "+colliderRun+" "+experiment+
		  "   h= "+Math::ftoa(Math::toleranceRound((i->second.getExpectedEvidence("h")),0.01))+
		  "   sm= "+Math::ftoa(Math::toleranceRound(-(i->second.getExpectedEvidence("sm")),0.01))+
		  "' >> "+QuaeroIO::getFilename(".answer")).c_str());
	}
      graphicalOutput["user"].newPage(4);
      GlobalVariables::graphicalOutput["user"] = graphicalOutput["user"] + GlobalVariables::graphicalOutput["user"];
      GlobalVariables::graphicalOutput["user"].addSummary(likelihoodRatios_exp);
      GlobalVariables::graphicalOutput["expert"] = graphicalOutput["expert"] + GlobalVariables::graphicalOutput["expert"];
      GlobalVariables::graphicalOutput["expert"].addSummary(likelihoodRatios_exp);
    }
  if(RCPParameters::debugLevel<=3)
    QuaeroIO::removeTmpDirectory(iSystematicShift); // remove temporary directory to save disk space
  return;
}


vector<string> determineFinalStates(string colliderRun, string experiment)
{
  printHeaderAndDate(GlobalVariables::chatterTab.tab(+1)+"Determining final states to consider in "+colliderRun+" "+experiment);

  map<string,double> b, wtSqd;
  map<string,int> Nbkg;  
  vector<string> finalStates;
  vector<double> s, sigWtSqd, bangPerBuck;
  string fs, semicolon; 
  double x; int n;
  ifstream fsigListing(QuaeroIO::getFilename("sigListing",colliderRun,experiment).c_str());
  while(fsigListing >> fs)
    {
      finalStates.push_back(fs);
      fsigListing >> x;
      s.push_back(x);
      fsigListing >> x;
      sigWtSqd.push_back(x);
      fsigListing >> semicolon; assert(semicolon==";");
    }
  fsigListing.close();
  bangPerBuck = s;

  ifstream fbkgListing(QuaeroIO::getFilename("bkgListing",colliderRun,experiment).c_str());
  while(fbkgListing >> fs)
    {
      fbkgListing >> x;  b[fs]=x;
      fbkgListing >> n;  Nbkg[fs]=n;
      fbkgListing >> x;  wtSqd[fs]=x;
      fbkgListing >> semicolon; assert(semicolon==";");
    }
  fbkgListing.close();
  

  vector<double> counter;
  for(size_t i=0; i<finalStates.size(); i++)
    {
      fs = finalStates[i];
      counter.push_back(i);      
      if((Nbkg.find(fs)==Nbkg.end())||
	 (Nbkg[fs]==0)||
	 (b[fs]==0))
	bangPerBuck[i] = FLT_MAX;
      else
	bangPerBuck[i] /= sqrt(b[fs])*Nbkg[fs];
    }
  
  // Show where the signal falls
  vector<double> counter2 = counter;
  Math::parallelQuickSort(s,counter2);
  cout << GlobalVariables::chatterTab.tab(+1) 
       << "Signal" << "                    " 
       << "# events" << "  "
       << "% events" << endl;
  double t = accumulate(s.begin(), s.end(), 0.);
  if(t==0)
    cout << GlobalVariables::chatterTab.tab() << "none" << endl;
  else
    for(int i=counter2.size()-1; i>=0; i--)
      {
	string w = Math::ftoa(((long int)(s[i]*10))/10.);
	if(w.find(".")==string::npos)
	  w += ".0";
	string p = Math::ftoa(((long int)(s[i]/t*100)));
	if(p!="0")
	  cout << GlobalVariables::chatterTab.tab() << finalStates[(int)(counter2[i]+0.5)]
	       << Math::spacePad(30-finalStates[(int)(counter2[i]+0.5)].length()-w.length()) << w << Math::spacePad(9-p.length()) << p << "%" << endl;
      }
  GlobalVariables::chatterTab.tab(-1);
  GlobalVariables::chatterTab.tab(-1);


  Math::parallelQuickSort(bangPerBuck,counter);
  vector<string> tmp = finalStates;
  for(size_t i=0; i<counter.size(); i++)
    finalStates[i] = tmp[(int)(counter[i]+0.5)];
  reverse(bangPerBuck.begin(), bangPerBuck.end());
  reverse(finalStates.begin(), finalStates.end());
  int nbkg = 0;
  double totalWeight=0, totalWeightSquared=0;
  for(size_t i=0; i<finalStates.size(); i++)
    {
      fs = finalStates[i];
      if(Nbkg.find(fs)==Nbkg.end())
	continue;
      if( (nbkg+Nbkg[fs]>RCPParameters::nBkgEvents) // too many events to handle
	  // || ((totalWeight==0 ? 0 : (totalWeightSquared/totalWeight)) > 1) // or gives epsWt > 1
	  )
	{
	  finalStates.erase(finalStates.begin()+i);
	  i--;
	}
      else
	{
	  nbkg += Nbkg[fs];
	  totalWeight += b[fs];
	  totalWeightSquared += wtSqd[fs];
	}
    }
  double epsWt = max(RCPParameters::epsilonWt.get(colliderRun, experiment),
		     (totalWeight==0 ? 0 : (totalWeightSquared/totalWeight)));
  RCPParameters::epsilonWt.store(colliderRun, experiment, epsWt);   // store this result in our Run Control Parameters namespace  
  cout << Math::spacePad(37) << "(epsWt = " << Math::nice(RCPParameters::epsilonWt.get(colliderRun, experiment)) << ")  " << endl;

  return(finalStates);
}


vector<double> resonanceMasses;  // I am declaring it here because gcc 2.96 does not optimize (-O2) correctly on the Rice Terascale Cluster when I pass it as an argument through setUpAnalysis and computeLikelihood


// Set up the analysis
//   Determine kernel estimates
//   Determine binning in the discriminant D

void setUpAnalysis(map<vector<string>,RelevantVariables> & relevantVariableses, 
		   map<vector<string>,Discriminator>& discriminators, 
		   map<string,map<vector<string>,GraphicalOutput> >& graphicalOutput)
{
  relevantVariableses.clear();
  discriminators.clear();
  printHeaderAndDate(GlobalVariables::chatterTab.tab(+1)+"Setting up Analysis");

  GlobalVariables::iSystematicShift = -3;
  QuaeroIO::makeTmpDirectory(GlobalVariables::iSystematicShift);
  shiftRefineAndPartition(GlobalVariables::iSystematicShift);

  // Determine relevant final states
  printHeaderAndDate(GlobalVariables::chatterTab.tab(+1)+"Determining relevant final states");
  map<string, vector<string> > finalStates;
  for(map<string,vector<string> >::iterator iColliderRun=RCPParameters::colliderExperiments.begin();  // loop over collider runs
      iColliderRun!=RCPParameters::colliderExperiments.end();
      iColliderRun++)
    for(size_t iExperiment=0; iExperiment<iColliderRun->second.size(); iExperiment++)  // loop over experiments
      {
	string colliderRun = iColliderRun->first;
	string experiment = iColliderRun->second[iExperiment];
	finalStates[colliderRun+" "+experiment] = determineFinalStates(colliderRun,experiment);
	for(size_t ifs=0; ifs<finalStates[colliderRun+" "+experiment].size(); ifs++)
	  {
	    string fs = finalStates[colliderRun+" "+experiment][ifs];
	    ifstream ftmp9(QuaeroIO::getFilename("bkgInitial",colliderRun,experiment,fs).c_str());
	    if(ftmp9)
	      {
		ftmp9.close();
		system(("cat "+QuaeroIO::getFilename("bkgInitial",colliderRun,experiment,fs)+
			" | sort -u -T "+QuaeroIO::getFilename("tmpDirectoryForSort", colliderRun, experiment)+
			" >> "+QuaeroIO::getFilename("metaBkg",colliderRun,experiment)).c_str());
	      }
	    else
	      ftmp9.close();
	  }
      }
  GlobalVariables::chatterTab.tab(-1);
  
  GlobalVariables::iSystematicShift = -2;
  QuaeroIO::makeTmpDirectory(GlobalVariables::iSystematicShift); // make temporary directory in which to manipulate files
  shiftRefineAndPartition(GlobalVariables::iSystematicShift);

  // Estimate kernel densities
  printHeaderAndDate(GlobalVariables::chatterTab.tab(+1)+"Constructing kernel density estimates");
  for(map<string,vector<string> >::iterator iColliderRun=RCPParameters::colliderExperiments.begin();  // loop over collider runs
      iColliderRun!=RCPParameters::colliderExperiments.end();
      iColliderRun++)
    for(size_t iExperiment=0; iExperiment<iColliderRun->second.size(); iExperiment++)  // loop over experiments
      {
	vector<string> tag1 = vector<string>(2);
	string colliderRun = tag1[0] = iColliderRun->first;
	string experiment = tag1[1] = iColliderRun->second[iExperiment];
	discriminators[tag1] = Discriminator(RCPParameters::epsilonWt.get(colliderRun, experiment));
	vector<double> signalWeightPerFinalState(finalStates[colliderRun+" "+experiment].size());
	cout << GlobalVariables::chatterTab.tab(+1) << colliderRun << " " << experiment << endl;
	
	// Compute likelihood for each experiment
	
	for(size_t ifs=0; ifs<finalStates[colliderRun+" "+experiment].size(); ifs++)
	  {
	    string fs = finalStates[colliderRun+" "+experiment][ifs];
	    
	    // Compute likelihood for each final state
	    
	    vector<string> tag2 = vector<string>(3);
	    tag2[0] = colliderRun;
	    tag2[1] = experiment;
	    tag2[2] = fs;
	    // Load events
	    vector<QuaeroEvent> eventsUniqueToH = QuaeroItemFile<QuaeroEvent>(QuaeroIO::getFilename("sig",colliderRun,experiment,fs)).loadEvents();
	    vector<QuaeroEvent> eventsUniqueToSM = QuaeroItemFile<QuaeroEvent>(QuaeroIO::getFilename("bkg",colliderRun,experiment,fs)).loadEvents(RCPParameters::leaveOut[colliderRun],"onlyInclude");
	    vector<QuaeroEvent> commonEvents = QuaeroItemFile<QuaeroEvent>(QuaeroIO::getFilename("bkg",colliderRun,experiment,fs)).loadEvents(RCPParameters::leaveOut[colliderRun]);  
	    vector<QuaeroEvent> dataEvents = getDataEvents(colliderRun,experiment,fs,RCPParameters::hintSpec);
	    
	    if(eventsUniqueToH.empty() && eventsUniqueToSM.empty())
	      continue;
	    
	    vector<string> distributionNames;
	    vector<double> weightsCommon, weightsUniqueToH, weightsUniqueToSM, weightsData;
	    vector<vector<double> > variableValuesCommon, variableValuesUniqueToH, variableValuesUniqueToSM, variableValuesData;
	    RelevantVariables relevantVariables(resonanceMasses);
	    if(RCPParameters::privilegeLevel[colliderRun+" "+experiment]>0)
	      relevantVariables.setPrivilegeLevel(RCPParameters::privilegeLevel[colliderRun+" "+experiment]);

	    bool sufficientlyDifferent = 
	      relevantVariables.compute(commonEvents, eventsUniqueToH, eventsUniqueToSM, dataEvents,     // input events
					colliderRun, experiment, fs, distributionNames,                  // input colliderRun, experiment, fs; output names of relevant distributions
					weightsCommon, weightsUniqueToH, weightsUniqueToSM, weightsData, // output event weights 
					variableValuesCommon, variableValuesUniqueToH, variableValuesUniqueToSM, variableValuesData);  // output variable values
	    
	    vector<vector<double> > hVariableValues = variableValuesCommon, smVariableValues = variableValuesCommon;
	    hVariableValues.insert(hVariableValues.end(), variableValuesUniqueToH.begin(), variableValuesUniqueToH.end());
	    smVariableValues.insert(smVariableValues.end(), variableValuesUniqueToSM.begin(), variableValuesUniqueToSM.end());
	    vector<double> hWeights = weightsCommon, smWeights = weightsCommon;
	    hWeights.insert(hWeights.end(), weightsUniqueToH.begin(), weightsUniqueToH.end());
	    smWeights.insert(smWeights.end(), weightsUniqueToSM.begin(), weightsUniqueToSM.end());
	    vector<vector<double> > variableValuesAll = hVariableValues;
	    variableValuesAll.insert(variableValuesAll.end(), variableValuesUniqueToSM.begin(), variableValuesUniqueToSM.end()); // this combines the data points of common, uniqueToH, and uniqueToSM, and is used for FewKDE::enclose
	    
	    signalWeightPerFinalState[ifs] = accumulate(weightsUniqueToH.begin(), weightsUniqueToH.end(), 0.);
	    
	    if(!sufficientlyDifferent)
	      continue;
	    
	    relevantVariableses[tag2] = relevantVariables;
	    
	    // Construct SAME kernel estimate for the variable values in common and then add the values unique to H and SM to avoid unwanted variation in results  		    
	    FewKDE commonEstimate, hEstimate_unique, smEstimate_unique; 
	    
	    // Set up the region on which the kernel estimates will be defined
	    commonEstimate.enclose(variableValuesAll);
	    hEstimate_unique.enclose(variableValuesAll);
	    smEstimate_unique.enclose(variableValuesAll);
	    
	    // Derive the pieces of the kernel estimates
	    assert(RCPParameters::nFewKdeTrials>0);
	    commonEstimate.setNFewKdeTrials(RCPParameters::nFewKdeTrials);
	    hEstimate_unique.setNFewKdeTrials(RCPParameters::nFewKdeTrials);
	    smEstimate_unique.setNFewKdeTrials(RCPParameters::nFewKdeTrials);
	    
	    commonEstimate.derive(variableValuesCommon, weightsCommon);
	    hEstimate_unique.derive(variableValuesUniqueToH, weightsUniqueToH);
	    smEstimate_unique.derive(variableValuesUniqueToSM, weightsUniqueToSM);
	    
	    // Put the pieces together
	    FewKDE hEstimate(accumulate(weightsUniqueToH.begin(), weightsUniqueToH.end(), 0.), hEstimate_unique, 
			     accumulate(weightsCommon.begin(), weightsCommon.end(), 0.), commonEstimate);
	    FewKDE smEstimate(accumulate(weightsUniqueToSM.begin(), weightsUniqueToSM.end(), 0.), smEstimate_unique,
			      accumulate(weightsCommon.begin(), weightsCommon.end(), 0.), commonEstimate);
	    
	    Discriminant discriminant(hEstimate, smEstimate, accumulate(hWeights.begin(), hWeights.end(), 0.), accumulate(smWeights.begin(), smWeights.end(), 0.));
	    
	    string tag2_string = tag2[0]+" "+tag2[1]+" "+tag2[2];
	    discriminators[tag1].addDiscriminant(tag2_string, discriminant);
	    graphicalOutput["user"][tag2].addComment(tag2_string+" densities");
	    graphicalOutput["expert"][tag2].addComment(tag2_string+" densities");
	    graphicalOutput["expert"][tag2].newPage(4);
	    graphicalOutput["expert"][tag2].add1dPlots(distributionNames, smWeights, hWeights, 
						       smVariableValues, hVariableValues, variableValuesData, 
						       vector<bool>(3,true), colliderRun, experiment, fs);
	    graphicalOutput["user"][tag2].add1dPlots(distributionNames, smWeights, hWeights, 
						     smVariableValues, hVariableValues, variableValuesData, 
						     vector<bool>(3,true), colliderRun, experiment, fs, 0, true);		
	  }
	GlobalVariables::chatterTab.tab(-1);
      }
  GlobalVariables::chatterTab.tab(-1);
  
  // Now determine the bins in the resulting discriminant D
  GlobalVariables::iSystematicShift = -1;
  QuaeroIO::makeTmpDirectory(GlobalVariables::iSystematicShift);
  shiftRefineAndPartition(GlobalVariables::iSystematicShift);

  printHeaderAndDate(GlobalVariables::chatterTab.tab(+1)+"Computing bin edges");
  for(map<string,vector<string> >::iterator iColliderRun=RCPParameters::colliderExperiments.begin();  // loop over collider runs
      iColliderRun!=RCPParameters::colliderExperiments.end();
      iColliderRun++)
    for(size_t iExperiment=0; iExperiment<iColliderRun->second.size(); iExperiment++)  // loop over experiments
      {
	string colliderRun = iColliderRun->first;
	string experiment = iColliderRun->second[iExperiment];
	vector<string> tag1 = vector<string>(2);
	tag1[0] = colliderRun;
	tag1[1] = experiment;
	
	cout << GlobalVariables::chatterTab.tab(+1) << colliderRun << " " << experiment << endl;
	for(size_t ifs=0; ifs<finalStates[colliderRun+" "+experiment].size(); ifs++)
	  {
	    // computeLikelihoodForFinalState
	    {
	      string fs = finalStates[colliderRun+" "+experiment][ifs];
	      vector<string> tag2 = vector<string>(3);
	      tag2[0] = colliderRun;
	      tag2[1] = experiment;
 	      tag2[2] = fs;
	      if(relevantVariableses.find(tag2)!=relevantVariableses.end())
		{
		  // cout << GlobalVariables::chatterTab.tab() << fs << "  " << flush;

		  // Load Events
		  vector<QuaeroEvent> eventsUniqueToH = QuaeroItemFile<QuaeroEvent>(QuaeroIO::getFilename("sig",colliderRun,experiment,fs)).loadEvents();
		  vector<QuaeroEvent> eventsUniqueToSM = QuaeroItemFile<QuaeroEvent>(QuaeroIO::getFilename("bkg",colliderRun,experiment,fs)).loadEvents(RCPParameters::leaveOut[colliderRun],"onlyInclude");
		  vector<QuaeroEvent> commonEvents = QuaeroItemFile<QuaeroEvent>(QuaeroIO::getFilename("bkg",colliderRun,experiment,fs)).loadEvents(RCPParameters::leaveOut[colliderRun]);  
		  vector<QuaeroEvent> dataEvents = getDataEvents(colliderRun,experiment,fs,RCPParameters::hintSpec);
		  
		  vector<string> distributionNames;
		  vector<double> weightsCommon, weightsUniqueToH, weightsUniqueToSM, weightsData;
		  vector<vector<double> > variableValuesCommon, variableValuesUniqueToH, variableValuesUniqueToSM, variableValuesData;
		  
		  bool sufficientlyDifferent = 
		    relevantVariableses[tag2].compute(commonEvents, eventsUniqueToH, eventsUniqueToSM, dataEvents, 
						      colliderRun, experiment, fs, distributionNames, 
						      weightsCommon, weightsUniqueToH, weightsUniqueToSM, weightsData, 
						      variableValuesCommon, variableValuesUniqueToH, variableValuesUniqueToSM, variableValuesData);
		  
		  vector<vector<double> > hVariableValues = variableValuesCommon, smVariableValues = variableValuesCommon;
		  vector<double> hWeights = weightsCommon, smWeights = weightsCommon;
		  if(variableValuesCommon.empty())
		    {
		      hVariableValues = variableValuesUniqueToH;
		      smVariableValues = variableValuesUniqueToSM;
		      hWeights = weightsUniqueToH;
		      smWeights = weightsUniqueToSM;
		    }
		  else
		    {
		      hVariableValues.insert(hVariableValues.end(), variableValuesUniqueToH.begin(), variableValuesUniqueToH.end());
		      smVariableValues.insert(smVariableValues.end(), variableValuesUniqueToSM.begin(), variableValuesUniqueToSM.end());
		      hWeights.insert(hWeights.end(), weightsUniqueToH.begin(), weightsUniqueToH.end());
		      smWeights.insert(smWeights.end(), weightsUniqueToSM.begin(), weightsUniqueToSM.end());
		    }
		  vector<vector<double> > variableValuesAll = hVariableValues;
		  if(variableValuesAll.empty())
		    variableValuesAll = variableValuesUniqueToSM;
		  else
		    variableValuesAll.insert(variableValuesAll.end(), variableValuesUniqueToSM.begin(), variableValuesUniqueToSM.end()); // this combines the data points of common, uniqueToH, and uniqueToSM, and is used for FewKDE::enclose
		  
		  assert(sufficientlyDifferent);
		  string tag2_string = tag2[0]+" "+tag2[1]+" "+tag2[2];
		  discriminators[tag1].primePredictedD(tag2_string, hVariableValues, smVariableValues, hWeights, smWeights);
		}
	    }
	  }
	cout << string(20,' ') << "Edge    ExpectedEvidence" << endl;
	discriminators[tag1].computeBinEdges();
	GlobalVariables::chatterTab.tab(-1);
      }
  GlobalVariables::chatterTab.tab(-1);
  return;
}


// Compute the likelihood

double computeLikelihood(double& err)
{
  cout << endl << GlobalVariables::chatterTab.tab(+1) << "Analyzing" << endl; 
  double lh=0., lsm0=0.;

  // Set up the analysis:  construct kernel estimates, determine the bins in the resulting discrimant D

  map<vector<string>, RelevantVariables> relevantVariables;
  map<vector<string>, Discriminator> discriminators;  
  map<string, map<vector<string>, GraphicalOutput> > graphicalOutput;

  setUpAnalysis(relevantVariables, discriminators, graphicalOutput);

  printHeaderAndDate(GlobalVariables::chatterTab.tab(+1)+"Dry run");

  // Put all of the background events that could possibly land in the relevant final states into a file called "relevantBkg".  
  // This can save a lot of time during the integration over systematic errors, since it saves Quaero from having to read and partition 
  // (and then discard) lots of irrelevant background events.

  cout << GlobalVariables::chatterTab.tab(+1) << "Extracting all relevant background events " << endl;
  map<string, vector<string> > relevantFinalStates;
  for(map<vector<string>,RelevantVariables>::iterator i=relevantVariables.begin(); i!=relevantVariables.end(); i++)
    {
      string cr = i->first[0];
      string ex = i->first[1];
      string fs = i->first[2];
      relevantFinalStates[cr+" "+ex].push_back(fs);
    }

  /* bktemp -- I will want to replace the below with something that grabs _all_ conceivably relevant final states, 
     including those final states that may fluctuate (under systematic shifts) into the final states we have chosen.  
     What I have below is a temporary solution. */

  for(map<string,vector<string> >::iterator iColliderRun=RCPParameters::colliderExperiments.begin();  // loop over collider runs
      iColliderRun!=RCPParameters::colliderExperiments.end();
      iColliderRun++)
    for(size_t iExperiment=0; iExperiment<iColliderRun->second.size(); iExperiment++)  // loop over experiments
      {
	string cr = iColliderRun->first;
	string ex = iColliderRun->second[iExperiment];
	PartitionRule partitionRule(cr+"-Quaero");
	cout << GlobalVariables::chatterTab.tab(+1) << cr << " " << ex << endl;
	if(RCPParameters::debugLevel>=3)
	  {
	    cout << GlobalVariables::chatterTab.tab() << flush;
	    for(size_t i=0; i<relevantFinalStates[cr+" "+ex].size(); i++)
	      cout << relevantFinalStates[cr+" "+ex][i] << " ";
	    cout << endl;
	  }
	GlobalVariables::chatterTab.tab(-1);
	bool keepOnlyThirdThird = true;
	partitionRule.justKeepTheRelevantStuff(relevantFinalStates[cr+" "+ex],
					       QuaeroIO::getFilename("metaBkg",cr,ex), 
					       QuaeroIO::getFilename("relevantBkg",cr,ex), keepOnlyThirdThird);
      }
  GlobalVariables::chatterTab.tab(-1);

  // Now perform a dry run
  GlobalVariables::iSystematicShift=0;
  computeLikelihoodForSystematicErrorShift(GlobalVariables::iSystematicShift, lh, lsm0, 
					   relevantVariables, discriminators, 
					   graphicalOutput);
  cout << GlobalVariables::chatterTab.tab(-1) 
       << "log10 p(D|H,s0) = " << lh << " ; " 
       << "log10 p(D|SM,s0) = " << lsm0 << endl; 

  // Now perform a real run
  printHeaderAndDate(GlobalVariables::chatterTab.tab(+1)+"Real run");
  double ans = 0.; err = 0.;
  long double hypothesisLikelihood = 0, smLikelihood = 0;
  if((abs(lh-lsm0)>10)  // so large we don't need to compute systematic errors
     || (RCPParameters::nSystematicShifts==0))
    {
      ans = lh-lsm0;
      err = 2;
    }
  else 
    {
      double err_h=0, err_sm=0;
      hypothesisLikelihood = probabilityOfObservingDataGivenHypothesis(relevantVariables, discriminators, "h", lsm0, err_h);
      smLikelihood = probabilityOfObservingDataGivenHypothesis(relevantVariables, discriminators, "sm", lsm0, err_sm);
      err = Math::addInQuadrature((fabs(log10(hypothesisLikelihood+err_h)-log10(hypothesisLikelihood-err_h))/2),(fabs(log10(smLikelihood+err_sm)-log10(smLikelihood-err_sm))/2));

      GlobalVariables::chatterTab.tab(-1);
      ans = log10(hypothesisLikelihood/smLikelihood);
    }
  if((Math::isNaNQ(ans)) ||
     (ans < -100))
    ans = -100;
  if(Math::isNaNQ(err))
    err = 10;
  double err_dAnswerdGraduateStudent = sqrt(10*RCPParameters::epsilonWt.get());
  err = Math::addInQuadrature(err,err_dAnswerdGraduateStudent);
  return(ans);
}


// Compute the answer

double computeAnswer(double& err)
{
  cout << endl;

  double targetTime = getAlgorithmParameters(RCPParameters::targetTime, 
					     RCPParameters::nSignalEvents, RCPParameters::nTurboSimTableLines, RCPParameters::nBkgEvents, 
					     RCPParameters::nFewKdeTrials, RCPParameters::nSystematicShifts);
  cout << "Target time:              " << RCPParameters::targetTime << " seconds" << endl;
  cout << "Achievable time:          " << targetTime << " seconds" << endl;
  cout << "  => nSignalEvents:       " << RCPParameters::nSignalEvents << endl;
  cout << "  => nTurboSimTableLines: " << RCPParameters::nTurboSimTableLines << endl;
  cout << "  => nBkgEvents:          " << RCPParameters::nBkgEvents << endl;
  cout << "  => nFewKdeTrials:       " << RCPParameters::nFewKdeTrials << endl;
  cout << "  => nSystematicShifts:   " << RCPParameters::nSystematicShifts << endl;

  // Signal generation

  for(map<string,vector<string> >::iterator i=RCPParameters::colliderExperiments.begin();
      i!=RCPParameters::colliderExperiments.end(); i++)
    RCPParameters::actualGeneratedLuminosity[i->first] = RCPParameters::desiredGeneratedLuminosity(i->first); // units are pb^-1

  resonanceMasses = vector<double>(0);
  resonanceMasses.push_back(80.4); resonanceMasses.push_back(91.1); resonanceMasses.push_back(175); 

  if((atoi(RCPParameters::signalFileStorage.id.c_str())>0)&&
     (atoi(RCPParameters::signalFileStorage.id.c_str())<atoi(GlobalVariables::id.c_str()))
     // &&(RCPParameters::signalFileStorage.scaleFactor<=1)
     )
    {
      system(("rm -r "+QuaeroIO::getFilename("signalfiles")+" "+GlobalVariables::silencerString+" ; cp -r "+QuaeroIO::getFilename("signalFileStorage")+" "+QuaeroIO::getFilename("signalfiles")).c_str());
    }
  else
    {

      for(map<string,string>::iterator i=RCPParameters::signalFormat.begin();
	  i!=RCPParameters::signalFormat.end(); i++)
	{
	  if(i->second=="stdhep")  // If an stdhep signal file is provided
	    RCPParameters::actualGeneratedLuminosity[i->first] = vector<double>(RCPParameters::actualGeneratedLuminosity[i->first].size(), 10000); // then set actualGeneratedLuminosity to 10 fb^-1 at each center of mass point
	  if(i->second=="pythia")  // If Pythia commands are provided 
	    QuaeroIO::generateModelPythia(i->first, resonanceMasses);  // then use Pythia to generate the signal events
	  if(i->second=="suspect")  // If Suspect commands are provided 
	    QuaeroIO::generateModelSuspect(i->first);  // then use Suspect+Pythia to generate the signal events
	  if(i->second=="madevent")  // If MadEvent commands are provided 
	    QuaeroIO::generateModelMadEvent(i->first, resonanceMasses);  // then use MadEvent to generate the signal events
	}
      
      // Detector simulation
      simulateDetectors();

    }

  // Compute answer
  err = 0;
  cout << "  Resonance masses (GeV) are:  "; print(resonanceMasses); 
  double ans = computeLikelihood(err);

  // Clean up
  if(RCPParameters::debugLevel<=3)
    system(("rm -r "+QuaeroIO::getFilename("signalfiles")).c_str());  // remove signal files to save disk space

  // Draw plots
  printHeaderAndDate("    Drawing plots");
  GlobalVariables::graphicalOutput["user"].draw(QuaeroIO::getFilename("plots")); // draw plots showing how Quaero performed the analysis
  GlobalVariables::graphicalOutput["expert"].draw(QuaeroIO::getFilename(".plots")); // additional plots for the expert
  system(("gzip -f "+QuaeroIO::getFilename("plots")).c_str()); // zip the file
  system(("gzip -f "+QuaeroIO::getFilename(".plots")).c_str()); 

  return(ans);
}

