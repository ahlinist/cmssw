#include "TauAnalysis/FittingTools/interface/RunningMean.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

RunningMean::RunningMean(unsigned d)
  : mean_(d), auxDelta_(d)
{
  numVar_ = d;

  iValue_ = 0;
}

void RunningMean::update(const TVectorD& value)
{
  if ( value.GetNoElements() != (int)numVar_ ) {
    edm::LogError("RunningMean::update") 
      << "Given value has invalid dimension = " << value.GetNoElements() << "," 
      << " expected = " << numVar_ << " --> mean value will NOT be updated !!";
    return;
  }

  if ( iValue_ == 0 ) {
    for ( unsigned iVar = 0; iVar < numVar_; ++iVar ) {
      mean_(iVar) = value(iVar);
    }
  } else {
    for ( unsigned iVar = 0; iVar < numVar_; ++iVar ) {
      auxDelta_(iVar) = value(iVar) - mean_(iVar);
      mean_(iVar) += auxDelta_(iVar)/(iValue_ + 1.);
    }
  }

  ++iValue_;
}

void RunningMean::print(std::ostream& outputStream, const std::vector<std::string>* varNames) const
{
  if ( varNames && varNames->size() != numVar_ ) {
    edm::LogError("RunningMean::print") 
      << "Given list of varable names of invalid size,"
      << " expected lenght = " << numVar_ << " !!";
    return;
  }

  std::vector<std::string> varNames_temp;
  for ( unsigned iVar = 0; iVar < numVar_; ++iVar ) {
    if ( varNames ) {
      varNames_temp.push_back(varNames->at(iVar));
    } else {
      std::ostringstream varName;
      varName << "var_" << iVar;
      varNames_temp.push_back(varName.str());
    }
  }

  for ( unsigned iVar = 0; iVar < numVar_; ++iVar ) {
    outputStream << varNames_temp[iVar] << ": mean = " << mean_(iVar) << std::endl;
  }

  outputStream << std::endl;  
} 



