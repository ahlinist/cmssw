
#ifndef __RelevantVariables__
#define __RelevantVariables__

#include <vector>
#include <string>
#include <fstream>
#include "Validation/VstQuaeroUtils/interface/QuaeroEvent.hh"

class RelevantVariables
{
  friend std::ostream& operator<<(std::ostream & fout, const RelevantVariables & relevantVariables);
  friend std::istream& operator>>(std::istream & fin, RelevantVariables & relevantVariables);

public:
  RelevantVariables();
  bool compute(
				// input
	       const std::vector<QuaeroEvent> & commonEvents, 
	       const std::vector<QuaeroEvent> & eventsUniqueToH,
	       const std::vector<QuaeroEvent> & eventsUniqueToSM,
	       const std::vector<QuaeroEvent> & dataEvents,
	       std::string colliderRun, 
	       std::string experiment, 
	       std::string fs,
				// output
	       std::vector<std::string> & _distributionNames, 
	       std::vector<double>& weightsCommon,
	       std::vector<double>& weightsUniqueToH,
	       std::vector<double>& weightsUniqueToSM,
	       std::vector<double>& weightsData,
	       std::vector<std::vector<double> >& variableValuesCommon,
	       std::vector<std::vector<double> >& variableValuesUniqueToH,
	       std::vector<std::vector<double> >& variableValuesUniqueToSM,
	       std::vector<std::vector<double> >& variableValuesData,
				// optional input
	       int nVarsToUse=-1, 
	       bool keepHighlyCorrelatedVariables=false);
  
  std::vector<double> getAllVariableValuesForThisEvent(std::string finalstate, const QuaeroEvent& event, bool fullVariableList = false);
  std::vector<std::string> getAllVariableNamesForThisEvent(std::string finalstate, bool pawFormatted = true, bool fullVariableList = false, std::string colliderType = "hadron");


private:
  void identify(const std::vector<std::vector<double> >& variableValuesCommon, 
		const std::vector<double>& weightsCommon, 
		const std::vector<std::vector<double> >& variableValuesUniqueToH, 
		const std::vector<double>& weightsUniqueToH, 
		const std::vector<std::vector<double> >& variableValuesUniqueToSM, 
		const std::vector<double>& weightsUniqueToSM, 
		std::string colliderRun, std::string experiment, std::string fs, 
		int nVarsToUse=-1, bool keepHighlyCorrelatedVariables=false);

  void getAllVariables(std::string fs, 
		       const std::vector<QuaeroEvent> & events, 
		       std::vector< std::vector<double> > & variableValues, 
		       std::vector<double>& weights,
		       bool fullVariableList=false);

  std::vector<std::string> objectList;
  bool sufficientlyDifferent;
  std::vector<std::string> distributionNames;
  std::vector<int> distributions;
  string locked;

};




#endif
