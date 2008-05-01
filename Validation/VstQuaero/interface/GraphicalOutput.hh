#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "Validation/VstQuaeroUtils/interface/GraphicalOutputSimple_paw.hh"
#include "Validation/VstQuaero/interface/Discriminator.hh"

#ifndef __GraphicalOutput__
#define __GraphicalOutput__


class GraphicalOutput : public GraphicalOutputSimple_paw
{

public:

  /*****  Constructor  *****/

  GraphicalOutput(std::string _pawCommand = "");

  /*****  Plots  *****/

  void addDiscriminatorPlots(std::vector<std::string> distributionNames,
			     std::string tag,
			     Discriminator& discriminator,
			     std::vector<std::vector<double> > hVariableValues, 
			     std::vector<std::vector<double> > smVariableValues, 
			     std::vector<double> hWeights, 
			     std::vector<double> smWeights,
			     std::vector<std::vector<double> > dataEvents,
			     bool showKernelEstimates = true
			     );
  void addDiscriminatorPlots(Discriminator& discriminator,
			     std::string colliderRun,
			     std::string experiment,
			     double likelihoodRatio = 0);
  void addDiscriminatorPlots(std::map<std::vector<std::string>,std::vector<std::vector<double> > > contributionsToD,
			      std::string colliderRun,
			      std::string experiment,
			      double expectedEvidence = 0,
			      double likelihoodRatio = 0);

  /*****  Adornment  *****/

  void addSummary(std::map<std::string,double> logLikelihoods);
  void showModelParameterMeasurement(std::vector<double> xValue, 
				     std::vector<double> xlogLikelihood, 
				     std::vector<double> xlogLikelihoodError=std::vector<double>(0)
				     );	    

  /*****  Utility methods  *****/

  GraphicalOutput operator+(const GraphicalOutput& rhs) const;

private:

};

#endif
