////////////////////////////////////////////////////////////////////////
//
// File: LLH.hpp
// Description: A class used to calculate log likelihood of a given
//   data distribution using the estimated background and signal events.
//
// Created: Nov. 2003  Taka Yasuda
//
//
////////////////////////////////////////////////////////////////////////

#ifndef LLH_H
#define LLH_H

#include <vector>

class LogLikelihood {

private:
  std::vector<double> _nPred; // Monte Carlo prediction for signal. 
  std::vector<double> _nBkg;  // Estimated background.
  std::vector<int> _nEvt;     // Observed events.

  double _sigS;  // Uncertainty for event selection efficiencies.
  double _sigB;  // Uncertainty for background estimate.
  double _sigL;  // Uncertainty for luminosity.

  double _xmu;   // Mean of Gaussian distribution
  double _xgsig; // Integration range in sigma for Gaussian distribution.
  int _npts;     // Number of steps in integration.

  double _logLikelihood;  // Log likelihood value

public:
  LogLikelihood(const std::vector<double>& nPred, const std::vector<double>& nBkg, const std::vector<int>& nEvt, const double& sigS, const double& sigB, const double& sigL, const double& xmu, const double& xgsig, const int& npts): _nPred(nPred), _nBkg(nBkg), _nEvt(nEvt), _sigS(sigS), _sigB(sigB), _sigL(sigL), _xmu(xmu), _xgsig(xgsig), _npts(npts) {};

  ~LogLikelihood() {};

  double calculate();  // This is the function that calculate log likelihood
                       // values.
  double gauss(double x, double xm, double sig);
  double poison(int n, double xm);
  // Simpson's numerical integration.
  double simpson(std::vector<double>& y, double xlo, double xhi);

};

#endif // LLH_H
