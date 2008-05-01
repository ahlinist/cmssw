/*********************************
CheckDistributions with cuts
*********************************/


// Include files

#include <vector>
#include <cassert>
#include <cmath>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <numeric>
#include "Validation/VstMathUtils/interface/Math.hh"
#include "Validation/VstQuaeroUtils/interface/BumpStatistic.hh"
#include "Validation/VstQuaeroUtils/interface/QuaeroEvent.hh"
#include "Validation/VstQuaeroUtils/interface/QuaeroItemFile.hh"
#include "Validation/VstQuaeroUtils/interface/FinalState.hh"
#include "Validation/VstQuaeroUtils/interface/GraphicalOutputSimple_root.hh"
#include "Validation/VstQuaeroUtils/interface/GraphicalOutputSimple_paw.hh"
#include "Validation/VstQuaeroUtils/interface/JargonReduction.hh"
#include "Validation/VstQuaeroUtils/interface/RelevantVariables.hh"
using namespace std;



/*******************************************************
                Impose your cut here
*******************************************************/

bool passesMyCutQ(string finalState, QuaeroEvent& e, double sumPtCut)
{
  if(e.sumPt()<sumPtCut)
    return(false);
  bool ans = true;
  
  if(false)
    {
      ans=false;
      HepLorentzVector o1 = e.getThisObject("e+",1)->getFourVector();  
      HepLorentzVector o2 = e.getThisObject("e-",1)->getFourVector();  
      /*
      HepLorentzVector tau2 = e.getThisObject("tau+",2)->getFourVector();  
      HepLorentzVector e1 = e.getThisObject("e+",1)->getFourVector();  
      HepLorentzVector j1 = e.getThisObject("b",1)->getFourVector();  
      HepLorentzVector mu1 = e.getThisObject("mu+",1)->getFourVector();  
      HepLorentzVector b1 = e.getThisObject("b",1)->getFourVector();  
      HepLorentzVector b2 = e.getThisObject("b",2)->getFourVector();  
      HepLorentzVector uncl = e.getThisObject("uncl")->getFourVector();  
      HepLorentzVector j1 = e.getThisObject("j",1)->getFourVector();  
      HepLorentzVector v1 = e.getPmiss();  
      HepLorentzVector w = e1+v1;
      HepLorentzVector h = b1+j1;
      double q = (w+h).m() - w.m() - h.m();
      double e1detEta = QuaeroRecoObject::getDetectorEta("cdf", "e", Math::theta2eta(e1.theta()), e.getZVtx());
      
      double deltaR = Math::deltaR(e1.phi(),Math::theta2eta(e1.theta()),
				   mu1.phi(),Math::theta2eta(mu1.theta()));
      double jdetEta = QuaeroRecoObject::getDetectorEta("cdf", "j", Math::theta2eta(j1.theta()), e.getZVtx());
      string location = "cc";
      if((fabs(e1.pseudoRapidity())<1)&&
	 (fabs(e2.pseudoRapidity())<1))
	location = "cc";
      else if((fabs(e1.pseudoRapidity())>1)&&
	      (fabs(e2.pseudoRapidity())>1))
	location = "pp";
      else
	location = "cp";
      if(location=="pp")
	ans = true;
      */
      double detEta = QuaeroRecoObject::getDetectorEta("cdf", "ph", Math::theta2eta(o2.theta()), e.getZVtx());
      if((o1.perp()>25)&&
	 (o2.perp()>25)&&
	 (fabs(detEta)>1.1)&&
	 (fabs(detEta)<1.35))
	ans = true;
    }

  return(ans);
}

/*******************************************************/

/****** Probably no need to change anything below ******/


