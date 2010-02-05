#ifndef LJDBT_HISTUTILS
#define LJDBT_HISTUTILS

#include <iostream>
#include <stdio.h>
#include <vector>
#include "TMath.h"
#include "TROOT.h"
#include "TH2F.h"
#include "TF1.h"
#include "TString.h"
#include "TVirtualFitter.h"
#include "TMatrixD.h"
#include "TMatrixDEigen.h"

#include "myMathUtils.c" // needed for passingFraction function

using std::endl;
using std::cout;
using std::cerr;

// Useful for drawing with a logy option
Stat_t minPositiveEntry (TH1& h1)
{
  int nBins = h1.GetNbinsX();
  Stat_t out = h1.GetMaximum();
  for (Int_t i=1; i<=nBins; ++i) {
    Stat_t cur = h1.GetBinContent (i);
    if (cur > 0 && cur < out) out = cur;
  }
  return out;
}

// ---------------------------------------------------------------------------------------------
double maxValMinusNSigma (TH1& h, double N = 1.)
{
  if( h.GetDimension() != 1 ) {cerr<<"NYI! maxValMinusSigma only does 1D now"<<endl; return 0;}
  TAxis* ax = h.GetXaxis();
  double maxVal = h.GetMinimum() - N * h.GetBinError( 1 ); 
  for (int binx=ax->GetFirst(); binx<=ax->GetLast(); ++binx) {
    Double_t c = h.GetBinContent(binx) - N * h.GetBinError(binx);
    if( c > maxVal ) maxVal = c;
  }
  return maxVal;
}

// binwise flips a histogram left to right (e.g. first bin <--> last bin)
TH1* flipHist (TH1& org)
{
  TString name = org.GetName();
  TH1 *pnew = (TH1*) org.Clone(name+"_folded");
  if (pnew == 0) {cout<<"Failed to clone input histogram"<<endl; return 0;}

  TAxis* ax = org.GetXaxis();
  int last = ax->GetLast();
  int first = ax->GetFirst();
  const int N = 3+last-first; // need under and overflow

  // fold over - we're leaving the (possible) middle bin as it was after the clone
  int target = (N+1)/2;
  int source = (N-2)/2;
  for (;target<=last+1; ++target, --source) {
    pnew->SetBinContent (target, org.GetBinContent (source));
    pnew->SetBinError   (target, org.GetBinError   (source));
    pnew->SetBinContent (source, org.GetBinContent (target));
    pnew->SetBinError   (source, org.GetBinError   (target));
  }
  pnew->SetEntries (org.GetEntries());
  return pnew;
}

TH1* flipHist (TH1* org)
{
  if (0 == org) {cerr<<"NULL pointer was input to flipHist"<<endl; return 0;}
  return flipHist (*org);
}

TH1F* flipTH1F (TH1F& org)
{
  return (TH1F*) flipHist (org);
}

TH1D* flipTH1D (TH1D& org)
{
  return (TH1D*) flipHist (org);
}

TH1F* flipTH1F (TH1F* org)
{
  return (TH1F*) flipHist (org);
}

TH1D* flipTH1D (TH1D* org)
{
  return (TH1D*) flipHist (org);
}

// shift histogram left or right, by moving bin contents 'shift' bins (+ = to the right).
// note: overwrites overflow bins with bad data
// todo: handle overflows option
TH1* shiftHist (TH1& org, int shift)
{
  TString name = org.GetName();
  TH1 *pnew = (TH1*) org.Clone(Form("%s_shift%d", name.Data(), shift));
  if (pnew == 0) {cout<<"Failed to clone input histogram"<<endl; return 0;}

  if (shift == 0) return pnew;

  TAxis* ax = org.GetXaxis();
  int last = ax->GetLast();
  int first = ax->GetFirst();

  for (int source = first; source<=last; ++source) {
    int target = TMath::Range (first-1, last+1, source + shift); // can shift into overflows, but really aught to add
    pnew->SetBinContent (target, org.GetBinContent (source));
    pnew->SetBinError   (target, org.GetBinError   (source));
  }
  pnew->SetEntries (org.GetEntries());
  return pnew;
}

// shift histogram left or right, by moving bin contents 'shift' bins (+ = to the right).
// note: overwrites overflow bins with bad data
// todo: handle overflows option
bool shiftHistInPlace (TH1& h1, int shift)
{
  if (shift == 0) return true;

  Stat_t entries = h1.GetEntries();

  TAxis* ax = h1.GetXaxis();
  int last = ax->GetLast();
  int first = ax->GetFirst();

  if (shift > 0) { // going right, start from right
    for (int source = last; source>=first; --source) {
      int target = TMath::Range (first-1, last+1, source + shift);
      h1.SetBinContent (target, h1.GetBinContent (source));
      h1.SetBinError   (target, h1.GetBinError   (source));
    }
  } else { // going left, start from left
    for (int source = first; source<=last; ++source) {
      int target = TMath::Range (first-1, last+1, source + shift);
      h1.SetBinContent (target, h1.GetBinContent (source));
      h1.SetBinError   (target, h1.GetBinError   (source));
    }
  }
  h1.SetEntries (entries);
  return true;
}




// subtracts histograms, assuming results and "bad" are subsets of all.
// so that the sum in quadrature of the resulting and "bad" errors is the "all" errors.
TH1F* subtractHists (TH1F& all, TH1F& bad, TString nameSuffix = "_sub")
{
  if (all.GetNbinsX() != bad.GetNbinsX()) {cerr<<"NbinsX differed in subtractHists"<<endl; return 0;}
  int n = 2+all.GetNbinsX(); // over and underflows
  TH1F* hnew = (TH1F*) all.Clone(Form("%s%s", all.GetName(), nameSuffix.Data()));
  if (!hnew) {cerr<<"Failed to clone histogram in subtractHists"<<endl; return 0;}
  for (int i=0; i<n; ++i) {
    Double_t a, b, da, db, s, ds;
    a  = all.GetBinContent (i);
    b  = bad.GetBinContent (i);
    da = all.GetBinError   (i);
    db = bad.GetBinError   (i);
    s = a - b;
    ds = da > db ? TMath::Sqrt (da*da - db*db) : 0;
    hnew->SetBinContent (i, s);
    hnew->SetBinError   (i, ds);
  }
  if (all.GetEntries() > bad.GetEntries()) hnew->SetEntries(all.GetEntries()-bad.GetEntries());
  return hnew;
}

