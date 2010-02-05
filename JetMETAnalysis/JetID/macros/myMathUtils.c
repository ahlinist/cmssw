#ifndef AH_MATHUTILS
#define AH_MATHUTILS
/*
 My math utils
*/
#include "TROOT.h"
#include "TMath.h"
#include "TH1.h"
#include <iostream>
#include <stdio.h>
using std::endl;
using std::cout;
using std::cerr;

// in the ROOT version used in D0 to date (8/31/06), GetEffectiveEntries doesn't exist yet.
Double_t GetEffectiveEntries (TH1& h)
{
  Double_t stats[11];
  h.GetStats (stats);
  return stats[1] ? stats[0]*stats[0] / stats[1] : 0.;
}
Double_t GetEffectiveEntries (TH1* h)
{
  return h ? GetEffectiveEntries (*h) : 0;
}

// My replacement to the buggy ROOT routine.
Double_t HistMeanError (TH1& h, Int_t axis=1)
{
  if (axis < 1 || axis > 3) return 0;
  Double_t stats[11];
  for (int i=4; i<11; ++i) stats[i] = 0;
  h.GetStats (stats);
  if (stats[1] == 0) return 0;
  Double_t rms = h.GetRMS (axis);
  Double_t effN = GetEffectiveEntries (h);
  return rms / TMath::Sqrt (effN);
}

Double_t HistMeanError (TH1* h, Int_t axis=1)
{
  return h ? HistMeanError (*h, axis) : 0;
}

// ---------------------------------------------------------------------------------------------

void doMatrixMethod (double Nl, double Nt, double eQCD, double eSig, double& Nf, double& NW)
{
  Nf = (eSig * Nl - Nt) / (eSig - eQCD);
  NW= Nl - Nf;
  return;
}

void printMatrixMethod (double Nl, double Nt, double eQCD, double eSig)
{
  double Nf, NW;
  doMatrixMethod (Nl, Nt, eQCD, eSig, Nf, NW);
  cout<<"Loose: "<<Nl<<" = "<<Nf     <<" fakes + "<<NW     <<" Ws"<<endl;
  cout<<"Tight: "<<Nt<<" = "<<Nf*eQCD<<" fakes + "<<NW*eSig<<" Ws"<<endl;
}


bool calcWeightedAverage (Double_t a, Double_t b, Double_t da, Double_t db, Double_t& v, Double_t& dv)
{
  v = 0; dv = 0;
  if (da == 0 && db == 0 && a == b) {v = a; dv = 0; return 1;} // trivial case
  if (da <= 0 || db <=0) {return 0;}
  double c = 1 / ( 1 / ( da*da ) + 1 / ( db*db ) );
  double wa = c / (da*da);
  double wb = c / (db*db);
  v = wa*a + wb*b;
  dv = TMath::Sqrt(c);
  return 1;
}

bool calcEff (Double_t a, Double_t b, Double_t da, Double_t db, Double_t& eff, Double_t& deff)
{
  Double_t tot = a + b;
  // adapted from the CGI bin in harel.info
  if (tot <= 0) return 0;
  eff = a / tot;
  deff = TMath::Sqrt (da*da*b*b + db*db*a*a) / (tot*tot);
  return 1;
}

bool calcEffTot (Double_t a, Double_t tot, Double_t da, Double_t dtot, Double_t& eff, Double_t& deff)
{
  Double_t b = tot - a;
  if (da > dtot) return 0;
  Double_t db = TMath::Sqrt (dtot*dtot - da*da);
  if (tot <= 0 || b < 0) return 0;
  return calcEff (a, b, da, db, eff, deff);
}

bool calcPoisEff (Double_t a, Double_t b, Double_t& eff, Double_t& deff)
{
  return calcEff (a, b, (a<1) ? 1 : TMath::Sqrt(a), (b<1) ? 1 : TMath::Sqrt(b), eff, deff);
}

bool calcPoisEffTot (Double_t a, Double_t tot, Double_t& eff, Double_t& deff)
{
  Double_t b = tot - a;
  if (b < 0 || tot < 0) return 0;
  return calcPoisEff (a, b, eff, deff);
}

// adapted from the CGI bin in harel.info
char* formValErr (Double_t val, Double_t err, Double_t acc = 1.449777)
{
  err = TMath::Abs (err);
  if (err == 0) {
    //cout <<val<<" +/- "<<err;
    return Form ("%g +/- 0",val);
  }
  Double_t dig = log(err)/log(10.) + acc;
  Double_t digits = floor (dig) - 2;
  Double_t p10 = TMath::Power (10, digits);
  if (digits < 0) {
    Int_t ierr = (Int_t) (0.5 + err / p10);
    //cout<<"digits: "<<digits<<"="<<(int)digits<<"-->"<<Form ("%.*f(%d)",(int) -digits,val,ierr)<<endl;
    return Form ("%.*f(%d)",(int) -digits,val,ierr);
  } else {
    Double_t ferr = p10 * int (0.5 + err / p10);
    Double_t fval = p10 * int (0.5 + val / p10);
    return Form ("%g +/- %g", fval, ferr);
  }
}

