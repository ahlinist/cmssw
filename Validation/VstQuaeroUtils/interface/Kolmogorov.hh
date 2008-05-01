/************************************************
The Kolmogorov class implements the Kolmogorov-Smirnov (KS) statistics
Bruce Knuteson 2003
************************************************/

#include <vector>
#include <string>

class Kolmogorov
{
public:
  Kolmogorov(const std::vector<double>& _bkgpts, const std::vector<double>& _bkgwt, 
	     const std::vector<double>& _datapts, const std::vector<double> datawt=std::vector<double>(0));

  Kolmogorov(const std::vector<double>& _bkgpts, const std::vector<double>& _bkgwt); //use pseudo-data

  void  printBkgAndData();//for debugging of pseudo-data
  double probability();
  double deviation();
  static double probability(double largestDeviation, double N);
  static void identifyRelevantVariables(std::vector<size_t>& whichVariables, std::vector<double>& similarity, 
				 const std::vector<std::vector<double> > & h1, const std::vector<std::vector<double> >& h2, 
				 std::vector<double> h1wt=std::vector<double>(0), std::vector<double> h2wt=std::vector<double>(0), 
				 int nVarsToUse=-1, bool keepHighlyCorrelatedVariables=false);

private:
  std::vector<double> producePseudoData(const std::vector<double>& _bkgpts, const std::vector<double>& _bkgwt); //taken from BumpStatistic.cc
  
  std::vector<double> bkgpts;
  std::vector<double> bkgwt;
  std::vector<double> datapts;
  std::vector<double> datawt;
};

/*
double kolmogorovProbability(const std::vector<double>& bkgpts, const std::vector<double>& bkgwt, const std::vector<double>& data, const std::vector<double>& datawt=std::vector<double>(0));

double kolmogorovProbability(double largestDeviation, int N);

double kolmogorovDeviation(const std::vector<double>& bkgpts, std::vector<double> bkgwt, const std::vector<double>& data, std::vector<double> datawt=std::vector<double>(0));

void identifyRelevantVariables(std::vector<size_t>& whichVariables, std::vector<double>& similarity, const std::vector<std::vector<double> > & h1, const std::vector<std::vector<double> >& h2, std::vector<double> h1wt=std::vector<double>(0), std::vector<double> h2wt=std::vector<double>(0), int nVarsToUse=-1, bool keepHighlyCorrelatedVariables=false);
*/
