/*********************************
CheckDistributions creates the Vista comparison of data to background.
Bruce Knuteson 2003
*********************************/


// Include files

#include <vector>
#include <cassert>
#include <cmath>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "Validation/VstMathUtils/interface/Math.hh"
#include "Validation/VstQuaeroUtils/interface/Kolmogorov.hh"
#include "Validation/VstQuaeroUtils/interface/BumpStatistic.hh"
#include "Validation/VstQuaeroUtils/interface/QuaeroEvent.hh"
#include "Validation/VstQuaeroUtils/interface/QuaeroItemFile.hh"
#include "Validation/VstQuaeroUtils/interface/FinalState.hh"
#include "Validation/VstQuaeroUtils/interface/GraphicalOutputSimple.hh"
#include "Validation/VstQuaeroUtils/interface/GraphicalOutputSimple_paw.hh"
#include "Validation/VstQuaeroUtils/interface/GraphicalOutputSimple_root.hh"
#include "Validation/VstQuaeroUtils/interface/RelevantVariables.hh"
#include "Validation/VstQuaeroUtils/interface/JargonReduction.hh"
using namespace std;


/// Return a string with n spaces
string spacePad(int n)
{
  string ans="";
  for(int i=0; i<n; i++)
    ans = ans + " ";
  return(ans);
}

/// Sum the event weights in a single file
/// Input:   
///   filename:  file name containing events
/// Output:
///    sources:  the event types that contribute in this file
///     weight:  the total weight contributed by the i^th event type 
///    return value:  the total weight
///
double sumSingleFile(string filename, vector<string> & sources, vector<double> & weight)
{
  ifstream fin(filename.c_str());
  sources = vector<string>(0);
  weight = vector<double>(0);
  QuaeroEvent event;
  double totalWeight = 0.;
  size_t i;
  while(fin >> event)
    {
      totalWeight += event.getWeight();
      i = find(sources.begin(),sources.end(),event.getEventType())-sources.begin();
      if(i==sources.size())
	{
	  sources.push_back(event.getEventType());
	  weight.push_back(0);
	}
      weight[i] += event.getWeight();
    }
  fin.close();
  return(totalWeight);
}



