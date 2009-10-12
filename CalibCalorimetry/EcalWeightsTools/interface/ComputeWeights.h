#ifndef CalibCalorimetry_EcalWeightsTools_ComputeWeights_h
#define CalibCalorimetry_EcalWeightsTools_ComputeWeights_h

/** \class ComputeWeights
  *  Class used to compute weights from a pulse shape profile.
  *
  *  $Date: 2006/07/19 22:58:42 $
  *  $Revision: 1.2 $
  *  \author R. Bruneliere - CERN
  */

#include <vector>

#include "CLHEP/Matrix/Matrix.h"
#include "CLHEP/Matrix/SymMatrix.h"

class ComputeWeights
{
 public:
  /// Constructor
  // explicit ComputeWeights(edm::ParameterSet const& pSet);
  ComputeWeights(int verbosity, bool doFitBaseline, bool doFitTime, 
		 int nPulseSamples, int nPrePulseSamples);

  /// Destructor
  ~ComputeWeights();

  /// Compute weigths from an input pulse shape
  bool compute(const std::vector<double>& pulseShape,
	       const std::vector<double>& pulseShapeDerivative,
	       const double tMax); //modif

  /// Get weight used to compute amplitude
  double getAmpWeight(int iSample) const;

  /// Get weight used to compute dynamic pedestal
  double getPedWeight(int iSample) const;

  /// Get weight used to compute time jitter
  double getTimeWeight(int iSample) const;

  /// Get chi2 matrix
  double getChi2Matrix(int iSample1, int iSample2) const;

 private:
  int verbosity_;        //< Messages verbosity
  bool doFitBaseline_;   //< Fit baseline or not
  bool doFitTime_;       //< Fit time jitter or not
  int nPulseSamples_;    //< Number of samples in the pulse
  int nPrePulseSamples_; //< Number of samples in the pre-pulse
  CLHEP::HepMatrix weights_;
  CLHEP::HepSymMatrix chi2_;
};
#endif // CalibCalorimetry_EcalWeightsTools_ComputeWeights_h