void checkDistributionsWithCuts(string baseDirectory, 
				string type1, string type2, 
				string colliderRun, string experiment, 
				string finalState, double sumPtCut, 
				string jargonReductionFileName="", 
				bool useRootForGraphics=false, 
				int NBkgContributionsforRoot=5, 
				string specificDistribution="all", 
				double multiplicativeFactor=1,
				bool bumpHunting=false)
{
  //  bool fullVariableList = true;
  //  vector<int> whichPlotPage(0);

  QuaeroEvent event;
  GraphicalOutputSimple* graphicalOutput;
  if(useRootForGraphics)
    graphicalOutput = new GraphicalOutputSimple_root("",NBkgContributionsforRoot);
  else
    graphicalOutput = new GraphicalOutputSimple_paw;

  FinalState FS(finalState);
  RelevantVariables relevantVariables;
  string machineType = "hadron";
  vector<QuaeroEvent> bkgEvents, dataEvents;
  QuaeroItemFile<QuaeroEvent> fbkg((baseDirectory+"/"+type1+"/"+type1+"_"+finalState+".txt"));
  vector<string> sources;
  vector<double> sourcesWeight;

  JargonReduction jargonReduction;
  if(jargonReductionFileName!="")
    {
      jargonReduction.load(jargonReductionFileName);
    }

  int roughNumberOfDataEventsInThisFinalState = atoi(Math::system("du --total -sk "+baseDirectory+"/"+type2+"/*_"+finalState+".txt | tail -1 | awk '{print $1}'").c_str())*5;
  int prescale = roughNumberOfDataEventsInThisFinalState/10000+1;
  double runningWeight = 0;
  double incrementPoint = (log10(1.*roughNumberOfDataEventsInThisFinalState)-3)*prescale;

  if(useRootForGraphics)    
    {
      prescale = 1;
      if(incrementPoint>0)
	incrementPoint /= 3;
    }
  
  while(fbkg.nextEvent(event))
    {
      event.reWeight(event.getWeight()*multiplicativeFactor);
      if(event.getWeight()<incrementPoint)
	{
	  runningWeight += event.getWeight();
	  if(runningWeight>incrementPoint)
	    {
	      runningWeight -= incrementPoint;
	      event.reWeight(incrementPoint);
	    }
	  else
	    continue;
	}
     if(passesMyCutQ(finalState,event,sumPtCut))
	{
	  bkgEvents.push_back(event);  
	  string sourceType = jargonReduction.interpret(event.getEventType());
	  // note this defaults to simply 'event.getEventType()' if jargonReduction file not used...
	  int i = find(sources.begin(),sources.end(),sourceType)-sources.begin();
	  if(i==sources.size())
	    {
	      sources.push_back(sourceType);
	      sourcesWeight.push_back(0);
	    }
	  sourcesWeight[i] += event.getWeight();
	}
    }

  Math::parallelBubbleSort<double,string>(sourcesWeight, sources);
  reverse(sources.begin(), sources.end());
  reverse(sourcesWeight.begin(), sourcesWeight.end());
  QuaeroItemFile<QuaeroEvent> fdata((baseDirectory+"/"+type2+"/"+type2+"_"+finalState+".txt"));
  runningWeight = 0;
  while(fdata.nextEvent(event))
    {
      event.reWeight(event.getWeight()*multiplicativeFactor);
      if(event.getWeight()<prescale)
	{
	  runningWeight += event.getWeight();
	  if(runningWeight>=prescale)
	    {
	      event.reWeight(prescale);
	      runningWeight -= prescale;
	    }
	  else
	    continue;
	}
      if(passesMyCutQ(finalState,event,sumPtCut))
	dataEvents.push_back(event);  
    }

  // Georgios: If data>0 but bkg=0, then we will not be able to compute relevant variables below, so we will not be able to plot even the data. In order to not have this problem, and to be able eventually to plot the data without any background, I create this "mock" background, which mimicks the data but has weight=0 and is being treated specially so as to not appear in the plots' legend.
  if ( bkgEvents.empty() && (!dataEvents.empty()) ) {
    bkgEvents = dataEvents;
    for (int i = 0; i<bkgEvents.size() ; i++) {
      bkgEvents[i].reWeight(0.0);
      bkgEvents[i].reType("mock");
      sourcesWeight.push_back(bkgEvents[i].getWeight());
      sources.push_back("mock");
    }
  }
  
  vector<string> distributionNames1=vector<string>(0);
  vector<double> bkgWeights, dataWeights, junkWeights, commonWeights;
  vector<vector<double> > bkgVariableValues, dataVariableValues, junkVariableValues, commonVariableValues;
  relevantVariables.compute(vector<QuaeroEvent>(0), bkgEvents, dataEvents, dataEvents, 
			    colliderRun, experiment, finalState, distributionNames1, 
			    commonWeights, bkgWeights, dataWeights, junkWeights, 
			    commonVariableValues, bkgVariableValues, dataVariableValues, junkVariableValues, 
			    150, true);

  /*
    for(int i=0; i<bkgVariableValues.size(); i++)
    for(int j=0; j<bkgVariableValues[i].size(); j++)
    if(Math::isNaNQ(bkgVariableValues[i][j]))
    cout << "nan found in distribution " << distributionNames1[j] << " in event " << bkgEvents[i] << endl;
  */

  //  int nSubComponents = ( (type2=="data") ? 5 : 1 );
  int nSubComponents = ( (type2=="data") ? NBkgContributionsforRoot : 1 );
  cout << finalState << "     " << accumulate(dataWeights.begin(), dataWeights.end(), 0.) << "     " << Math::nice(accumulate(sourcesWeight.begin(),sourcesWeight.end(),0.)) << "  ( ";
  // first find the maximum weight in the vector and determine the smallestPrintableValue
  double maxWeight = 0;
  for(int i=0; i<sourcesWeight.size(); i++)
    if ( sourcesWeight[i] > maxWeight ) maxWeight=sourcesWeight[i];
  double smallestPrintableValue = 0.1;
  if ( maxWeight < 2.0 ) smallestPrintableValue = 0.01;
  for(int i=0; i<sourcesWeight.size(); i++)
    if(sourcesWeight[i] > smallestPrintableValue)
      cout << sources[i] << " = " << Math::toleranceRound(sourcesWeight[i],smallestPrintableValue) << ", " << flush;
  cout << ")" << endl;
  vector<vector<double> > bkgWeightsSubComponents(min(nSubComponents,(int)sources.size()));
  vector<vector<vector<double> > > bkgVariableValuesSubComponents(min(nSubComponents,(int)sources.size()));
  // Fill in the distinct background components.
  for(int l=0; l<bkgEvents.size(); l++)
    {
      int q = min(nSubComponents-1,(int)(find(sources.begin(), sources.end(), jargonReduction.interpret(bkgEvents[l].getEventType()))-sources.begin()));
      bkgWeightsSubComponents[q].push_back(bkgWeights[l]/prescale);
      bkgVariableValuesSubComponents[q].push_back(bkgVariableValues[l]);
    }
  vector<bool> showBkgSigData = vector<bool>(3,true); // { show bkg, show sig, show data }
  if(type2=="data")
    showBkgSigData[1] = false; // no showSig
  else
    showBkgSigData[2] = false; // no showData
  if(!useRootForGraphics)
    graphicalOutput->newPage(4);

  // replace jargon from final state name
  // note this does not work for Vista final states,
  // because there are too many to write to a file
  // we will write some code to parse and interpret the finalState name
  string finalState_nojargon = jargonReduction.interpret(finalState);

  //Georgios: Check if this final state has any significant bumps, to indicate them on the plots if there are any.
  vector<double> bumpProbability(distributionNames1.size(),1);
  vector<double> bumpLimitsLeft(distributionNames1.size(),-1);
  vector<double> bumpLimitsRight(distributionNames1.size(),-1);
  if ( bumpHunting ) {
    //    vector< pair<double, double> > bumpLimits(distributionNames1.size(), pair<double,double>(-1,-1));
    for ( int j=0; j < distributionNames1.size(); j++ ) {
      if( Bump::searchForBumpsHereOrNot(distributionNames1[j]) ) {
	vector<double> bkgv, datav, bkgwt, datawt;
	for ( int i = 0; i < bkgVariableValues.size(); i++) {
	  bkgv.push_back(bkgVariableValues[i][j]);
	  bkgwt.push_back(bkgWeights[i]);
	}
	for ( int i = 0; i < dataVariableValues.size(); i++) {
	  datav.push_back(dataVariableValues[i][j]);
	  datawt.push_back(dataWeights[i]);
	}
	BumpStatistic bumpStat(bkgv, bkgwt, datav, datawt);
	bumpProbability[j]=bumpStat.probability();
	pair<double,double> bumpLimits = bumpStat.getLimits();
	bumpLimitsLeft[j]=bumpLimits.first;
	bumpLimitsRight[j]=bumpLimits.second;
	//	cout << "We found in " << distributionNames1[j] << " a bump of probability : " << bumpProbability[j] << " and limits: ["<< bumpLimits[j].first << "," << bumpLimits[j].second << "]" << endl;
      }//if bumphuntable
    }//for each distribution
  } //if bumpHunting

  //Georgios: just before outputting, I want to redefine all the needed vectors, keeping just the elements that I want to be written out. That is to be able to plot just some specified kinematic distribution of this final state, instead of them all.
  graphicalOutput->add1dPlots(distributionNames1, bkgWeightsSubComponents, vector<vector<double> >(1,dataWeights), 
			     bkgVariableValuesSubComponents, 
			     vector<vector<vector<double> > >(1,dataVariableValues), 
			     dataVariableValues, sources, sourcesWeight,
			     showBkgSigData, colliderRun, experiment, finalState_nojargon, 0, false, specificDistribution, bumpLimitsLeft, bumpLimitsRight);
  string plotsFileName = "plots_withCuts_"+finalState+".ps"; 
  string macroFileName = "plots_withCuts_"+finalState+".C"; 
  graphicalOutput->draw(plotsFileName.data(),macroFileName.data());

  return;
}


