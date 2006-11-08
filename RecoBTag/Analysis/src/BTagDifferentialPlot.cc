#include "RecoBTag/Analysis/interface/BTagDifferentialPlot.h"
#include "RecoBTag/Analysis/interface/EffPurFromHistos.h"
#include "RecoBTag/Analysis/interface/Tools.h"

#include <iostream>
using namespace std ;



BTagDifferentialPlot::BTagDifferentialPlot (double bEff, ConstVarType constVariable) :
	fixedBEfficiency     ( bEff )  , verbose(false), constVar(constVariable),
	noProcessing         ( false ) , processed(false),
	constVariableName    ( "" )    , diffVariableName     ( "" )    ,
	constVariableValue   ( 999.9 , 999.9 ) , commonName( "MisidForBEff_" ) ,
	theDifferentialHistoB_d    ( 0 ) ,
	theDifferentialHistoB_u    ( 0 ) ,
	theDifferentialHistoB_s    ( 0 ) ,
	theDifferentialHistoB_c    ( 0 ) ,
	theDifferentialHistoB_b    ( 0 ) ,
	theDifferentialHistoB_g    ( 0 ) ,
	theDifferentialHistoB_ni   ( 0 ) ,
	theDifferentialHistoB_dus  ( 0 ) ,
	theDifferentialHistoB_dusg ( 0 )   {}


BTagDifferentialPlot::~BTagDifferentialPlot () {
  if (processed) {
    delete  theDifferentialHistoB_d    ;
    delete  theDifferentialHistoB_u    ;
    delete  theDifferentialHistoB_s    ;
    delete  theDifferentialHistoB_c    ;
    delete  theDifferentialHistoB_b    ;
    delete  theDifferentialHistoB_g    ;
    delete  theDifferentialHistoB_ni   ;
    delete  theDifferentialHistoB_dus  ;
    delete  theDifferentialHistoB_dusg ;
  }
}



void BTagDifferentialPlot::write () {
  if (processed) {
  gFile->cd();
  gFile->mkdir(commonName);
  gFile->cd(commonName);
    theDifferentialHistoB_d   ->Write() ;
    theDifferentialHistoB_u   ->Write() ;
    theDifferentialHistoB_s   ->Write() ;
    theDifferentialHistoB_c   ->Write() ;
    theDifferentialHistoB_b   ->Write() ;
    theDifferentialHistoB_g   ->Write() ;
    theDifferentialHistoB_ni  ->Write() ;
    theDifferentialHistoB_dus ->Write() ;
    theDifferentialHistoB_dusg->Write() ;
  }
}



