#include "RecoBTag/Analysis/interface/EffPurFromHistos.h"
#include "RecoBTag/Analysis/src/Tools.h"

#include "TStyle.h"
#include "TCanvas.h"

#include <iostream>
using namespace std;



EffPurFromHistos::EffPurFromHistos ( TString ext, TH1F * h_d, TH1F * h_u,
	TH1F * h_s, TH1F * h_c, TH1F * h_b, TH1F * h_g,	TH1F * h_ni,
	TH1F * h_dus, TH1F * h_dusg, int nBin, float startO, float endO) :
	//BTagPlotPrintC(),
	histoExtension(ext), effVersusDiscr_d(h_d), effVersusDiscr_u(h_u),
	effVersusDiscr_s(h_s), effVersusDiscr_c(h_c), effVersusDiscr_b(h_b),
	effVersusDiscr_g(h_g), effVersusDiscr_ni(h_ni), effVersusDiscr_dus(h_dus),
	effVersusDiscr_dusg(h_dusg), nBinOutput(nBin), startOutput(startO),
	endOutput(endO), plotCanvas(0)
{
  // consistency check
  if ( !(check()) ) {
    cout << "EffPurFromHistos::EffPurFromHistos : consistency check failed -> exit!!" << endl;
    exit (1);
  }


  // to have shorter names ......
  TString & hE = histoExtension;
  TString hB = "FlavEffVsBEff_";


  // create histograms from base name and extension as given from user
  // BINNING MUST BE IDENTICAL FOR ALL OF THEM!!
  EffFlavVsBEff_d    = new TH1F ( hB + "D"    + hE , hB + "D"    + hE , nBinOutput , startOutput , endOutput );
  EffFlavVsBEff_u    = new TH1F ( hB + "U"    + hE , hB + "U"    + hE , nBinOutput , startOutput , endOutput ) ;
  EffFlavVsBEff_s    = new TH1F ( hB + "S"    + hE , hB + "S"    + hE , nBinOutput , startOutput , endOutput ) ;
  EffFlavVsBEff_c    = new TH1F ( hB + "C"    + hE , hB + "C"    + hE , nBinOutput , startOutput , endOutput ) ;
  EffFlavVsBEff_b    = new TH1F ( hB + "B"    + hE , hB + "B"    + hE , nBinOutput , startOutput , endOutput ) ;
  EffFlavVsBEff_g    = new TH1F ( hB + "G"    + hE , hB + "G"    + hE , nBinOutput , startOutput , endOutput ) ;
  EffFlavVsBEff_ni   = new TH1F ( hB + "NI"   + hE , hB + "NI"   + hE , nBinOutput , startOutput , endOutput ) ;
  EffFlavVsBEff_dus  = new TH1F ( hB + "DUS"  + hE , hB + "DUS"  + hE , nBinOutput , startOutput , endOutput ) ;
  EffFlavVsBEff_dusg = new TH1F ( hB + "DUSG" + hE , hB + "DUSG" + hE , nBinOutput , startOutput , endOutput ) ;
}


EffPurFromHistos::~EffPurFromHistos () {
  delete EffFlavVsBEff_d   ;
  delete EffFlavVsBEff_u   ;
  delete EffFlavVsBEff_s   ;
  delete EffFlavVsBEff_c   ;
  delete EffFlavVsBEff_b   ;
  delete EffFlavVsBEff_g   ;
  delete EffFlavVsBEff_ni  ;
  delete EffFlavVsBEff_dus ;
  delete EffFlavVsBEff_dusg;
}


void EffPurFromHistos::write () {
  EffFlavVsBEff_d   ->Write();
  EffFlavVsBEff_u   ->Write();
  EffFlavVsBEff_s   ->Write();
  EffFlavVsBEff_c   ->Write();
  EffFlavVsBEff_b   ->Write();
  EffFlavVsBEff_g   ->Write();
  EffFlavVsBEff_ni  ->Write();
  EffFlavVsBEff_dus ->Write();
  EffFlavVsBEff_dusg->Write();
}


void EffPurFromHistos::epsPlot(const TString & name)
{
  plot(name, ".eps");
}

void EffPurFromHistos::psPlot(const TString & name)
{
  plot(name, ".ps");
}

void EffPurFromHistos::plot(const TString & name, const TString & ext)
{
   TCanvas tc ("FlavEffVsBEff" +histoExtension ,
	"Flavour misidentification vs. b-tagging efficiency " + histoExtension);
   plot(&tc);
   tc.Print(TString(name + "FlavEffVsBEff" + histoExtension + ext));
}