// replace histogram context f(x) with the cumulative integral of f(x)
void accumulate( TH1& hist, const bool from_left = true )
{
  Double_t entries = hist.GetEntries();
  TAxis* ax = hist.GetXaxis();
  int last = ax->GetLast();
  int first = ax->GetFirst();

  Double_t sum = 0, dsum2 = 0;
  if( from_left ) {
    for( int ibin = first - 1; ibin <= last + 1; ++ibin ) {
      sum += hist.GetBinContent( ibin );
      dsum2 += TMath::Power( hist.GetBinError( ibin ), 2 );

      hist.SetBinContent( ibin, sum );
      hist.SetBinError( ibin, TMath::Sqrt( dsum2 ) );
    }
  } else {
    for( int ibin = last + 1; ibin >= first-1; --ibin ) {
      sum += hist.GetBinContent( ibin );
      dsum2 += TMath::Power( hist.GetBinError( ibin ), 2 );

      hist.SetBinContent( ibin, sum );
      hist.SetBinError( ibin, TMath::Sqrt( dsum2 ) );
    }
  }
  // patch for ROOT versions <5.24 is commenting out the line: hist.ResetStats();
  hist.SetEntries( entries );
  return;
}

inline bool accumulate( TH1* hist, const bool from_left = true )
{
  if( 0 == hist ) {cerr<<"NULL pointer was input to accumulate"<<endl; return false;}
  accumulate( *hist, from_left );
  return true;
}

void sumBins (TH1* h, const Int_t binx1, const Int_t binx2, double& sum, double& dsum)
{
  if (h == 0) return;
  sum = dsum = 0;
  for (Int_t i=binx1; i<=binx2; ++i) {
    sum += h->GetBinContent (i);
    dsum += TMath::Power (h->GetBinError (i), 2);
  }
  if (dsum > 0) dsum = TMath::Sqrt (dsum);
  return;
}

inline void sumBins (TH1& h, const Int_t binx1, const Int_t binx2, double& sum, double& dsum)
{
  return sumBins (&h, binx1, binx2, sum, dsum);
}

inline void countEntries (TH1& h, double& sum, double& dsum)
{
  return sumBins (h, 0, 1 + h.GetNbinsX(), sum, dsum);
}

inline void countEntries (TH1* h, double& sum, double& dsum)
{
  if (h == 0) return;
  return countEntries (*h, sum, dsum);
}

// this is a rough approximation (and probably an upper bound)
Double_t getEffectiveEntriesWithOverflows( TH1& h )
{
  if( h.GetDimension() != 1 ) {cerr<<"getEffectiveEntriesWithOverflows called for non-1D histogram"<<endl; return -1;}

  Int_t nBins = h.GetNbinsX();
  Double_t N = h.GetEffectiveEntries();
  Double_t cU = h.GetBinContent( 0 );
  Double_t eU = h.GetBinError( 0 );
  if( eU > 0 && cU > 0 ) N += cU*cU / (eU*eU);
  Double_t cO = h.GetBinContent( 1 + nBins );
  Double_t eO = h.GetBinError( 1 + nBins );
  if( eO > 0 && cO > 0 ) N += cO*cO / (eO*eO);
  return N;
}

Double_t getEffectiveEntriesWithOverflows( TH1* h )
{
  if( 0 == h ) {cerr<<"NULL pointer sent to getEffectiveEntriesWithOverflows"<<endl; return -2;}
  return getEffectiveEntriesWithOverflows( *h );
}


// includes interpolation to avoid binning noise
bool passingFraction (TH1& hist, const float cutVal, double& frac, double& dfrac, const bool passAbove = true, 
		      const int iDebug = 0, const bool ignoreOverflows = false)
{
  int nBins = hist.GetNbinsX();
  int cutBin = hist.FindBin(cutVal);
  if (cutBin < 1 || cutBin > nBins) {cerr<<"cutVal ("<<cutVal<<") is outside histogram: "
					 <<hist.GetName()<<endl; return false;}
  double left, dleft, right, dright, bin, dbin;
  sumBins (hist, ignoreOverflows ? 1 : 0, cutBin-1                         , left , dleft);
  sumBins (hist, cutBin                 , cutBin                           , bin  , dbin);
  sumBins (hist, 1+cutBin               , ignoreOverflows ? nBins : 1+nBins, right, dright);
  if (iDebug > 3) cout<<"dleft: "<<dleft<<", dbin: "<<dbin<<", dright: "<<dright<<endl;

  double xleft  = hist.GetBinLowEdge (  cutBin);
  double xright = hist.GetBinLowEdge (1+cutBin);
  double width = xright - xleft;

  double corleft = left, corright = right, cordleft = dleft, cordright = dright;
  if (width > 0) { // if not, ignore any content of this funny bin
    double frac = (cutVal - xleft) / width;
    if (iDebug > 1) {cout<<"width: "<<width<<" frac: "<<frac<<", bin: "<<bin<<" @ "<<dbin<<endl;}
    corleft += frac * bin;
    cordleft = addInQuadrature (cordleft, frac * dbin);
    corright += (1-frac) * bin;
    cordright = addInQuadrature (cordright, (1-frac) * dbin);
  }

  double fail, dfail, pass, dpass;
  if (passAbove) {fail = corleft; dfail = cordleft; pass = corright; dpass = cordright;}
  else {fail = corright; dfail = cordright; pass = corleft; dpass = cordleft;}

  if (iDebug > 2) cout<<"pass: "<<pass<<" @ "<<dpass<<",  fail: "<<fail<<" @ "<<dfail<<endl;
  // what's the uncertainty on 0? Estimate from other sample
  // estimate must scale with overall normalization (when Neff=(N/dN)^2 is const), and yield "1" for N +/- sqrt(N) -->
  if( dpass <= 0 && fail > 0) dpass = dfail*dfail / fail;
  if( dfail <= 0 && pass > 0) dfail = dpass*dpass / pass;
  bool OK = calcEff (pass, fail, dpass, dfail, frac, dfrac);
  if (iDebug) cout<<"DBG passingFraction - OK: "<<OK<<", f: "<<formValErr(fail,dfail)<<", p: "<<formValErr(pass,dpass)
		  <<" --> "<<frac<<" +/- "<<dfrac<<endl;
  if (!OK) {cerr<<"calcEff failed for "<<hist.GetName()<<" @ "<<cutVal<<endl; return false;}
  return true;
}