int main(int argc, char* argv[])
{
  if(argc<=1)
    {
      cout << "Usage:  cd $RELEASE_DIR/; checkDistributionsWithCuts [-paw|-root] <finalState> [sumPtCut] [-jargonReductionFile=<jargonReductionFileName>] [-bkgColors=< int <= 13 >] [-distribution=<string>] [-showBumps=<true|false>]" << endl;
      cout << "   or:  checkDistributionsWithCuts [-paw|-root] <baseDirectory> <type1> <type2> <colliderRun> <experiment> <finalState> [sumPtCut] [-jargonReductionFile=<jargonReductionFileName>] " << endl;
      exit(0);
    }
  assert((argc>=2)&&(argc<=11));
  string baseDirectory, type1, type2, colliderRun, experiment, finalState;
  string jargonReductionFileName="";
  bool useRootForGraphics = false;
  int NBkgContributionsforRoot = 5;
  string specificDistribution = "all";
  bool bumpHunting = false;
  double sumPtCut = 0;
  double multiplicativeFactor=1;
  if(getenv("multiplicativeFactor")!=NULL)
    multiplicativeFactor = atof(getenv("multiplicativeFactor"));
  int k=1;
  string s = argv[1];
  if((s=="-paw")||(s=="-root"))
    {
      if(s=="-root")
	useRootForGraphics = true;
      k++;
    }

  if(argc<=7)
    {
      baseDirectory = "Vista/results/baseDirectory/";
      type1 = "bkg";
      type2 = "data";
      colliderRun = "tev2";
      experiment = "cdf";
      if(getenv("experiment")!=NULL)
	{
	  experiment = getenv("experiment");
	  assert((experiment=="cdf")||
		 (experiment=="d0"));
	}
      finalState = argv[k++];
      while(k<argc)
	{
	  if(((string)argv[k]).substr(0,21)=="-jargonReductionFile=")
	    jargonReductionFileName = ((string)argv[k++]).substr(21);
	  else if (((string)argv[k]).substr(0,11)=="-bkgColors=")
	    NBkgContributionsforRoot = atoi((((string)argv[k++]).substr(11)).data());
	  else if (((string)argv[k]).substr(0,14)=="-distribution=")
	    specificDistribution = (((string)argv[k++]).substr(14)).data();
	  else if (((string)argv[k]).substr(0,11)=="-showBumps=") {
	    string bumpHuntingArgument = (((string)argv[k++]).substr(11)).data();
	    assert((bumpHuntingArgument=="true")||(bumpHuntingArgument=="false"));
	    if ( bumpHuntingArgument == "true" ) bumpHunting = true;
	    if ( bumpHuntingArgument == "false" ) bumpHunting = false;
	  }
	  else
	    sumPtCut = atof(argv[k++]);
	}
    }
  else
    {
      baseDirectory = argv[k++];
      type1 = argv[k++];
      type2 = argv[k++];
      colliderRun = argv[k++];
      experiment = argv[k++];
      finalState = argv[k++];
      while(k<argc)
	{
	  if(((string)argv[k]).substr(0,21)=="-jargonReductionFile=")
	    jargonReductionFileName = ((string)argv[k++]).substr(21);
	  else if (((string)argv[k]).substr(0,11)=="-bkgColors=")
	    NBkgContributionsforRoot = atoi((((string)argv[k++]).substr(11)).data());
	  else if (((string)argv[k]).substr(0,14)=="-distribution=")
	    specificDistribution = (((string)argv[k++]).substr(14)).data();
	  else if (((string)argv[k]).substr(0,11)=="-showBumps=") {
	    string bumpHuntingArgument = (((string)argv[k++]).substr(11)).data();
	    assert((bumpHuntingArgument=="true")||(bumpHuntingArgument=="false"));
	    if ( bumpHuntingArgument == "true" ) bumpHunting = true;
	    if ( bumpHuntingArgument == "false" ) bumpHunting = false;
	  }
	  else
	    sumPtCut = atof(argv[k++]);
	}
    }
  checkDistributionsWithCuts(baseDirectory, type1, type2, colliderRun, experiment, finalState, sumPtCut, jargonReductionFileName, useRootForGraphics, NBkgContributionsforRoot, specificDistribution, multiplicativeFactor, bumpHunting);

  // Copy the resulting plots file to archival and viewing directories
  if(((string)getenv("USER"))=="knuteson")
    {
      system(("cp -p plots_withCuts_"+finalState+".ps plots_withCuts.ps").c_str());
      if(getenv("experiment")!=NULL)
	system(("ps2pdf plots_withCuts_"+finalState+".ps ~/Public/Quaero/quaero_development/doc/devel/"+getenv("experiment")+"/Vista/current/plots_"+finalState+".pdf").c_str());
      system(("gzip -f plots_withCuts.ps;"));
      // system(("cp -p plots_withCuts.ps.gz Quaero/doc/devel/cdf/Vista/current/plots_"+finalState+".ps.gz").c_str());
      // system(("cp -p plots_withCuts.ps.gz ~/Public/Quaero/quaero_development/doc/devel/cdf/Vista/current/plots_"+finalState+".ps.gz").c_str());
    }

  return(0);
}

