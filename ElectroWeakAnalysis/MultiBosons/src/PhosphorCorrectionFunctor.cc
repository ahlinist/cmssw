/**
 * @brief       PhosphorCorrectionFunctor class implementation.
 * @details     This is the implementation of the PhosphorCorrectionFunctor
 *              class that provides photon energy corrections based on
 *              the PHOSPHOR Fit results targeted for the Vgamma 
 *              cross-section measurement using 2011 data. See the 
 *              header file for details, 
 *              @see ElectroWeakAnalysis/MultiBosons/interface/PhosphorCorrectionFunctor.h
 * @author      Jan Veverka, Caltech, veverka@caltech.edu
 * @version     0.4
 * @date        14 March 2012 - 15 April 2012
 *              
 */

#include <assert.h>
#include <math.h>
#include <vector>

#include "ElectroWeakAnalysis/MultiBosons/interface/PhosphorCorrectionFunctor.h"

// using vgamma::PhosphorCorrectionFunctor;
using namespace vgamma;

/// Make this a ROOT class
#if !defined(__CINT__)
  ClassImp(vgamma::PhosphorCorrectionFunctor)
#endif 


//______________________________________________________________________________
/// Default ctor for ROOT
PhosphorCorrectionFunctor::PhosphorCorrectionFunctor() :
  mode_(kMonteCarlo2011A), configuration_(""), scales_(), correctionFactors_(),
  alphas_()
{} // PhosphorCorrectionFunctor::PhosphorCorrectionFunctor()
  

//______________________________________________________________________________
/// Sandard ctor
PhosphorCorrectionFunctor::PhosphorCorrectionFunctor(
  Mode mode,
  const char * config
) : 
  mode_(mode),
  configuration_(config),
  scales_(kNumCategories),
  correctionFactors_(kNumCategories),
  alphas_(kNumCategories, 1.)
{
  init();
} // PhosphorCorrectionFunctor::PhosphorCorrectionFunctor(mode, config)