void BTagDifferentialPlot::plot (TCanvas & thePlotCanvas ) {

//   thePlotCanvas = new TCanvas(  commonName ,
// 				commonName ,
// 				btppXCanvas , btppYCanvas ) ;
//
//   if ( !btppTitle ) gStyle->SetOptTitle ( 0 ) ;

  if (!processed) return;
//fixme:
  bool btppNI = false;
  bool btppColour = true;

  thePlotCanvas.SetFillColor ( 0 ) ;
  thePlotCanvas.cd ( 1 ) ;
  gPad->SetLogy  ( 1 ) ;
  gPad->SetGridx ( 1 ) ;
  gPad->SetGridy ( 1 ) ;

  int col_b   ;
  int col_c   ;
  int col_g   ;
  int col_dus ;
  int col_ni  ;

  int mStyle_b   ;
  int mStyle_c   ;
  int mStyle_g   ;
  int mStyle_dus ;
  int mStyle_ni  ;

  // marker size (same for all)
  float mSize = 1.5 ;

  if ( btppColour ) {
    col_b    = 2 ;
    col_c    = 6 ;
    col_g    = 3 ;
    col_dus  = 4 ;
    col_ni   = 5 ;
    mStyle_b   = 20 ;
    mStyle_c   = 20 ;
    mStyle_g   = 20 ;
    mStyle_dus = 20 ;
    mStyle_ni  = 20 ;
  }
  else {
    col_b    = 1 ;
    col_c    = 1 ;
    col_g    = 1 ;
    col_dus  = 1 ;
    col_ni   = 1 ;
    mStyle_b   = 12 ;
    mStyle_c   = 22 ;
    mStyle_g   = 29 ;
    mStyle_dus = 20 ;
    mStyle_ni  = 27 ;
  }

  // for the moment: plot b (to see what the constant b-efficiency is), c, g, uds
  // b in red
  // No, do not plot b (because only visible for the soft leptons)
  // theDifferentialHistoB_b   -> GetXaxis()->SetTitle ( diffVariableName ) ;
  // theDifferentialHistoB_b   -> GetYaxis()->SetTitle ( "non b-jet efficiency" ) ;
  // theDifferentialHistoB_b   -> GetYaxis()->SetTitleOffset ( 1.25 ) ;
  // theDifferentialHistoB_b   -> SetMaximum ( 0.4 )  ;
  // theDifferentialHistoB_b   -> SetMinimum ( 1.e-4 )  ;
  // theDifferentialHistoB_b   -> SetMarkerColor ( col_b ) ;
  // theDifferentialHistoB_b   -> SetLineColor   ( col_b ) ;
  // theDifferentialHistoB_b   -> SetMarkerSize  ( mSize ) ;
  // theDifferentialHistoB_b   -> SetMarkerStyle ( mStyle_b ) ;
  // theDifferentialHistoB_b   -> SetStats ( false ) ;
  // theDifferentialHistoB_b   -> Draw ( "pe" ) ;
  // c in magenta
  theDifferentialHistoB_c   -> SetMaximum ( 0.4 )  ;
  theDifferentialHistoB_c   -> SetMinimum ( 1.e-4 )  ;
  theDifferentialHistoB_c   -> SetMarkerColor ( col_c ) ;
  theDifferentialHistoB_c   -> SetLineColor   ( col_c ) ;
  theDifferentialHistoB_c   -> SetMarkerSize  ( mSize ) ;
  theDifferentialHistoB_c   -> SetMarkerStyle ( mStyle_c ) ;
  theDifferentialHistoB_c   -> SetStats     ( false ) ;
  //  theDifferentialHistoB_c   -> Draw("peSame") ;
  theDifferentialHistoB_c   -> Draw("pe") ;
  // uds in blue
  theDifferentialHistoB_dus -> SetMarkerColor ( col_dus ) ;
  theDifferentialHistoB_dus -> SetLineColor   ( col_dus ) ;
  theDifferentialHistoB_dus -> SetMarkerSize  ( mSize ) ;
  theDifferentialHistoB_dus -> SetMarkerStyle ( mStyle_dus ) ;
  theDifferentialHistoB_dus -> SetStats     ( false ) ;
  theDifferentialHistoB_dus -> Draw("peSame") ;
  // g in green
  // only uds not to confuse
  theDifferentialHistoB_g   -> SetMarkerColor ( col_g ) ;
  theDifferentialHistoB_g   -> SetLineColor   ( col_g ) ;
  theDifferentialHistoB_g   -> SetMarkerSize  ( mSize ) ;
  theDifferentialHistoB_g   -> SetMarkerStyle ( mStyle_g ) ;
  theDifferentialHistoB_g   -> SetStats     ( false ) ;
  theDifferentialHistoB_g   -> Draw("peSame") ;

  // NI if wanted
  if ( btppNI ) {
    theDifferentialHistoB_ni -> SetMarkerColor ( col_ni ) ;
    theDifferentialHistoB_ni -> SetLineColor   ( col_ni ) ;
    theDifferentialHistoB_ni -> SetMarkerSize  ( mSize ) ;
    theDifferentialHistoB_ni -> SetMarkerStyle ( mStyle_ni ) ;
    theDifferentialHistoB_ni -> SetStats     ( false ) ;
    theDifferentialHistoB_ni -> Draw("peSame") ;
  }
}

void BTagDifferentialPlot::epsPlot(const TString & name)
{
  plot(name, ".eps");
}

void BTagDifferentialPlot::psPlot(const TString & name)
{
  plot(name, ".ps");
}

void BTagDifferentialPlot::plot(const TString & name, const TString & ext)
{
  if (!processed) return;
   TCanvas tc(commonName, commonName);
   plot(tc);
   tc.Print(TString(name + commonName + ext));
}