void EffPurFromHistos::plot (TPad * plotCanvas ) {

//fixme:
  bool btppNI = false;
  bool btppColour = true;

//   // might be called more than once -> check if canvas already created
//   // if yes -> don't do anything
//
//   if ( plotCanvas != 0 ) return;
//
//   if ( !btppTitle ) gStyle->SetOptTitle ( 0 );
//
//   plotCanvas = new TCanvas( "FlavEffVsBEff" +histoExtension ,
// 			    "Flavour misidentification vs. b-tagging efficiency " + histoExtension ,
// 			    btppXCanvas , btppYCanvas );

  plotCanvas->SetFillColor ( 0 );
  plotCanvas->cd ( 1 );
  gPad->SetLogy  ( 1 );
  gPad->SetGridx ( 1 );
  gPad->SetGridy ( 1 );

  int col_c  ;
  int col_g  ;
  int col_dus;
  int col_ni ;

  int mStyle_c  ;
  int mStyle_g  ;
  int mStyle_dus;
  int mStyle_ni ;

  // marker size (same for all)
  float mSize = 1.2;

  if ( btppColour ) {
    col_c    = 6;
    col_g    = 3; // g in green
    col_dus  = 4; // uds in blue
    col_ni   = 5; // ni in ??
    mStyle_c   = 20;
    mStyle_g   = 20;
    mStyle_dus = 20;
    mStyle_ni  = 20;
  }
  else {
    col_c    = 1;
    col_g    = 1;
    col_dus  = 1;
    col_ni   = 1;
    mStyle_c   = 22;
    mStyle_g   = 29;
    mStyle_dus = 20;
    mStyle_ni  = 27;
  }


  // for the moment: plot c,dus,g
  EffFlavVsBEff_dus ->GetXaxis()->SetTitle ( "b-jet efficiency" );
  EffFlavVsBEff_dus ->GetYaxis()->SetTitle ( "non b-jet efficiency" );
  EffFlavVsBEff_dus ->GetYaxis()->SetTitleOffset ( 1.25 );
  EffFlavVsBEff_dus ->SetMaximum     ( 1.1 );
  EffFlavVsBEff_dus ->SetMinimum     ( 1.e-5 );
  EffFlavVsBEff_dus ->SetMarkerColor ( col_dus );
  EffFlavVsBEff_dus ->SetLineColor   ( col_dus );
  EffFlavVsBEff_dus ->SetMarkerSize  ( mSize );
  EffFlavVsBEff_dus ->SetMarkerStyle ( mStyle_dus );
  EffFlavVsBEff_dus ->SetStats     ( false );
  EffFlavVsBEff_dus ->Draw("pe");

  EffFlavVsBEff_g   ->SetMarkerColor ( col_g );
  EffFlavVsBEff_g   ->SetLineColor   ( col_g );
  EffFlavVsBEff_g   ->SetMarkerSize  ( mSize );
  EffFlavVsBEff_g   ->SetMarkerStyle ( mStyle_g );
  EffFlavVsBEff_g   ->SetStats     ( false );
  EffFlavVsBEff_g   ->Draw("peSame");

  EffFlavVsBEff_c   ->SetMarkerColor ( col_c );
  EffFlavVsBEff_c   ->SetLineColor   ( col_c );
  EffFlavVsBEff_c   ->SetMarkerSize  ( mSize );
  EffFlavVsBEff_c   ->SetMarkerStyle ( mStyle_c );
  EffFlavVsBEff_c   ->SetStats     ( false );
  EffFlavVsBEff_c   ->Draw("peSame");

  // plot separately u,d and s
//  EffFlavVsBEff_d ->GetXaxis()->SetTitle ( "b-jet efficiency" );
//  EffFlavVsBEff_d ->GetYaxis()->SetTitle ( "non b-jet efficiency" );
//  EffFlavVsBEff_d ->GetYaxis()->SetTitleOffset ( 1.25 );
//  EffFlavVsBEff_d ->SetMaximum     ( 1.1 );
//  EffFlavVsBEff_d ->SetMinimum     ( 1.e-5 );
//  EffFlavVsBEff_d ->SetMarkerColor ( col_dus );
//  EffFlavVsBEff_d ->SetLineColor   ( col_dus );
//  EffFlavVsBEff_d ->SetMarkerSize  ( mSize );
//  EffFlavVsBEff_d ->SetMarkerStyle ( mStyle_dus );
//  EffFlavVsBEff_d ->SetStats     ( false );
//  EffFlavVsBEff_d ->Draw("pe");
//
//  EffFlavVsBEff_u   ->SetMarkerColor ( col_g );
//  EffFlavVsBEff_u   ->SetLineColor   ( col_g );
//  EffFlavVsBEff_u   ->SetMarkerSize  ( mSize );
//  EffFlavVsBEff_u   ->SetMarkerStyle ( mStyle_g );
//  EffFlavVsBEff_u   ->SetStats     ( false );
//  EffFlavVsBEff_u   ->Draw("peSame");
//
//  EffFlavVsBEff_s   ->SetMarkerColor ( col_c );
//  EffFlavVsBEff_s   ->SetLineColor   ( col_c );
//  EffFlavVsBEff_s   ->SetMarkerSize  ( mSize );
//  EffFlavVsBEff_s   ->SetMarkerStyle ( mStyle_c );
//  EffFlavVsBEff_s   ->SetStats     ( false );
//  EffFlavVsBEff_s   ->Draw("peSame");

  // only if asked: NI
  if ( btppNI ) {
    EffFlavVsBEff_ni   ->SetMarkerColor ( col_ni );
    EffFlavVsBEff_ni   ->SetLineColor   ( col_ni );
    EffFlavVsBEff_ni   ->SetMarkerSize  ( mSize );
    EffFlavVsBEff_ni   ->SetMarkerStyle ( mStyle_ni );
    EffFlavVsBEff_ni   ->SetStats     ( false );
    EffFlavVsBEff_ni   ->Draw("peSame");
  }

}


