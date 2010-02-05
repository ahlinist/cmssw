#ifndef AH_JCUTILS
#define AH_JCUTILS

#include <iostream>
#include <stdio.h>
#include "TMath.h"
#include "TROOT.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH3F.h"
#include "TCanvas.h"
#include "TSystem.h"
#include "TRegexp.h"
#include "TLegend.h"
#include "TLine.h"
#include "TKey.h"
#include "TCut.h"
#include "TLatex.h"
#include "TArrow.h"
#include "TPaveStats.h"
#include "TPaletteAxis.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
//#include "TMultiGraph.h"
//#include "THStack.h"
#include "TProfile.h"
#include "TStyle.h"
//#include "TPostScript.h"
#include "TNtuple.h"
#include "TRandom3.h"

#include <limits>
//#include "stdlib.h"
#include <string>
#include <sstream>
#include <map>
#include <iterator>

#include "myMathUtils.c"
//#include "mystyle.c"
#include "histUtils.c"
#include "rangeUtils.c"
#include "debugUtils.c"

using std::cout;
using std::endl;
using std::cerr;
using std::vector;


// Functions to be defined before the main in jc.c and/or nj.c

// ---------------------------------------------------------------------------------------------
// The SLAC version
bool relocateStatBox (float x1, float y1, float x2=-1, float y2=-1, TString newname = "") 
{ 
  TPaveStats *st = (TPaveStats*)gPad->GetPrimitive("stats");
  if (0 == st) {cerr<<"Can't find stat box in pad"<<endl; return 0;}
  if (newname.Length() > 0) st->SetName(newname);
  st->SetX1NDC(x1);
  if (x2 != -1) st->SetX2NDC(x2);
  st->SetY1NDC(y1);
  if (y2 != -1) st->SetY2NDC(y2);
  st->Draw();
  return 1;
}
// Resize any histograms (they all inherit from h1) stat box. Uses NDC coordinates (e.g. 0.65, 0.5)
bool resizeStatBox( TCanvas *c1, TH1* h1, float x1, float y1, float x2=-1, float y2=-1, 
		    TString newname = "", int color = -1, TString oldname = "", int iDebug = 0 ) 
{ 
  if( !c1 || !h1 ) {cerr<<"Null pointer given to resizeStatBox!"<<endl; return 0;}
  c1->Update();
  if( oldname.Length() <= 0 ) oldname = "stats";
  if( iDebug > 20 ) print_list_of_functions( *h1 );
  TPaveStats *stats = (TPaveStats*) h1->FindObject( oldname );
  if( iDebug ) cout<<"Found old name (\""<<oldname<<"\"? "<<( stats != 0 )<<endl;
  if( (!stats) && newname.Length() > 0 ){ // maybe it was already renamed
    stats = (TPaveStats*) h1->FindObject( newname );
    if( iDebug ) cout<<"Found new name (\""<<newname<<"\"? "<<(stats != 0)<<endl;
  }
  if( !stats ) {cerr<<"Can't find stat box"<<endl; return 0;}
  stats->SetX1NDC( x1 );
  stats->SetY1NDC( y1 );
  if( x2 >= 0 ) stats->SetX2NDC( x2 );
  if( y2 >= 0 ) stats->SetY2NDC( y2 );
  if( newname.Length() > 0 ) {
    stats->SetName( newname );
    if( iDebug ) cout<<"SetName to "<<newname<<endl;
  }
  if( color != -1 ) stats->SetTextColor (color);
  stats->Draw(); // maybe, just maybe, this will finally make them appear every time, even with draw "same"
  return 1;
}

bool resizeStatBox (TCanvas *c1, TGraph* g1, float x1, float y1, float x2=-1, float y2=-1) 
{ 
  if (!c1 || !g1) {cerr<<"Null pointer given to resizeStatBox!"<<endl; return 0;}
  c1->Update();
  TPaveStats *stats = (TPaveStats*) g1->FindObject("stats");
  if (!stats) {cerr<<"Can't find stat box"<<endl; return 0;}
  stats->SetX1NDC(x1);
  stats->SetY1NDC(y1);
  if (x2 >= 0) stats->SetX2NDC(x2);
  if (y2 >= 0) stats->SetY2NDC(y2);
  return 1;
}

