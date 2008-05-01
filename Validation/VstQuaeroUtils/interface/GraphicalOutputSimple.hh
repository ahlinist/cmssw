/*************************************************************
GraphicalOutputSimple provides routines for making plots using PAW.
This class is used by Vista to show comparisons of all relevant kinematic distributions, 
and is used by Quaero to generate plots showing how each Quaero analysis is performed.

Bruce Knuteson 2003
**************************************************************/

//================================================================
// Make this an abstract base class; functionality now implemented
// in the derived classes specifically for PAW and ROOT
// Conor Henderson, May 2006
//================================================================


#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "Validation/VstMathUtils/interface/Math.hh"

#ifndef __GraphicalOutputSimple__
#define __GraphicalOutputSimple__


class GraphicalOutputSimple
{
public:

  /***  Constructor  ***/
  GraphicalOutputSimple(std::string _pawCommand = "");

  /*** Adornment methods ***/
  virtual void addComment(std::string comment) =0;
  void addCredit(std::string credit);
  virtual  void newPage(int numberOfPlotsOnPage)=0;
  void addHeaderAndFooter(std::string colliderRun="", 
			  std::string experiment="", 
			  std::string fs="",
			  double logLikelihood=0,
			  double expectedEvidence=0,
			  double pageNumber=0,
			  bool singlePane=false
			  );
  void addLikelihoodRatio(std::string colliderRun, 
			  std::string experiment, 
			  std::string fs, 
			  double logLikelihood,
			  double expectedEvidence=0);

  virtual ~GraphicalOutputSimple(){}

  /***  Utility methods  ***/

  static std::vector<double> deriveEvenlySpacedBinEdges(double lo, double hi, int nbins);

  /***  Plot methods ***/

  virtual  void add1dPlots(std::vector<std::string> distributionNames, 
		  std::vector<bool> showBkgSigData = std::vector<bool>(3,true),
		  std::string colliderRun = "",
		  std::string experiment = "", 
		  std::string finalstate = "",
		  double logLikelihood = 0
		  )=0;
  virtual  void add1dPlots(std::vector<std::string> distributionNames, 
		  const std::vector<double>& weightsBkgSM,
		  const std::vector<double>& weightsSigBkg,
		  const std::vector<std::vector<double> >& variableValuesBkgSM, 
		  const std::vector<std::vector<double> >& variableValuesSigBkg, 
      		  const std::vector<std::vector<double> >& variableValuesData,
		  std::vector<bool> showBkgSigData = std::vector<bool>(3,true),
		  std::string colliderRun = "", 
		  std::string experiment = "", 
		  std::string finalstate = "", 
		  double logLikelihood = 0,
		  bool justShowOne = false
		  )=0;
  virtual  void add1dPlots(std::vector<std::string> distributionNames, 
			   const std::vector<std::vector<double> >& weightsBkgSM,
			   const std::vector<std::vector<double> >& weightsSigBkg,
			   const std::vector<std::vector<std::vector<double> > >& variableValuesBkgSM, 
			   const std::vector<std::vector<std::vector<double> > >& variableValuesSigBkg, 
			   const std::vector<std::vector<double> >& variableValuesData,
			   std::vector<std::string> sources, std::vector<double> sourcesWeight,
			   std::vector<bool> showBkgSigData = std::vector<bool>(3,true),
			   std::string colliderRun = "", 
			   std::string experiment = "", 
			   std::string finalstate = "", 
			   double logLikelihood = 0,
			   bool justShowOne = false,
			   std::string specificDistribution="all",
			   std::vector<double> windowLimitsLeft=std::vector<double>(1,-1),
			   std::vector<double> windowLimitsRight=std::vector<double>(1,-1)
		  )=0;
  virtual  void add2dPlot(std::string distributionName1, std::string distributionName2, 
		 std::vector<double> range1, std::vector<double> range2, 
		 int nbins1, int nbins2, 
		 std::vector<std::vector<double> > fs, 
		 std::vector<std::vector<double> > fb, 
		 std::vector<std::vector<int> > fd, 
		 std::vector<bool> showBkgSigData)=0;

  /***  Draw method  ***/

  virtual void draw(std::string plotFileName, std::string pawCommandFileName = Math::getTmpFilename(), std::string localDir = "./") =0;
  void writePawCommand(std::string pawCommandFileName);


protected:

  std::string pawCommand;  ///<  the paw command to be executed

  virtual void reformatDistributionName(std::string& distributionName)=0;
  std::string setShadedLevels(int nLevels, std::vector<double> rgb, std::vector<double> range);
  std::vector<std::vector<double> > range;

  int paneNumber;

};

#endif
