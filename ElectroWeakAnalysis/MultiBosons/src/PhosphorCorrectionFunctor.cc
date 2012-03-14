/**
 * @brief       PhosphorCorrectionFunctor class implementation.
 * @details     This is the implementation of the PhosphorCorrectionFunctor
 *              class that provides photon energy corrections based on
 *              the PHOSPHOR Fit results targeted for the Vgamma 
 *              cross-section measurement using 2011 data. See the 
 *              header file for details, 
 *              @see MultiBosons/interface/PhosphorCorrectionFunctor.h
 * @author      Jan Veverka, Caltech, veverka@caltech.edu
 * @version     0.1
 * @date        14 March 2012
 *              
 */

#include <assert.h>

#include "ElectroWeakAnalysis/MultiBosons/interface/PhosphorCorrectionFunctor.h"

using vgamma::PhosphorCorrectionFunctor;


//______________________________________________________________________________
/// ctor
PhosphorCorrectionFunctor::PhosphorCorrectionFunctor(
  Mode mode,
  const char * configuration
) : 
  mode_(mode),
  configuration_(configuration),
  correctionFactors_(kNumCategories)
{
  init();
}


//______________________________________________________________________________
/// Parse the configuration and set the values of data members accordingly.
void
PhosphorCorrectionFunctor::init()
{  
  /// Measured values of scale in (%)
  std::vector<double> scales(kNumCategories);
  scales[kMonteCarloBarrelEt12to15    ] =  0.45;
  scales[kRealData2011ABarrelEt12to15 ] = -1.16;
  scales[kRealData2011BBarrelEt12to15 ] = -1.50;

  scales[kMonteCarloBarrelEt15to20    ] =  0.29;
  scales[kRealData2011ABarrelEt15to20 ] = -0.36;
  scales[kRealData2011BBarrelEt15to20 ] = -1.00;

  scales[kMonteCarloEndcapsEt12to15   ] =  0.64;
  scales[kRealData2011AEndcapsEt12to15] = -1.80;
  scales[kRealData2011BEndcapsEt12to15] =  0.99;

  scales[kMonteCarloEndcapsEt15to25   ] =  0.47;
  scales[kRealData2011AEndcapsEt15to20] = -2.55;
  scales[kRealData2011BEndcapsEt15to20] =  0.05;

  correctionFactors_.reserve(scales.size());
  correctionFactors_.clear();
  for (vector<double>::const_iterator s = scales_->begin();
       s != scales_->end(); ++s) {
    correctionFactors_.push_back(1. / (1. + 0.01 * s));
  }

  /// Fill the scale correction factors with the meaured scale values in %.
  // correctionFactors_[kMonteCarlo][kCategory1] = 
}


//______________________________________________________________________________
/// Scale correction for photons in real data
double
PhosphorCorrectionFunctor::operator()(double pt, double eta, int runNumber)
{
  assert(mode_ == kRealData);

  // Category cat = kNumCategories;
  // if (10. <= pt && pt < 12 && fabs(eta) < 1.5 && runNumber 
  return pt;
}


//______________________________________________________________________________
/// Scale correction and correlated resolution smearing for photons in
/// simulation
double
PhosphorCorrectionFunctor::operator()(double pt, double eta, double genEnergy)
{
  assert(mode_ == kMonteCarlo);
  return pt;
}