// ---------------------------------------------------------------------------------------------
// Note: TGraphAsymmErrors and TGraphErrors are merely siblings, and joint interface doesn't provide what we need
Double_t getGraphsGraphicalMax (TGraphErrors* g)
{
  Double_t max = g->GetMean(2) - 1; // something's got to be average or above
  int N = g->GetN();
  Double_t *vals = g->GetY();
  Double_t *errs = g->GetEY();
  for (int i=0; i<N; ++i) {
    Double_t cur = vals[i] + errs[i];
    if (cur > max) max = cur;
  }
  return max;
}

Double_t getGraphsGraphicalMax (TGraphAsymmErrors* g)
{
  Double_t max = g->GetMean(2) - 1; // something's got to be average or above
  int N = g->GetN();
  Double_t *vals = g->GetY();
  Double_t *errs = g->GetEYhigh();
  for (int i=0; i<N; ++i) {
    Double_t cur = vals[i] + errs[i];
    if (cur > max) max = cur;
  }
  return max;
}

Double_t getGraphsGraphicalMin (TGraphErrors* g)
{
  Double_t min = g->GetMean(2) + 1; // something's got to be average or below
  int N = g->GetN();
  Double_t *vals = g->GetY();
  Double_t *errs = g->GetEY();
  for (int i=0; i<N; ++i) {
    Double_t cur = vals[i] - errs[i];
    if (cur < min) min = cur;
  }
  return min;
}

Double_t getGraphsGraphicalMin (TGraphAsymmErrors* g)
{
  Double_t min = g->GetMean(2) + 1; // something's got to be average or below
  int N = g->GetN();
  Double_t *vals = g->GetY();
  Double_t *errs = g->GetEYlow();
  for (int i=0; i<N; ++i) {
    Double_t cur = vals[i] - errs[i];
    if (cur < min) min = cur;
  }
  return min;
}

// ---------------------------------------------------------------------------------------------
TH1F* getTH1F (TString name, TFile& file)
{
  if (! file.IsOpen()) {cerr<<"Can't getTH1F from a file that isn't open"<<endl; return 0;}
  TH1F* h1 = (TH1F*) file.Get (name);
  if (h1 == 0) {cerr<<"Can't find TH1F histogram \""<<name<<"\""<<endl;} // no way to return to command prompt !%$!@#$&!*@#$
  return h1;
}


// ---------------------------------------------------------------------------------------------

// refactored version now in histUtils
//Axis_t lowestNonZeroBinsHighEdge (TH1F& h)
//{
//  TAxis* ax = h.GetXaxis();
//  for (int binx=ax->GetFirst(); binx<=ax->GetLast(); ++binx) {
//    Double_t c = TMath::Abs (h.GetBinContent(binx));
//    if (c>0) return h.GetBinLowEdge (binx+1);
//  }
//  return h.GetBinLowEdge (ax->GetLast()+2);
//}


// This method assumes that statistical fluctuation in passing events are uncorrelated with
// those for failing events, which is normally the case when you get your events from data.
// This is in contrast to TGraphAsymmErrors Bayes constructor which assumes full anti-correlation,
// which is the case when you start with an MC sample and then categorize events as passing and failing.

