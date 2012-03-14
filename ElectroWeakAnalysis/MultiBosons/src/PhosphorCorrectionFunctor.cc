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


//____________________________________________________________________________
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


//____________________________________________________________________________
/// Parse the configuration and set the values of data members accordingly.
void
PhosphorCorrectionFunctor::init()
{  
  /// Fill the scale correction factors with the meaured scale values in %.
  // correctionFactors_[kMonteCarlo][kCategory1] = 
}


//____________________________________________________________________________
/// Scale correction for photons in real data
double
PhosphorCorrectionFunctor::operator()(double energy, double eta, int runNumber)
{
  assert(mode_ == kRealData);
  return energy;
}


//____________________________________________________________________________
/// Scale correction and correlated resolution smearing for photons in
/// simulation
double
PhosphorCorrectionFunctor::operator()(double energy, double eta,
                                      double genEnergy)
{
  assert(mode_ == kMonteCarlo);
  return energy;
}

