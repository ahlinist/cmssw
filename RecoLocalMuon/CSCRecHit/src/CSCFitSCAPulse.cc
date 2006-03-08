#include "Minuit2/VariableMetricMinimizer.h"
#include "Minuit2/FunctionMinimum.h"

#include <RecoLocalMuon/CSCRecHit/src/CSCFitSCAPulse.h>
#include <RecoLocalMuon/CSCRecHit/src/CSCPulseHeightFcn.h>
#include "CLHEP/Units/PhysicalConstants.h"
#include <DataFormats/CSCDigi/interface/CSCStripDigi.h>
//@@ Why do we need Specs?? To access various chamber parameters?
#include <Geometry/CSCSimAlgo/interface/CSCChamberSpecs.h>
#include <cmath>
#include <vector>

using namespace ROOT::Minuit2;

CSCFitSCAPulse::CSCFitSCAPulse() {
  theOBJfun = new CSCPulseHeightFcn();
  theFitter = new VariableMetricMinimizer();
}


CSCFitSCAPulse::~CSCFitSCAPulse() {
  delete theFitter;
  delete theOBJfun;
}


bool CSCFitSCAPulse::peakAboveBaseline(const CSCStripDigi & digi, 
                         const CSCChamberSpecs & chamberSpecs,
                         double & height, double & sigma) const {
  bool status = false;			 
 
  // save so the chi2 and baseline routines can use it			 
  double scaNoise = 0.72;   //@@ same as in CSCStripElectronicsSim
  //theCalibrationError = chamberSpecs.calibrationError();
  float shapingTime = 100.;
  float baselineNoise = chamberSpecs.stripNoise(shapingTime) *  e_SI * 1.e15;
  float noiseTerm = scaNoise*scaNoise + baselineNoise*baselineNoise;
  
  height = sigma = 0.;
  double chisq = 0.;

  std::vector<int> sca = digi.getADCCounts();

  // uses a parabolic fit to bins 3, 4, and 5.  We'll try letting the
  // width vary at first, and then try fixing it.
  double x3 = sca[3];
  double x4 = sca[4];
  double x5 = sca[5];
  double centroid = (3*x3 + 4*x4 + 5*x5)/(x3 + x4 + x5);

  std::vector<double> par(3,0);
  std::vector<double> err(3,0);

  par[0] = centroid; // sample time
  par[1] = x4;       // height
  par[2] = 15.;      // width
  err[0] = 0.01;
  err[1] = 1.;
  err[2] = 1.;

  theOBJfun->setDigi(&digi);
  theOBJfun->setNoise(noiseTerm);

  FunctionMinimum fmin = theFitter->Minimize(*theOBJfun, par, err, 1, 500, 0.1);

  status = fmin.IsValid();

  //  MinimumState minst = fmin.states().back(); // STL back()
  //  MinimumState minst = fmin.state();
  //  chisq = minst.fval();
  //  height = minst.vec()(1);
  //  sigma = sqrt(minst.error().matrix()(1,1));

  // Minuit now has extended interface which avoids use of MinimumState as done above
  chisq  = fmin.Fval();
  height = fmin.Parameters().Vec()(1);
  sigma  = sqrt( fmin.Error().Matrix()(1,1) );

  sigma = std::max(sigma, 0.5); // prevent zero
 
// Finally, correct for baseline...
  height -= baseline(digi);

//  float easyHeight = sca[4] - baseline(digi);
//  cout << "MEPHF: " << digi.getAdcCounts() << " " << easyHeight
//                       << " " << height << endl;
  return status;
}