// nameSuffix is appended to histograms name, making the graph's name
TGraphAsymmErrors* makeEffGraph (TH1F& h, TString nameSuffix = "eff")
{
  int n = 1+h.GetNbinsX();

  vector<Double_t> ps(n); // a partial sum: integral of h from -infinity to high edge of bin n, counted TH1 style
  for (int i=0; i<n; ++i) {
    ps [i] = h.GetBinContent (i) + (i>0 ? ps [i-1] : 0);
  }

  Double_t *x = new Double_t [n], *y = new Double_t [n], *exl = new Double_t [n], *exh = new Double_t [n];
  Double_t *eyl = new Double_t [n], *eyh = new Double_t [n]; // for the graph

  for (int i=0; i<n; ++i) {
    exl [i] = exh [i] = 0;
    x [i] = h.GetBinLowEdge (i+1); // low edge of nBins+1 is the right edge of h.
    Double_t eff, deff;
    calcPoisEff (ps[n-1] - ps[i], ps[i], eff, deff);
    y [i] = eff;
    makeAsymmEffErrors (eff, deff, eyl [i], eyh [i]);
  }
  TGraphAsymmErrors *gae = new TGraphAsymmErrors (n, x, y, exl, exh, eyl, eyh);
  gae->SetTitle (h.GetTitle());
  gae->GetXaxis()->SetTitle (h.GetXaxis()->GetTitle());
  gae->GetYaxis()->SetTitle ("Efficiency");
  gae->SetName (TString (h.GetName()) + nameSuffix);

  delete[] x;
  delete[] y;
  delete[] exl;
  delete[] exh;
  delete[] eyl;
  delete[] eyh;
  return gae;
}

// ---------------------------------------------------------------------------------------------
TH1F* makeEffHist (TH1F& good, TH1F& bad, TString nameSuffix = "_gob") // Good Over Both
{
  if (good.GetNbinsX() != bad.GetNbinsX()) {cerr<<"NbinsX differed in makeEffHist"<<endl; return 0;}
  int n = 2+good.GetNbinsX(); // over and underflows
  TH1F* hnew = (TH1F*) good.Clone(Form("%s%s", good.GetName(), nameSuffix.Data()));
  if (!hnew) {cerr<<"Failed to clone histogram in makeEffHist"<<endl; return 0;}
  hnew->SetNormFactor(0); // good's display options gets cloned and are irrelevant
  for (int i=0; i<n; ++i) {
    Double_t a, b, da, db, eff, deff;
    a  = good.GetBinContent (i);
    b  = bad .GetBinContent (i);
    da = good.GetBinError   (i);
    db = bad .GetBinError   (i);
    if (a==0 && da==0 && b>0) da=TMath::Min(1.,db);
    if (b==0 && db==0 && a>0) db=TMath::Min(1.,da);
    bool OK = calcEff (a, b, da, db, eff, deff);
    if (!OK) {eff = 0.5; deff = 0.5;}
    hnew->SetBinContent (i, eff);
    hnew->SetBinError   (i, deff);
  }
  return hnew;
}

TH1D* makeEffHistD (TH1F& good, TH1F& bad, TString nameSuffix = "_gob") // Good Over Both
{
  TH1F* hf =  makeEffHist (good, bad, Form("%sF",nameSuffix.Data()));
  if (hf == 0) return 0; // error message must've been given
  return convertToTH1D (hf, Form("%s%s", good.GetName(), nameSuffix.Data()));
}

// ---------------------------------------------------------------------------------------------
TGraphAsymmErrors* contrastEffs (TGraphAsymmErrors& gse, TGraphAsymmErrors& gbe)
{
  int n = gse.GetN();
  if (n != gbe.GetN()) {cerr<<"graph definitions differ in contrastEffs"<<endl; return 0;}

  Double_t *x = gse.GetY();
  Double_t *exl = gse.GetEYlow();
  Double_t *exh = gse.GetEYhigh();
  Double_t *y = gbe.GetY();
  Double_t *eyl = gbe.GetEYlow();
  Double_t *eyh = gbe.GetEYhigh();

  TGraphAsymmErrors *gae = new TGraphAsymmErrors (n, x, y, exl, exh, eyl, eyh);
  gae->SetTitle (gse.GetTitle());
  gae->GetXaxis()->SetTitle (gse.GetYaxis()->GetTitle());
  gae->GetYaxis()->SetTitle (gbe.GetYaxis()->GetTitle());
  gae->SetName (TString (gse.GetName()) + "vs" + TString (gbe.GetName()));
  return gae;
}