//______________________________________________________________________________
/// Parse the configuration and set the values of data members accordingly.
void
PhosphorCorrectionFunctor::init()
{  
  /// Measured values of scale in (%)
  std::vector<double> scaleMeasurements(kNumCategories);
    
  /// No correction for all the other photons for now.
  scaleMeasurements[kUnspecified                      ] =  0.00;

  scaleMeasurements[kMonteCarlo2011ABarrelEt10to12    ] =   1.46;
  scaleMeasurements[kMonteCarlo2011BBarrelEt10to12    ] =   1.39;
  scaleMeasurements[kMonteCarlo2011allBarrelEt10to12  ] =   1.50;
  scaleMeasurements[kRealData2011ABarrelEt10to12      ] =  -0.65;
  scaleMeasurements[kRealData2011BBarrelEt10to12      ] =  -2.94;
  scaleMeasurements[kRealData2011allBarrelEt10to12    ] =  -1.40;
  
  scaleMeasurements[kMonteCarlo2011ABarrelEt12to15    ] =   0.86;
  scaleMeasurements[kMonteCarlo2011BBarrelEt12to15    ] =   0.88;
  scaleMeasurements[kMonteCarlo2011allBarrelEt12to15  ] =   0.79;
  scaleMeasurements[kRealData2011ABarrelEt12to15      ] =  -1.13;
  scaleMeasurements[kRealData2011BBarrelEt12to15      ] =  -2.26;
  scaleMeasurements[kRealData2011allBarrelEt12to15    ] =  -1.85;
  
  scaleMeasurements[kMonteCarlo2011ABarrelEt15to20    ] =   0.41;
  scaleMeasurements[kMonteCarlo2011BBarrelEt15to20    ] =   0.79;
  scaleMeasurements[kMonteCarlo2011allBarrelEt15to20  ] =   0.69;
  scaleMeasurements[kRealData2011ABarrelEt15to20      ] =  -0.87;
  scaleMeasurements[kRealData2011BBarrelEt15to20      ] =  -1.61;
  scaleMeasurements[kRealData2011allBarrelEt15to20    ] =  -1.22;

  scaleMeasurements[kMonteCarlo2011ABarrelEt20to999   ] =   0.17;
  scaleMeasurements[kMonteCarlo2011BBarrelEt20to999   ] =  -0.06;
  scaleMeasurements[kMonteCarlo2011allBarrelEt20to999 ] =   0.04;
  scaleMeasurements[kRealData2011ABarrelEt20to999     ] =  -0.58;
  scaleMeasurements[kRealData2011BBarrelEt20to999     ] =  -1.75;
  scaleMeasurements[kRealData2011allBarrelEt20to999   ] =  -1.22;
  
  scaleMeasurements[kMonteCarlo2011AEndcapsEt10to12   ] =   1.43;
  scaleMeasurements[kMonteCarlo2011BEndcapsEt10to12   ] =   3.54;
  scaleMeasurements[kMonteCarlo2011allEndcapsEt10to12 ] =   2.89;
  scaleMeasurements[kRealData2011AEndcapsEt10to12     ] =  -3.22;
  scaleMeasurements[kRealData2011BEndcapsEt10to12     ] =  -1.62;
  scaleMeasurements[kRealData2011allEndcapsEt10to12   ] =  -1.84;
  
  scaleMeasurements[kMonteCarlo2011AEndcapsEt12to15   ] =   0.23;
  scaleMeasurements[kMonteCarlo2011BEndcapsEt12to15   ] =   0.54;
  scaleMeasurements[kMonteCarlo2011allEndcapsEt12to15 ] =   0.34;
  scaleMeasurements[kRealData2011AEndcapsEt12to15     ] =  -1.71;
  scaleMeasurements[kRealData2011BEndcapsEt12to15     ] =  -0.23;
  scaleMeasurements[kRealData2011allEndcapsEt12to15   ] =  -0.87;
  
  scaleMeasurements[kMonteCarlo2011AEndcapsEt15to20   ] =   0.07;
  scaleMeasurements[kMonteCarlo2011BEndcapsEt15to20   ] =   0.69;
  scaleMeasurements[kMonteCarlo2011allEndcapsEt15to20 ] =   0.47;
  scaleMeasurements[kRealData2011AEndcapsEt15to20     ] =  -2.97;
  scaleMeasurements[kRealData2011BEndcapsEt15to20     ] =  -0.73;
  scaleMeasurements[kRealData2011allEndcapsEt15to20   ] =  -1.68;
  
  scaleMeasurements[kMonteCarlo2011AEndcapsEt20to999  ] =   0.53;
  scaleMeasurements[kMonteCarlo2011BEndcapsEt20to999  ] =   0.79;
  scaleMeasurements[kMonteCarlo2011allEndcapsEt20to999] =   0.64;
  scaleMeasurements[kRealData2011AEndcapsEt20to999    ] =  -1.36;
  scaleMeasurements[kRealData2011BEndcapsEt20to999    ] =   0.70;
  scaleMeasurements[kRealData2011allEndcapsEt20to999  ] =  -0.15;
  
  /// Measured values of resolution in (%)
  std::vector<double> resolutionMeasurements(kNumCategories);
  
  /// No correction for all the other photons for now.
  resolutionMeasurements[kUnspecified                      ] =  5.00; 

  resolutionMeasurements[kMonteCarlo2011ABarrelEt10to12    ] =   5.81;
  resolutionMeasurements[kMonteCarlo2011BBarrelEt10to12    ] =   6.44;
  resolutionMeasurements[kMonteCarlo2011allBarrelEt10to12  ] =   6.16;
  resolutionMeasurements[kRealData2011ABarrelEt10to12      ] =   7.03;
  resolutionMeasurements[kRealData2011BBarrelEt10to12      ] =   9.40;
  resolutionMeasurements[kRealData2011allBarrelEt10to12    ] =   7.57;
  
  resolutionMeasurements[kMonteCarlo2011ABarrelEt12to15    ] =   4.69;
  resolutionMeasurements[kMonteCarlo2011BBarrelEt12to15    ] =   5.37;
  resolutionMeasurements[kMonteCarlo2011allBarrelEt12to15  ] =   5.06;
  resolutionMeasurements[kRealData2011ABarrelEt12to15      ] =   6.64;
  resolutionMeasurements[kRealData2011BBarrelEt12to15      ] =   6.64;
  resolutionMeasurements[kRealData2011allBarrelEt12to15    ] =   6.63;
  
  resolutionMeasurements[kMonteCarlo2011ABarrelEt15to20    ] =   4.16;
  resolutionMeasurements[kMonteCarlo2011BBarrelEt15to20    ] =   4.31;
  resolutionMeasurements[kMonteCarlo2011allBarrelEt15to20  ] =   4.26;
  resolutionMeasurements[kRealData2011ABarrelEt15to20      ] =   4.68;
  resolutionMeasurements[kRealData2011BBarrelEt15to20      ] =   5.39;
  resolutionMeasurements[kRealData2011allBarrelEt15to20    ] =   5.15;
  
  resolutionMeasurements[kMonteCarlo2011ABarrelEt20to999   ] =   3.18;
  resolutionMeasurements[kMonteCarlo2011BBarrelEt20to999   ] =   3.42;
  resolutionMeasurements[kMonteCarlo2011allBarrelEt20to999 ] =   3.32;
  resolutionMeasurements[kRealData2011ABarrelEt20to999     ] =   3.29;
  resolutionMeasurements[kRealData2011BBarrelEt20to999     ] =   3.99;
  resolutionMeasurements[kRealData2011allBarrelEt20to999   ] =   3.71;
  
  resolutionMeasurements[kMonteCarlo2011AEndcapsEt10to12   ] =   9.91;
  resolutionMeasurements[kMonteCarlo2011BEndcapsEt10to12   ] =   8.22;
  resolutionMeasurements[kMonteCarlo2011allEndcapsEt10to12 ] =   8.94;
  resolutionMeasurements[kRealData2011AEndcapsEt10to12     ] =  15.00;
  resolutionMeasurements[kRealData2011BEndcapsEt10to12     ] =  12.45;
  resolutionMeasurements[kRealData2011allEndcapsEt10to12   ] =  12.81;
  
  resolutionMeasurements[kMonteCarlo2011AEndcapsEt12to15   ] =   8.29;
  resolutionMeasurements[kMonteCarlo2011BEndcapsEt12to15   ] =   9.16;
  resolutionMeasurements[kMonteCarlo2011allEndcapsEt12to15 ] =   8.76;
  resolutionMeasurements[kRealData2011AEndcapsEt12to15     ] =   9.43;
  resolutionMeasurements[kRealData2011BEndcapsEt12to15     ] =  11.80;
  resolutionMeasurements[kRealData2011allEndcapsEt12to15   ] =  10.54;
  
  resolutionMeasurements[kMonteCarlo2011AEndcapsEt15to20   ] =   7.54;
  resolutionMeasurements[kMonteCarlo2011BEndcapsEt15to20   ] =   8.06;
  resolutionMeasurements[kMonteCarlo2011allEndcapsEt15to20 ] =   7.87;
  resolutionMeasurements[kRealData2011AEndcapsEt15to20     ] =   9.38;
  resolutionMeasurements[kRealData2011BEndcapsEt15to20     ] =  11.39;
  resolutionMeasurements[kRealData2011allEndcapsEt15to20   ] =  10.32;
  
  resolutionMeasurements[kMonteCarlo2011AEndcapsEt20to999  ] =   5.26;
  resolutionMeasurements[kMonteCarlo2011BEndcapsEt20to999  ] =   5.83;
  resolutionMeasurements[kMonteCarlo2011allEndcapsEt20to999] =   5.59;
  resolutionMeasurements[kRealData2011AEndcapsEt20to999    ] =   8.17;
  resolutionMeasurements[kRealData2011BEndcapsEt20to999    ] =   8.32;
  resolutionMeasurements[kRealData2011allEndcapsEt20to999  ] =   8.33;
  
  /// Calculate scales in units of 1 and correction factors.
  for (unsigned i=0; i < scaleMeasurements.size(); ++i) {
    /// 0.01 accounts for the %
    scales_[i] = 0.01 * scaleMeasurements[i];
    correctionFactors_[i] = (1. / (1. + scales_[i]));
  }

  if (mode_ == kMonteCarloSmearedTo2011A || 
      mode_ == kMonteCarloSmearedTo2011B ||
      mode_ == kMonteCarloSmearedTo2011all) {
    /// Initialize the ratios of data / MC resolutions.
    Category firstDataCat = kRealData2011ABarrelEt10to12;
    Category firstMCCat = kMonteCarlo2011ABarrelEt10to12;
    Category lastMCCat  = kMonteCarlo2011allEndcapsEt20to999;
    unsigned numMCCats = lastMCCat - firstMCCat + 1;
    for (unsigned i=0; i < numMCCats; ++i) {
      double dataResolution = resolutionMeasurements[firstDataCat + i];
      double mcResolution   = resolutionMeasurements[firstMCCat + i];
      alphas_[firstMCCat + i] = dataResolution / mcResolution;
    }
  }  
} /// PhosphorCorrectionFunctor::init()


