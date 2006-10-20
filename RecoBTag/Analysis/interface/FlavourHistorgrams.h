
#ifndef FlavourHistorgrams_H
#define FlavourHistorgrams_H

// #include "BTagPlotPrintC.h"

#include "TString.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TStyle.h"

#include "RecoBTag/Analysis/interface/Tools.h"

#include <iostream>

using namespace std ;
using namespace RecoBTag;


//
// class to describe Histo
//
template <class T>
class FlavourHistorgrams {

public:

  FlavourHistorgrams (TString baseNameTitle_ , TString baseNameDescription_ ,
		      int nBins_ , double lowerBound_ , double upperBound_ ,
		      bool statistics_ ,
		      bool plotLog_ ,
		      bool plotNormalized_ ,
		      TString plotFirst_ , bool update = false) ;

  virtual ~FlavourHistorgrams () ;


  // define arrays (if needed)
//   void defineArray ( int * dimension , int max , int indexToPlot ) ;
  
  // fill entry
  // For single variables and arrays (for arrays only a single index can be filled)
  void fill ( const int & flavour,  const T & variable) const;

  // For single variables and arrays
  void fill ( const int & flavour,  const T * variable) const;

  void write () const ;
  
  void plot (TPad * theCanvas) ;
//   void plot (TCanvas * theCanvas) ;

  void epsPlot(TString name);

  // needed for efficiency computations -> this / b
  // (void : alternative would be not to overwrite the histos but to return a cloned HistoDescription)
  void divide ( FlavourHistorgrams<T> & bHD ) const ;
  
  // trivial access functions
  TString  baseNameTitle       () const { return theBaseNameTitle       ; }
  TString  baseNameDescription () const { return theBaseNameDescription ; }
  int    nBins               () const { return theNBins               ; }
  double lowerBound          () const { return theLowerBound          ; } 
  double upperBound          () const { return theUpperBound          ; }
  bool   statistics          () const { return theStatistics          ; }
  bool   plotLog             () const { return thePlotLog             ; }
  bool   plotNormalized      () const { return thePlotNormalized      ; }
  TString  plotFirst           () const { return thePlotFirst           ; }

  // access to the histos
  TH1F * histo_all  () const { return theHisto_all  ; }    
  TH1F * histo_d    () const { return theHisto_d    ; }    
  TH1F * histo_u    () const { return theHisto_u    ; }
  TH1F * histo_s    () const { return theHisto_s    ; }
  TH1F * histo_c    () const { return theHisto_c    ; }
  TH1F * histo_b    () const { return theHisto_b    ; }
  TH1F * histo_g    () const { return theHisto_g    ; }
  TH1F * histo_ni   () const { return theHisto_ni   ; }
  TH1F * histo_dus  () const { return theHisto_dus  ; }
  TH1F * histo_dusg () const { return theHisto_dusg ; }

  vector<TH1F*> getHistoVector() const;

  

protected:

  void fillVariable ( const int & flavour , const T & var ) const;
  
  //
  // the data members
  //
  
//   T *   theVariable   ;

  // for arrays
  int * theArrayDimension ;
  int   theMaxDimension ;
  int   theIndexToPlot ; // in case that not the complete array has to be plotted

  TString  theBaseNameTitle ;
  TString  theBaseNameDescription ;
  int    theNBins ;
  double theLowerBound ;
  double theUpperBound ;
  bool   theStatistics ;
  bool   thePlotLog    ;
  bool   thePlotNormalized ;
  TString  thePlotFirst  ; // one character; gives flavour to plot first: l (light) , c , b

  // the histos
  TH1F *theHisto_all  ;    
  TH1F *theHisto_d    ;    
  TH1F *theHisto_u    ;
  TH1F *theHisto_s    ;
  TH1F *theHisto_c    ;
  TH1F *theHisto_b    ;
  TH1F *theHisto_g    ;
  TH1F *theHisto_ni   ;
  TH1F *theHisto_dus  ;
  TH1F *theHisto_dusg ;


  // the canvas to plot
  TCanvas* theCanvas ;
  private:
  FlavourHistorgrams(){}

} ;



