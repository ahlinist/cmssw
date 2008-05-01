#ifndef __OptimalBinning
#define __OptimalBinning

#include <vector>
#include <string>
#include "Validation/VstOptimalBinning/interface/ExpectedEvidence.hh"

namespace OptimalBinning
{
  // This is the public routine

  void chooseBinPoints(
		       std::vector<double> h, 
		       std::vector<double> hwt, 
		       std::vector<double> b, 
		       std::vector<double> bwt, 
		       std::vector<double>& binPoints, 
		       std::vector<double>& improvement,
		       double epsilonWt=0
		       );

  double minWeight(double epsilonWt);

  /*********************************************************
                Effective number of events
  **********************************************************/

  // Determine the effective number of Monte Carlo events, given vectors of weights for h and b
  double nMC(const std::vector<double>& hwt, const std::vector<double>& bwt);

  // Determine the effective weight, given a vector of weights
  double wtMC(const std::vector<double>& wt);

  // Determine the effective weight, given vectors of weights for h and b
  double wtMC(const std::vector<double>& hwt, const std::vector<double>& bwt);

  /*********************************************************
                Penalty term routines
  **********************************************************/

  // Determine the contribution to the penalty term from w
  double penaltyTermPerBin_w(double hwt, double bwt=0, double p=1);

  // Determine the contribution to the penalty term from N
  double penaltyTermPerBin_n(double wn, bool binsHaveBeenOptimizedQ = true);

  // Determine the contribution to the penalty term from k
  double penaltyTermPerBin_k(double k, double wn);

  double penaltyTermPerBin(double w, double n, double k, bool binsHaveBeenOptimizedQ = true);
  double penaltyTermPerBin(double hwt, double bwt, double p, double n, double k, bool binsHaveBeenOptimizedQ = true);

  /*********************************************************
             Calculate binning figure of merit
  **********************************************************/

  ExpectedEvidence binningFigureOfMerit_highStatisticsLimit(double h, double b, double dh, double db);

  ExpectedEvidence binningFigureOfMerit(double h, double b, double dh, double db);

  void getBinContents(const std::vector<double>& a, const std::vector<double>& awt, 
		      const std::vector<double>& binPoints, 
		      std::vector<double>& ak, std::vector<double>& dak, 
		      double epsilonWt=0);

  ExpectedEvidence  binningFigureOfMerit(const std::vector<double>& h, const std::vector<double>& b, 
					 const std::vector<double>& dh, const std::vector<double>& db, 
					 double wtMC, double nMC, 
					 bool binsHaveBeenOptimizedQ = true, bool usePenaltyFactorQ = true);

  ExpectedEvidence binningFigureOfMerit(const std::vector<double>& h, const std::vector<double>& b, 
					const std::vector<double>& hwt, const std::vector<double>& bwt, 
					const std::vector<double>& binPoints, 
					double epsilonWt=0, bool binsHaveBeenOptimizedQ = true, bool usePenaltyFactorQ = true);

  double calculateTheLikelihood(const std::vector<double>& h, const std::vector<double>& b, 
				const std::vector<double>& hwt, const std::vector<double>& bwt, 
				const std::vector<double>& d, 
				const std::vector<double>& binPoints, double epsilonWt=0);

  void chooseSingleBinPoint(const std::vector<double>& h, const std::vector<double>& hwt, 
			    const std::vector<double>& b, const std::vector<double>& bwt, 
			    double& binPoint, double& improvement, double epsilonWt=0);   // h and b must be sorted!!

  void chooseBinPoints(const std::vector<double>& h, const std::vector<double>& b, 
		       double wtMC, 
		       std::vector<double>& binPoints, std::vector<double>& improvement);
  
  void chooseBinPoints_alreadySorted(const std::vector<double>& h, const std::vector<double>& hwt, 
				     const std::vector<double>& b, const std::vector<double>& bwt, 
				     std::vector<double>& binPoints, std::vector<double>& improvement, 
				     double epsilonWt=0);

}




#endif
