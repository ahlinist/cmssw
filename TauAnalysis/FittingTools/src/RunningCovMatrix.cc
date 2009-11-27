#include "TauAnalysis/FittingTools/interface/RunningCovMatrix.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <TMath.h>

#include <iomanip>

RunningCovMatrix::RunningCovMatrix(unsigned d)
  : cov_(d, d), auxDelta_(d), auxMean_(d)
{
  //std::cout << "<RunningCovMatrix::RunningCovMatrix>:" << std::endl;

  numVar_ = d;
  //std::cout << " numVar = " << numVar_ << std::endl;

  iValue_ = 0;
  //std::cout << " iValue = " << iValue_ << std::endl;
}

void RunningCovMatrix::update(const TVectorD& value)
{
  //std::cout << "<RunningCovMatrix::update>:" << std::endl;
  //std::cout << " iValue = " << iValue_ << std::endl;

  if ( value.GetNoElements() != (int)numVar_ ) {
    edm::LogError("RunningCovMatrix::update") << "Given value has invalid dimension = " << value.GetNoElements() << "," 
					      << " expected = " << numVar_ << " --> mean value will NOT be updated !!";
    return;
  }

  if ( iValue_ == 0 ) {
    for ( unsigned iRow = 0; iRow < numVar_; ++iRow ) {
      for ( unsigned iColumn = 0; iColumn < numVar_; ++iColumn ) {
	cov_(iRow, iColumn) = 0.;
      }
    }

    for ( unsigned iVar = 0; iVar < numVar_; ++iVar ) {
      auxMean_(iVar) = value(iVar);
    }
  } else {
    double weight = iValue_/(iValue_ + 1.);

    for ( unsigned iVar = 0; iVar < numVar_; ++iVar ) {
      auxDelta_(iVar) = value(iVar) - auxMean_(iVar);
      //std::cout << " auxDelta(iVar = " << iVar << ") = " << auxDelta_(iVar) << std::endl;
    }
    
    for ( unsigned iRow = 0; iRow < numVar_; ++iRow ) {
      for ( unsigned iColumn = 0; iColumn < numVar_; ++iColumn ) {
	cov_(iRow, iColumn) += weight*auxDelta_(iRow)*auxDelta_(iColumn);
	//std::cout << "cov(iRow = " << iRow << ", iColumn = " << iColumn << ") = " << cov_(iRow, iColumn) << std::endl;
      }
    }
    
    for ( unsigned iVar = 0; iVar < numVar_; ++iVar ) {
      auxMean_(iVar) += auxDelta_(iVar)/(iValue_ + 1.);
      //std::cout << " auxMean(iVar = " << iVar << ") = " << auxMean_(iVar) << std::endl;
    }
  }

  ++iValue_;
}

TMatrixD RunningCovMatrix::operator()() const
{
//--- normalize covariance matrix
//    to number of values for which it has been computer

  //std::cout << "<RunningCovMatrix::operator()>:" << std::endl;

  TMatrixD cov_normalized(numVar_, numVar_);

  if ( iValue_ > 0 ) {
    for ( unsigned iRow = 0; iRow < numVar_; ++iRow ) {
      for ( unsigned iColumn = 0; iColumn < numVar_; ++iColumn ) {
	cov_normalized(iRow, iColumn) = cov_(iRow, iColumn)/iValue_;
	//std::cout << "cov_normalized(iRow = " << iRow << ", iColumn = " << iColumn << ")" 
	//	    << " = " << cov_normalized(iRow, iColumn) << std::endl;
      }
    }
  }
  
  return cov_normalized;
}

double RunningCovMatrix::sigma(unsigned i) const
{
  if ( i >= 0 && i < numVar_ ) {
    if ( iValue_ > 0 ) return TMath::Sqrt(cov_(i, i)/iValue_);
  } else {
    edm::LogError("RunningCovMatrix::sigma") << "Given index i = " << i << " out of bounds,"
					     << " expected range = 0.." << (numVar_ - 1) << " !!";
  }
  
  return 0.;
}

double RunningCovMatrix::correlation(unsigned i, unsigned j) const
{
  if ( i >= 0 && i < numVar_ &&
       j >= 0 && j < numVar_ ) {
    double sigmaI = TMath::Sqrt(cov_(i, i));
    double sigmaJ = TMath::Sqrt(cov_(j, j));
    return cov_(i,j)/(sigmaI*sigmaJ);
  } else {
    edm::LogError("RunningCovMatrix::correlation") << "Given indices i = " << i << ", j = " << j << " out of bounds,"
						   << " expected range = 0.." << (numVar_ - 1) << " !!";
    return 0.;
  }
}

void RunningCovMatrix::print(std::ostream& outputStream, const std::vector<std::string>* varNames) const
{
  if ( varNames && varNames->size() != numVar_ ) {
    edm::LogError("RunningCovMatrix::print") << "Given list of varable names of invalid size,"
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
    outputStream << varNames_temp[iVar] << ": mean = " << auxMean_(iVar) << ", sigma = " << sigma(iVar) << std::endl;
  }
  
  outputStream << "Covariance Matrix:" << std::endl;
  const unsigned widthColumn = 20;
  outputStream << std::setw(widthColumn) << std::left << " ";
  for ( unsigned iVar = 0; iVar < numVar_; ++iVar ) {
    outputStream << std::setw(widthColumn) << std::left << varNames_temp[iVar];
  }
  outputStream << std::endl;
  for ( unsigned iCharacter = 0; iCharacter < (widthColumn*(numVar_ + 1)); ++iCharacter ) {
    outputStream << "-";
  }
  outputStream << std::endl;
  TMatrixD cov_normalized = this->operator()();
  for ( unsigned iRow = 0; iRow < numVar_; ++iRow ) {
    outputStream << std::setw(widthColumn) << std::left << varNames_temp[iRow];
    for ( unsigned iColumn = 0; iColumn < numVar_; ++iColumn ) {
      outputStream << std::setw(4) << std::left << " ";
      outputStream << std::setw(widthColumn - 4) << std::setprecision(3) << std::fixed << std::left << cov_normalized(iRow, iColumn);
    }
    outputStream << std::endl;
  }
  for ( unsigned iCharacter = 0; iCharacter < (widthColumn*(numVar_ + 1)); ++iCharacter ) {
    outputStream << "-";
  }

  outputStream << std::endl;  
} 