//______________________________________________________________________________
/// Scale correction for photons in real data
double
PhosphorCorrectionFunctor::operator()(double pt, double eta, int runNumber)
{
  assert(mode_ == kRealData ||
	 mode_ == kRealDataAverage2011all);

  /// Category of this photon.
  /// kUnspecified means none of the other categories; no correction.
  unsigned cat = categorize(pt, eta, runNumber);

  /// Return the corrected pt.
  return pt * correctionFactors_[cat];
} /// PhosphorCorrectionFunctor::operator()(...)


//______________________________________________________________________________
/// Correction for photons in simulation
double
PhosphorCorrectionFunctor::operator()(double pt, double eta, double genEnergy)
{
  assert(mode_ == kMonteCarlo2011A ||
	 mode_ == kMonteCarlo2011B ||
	 mode_ == kMonteCarlo2011all ||
	 mode_ == kMonteCarloSmearedTo2011A ||
	 mode_ == kMonteCarloSmearedTo2011B ||
	 mode_ == kMonteCarloSmearedTo2011all);

  double correctedPt = pt;
  switch(mode_) {
  case kMonteCarlo2011A: 
  case kMonteCarlo2011B: 
  case kMonteCarlo2011all: 
    correctedPt = correctedMCPt(pt, eta);
    break;
  case kMonteCarloSmearedTo2011A: 
  case kMonteCarloSmearedTo2011B:
  case kMonteCarloSmearedTo2011all:
    /// Was the gen-level match found and a valid genEnergy value is available?
    if (genEnergy > 0.) {
      /// Yes, we have a valid genEnergy value and can do the MC smearing.
      correctedPt = correctedAndSmearedMCPt(pt, eta, genEnergy);
    } else {
      /// No, we don't have a valid genEnvergy value and have to only do
      /// the MC scaling.
      correctedPt = correctedMCPt(pt, eta);
    }
    break;
  default:
    /// No correction is applied.  This should never happen.
    correctedPt = pt;
  }
  return correctedPt;
} /// PhosphorCorrectionFunctor::operator()(...)


