#include <RecoLocalMuon/CSCRecHit/src/CSCStripClusterFcn.h>
#include <Geometry/CSCSimAlgo/interface/CSCChamberSpecs.h>
#include <FWCore/MessageLogger/interface/MessageLogger.h>

#include <cmath>
#include <iostream>

void CSCStripClusterFcn::initChamberSpecs(const CSCChamberSpecs & chamberSpecs) {

  LogDebug("CSC") << " calling initChamberSpecs" <<  "\n";

  fitFunction_.initChamberSpecs(chamberSpecs);
}


double CSCStripClusterFcn::operator()(const std::vector<double>& x) const {
  // finds the chi-squared for this hypothesis
  double hitPosition = x[0];  // fitted center of cluster
  double q  = exp(x[1]);      // total charge
  double chisq = 0.; 

  LogDebug("CSC") << "start at " << hitPosition <<  "\n";
 
  for( size_t i = 0; i < theData.size(); ++i) {
    double distanceFromHit = theData[i].x() - hitPosition;
    double stripWidth = (i>0) ? theData[i].x() - theData[i-1].x() :
                                theData[1].x() - theData[0].x();
    double predictedQ = q * fitFunction_.binValue(distanceFromHit, stripWidth);
    double dQ = theData[i].dy(); // estimated error on measured charge
    double diffQ = theData[i].y() - predictedQ; // difference between measured and predicted charge
    chisq += (diffQ/dQ)*(diffQ/dQ);

    LogDebug("CSC") << "bin " << i << " from_cen " << distanceFromHit << 
        " meas " << theData[i].y() << " func " << predictedQ << 
        " meas_err " << dQ << " accum_chisq " << chisq <<  "\n";

  }

  return chisq; 
}