// ---------------------------------------------------------------------------------------------
TString stripStart( const TString& in ) {
  int lastOFirstWhiteSpaces = in(TRegexp("^[ \t\n]*")).Length();
  return in( lastOFirstWhiteSpaces, in.Length() );
}

TString stripEnd( const TString& in ) {
  int firstOfLastWhiteSpaces = in.Index(TRegexp("[ \t\n]*$"));
  if( firstOfLastWhiteSpaces < 0 ) firstOfLastWhiteSpaces = in.Length();
  return in( 0, firstOfLastWhiteSpaces );
}

bool allInBrackets( const TString& in, char opening_bracket = '(', char closing_bracket = ')' ) 
{
  int n_in = 0;
  for( int ii = 0; ii<in.Length(); ++ii) {
    char c = in[ii]; 
    if( c == opening_bracket ) {
      ++n_in; 
    } else if( c == closing_bracket ) {
      --n_in;
    } else if( c != ' ' && n_in <= 0 ) {
      return false;
    }
  }
  return true;
}

// ---------------------------------------------------------------------------------------------
TString weightCut (TCut cut, bool needMCweight, int treeVersion = 2, bool pTweighted = false)
{
  TString sWMC (treeVersion <= 1 ? "xsec" : "w");
  TString sPT (treeVersion <= 2 ? "jtpt" : "jtrawpt");
  TString title( stripStart( stripEnd( cut.GetTitle() ) ) );
  
  TString title_in_parenthesis( title );
  if( ! allInBrackets( title ) ) title_in_parenthesis = "("+title+")";
  if( needMCweight || pTweighted ) { // need some sort of weight
    TString sW;
    if( needMCweight && !pTweighted ) sW = sWMC.Data();
    if( pTweighted && !needMCweight ) sW = sPT.Data();
    if( needMCweight && pTweighted )  sW = Form("(%s*%s)", sWMC.Data(), sPT.Data());
    if( title.IsWhitespace() ) {
      return sW;
    } else {
      return title_in_parenthesis + "*" + sW;
    }
  } else { // no weights needed. 
    // But this is a cut, not a weigh, so add parenthesis and a test to prevent weighting
    if( title.IsWhitespace() ) return "";
    return title_in_parenthesis+">0";
  }
}

int currentUnits( TH1& h1 )
{
  TString ss( h1.GetYaxis()->GetTitle() );
  ss.ReplaceAll ("_{T}", "_%@#"); // otherwise it messes up the reg. exp.
  // all backslashes are doubled, to get by the compiler string parsing, leaving \[ for TRegexp --> literal [s & ]s
  int iOpenBracket = ss.Index (TRegexp ("\\[[^\\]]*\\][^\\[\\]]*$"));// That is, the start of the last square brackets
  int iCloseBracket = ss.Last(']'); // no need for quotes with char
  if( iOpenBracket < 0 || iCloseBracket <= iOpenBracket ) {
    // can't find units
    return -666;
  }

  // parse input units
  TString inputUnits = ss( 1+iOpenBracket, -1 + iCloseBracket - iOpenBracket);
  int curPrefix = 0;
  if( inputUnits.BeginsWith( "m"   ) ) curPrefix = -3;
  if( inputUnits.BeginsWith( "#mu" ) ) curPrefix = -6;
  if( inputUnits.BeginsWith( "n"   ) ) curPrefix = -9; 
  if( inputUnits.BeginsWith( "p"   ) ) curPrefix = -12;
  if( inputUnits.BeginsWith( "f"   ) ) curPrefix = -15;
  if( inputUnits.BeginsWith( "k"   ) ) curPrefix = 3;
  return curPrefix;
}