void BTagDifferentialPlot::process () {
  setVariableName () ; // also sets noProcessing if not OK
  if ( noProcessing ) return ;
  bookHisto () ;
  fillHisto () ;
  processed = true;
}


void BTagDifferentialPlot::setVariableName ()
{
  if ( constVar==constETA ) {
    constVariableName  = "eta" ;
    diffVariableName   = "pt"  ;
    constVariableValue = make_pair ( theBinPlotters[0]->etaPtBin().getEtaMin() , theBinPlotters[0]->etaPtBin().getEtaMax() ) ;
  }
  if ( constVar==constPT  ) {
    constVariableName = "pt"  ;
    diffVariableName  = "eta" ;
    constVariableValue = make_pair ( theBinPlotters[0]->etaPtBin().getPtMin() , theBinPlotters[0]->etaPtBin().getPtMax() ) ;
  }

  if ( verbose ) {
    cout << "====>>>> BTagDifferentialPlot::setVariableName() : set const/diffVariableName to : "
	 << constVariableName << " / " << diffVariableName << endl
	 << "====>>>>                                            constant value interval : "
	 << constVariableValue.first  << " - " << constVariableValue.second << endl ;
  }
}



void BTagDifferentialPlot::bookHisto () {

  // vector with ranges
  vector<double> variableRanges ;

  for ( unsigned int iP = 0 ; iP < theBinPlotters.size() ; iP++ ) {
    const EtaPtBin & currentBin = theBinPlotters[iP]->etaPtBin()  ;
    if ( diffVariableName == "eta" ) {
      // only active bins in the variable on x-axis
      if ( currentBin.getEtaActive() ) {
	variableRanges.push_back ( currentBin.getEtaMin() ) ;
	// also max if last one
	if ( iP == theBinPlotters.size()-1 ) variableRanges.push_back ( currentBin.getEtaMax() ) ;
      }
    }
    if ( diffVariableName == "pt" ) {
      // only active bins in the variable on x-axis
      if ( currentBin.getPtActive() ) {
	variableRanges.push_back ( currentBin.getPtMin() ) ;
	// also max if last one
	if ( iP == theBinPlotters.size()-1 ) variableRanges.push_back ( currentBin.getPtMax() ) ;
      }
    }
  }

  // to book histo with variable binning -> put into array
  int      nBins    = variableRanges.size() - 1 ;
  double * binArray = new double [nBins+1] ;

  for ( int i = 0 ; i < nBins + 1 ; i++ ) {
    binArray[i] = variableRanges[i] ;
  }


  // part of the name common to all flavours
  commonName += fixedBEfficiency ;
  commonName += "_Const_" ;
  commonName += constVariableName ;
  commonName += "_" ;
  commonName += constVariableValue.first ;
  commonName += "-" ;
  commonName += constVariableValue.second ;
  commonName += "_" ;
  commonName += "_Vs_" ;
  commonName += diffVariableName ;
  commonName.ReplaceAll ( " " , "" ) ;

  theDifferentialHistoB_d    = new TH1F ( "D_"    + commonName , "D_"    + commonName , nBins , binArray ) ;
  theDifferentialHistoB_u    = new TH1F ( "U_"    + commonName , "U_"    + commonName , nBins , binArray ) ;
  theDifferentialHistoB_s    = new TH1F ( "S_"    + commonName , "S_"    + commonName , nBins , binArray ) ;
  theDifferentialHistoB_c    = new TH1F ( "C_"    + commonName , "C_"    + commonName , nBins , binArray ) ;
  theDifferentialHistoB_b    = new TH1F ( "B_"    + commonName , "B_"    + commonName , nBins , binArray ) ;
  theDifferentialHistoB_g    = new TH1F ( "G_"    + commonName , "G_"    + commonName , nBins , binArray ) ;
  theDifferentialHistoB_ni   = new TH1F ( "NI_"   + commonName , "NI_"   + commonName , nBins , binArray ) ;
  theDifferentialHistoB_dus  = new TH1F ( "DUS_"  + commonName , "DUS_"  + commonName , nBins , binArray ) ;
  theDifferentialHistoB_dusg = new TH1F ( "DUSG_" + commonName , "DUSG_" + commonName , nBins , binArray ) ;
}


