/*********************************************************
Implementation of the SystematicShift class.

Bruce Knuteson (knuteson@mit.edu)
*********************************************************/

#include <stdexcept>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include "Validation/VstMathUtils/interface/Math.hh"
#include "Validation/VstQuaero/interface/QuaeroIO.hh"
#include "Validation/VstQuaero/interface/SystematicShift.hh"
//using namespace std;
using namespace Math;


int SystematicShift::getInternalErrorNumber(int externalErrorNumber)
{
  size_t ans = find(errorNumber.begin(), errorNumber.end(), externalErrorNumber) - errorNumber.begin();
  if(ans>=errorNumber.size())
    {
      print(errorNumber);
      std::cout << (std::string)"An unregistered systematic error source (#"+Math::ftoa(externalErrorNumber)+") is referred to." << std::endl;
      exit(1);
    }
  return(ans);
}

SystematicShift::SystematicShift()
{
  /* Read file of systematic error sources */
  int number, nCases; char type; std::string blah;
  std::ifstream fsysSources(QuaeroIO::getFilename("sysSources").c_str());

  if(!fsysSources) {
    std::cerr << "Can't open fsysSources in SystematicShift " << std::endl;
    //    throw std::runtime_error("Can't open file");
  } else {

  while(fsysSources >> number)
    {
      errorNumber.push_back(number);
      fsysSources >> blah;
      type = blah.substr(0,1).c_str()[0];
      errorType.push_back(type);
      nCases=0;
      if(type=='c')
	nCases=atoi(blah.substr(1).c_str()); 
      discreteErrNumberOfCases.push_back(nCases);
      getline(fsysSources,blah);
    }
  fsysSources.close();
  }
  size_t nSystematicErrorSources = errorNumber.size();

  /* Read file of systematic error correlations */
  std::ifstream fsysCorrelations(QuaeroIO::getFilename("sysCorrelations").c_str());

  if(!fsysCorrelations) {
    std::cerr << "Can't open fsysCorrelations in SystematicShift " << std::endl;
    //    throw std::runtime_error("Can't open file");
  } else {

  int err1, err2; double correlation;
  correlationMatrix = matrix(nSystematicErrorSources,nSystematicErrorSources);
  for(size_t i=0; i<nSystematicErrorSources; i++)
    correlationMatrix[i][i]=1;
  while(fsysCorrelations >> err1)
    {
      fsysCorrelations >> err2 >> correlation;
      if(fabs(correlation)>1)
	{
	  std::cout << (std::string)"Correlation of systematic error sources "+Math::ftoa(err1)+" and "+Math::ftoa(err2)+" exceeds unity." << std::endl;
	  exit(1);
	}
      correlationMatrix[getInternalErrorNumber(err1)][getInternalErrorNumber(err2)] = correlation;
      correlationMatrix[getInternalErrorNumber(err2)][getInternalErrorNumber(err1)] = correlation;
    }
  fsysCorrelations.close();
  }
  setDefault();
}

void SystematicShift::setDefault()
{
  systematicShift = std::vector<double>(nSystematicErrorSources,0.);
}

double SystematicShift::modifyValue(double originalValue, int nSysErr, double magnitude)
{
  double ans = originalValue;
  if(defaultQ())
    return(ans);

  if(nSysErr==0) // Monte Carlo statistics
    {
      if(drand48()<.5)
	return(2*ans);
      else
	return(0);
    }
  
  nSysErr = getInternalErrorNumber(nSysErr);
  
  if(errorType[nSysErr]=='1')
    ans += Math::gasdev(0,1)*magnitude;
  if(errorType[nSysErr]=='m')
    ans += systematicShift[nSysErr]*magnitude;
  if(errorType[nSysErr]=='c')
    if(magnitude!=systematicShift[nSysErr])
      ans = 0.;

  return(ans);
}


double SystematicShift::modifyValue(double originalValue, std::vector<int> nSysErr, std::vector<double> magnitude)
{
  assert(nSysErr.size()==magnitude.size());
  double ans = originalValue;
  for(size_t i=0; i<nSysErr.size(); i++)
    ans = modifyValue(ans, nSysErr[i], magnitude[i]);
  return(ans);
}

std::string SystematicShift::modifyValue(std::string originalValue, int nSysErr, std::string newID, double chance)
{
  std::string ans = originalValue;
  nSysErr = getInternalErrorNumber(nSysErr);
  
  if(systematicShift[nSysErr]==0)
    systematicShift[nSysErr]=0.5; // default value
  
  if(1-systematicShift[nSysErr]<chance)
    ans = newID;
  
  return(ans);
}

std::string SystematicShift::modifyValue(std::string originalValue, std::vector<int> nSysErr, std::vector<std::string> newID, std::vector<double> chance)
{
  assert(nSysErr.size()==newID.size());
  assert(nSysErr.size()==chance.size());
  std::string ans = originalValue;
  for(size_t i=0; i<nSysErr.size(); i++)
    ans = modifyValue(ans, nSysErr[i], newID[i], chance[i]);
  return(ans);  
}

void SystematicShift::reShift(std::vector<double> _systematicShift)
{
  if(_systematicShift.empty())
    {
      std::vector<double> mean = std::vector<double>(nSystematicErrorSources,0.);
      systematicShift = Math::randMultiGauss(mean,correlationMatrix);
      for(int i=0; i<nSystematicErrorSources; i++)
	{
	  if(errorType[i]=='i')
	    systematicShift[i] = Math::sigma2prob(systematicShift[i]);
	  if(errorType[i]=='c')
	    systematicShift[i] = (int)(drand48()*discreteErrNumberOfCases[i]);
	}
      std::cout << "      systematic shift = "; print(systematicShift);
      return;
    }
  else
    {
      assert(_systematicShift.size()==(size_t) nSystematicErrorSources);
      
      // convert from uniform distribution to gaussian in each shift
      for(int i=0; i<nSystematicErrorSources; i++)
	_systematicShift[i] = Math::prob2sigma(_systematicShift[i]); 
      
      matrix correlationMatrixSqrt = correlationMatrix.power(0.5);
      systematicShift = std::vector<double>(nSystematicErrorSources);
      
      // convert from uncorrelated gaussian to correlated gaussian
      for(int i=0; i<nSystematicErrorSources; i++)
	for(int j=0; j<nSystematicErrorSources; j++)
	  systematicShift[i] += correlationMatrix[i][j]*_systematicShift[j];
      
      // handle identification and choice systematic shifts
      for(int i=0; i<nSystematicErrorSources; i++)
	{
	  if(errorType[i]=='i')
	    systematicShift[i] = Math::sigma2prob(systematicShift[i]);
	  if(errorType[i]=='c')
	    systematicShift[i] = (int)(Math::sigma2prob(systematicShift[i])*discreteErrNumberOfCases[i]);
	}
      return;
    }
  return;
}

int SystematicShift::getNumberOfSystematicErrorSources()
{
  return(nSystematicErrorSources);
}


bool SystematicShift::defaultQ()
{
  return(systematicShift==std::vector<double>(nSystematicErrorSources,0.));
}