bool EffPurFromHistos::check () {
  // number of bins
  int nBins_d    = effVersusDiscr_d    -> GetNbinsX();
  int nBins_u    = effVersusDiscr_u    -> GetNbinsX();
  int nBins_s    = effVersusDiscr_s    -> GetNbinsX();
  int nBins_c    = effVersusDiscr_c    -> GetNbinsX();
  int nBins_b    = effVersusDiscr_b    -> GetNbinsX();
  int nBins_g    = effVersusDiscr_g    -> GetNbinsX();
  int nBins_ni   = effVersusDiscr_ni   -> GetNbinsX();
  int nBins_dus  = effVersusDiscr_dus  -> GetNbinsX();
  int nBins_dusg = effVersusDiscr_dusg -> GetNbinsX();

  bool lNBins =
    ( nBins_d == nBins_u    &&
      nBins_d == nBins_s    &&
      nBins_d == nBins_c    &&
      nBins_d == nBins_b    &&
      nBins_d == nBins_g    &&
      nBins_d == nBins_ni   &&
      nBins_d == nBins_dus  &&
      nBins_d == nBins_dusg     );

  if ( !lNBins ) {
    cout << "EffPurFromHistos::check() : Input histograms do not all have the same number of bins!" << endl;
    return false;
  }


  // start
  float sBin_d    = effVersusDiscr_d    -> GetBinCenter(1);
  float sBin_u    = effVersusDiscr_u    -> GetBinCenter(1);
  float sBin_s    = effVersusDiscr_s    -> GetBinCenter(1);
  float sBin_c    = effVersusDiscr_c    -> GetBinCenter(1);
  float sBin_b    = effVersusDiscr_b    -> GetBinCenter(1);
  float sBin_g    = effVersusDiscr_g    -> GetBinCenter(1);
  float sBin_ni   = effVersusDiscr_ni   -> GetBinCenter(1);
  float sBin_dus  = effVersusDiscr_dus  -> GetBinCenter(1);
  float sBin_dusg = effVersusDiscr_dusg -> GetBinCenter(1);

  bool lSBin =
    ( sBin_d == sBin_u    &&
      sBin_d == sBin_s    &&
      sBin_d == sBin_c    &&
      sBin_d == sBin_b    &&
      sBin_d == sBin_g    &&
      sBin_d == sBin_ni   &&
      sBin_d == sBin_dus  &&
      sBin_d == sBin_dusg     );

  if ( !lSBin ) {
    cout << "EffPurFromHistos::check() : Input histograms do not all have the same start bin!" << endl;
    return false;
  }


  // end
  float eBin_d    = effVersusDiscr_d    -> GetBinCenter( nBins_d - 1 );
  float eBin_u    = effVersusDiscr_u    -> GetBinCenter( nBins_d - 1 );
  float eBin_s    = effVersusDiscr_s    -> GetBinCenter( nBins_d - 1 );
  float eBin_c    = effVersusDiscr_c    -> GetBinCenter( nBins_d - 1 );
  float eBin_b    = effVersusDiscr_b    -> GetBinCenter( nBins_d - 1 );
  float eBin_g    = effVersusDiscr_g    -> GetBinCenter( nBins_d - 1 );
  float eBin_ni   = effVersusDiscr_ni   -> GetBinCenter( nBins_d - 1 );
  float eBin_dus  = effVersusDiscr_dus  -> GetBinCenter( nBins_d - 1 );
  float eBin_dusg = effVersusDiscr_dusg -> GetBinCenter( nBins_d - 1 );

  bool lEBin =
    ( eBin_d == eBin_u    &&
      eBin_d == eBin_s    &&
      eBin_d == eBin_c    &&
      eBin_d == eBin_b    &&
      eBin_d == eBin_g    &&
      eBin_d == eBin_ni   &&
      eBin_d == eBin_dus  &&
      eBin_d == eBin_dusg     );

  if ( !lEBin ) {
    cout << "EffPurFromHistos::check() : Input histograms do not all have the same end bin!" << endl;
    return false;
  }



  return true;
}


