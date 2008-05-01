/**************************
SystematicShift implements the effect of systematic errors on all numbers in a Quaero event.
Bruce Knuteson 2003
**************************/


#ifndef __SystematicShift__
#define __SystematicShift__

#include <vector>
#include <string>
#include "Validation/VstMathUtils/interface/matrix.hh"

class SystematicShift
{
public:

  /***  Constructor  ***/

  SystematicShift();

  /***    Methods    ***/
  
  /// Set systematic shifts to the default:  zero shift
  void setDefault();

  /// Impose systematic error number <nSysErr> with magnitude <magnitude> on <originalValue>
  /// Return the resulting modified value
  double modifyValue(double originalValue, int nSysErr, double magnitude=0);

  /// Impose (several) systematic error numbers <nSysErr> with magnitudes <magnitude> on <originalValue>
  /// Return the resulting modified value
  double modifyValue(double originalValue, std::vector<int> nSysErr, std::vector<double> magnitude);

  /// Impose systematic error number <nSysErr> on <originalValue>, which has a chance <chance> of changing <originalValue> to <newID>
  /// Return the resulting string
  std::string modifyValue(std::string originalValue, int nSysErr, std::string newID, double chance);

  /// Impose (several) systematic error numbers <nSysErr> on <originalValue>, which have chances <chance> of changing <originalValue> to <newID>
  /// Return the resulting string
  std::string modifyValue(std::string originalValue, std::vector<int> nSysErr, std::vector<std::string> newID, std::vector<double> chance);

  /// Change the vector of systematic shifts
  /// Set to _systematicShift, if specified  
  /// Pull randomly from a multivariate gaussian distribution, if _systematicShift is not specified
  void reShift(std::vector<double> _systematicShift = std::vector<double>(0));

  /// Are all systematic shifts currently zero?
  bool defaultQ();

  /// Return the number of different systematic error sources
  int getNumberOfSystematicErrorSources();

private:

  /***  Private Methods  ***/
  /// Convert between external error number (specifed in Quaero's systematicSources.txt) and SystematicShift's internal error number
  int getInternalErrorNumber(int externalErrorNumber);

  /***  Members  ***/
  int nSystematicErrorSources; ///< number of systematic error sources specified in systematicSources.txt
  std::vector<int> errorNumber; ///< vector of the error codes specified in systematicSources.txt
  std::vector<char> errorType; ///< vector of the type of each systematic error source specified in systematicSources.txt
  std::vector<int> discreteErrNumberOfCases; ///< vector of the number of discrete cases, for systematic errors of type "choice" ('c')
  matrix correlationMatrix; ///< correlation matrix of systematic errors
  std::vector<double> systematicShift; ///< vector of current systematic shifts

};

#endif