//______________________________________________________________________________
/// Scale correction for photons in simulation
double 
PhosphorCorrectionFunctor::correctedMCPt(double pt, double eta) 
{
  /// Category of this photon.
  unsigned cat = categorize(pt, eta);

  /// Return the corrected pt.
  return pt * correctionFactors_[cat];
} /// PhosphorCorrectionFunctor::correctedMCPt(...)


//______________________________________________________________________________
/// Scale correction and resolution smearing for photons in simulation to
/// match the resolution in data.
double 
PhosphorCorrectionFunctor::correctedAndSmearedMCPt(double pt, double eta,
						   double genEnergy) 
{
  /// Category of this photon.
  /// kUnspecified means none of the other categories; no correction.
  unsigned cat = categorize(pt, eta);

  /// Return the corrected pt.
  /// This formula comes from the equation
  /// (x-m)/s = (x'-m')/s'  [1]
  /// where
  /// x = E/Egen is the photon energy response,
  /// m is the photon energy scale (mode of the PDF of x),
  /// s is the photon energy resolution (effective sigma of the PDF of x), and
  /// the prime (') denotes the corrected and smeared values, such that
  /// x' = E'/Egen with E' being the corrected and smeared energy
  /// s' is the smeared resolution, i.e. the resolution in data
  /// m' = 0 is the corrected scale.
  /// Alpha is a shorthand for the ratio of the resolutions,
  /// alpha = s' / s.  
  /// Expressing E and E' in terms of the transverse momenta 
  /// E = pt * cosh(eta) and E' = pt' * cosh(eta) and solving for
  /// for the corrected pt denoted pt' gives the result below.
  /// The notation in the code is a little different:
  /// alphas_[cat] is alpha = s'/s,
  /// pt is pt, the uncorrected photon transverse energy,
  /// scales_[cat] is m,
  /// genEnergy is Egen, and
  /// eta is eta.
  double genEnergyFactor = (1. - alphas_[cat] * (1 + scales_[cat])) / cosh(eta);
  return alphas_[cat] * pt + genEnergyFactor * genEnergy;
} /// PhosphorCorrectionFunctor::correctedAndSmearedMCPt 