void EffPurFromHistos::compute () {

  // loop over eff. vs. discriminator cut b-histo and look in which bin the closest entry is;
  // use fact that eff decreases monotonously

  // any of the histos to be created can be taken here:
  TH1F * EffFlavVsBEff = EffFlavVsBEff_d;

  int nBinB = EffFlavVsBEff->GetNbinsX() - 2; // -2 because we don't include under/overflow in loop

  for ( int iBinB = 1; iBinB <= nBinB; iBinB++ ) {  // loop over the bins on the x-axis of the histograms to be filled

    float effBBinWidth = EffFlavVsBEff->GetBinWidth  ( iBinB );
    float effBMid      = EffFlavVsBEff->GetBinCenter ( iBinB ); // middle of b-efficiency bin
    float effBLeft     = effBMid - 0.5*effBBinWidth;              // left edge of bin
    float effBRight    = effBMid + 0.5*effBBinWidth;              // right edge of bin
    // find the corresponding bin in the efficiency versus discriminator cut histo: closest one in efficiency
    int   binClosest = findBinClosestYValue ( effVersusDiscr_b , effBMid , effBLeft , effBRight );
    bool  binFound   = ( binClosest > 0 ) ;
    //
    if ( binFound ) {
      // fill the histos
      EffFlavVsBEff_d    -> Fill ( effBMid , effVersusDiscr_d   ->GetBinContent ( binClosest ) );
      EffFlavVsBEff_u    -> Fill ( effBMid , effVersusDiscr_u   ->GetBinContent ( binClosest ) );
      EffFlavVsBEff_s    -> Fill ( effBMid , effVersusDiscr_s   ->GetBinContent ( binClosest ) );
      EffFlavVsBEff_c    -> Fill ( effBMid , effVersusDiscr_c   ->GetBinContent ( binClosest ) );
      EffFlavVsBEff_b    -> Fill ( effBMid , effVersusDiscr_b   ->GetBinContent ( binClosest ) );
      EffFlavVsBEff_g    -> Fill ( effBMid , effVersusDiscr_g   ->GetBinContent ( binClosest ) );
      EffFlavVsBEff_ni   -> Fill ( effBMid , effVersusDiscr_ni  ->GetBinContent ( binClosest ) );
      EffFlavVsBEff_dus  -> Fill ( effBMid , effVersusDiscr_dus ->GetBinContent ( binClosest ) );
      EffFlavVsBEff_dusg -> Fill ( effBMid , effVersusDiscr_dusg->GetBinContent ( binClosest ) );

      EffFlavVsBEff_d    -> SetBinError ( iBinB , effVersusDiscr_d   ->GetBinError ( binClosest ) );
      EffFlavVsBEff_u    -> SetBinError ( iBinB , effVersusDiscr_u   ->GetBinError ( binClosest ) );
      EffFlavVsBEff_s    -> SetBinError ( iBinB , effVersusDiscr_s   ->GetBinError ( binClosest ) );
      EffFlavVsBEff_c    -> SetBinError ( iBinB , effVersusDiscr_c   ->GetBinError ( binClosest ) );
      EffFlavVsBEff_b    -> SetBinError ( iBinB , effVersusDiscr_b   ->GetBinError ( binClosest ) );
      EffFlavVsBEff_g    -> SetBinError ( iBinB , effVersusDiscr_g   ->GetBinError ( binClosest ) );
      EffFlavVsBEff_ni   -> SetBinError ( iBinB , effVersusDiscr_ni  ->GetBinError ( binClosest ) );
      EffFlavVsBEff_dus  -> SetBinError ( iBinB , effVersusDiscr_dus ->GetBinError ( binClosest ) );
      EffFlavVsBEff_dusg -> SetBinError ( iBinB , effVersusDiscr_dusg->GetBinError ( binClosest ) );
    }
    else {
      //CW      cout << "Did not find right bin for b-efficiency : " << effBMid << endl;
    }

  }

}
