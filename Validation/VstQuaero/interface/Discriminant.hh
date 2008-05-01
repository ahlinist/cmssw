/*  A Discriminant represents a function D(x), where x is a vector in specific variable space in a specific final state
    for a specific experiment in a specific collider run.  For example, we might have x = { 90.6, 42.3 } in the variable
    space { m_{e+e-}, e+_pt } in the final state 1e+1e- in the experiment CDF in the collider run Tevatron Run II.
    D(x) is constructed from two hypotheses, referred to as h (for "new hypothesis") and b (for "background"), following
    the notation in our OptimalBinning/interface note.  D(x) returns a real number between 0 and 1, implemented by Discriminant::compute(x).
    D(x) returns a number close to 0 if x lives in a region of variable space dominated by Monte Carlo points from b;
    D(x) returns a number close to 1 if x lives in a region of variable space dominated by Monte Carlo points from h.
*/

#include <vector>
#include <string>
#include <iostream>
#include "Validation/VstFewKDE/interface/FewKDE.hh"

#ifndef __Discriminant__
#define __Discriminant__

class GraphicalOutput;

class Discriminant
{
  // Methods for easy reading and writing
  friend std::istream &operator>>(std::istream&, Discriminant &);
  friend std::ostream &operator<<(std::ostream&, const Discriminant &);
  friend class GraphicalOutput;

public:

  /* Default constructor */
  Discriminant();

  /* Constructor with Monte Carlo points for the hypothesis h,
                      Monte Carlo points for the hypothesis b,
                      the weights of the Monte Carlo points for the hypothesis h, and
                      the weights of the Monte Carlo points for the hypothesis b */
  Discriminant(const std::vector<std::vector<double> >& hPoints, 
	       const std::vector<std::vector<double> >& bPoints, 
	       const std::vector<double>& hWt, 
	       const std::vector<double>& bWt);

  /* Constructor with a density estimate for the hypothesis h,
                      a density estimate for the hypothesis b,
                      the total (integrated) weight for the hypothesis h, and
                      the total (integrated) weight for the hypothesis b. */
  Discriminant(const FewKDE& h, const FewKDE& b, double hWt=1, double bWt=1);

  /* Method to compute the discrimant D at a particle point x in variable space */
  double compute(const std::vector<double>& x) const;

private:
  FewKDE h, b;        // density estimates for the two hypotheses h and b
  double hWt, bWt;    // the integrated weight (number of predicted events) for the two hypotheses h and b

};
							 
#endif
