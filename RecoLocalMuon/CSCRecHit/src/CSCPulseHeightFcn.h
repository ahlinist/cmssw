#ifndef CSCRecHit_CSCPulseHeightFcn_h
#define CSCRecHit_CSCPulseHeightFcn_h

/** \class CSCPulseHeightFcn
 *
 * Model functional form for fitting SCA pulse height
 * information from Muon Endcap CSC's. This version
 * is for ROOT Minuit2.
 *
 */

#include "Minuit2/FCNBase.h"
#include <vector>

using namespace ROOT::Minuit2;

class CSCStripDigi;

class CSCPulseHeightFcn : public FCNBase {
public:
  ///Cache the current strip digi
  void setDigi(const CSCStripDigi * digi) { theDigi = digi; };

  /// Set the noise contribution
  void setNoise(float noise) { theNoise2 = noise*noise; };
 
  /// Provide the chi-squared function for the given data, based on the input parameters
  virtual double operator()(const std::vector<double>&) const;

  ///@@ What?
  virtual double Up() const {return 1.;}

private:
  // event data
  double theNoise2;  // noise constants for the detector
  const CSCStripDigi * theDigi;
};

#endif

