/*   A Discriminator 
 */

#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include "Validation/VstQuaero/interface/Discriminant.hh"
#include "Validation/VstOptimalBinning/interface/ExpectedEvidence.hh"


#ifndef __Discriminator__
#define __Discriminator__

class GraphicalOutput;

class Discriminator
{
  // Routines to allow easy reading, writing, and drawing
  friend std::istream &operator>>(std::istream&, Discriminator &);
  friend std::ostream &operator<<(std::ostream&, Discriminator &);
  friend class GraphicalOutput;

public:
  Discriminator(double epsilonWt=0);
  void addDiscriminant(std::string _tag, const Discriminant& discriminant);
  void primePredictedD(std::string _tag, 		
		       const std::vector<std::vector<double> >& hPoints, 
		       const std::vector<std::vector<double> >& bPoints, 
		       const std::vector<double>& hWt, 
		       const std::vector<double>& bWt);
  std::vector<std::vector<double> > addToPredictedD(std::string _tag, 		
						    const std::vector<std::vector<double> >& hPoints, 
						    const std::vector<std::vector<double> >& bPoints, 
						    const std::vector<double>& hWt, 
						    const std::vector<double>& bWt);
  std::vector<double> addToPredictedD(std::string _tag, 
				      std::vector<std::vector<double> >& data);

  // Compute best bin edges in D, in the unit interval [0,1]
  void computeBinEdges();

  // Clear the histogram of D, but remember the positions of the bin edges
  void clearBinContents();

  // Compute the log likelihood ratio, given the D histograms for h, b, and the data D
  double computeLogLikelihoodRatio(bool verbose=true);

  // Compute the individual likelihoods p(D|h) and p(D|b)
  void computeLogLikelihoods(double& hLogLikelihood, 
			     double& bLogLikelihood,
			     bool verbose=true);

  // Return the expected evidence, given the D histograms for h and b
  double getExpectedEvidence(std::string type="");
  
private:
  void setLiveBins();
  void fillDbins();
  double computeRelevantLogLikelihoods(double& hLogLikelihood, 
				       double& bLogLikelihood,
				       bool verbose=true);
  std::map<std::string, Discriminant> discriminants;            // a map of (final state, Discriminant for this final state)
  std::vector<double> hD, bD;                                   // vector of discriminant values for Monte Carlo points from hypotheses h and b
                                                                // hD and bD are the union of Monte Carlo points from all final states in a specific experiment
  std::vector<double> hWt, bWt;                                 // weights of Monte Carlo points stored in hD and bD.  assert((hWt.size()==hD.size())&&(bWt.size()==bD.size()));
  std::vector<double> binPoints;                                // vector of the bin edges in D, within the unit interval [0,1]
  std::vector<std::vector<double> > hwtk, bwtk;                 
  std::vector<double> hk, bk, hkerr, bkerr;
  std::vector<int> dk;
  std::vector<bool> liveBins;                                   // vector specifying which bins are live.  The i^th bin is live if liveBins[i]==true
  int nBins;                                                    // number of bins in the discriminant D
  double epsilonWt;

};

#endif