// ---------------------------------------------------------------------------------------------
TH1D* convertToTH1D (TH1* h1, TString name = "")
{
  if (h1 == 0) {cerr<<"convertToTH1D: Null pointer given"<<endl; return 0;}
  TAxis *pax = h1->GetXaxis();
  if (pax == 0) {cerr<<"convertToTH1D was given a histogram without an x axis (?)"<<endl; return 0;}

  if (name.Length() == 0) {
    name = h1->GetName();
    name += "D";
  }
  TH1D* hn = new TH1D (name, h1->GetTitle(), h1->GetNbinsX(), 
		       pax->GetXmin(), pax->GetXmax());
  if (hn == 0) {cerr<<"convertToTH1D: Allocation error"<<endl;  return 0;}

  for (int i=0; i<=1+h1->GetNbinsX(); ++i) { // copy over nad under flow bins
    hn->SetBinContent (i, h1->GetBinContent(i));
    hn->SetBinError   (i, h1->GetBinError(i));
  }
  hn->SetEntries (h1->GetEntries());
  Stat_t stats[20]; // I think only 11 can possible be used for TH3. 4 really enough for TH1
  h1->GetStats (stats);
  hn->PutStats (stats);
  hn->GetXaxis()->SetTitle (h1->GetXaxis()->GetTitle());
  hn->GetYaxis()->SetTitle (h1->GetYaxis()->GetTitle());
  return hn;
}

TH1F* convertToTH1F (TH1* h1, TString name = "")
{
  if (h1 == 0) {cerr<<"convertToTH1F: Null pointer given"<<endl; return 0;}
  TAxis *pax = h1->GetXaxis();
  if (pax == 0) {cerr<<"convertToTH1F was given a histogram without an x axis (?)"<<endl; return 0;}

  if (name.Length() == 0) {
    name = h1->GetName();
    name += "F";
  }
  TH1F* hn = new TH1F (name, h1->GetTitle(), h1->GetNbinsX(), 
		       pax->GetXmin(), pax->GetXmax());
  if (hn == 0) {cerr<<"convertToTH1F: Allocation error"<<endl;  return 0;}

  for (int i=0; i<=1+h1->GetNbinsX(); ++i) { // copy over nad under flow bins
    hn->SetBinContent (i, h1->GetBinContent(i));
    hn->SetBinError   (i, h1->GetBinError(i));
  }
  hn->SetEntries (h1->GetEntries());
  Stat_t stats[20]; // I think only 11 can possible be used for TH3. 4 really enough for TH1
  h1->GetStats (stats);
  hn->PutStats (stats);
  hn->GetXaxis()->SetTitle (h1->GetXaxis()->GetTitle());
  hn->GetYaxis()->SetTitle (h1->GetYaxis()->GetTitle());
  return hn;
}


// Note: the histograms "stats" are not recalculated!
void packFlows (TH1* h1)
{
  if( h1 == 0 ) {cerr<<"packFlows was given a NULL pointer"<<endl; return;}
  if( h1->GetDimension() != 1 ) {cerr<<"packFlows can only handle 1D histograms (yet?)"<<endl; return;}
  int nBins = h1->GetNbinsX();
  Stat_t nEntries = h1->GetEntries();
  // pack overflow
  h1->SetBinContent (nBins, h1->GetBinContent(nBins) + h1->GetBinContent(1+nBins));
  h1->SetBinError   (nBins, sqrt ( pow(h1->GetBinError(nBins),2) + pow(h1->GetBinError(1+nBins),2) ) );
  h1->SetBinContent (1+nBins, 0);
  h1->SetBinError   (1+nBins, 0);
  // pack underflow
  h1->SetBinContent (1, h1->GetBinContent(1) + h1->GetBinContent(0));
  h1->SetBinError   (1, sqrt ( pow(h1->GetBinError(1),2) + pow(h1->GetBinError(0),2) ) );
  h1->SetBinContent (0, 0);
  h1->SetBinError   (0, 0);

  h1->SetEntries (nEntries); // otherwise it increases
}

// Make a copy of the 1D histograms with a different axis range
TH1* cloneRange (const TH1* h1, const Axis_t x1, const Axis_t x2, const TString name = "")
{
  if (x2 <= x1) {cerr<<"cloneRange was given a nonsensical range: "<<x1<<" - "<<x2<<endl; return 0;}
  if (h1 == 0) {cerr<<"cloneRange: Null pointer given"<<endl; return 0;}

  TString newName( name );
  if (newName.Length() == 0) {
    newName = h1->GetName();
    newName += "G";
  }
  TH1* hn = (TH1*) h1->Clone (newName);
  if (hn == 0) {cerr<<"cloneRange problem - Failed to clone input histogram - Allocation error"<<endl;  return 0;}
  TAxis *pax = hn->GetXaxis();
  if (pax == 0) {cerr<<"cloneRange problem - no x axis (?)"<<endl; return 0;}
  Double_t oldMin = pax->GetXmin();
  Double_t oldMax = pax->GetXmax();
  Double_t oldN   = pax->GetNbins();
  if (oldN <= 0) {cerr<<"cloneRange problem - oldN: "<<oldN<<endl; return 0;}
  Double_t width = (oldMax - oldMin) / oldN;

  int N = int (0.5 + (x2 - x1) / width);
  pax->Set (N, x1, x2); // this completely messes up the histogram.

  for (int i=0; i<=N+1; ++i) { // so reset
    hn->SetBinContent (i, 0.);
    hn->SetBinError (i, 0.);
  }

  for (int i=0; i<=1+oldN; ++i) { // and prepare, using all (including over and under flow) bins from original
    Axis_t xold = h1->GetBinCenter (i);
    int target = 0;
    if (xold > x1) {
      if (xold > x2) {
	target = 1+N; // into overflow
      } else {
	target = hn->FindBin (xold); // in range
      }
    } // else underflow
    Double_t val = hn->GetBinContent (target);
    Double_t err = hn->GetBinError   (target);
    hn->SetBinContent (target, val + h1->GetBinContent(i));
    hn->SetBinError   (target, sqrt (pow (err, 2) + pow (h1->GetBinError(i), 2)));
  }
  hn->SetEntries (h1->GetEntries());
  Stat_t stats[20]; // I think only 11 can possible be used for TH3. 4 really enough for TH1
  h1->GetStats (stats);
  hn->PutStats (stats); // If they get recalculated further on, great. For now it's better than nothing.
  hn->GetXaxis()->SetTitle (h1->GetXaxis()->GetTitle());
  hn->GetYaxis()->SetTitle (h1->GetYaxis()->GetTitle());
  return hn;
}

void addConst (TH1* h, float c)
{
  TAxis* ax = h->GetXaxis();
  int last = ax->GetLast();
  int first = ax->GetFirst();
  for (int binx=first-1; binx<=last+1; ++binx) {
    h->SetBinContent (binx, c + h->GetBinContent(binx));
  }
}