TString formatInNaturalPrecision( Double_t val, int maxDigiAfterPeriod = 2 )
{
  if( maxDigiAfterPeriod < 0 ) return "";
  TString s1( Form( "%.*f", maxDigiAfterPeriod, val ) );
  while( s1.Contains('.') && s1.Length() > 1 && s1.EndsWith("0") ) 
    s1.Remove( -1+s1.Length() );
  if( s1.Length() > 1 && s1.EndsWith(".") )
    s1.Remove (-1+s1.Length());
  if( s1 == "-0" ) s1 = "0";
  return s1;
}
int find_exponent( double num )
{
  // use string processing since their internal trickery is ~20 times more
  // accurate than what I managed to do with logs, rounds offs, etc.
  TString ss( Form("%e", num ) );
  int n = ss.Length();
  TString s2( ss( n-3, 3 ) );
  return s2.Atoi();
}


double find_round_number_in_range( double min, double max )
{
  if( min * max <= 0 ) return 0;
  // so now both non-zero and have same sign 

  // arrange signs and sort
  bool negative = min < 0;
  if( negative ) {min *= -1; max *= -1;}
  if( max < min ) {
    double tmp = min;
    min = max;
    max = tmp;
  }

  int min_0s = find_exponent( max - min );
  int max_0s = find_exponent( max );
  
  double out = -1;
  double mid = ( min + max ) / 2;
  for( int n0s = max_0s; n0s >= min_0s; --n0s ) {
    double one_and_0s = pow( 10, n0s );
    double last = one_and_0s * TMath::Floor( max / one_and_0s );
    if( last >= min ) {
      double mid_low = one_and_0s * TMath::Floor( mid / one_and_0s );
      double mid_high = one_and_0s * TMath::Ceil( mid / one_and_0s );
      if( mid_low < min ) { out = mid_high; break; }
      if( mid_high > max ) { out = mid_low; break; }
      double d_low = mid - mid_low;
      double d_high = mid_high - mid;
      if( d_low < d_high ) { out = mid_low; break; }
      out = mid_high; break;
    }
  }
  if( out < 0 ) {
    cerr<<"BUG! out<0 after main loop of find_round_number_in_range(+/- "<<min<<" <--> "<<max<<")"<<endl; 
    out=mid; // at least return something within the range
  }

  if( negative ) out *= -1;
  return out;
}

Double_t addInQuadrature (Double_t a, Double_t b)
{
  return TMath::Sqrt (a*a + b*b);
}

Double_t dProduct (Double_t a, Double_t b, Double_t da, Double_t db)
{
  return TMath::Sqrt (a*a*db*db + b*b*da*da);
}

Double_t dRatio (Double_t a, Double_t b, Double_t da, Double_t db)
{
  if (b == 0) return a == 0 ? 0 : 987654321; 
  return TMath::Sqrt (a*a*db*db + b*b*da*da) / (b*b);
}

// assume a = b * r and hence the error on it. What's r and dr?
bool unmultiply (double a, double b, double da, double db, double& r, double& dr)
{
  if (b == 0) {cerr<<"b==0!"<<endl; return false;}
  r = a/b;
  double temp = pow(da, 2) - pow(db*r,2);
  if (temp < 0) {cerr<<"Relative error on a is smaller than on b!"<<endl; return false;}
  dr = TMath::Sqrt (temp) / b;
  return true;
}


// calculate the difference between two efficiencies taken from 
// samples with overlap.
// "Efficiencies" are just a/(a+b).
// Uses given errors for each of the 6 subsamples:
// a = yes, b=no
// "a" = in both samples, "a1" = only in 1st sample, "a2" = only in 2nd sample
// similarly for b
bool calcEffDiff (double a, double a1, double a2, 
		  double b, double b1, double b2,
		  double da, double da1, double da2,
		  double db, double db1, double db2,
		  double& ed, double& ded)
{
  if (a < 0 || a1 < 0 || a2 < 0 || b < 0 || b1 < 0 || b2 < 0) return false;
  if (da < 0 || da1 < 0 || da2 < 0 || db < 0 || db1 < 0 || db2 < 0) return false;
  double n = a + b;
  double n1 = a1 + b1;
  double n2 = a2 + b2;
  double N1 = n + n1;
  double N2 = n + n2;
  //cout<<"DBG: n:"<<n<<", N1: "<<N1<<", N2: "<<N2<<endl;
  // didn't do the math for those special cases
  if (N1 <= 0 || N2 <= 0) return false;

  ed = (a + a1)/N1 - (a + a2)/N2;
  //cout<<"a: "<<a<<", a1: "<<a1<<", N1: "<<N1<<" --> "<<((a + a1)/N1)<<endl;
  //cout<<"a: "<<a<<", a2: "<<a2<<", N2: "<<N2<<" --> "<<((a + a2)/N2)<<endl;

  // the denominator
  double F = a*(b2-b1) + b*(a1-a2) + a1*b2 - a2*b1;

  //cout<<"DBG Check -    F: "<<F<<"  =? N1*N2*ed: "<<(N1*N2*ed)<<endl;

  // the derivatives, each times (N1*N2)^2
  double dfda  =    (b2 - b1)*N1*N2 - F*(N1+N2);
  double dfdb  =    (a1 - a2)*N1*N2 - F*(N1+N2);
  double dfda1 =    (b  + b2)*N1*N2 - F*N2;
  double dfdb1 = -1*(a  + a2)*N1*N2 - F*N2;
  double dfda2 = -1*(b  + b1)*N1*N2 - F*N1;
  double dfdb2 =    (a  + a1)*N1*N2 - F*N1;
  //cout<<"             dfda: "<<dfda <<",    dfdb: "<<dfdb <<endl;
  //cout<<"            dfda1: "<<dfda1<<",   dfdb1: "<<dfdb1<<endl;
  //cout<<"            dfda2: "<<dfda2<<",   dfdb2: "<<dfdb2<<endl;

  // the variance, times (N1*N2)^4
  double var = da *da *dfda *dfda  + db *db *dfdb *dfdb  + 
               da1*da1*dfda1*dfda1 + db1*db1*dfdb1*dfdb1 +
               da2*da2*dfda2*dfda2 + db2*db2*dfdb2*dfdb2;

  ded = TMath::Sqrt (var) / (N1*N1*N2*N2);
  return true;
}