template <class T>
FlavourHistorgrams<T>::FlavourHistorgrams (TString baseNameTitle_ , TString baseNameDescription_ ,
					  int nBins_ , double lowerBound_ , double upperBound_ ,
					  bool statistics_ ,
					  bool plotLog_ ,
					  bool plotNormalized_ ,
					  TString plotFirst_, bool update) :
//   BaseFlavourHistorgrams () ,
  // theVariable ( variable_ ) ,
  theBaseNameTitle ( baseNameTitle_ ) , theBaseNameDescription ( baseNameDescription_ ) ,
  theNBins ( nBins_ ) , theLowerBound ( lowerBound_ ) , theUpperBound ( upperBound_ ) ,
  theStatistics ( statistics_ ) ,
  thePlotLog ( plotLog_ ) ,
  thePlotNormalized ( plotNormalized_ ) ,
  thePlotFirst ( plotFirst_ )  
{
  // defaults for array dimensions
  theArrayDimension = 0  ;
  theMaxDimension   = -1 ;
  theIndexToPlot    = -1 ;
    
  // check plot order string 
  if ( thePlotFirst == "l" || thePlotFirst == "c" || thePlotFirst == "b" ) {
    // OK
  }
  else {
    // not correct: print warning and set default (l)
    cout << "FlavourHistorgrams::FlavourHistorgrams : thePlotFirst was not correct : " << thePlotFirst << endl ;
    cout << "FlavourHistorgrams::FlavourHistorgrams : Set it to default value (l)! " << endl ;
    thePlotFirst = "l" ;
  }

  if (!update) {
    // book histos
    theHisto_all   = new TH1F ( theBaseNameTitle + "ALL"  , theBaseNameDescription + " all jets"  , theNBins , theLowerBound , theUpperBound ) ; 
    theHisto_d     = new TH1F ( theBaseNameTitle + "D"    , theBaseNameDescription + " d-jets"    , theNBins , theLowerBound , theUpperBound ) ; 
    theHisto_u     = new TH1F ( theBaseNameTitle + "U"    , theBaseNameDescription + " u-jets"    , theNBins , theLowerBound , theUpperBound ) ; 
    theHisto_s     = new TH1F ( theBaseNameTitle + "S"    , theBaseNameDescription + " s-jets"    , theNBins , theLowerBound , theUpperBound ) ; 
    theHisto_c     = new TH1F ( theBaseNameTitle + "C"    , theBaseNameDescription + " c-jets"    , theNBins , theLowerBound , theUpperBound ) ; 
    theHisto_b     = new TH1F ( theBaseNameTitle + "B"    , theBaseNameDescription + " b-jets"    , theNBins , theLowerBound , theUpperBound ) ; 
    theHisto_g     = new TH1F ( theBaseNameTitle + "G"    , theBaseNameDescription + " g-jets"    , theNBins , theLowerBound , theUpperBound ) ; 
    theHisto_ni    = new TH1F ( theBaseNameTitle + "NI"   , theBaseNameDescription + " ni-jets"   , theNBins , theLowerBound , theUpperBound ) ; 
    theHisto_dus   = new TH1F ( theBaseNameTitle + "DUS"  , theBaseNameDescription + " dus-jets"  , theNBins , theLowerBound , theUpperBound ) ; 
    theHisto_dusg  = new TH1F ( theBaseNameTitle + "DUSG" , theBaseNameDescription + " dusg-jets" , theNBins , theLowerBound , theUpperBound ) ;
    // statistics if requested
    if ( theStatistics ) {
      theHisto_all ->Sumw2() ; 
      theHisto_d   ->Sumw2() ; 
      theHisto_u   ->Sumw2() ; 
      theHisto_s   ->Sumw2() ; 
      theHisto_c   ->Sumw2() ; 
      theHisto_b   ->Sumw2() ; 
      theHisto_g   ->Sumw2() ; 
      theHisto_ni  ->Sumw2() ; 
      theHisto_dus ->Sumw2() ; 
      theHisto_dusg->Sumw2() ;
    }
  } else {
    theHisto_all   = (TH1F*) gDirectory->Get(theBaseNameTitle + "ALL" ) ; 
    theHisto_d     = (TH1F*) gDirectory->Get(theBaseNameTitle + "D"   ) ; 
    theHisto_u     = (TH1F*) gDirectory->Get(theBaseNameTitle + "U"   ) ; 
    theHisto_s     = (TH1F*) gDirectory->Get(theBaseNameTitle + "S"   ) ; 
    theHisto_c     = (TH1F*) gDirectory->Get(theBaseNameTitle + "C"   ) ; 
    theHisto_b     = (TH1F*) gDirectory->Get(theBaseNameTitle + "B"   ) ; 
    theHisto_g     = (TH1F*) gDirectory->Get(theBaseNameTitle + "G"   ) ; 
    theHisto_ni    = (TH1F*) gDirectory->Get(theBaseNameTitle + "NI"  ) ; 
    theHisto_dus   = (TH1F*) gDirectory->Get(theBaseNameTitle + "DUS" ) ; 
    theHisto_dusg  = (TH1F*) gDirectory->Get(theBaseNameTitle + "DUSG") ;
  }

  // defaults for other data members
  theCanvas = 0 ;
}