void normalizeIntegral (TH1F& h, Double_t d = 1.)
{
  h.Sumw2(); // otherwise errors will be all wrong
  Stat_t sum = h.Integral();
  if (sum>0) {
    Double_t scale = d/sum;
    h.Scale (scale);
  }
}

void normalize (TH1F& h, Double_t d = 1.)
{
  h.Sumw2(); // otherwise errors will be all wrong
  Stat_t sum = h.GetSum();
  if (sum>0) {
    Double_t scale = d/sum;
    h.Scale (scale);
  }
}
void normalize (TH1D& h, Double_t d = 1.)
{
  h.Sumw2(); // otherwise errors will be all wrong
  Stat_t sum = h.GetSum();
  if (sum>0) {
    Double_t scale = d/sum;
    h.Scale (scale);
  }
}
bool normalize( TH1&  h, Double_t d = 1. )
{
  if( h.InheritsFrom( "TH1F" ) ) {normalize( (TH1F&) h, d ); return true; }
  if( h.InheritsFrom( "TH1D" ) ) {normalize( (TH1F&) h, d ); return true; }
  return false;
}

// ---------------------------------------------------------------------------------------------
Int_t highestNonZeroBin( TH1& h )
{
  TAxis* ax = h.GetXaxis();
  for( int binx=ax->GetLast(); binx>=ax->GetFirst(); --binx ) {
    Double_t c = TMath::Abs( h.GetBinContent( binx ));
    if( c>0 )return binx;
  }
  return -1 + ax->GetFirst();
}

Axis_t highestNonZeroBinsHighEdge( TH1& h )
{
  return h.GetBinLowEdge( 1 + highestNonZeroBin( h ));
}

Int_t lowestNonZeroBin( TH1& h )
{
  TAxis* ax = h.GetXaxis();
  for( int binx=ax->GetFirst(); binx<=ax->GetLast(); ++binx ) {
    Double_t c = TMath::Abs( h.GetBinContent( binx ));
    if( c>0 ) return binx;
  }
  return 1 + ax->GetLast();
}

Axis_t lowestNonZeroBinsHighEdge( TH1& h )
{
  return h.GetBinLowEdge( 1 + lowestNonZeroBin( h ));
}


// Name is somewhat misleading: if well measured bins are intermittent, this will be the last of the first sequence.
// Returns a point between center and edge, so that the center of the last well measured bin is included in range.
// Note that it relative error mode, 0+/-0 is always bad (as it indicates not enough statistics).
// tolerance is the number of bad bin that can be tolerated in a row before giving up
Axis_t findLastWellMeasuredBin (TH1& h, double maxErr = 0.1, bool findFirstInstead = false, bool relativeErr = false,
                                int tolerance = 0, int dbgLevel=0)
{
  TAxis* ax = h.GetXaxis();
  int nBad = 0;
  if (findFirstInstead) {
    for (int binx=ax->GetLast(); binx>=ax->GetFirst(); --binx) {
      Double_t err = TMath::Abs (h.GetBinError(binx));
      bool badlyMeasured = relativeErr
        ? (err >= maxErr*TMath::Abs(h.GetBinContent(binx)))
        : (err > maxErr);
      // edges are subject to nasty round off errors.
      if (badlyMeasured) ++nBad;
      else nBad = 0;
      if (nBad > tolerance) return 0.75 * h.GetBinLowEdge (binx+nBad) + 0.25 * h.GetBinLowEdge (1+binx+nBad);
    }
    return 0.75 * h.GetBinLowEdge (ax->GetFirst()+nBad) + 0.25 * h.GetBinLowEdge (1+ax->GetFirst()+nBad);
  } else {
    for (int binx=ax->GetFirst(); binx<=ax->GetLast(); ++binx) {
      Double_t err = TMath::Abs (h.GetBinError(binx));
      bool badlyMeasured = relativeErr
        ? (err >= maxErr*TMath::Abs(h.GetBinContent(binx)))
        : (err > maxErr);
      if (dbgLevel) cout<<"DBG binx: "<<binx<<" err: "<<err<<" |con|: "<<TMath::Abs(h.GetBinContent(binx))
                        <<" bad? "<<badlyMeasured<<endl;

      // edges are subject to nasty round off errors.
      if (badlyMeasured) ++nBad;
      else nBad = 0;
      if (nBad > tolerance) return 0.75 * h.GetBinLowEdge (binx+1-nBad) + 0.25 * h.GetBinLowEdge (binx-nBad);
    }
    return 0.75 * h.GetBinLowEdge (1+ax->GetLast()-nBad) + 0.25 * h.GetBinLowEdge (ax->GetLast()-nBad);
  }
}


// Note: the histograms "stats" are not recalculated!
void foldDownTo (TH1* h1, Axis_t xmax)
{
  if (h1 == 0) {cerr<<"foldDownTo was given a NULL pointer"<<endl; return;}
  int nBins = h1->GetNbinsX();
  int iMax = h1->FindBin (xmax);
  if (iMax > nBins) return; // nothing to do
  if (iMax < 1) {cerr<<"foldDownTo aborts! xmax is too low"<<endl; return;}

  Stat_t nEntries = h1->GetEntries();

  Double_t sum = 0, dsum2 = 0;
  for (int binx=1+nBins; binx>=iMax; --binx) {
    sum   += h1->GetBinContent (binx);
    dsum2 += TMath::Power(h1->GetBinError (binx), 2);
    h1->SetBinContent (binx, 0);
    h1->SetBinError   (binx, 0);
  }
  if (dsum2 > 0) dsum2 = TMath::Sqrt (dsum2);
  h1->SetBinContent (iMax, sum);
  h1->SetBinError   (iMax, dsum2);

  h1->SetEntries (nEntries); // otherwise it increases
}

// Notes:
// 1) the histograms "stats" are not recalculated!
// 2) the "Entries" are not (can not be?) recalculated!
bool zeroRange (TH1* h1, Int_t iFirstBin, Int_t iLastBin)
{
  if (h1 == 0) {cerr<<"zeroRange was given a NULL pointer"<<endl; return false;}
  int nBins = h1->GetNbinsX();
  if (iLastBin < iFirstBin) {cerr<<"zeroRange aborts! iLastBin < iFirstBin"<<endl; return false;}
  if (iFirstBin > nBins) return true; // nothing to do
  if (iLastBin < 1)      return true; // nothing to do

  Stat_t nEntries = h1->GetEntries();

  for (int binx=iFirstBin; binx<=iLastBin; ++binx) {
    h1->SetBinContent (binx, 0);
    h1->SetBinError   (binx, 0);
  }

  h1->SetEntries (nEntries); // otherwise it increases
  return true;
}

