#ifndef TauAnalysis_CandidateTools_IndepCombinatoricsGeneratorT_h
#define TauAnalysis_CandidateTools_IndepCombinatoricsGeneratorT_h

/** \class IndepCombinatoricsGeneratorT
 *
 * Auxiliary class providing functionality to iterate over all combinations of values { x_1, x_2, .., x_N }
 * where each x_i takes all values between xl_i and xh_i, in increments of size deltax_i.
 * 
 * NOTE: 
 *      (1) The IndepCombinatoricsGenerator class is templated,
 *          so that it can be used to iterate over integer (e.g. indices) 
 *          as well as floating-point (e.g. physical observables) quantities
 *
 *      (2) This class is similar to RecoTauTag/RecoTau/interface/CombinatoricGenerator.h,
 *          but simpler, since to special care is taken to avoid combinations in which two values x_i, x_j are equal
 *         (all values are taken to be independent in IndepCombinatoricsGenerator)
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: IndepCombinatoricsGeneratorT.h,v 1.1 2011/02/27 16:45:16 veelken Exp $
 *
 */

#include "FWCore/Utilities/interface/Exception.h"

#include <iostream>
#include <iomanip>

template <typename T>
class IndepCombinatoricsGeneratorT
{
 public:
  IndepCombinatoricsGeneratorT(unsigned N)
    : N_(N),
      lowerLimits_(N),
      upperLimits_(N),
      stepSizes_(N),
      currentValues_(N),
      isFirst_(true),
      isValid_(true)
  {
    for ( unsigned i = 0; i < N_; ++i ) {
      lowerLimits_[i]   = 0;
      upperLimits_[i]   = 0;
      stepSizes_[i]     = 1;
    }
  }
  ~IndepCombinatoricsGeneratorT() {}

  void setLowerLimit(unsigned idx, const T& lowerLimit) 
  {
    checkInputParameter(idx, "setLowerLimit");
    lowerLimits_[idx] = lowerLimit;
  }

  void setUpperLimit(unsigned idx, const T& upperLimit) 
  {
    checkInputParameter(idx, "setUpperLimit");
    upperLimits_[idx] = upperLimit;
  }

  void setStepSize(unsigned idx, const T& stepSize) 
  {
    checkInputParameter(idx, "setUpperLimit");
    stepSizes_[idx] = stepSize;
  }

  void reset() 
  {
    isFirst_ = true;
    isValid_ = true;
  }

  unsigned N() const { return N_; }

  bool isValid() const { return isValid_; }

  T operator[](unsigned idx) const
  {
    if ( idx >= N_ ) {
      throw cms::Exception("IndepCombinatoricsGeneratorT::operator[]")
	<< " Invalid parameter idx = " << idx << " passed as function argument, expected range = [0.." << N_ << "] !!";
    }
    return currentValues_[idx];
  }

  void next()
  {
    if ( !isValid_ ) assert(0);

    if ( isFirst_ ) {
      for ( unsigned i = 0; i < N_; ++i ) {
	currentValues_[i] = lowerLimits_[i];
      }
      isFirst_ = false;
    }

    for ( int i = (int)N_ - 1; i >= 0; --i ) {
      if ( currentValues_[i] < (upperLimits_[i] - stepSizes_[i]) ) {
	currentValues_[i] += stepSizes_[i];
	for ( int j = (i + 1); j < (int)N_; ++j ) currentValues_[j] = lowerLimits_[i];
        break;
      } else if ( i == 0 ) isValid_ = false;
    }
  }

 protected:
  void checkInputParameter(unsigned idx, const std::string& functionName)
  {
    std::string class_plus_functionName = std::string("IndepCombinatoricsGeneratorT").append("::").append(functionName);
    
    if ( !isFirst_ ) {
      throw cms::Exception(class_plus_functionName)
	<< " Function must **not** be called after starting iteration !!";
    }
    
    if ( idx >= N_ ) {
      throw cms::Exception(class_plus_functionName)
	<< " Invalid parameter idx = " << idx << " passed as function argument, expected range = [0.." << N_ << "] !!";
    }
  }

 private:
  unsigned N_;

  std::vector<T> lowerLimits_;
  std::vector<T> upperLimits_;
  std::vector<T> stepSizes_;

  std::vector<T> currentValues_;

  bool isFirst_;
  bool isValid_;
};

template <typename T>
std::ostream& operator<<(std::ostream& stream, const IndepCombinatoricsGeneratorT<T>& generator)
{
  stream << "{ ";

  unsigned N = generator.N();
  for ( unsigned i = 0; i < N; ++i ) {
    if ( i > 0 ) stream << ", ";
    stream << generator[i];
  }
  
  stream << " }";

  return stream;
}

#endif
