#ifndef TauAnalysis_FittingTools_RunningCovMatrix_h  
#define TauAnalysis_FittingTools_RunningCovMatrix_h

/** \class RunningCovMatrix
 *
 * Compute correlation between variables,
 * estimated from a list of values,
 * each value being a vector of dimension d
 *
 *    NOTE: algorithms for numerically stable computation of mean and covariance matrix 
 *          implemented in RunningMean and RunningCovMatrix classes 
 *          taken from:
 *            Ronald A. Thisted 
 *            "Elements of Statistical Computing: Numerical Computation", pp. 84-91
 *            Chapman & Hall, 1988 
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: RunningCovMatrix.h,v 1.1 2009/08/04 14:35:09 veelken Exp $
 *
 */

#include "TauAnalysis/FittingTools/interface/RunningMean.h"

#include <TMatrixD.h>
#include <TVectorD.h>

#include <iostream>
#include <vector>
#include <string>

class RunningCovMatrix
{
 public:
  explicit RunningCovMatrix(unsigned d);
  ~RunningCovMatrix() {}

  void update(const TVectorD& value);

  TMatrixD operator()() const;
  double operator()(unsigned i, unsigned j) const { return ( iValue_ > 0 ) ? cov_(i, j)/iValue_ : 0.; }
  double sigma(unsigned i) const;
  double correlation(unsigned i, unsigned j) const;
  
  void print(std::ostream&, const std::vector<std::string>* = 0) const;

 private:
  TMatrixD cov_;
  unsigned numVar_;
  int iValue_;
  TVectorD auxDelta_;
  TVectorD auxMean_;
};

#endif  


