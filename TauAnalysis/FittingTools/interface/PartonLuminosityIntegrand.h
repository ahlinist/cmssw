#ifndef TauAnalysis_FittingTools_PartonLuminosityIntegrand_h
#define TauAnalysis_FittingTools_PartonLuminosityIntegrand_h

/** \class PartonLuminosityIntegrand
 *
 * Auxiliary class for computing performing integration over parton distribution functions
 * when computing parton luminosity
 *
 * Class implements formula given in equation (7) of:
 *   M. Spira: "HIGLU: A Program for the Calculation of the Total Higgs Production Cross
 *              Section at Hadron Colliders via Gluon Fusion including QCD Corrections",
 *   arXiv:hep-ph/9510347
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.5 $
 *
 * $Id: PartonLuminosityIntegrand.h,v 1.5 2010/09/26 16:50:07 veelken Exp $
 *
 */

#include <Math/IFunction.h>
#include <Math/IFunctionfwd.h>

class PartonLuminosityIntegrand : public ROOT::Math::IGenFunction
{
 public:
  PartonLuminosityIntegrand();
  PartonLuminosityIntegrand(const PartonLuminosityIntegrand&);
  virtual ~PartonLuminosityIntegrand();

  PartonLuminosityIntegrand& operator=(const PartonLuminosityIntegrand&);

  void SetParameterSqrtS(double);
  void SetParameterQ(double);
  void SetParameterTau(double);
  void SetParameterFlavor1(int);
  void SetParameterFlavor2(int);

  virtual ROOT::Math::IGenFunction* Clone () const { return new PartonLuminosityIntegrand(*this); }

 private:
  double DoEval(double) const;

  double sqrtS_;
  double Q_;
  double tau_;
  int flavor1_;
  int flavor2_;
};

#endif
