#include <cmath>
#include <iostream>

#include "LLH.hpp"

using namespace std;

double LogLikelihood::calculate()
{
  std::vector<double> fxl;

  double xgslo = _xmu - _xgsig*_sigS;
  double xgshi = _xmu + _xgsig*_sigS;
  double bings = (xgshi - xgslo)/(_npts-1);

  double xgblo = _xmu - _xgsig*_sigB;
  double xgbhi = _xmu + _xgsig*_sigB;
  double bingb = (xgbhi - xgblo)/(_npts-1);

  double xgllo = _xmu - _xgsig*_sigL;
  double xglhi = _xmu + _xgsig*_sigL;
  double bingl = (xglhi - xgllo)/(_npts-1);

  // integrate over luminosity uncertainty distribution
  for (int igl=0; igl<_npts; igl++) {
    double xgl = xgllo + igl*bingl;
    std::vector<double> fxs;
    // integrate over signal selection uncertainty distribution
    for (int igs=0; igs<_npts; igs++) {
      double xgs = xgslo + igs*bings;
      std::vector<double> fxb;
      // integrate over background uncertainty distribution
      for (int igb=0; igb<_npts; igb++) {
        double xgb = xgblo + igb*bingb;
        double llh_bin = 0;
	//if (xgb>0 && xgs*xgl>0){
	// loop over  spectrum
	for (int j=0; j<_nPred.size(); j++) {
	  
	  double xmui = _nPred[j]*xgs*xgl + _nBkg[j]*xgb;
	  //cout <<"INside LLH1 "  << _nEvt[j] << " " << xmui <<endl;
	  double pp = poison(_nEvt[j], xmui);
	  //if (pp<1.e-80)pp=1.e-80;
	  //cout << "BIN" << j << " nEvt "  << _nEvt[j] << " nPred "<<  _nPred[j] << " nBkg "<<  _nBkg[j]
	  //<< " sigCo "<< xgs*xgl << " bkgCo "<< xgb << " pred+bkg " << xmui << " llh_xyBin "<<  llh_bin << " log(pp) "<<  log(pp) <<endl; 
	  llh_bin = llh_bin+log(pp);
	}//for j..._nPred
	//}//integrate >0.
	double llh_b = exp(llh_bin)*gauss(xgb,_xmu,_sigB);
	//cout << "llh_b" << llh_b << endl;
	fxb.push_back(llh_b);
	//cout << llh_b << endl;
      }
      double llh_s = simpson(fxb,xgblo,xgbhi)*gauss(xgs,_xmu,_sigS);
      fxs.push_back(llh_s);
      //cout << llh_s << endl;
    }
    double llh_l = simpson(fxs,xgslo,xglhi)*gauss(xgl,_xmu,_sigL);
    fxl.push_back(llh_l);
    //cout << llh_l << endl;
  }
  double tot = simpson(fxl,xgllo,xglhi);
  //cout << "tot \t" << tot << "\t log(tot) \t" << log(tot) << endl;
  return log(tot);
};

double LogLikelihood::gauss(double x, double xm, double sig)
{
  double twopi = 6.28318530718;
  //cout << "Inside gauss " << (x-xm)*(x-xm)/sig/sig << " " << exp(-((x-xm)*(x-xm)/sig/sig)/2.0) << endl;
  double val = exp(-((x-xm)*(x-xm)/sig/sig)/2.0)/sqrt(twopi)/sig;
  return val;
};

double LogLikelihood::poison(int n, double xm)
{
  double xn = n;
  double val = exp(-xm);
  if (n > 0) {
    if (n > 25) {
      //cout << "Inside pois " << xn<< " "<<  xm << " "<< sqrt(xm) <<endl;
      val = gauss(xn, xm, sqrt(xm));
    }
    else {
      for (int i=0; i<n; i++) {
        val = val*xm/(n-i);
      }
    }
  }
  return val;
};

double LogLikelihood::simpson(std::vector<double>& y, double xlo, double xhi)
{
  int n = y.size();
  double val;
  if (n < 3)
    val = -1;
  else {
    double dx = (xhi-xlo)/n;
    // the integral between the first two points is calculated by
    // (5*y[0]+8*y[1]-y[2])*dx/12
    val = (5*y[0]+8*y[1]-y[2])*dx/12;
    // the integral between the last two points is calculated by
    // (-y[n-3]+8*y[n-2]+5*y[n-1])*dx/12
    val = val + (-y[n-3]+8*y[n-2]+5*y[n-1])*dx/12;
    // the rest is calculated by the well-known Simpson's rule
    // (y[i]+4*y[i+1]+y[i+2])*dx/3
    for (int i=0; i<n-2; i++) {
      val = val + (y[i]+4*y[i+1]+y[i+2])*dx/3;
    }
  }
  // divide val by 2, since every interval is integrated twice.
  val = val/2;
  return val;
};