// calculate the difference between two efficiencies taken from 
// samples with overlap.
// "Efficiencies" are just a/(a+b). In fact this was built for asymmetries
// where a=forward, b=backwards.
// Uses Poisson statistics (really VarX = X) for each of the 6 subsamples:
// a = yes, b=no
// "a" = in both samples, "a1" = only in 1st sample, "a2" = only in 2nd sample
// similarly for b
bool calcEffDiffPois (double a, double a1, double a2, 
		      double b, double b1, double b2,    
		      double& ed, double& ded)
{
  if (a < 0 || a1 < 0 || a2 < 0 || b < 0 || b1 < 0 || b2 < 0) return false;
  return calcEffDiff (a, a1, a2, b, b1, b2, 
		      TMath::Sqrt(a), TMath::Sqrt(a1), TMath::Sqrt(a2), 
		      TMath::Sqrt(b), TMath::Sqrt(b1), TMath::Sqrt(b2), 
		      ed, ded);
}

// calculate the difference between two asymmetries taken from 
// samples with overlap.
// "Asymmetries" are just (b-a)/(b+a). In fact this was built for asymmetries
// where b=forward, a=backwards.
// Uses given errors for each of the 6 subsamples:
// a = yes, b=no
// "a" = in both samples, "a1" = only in 1st sample, "a2" = only in 2nd sample
// similarly for b
bool calcAsymDiff (double a, double a1, double a2, 
		   double b, double b1, double b2,
		   double da, double da1, double da2,
		   double db, double db1, double db2,
		   double& dasym, double& ddasym)
{
  double eff, deff;
  bool OK =  calcEffDiff (a, a1, a2, b, b1, b2, 
			  da, da1, da2, db, db1, db2,
			  eff, deff);
  if (!OK) return false;

  // just a linear transformation: asym = 1 - 2*eff
  //                     --->     dasym = 2*deff;
  //                     -->     ddasym = 2*ddeff;
  //cout<<"DBG - eff:"<<eff<<" @ "<<deff<<endl;
  dasym = 2*eff;
  ddasym = 2*deff;
  return true;
}
    
// treat eff & deff as mean and rms of a normal distribution whose edges outside [0,1] are
// cut. Then return "68%" on either side of eff.
// Please note that this means that when deff-->+inf, dlow+dhigh --> "68%"
bool makeAsymmEffErrors (Double_t eff, Double_t deff, Double_t& dlow, Double_t& dhigh)
{
  const Double_t f68 = 2*TMath::Freq(1) - 1.;

  if (eff>1 || eff<0) {dhigh = dlow = 0; return false;}
  if (deff <= 0) {dhigh = dlow = 0; return true;}

  Double_t integlow  = TMath::Freq (-eff/deff);
  Double_t integhigh = TMath::Freq ((1-eff)/deff);
  /*
  cout<<"dbg: integrals l: "<<integlow<<" h: "<<integhigh<<endl;
  cout<<"dbg: --> "<<(0.5 - f68 * (0.5-integlow ))<<" & "<<(0.5 + f68 * (integhigh-0.5))<<endl;
  double lowedge  = eff + deff * TMath::NormQuantile (0.5 - f68 * (0.5-integlow ));
  double highedge = eff + deff * TMath::NormQuantile (0.5 + f68 * (integhigh-0.5));
  cout<<"dbg: iedges l: "<<lowedge<<" h: "<<highedge<<endl;
  dlow  = eff - lowedge;
  dhigh = highedge - eff;
  cout<<"dbg: deltas l: "<<dlow<<" h: "<<dhigh<<endl;
  */
  dlow  = -deff * TMath::NormQuantile (0.5 - f68 * (0.5-integlow ));
  dhigh =  deff * TMath::NormQuantile (0.5 + f68 * (integhigh-0.5));
  return true;
}
#endif