// Notes:
// 1) the histograms "stats" are not recalculated!
// 2) the "Entries" are not (can not be?) recalculated!
bool zeroRange (TH1* h1, Axis_t xmin, Axis_t xmax)
{
  if (h1 == 0) {cerr<<"zeroRange was given a NULL pointer"<<endl; return false;}
  int iMin = h1->FindBin (xmin);
  int iMax = h1->FindBin (xmax);
  return zeroRange (h1, iMin, iMax);
}

// Notes:
// 1) the bin that's at xmin is NOT zeroed.
// 2) the histograms "stats" are not recalculated!
// 3) the "Entries" are not (can not be?) recalculated!
bool zeroRangeAbove (TH1* h1, Axis_t xmin)
{
  if (h1 == 0) {cerr<<"zeroRangeAbove was given a NULL pointer"<<endl; return false;}
  int nBins = h1->GetNbinsX();
  int iMin = h1->FindBin (xmin);
  if (iMin > nBins) return true; // nothing to do
  return zeroRange (h1, 1+iMin, 1+nBins);
}

// make an error histogram whose +/- 1 sigma edges are at the given functions
TH1D* funcsToErrorHist (TF1& func1, TF1& func2, TString suffix="")
{
  TH1* phorg = func1.GetHistogram();
  if (phorg == 0) {cerr<<"Can't find histogram of func1"<<endl; return 0;}
  Int_t nBin = phorg->GetNbinsX();

  TH1D* ph = (TH1D*) phorg->Clone (Form("%s%s", func1.GetName(), suffix.Data()));
  if (ph->GetBinCenter (1) < func2.GetXmin() ||
      ph->GetBinCenter (nBin) > func2.GetXmax() ) {cerr<<"Function ranges don't match (1 > 2)"<<endl;}

  for (int binx=1; binx<=nBin; ++binx) { // ignore under/overflow bins
    Axis_t x = ph->GetBinCenter (binx);
    Double_t y1 = func1.Eval (x);
    Double_t y2 = func2.Eval (x);
    ph->SetBinError   (binx, TMath::Abs(y1-y2)/2. );
    ph->SetBinContent (binx, (y1+y2)/2.);
  }
  return ph;
}


// ==============================================================================

// returns a histogram with the statistical uncertainties on the fitted
// polynomial in the errors (and the fitted function in the bin contents)
// the function is available in the original histogram (controlled by fitOpt)
// drawOpt applies to the original histogram. "" means don't draw. "E1" looks nice
TH1* fitpol (TH1& h1, const char* funcName, Double_t xmin, Double_t xmax,
             const std::vector<int> powers, TString fitOpt = "ME0", TString drawOpt = "")
{
  unsigned int nTerms = powers.size();
  if (nTerms <= 0) {cerr<<"meaningless fit expression: nTerms<=0"<<endl; return 0;}

  // -- Prepare output histogram with no associated funtions
  TH1* ph = (TH1*) h1.Clone (Form("%s_fiterr", h1.GetName()));
  if (ph == 0) {cerr<<"fitpol can't allocate histogram"<<endl; return 0;}
  TList* pl = ph->GetListOfFunctions();
  if (pl) pl->Clear();
  ph->SetFillColor(3);
  ph->SetMarkerSize(0);

  // -- Prepare Formula

  // ROOT doesn't let you use polynomials when defining other functions
  // so not using polynomials.
  TString formula;
  for (unsigned int i=0; i<nTerms; ++i) {
    if (i>0) formula += "+";
    formula += Form ("[%d]*x", i);
    if (powers[i] == 2) {
      formula += "*x";
    } else if (powers[i] != 1) {
      formula += Form ("**%d", powers[i]);
    }
  }

  // -- Fit

  TF1* pf = new TF1 (funcName, formula.Data(), xmin, xmax);
  if (pf == 0) {cerr<<"fitpol can't allocate function"<<endl; return 0;}
  h1.Fit (pf, fitOpt, "", xmin, xmax);

  // -- Get Covariance Matrix

  // can only get to the covariance matrix through ROOT's globals
  TVirtualFitter *fitter = TVirtualFitter::GetFitter();
  TMatrixD matCov(nTerms, nTerms, fitter->GetCovarianceMatrix()); // M

  // get eigenvectors and eigenvalues
  TMatrixDEigen matEig (matCov);                                  // M (another representation)
  TMatrixD eigenVectors = matEig.GetEigenVectors();               // U
  TMatrixD matVal = matEig.GetEigenValues();                      // D
  TMatrixDDiag eVals (matVal);

  // -- Calculate errors and put them in output histogram

  TAxis* ax = ph->GetXaxis();
  for (int binx=ax->GetFirst()-1; binx<=ax->GetLast()+1; ++binx) { // also do overflow bins
    Axis_t x = ph->GetBinCenter (binx);
    Double_t vz =0;                                               // Var(z); z=y(x) for this specific x
    if (x < xmin || x > xmax) {
      vz = 0; // outside fit range
    } else {
      std::vector<Double_t> xpows (nTerms);
      //Double_t xpows [nTerms]; // the needed powers of x, to avoid function call in innermost loop.
      for (unsigned int it = 0; it<nTerms; ++it) xpows [it] = pow (x, powers[it]);

      for (unsigned int j = 0; j<nTerms; ++j) { // loop over eigenvectors. Get contribution from each one
        Double_t coeff = 0;
        for (unsigned int i = 0; i<nTerms; ++i) { // loop over terms, get each one's contribution to the eigenvector's coeff
          coeff += eigenVectors[i][j] * xpows[i];
          //if (binx == 7) {cout<<"DBG x: "<<x<<" ev["<<i<<"]["<<j<<"] = "<<eigenVectors[i][j]
          //          <<" *  "<<xpows[i]<<" --> "<<coeff<<endl;}
        }
        vz += coeff*coeff * eVals (j); // eigenValues are variances, not sigmas...
        //if (binx == 7) {cout<<"DBG x: "<<x<<" eVals["<<j<<"] = "<<eVals[j]<<" --> "<<vz<<endl;}
      }
      if (vz > 0) vz = sqrt (vz);
    }
    ph->SetBinError (binx, vz);
    ph->SetBinContent (binx, pf->Eval(x));
  }
  if (drawOpt.Length()) {
    h1.Draw(drawOpt);
    ph->Draw("E3same");
    h1.Draw(Form("same%s", drawOpt.Data()));
  }
  return ph;
}

