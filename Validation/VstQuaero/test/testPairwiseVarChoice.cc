// This is a routine to test the merit of choosing 
// variables pairwise instead of one at a time
//
// Khaldoun Makhoul 6/23/2003

#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
#include <cassert>
#include <cmath>
#include <algorithm>
#include <numeric>
#include "Quaero/Math.hh"
#include "Quaero/QuaeroEvent.hh"
#include "Quaero/QuaeroItemFile.hh"
#include "Quaero/GraphicalOutput.hh"
#include "Quaero/FinalState.hh"
#include "Quaero/PartitionRule.hh"
#include "Quaero/kolmogorov.hh"
#include "Quaero/quaeroIO.hh"
#include "Quaero/RelevantVariables.hh"
#include "Quaero/quaero.hh"
#include "Quaero/Discriminator.hh"

using namespace std;


int main(int argc, char* argv[])
{
  assert(argc==3);
  string sigFilename = argv[1];
  string commonFilename = argv[2];

  // set some parameters for the test
  string colliderRun = "tev2";
  string experiment = "d0";
  string fs = "1e+1e-";
 
  // get files containing events.  note there is no file for data
  // since this is only a test of the best way to choose var's regardless 
  // of data. note also there is no file for events unique to SM: 
  // we assume all backgrounds are included in hypothesis.

  vector<QuaeroEvent> eventsUniqueToH = QuaeroItemFile<QuaeroEvent>(sigFilename).loadEvents();
  vector<QuaeroEvent> commonEvents = QuaeroItemFile<QuaeroEvent>(commonFilename).loadEvents();

  double figureOfMerit_Quaero = 0;
  vector<string> distributionNames_Quaero, distributionNames_Pairwise;
  string colliderType;

  GraphicalOutput graphicalOutput;

  { // compute Figure of Merit using current Quaero algorithm.
    // we will let the algorithm choose which variables are relevant
    // and let it choose the best ones according to the KS statistic
    // for their cumulative distribution functions.

    cout << "Computing Figure of Merit for one-by-one var choice" << endl;
    system("date");

    vector<string> distributionNames;
    vector<double> weightsCommon, weightsUniqueToH, weightsUniqueToSM, weightsData;
    vector<vector<double> > variableValuesCommon, variableValuesUniqueToH, variableValuesUniqueToSM, variableValuesData;

    RelevantVariables relevantVariables;
    bool makePlots = true;

    bool sufficientlyDifferent = relevantVariables.compute(makePlots, commonEvents, eventsUniqueToH, vector<QuaeroEvent>(0), vector<QuaeroEvent>(0), colliderRun, experiment, fs, distributionNames, weightsCommon, weightsUniqueToH, weightsUniqueToSM, weightsData, variableValuesCommon, variableValuesUniqueToH, variableValuesUniqueToSM, variableValuesData);

    vector<vector<double> > hVariableValues = variableValuesCommon, smVariableValues = variableValuesCommon;
    hVariableValues.insert(hVariableValues.end(), variableValuesUniqueToH.begin(), variableValuesUniqueToH.end());
    smVariableValues.insert(smVariableValues.end(), variableValuesUniqueToSM.begin(), variableValuesUniqueToSM.end());
    vector<double> hWeights = weightsCommon, smWeights = weightsCommon;
    hWeights.insert(hWeights.end(), weightsUniqueToH.begin(), weightsUniqueToH.end());
    smWeights.insert(smWeights.end(), weightsUniqueToSM.begin(), weightsUniqueToSM.end());

    Discriminator discriminator;

    KernelEstimate commonEstimate, hEstimate_unique, smEstimate_unique; 
    commonEstimate.derive(variableValuesCommon, weightsCommon);
    hEstimate_unique.derive(variableValuesUniqueToH, weightsUniqueToH);
    smEstimate_unique.derive(variableValuesUniqueToSM, weightsUniqueToSM);
    KernelEstimate hEstimate(accumulate(weightsUniqueToH.begin(), weightsUniqueToH.end(), 0.), hEstimate_unique, 
			     accumulate(weightsCommon.begin(), weightsCommon.end(), 0.), commonEstimate);
    KernelEstimate smEstimate(accumulate(weightsUniqueToSM.begin(), weightsUniqueToSM.end(), 0.), smEstimate_unique,
			      accumulate(weightsCommon.begin(), weightsCommon.end(), 0.), commonEstimate);

    Discriminant discriminant(hEstimate, smEstimate);

    discriminator = Discriminator(discriminant, hVariableValues, smVariableValues, hWeights, smWeights, RCPParameters::epsilonWt);

    distributionNames_Quaero = distributionNames;

    figureOfMerit_Quaero = discriminator.getExpectedEvidence();

    cout << "Figure of Merit for one-by-one algorithm is equal to \n" << figureOfMerit_Quaero << endl;
    cout << "The variables chosen were ";    
    for (int n = 0; n < distributionNames_Quaero.size(); n++)
      {
	if (n != (distributionNames_Quaero.size() - 1))
	  cout << distributionNames_Quaero[n] << " and ";
	else
	  cout << distributionNames_Quaero[n] << endl;
      }
    system("date");
    cout << endl;
    
    double L = 0;
    vector<bool> showBkgSigData = vector<bool>(3,true); showBkgSigData[2] = false;

    graphicalOutput.add1dPlots(distributionNames_Quaero, smWeights, hWeights, smVariableValues, hVariableValues,  vector<vector<double> >(0),  showBkgSigData, colliderRun, experiment, fs, L );

    vector<vector<double> > range; vector<int> nbins;
    Math::makeNiceHistogramRange(smWeights, hWeights, smVariableValues, hVariableValues, vector<vector<double> >(0) , range, nbins);
    graphicalOutput.addDiscriminatorPlots(distributionNames_Quaero, discriminator, range, vector<vector<double> >(0));

  } // end one-by-one variable choice


    
  { // compute Figure of Merit using pairwise algorithm
    // this time we ask the algorithm to give us all variables.
    // we then form kernel estimates for each possible pair of variables
    // and choose the best pair according to its figure of merit.

    cout << "Computing Figure of Merit for pairwise var choice" << endl;

    vector<string> distributionNames;
    vector<double> weightsCommon, weightsUniqueToH, weightsUniqueToSM, weightsData;
    vector<vector<double> > variableValuesCommon, variableValuesUniqueToH, variableValuesUniqueToSM, variableValuesData;

    RelevantVariables relevantVariables;
    bool makePlots = true;

    bool sufficientlyDifferent = relevantVariables.compute(makePlots, commonEvents, eventsUniqueToH, vector<QuaeroEvent>(0), vector<QuaeroEvent>(0), colliderRun, experiment, fs, distributionNames, weightsCommon, weightsUniqueToH, weightsUniqueToSM, weightsData, variableValuesCommon, variableValuesUniqueToH, variableValuesUniqueToSM, variableValuesData);

    // get all variable values
    bool fullVariableList = true;
    for (int m = 0; m < eventsUniqueToH.size(); m++)
      variableValuesUniqueToH[m] = relevantVariables.getAllVariableValuesForThisEvent(fs, eventsUniqueToH[m], fullVariableList);
    for (int m = 0; m < commonEvents.size(); m++)    
      variableValuesCommon[m] = relevantVariables.getAllVariableValuesForThisEvent(fs, commonEvents[m], fullVariableList);

    // get all variable names
    if (colliderRun == "tev2")
      colliderType = "hadron";
    else if (colliderRun == "lep2")
      colliderType == "lepton";
    bool pawFormatted = true;
    distributionNames_Pairwise = relevantVariables.getAllVariableNamesForThisEvent(fs, pawFormatted, fullVariableList, colliderType);

    cout << "The full variable list for this event includes: ";
    for (int s = 0; s <distributionNames_Pairwise.size(); s++)
      {
	if (s != (distributionNames_Pairwise.size() - 1))
	  cout << distributionNames_Pairwise[s] << ", "; 
	else 
	  cout << distributionNames_Pairwise[s] << endl;
      }

    vector<vector<double> > hVariableValues = variableValuesCommon, smVariableValues = variableValuesCommon;
    vector<double> hWeights = weightsCommon, smWeights = weightsCommon;
    hVariableValues.insert(hVariableValues.end(), variableValuesUniqueToH.begin(), variableValuesUniqueToH.end());
    hWeights.insert(hWeights.end(), weightsUniqueToH.begin(), weightsUniqueToH.end());

    vector<vector<double> > pairVariableValuesCommon = vector<vector<double> >(variableValuesCommon.size(), vector<double>(2));
    vector<vector<double> > pairVariableValuesUniqueToH = vector<vector<double> >(variableValuesUniqueToH.size(), vector<double>(2));
    vector<vector<double> > pairVariableValuesUniqueToSM = vector<vector<double> >(variableValuesUniqueToSM.size(), vector<double>(2));

    vector<double> pairWeightsCommon = vector<double>(weightsCommon.size());;
    vector<double> pairWeightsUniqueToH = vector<double>(weightsUniqueToH.size()); 
    vector<double> pairWeightsUniqueToSM = vector<double>(weightsUniqueToSM.size());
    vector<string> pairVarNames = vector<string>(2), distributionNames_PairwiseBest = vector<string>(2);

    int nVars = 0;
    if (hVariableValues.size() > 0) 
      nVars = hVariableValues[0].size();

    double figureOfMerit_Pairwise = 0;

    cout << "Looping over all variable pairs" << endl;
    for(int i = 0; i < nVars; i++)  // loop over all variables
      for(int j = i+1; j < nVars; j++) 
	{
	  pairVarNames[0] = distributionNames_Pairwise[i];
	  pairVarNames[1] = distributionNames_Pairwise[j];

	  if(
	     (pairVarNames[0] == "e^+!#-#pt" &&  pairVarNames[1] == "mass(e^+!,e^-!)") 
	     || true // all for now
	     )
	    { 
	      for(int k = 0; k < variableValuesCommon.size(); k++) // loop over all events in common
		{
		  pairVariableValuesCommon[k][0] = variableValuesCommon[k][i];
		  pairVariableValuesCommon[k][1] = variableValuesCommon[k][j];
	    
		  pairWeightsCommon[k] = weightsCommon[k] ;
		  pairWeightsCommon[k] = weightsCommon[k] ;
		}

	      for(int l = 0; l < variableValuesUniqueToH.size(); l++)  // loop over all events in h
		{
		  pairVariableValuesUniqueToH[l][0] = variableValuesUniqueToH[l][i] ;
		  pairVariableValuesUniqueToH[l][1] = variableValuesUniqueToH[l][j] ;

		  pairWeightsUniqueToH[l] = weightsUniqueToH[l] ;
		  pairWeightsUniqueToH[l] = weightsUniqueToH[l] ;
		}

	      for(int m = 0; m < variableValuesUniqueToSM.size(); m++)  // loop over all events in sm
		{
		  pairVariableValuesUniqueToSM[m][0] = variableValuesUniqueToSM[m][i] ;
		  pairVariableValuesUniqueToSM[m][1] = variableValuesUniqueToSM[m][j] ;

		  pairWeightsUniqueToSM[m] = weightsUniqueToSM[m] ;
		  pairWeightsUniqueToSM[m] = weightsUniqueToSM[m] ;
		}

	      Discriminator discriminator;

	      // fill in variable values for this pair to be used by discriminator below
	      vector<vector<double> > pairHVariableValues = pairVariableValuesCommon, pairSmVariableValues = pairVariableValuesCommon;
	      pairHVariableValues.insert(pairHVariableValues.end(), pairVariableValuesUniqueToH.begin(), pairVariableValuesUniqueToH.end());
	      vector<double> pairHWeights = pairWeightsCommon, pairSmWeights = pairWeightsCommon;
	      pairHWeights.insert(pairHWeights.end(), pairWeightsUniqueToH.begin(), pairWeightsUniqueToH.end());
	      
	      cout << "Before derivation of kernel est." << endl;
	      system("date");

	      // form kernel estimate for this pair
	      KernelEstimate pairCommonEstimate, pairHEstimate_unique, pairSmEstimate_unique; 
	      pairCommonEstimate.derive(pairVariableValuesCommon, pairWeightsCommon);
	      pairHEstimate_unique.derive(pairVariableValuesUniqueToH, pairWeightsUniqueToH);
	      pairSmEstimate_unique.derive(pairVariableValuesUniqueToSM, pairWeightsUniqueToSM);
	      KernelEstimate pairHEstimate(accumulate(pairWeightsUniqueToH.begin(), pairWeightsUniqueToH.end(), 0.), pairHEstimate_unique, 
					   accumulate(pairWeightsCommon.begin(), pairWeightsCommon.end(), 0.), pairCommonEstimate);
	      KernelEstimate pairSmEstimate(accumulate(pairWeightsUniqueToSM.begin(), pairWeightsUniqueToSM.end(), 0.), pairSmEstimate_unique, 
					    accumulate(pairWeightsCommon.begin(), pairWeightsCommon.end(), 0.), pairCommonEstimate);

	      cout << "After derivation of kernel est." << endl;
	      system("date");

	      Discriminant discriminant(pairHEstimate, pairSmEstimate);
	
	      discriminator = Discriminator(discriminant, pairHVariableValues, pairSmVariableValues, pairHWeights, pairSmWeights, RCPParameters::epsilonWt);

	      double _figureOfMerit_Pairwise = discriminator.getExpectedEvidence();

	      cout << "Figure of Merit for variables " << pairVarNames[0] << " and " << pairVarNames[1] << endl;
	      cout << "is equal to " << _figureOfMerit_Pairwise << endl;

	  // choose best pair
	      if (_figureOfMerit_Pairwise > figureOfMerit_Pairwise)
		{
		  distributionNames_PairwiseBest = pairVarNames;
		  figureOfMerit_Pairwise = _figureOfMerit_Pairwise;
		}

	      double L = 0;
	      vector<bool> showBkgSigData = vector<bool>(3,true); showBkgSigData[2] = false;

	      graphicalOutput.add1dPlots(pairVarNames, pairSmWeights, pairHWeights, pairSmVariableValues, pairHVariableValues,  vector<vector<double> >(0),  showBkgSigData, colliderRun, experiment, fs, L);

	      vector<vector<double> > range; vector<int> nbins;
	      Math::makeNiceHistogramRange(pairSmWeights, pairHWeights, pairSmVariableValues, pairHVariableValues, vector<vector<double> >(0) , range, nbins);
	      graphicalOutput.addDiscriminatorPlots(pairVarNames, discriminator, range, vector<vector<double> >(0));
	    }

	} // end loops over all variable pairs

   
    cout << "Figure of Merit for pairwise algorigthm is equal to \n" << figureOfMerit_Pairwise << endl;
    cout << "The variables chosen were ";

    for (int n = 0; n <distributionNames_PairwiseBest.size(); n++)
      {
	if (n != (distributionNames_PairwiseBest.size() - 1))
	  cout << distributionNames_PairwiseBest[n] << " and "; 
	else 
	  cout << distributionNames_PairwiseBest[n] << endl;
      }
  } // end pairwise variable choice
  
  cout << "Generating PAW graphical output" << endl;

  graphicalOutput.draw("testAllPairs3KFix.ps");
  return(0);
}
    

