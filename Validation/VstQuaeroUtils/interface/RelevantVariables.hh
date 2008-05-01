/***********************************************
RelevantVariables constructs all potentially relevant variables from 4-vector quantities
Bruce Knuteson 2003
************************************************/


#ifndef __RelevantVariables__
#define __RelevantVariables__

#include <vector>
#include <string>
#include <fstream>
#include <map>
#include "Validation/VstQuaeroUtils/interface/QuaeroEvent.hh"

class RelevantVariables
{
  /// Support streamed input
  friend std::ostream& operator<<(std::ostream & fout, const RelevantVariables & relevantVariables);

  /// Support streamed output
  friend std::istream& operator>>(std::istream & fin, RelevantVariables & relevantVariables);

public:

  /****  Constructor  ****/
  RelevantVariables(vector<double> _resonanceMasses = vector<double>(0));

  /****    Methods   ****/

  /// Return the names of all relevant variables
  std::vector<std::string> getAllVariableNamesForThisEvent(std::string finalstate, bool pawFormatted = true, bool fullVariableList = false, std::string colliderRun = "hadron");

  /// Return the values of all relevant variables
 std::vector<double> getAllVariableValuesForThisEvent(std::string finalstate, const QuaeroEvent& event, bool fullVariableList = false);

  /// Determine which variables are most relevant
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
   
  /// Set privilege level, allowing depth of access to variables
  void setPrivilegeLevel(int _privilegeLevel);

private:

  /****  Private methods  ****/

  /// Get all relevant variables
  void getAllVariables(std::string fs, 
		       const std::vector<QuaeroEvent> & events, 
		       std::vector< std::vector<double> > & variableValues, 
		       std::vector<double>& weights,
		       bool fullVariableList=false);

  /// Identify the most relevant variables
  void identify(const std::vector<std::vector<double> >& variableValuesCommon, 
		const std::vector<double>& weightsCommon, 
		const std::vector<std::vector<double> >& variableValuesUniqueToH, 
		const std::vector<double>& weightsUniqueToH, 
		const std::vector<std::vector<double> >& variableValuesUniqueToSM, 
		const std::vector<double>& weightsUniqueToSM, 
		std::string colliderRun, std::string experiment, std::string fs, 
		int nVarsToUse=-1, bool keepHighlyCorrelatedVariables=false);

  /****  Private members  ****/
  std::vector<std::string> objectList;  ///< List of all allowed final state objects
  bool sufficientlyDifferent;  ///< Are H and SM sufficiently different to be worth considering further?
  std::vector<std::string> distributionNames;  ///< Names of relevant distributions
  std::vector<int> distributions;  ///< Index of which variables are most relevant
  string locked; ///< Flag setting whether the most relevant variables are "locked", or whether they shoud be re-computed
  std::vector<double> resonanceMasses; ///< Masses of resonances in the model, in units of GeV
  int privilegeLevel; ///< Level of privilege for variables

  //Georgios
  std::map<string,double> integratedLumiMap;
  void computeIntegratedLumiMap();
  
};


#endif