TH1* fitpol (TH1& h1, const char* funcName, Double_t xmin, Double_t xmax,
             const int nTerms, int powers[], TString fitOpt = "ME0", TString drawOpt = "")
{
  std::vector<int> vpowers (nTerms);
  for (int it=0; it<nTerms; ++it) vpowers.push_back(powers[it]);
  return fitpol (h1, funcName, xmin, xmax, vpowers, fitOpt, drawOpt);
}

TH1* fitpol (TH1& h1, const char* funcName, Double_t xmin, Double_t xmax,
             const int nTerms, int pow0 = 0, int pow1 = 1, int pow2 = 2,
             int pow3 = 3, int pow4 = 4, int pow5 = 5, int pow6 = 6, int pow7 = 7, int pow8 = 8)
{
  int Nused = TMath::Min (9, nTerms);
  std::vector<int> powers (Nused);
  if (Nused > 0) powers[0] = pow0;
  if (Nused > 1) powers[1] = pow1;
  if (Nused > 2) powers[2] = pow2;
  if (Nused > 3) powers[3] = pow3;
  if (Nused > 4) powers[4] = pow4;
  if (Nused > 5) powers[5] = pow5;
  if (Nused > 6) powers[6] = pow6;
  if (Nused > 7) powers[7] = pow7;
  if (Nused > 8) powers[8] = pow8;
  return fitpol (h1, funcName, xmin, xmax, powers);
}

TH1* fitpol (TH1& h1, const char* funcName, Double_t xmin, Double_t xmax, TString fitOpt, TString drawOpt,
             const int nTerms, int pow0 = 0, int pow1 = 1, int pow2 = 2,
             int pow3 = 3, int pow4 = 4, int pow5 = 5, int pow6 = 6, int pow7 = 7, int pow8 = 8)
{
  int Nused = TMath::Min (9, nTerms);
  std::vector<int> powers (Nused);
  if (Nused > 0) powers[0] = pow0;
  if (Nused > 1) powers[1] = pow1;
  if (Nused > 2) powers[2] = pow2;
  if (Nused > 3) powers[3] = pow3;
  if (Nused > 4) powers[4] = pow4;
  if (Nused > 5) powers[5] = pow5;
  if (Nused > 6) powers[6] = pow6;
  if (Nused > 7) powers[7] = pow7;
  if (Nused > 8) powers[8] = pow8;
  return fitpol (h1, funcName, xmin, xmax, powers, fitOpt, drawOpt);
}


// ==============================================================================
// Short: Fits func to h1, with fit errors

// Long: Adds to the input histogram's object list the fitted function (as usual
// in fitting this is controlled by fitOpt), and also a new histogram with
// the statistical (fit) uncertainties on the
// fitted function (whose name is the original+suffix).
// drawOpt applies to the original histogram. "" means don't draw. "E1" looks nice

// if changing the fitted function according to a variable's uncertainties
// changes it in the same direction when the variable is changes up or down,
// it's either a roundoff errors or a real problem. If differences are below
// errThreshold, this is assumed to be legit.