//______________________________________________________________________________
/// Returns the category of a photon.
unsigned
PhosphorCorrectionFunctor::categorize(double pt, double eta, int runNumber)
{
  
  unsigned cat = kUnspecified;
  /// This algorithm relies heavily on the order of the categories in the 
  /// enum definition.
  const Category kEndcapsOffset  = kMonteCarlo2011AEndcapsEt10to12;
  const Category k2011BOffset    = kMonteCarlo2011BBarrelEt10to12;
  const Category k2011allOffset  = kMonteCarlo2011allBarrelEt10to12;
  const Category kRealDataOffset = kRealData2011ABarrelEt10to12;

  /// Find in what category is this particular photon.
  if (             pt < 10.) return kUnspecified;
  else if (10. <= pt && pt < 12.) cat = kMonteCarlo2011ABarrelEt10to12;
  else if (12. <= pt && pt < 15.) cat = kMonteCarlo2011ABarrelEt12to15;
  else if (15. <= pt && pt < 20.) cat = kMonteCarlo2011ABarrelEt15to20;
  else if (20. <= pt            ) cat = kMonteCarlo2011ABarrelEt20to999;
  
  if (fabs(eta) >= 1.5) {
    /// This means Endcaps, add the corresponding offset.
    cat = cat + kEndcapsOffset;
  }

  /// Add a category offset based on the data-taking period:
  switch (mode_){
  case kMonteCarlo2011A:
  case kMonteCarloSmearedTo2011A:
    /// No need to change anything for 2011A
    break;
  case kMonteCarlo2011B:
  case kMonteCarloSmearedTo2011B:
    /// Add an offset to change 2011A to 2011B:
    cat = cat + k2011BOffset;
    break;
  case kMonteCarlo2011all:
  case kMonteCarloSmearedTo2011all:
    /// Add an offset to change 2011A to 2011B:
    cat = cat + k2011allOffset;
    break;
  case kRealData:
    /// Add an offset to change MonteCarlo to RealData
    cat = cat + kRealDataOffset;
    if (runNumber > kLastRun2011A)
      /// Add an offset to change 2011A to 2011B
      cat = cat + k2011BOffset;
    break;
  case kRealDataAverage2011all:
    cat = cat + k2011allOffset + kRealDataOffset;
    break;
  default:
    /// This should never happen.
    break;
  }
  return cat;
} /// PhosphorCorrectionFunctor::categorize(pt, eta, runNumber=0)