template <class T>
FlavourHistorgrams<T>::~FlavourHistorgrams () {
  // delete the histos
  delete theHisto_all   ;
  delete theHisto_d     ;
  delete theHisto_u     ;
  delete theHisto_s     ;
  delete theHisto_c     ;
  delete theHisto_b     ;
  delete theHisto_g     ;
  delete theHisto_ni    ;
  delete theHisto_dus   ;
  delete theHisto_dusg  ;
  // delete the canvas
  delete theCanvas ;
}


// define arrays (if needed)
// template <class T>
// void FlavourHistorgrams<T>::defineArray ( int * dimension , int max , int indexToPlot ) {
//   // indexToPlot < 0 if all to be plotted
//   theArrayDimension = dimension ;
//   theMaxDimension   = max ;
//   theIndexToPlot    = indexToPlot ;
// }
  
// fill entry
template <class T> void
FlavourHistorgrams<T>::fill ( const int & flavour,  const T & variable) const 
{
  // For single variables and arrays (for arrays only a single index can be filled)
  fillVariable ( flavour , variable ) ;
}

template <class T> void
FlavourHistorgrams<T>::fill ( const int & flavour,  const T * variable) const
{
  if ( theArrayDimension == 0 ) {       
    // single variable
    fillVariable ( flavour , *variable ) ;
  } else {
    // array      
    int iMax = *theArrayDimension ;
    if ( *theArrayDimension > theMaxDimension ) iMax = theMaxDimension ;
    //
    for ( int i = 0 ; i < iMax ; i++ ) {
      // check if only one index to be plotted (<0: switched off -> plot all)
      if ( ( theIndexToPlot < 0 ) || ( i == theIndexToPlot ) ) { 
	fillVariable ( flavour , *(variable+i) ) ;
      }
    }

    // if single index to be filled but not enough entries: fill 0.0 (convention!)
    if ( theIndexToPlot >= iMax ) { 
      // cout << "==>> The index to be filled is too big -> fill 0.0 : " << theBaseNameTitle << " : " << theIndexToPlot << " >= " << iMax << endl ;
      T theZero = static_cast<T> ( 0.0 ) ;
      fillVariable ( flavour , theZero ) ;
    }
  }
} 


