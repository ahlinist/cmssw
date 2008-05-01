//===============================================================
// This class inherits from GraphicalOutputSimple and implements
// the graphical functionality using PAW
// Conor Henderson 2006
//===============================================================



#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "Validation/VstMathUtils/interface/Math.hh"

#include "Validation/VstQuaeroUtils/interface/GraphicalOutputSimple.hh"

#ifndef __GraphicalOutputSimple_paw_
#define __GraphicalOutputSimple_paw_


class GraphicalOutputSimple_paw: public GraphicalOutputSimple
{
public:

  /***  Constructor  ***/
  GraphicalOutputSimple_paw(std::string _pawCommand = "");

  /*** Adornment methods ***/
  void addComment(std::string comment);
  void addCredit(std::string credit);
  void newPage(int numberOfPlotsOnPage);
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

  /***  Utility methods  ***/

  GraphicalOutputSimple_paw operator+(const GraphicalOutputSimple_paw& rhs) const;

  template<class T> std::string fillPawVector(std::string vectorName, 
					      const std::vector<T>& values);

  /***  Plot methods ***/

  void add1dPlot(std::string distributionName,
		 std::vector<double> binEdges,
		 std::vector<double> fs, std::vector<double> fb, std::vector<int> fd,
		 std::vector<bool> showBkgSigData,
		 std::string plotSize="small"
		 );
  void add1dPlot(std::string distributionName,
		 std::vector<double> binEdges,
		 std::vector<std::vector<double> > fs, std::vector<std::vector<double> > fb, std::vector<int> fd,
		 std::vector<bool> showBkgSigData,
		 std::string plotSize="small"
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
		  std::vector<double> windowLimitsLeft=std::vector<double>(1,-1),
		  std::vector<double> windowLimitsRight=std::vector<double>(1,-1)
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


protected:

  std::string addUnits(std::string distributionName);
  void reformatDistributionName(std::string& distributionName);
  std::string setShadedLevels(int nLevels, std::vector<double> rgb, std::vector<double> range);
};

#endif
