#include <RecoLocalMuon/CSCRecHit/src/CSCPulseHeightFcn.h>
#include <DataFormats/CSCDigi/interface/CSCStripDigi.h>
#include <FWCore/MessageLogger/interface/MessageLogger.h>
#include <cmath>
#include <iostream>
#include <vector>

template <typename T>
inline T sqr(const T& t) {return t*t;}

double CSCPulseHeightFcn::operator()(const std::vector<double>& x) const {
  // fits to an inverted parabola

  std::vector<int> sca = theDigi->getADCCounts();

  double chi2 = 0.;
  for(int i = 3; i <= 5; ++i) {
	
    // take the zero of time to be SCA bin 4 //@@ Surely this means 'assume peak is in bin 4'?
    double diff = x[1] - x[2] * sqr(i-x[0]) - sca[i];
    chi2 += diff*diff / theNoise2;

    LogDebug("CSC") << "fit " << i << " " << 
         x[0] << " " << x[1] << "  "  << x[2] << " " <<
        sca[i] << ":" << x[1] - x[2] * pow(i-x[0], 2) <<
         " chisq " << chi2 <<  "\n";
  }

  return chi2;
}