template <class T>
void FlavourHistorgrams<T>::write () const {
  // here: write histogram to specified output file
  theHisto_all ->Write() ; 
  theHisto_d   ->Write() ; 
  theHisto_u   ->Write() ; 
  theHisto_s   ->Write() ; 
  theHisto_c   ->Write() ; 
  theHisto_b   ->Write() ; 
  theHisto_g   ->Write() ; 
  theHisto_ni  ->Write() ; 
  theHisto_dus ->Write() ; 
  theHisto_dusg->Write() ;
}

  
template <class T>
void FlavourHistorgrams<T>::plot (TPad * theCanvas) {
// template <class T>
// void FlavourHistorgrams<T>::plot (TCanvas * theCanvas) {

//fixme:
  bool btppNI = false;
  bool btppColour = true;
  setTDRStyle()->cd();
  theCanvas->UseCurrentStyle();
//   if ( !btppTitle ) gStyle->SetOptTitle ( 0 ) ;
//   
//   // here: plot histograms in a canvas
//   theCanvas = new TCanvas ( "C" + theBaseNameTitle , "C" + theBaseNameDescription ,
// 			    btppXCanvas , btppYCanvas ) ;
//   theCanvas->SetFillColor ( 0 ) ;
//   theCanvas->cd  ( 1 ) ;
  gPad->SetLogy  ( 0 ) ;
  if ( thePlotLog ) gPad->SetLogy ( 1 ) ; 
  gPad->SetGridx ( 0 ) ;
  gPad->SetGridy ( 0 ) ;
  gPad->SetTitle ( false ) ;

  // plot order ("highest" one first)
  TH1F * histo_1 ;
  TH1F * histo_2 ;
  TH1F * histo_3 ;
  TH1F * histo_4 ; // only if bttpNI

  int col_1 ;
  int col_2 ;
  int col_3 ;
  int col_4 ;
  
  int lineStyle_1 ;
  int lineStyle_2 ;
  int lineStyle_3 ;
  int lineStyle_4 ;

  int lineWidth = 1 ;

  // default (l)
  histo_1 = theHisto_dusg ;
  //CW histo_1 = theHisto_dus ;
  histo_2 = theHisto_b ;
  histo_3 = theHisto_c ;
  histo_4 = 0 ; if (btppNI) histo_4 = theHisto_ni ;
  
  if ( btppColour ) { // print colours 
    col_1   = 4 ;
    col_2   = 2 ;
    col_3   = 6 ;
    col_4   = 3 ;
    lineStyle_1  = 1 ;
    lineStyle_2  = 1 ;
    lineStyle_3  = 1 ;
    lineStyle_4  = 1 ;
    lineWidth = 3 ;
  }
  else { // different marker/line styles
    col_1   = 1 ;
    col_2   = 1 ;
    col_3   = 1 ;
    col_4   = 1 ;
    lineStyle_1  = 2 ;
    lineStyle_2  = 1 ;
    lineStyle_3  = 3 ;
    lineStyle_4  = 4 ;
  }

  // if changing order (NI stays always last)
  
  // c to plot first   
  if ( thePlotFirst == "c" ) {
    histo_1 = theHisto_c ;
    if ( btppColour  ) col_1   = 6 ;
    if ( !btppColour ) lineStyle_1  = 3 ;
    histo_3 = theHisto_dusg ;
    if ( btppColour  ) col_3   = 4 ;
    if ( !btppColour ) lineStyle_2  = 2 ;
  }

  // b to plot first   
  if ( thePlotFirst == "b" ) {
    histo_1 = theHisto_b ;
    if ( btppColour  ) col_1   = 2 ;
    if ( !btppColour ) lineStyle_1  = 1 ;
    histo_2 = theHisto_dusg ;
    if ( btppColour  ) col_2   = 4 ;
    if ( !btppColour ) lineStyle_2  = 2 ;
  }

  histo_1 ->GetXaxis()->SetTitle ( theBaseNameTitle ) ;
  histo_1 ->GetYaxis()->SetTitle ( "Arbitrary Units" ) ;
  histo_1 ->GetYaxis()->SetTitleOffset(1.25) ;
  histo_1 ->SetStats ( false ) ;
  histo_1 ->SetLineStyle ( lineStyle_1 ) ;
  histo_1 ->SetLineWidth ( lineWidth ) ;
  histo_1 ->SetLineColor ( col_1 ) ;
    
  histo_2 ->SetStats     ( false ) ;
  histo_2 ->SetLineStyle ( lineStyle_2 ) ;
  histo_2 ->SetLineWidth ( lineWidth ) ;
  histo_2 ->SetLineColor ( col_2 ) ;

  histo_3 ->SetStats     ( false ) ;
  histo_3 ->SetStats ( "0" ) ;
  histo_3 ->SetLineWidth ( lineWidth ) ;
  histo_3 ->SetLineStyle ( lineStyle_3 ) ;
  histo_3 ->SetLineColor ( col_3 ) ;

  if ( histo_4 != 0 ) {
    histo_4 ->SetStats     ( false ) ;
    histo_4 ->SetStats ( "0" ) ;
    histo_4 ->SetLineWidth ( lineWidth ) ;
    histo_4 ->SetLineStyle ( lineStyle_4 ) ;
    histo_4 ->SetLineColor ( col_4 ) ;
  }

  if ( thePlotNormalized ) {
    histo_1 ->DrawNormalized() ;
    histo_2 ->DrawNormalized("Same") ;
    histo_3 ->DrawNormalized("Same") ;
    if ( histo_4 != 0 ) histo_4 ->DrawNormalized("Same") ;
  }
  else {
    histo_1 ->Draw() ;
    histo_2 ->Draw("Same") ;
    histo_3 ->Draw("Same") ;
    if ( histo_4 != 0 ) histo_4 ->Draw("Same") ;
  }
//   theHisto_all->Draw() ;

}