// ---------------------------------------------------------------------------------------------
// returns scaling (change in normalization, in log 10) or -666 if failed
// optional parameter accepts the desired unit prefix (in log 10)
int adjustUnits( TH1& h1, int forceNewPrefix = -666)
{
  if( h1.GetDimension() != 1) {cerr<<"ERROR! 'adjustUnits' works only for 1D histograms!"<<endl; return -666;}
  bool forced = forceNewPrefix != -666;
  TString ss( h1.GetYaxis()->GetTitle() );
  ss.ReplaceAll ("_{T}", "_%@#"); // otherwise it messes up the reg. exp.
  // all backslashes are doubled, to get by the compiler string parsing, leaving \[ for TRegexp --> literal [s & ]s
  int iOpenBracket = ss.Index (TRegexp ("\\[[^\\]]*\\][^\\[\\]]*$"));// That is, the start of the last square brackets
  int iCloseBracket = ss.Last(']'); // no need for quotes with char
  if( iOpenBracket < 0 || iCloseBracket <= iOpenBracket ) {
    // can't find units
    return -666;
  }
  Double_t oldMax = h1.GetMaximum();
  h1.SetMaximum(); // unsets
  Double_t max = h1.GetMaximum();
  Double_t max10 = TMath::Log10( max );
  if( -3 < max10 && max10 < 3 && ! forced ) return 0; // no rescaling needed
  
  // parse input units
  TString inputUnits = ss( 1+iOpenBracket, -1 + iCloseBracket - iOpenBracket);
  int curPrefix = 0, prefixLength = 0;
  if( inputUnits.BeginsWith( "m" ) ) {curPrefix = -3; prefixLength = 1;}
  if( inputUnits.BeginsWith( "#mu" ) ) {curPrefix = -6; prefixLength = 3;}
  if( inputUnits.BeginsWith( "n" ) ) {curPrefix = -9; prefixLength = 1;}
  if( inputUnits.BeginsWith( "p" ) ) {curPrefix = -12; prefixLength = 1;}
  if( inputUnits.BeginsWith( "f" ) ) {curPrefix = -15; prefixLength = 1;}
  if( inputUnits.BeginsWith( "k" ) ) {curPrefix = 3; prefixLength = 1;}
  // add more as needed....
  TString baseUnit( inputUnits( prefixLength, inputUnits.Length() - prefixLength ) );

  // find target units
  int iNewPrefix = 3 * TMath::Floor( 0.5 + (curPrefix + max10)/3. );
  if( forced ) iNewPrefix = forceNewPrefix;

  // prepare new units and scaling
  TString sNewPrefix;
  int scale10 = 1;
  if( iNewPrefix > 3 ) {cerr<<"adjustUnits - NYI for >kilo"<<endl;}
  if( iNewPrefix >= 3 ) {sNewPrefix = "k"; scale10 = curPrefix - 3;}

  if( -15 < iNewPrefix && iNewPrefix < 3 ) scale10 = curPrefix- iNewPrefix;
  if( iNewPrefix == 0   ) sNewPrefix = "";
  if( iNewPrefix == -3  ) sNewPrefix = "m";
  if( iNewPrefix == -6  ) sNewPrefix = "#mu";
  if( iNewPrefix == -9  ) sNewPrefix = "n";
  if( iNewPrefix == -12 ) sNewPrefix = "p";

  if( iNewPrefix < -15 ) {cerr<<"adjustUnits - NYI for <femto"<<endl;}
  if( iNewPrefix <= -15 ) {sNewPrefix = "f"; scale10 = curPrefix + 15;}
  
  // sanity checks
  if( forced && forceNewPrefix != 0 && sNewPrefix == "" ) {
    cerr<<"adjustUnits - illegal new prefix forced!"<<endl;
    return -666;
  }
  if( scale10 == 0 && ! forced ) {cerr<<"Bug in adjustUnits? scale10 == 0 .... "<<endl; return -666;}

  // good to go - changing the histogram
  Double_t scale = TMath::Power( 10, scale10 );
  h1.Scale( scale );
  h1.SetMaximum( oldMax * scale );
  h1.GetYaxis()->SetTitle( ss( 0, iOpenBracket + 1 ) + sNewPrefix + baseUnit 
			   + ss( iCloseBracket, ss.Length() - iCloseBracket ) );
  return scale10;
}
#endif
