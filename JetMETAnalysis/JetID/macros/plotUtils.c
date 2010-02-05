#include <iostream>
#include <stdio.h>
#include "TRandom3.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TROOT.h"
#include "TH1.h"
#include "TH2F.h"

#include "histUtils.c"

using std::endl;
using std::cout;
using std::cerr;

TRandom3 g_test_rnd;

// leave enough space on top of a histogram for text and such
// a negative logyOpt meand the gPad is queried for its state
// a negative fraction gives ROOT default behavior (with the maximum unset)
bool set_top_fraction( TH1& h1, const double fraction, const int logyOpt = -1, const int iDebug = 0 )
{
  bool logy = logyOpt;
  if( logyOpt < 0 ) {
    if( gPad == 0 ) {
      cerr<<"ERROR set_top_fraction: Can't get logy from gPad since it's undefined"<<endl;
      return false;
    }
    logy = gPad->GetLogy();
  }

  h1.SetMaximum(); // unset
  if( fraction >= 0 ) {
    if( fraction > 0.97 ) {cerr<<"ERROR set_top_fraction: fraction is too big."<<endl; return false;}
    Stat_t minCurY = h1.GetYaxis()->GetXmin();
    Stat_t maxCurY = h1.GetMaximum();
    if( iDebug > 2 ) cout<<"top fraction: "<<fraction<<", curY: "<<minCurY<<" - "<<maxCurY<<endl;
    if( maxCurY > minCurY ) {
      if( logy ) {
	if( minCurY == 0 ) { // hard to figure out what ROOT does in this case,
	  minCurY = minPositiveEntry (h1) / 3.; // this is a decent approx.
	  h1.SetMinimum( minCurY );
	  if( iDebug > 2 ) cout<<"    ... minCurY: "<<minCurY<<endl;
	}
	if( minCurY > 0 ) {
	  float deltaY = TMath::Log( maxCurY ) - TMath::Log( minCurY );
	  h1.SetMaximum( minCurY * TMath::Exp( deltaY/(1-fraction) ));
	} else {
	  cerr<<"ERROR set_top_fraction: can't do it with logy options && minCurY<=0"<<endl;
	  return false;
	}
      } else {
	h1.SetMaximum( minCurY + (maxCurY - minCurY) / (1-fraction) );
      }
    }
    if (iDebug > 2) cout<<"'------> "<<h1.GetMaximum()<<endl;
  }
  return true;
}

void test_set_top_fraction ()
{
  bool rc;

  TCanvas* c7 = new TCanvas ("topFracCanvas", "Testing set_top_fraction");
  c7->Divide(3, 2);
  TH1F* h1 = new TH1F( "h1", "h1", 10, 0, 1 );
  TH1F* h2 = new TH1F( "h2", "h2", 10, 0, 1 );
  for (int ie=0; ie<200; ++ie) {
    float x = g_test_rnd.Gaus (0.5,0.2);
    h1->Fill (x);
    x = g_test_rnd.Gaus (0.4,0.1);
    h2->Fill (x);
  }
  c7->cd(1);
  rc = set_top_fraction( *h1, 0.4, false);
  if( !rc ) {cerr<<"test #1 failed"<<endl; return;}
  h1->Draw();

  c7->cd(2);
  gPad->SetLogy(true);
  rc = set_top_fraction( *h2, 0.3, true);
  if( !rc ) {cerr<<"test #2 failed"<<endl; return;}
  h2->Draw();

  TH1F* h3 = (TH1F*) h1->Clone("h3");
  TH1F* h4 = (TH1F*) h2->Clone("h4");

  c7->cd(3);
  rc = set_top_fraction( *h3, 0.2 );
  if( !rc ) {cerr<<"test #3 failed"<<endl; return;}
  h3->Draw();

  c7->cd(4);
  gPad->SetLogy(true);
  rc = set_top_fraction( *h4, 0.1 );
  if( !rc ) {cerr<<"test #4 failed"<<endl; return;}
  h4->Draw();

  TH1F* h5 = (TH1F*) h1->Clone("h5");
  TH1F* h6 = (TH1F*) h2->Clone("h6");

  c7->cd(5);
  rc = set_top_fraction( *h5, 0 );
  if( !rc ) {cerr<<"test #5 failed"<<endl; return;}
  h5->Draw();

  c7->cd(6);
  gPad->SetLogy(true);
  rc = set_top_fraction( *h6, -0.1 );
  if( !rc ) {cerr<<"test #6 failed"<<endl; return;}
  h6->Draw();
  double max6 = h6->GetMaximum();
  h6->SetMaximum();
  if( max6 != h6->GetMaximum() ) {cerr<<"test #6 failed - max was set"<<endl; return;}

  cerr<<"Should get error messages from set_top_fraction now"<<endl;
  TH1F* h7 = (TH1F*) h1->Clone("h7");
  rc = set_top_fraction( *h7, 1.1 );
  if( rc ) {cerr<<"failed to recognize frac>1 error"<<endl; return;}
  rc = set_top_fraction( *h7, 0.999 );
  if( rc ) {cerr<<"failed to recognize frac~1 error"<<endl; return;}
}

// to allow displaying overlaid histograms with an offset
// offset is given in units of bin width (for bin #1)
TH1* cloneWithOffset( const TH1& hist, const float offset, const TString newname = "" )
{
  if( offset < -0.5 || offset > 0.5 ) {
    cerr<<"cloneWithOffset is implemented in a very lazy way that only works for |offset|<0.5 ("<<offset<<")"<<endl;
    return 0;
  }
  double dx = offset * hist.GetBinWidth( 1 );
  return cloneRange( &hist, hist.GetXaxis()->GetXmin() + dx, hist.GetXaxis()->GetXmax() + dx, newname );
}


void clear_stat_box( TH1& hist )
{
  // The catch - can't delete objects that are being displayed
  if( gPad ) gPad->Delete();
  
  TList* list = hist.GetListOfFunctions();
  if( list == 0 ) return;
  for( int index = -1 + list->GetEntries(); index >= 0; --index ) {
    if( list->At( index )->InheritsFrom( "TPaveStats") ) list->RemoveAt( index );
  } 
}


// integrated tests, but checking graphical output automatically is a bit much :-(
void plotUtils () {
  test_set_top_fraction ();
}
