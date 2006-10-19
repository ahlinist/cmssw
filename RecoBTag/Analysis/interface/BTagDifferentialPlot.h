#ifndef BTagDifferentialPlot_H
#define BTagDifferentialPlot_H

// #include "BTagPlotPrintC.h"

#include "TH1F.h"
#include "TString.h"
#include "TCanvas.h"

#include <vector>
using namespace std ;

#include "RecoBTag/Analysis/interface/EtaPtBin.h"
#include "RecoBTag/Analysis/interface/BaseBTagPlotter.h"

//#include "EffPurFromHistosC.h"

class BTagDifferentialPlot /*: public BTagPlotPrintC */{


  
 public:

  BTagDifferentialPlot ( double ) ;

  ~BTagDifferentialPlot () ;


  void setVerbose ( bool v ) { verbose = v ; }  
  
  void addBinPlotter ( BaseBTagPlotter * aPlotter ) { theBinPlotters.push_back ( aPlotter ) ; }

  void process () ;

  void write () ; 

  void epsPlot(const TString & name);

  void psPlot(const TString & name);

  void plot (TCanvas & theCanvas) ;

  void plot(const TString & name, const TString & ext);

// 
//   void print () const ;

  TH1F * getDifferentialHistoB_d    () { return theDifferentialHistoB_d    ; }
  TH1F * getDifferentialHistoB_u    () { return theDifferentialHistoB_u    ; }
  TH1F * getDifferentialHistoB_s    () { return theDifferentialHistoB_s    ; }
  TH1F * getDifferentialHistoB_c    () { return theDifferentialHistoB_c    ; }
  TH1F * getDifferentialHistoB_b    () { return theDifferentialHistoB_b    ; }
  TH1F * getDifferentialHistoB_g    () { return theDifferentialHistoB_g    ; }
  TH1F * getDifferentialHistoB_ni   () { return theDifferentialHistoB_ni   ; }
  TH1F * getDifferentialHistoB_dus  () { return theDifferentialHistoB_dus  ; }
  TH1F * getDifferentialHistoB_dusg () { return theDifferentialHistoB_dusg ; }
  
  


  
 private:
  
  void setVariableName () ;
  
  void bookHisto () ;

  void fillHisto () ;


  // the fixed b-efficiency (later: allow more than one) for which the misids have to be plotted
  double fixedBEfficiency ;

  // verbosity flag
  bool verbose ;
  
  // flag if processing should be skipped
  bool noProcessing ;
  bool processed;

  // the name for the variable with constant value
  TString constVariableName ;
  // the name of the variable to be plotted on the x-axis (e.g. "eta", "pt")
  TString diffVariableName ;

  // value of the constant variable (lower/upper edge of interval)
  pair<double,double> constVariableValue ;

  // the common name to describe histograms
  TString commonName ;


  // the input
  vector<BaseBTagPlotter *> theBinPlotters ;

  // the histo to create/fill
  TH1F * theDifferentialHistoB_d    ;
  TH1F * theDifferentialHistoB_u    ;
  TH1F * theDifferentialHistoB_s    ;
  TH1F * theDifferentialHistoB_c    ;
  TH1F * theDifferentialHistoB_b    ;
  TH1F * theDifferentialHistoB_g    ;
  TH1F * theDifferentialHistoB_ni   ;
  TH1F * theDifferentialHistoB_dus  ;
  TH1F * theDifferentialHistoB_dusg ;

  // the plot Canvas
//   TCanvas * thePlotCanvas ;
  
} ;


#endif