// Caveat1: only evaluates differences at 1 variation, given by "dist"
// Caveat2: for now can not handle fixed parameters
TH1* fitWithErr (TH1& h1, TF1& func, Double_t xmin, Double_t xmax,
                 TString fitOpt = "ME0", TString drawOpt = "", TString suffix = "_fiterr",
                 float dist = 1., int nMinuitFits = 1, float errThreshold = 1E-6, int debugLevel=0)
{
  if (dist <= 0) {cerr<<"fitWithErr aborts! input dist is non-positive."<<endl; return 0;}
  if (nMinuitFits <= 0) {cerr<<"fitWithErr aborts! input nMinuitFits is non-positive."<<endl; return 0;}
  int Nf = func.GetNumberFreeParameters();
  if (Nf < 1) {cerr<<"fitWithErr aborts! # of free parameters is non-positive."<<endl; return 0;}
  int Npar = func.GetNpar();
  if (Npar != Nf) {cerr<<"fitWithErr aborts! Fixed parameters NYI."<<endl; return 0;}

  // -- Prepare indices to free parameters - NYI

  // -- Prepare output histogram with no associated funtions, same binning as h1 & range from xmin to xmas.
  Int_t iMin = h1.FindBin (xmin);
  Int_t iMax = h1.FindBin (xmax);
  if (h1.GetBinCenter (iMin) < xmin) ++iMin;
  if (h1.GetBinCenter (iMax) > xmax) --iMax;
  if (iMin < 1) iMin = 1;
  if (iMax > h1.GetNbinsX()) iMax = h1.GetNbinsX();
  if (iMax < iMin) {cerr<<"fitWithErr can't figure out the range "<<iMin<<" to "<<iMax<<"?!"<<endl; return 0;}
  Axis_t* edges = new Axis_t [2+iMax-iMin]; // need to handle possible variable binning
  for (int ib = iMin; ib<iMax+2; ++ib) edges [ib-iMin] = h1.GetBinLowEdge (ib);
  TH1F* ph = new TH1F (Form("%s%s", h1.GetName(), suffix.Data()), h1.GetTitle(),
                       1+iMax-iMin, edges);
  delete[] edges;
  if (ph == 0) {cerr<<"fitWithErr can't allocate histogram"<<endl; return 0;}
  TList* pl = ph->GetListOfFunctions();
  if (pl) pl->Clear();
  ph->SetFillColor(3);
  ph->SetMarkerSize(0);

  // -- Fit
  // note range, which is forced to be consistent with the new histogram despite possible rounding errors (which do differ!)
  for (int ifit=0; ifit<nMinuitFits; ++ifit) h1.Fit (&func, fitOpt, "", h1.GetBinLowEdge (iMin), h1.GetBinLowEdge (1+iMax));

  // -- Get Covariance Matrix

  // can only get to the covariance matrix through ROOT's globals
  TVirtualFitter *fitter = TVirtualFitter::GetFitter();
  TMatrixD matCov(Npar, Npar, fitter->GetCovarianceMatrix());     // M  (if Nf<Npar, only upper left corner is meaningful)

  // get eigenvectors and eigenvalues
  TMatrixDEigen matEig (matCov);                                  // M (another representation)
  TMatrixD eigenVectors = matEig.GetEigenVectors();               // U
  TMatrixD matVal = matEig.GetEigenValues();                      // D
  TMatrixDDiag eVals (matVal);
  if (debugLevel > 1) {
    cout<<"DBG eigen values: ";
    for (int id=0; id<eVals.GetNdiags(); ++id) cout<<eVals[id]<<" ";
    cout<<", eigenVectors: "<<endl;
    eigenVectors.Print();
  }
  // -- Prepare functions for numerically estimating the deriviatives
  std::vector<TF1*> ups (Nf);
  std::vector<TF1*> downs (Nf);
  for (int iv=0; iv<Nf; ++iv) {
    ups  [iv] = (TF1*) func.Clone (Form("%s_p%du", func.GetName(), iv));
    downs[iv] = (TF1*) func.Clone (Form("%s_p%dd", func.GetName(), iv));
    if (ups[iv] == 0 || downs[iv] == 0) {cerr<<"Allocation failed in fitWithErr."<<endl; return 0;}

    if (func.GetParError (iv) <= 0) {cerr<<"fitWithErr aborts! error on par #"<<iv<<" is non-positive."<<endl; return 0;}
    ups  [iv]->SetParameter(iv, func.GetParameter (iv) + func.GetParError (iv) * dist);
    downs[iv]->SetParameter(iv, func.GetParameter (iv) - func.GetParError (iv) * dist);
  }

  // -- Calculate errors and put them in output histogram

  TAxis* ax = ph->GetXaxis();
  for (int binx=ax->GetFirst(); binx<=ax->GetLast(); ++binx) { // also do overflow bins
    Axis_t x = ph->GetBinCenter (binx);
    Double_t vz =0;                                               // Var(z); z=y(x) for this specific x
    if (x < xmin || x > xmax) { // outside fit range
      cout<<"Warning from fitWithErr: Out of fit range in new histogram??? "<<xmin<<" <? "<<x<<" <? "<<xmax<<endl;
    }
    std::vector<Double_t> derivatives (Nf); // the needed derivatives of x can by taken outside of innermost loop.
    double y0 = func      .Eval (x);
    for (int iv = 0; iv<Nf; ++iv) {
      double yu = ups  [iv]->Eval (x);
      double yd = downs[iv]->Eval (x);
      derivatives [iv] = (yu-yd) / (2*dist*func.GetParError (iv));
      if (y0 - errThreshold > TMath::Max (yu, yd) ||
          y0 + errThreshold < TMath::Min (yu, yd) ) {
        cerr<<"fitWithErr failed to evaluate numerically the derivative of parameter: "<<iv
            <<" at x: "<<x<<". y0: "<<y0<<" up: "<<yu<<" down: "<<yd<<endl;
        return 0;
      }
    }

    for (int j = 0; j<Nf; ++j) { // loop over eigenvectors. Get contribution from each one
      Double_t coeff = 0;
      for (int i = 0; i<Nf; ++i) { // loop over terms, get each one's contribution to the eigenvector's coeff
        coeff += eigenVectors[i][j] * derivatives[i];
        if (debugLevel > 2 || (debugLevel && binx==7)) {
          cout<<"DBG x: "<<x<<" ev["<<i<<"]["<<j<<"] = "<<eigenVectors[i][j]
              <<" *  "<<derivatives[i]<<" --> "<<coeff<<endl;}
      }
      vz += coeff*coeff * eVals (j); // eigenValues are variances, not sigmas...
      if (debugLevel > 2 || (debugLevel && binx==7)) {cout<<"DBG x: "<<x<<" eVals["<<j<<"] = "<<eVals[j]<<" --> "<<vz<<endl;}
    }
    if (vz > 0) vz = sqrt (vz);
    else vz = 0;
    ph->SetBinError (binx, vz);
    ph->SetBinContent (binx, func.Eval(x));
    if (debugLevel > 1 || (debugLevel && binx==7)) {cout<<"DBG ph["<<binx<<"] = "<<ph->GetBinContent(binx)
                                                        <<" +/- "<<ph->GetBinError(binx)<<endl;}
  }
  if (drawOpt.Length()) {
    h1.Draw(drawOpt);
    ph->Draw("E3same");
    h1.Draw(Form("same%s", drawOpt.Data()));
  }
  /* Can't do this now, since h1.Draw() will then be superceded by the new histogram's draw func.
  TList* pl1 = h1.GetListOfFunctions();
  if (pl1) pl1->Add (ph);*/

  return ph;
}

// ===========================================================================================================









// Limitation: resulting errors are rubbish
TH2F* equalizeXstats (TH2& hin, const int nBins, TString name = "")
{
  if (nBins <= 0) {cerr<<"equalizeXstats input error: nBins ("<<nBins<<") <= 0!"<<endl; return 0;}

  Stat_t N = hin.GetEntries();
  if (N <= 0) {cerr<<"equalizeXstats can only handle positive entries. #Entries is non-positive."<<endl; return 0;}

  if (name.Length() == 0) {
    name = hin.GetName();
    name += "Q";
  }
  //TH2* hn = (TH2*) hin.Clone (name);
  //if (hn == 0) {cerr<<"equalizeXstats problem - Failed to clone input histogram - Allocation error"<<endl;  return 0;}
  TAxis *pay = hin.GetYaxis();
  if (pay == 0) {cerr<<"equalizeXstats problem - no y axis (?)"<<endl; return 0;}
  Double_t ymin = pay->GetXmin();
  Double_t ymax = pay->GetXmax();
  Int_t nbx  = hin.GetNbinsX();
  Int_t nby  = hin.GetNbinsY();
  if (nbx <= 0) {cerr<<"equalizeXstats problem - nbx: "<<nbx<<endl; return 0;}
  if (nby <= 0) {cerr<<"equalizeXstats problem - nby: "<<nby<<endl; return 0;}
  
  N = hin.Integral (1,nbx, 0, nby+1); // can handle overflows in y, but not in x
  if (N <= 0) {cerr<<"equalizeXstats can only handle positive entries. #Entries with legal 'x' is non-positive."<<endl; return 0;}

  Stat_t want = N / nBins; // includes overflows
  Double_t *xLows = new Double_t [nBins+1];
  xLows [0] = hin.GetBinLowEdge (1);
  Stat_t sumSoFar = 0;
  int iFilled = 0;
  double precision = 0.01 / nBins;
  
  for (int ix=1; ix <= nbx; ++ix) { // excluding overflows
    Stat_t integ = hin.Integral (ix,ix, 0,nby+1); // including overflows
    sumSoFar += integ;
    if (sumSoFar >= (1+iFilled) * want * (1-precision)) { // enough for the next* one?   * hence "1+"
      xLows [1+iFilled] = hin.GetBinLowEdge (ix+1);
      ++iFilled;
    }
    //    cout<<"DBG ix: "<<ix<<" integ: "<<integ<<" sumSoFar: "<<sumSoFar<<" iFilled: "<<iFilled<<endl;
  }

  if (iFilled < nBins) {
    xLows [1+iFilled] = hin.GetBinLowEdge (nbx+1); ++iFilled;
  }
  if (iFilled != nBins) {cerr<<"Bug! iFilled: "<<iFilled<<" != nBins: "<<nBins<<endl; return 0;}
  xLows [nBins] = hin.GetBinLowEdge (nbx+1); // force last slice to end at edge of original histogram

  //  cout<<"DBG Bin edges: ";
  for (int ib=0; ib<1+nBins; ++ib) cout<<xLows[ib]<<" ";
  cout<<endl;
  TH2F* hn = new TH2F (name, hin.GetTitle(), nBins, xLows, nby, ymin, ymax);
  hn->GetXaxis()->SetTitle( hin.GetXaxis()->GetTitle() );
  hn->GetYaxis()->SetTitle( hin.GetYaxis()->GetTitle() );

  // copy overflows
  for (int iy=0; iy <= nby+1; ++iy) { // including overflows
    hn->SetBinContent (0      , iy, hin.GetBinContent (0    , iy));
    hn->SetBinContent (1+nBins, iy, hin.GetBinContent (1+nbx, iy));
  }
  // sum up content
  int iFilling = 1;
  for (int ix=1; ix <= nbx; ++ix) { // excluding overflows
    if (hin.GetBinCenter (ix) > xLows[iFilling]) ++iFilling;
    //    cout<<"DBG ix: "<<ix<<" --> iFilling: "<<iFilling<<endl;
    if (hin.GetBinCenter (ix) > xLows[iFilling]) {cerr<<"Bug! old center["<<ix<<"] > new left["
						      <<iFilling<<"]"<<endl; return 0;}
    for (int iy=0; iy <= nby+1; ++iy) { // including overflows
      Stat_t prev = hn->GetBinContent (iFilling, iy);
      hn->SetBinContent (iFilling, iy, prev + hin.GetBinContent (ix, iy));
    }
  }
  if (iFilling != nBins) {cerr<<"Bug! iFilling != nBins after content loop"<<endl; return 0;}

  delete [] xLows;

  hn->SetEntries (hin.GetEntries());
  Stat_t stats[20]; // I think only 11 can possible be used for TH3. 4 really enough for TH1
  hin.GetStats (stats);
  hn->PutStats (stats);
  return hn;
}