/// Produce a global comparison between two samples
/// The samples must already have been partitioned
///   files1:  <baseDirectory>/<type1>_<finalState>.txt
///   files2:  <baseDirectory>/<type2>_<finalState>.txt
///
void checkDistributions(string baseDirectory, string type1, string type2, 
			string colliderRun, string experiment,
			bool bumpHunting=false,
			int showPlotLevel=1,
			bool useRootForGraphics=false,
			string jargonReductionFileName="",
			double multiplicativeFactor=1,
			pair<int,int> cafSection=pair<int,int>(-1,-1)     // cafSection.first of cafSection.second (e.g., cafSection #3 out of 21 total
			)
{
  bool fullVariableList = true;
  string cafSectionString = "";
  if(cafSection.second>0)
    cafSectionString = "_cafSection_"+Math::ftoa(cafSection.first);
  string fileNameNorm = "norm"+cafSectionString+".txt";
  string fileNameShape = "shape"+cafSectionString+".txt";
  string fileNameBump = "bump"+cafSectionString+".txt";
  system(("rm date1"+cafSectionString+".txt >& /dev/null; date > date1"+cafSectionString+".txt").c_str());
  const bool calculateNormalizationProbability = true; // uncomment the relevant line below if you change this to true;
  const bool showLotsOfPlots = (showPlotLevel>=2);
  const bool showPlots = (showPlotLevel>=1);
  ofstream fNorm(fileNameNorm.c_str());
  ofstream fShape(fileNameShape.c_str());
  ofstream fBump(fileNameBump.c_str());

  string type1a = type1, type2a = type2;
  if(type1a.substr(0,4)=="bkg_")
    type1a = "bkg";
  if(type2a.substr(0,4)=="bkg_")
    type2a = "bkg";

  JargonReduction jargonReduction;
  jargonReduction.load(jargonReductionFileName);

  vector<int> whichPlotPage(0);
  string fs;
  vector<double> ans_shape_probabilities;
  vector<double> ans_bump_probabilities;
  vector<pair<double,double> > ans_bump_limits;
  vector<string> ans_shape_finalStates2;
  vector<string> ans_shape_whichDistribution;
  vector<double> excessInData;
  vector<double> ans_normalizationProbabilities;
  vector<string> ans_finalStates1;
  vector<string> notSufficientStatistics_finalStates;
  vector<double> dataWeight_in_notSufficientStatistics_finalStates;
  vector<double> bkgWeight_in_notSufficientStatistics_finalStates;
  vector<double> ans_dataWeight;
  vector<double> ans_bkgWeight;
  vector<double> ans_deltaBkgWeight;
  vector<double> ans_deltaDataWeight;
  vector<vector<string> > ans_bkgSources;
  vector<vector<double> > ans_bkgSourcesWeight;
  //map<string,vector<HepLorentzVector> > obj_dataObj, obj_bkgObj;
  //map<string,vector<double> > obj_dataWeight, obj_bkgWeight;
  int currentPlotPage = 1;

  int nfs = 0;
  QuaeroEvent event;
  GraphicalOutputSimple* graphicalOutput;

  if(useRootForGraphics)
    graphicalOutput = new GraphicalOutputSimple_root;
  else
    graphicalOutput = new GraphicalOutputSimple_paw;

  /***  Perform comparison  ***/

  vector<string> dataFilenames = Math::getFilesInDirectory(baseDirectory+"/"+type2,type2a+"_*.txt");
  for(size_t i=0; i<dataFilenames.size(); i++)
    {
      if((cafSection.second<0)||
	 (((int) i)%cafSection.second==cafSection.first-1))
	{
      string dataFilename = dataFilenames[i];
      string fs = dataFilename.substr(type2a.length()+1,dataFilename.length()-(type2a.length()+1)-4); // extract <fs> from "type2a_<fs>.txt" 
      FinalState FS(fs);
      RelevantVariables relevantVariables;

      cerr << "checking final state " << fs << " " << flush;
      nfs ++;
      vector<QuaeroEvent> bkgEvents, dataEvents;
      double dataWeight = 0, bkgWeight = 0;
      map<string,double> sourcesWeightMap;

      QuaeroItemFile<QuaeroEvent> fbkg((baseDirectory+"/"+type1+"/"+type1a+"_"+fs+".txt"));
      vector<double> bkgwt(0), datawt(0);
      vector< vector<double> > bkgvAll(0);
      cerr << "bkg " << flush;
      int counter=0;
      //int n=0;
      double runningWeight = 0;

      int roughNumberOfDataEventsInThisFinalState = atoi(Math::system("du --total -sk "+baseDirectory+"/"+type2+"/*_"+fs+".txt | tail -1 | awk '{print $1}'").c_str())*5;
//      int prescale = roughNumberOfDataEventsInThisFinalState/10000+1;
//      double incrementPoint = (log10(1.*roughNumberOfDataEventsInThisFinalState)-3)*prescale;
//mrenna 10-03-07
      int prescale=1;
      double incrementPoint=0.;

      if(useRootForGraphics)    
	{
	  prescale = 1;
	  if(incrementPoint>0)
	    incrementPoint /= 3;
	}

      if(colliderRun=="hera")
	prescale = 1;

      while(fbkg.nextEvent(event))
	{
	  event.reWeight(event.getWeight()*multiplicativeFactor);
	  bkgWeight += event.getWeight();

	  string sourceType = jargonReduction.interpret(event.getEventType());
	  if(sourcesWeightMap.find(sourceType)==sourcesWeightMap.end())
	    sourcesWeightMap[sourceType] = 0;
	  sourcesWeightMap[sourceType] += event.getWeight();
	  
	  double incrementPoint1 = incrementPoint;
	  //If incrementPoint1==0 then we don't reduce the statistics.
	  //HACK: JUST FOR 1e+1j AND 1e+2j, hide the 3j-effect that reflects there, by
	  //artificially increasing the statistical uncertainty, by reducing
	  //the statistics.
	  /*	  if(fs=="1e+1j"||fs=="1e+2j" || fs=="1mu+1pmiss")
	    //reduce normally just for these guys.  1mu+1pmiss is reduced simply because it takes too long.
	    incrementPoint1=incrementPoint;
	    else */ 
	    incrementPoint1 = 0; //don't reduce statistics

	  if(event.getWeight()<incrementPoint1)
	    {
	      runningWeight += event.getWeight();
	      if(runningWeight>incrementPoint1)
		{
		  runningWeight -= incrementPoint1;
		  event.reWeight(incrementPoint1);
		}
	      else
		continue;
	    }
	  counter++;

	  bkgvAll.push_back(relevantVariables.getAllVariableValuesForThisEvent(fs,event,fullVariableList));
	  bkgwt.push_back(event.getWeight());
	  bkgEvents.push_back(event);  
	}

      double deltaBkgWeight = 0.;
      //mrenna:  don't mess around with mc weights
      if(bkgWeight>0)
	{
	  deltaBkgWeight = bkgWeight/sqrt(Math::effectiveNumberOfEvents(bkgwt)) + 1;
	  /* At the tevatron, the low-pt jet final states have an additional uncertainty from
	     pyth_jj_000 (or some equivalent sample), since one of these events can fluctuate high
	     into one of these final states, with a large weight.  If none of these final states have
	     such an event, this pyth_jj_000 "weight quantum" will not be included in the error estimate
	     unless we add this explicitly, which we do here.  */
	  if((colliderRun=="tev2")&&
	     ((fs=="2j_sumPt0-400")||
	      (fs=="2j1pmiss_sumPt0-400")||
	      (fs=="3j_sumPt0-400")||
	      (fs=="4j_sumPt0-400")
	      ))
	      deltaBkgWeight = sqrt(deltaBkgWeight*deltaBkgWeight+100*100);
	}
      else
	deltaBkgWeight = 1.;
   
      QuaeroItemFile<QuaeroEvent> fdata((baseDirectory+"/"+type2+"/"+type2a+"_"+fs+".txt"));
      vector< vector<double> > datavAll(0);
      cerr << "data " << flush;
      runningWeight = 0;
      while(fdata.nextEvent(event))
	{
	  event.reWeight(event.getWeight()*multiplicativeFactor);
	  //mrenna:  for mc comparisons, don't want to muck with "data"
	  if(event.getWeight()<prescale && experiment!="mc")
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
	  datavAll.push_back(relevantVariables.getAllVariableValuesForThisEvent(fs,event,fullVariableList));
	  datawt.push_back(event.getWeight());
	  dataWeight += event.getWeight();
	  dataEvents.push_back(event);  
	  if(colliderRun=="")
	    colliderRun = event.collider();
	}
      double deltaDataWeight = 0.;
      if(dataWeight>0)
	deltaDataWeight = max(sqrt(bkgWeight),dataWeight/sqrt(Math::effectiveNumberOfEvents(datawt)));
      else
	deltaBkgWeight = 1.;

      deltaBkgWeight = sqrt(pow(deltaBkgWeight,2.) + pow(0.00*bkgWeight,2.));  // add a 0% "systematic" in quadrature

      cerr << "normProb " << flush;
      double normalizationSigma = 0;
      if(calculateNormalizationProbability)
	//	normalizationSigma = Math::roughMagnitudeOfDiscrepancy((int)dataWeight, bkgWeight, deltaBkgWeight);
	normalizationSigma = Math::accurateMagnitudeOfDiscrepancy((int)dataWeight, bkgWeight, deltaBkgWeight);
      cerr << "FOR " << fs << " : " << dataWeight << "  " << bkgWeight << " +- " << deltaBkgWeight << " ==> " << normalizationSigma << endl << flush;
      cerr << "! " << flush;
      excessInData.push_back((normalizationSigma>0));
      ans_normalizationProbabilities.push_back(Math::sigma2prob(fabs(normalizationSigma)));

      vector<pair<double,string> > sourcesWeightPair;
      for(map<string,double>::iterator j = sourcesWeightMap.begin(); j!=sourcesWeightMap.end(); j++)
	sourcesWeightPair.push_back(pair<double,string>(j->second,j->first));
      sort(sourcesWeightPair.begin(),sourcesWeightPair.end());
      reverse(sourcesWeightPair.begin(),sourcesWeightPair.end());
      vector<string> sources;
      vector<double> sourcesWeight;
      for(vector<pair<double,string> >::iterator j = sourcesWeightPair.begin(); j!=sourcesWeightPair.end(); j++)
	{
	  sources.push_back(j->second);
	  sourcesWeight.push_back(j->first);
	}
      ans_bkgSources.push_back(sources);
      ans_bkgSourcesWeight.push_back(sourcesWeight);
      ans_bkgWeight.push_back(bkgWeight);
      ans_deltaBkgWeight.push_back(deltaBkgWeight);
      ans_dataWeight.push_back(dataWeight);
      ans_deltaDataWeight.push_back(deltaDataWeight);
      ans_finalStates1.push_back(fs);      

      if((bkgwt.size()<20))
	{
	  notSufficientStatistics_finalStates.push_back(fs);
	  whichPlotPage.push_back(0);
	  dataWeight_in_notSufficientStatistics_finalStates.push_back(dataWeight);
	  bkgWeight_in_notSufficientStatistics_finalStates.push_back(bkgWeight);
	  cerr << "-" << flush;
	}
      else
	{
	  vector<string> distributionNames=relevantVariables.getAllVariableNamesForThisEvent(fs,false,fullVariableList,colliderRun);

	  int nbkgEvents = bkgvAll.size();
	  int ndataEvents = datavAll.size();
	  vector<double> bkgv(nbkgEvents);
	  vector<double> datav(ndataEvents);
	  
	  size_t ndists = bkgvAll[0].size();
	  for(int j=0; j<nbkgEvents; j++)
	    {
	      if((bkgvAll[j].size()!=ndists)||
		 (ndists!=distributionNames.size()))
		{
		  // bktemp
		  cout << "ndists = " << ndists << endl;
		  cout << "bkgvAll[j].size() = " << bkgvAll[j].size() << endl;
		  cout << "distributionNames.size() = " << distributionNames.size() << endl;
		  for(size_t k=0; k<min(distributionNames.size(),bkgvAll[j].size()); k++)
		    cout << distributionNames[k] << " \t" << bkgvAll[j][k] << endl;
		}
	      assert((bkgvAll[j].size()==ndists)&&
		     (ndists==distributionNames.size()));
	    }
	  for(int j=0; j<ndataEvents; j++)
	    {
	      if(datavAll[j].size()!=ndists)
	        {
		  cout << datavAll[j].size() << " " << ndists << " ";
		  print(distributionNames);
		}
	      assert(datavAll[j].size()==ndists);
	    }
	  for(size_t i=0; i<ndists; i++)
	    {
	      cerr << i << " " << flush;
	      for(int j=0; j<nbkgEvents; j++)
		bkgv[j] = bkgvAll[j][i];
	      for(int j=0; j<ndataEvents; j++)
		datav[j] = datavAll[j][i];
	      

	      //////////////START/////////////
	      ans_shape_probabilities.push_back(Kolmogorov(bkgv, bkgwt, datav, datawt).probability());
	      //Georgios: To use pseudo-data, use this (instead of the line above):
	      //	      Kolmogorov kolm(bkgv,bkgwt);
	      //	      double thisKSprobability=kolm.probability();
	      //      ans_shape_probabilities.push_back(thisKSprobability);
	      ///////////////END//////////////
	      ans_shape_finalStates2.push_back(fs);
	      ans_shape_whichDistribution.push_back(distributionNames[i]);
	      // cout << "KStest: " << fs << " " << distributionNames[i] << " " << Kolmogorov(bkgv, bkgwt, datav, datawt).probability() << endl << flush;

	      if ( bumpHunting && Bump::searchForBumpsHereOrNot(distributionNames[i]) ) {
		BumpStatistic bumpStat(fs,distributionNames[i],bkgv, bkgwt, datav);

		/////////START///////////////
		int normalOrJustDataBumps = 1; //1=normal:data vs bkg. 2=justData.
		if(normalOrJustDataBumps==1) {
		  //Georgios: This is the main thing, to find the limits of the bump
		  bumpStat.findBumps();
		  ans_bump_limits.push_back(bumpStat.getLimits());
		  ans_bump_probabilities.push_back(bumpStat.probability());
		}
		else if (normalOrJustDataBumps==2) {
		  //Optionally, use this to find bumps with justData.  The trials factor in this case needs more thought, because it isn't applied fully.  We take the pvalmin and reduce it by the number of bumphuntable variables.  The internal trials factor is missing.
		  bumpStat.findBumpsJustData();
		  ans_bump_limits.push_back(bumpStat.getLimitsJustData());
		  ans_bump_probabilities.push_back(bumpStat.getPvalminJustData());
		}
		//Optionally, use this to get an estimation of the sensitivity. 
		//If all you want to know is sensitivity, you may want to save time
		//by replacing the above bumpStat.probability() with just "1".
// 		ans_bump_probabilities.push_back(1);
// 		ofstream outputFile;
// 		outputFile.open("bumpSensitivity_all.txt",ios::app);
// 		for ( double mass=20; mass <= 1000; mass += 20 ) {
// 		  outputFile << "finalState= " << fs << " distribution= " << distributionNames[i] << " signalMass= " << mass << " signalNeeded= " << bumpStat.approximateSignalNeededForDiscovery(5,mass) << endl;
// 		}
// 		outputFile.close();
		///////////END///////////////
	      }
	      else {
		// In bumpHunting mode, ignore non-mass distributions.
		// In non-bumpHunting mode, make all bumps uninteresting.
		ans_bump_probabilities.push_back(1.0);
		ans_bump_limits.push_back(pair<double, double>(0,0));
	      }
	      
	    }
	  
	  if(showPlots)
	    {
	      if((bkgWeight>200)||
		 (showLotsOfPlots)||
		 (colliderRun=="lep2")||
		 (fabs(normalizationSigma)>3)||
		 ((colliderRun=="tev2")&&
		  (fs=="1j1tau+")))
	      {
		cerr << "plots " << flush;

		int numberOfPlotsPerFinalState = 16;
		if(colliderRun=="hera")
		  numberOfPlotsPerFinalState = 40;
		if(experiment=="l3")
		  numberOfPlotsPerFinalState = 40;
		if((experiment=="aleph")&&(bkgWeight>50))
		  numberOfPlotsPerFinalState = 40;

		if(getenv("numberOfPlotsPerFinalState")!=NULL)
		  numberOfPlotsPerFinalState = atoi(getenv("numberOfPlotsPerFinalState"));

		vector<string> distributionNames1=vector<string>(0);
		vector<double> bkgWeights, dataWeights, junkWeights, commonWeights;
		vector<vector<double> > bkgVariableValues, dataVariableValues, junkVariableValues, commonVariableValues;
		relevantVariables.compute(vector<QuaeroEvent>(0), bkgEvents, dataEvents, dataEvents, 
					  colliderRun, experiment, fs, distributionNames1, 
					  commonWeights, bkgWeights, dataWeights, junkWeights, 
					  commonVariableValues, bkgVariableValues, dataVariableValues, junkVariableValues, 
					  numberOfPlotsPerFinalState, true);
		int nSubComponents = ( (type2=="data") ? 5 : 1 );	      
		vector<vector<double> > bkgWeightsSubComponents(min(nSubComponents,(int)sources.size()));
		vector<vector<vector<double> > > bkgVariableValuesSubComponents(min(nSubComponents,(int)sources.size()));
		map<string,int> sourcesMap;
		for(size_t l=0; l<sources.size(); l++)
		  sourcesMap[sources[l]] = l;
		for(size_t l=0; l<bkgEvents.size(); l++)
		  {
		    int q = min(nSubComponents-1,(int)(sourcesMap[jargonReduction.interpret(bkgEvents[l].getEventType())]));
		    bkgWeightsSubComponents[q].push_back(bkgWeights[l]/prescale);
		    bkgVariableValuesSubComponents[q].push_back(bkgVariableValues[l]);
		  } 
		whichPlotPage.push_back(currentPlotPage);
		int nPlotsPerPage = 4;
		if(useRootForGraphics)
		  nPlotsPerPage = 1;
		graphicalOutput->newPage(nPlotsPerPage);
		currentPlotPage += ((int)(1.*distributionNames1.size()/nPlotsPerPage-1./nPlotsPerPage+(distributionNames1.size()==0)))+1;
		vector<bool> showBkgSigData = vector<bool>(3,true); // { show bkg, show sig, show data }
		if(type2=="data")
		  showBkgSigData[1] = false; // no showSig
		else
		  showBkgSigData[2] = false; // no showData
		cerr << "| " << flush;
		graphicalOutput->add1dPlots(distributionNames1, bkgWeightsSubComponents, vector<vector<double> >(1,dataWeights), 
					   bkgVariableValuesSubComponents, 
					   vector<vector<vector<double> > >(1,dataVariableValues), 
					    dataVariableValues, sources, sourcesWeight, 
					   showBkgSigData, colliderRun, experiment, fs, 0);
	      }
	    else
	      whichPlotPage.push_back(0);
	    }
	}
      cerr << endl;
    }
    }
  
  /***  Summarize comparison  ***/

  int nDistributions = ans_shape_probabilities.size();
  
  int nDistributionsBumpHuntable = 0;
  for ( size_t i=0; i<ans_shape_whichDistribution.size(); i++)
    if (Bump::searchForBumpsHereOrNot(ans_shape_whichDistribution[i]))
      nDistributionsBumpHuntable++;
  

  
  vector<pair<double,int> > normalizationDissimilarities(nfs,pair<double,int>(0,0));
  vector<pair<double,int> > shapeDissimilarities(nDistributions,pair<double,int>(0,0));
  vector<pair<double,int> > bumpDissimilarities(nDistributions,pair<double,int>(0,0));
  for(int i=0; i<nfs; i++)
    {
      normalizationDissimilarities[i].first = ans_normalizationProbabilities[i];
      normalizationDissimilarities[i].second = i;
    }
  for(int i=0; i<nDistributions; i++)
    {
      shapeDissimilarities[i].first = ans_shape_probabilities[i];
      shapeDissimilarities[i].second = i;
      bumpDissimilarities[i].first = ans_bump_probabilities[i];
      bumpDissimilarities[i].second = i;
    }
  sort(normalizationDissimilarities.begin(), normalizationDissimilarities.end());
  sort(shapeDissimilarities.begin(), shapeDissimilarities.end());
  sort(bumpDissimilarities.begin(), bumpDissimilarities.end());

  string lumi = "";
  fNorm  << endl << "With " << nDistributions << " distributions considered in " << nfs << " final states" << (lumi=="" ? "" : " in ") << lumi << ", " << endl << endl;
  fShape << endl << "With " << nDistributions << " distributions considered in " << nfs << " final states" << (lumi=="" ? "" : " in ") << lumi << ", " << endl << endl;
  fBump << endl << "With " << nDistributionsBumpHuntable << " mass distributions considered in " << nfs << " final states" << (lumi=="" ? "" : " in ") << lumi << ", " << endl << endl;
  fNorm << "Dissimilarities follow:  " << endl << endl;
  fShape << "Dissimilarities follow:  " << endl << endl;
  fBump << "Dissimilarities follow:  " << endl << endl;
  fNorm  << "    Final state" << spacePad(35) << type2 << "            " << type1 << endl << endl;
  fShape << "    Final state" << spacePad(35) << type2 << "            " << type1 << endl << endl;
  fBump  << "    Final state" << spacePad(35) << type2 << "            " << type1 << endl << endl;
  int disagreeingFinalStates = 0, disagreeingDistributions = 0, disagreeingBumpHuntableDistributions = 0 ;
  int nominalNumberOfFinalStates = nfs;
  if(cafSection.second>0)
    nominalNumberOfFinalStates = 415;
  for(int i=0; i<nfs; i++)
    {
      int fsIndex = (int)normalizationDissimilarities[i].second;
      fNorm  << "  ";
      fShape << "  ";
      fBump  << "  ";
      double p = ans_normalizationProbabilities[fsIndex]*nominalNumberOfFinalStates;
      if(p>0.1)
	p = 1-pow(1-ans_normalizationProbabilities[fsIndex],1.*nominalNumberOfFinalStates);
      if(p<0.001)
	{
	  fNorm  << "* ";
	  fShape << "* ";
	  fBump  << "* ";
	  disagreeingFinalStates++;
	}
      else
	{
	  fNorm  << "  ";
	  fShape << "  ";
	  fBump  << "  ";
	}
      double s = ((int)(Math::prob2sigma(p)*10))/10.; 
      if(s==8.4)
	s=9.9;
      if(s<0) s=0;
      fNorm << ans_finalStates1[fsIndex];
      fShape << ans_finalStates1[fsIndex];
      fBump << ans_finalStates1[fsIndex];
      if((showPlots)&&(whichPlotPage[fsIndex]>0))
	{
	  fNorm  << " [" << whichPlotPage[fsIndex] << "] ";
	  fShape << " [" << whichPlotPage[fsIndex] << "] ";
	  fBump  << " [" << whichPlotPage[fsIndex] << "] ";
	}
      else
	{
	  fNorm  << " [" << "-" << "] ";
	  fShape << " [" << "-" << "] ";
	  fBump  << " [" << "-" << "] ";
	}
      fNorm  << spacePad(30-ans_finalStates1[fsIndex].length()-(4+(showPlots ? Math::ftoa(whichPlotPage[fsIndex]).length() : 1))-1*(s>10)-1);
      fShape << spacePad(30-ans_finalStates1[fsIndex].length()-(4+(showPlots ? Math::ftoa(whichPlotPage[fsIndex]).length() : 1))-1*(s>10)-1);
      fBump  << spacePad(30-ans_finalStates1[fsIndex].length()-(4+(showPlots ? Math::ftoa(whichPlotPage[fsIndex]).length() : 1))-1*(s>10)-1);
      if(s==0)
	{
	  fNorm  << " ";
	  fShape << " ";
	  fBump  << " ";
	}
      else
	{
	  if(excessInData[fsIndex])
	    {
	      fNorm  << "+";
	      fShape << "+";
	      fBump  << "+";
	    }
	  if(!excessInData[fsIndex])
	    {
	      fNorm  << "-";
	      fShape << "-";
	      fBump  << "-";
	    }
	}
      fNorm  << s;
      fShape << s;
      fBump  << s;
      if(s==(int)s)
	{
	  fNorm  << "  "; // ".0"
	  fShape << "  "; // ".0"
	  fBump  << "  "; // ".0"
	}
      fNorm  << spacePad(4); // lines up with the end of sigmas of distributions
      fShape << spacePad(4); // lines up with the end of sigmas of distributions
      fBump  << spacePad(4); // lines up with the end of sigmas of distributions
      fNorm  << spacePad(5);
      fShape << spacePad(5);
      fBump  << spacePad(5);
      ans_dataWeight[fsIndex] = Math::toleranceRound(ans_dataWeight[fsIndex],0.1);
      ans_deltaDataWeight[fsIndex] = Math::toleranceRound(ans_deltaDataWeight[fsIndex],0.1);
      fNorm  << spacePad(8-Math::ftoa(ans_dataWeight[fsIndex]).length()) << ans_dataWeight[fsIndex];
      fShape << spacePad(8-Math::ftoa(ans_dataWeight[fsIndex]).length()) << ans_dataWeight[fsIndex];
      fBump  << spacePad(8-Math::ftoa(ans_dataWeight[fsIndex]).length()) << ans_dataWeight[fsIndex];
      ans_bkgWeight[fsIndex] = Math::toleranceRound(ans_bkgWeight[fsIndex],0.1);
      ans_deltaBkgWeight[fsIndex] = Math::toleranceRound(ans_deltaBkgWeight[fsIndex],0.1);
      fNorm  << spacePad(15-Math::ftoa((int)ans_bkgWeight[fsIndex]).length()) << ans_bkgWeight[fsIndex];
      fShape << spacePad(15-Math::ftoa((int)ans_bkgWeight[fsIndex]).length()) << ans_bkgWeight[fsIndex];
      fBump  << spacePad(15-Math::ftoa((int)ans_bkgWeight[fsIndex]).length()) << ans_bkgWeight[fsIndex];
      if((ans_bkgWeight[fsIndex]>100000)||(ans_bkgWeight[fsIndex]==(int)ans_bkgWeight[fsIndex]))
	{
	  fNorm  << "  "; // ".0"
	  fShape << "  "; // ".0"
	  fBump  << "  "; // ".0"
	}
      fNorm  << " +- " << spacePad(4-Math::ftoa((int)ans_deltaBkgWeight[fsIndex]).length()) << ans_deltaBkgWeight[fsIndex];
      fShape << " +- " << spacePad(4-Math::ftoa((int)ans_deltaBkgWeight[fsIndex]).length()) << ans_deltaBkgWeight[fsIndex];
      fBump  << " +- " << spacePad(4-Math::ftoa((int)ans_deltaBkgWeight[fsIndex]).length()) << ans_deltaBkgWeight[fsIndex];
      if(ans_deltaBkgWeight[fsIndex]==(int)ans_deltaBkgWeight[fsIndex])
	{
	  fNorm  << "  "; // ".0"
	  fShape << "  "; // ".0"
	  fBump  << "  "; // ".0"
	}
      fNorm  << "    ";
      fShape << "    ";
      fBump  << "    ";
      string c = "( ";
      for(size_t j=0; j<ans_bkgSources[fsIndex].size(); j++)
	{
	  ans_bkgSourcesWeight[fsIndex][j] = Math::toleranceRound(ans_bkgSourcesWeight[fsIndex][j],0.1);
	  if(ans_bkgSourcesWeight[fsIndex][j]>0)
	    c = c + ans_bkgSources[fsIndex][j] + " = " + Math::ftoa(ans_bkgSourcesWeight[fsIndex][j]) + " , ";
	}
      if(false)
	if(ans_bkgSources[fsIndex].size()==0)
	  {
	    QuaeroItemFile<QuaeroEvent> efile((baseDirectory+"/"+type2+"/"+type2a+"_"+ans_finalStates1[fsIndex]+".txt"));
	    QuaeroEvent e;
	    while(efile.nextEvent(e))
	      c = c + e.getRunNumber() + ", ";
	  }
      if(c.length()>2)
	c = c.substr(0,c.length()-2); // strip off the trailing ", "
      c = c + ")";
      fNorm  << c << endl;
      fShape << c << endl;
      fBump  << c << endl;

      int nominalNumberOfDistributions = nDistributions;
      int nominalNumberOfBumpHuntableDistributions = nDistributionsBumpHuntable;
      if(cafSection.second>0){
	nominalNumberOfDistributions = 20000;
	nominalNumberOfBumpHuntableDistributions = 5100;
      }
      for(size_t k=0; k<ans_shape_probabilities.size(); k++)
	{
	  int ksIndex = (int)shapeDissimilarities[k].second;
	  if(ans_shape_finalStates2[ksIndex]==ans_finalStates1[fsIndex]) {
	    double pShape = ans_shape_probabilities[ksIndex]*nominalNumberOfDistributions;
	    if(pShape>0.1)
	      pShape = 1-pow(1-ans_shape_probabilities[ksIndex],1.*nominalNumberOfDistributions);
	    if(pShape<0.01) {
	      double s = ((int)(Math::prob2sigma(pShape)*10))/10.;
	      if(ans_shape_whichDistribution[ksIndex]!="'generated sumPt'") {
		fShape << "  *   " << ans_shape_whichDistribution[ksIndex] << spacePad(4+30-2-ans_shape_whichDistribution[ksIndex].length()-1*(s>10)) << s ;
		fShape << endl;
		disagreeingDistributions++;
	      }
	    }
	  }
	  int bumpIndex = (int)bumpDissimilarities[k].second;
	  if(ans_shape_finalStates2[bumpIndex]==ans_finalStates1[fsIndex]) {
	    double pBump = ans_bump_probabilities[bumpIndex]*nominalNumberOfBumpHuntableDistributions;
	    if(pBump>0.1)
	      pBump  = 1-pow(1-ans_bump_probabilities[bumpIndex],1.*nominalNumberOfBumpHuntableDistributions);
	    if(pBump<0.01) {
	      double s = ((int)(Math::prob2sigma(pBump)*10))/10.;
	      double bumpLimitLeft = ans_bump_limits[bumpIndex].first;
	      double bumpLimitRight = ans_bump_limits[bumpIndex].second;
	      fBump << "  *   " << ans_shape_whichDistribution[bumpIndex] << spacePad(4+30-2-ans_shape_whichDistribution[bumpIndex].length()-1*(s>10)) << s ;
	      fBump << "  [" << bumpLimitLeft  << "," << bumpLimitRight << "]" << endl;
	      disagreeingBumpHuntableDistributions++;
	    }
	  }
	}
    }
  fNorm  << endl;
  fShape << endl;
  fBump  << endl;
  fNorm  << "Number of disagreeing final states: " << disagreeingFinalStates << endl << endl;
  fShape << "  * score: " << disagreeingFinalStates+disagreeingDistributions << endl << endl;
  fBump  << "  * bumps: " << disagreeingBumpHuntableDistributions << endl << endl;
  
  fNorm.close();
  fShape.close();
  fBump.close();
  system(("rm date2"+cafSectionString+".txt >& /dev/null; date > date2"+cafSectionString+".txt").c_str()); 
  system(("mv "+fileNameNorm+" tmp"+cafSectionString+".txt; cat date1"+cafSectionString+".txt tmp"+cafSectionString+".txt date2"+cafSectionString+".txt > "+fileNameNorm+";").c_str());
  system(("mv "+fileNameShape+" tmp"+cafSectionString+".txt; cat date1"+cafSectionString+".txt tmp"+cafSectionString+".txt date2"+cafSectionString+".txt > "+fileNameShape+";").c_str());
  system(("mv "+fileNameBump+" tmp"+cafSectionString+".txt; cat date1"+cafSectionString+".txt tmp"+cafSectionString+".txt date2"+cafSectionString+".txt > "+fileNameBump+";").c_str());
  system(("rm date1"+cafSectionString+".txt date2"+cafSectionString+".txt tmp"+cafSectionString+".txt >& /dev/null").c_str());

  /*
  GraphicalOutputSimple frontPage;
  frontPage.newPage(4);
  vector<string> distributionNames(1);
  vector<vector<double> > discrepancies;
  int n=0;
  vector<bool> showBkgSigData(3,false);
  showBkgSigData[1] = true;
  distributionNames[0] = "normalizations";
  n = ans_normalizationProbabilities.size();
  discrepancies = vector<vector<double> >(n);
  for(int i=0; i<n; i++)
    discrepancies[i] = vector<double>(1,(excessInData[i] ? +1 : -1) * Math::prob2sigma(ans_normalizationProbabilities[i]));
  frontPage.add1dPlots(distributionNames, vector<double>(n,1.), vector<double>(n,1), discrepancies, discrepancies, discrepancies, showBkgSigData);
  distributionNames[0] = "shapes";
  n = ans_shape_probabilities.size();
  discrepancies = vector<vector<double> >(n);
  for(int i=0; i<n; i++)
    discrepancies[i] = vector<double>(1,Math::prob2sigma(ans_shape_probabilities[i]));
  frontPage.add1dPlots(distributionNames, vector<double>(n,1.), vector<double>(n,1), discrepancies, discrepancies, discrepancies, showBkgSigData);
  
  //graphicalOutput = frontPage + graphicalOutput;
  */

  if(cafSection.second<0)
    graphicalOutput->draw("plots.ps");
  else
    {
      graphicalOutput->writePawCommand("plots"+cafSectionString+".kumac");
      ofstream fpp(("numberOfPlotPages"+cafSectionString+".txt").c_str());
      fpp << (currentPlotPage-1) << endl;
    }

  ofstream fshape(("tmp_shape"+cafSectionString+".txt").c_str());
  for(size_t i=0; i<ans_shape_probabilities.size(); i++) {
    fshape << Math::prob2sigma(ans_shape_probabilities[i]) << endl;
  }
  fshape.close(); 
  ofstream fbump(("tmp_bump"+cafSectionString+".txt").c_str());
  for(size_t i=0; i<ans_bump_probabilities.size(); i++) {
    if( Bump::searchForBumpsHereOrNot(ans_shape_whichDistribution[i]) )
      fbump << Math::prob2sigma(ans_bump_probabilities[i]) << endl;
  }
  fbump.close(); 
  ofstream fnorm(("tmp_norm"+cafSectionString+".txt").c_str());
  for(size_t i=0; i<ans_normalizationProbabilities.size(); i++)
    fnorm << (excessInData[i] ? +1 : -1) * Math::prob2sigma(ans_normalizationProbabilities[i]) << endl;
  fnorm.close();

  return;
}


