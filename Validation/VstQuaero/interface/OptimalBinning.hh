#ifndef __OptimalBinning/interface
#define __OptimalBinning/interface

#include <vector>
#include <string>

namespace OptimalBinning/interface
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

  // The following are utility routines

  double nMC(const std::vector<double>& hwt, const std::vector<double>& bwt);
  double wtMC(const std::vector<double>& wt);
  double wtMC(const std::vector<double>& hwt, const std::vector<double>& bwt);
  double penaltyTermPerBin_w(double hwt, double bwt=0, double p=1);
  double penaltyTermPerBin_n(double wn, bool binsHaveBeenOptimizedQ = true);
  double penaltyTermPerBin_k(double k, double wn);
  double penaltyTermPerBin(double w, double n, double k, bool binsHaveBeenOptimizedQ = true);
  double penaltyTermPerBin(double hwt, double bwt, double p, double n, double k, bool binsHaveBeenOptimizedQ = true);
  double binningFigureOfMerit_highStatisticsLimit(double h, double b, double dh, double db);
  double binningFigureOfMerit(double h, double b, double dh, double db);
  void getBinContents(const std::vector<double>& a, const std::vector<double>& awt, const std::vector<double>& binPoints, std::vector<double>& ak, std::vector<double>& dak, double epsilonWt=0);
  double binningFigureOfMerit(const std::vector<double>& h, const std::vector<double>& b, const std::vector<double>& dh, const std::vector<double>& db, double wtMC, double nMC, bool binsHaveBeenOptimizedQ = true, bool usePenaltyFactorQ = true);
  double binningFigureOfMerit(const std::vector<double>& h, const std::vector<double>& b, const std::vector<double>& hwt, const std::vector<double>& bwt, const std::vector<double>& binPoints, double epsilonWt=0, bool binsHaveBeenOptimizedQ = true, bool usePenaltyFactorQ = true);

  double calculateTheLikelihood(const std::vector<double>& h, const std::vector<double>& b, const std::vector<double>& hwt, const std::vector<double>& bwt, const std::vector<double>& d, const std::vector<double>& binPoints, double epsilonWt=0);
void chooseSingleBinPoint(const std::vector<double>& h, const std::vector<double>& hwt, const std::vector<double>& b, const std::vector<double>& bwt, double& binPoint, double& improvement, double epsilonWt=0);   // h and b must be sorted!!
  void chooseBinPoints(const std::vector<double>& h, const std::vector<double>& b, double wtMC, std::vector<double>& binPoints, std::vector<double>& improvement);
  void chooseBinPoints_alreadySorted(const std::vector<double>& h, const std::vector<double>& hwt, const std::vector<double>& b, const std::vector<double>& bwt, std::vector<double>& binPoints, std::vector<double>& improvement, double epsilonWt=0);


}




#endif