// NYI: respecting variable width bins
TH1D* delta( const TH1& h1, TString name = "")
{
  if( h1.GetDimension() != 1) {cerr<<"ERROR! 'delta' works only for 1D histograms!"<<endl; return 0;}
  int nOutBins = -1 + h1.GetNbinsX();
  if( nOutBins <= 0) {cerr<<"ERROR! Not enough bins in histogram input to 'delta'"<<endl; return 0;}

  if( name.Length() == 0) {
    name = h1.GetName();
    name += "_delta";
  }
  TH1D* out = new TH1D( name, h1.GetTitle(), nOutBins,
			h1.GetBinCenter(1), h1.GetBinCenter(1 + nOutBins));

  for( int ib=1; ib<=nOutBins; ++ib) {
    out->SetBinContent( ib, h1.GetBinContent( 1+ib ) - h1.GetBinContent( ib ) );
    out->SetBinError  ( ib, TMath::Sqrt( TMath::Power( h1.GetBinError( 1+ib ), 2) + 
					 TMath::Power( h1.GetBinError( ib ), 2) ) );
  }
  out->GetXaxis()->SetTitle (h1.GetXaxis()->GetTitle());
  out->GetYaxis()->SetTitle (Form("#Delta%s", h1.GetYaxis()->GetTitle()));
  return out;
}

// NYI: respecting variable width bins
TH1D* delta( const TF1& f1, TString name = "")
{
  if (f1.GetHistogram() == 0) return 0;
  return delta( *f1.GetHistogram(), name );
}


// overflows in y direction are normalized as part of the slice
TH2* normalize_x_slices( TH2& hin, double norm = 0, TString name = "" )
{
  if( name.Length() == 0 ) {
    name = hin.GetName();
    name += "NX";
  }
  TH2 *out = ( TH2* ) hin.Clone( name );
  if( out == 0) { cout<<"ERROR! normalize_x_slices failed to clone input histogram"<<endl; return 0; }
  out->SetMaximum();
  out->SetMinimum();

  Int_t nbx  = hin.GetNbinsX();
  Int_t nby  = hin.GetNbinsY();
  
  if( norm == 0 ) norm = hin.Integral( 1, 1, 0, 1+nby ); // can handle overflows in y direction
  if( norm == 0 ) { cerr<<"ERROR! can't take normalization from first x slice (nothing there)."<<endl; return 0; }

  for( int ix=1; ix <= nbx; ++ix ) { // excluding overflows
    Stat_t integ = hin.Integral( ix, ix, 0, 1+nby ); // including overflows
    if( integ == 0 ) continue;

    double scale = norm / integ;

    for( int iy=0; iy <= nby+1; ++iy ) { // including overflows
      Stat_t prev = hin.GetBinContent( ix, iy );
      out->SetBinContent( ix, iy, prev * scale );
    }
  }
  return out;
}

// overflows in y direction are normalized as part of the slice
TH2* equalize_max_of_x_slices( TH2& hin, double norm = 1, TString name = "" )
{
  if( name.Length() == 0 ) {
    name = hin.GetName();
    name += "EX";
  }
  TH2 *out = ( TH2* ) hin.Clone( name );
  if( out == 0) { cout<<"ERROR! equalize_max_of_x_slices failed to clone input histogram"<<endl; return 0; }
  out->SetMaximum();
  out->SetMinimum();

  Int_t nbx  = hin.GetNbinsX();
  Int_t nby  = hin.GetNbinsY();

  for( int ix=1; ix <= nbx; ++ix ) { // excluding overflows

    TH1D* slice = hin.ProjectionY( "_py", ix, ix );
    if( 0 == slice ) { cerr<<"ERROR! equalize_max_of_x_slices failed to take a slice of "<<hin.GetName()<<endl; return 0;}
    slice->SetMaximum();
    Double_t max = slice->GetMaximum();
    
    if( max == 0 ) continue;

    double scale = norm / max;

    for( int iy=0; iy <= nby+1; ++iy ) { // including overflows
      Stat_t prev = slice->GetBinContent( iy );
      out->SetBinContent( ix, iy, prev * scale );
    }
    // do not deallocate slice, as TH2::DoProjection is optimized to reuse the histogram by name
  }
  return out;
}

#endif
