//====================================================================
// This class inherits from GraphicalOutputSimple and implements
// the graphical functionality using ROOT
//
// Conor Henderson 2006
//====================================================================


#include <vector>
#include <utility>
#include <string>
#include <fstream>
#include <iostream>
#include "Validation/VstMathUtils/interface/Math.hh"

#include "Validation/VstQuaeroUtils/interface/GraphicalOutputSimple.hh"

#ifndef __GraphicalOutputSimple_root__
#define __GraphicalOutputSimple_root_


class GraphicalOutputSimple_root: public GraphicalOutputSimple
//class GraphicalOutputSimple_root
{
public:

  /***  Constructor  ***/
  GraphicalOutputSimple_root(std::string _pawCommand = "", int NBkgContributions=5);

  //Conor:  re-declare the functions to be overloaded

  /*** Adornment methods ***/
  void addComment(std::string comment);
  void newPage(int numberOfPlotsOnPage=1);

  /***  Utility methods  ***/

  GraphicalOutputSimple_root operator+(const GraphicalOutputSimple_root& rhs) const;

  std::string addPreliminaryLabel(std::string colliderRun, std::string experiment);
  std::string createLegend(std::string legendName, int n=0);
  std::string createHistogramTitle(std::string histTitle);
  std::string addLegendEntry(std::string legendName, std::string histName, std::string text, std::string option="F", int n=0);
  std::string setGlobalStyle();
  std::string defineBkgColors();
  std::string labelHistogramAxis(std::string histName, std::string axisLabel, std::string whichAxis="X");
  std::string createRootHistogram(std::string histName, std::string binEdgesArrayName);
  std::string fillRootHistogram(std::string histName, std::string contentsArrayName);
  std::string fillRootArray(std::string arrayName, const std::vector<double>& values);
  std::string fillRootArray(std::string arrayName, const std::vector<int>& values);
  std::string markRegionOnAxis(std::string distributionCode, double limitLeft = -1, double limitRight = -1);
  void addPrescaleArrow(std::string label,double x,double YMAX);


  /***  Plot methods ***/

  void add1dPlot(std::string distributionName,
		 std::vector<double> binEdges,
		 std::vector<double> fs, std::vector<double> fb, std::vector<int> fd,
		 std::vector<bool> showBkgSigData
		 );
  void add1dPlot(std::string distributionName,
		 std::vector<double> binEdges,
		 std::vector<std::vector<double> > fs, std::vector<std::vector<double> > fb, std::vector<int> fd,
		 std::vector<bool> showBkgSigData, 
		 std::vector<std::string> sources, std::vector<double> sourcesWeight,
		 std::string colliderRun="tev2",
		 std::string experiment="cdf",
		 std::string finalState="",
		 std::vector<double> windowLimitsLeft=std::vector<double> (1,-1),
		 std::vector<double> windowLimitsRight=std::vector<double> (1,-1)
		 );
  void add1dPlots(std::vector<std::string> distributionNames, 
		  std::vector<bool> showBkgSigData = std::vector<bool>(3,true),
		  std::string colliderRun = "",
		  std::string experiment = "", 
		  std::string finalstate = "",
		  double logLikelihood = 0
		  );
  void add1dPlots(std::vector<std::string> distributionNames, 
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
		  );
  void add1dPlots(std::vector<std::string> distributionNames, 
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
		  std::vector<double> windowLimitsLeft = std::vector<double>(1,-1),
		  std::vector<double> windowLimitsRight = std::vector<double>(1,-1)
		  );

  void add2dPlot(std::string distributionName1, std::string distributionName2, 
		 std::vector<double> range1, std::vector<double> range2, 
		 int nbins1, int nbins2, 
		 std::vector<std::vector<double> > fs, 
		 std::vector<std::vector<double> > fb, 
		 std::vector<std::vector<int> > fd, 
		 std::vector<bool> showBkgSigData);

  /***  Draw method  ***/

  void draw(std::string plotFileName, std::string pawCommandFileName = Math::getTmpFilename(), std::string localDir = "./");

  //Conor: rename paw -> Main everywhere I expect

protected:

  void reformatDistributionName(std::string& distributionName);
  void reformatStringForRoot(std::string& s);
  void addUnits(std::string& distributionName);
  // Keep count of all distributions plotted, so we can give them unique names
  int distributionNumber;

  // this is how we control number of bkg contributions to show
  int fNBkgContributions; // defaults to 5, but can be up to 13

};

#endif