void combineCafSections(string baseDirectory, string type1, string type2, bool useRootForGraphics=false)
{
  system(((string)"date > date1.txt").c_str());
  system(((string)"cat date1.txt > norm.txt").c_str());
  system(((string)"cat date1.txt > shape.txt").c_str());
  system(((string)"cat date1.txt > bump.txt").c_str());
  system(((string)"echo '' >> norm.txt").c_str());
  system(((string)"echo '' >> shape.txt").c_str());
  system(((string)"echo '' >> bump.txt").c_str());
  system(((string)"echo 'With'`cat tmp_shape_cafSection_[0-9]*.txt | wc -l`' distributions considered in'`cat tmp_norm_cafSection_[0-9]*.txt | wc -l`' final states,' >> norm.txt").c_str());
  system(((string)"echo 'With'`cat tmp_shape_cafSection_[0-9]*.txt | wc -l`' distributions considered in'`cat tmp_norm_cafSection_[0-9]*.txt | wc -l`' final states,' >> shape.txt").c_str());
  system(((string)"echo 'With'`cat tmp_bump_cafSection_[0-9]*.txt | wc -l`' mass distributions considered in'`cat tmp_norm_cafSection_[0-9]*.txt | wc -l`' final states,' >> bump.txt").c_str());
  system(((string)"echo '' >> norm.txt").c_str());
  system(((string)"echo '' >> shape.txt").c_str());
  system(((string)"echo '' >> bump.txt").c_str());
  system(((string)"echo 'Dissimilarities follow:' >> norm.txt").c_str());
  system(((string)"echo 'Dissimilarities follow:' >> shape.txt").c_str());
  system(((string)"echo 'Dissimilarities follow:' >> bump.txt").c_str());
  system(((string)"echo '' >> norm.txt").c_str());
  system(((string)"echo '' >> shape.txt").c_str());
  system(((string)"echo '' >> bump.txt").c_str());
  system(((string)"echo '    Final state                                        data       bkg' >> norm.txt").c_str());
  system(((string)"echo '    Final state                                        data       bkg' >> shape.txt").c_str());
  system(((string)"echo '    Final state                                        data       bkg' >> bump.txt").c_str());
  system(((string)"echo '' >> norm.txt").c_str());
  system(((string)"echo '' >> shape.txt").c_str());
  system(((string)"echo '' >> bump.txt").c_str());

  map<int,int> numberOfPagesInThisCafSection;
  int nCafSections = Math::getFilesInDirectory("./","numberOfPlotPages_cafSection_*.txt").size();
  for(int i=1; i<=nCafSections; i++)
    numberOfPagesInThisCafSection[i] = atoi(Math::system("cat numberOfPlotPages_cafSection_"+Math::ftoa(i)+".txt").c_str());
  vector<int> needToAddThisManyPagesBefore(nCafSections+1);
  for(int i=1; i<=nCafSections; i++)
    for(int j=1; j<i; j++)
      needToAddThisManyPagesBefore[i] += numberOfPagesInThisCafSection[j];

  for(int i=1; i<=nCafSections; i++)
    {
      system(((string)"cat norm_cafSection_"+Math::ftoa(i)+".txt | egrep -i '\\[([0-9]*)\\]' | sed -e 's/.*\\[//' | sed -e 's/\\].*//' > tmpPageNumbers.tmp").c_str());
      ifstream fin("tmpPageNumbers.tmp");
      int q;
      while(fin >> q)
	{
	  string qq = Math::spacePad(((int)log10(1.0*needToAddThisManyPagesBefore[i]+q))-
					 ((int)log10(1.*q)));
	  system(((string)"cat norm_cafSection_"+Math::ftoa(i)+".txt | sed -e 's/\\["+Math::ftoa(q)+"\\]"+qq+"/["+Math::ftoa(needToAddThisManyPagesBefore[i]+q)+"qQq]/' > tmp.tmp; mv tmp.tmp norm_cafSection_"+Math::ftoa(i)+".txt").c_str());
	  system(((string)"cat shape_cafSection_"+Math::ftoa(i)+".txt | sed -e 's/\\["+Math::ftoa(q)+"\\]"+qq+"/["+Math::ftoa(needToAddThisManyPagesBefore[i]+q)+"qQq]/' > tmp.tmp; mv tmp.tmp shape_cafSection_"+Math::ftoa(i)+".txt").c_str());
	  system(((string)"cat bump_cafSection_"+Math::ftoa(i)+".txt | sed -e 's/\\["+Math::ftoa(q)+"\\]"+qq+"/["+Math::ftoa(needToAddThisManyPagesBefore[i]+q)+"qQq]/' > tmp.tmp; mv tmp.tmp bump_cafSection_"+Math::ftoa(i)+".txt").c_str());
	}
      system(((string)"cat norm_cafSection_"+Math::ftoa(i)+".txt | sed -e 's/qQq//' > tmp.tmp; mv tmp.tmp norm_cafSection_"+Math::ftoa(i)+".txt").c_str());
      system(((string)"cat shape_cafSection_"+Math::ftoa(i)+".txt | sed -e 's/qQq//' > tmp.tmp; mv tmp.tmp shape_cafSection_"+Math::ftoa(i)+".txt").c_str());
      system(((string)"cat bump_cafSection_"+Math::ftoa(i)+".txt | sed -e 's/qQq//' > tmp.tmp; mv tmp.tmp bump_cafSection_"+Math::ftoa(i)+".txt").c_str());
    }
	
  system(((string)"cat norm_cafSection_[0-9]*.txt | grep -i ' +- ' | grep -i ' \\* ' | sed -e 's/ -/ - /' | sed -e 's/ +/ + /' | sort --key=5 -n -r | sed -e 's/ + / +/' | sed -e 's/ - / -/' >> norm.txt").c_str());
  system(((string)"cat norm_cafSection_[0-9]*.txt | grep -i ' +- ' | grep -v ' \\* ' | sed -e 's/ 0 / +0 /g' | sed -e 's/ -/ - /' | sed -e 's/ +/ + /' | sort --key=4 -n -r | sed -e 's/ + / +/' | sed -e 's/ - / -/' | sed -e 's/ +0 / 0 /g' >> norm.txt").c_str());
  system(((string)"echo '' >> norm.txt").c_str());
  system(((string)"echo 'Number of disagreeing final states: '`cat norm.txt | egrep -i '[*]' | wc -l` >> norm.txt").c_str());

  //Write list of discrepant shapes
  system(((string)"cat norm.txt | grep -i ' +- ' | sed -e 's/\\*//' | awk '{print $1}' > orderedListOfFinalStates.txt").c_str());
  system(((string)"cat shape_cafSection_[0-9]*.txt | egrep -i '[-+*]' | grep -v 'score' > shape.tmp").c_str());
  system(((string)"cat shape.tmp | grep -n -i ' +- ' | awk '{print $1}' | sed -e 's/://' > shapeLineNumbers.tmp").c_str());

  ifstream fShapeLineNumbers("shapeLineNumbers.tmp");
  map<int,bool> shapeLineNumbers;
  int l;
  while(fShapeLineNumbers >> l)
    shapeLineNumbers[l]=true;
  fShapeLineNumbers.close();
  ifstream fOrderedListOfFinalStates("orderedListOfFinalStates.txt");
  string fs;
  while(fOrderedListOfFinalStates >> fs) {
    l = atoi(Math::system("grep -n -i ' "+fs+" ' shape.tmp | awk '{print $1}' | sed -e 's/://'").c_str());
    map<int,bool>::iterator l2 = shapeLineNumbers.find(l);
    l2++;
    system(((string)"cat shape.tmp | tail +"+Math::ftoa(l)+" | head -"+(l2==shapeLineNumbers.end() ? Math::ftoa(100) : Math::ftoa(l2->first-l))+ " >> shape.txt").c_str());
  }
  fOrderedListOfFinalStates.close();
  system(((string)"echo '' >> shape.txt").c_str());
  system(((string)"echo '  * score: '`cat shape.tmp | egrep -i '[*]' | grep -v score | wc -l` >> shape.txt").c_str());
  
  //Write list of discrepant bumps
  system(((string)"cat bump_cafSection_[0-9]*.txt | egrep -i '[-+*]' | grep -v 'bumps' > bump.tmp").c_str());
  system(((string)"cat bump.tmp | grep -n -i ' +- ' | awk '{print $1}' | sed -e 's/://' > bumpLineNumbers.tmp").c_str());
  
  ifstream fBumpLineNumbers("bumpLineNumbers.tmp");
  map<int,bool> bumpLineNumbers;
  while(fBumpLineNumbers >> l)
    bumpLineNumbers[l]=true;
  fBumpLineNumbers.close();
  ifstream fOrderedListOfFinalStates2("orderedListOfFinalStates.txt");
  while(fOrderedListOfFinalStates2 >> fs) {
    l = atoi(Math::system("grep -n -i ' "+fs+" ' bump.tmp | awk '{print $1}' | sed -e 's/://'").c_str());
    map<int,bool>::iterator l2 = bumpLineNumbers.find(l);
    l2++;
    // if the following line happens, it's likely that the bumpLineNumbers.tmp contains non-number elements.
    if((l2!=bumpLineNumbers.end())&&(l2->first-l<0))
      cout << "Error with bumpLineNumbers -- we find l2->first-l<0" << endl;
    system(((string)"cat bump.tmp | tail +"+Math::ftoa(l)+" | head -"+(l2==bumpLineNumbers.end() ? Math::ftoa(100) : Math::ftoa(l2->first-l))+ " >> bump.txt").c_str());
  }
  fOrderedListOfFinalStates2.close();
  system(((string)"echo '' >> bump.txt").c_str());
  system(((string)"echo '  * bumps: '`cat bump.tmp | egrep -i '[*]' | grep -v bumps | wc -l` >> bump.txt").c_str());

  system(((string)"echo '' >> norm.txt").c_str());
  system(((string)"echo '' >> shape.txt").c_str());
  system(((string)"echo '' >> bump.txt").c_str());
  system(((string)"cat date1.txt >> norm.txt").c_str());
  system(((string)"cat date1.txt >> shape.txt").c_str());
  system(((string)"cat date1.txt >> bump.txt").c_str());

  /*
  string plotsFiles = "";
  for(int i=1; i<=nCafSections; i++)
    plotsFiles += "plots_cafSection_"+Math::ftoa(i)+".ps ";
  system(("gs -q -dNOPAUSE -dBATCH -sDEVICE=pswrite -sOutputFile=plots.ps "+plotsFiles).c_str());
  */

  string pawCommand = "";
  for(int i=1; i<=nCafSections; i++)
    pawCommand += Math::system("cat plots_cafSection_"+Math::ftoa(i)+".kumac");

  if(useRootForGraphics)
    GraphicalOutputSimple_root(pawCommand).draw("plots.ps");
  else
    GraphicalOutputSimple_paw(pawCommand).draw("plots.ps");
   

  system("cat tmp_norm_cafSection_[0-9]*.txt > tmp_norm.txt");
  system("cat tmp_shape_cafSection_[0-9]*.txt > tmp_shape.txt");
  system("cat tmp_bump_cafSection_[0-9]*.txt > tmp_bump.txt");
  system("rm date1.txt orderedListOfFinalStates.txt shapeLineNumbers.tmp tmpPageNumbers.tmp numberOfPlotPages_cafSection_[0-9]*.txt shape.tmp norm_cafSection_[0-9]*.txt shape_cafSection_[0-9]*.txt plots_cafSection_[0-9]*.kumac tmp_norm_cafSection_[0-9]*.txt tmp_shape_cafSection_[0-9]*.txt bumpLineNumbers.tmp bump.tmp bump_cafSection_[0-9]*.txt tmp_bump_cafSection_[0-9]*.txt ");

  return;
}


