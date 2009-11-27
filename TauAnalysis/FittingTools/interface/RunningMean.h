#ifndef TauAnalysis_FittingTools_RunningMean_h  
#define TauAnalysis_FittingTools_RunningMean_h

/** \class RunningMean
 *
 * Compute average of a list of values,
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
 * \version $Revision: 1.2 $
 *
 * $Id: RunningMean.h,v 1.2 2009/08/07 11:49:11 veelken Exp $
 *
 */

#include <TVectorD.h>

#include <iostream>
#include <vector>
#include <string>

class RunningMean
{
 public:  
  explicit RunningMean(unsigned d);
  ~RunningMean() {}
  
  void update(const TVectorD& value);

  const TVectorD& operator()() const { return mean_; }
  double operator()(unsigned i) const { return mean_(i); }

  void print(std::ostream&, const std::vector<std::string>* = 0) const;

 private:
  TVectorD mean_;
  unsigned numVar_;
  int iValue_;
  TVectorD auxDelta_;
};

#endif  