template <class T>
void FlavourHistorgrams<T>::epsPlot(TString name)
{
   TCanvas tc(theBaseNameTitle , theBaseNameDescription);
   
//    theHisto_all->Draw() ;
   plot(&tc);
   tc.Print(TString(name + theBaseNameTitle + ".eps"));
}


// needed for efficiency computations -> this / b
// (void : alternative would be not to overwrite the histos but to return a cloned HistoDescription)
template <class T>
void FlavourHistorgrams<T>::divide ( FlavourHistorgrams<T> & bHD ) const {
  // divide histos using binomial errors
  //
  // ATTENTION: It's the responsability of the user to make sure that the HistoDescriptions
  //            involved in this operation have been constructed with the statistics option switched on!!
  //
  theHisto_all  -> Divide ( theHisto_all  , bHD.histo_all () , 1.0 , 1.0 , "b" ) ;    
  theHisto_d    -> Divide ( theHisto_d    , bHD.histo_d   () , 1.0 , 1.0 , "b" ) ;    
  theHisto_u    -> Divide ( theHisto_u    , bHD.histo_u   () , 1.0 , 1.0 , "b" ) ;
  theHisto_s    -> Divide ( theHisto_s    , bHD.histo_s   () , 1.0 , 1.0 , "b" ) ;
  theHisto_c    -> Divide ( theHisto_c    , bHD.histo_c   () , 1.0 , 1.0 , "b" ) ;
  theHisto_b    -> Divide ( theHisto_b    , bHD.histo_b   () , 1.0 , 1.0 , "b" ) ;
  theHisto_g    -> Divide ( theHisto_g    , bHD.histo_g   () , 1.0 , 1.0 , "b" ) ;
  theHisto_ni   -> Divide ( theHisto_ni   , bHD.histo_ni  () , 1.0 , 1.0 , "b" ) ;
  theHisto_dus  -> Divide ( theHisto_dus  , bHD.histo_dus () , 1.0 , 1.0 , "b" ) ;
  theHisto_dusg -> Divide ( theHisto_dusg , bHD.histo_dusg() , 1.0 , 1.0 , "b" ) ;
}
  

template <class T>
void FlavourHistorgrams<T>::fillVariable ( const int & flavour , const T & var ) const {
  // all
  theHisto_all                ->Fill ( var ) ;
  // flavour specific
  if ( flavourIsD   (flavour) ) theHisto_d    ->Fill ( var ) ; 
  if ( flavourIsU   (flavour) ) theHisto_u    ->Fill ( var ) ; 
  if ( flavourIsS   (flavour) ) theHisto_s    ->Fill ( var ) ; 
  if ( flavourIsC   (flavour) ) theHisto_c    ->Fill ( var ) ; 
  if ( flavourIsB   (flavour) ) theHisto_b    ->Fill ( var ) ; 
  if ( flavourIsG   (flavour) ) theHisto_g    ->Fill ( var ) ; 
  if ( flavourIsNI  (flavour) ) theHisto_ni   ->Fill ( var ) ; 
  if ( flavourIsDUS (flavour) ) theHisto_dus  ->Fill ( var ) ; 
  if ( flavourIsDUSG(flavour) ) theHisto_dusg ->Fill ( var ) ;
}

template <class T>
vector<TH1F*> FlavourHistorgrams<T>::getHistoVector() const
{
  vector<TH1F*> histoVector;
  histoVector.push_back ( theHisto_all );
  histoVector.push_back ( theHisto_d   );
  histoVector.push_back ( theHisto_u   );
  histoVector.push_back ( theHisto_s   );
  histoVector.push_back ( theHisto_c   );
  histoVector.push_back ( theHisto_b   );
  histoVector.push_back ( theHisto_g   );
  histoVector.push_back ( theHisto_ni  );
  histoVector.push_back ( theHisto_dus );
  histoVector.push_back ( theHisto_dusg);
  return histoVector;
}


#endif