int main(int argc, char* argv[])
{
  if(argc<6)
    {
      cout << "Usage:  checkDistributions [-paw|-root] <baseDirectory> <type1(bkg,red)> <type2(data,green)> <colliderRun> <experiment> [-bumpHunting] [short|noPlots] [-jargonReductionFile=<jargonReductionFileName>] [-x<multiplicativeFactor>] [-cafSection=<cafSection(1--n)>/<totalNumberOfCafSections(n)> | -combineCafSections]" << endl;
      exit(0);
    }
  assert((argc>=6)&&(argc<=12));
  bool useRootForGraphics = false;
  int k=1;
  string s = argv[1];
  if((s=="-paw")||(s=="-root"))
    {
      if(s=="-root")
	useRootForGraphics = true;
      k++;
    }
    
  string baseDirectory = (string)argv[k++];
  string type1 = (string)argv[k++];
  string type2 = (string)argv[k++];
  string colliderRun = (string)argv[k++];
  string experiment = (string)argv[k++];
  int showPlotLevel = 2;
  pair<int,int> cafSection = pair<int,int>(-1,-1);
  string jargonReductionFileName = "";
  bool bumpHunting = false;
  double multiplicativeFactor=1;
  if((k<argc)&&
     ((string)argv[k])=="-bumpHunting")
    {
      bumpHunting = true;
      k++;
    }
  if((k<argc)&&
     ((string)argv[k])=="short")
    {
      showPlotLevel=1;
      k++;
    }
  if((k<argc)&&
     ((string)argv[k])=="noPlots")
    {
      showPlotLevel=0;
      k++;
    }
  if((k<argc)&&
     (((string)argv[k]).substr(0,21)=="-jargonReductionFile="))
    {
      jargonReductionFileName = ((string)argv[k]).substr(21);
      k++;
    }
  if((k<argc)&&
     (((string)argv[k]).substr(0,2)=="-x"))
    {
      multiplicativeFactor = atof(((string)argv[k]).substr(2).c_str());
      k++;
    }
  if((k<argc)&&
     (((string)argv[k]).substr(0,12)=="-cafSection="))
    {
      string s = ((string)argv[k]).substr(12);
      size_t marker = 0;
      while((marker<s.length())&&
	    (s.substr(marker,1)!="/"))
	marker++;
      if(marker==s.length())
	{
	  cout << "ERROR: CAF section specification should be in the form -cafSection=<i>/<n>" << endl;
	  exit(1);
	}
      cafSection = pair<int,int>(atoi(s.substr(0,marker).c_str()),
				 atoi(s.substr(marker+1).c_str()));
    }
  if((k<argc)&&
     (((string)argv[k])=="-combineCafSections"))
    {
      combineCafSections(baseDirectory, type1, type2, useRootForGraphics);
      return(0);
    }
  
  checkDistributions(baseDirectory, type1, type2, colliderRun, experiment, bumpHunting, showPlotLevel, useRootForGraphics, jargonReductionFileName, multiplicativeFactor, cafSection);
  return(0);
}

