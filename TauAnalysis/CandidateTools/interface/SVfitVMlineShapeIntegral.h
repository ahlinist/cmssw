#ifndef TauAnalysis_CandidateTools_SVfitVMlineShapeIntegral_h
#define TauAnalysis_CandidateTools_SVfitVMlineShapeIntegral_h

/** \class SVfitVMlineShapeIntegral
 *
 * Auxiliary class for computing decay distributions for polarized tau leptons
 * averaged over vector-meson (either rho or a1) line-shape factors
 *
 * The class implements formulas taken from the papers
 *  [1] "Tau polarization and its correlations as a probe of new physics",
 *      B.K. Bullock, K. Hagiwara and A.D. Martin,
 *      Nucl. Phys. B395 (1993) 499.
 *     (formulas 2.16 to 2.25)
 *  [2] "Charged Higgs boson search at the TeVatron upgrade using tau polarization",
 *      S. Raychaudhuri and D.P. Roy,
 *      Phys. Rev.  D52 (1995) 1556.           
 *     (formulas 32 to 38)
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.3 $
 *
 * $Id: SVfitVMlineShapeIntegral.h,v 1.3 2010/09/09 14:08:22 veelken Exp $
 *
 */

#include "TauAnalysis/CandidateTools/interface/SVfitVMlineShapeIntegrand.h"

#include <Math/Integrator.h>

class SVfitVMlineShapeIntegral
{
 public:
  SVfitVMlineShapeIntegral(SVfitVMlineShapeIntegrand::VMtype, SVfitVMlineShapeIntegrand::VMpol, bool);
  SVfitVMlineShapeIntegral(const SVfitVMlineShapeIntegral&);
  virtual ~SVfitVMlineShapeIntegral();

  SVfitVMlineShapeIntegral& operator=(const SVfitVMlineShapeIntegral&);

  double operator()(double, double) const;

 private:
  ROOT::Math::Integrator* integrator_;
  mutable SVfitVMlineShapeIntegrand* integrand_;


//--- temporary variables to speed-up computations
//    (computed once in constructor)
  double minMass2_;
  double norm_;
};

#endif
