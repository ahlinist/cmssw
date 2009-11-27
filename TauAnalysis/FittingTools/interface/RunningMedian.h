#ifndef TauAnalysis_FittingTools_RunningMedian_h  
#define TauAnalysis_FittingTools_RunningMedian_h

/** \class RunningMedian
 *
 * Compute median of a list of values;
 * in case of values being a vector of dimension d,
 * the median of the vector is computed as vector of medians for each component
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: RunningMedian.h,v 1.1 2009/08/21 13:59:35 veelken Exp $
 *
 */

#include <TVectorD.h>

#include <iostream>
#include <vector>
#include <string>

class RunningMedian
{
 public:  
  explicit RunningMedian(unsigned d);
  ~RunningMedian() {}
  
  void update(const TVectorD& value);

  const TVectorD& operator()() const;
  double operator()(unsigned i) const { return (*this)()(i); }

  void print(std::ostream&, const std::vector<std::string>* = 0) const;

 private:
  typedef std::vector<double> vComponent;
  mutable std::vector<vComponent> values_;
  unsigned numVar_;
  mutable bool isSorted_;
  mutable TVectorD auxMedian_; 
};

#endif  