void BTagDifferentialPlot::fillHisto () {
  // loop over bins and find corresponding misid. in the MisIdVs..... histo
  for ( unsigned int iP = 0 ; iP < theBinPlotters.size() ; iP++ ) {
    const EtaPtBin   & currentBin              = theBinPlotters[iP]->etaPtBin() ;
    EffPurFromHistos * currentEffPurFromHistos = theBinPlotters[iP]->getEffPurFromHistos() ;
    //
    bool   isActive   = true ;
    double valueXAxis = -999.99 ;
    // find right bin based on middle of the interval
    if ( diffVariableName == "eta" ) {
      isActive = currentBin.getEtaActive() ;
      valueXAxis = 0.5 * ( currentBin.getEtaMin() + currentBin.getEtaMax() ) ;
    }
    else {
      if ( diffVariableName == "pt"  ) {
	isActive = currentBin.getPtActive() ;
	valueXAxis = 0.5 * ( currentBin.getPtMin() + currentBin.getPtMax() ) ;
      }
      else {
	cout << "====>>>> BTagDifferentialPlot::fillHisto() : illegal diffVariableName = " << diffVariableName << endl
	     << "====>>>> BTagDifferentialPlot::fillHisto() : -> I'll exit!! " << endl ;
	exit (1) ;
      }
    }


    // for the moment: ignore inactive bins
    // (maybe later: if a Bin is inactive -> set value to fill well below left edge of histogram to have it in the underflow)

    if ( !isActive ) continue ;

    // to have less lines of code ....
    vector< pair<TH1F*,TH1F*> > effPurDifferentialPairs ;

    // all flavours (b is a good cross check! must be constant and = fixed b-efficiency)
    // get histo; find the bin of the fixed b-efficiency in the histo and get misid; fill

    effPurDifferentialPairs.push_back ( make_pair ( currentEffPurFromHistos->getEffFlavVsBEff_d()    , theDifferentialHistoB_d    ) ) ;
    effPurDifferentialPairs.push_back ( make_pair ( currentEffPurFromHistos->getEffFlavVsBEff_u()    , theDifferentialHistoB_u    ) ) ;
    effPurDifferentialPairs.push_back ( make_pair ( currentEffPurFromHistos->getEffFlavVsBEff_s()    , theDifferentialHistoB_s    ) ) ;
    effPurDifferentialPairs.push_back ( make_pair ( currentEffPurFromHistos->getEffFlavVsBEff_c()    , theDifferentialHistoB_c    ) ) ;
    effPurDifferentialPairs.push_back ( make_pair ( currentEffPurFromHistos->getEffFlavVsBEff_b()    , theDifferentialHistoB_b    ) ) ;
    effPurDifferentialPairs.push_back ( make_pair ( currentEffPurFromHistos->getEffFlavVsBEff_g()    , theDifferentialHistoB_g    ) ) ;
    effPurDifferentialPairs.push_back ( make_pair ( currentEffPurFromHistos->getEffFlavVsBEff_ni()   , theDifferentialHistoB_ni   ) ) ;
    effPurDifferentialPairs.push_back ( make_pair ( currentEffPurFromHistos->getEffFlavVsBEff_dus()  , theDifferentialHistoB_dus  ) ) ;
    effPurDifferentialPairs.push_back ( make_pair ( currentEffPurFromHistos->getEffFlavVsBEff_dusg() , theDifferentialHistoB_dusg ) ) ;

    for ( vector< pair<TH1F*,TH1F*> >::const_iterator itP  = effPurDifferentialPairs.begin() ;
	                                              itP != effPurDifferentialPairs.end()   ; itP++ ) {
      TH1F * effPurHist = itP->first  ;
      TH1F * diffHist   = itP->second ;
      int iBinGet = effPurHist->FindBin ( fixedBEfficiency ) ;
      double effForBEff    = effPurHist->GetBinContent ( iBinGet ) ;
      double effForBEffErr = effPurHist->GetBinError   ( iBinGet ) ;
      int iBinSet = diffHist->FindBin(valueXAxis) ;
      diffHist->SetBinContent ( iBinSet , effForBEff    ) ;
      diffHist->SetBinError   ( iBinSet , effForBEffErr ) ;
    }

  }

}
