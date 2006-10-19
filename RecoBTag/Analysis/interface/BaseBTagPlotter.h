#ifndef BaseBTagPlotter_H
#define BaseBTagPlotter_H

// #include "BTagPlotPrintC.h"

#include "FWCore/Framework/interface/Event.h"
#include "RecoBTag/Analysis/interface/EtaPtBin.h"
#include "TString.h"

class EffPurFromHistos;

// inheritance from BTagPlotPrintC is for practical reasons only
// to simplify passing of plot/print options to HistoDescription that

class BaseBTagPlotter /*: public BTagPlotPrintC */{

 public:

  BaseBTagPlotter ( const EtaPtBin & etaPtBin) :
	etaPtBin_(etaPtBin), theExtensionString (etaPtBin.getDescriptionString()) {};

  virtual ~BaseBTagPlotter () {};
  
  const EtaPtBin& etaPtBin() { return etaPtBin_ ;}
  
  // analyzeMe has to be implemented
//   virtual void analyzeEvent (const edm::Event& iEvent) = 0;

  // final computation, plotting, printing .......
  virtual void finalize () = 0;

  // get "2d" histograms for misid. vs. b-eff
  virtual EffPurFromHistos * getEffPurFromHistos () = 0;

  virtual void write () = 0 ;

  virtual void epsPlot(const TString & name) = 0;

  virtual void psPlot(const TString & name) = 0;

//   // dummies from BTagPlotPrintC
//   void plot () {}
//   void print () const {} 
// 
//   
//   // common one applying the filter and calling analyzeMe()
//   void analyze () { if ( acceptEvent() ) analyzeMe() ; }
// 
//   // this one has to be implemented by the derived classes
//   // however, not defined pure virtual here to allow to instantiate an object
//   // to get access without overhead to the variables
//   virtual void analyzeMe () {}
// 
//   // event selection
//   virtual bool acceptEvent () ;
// 
//   // final computation, plotting, printing .......
//   virtual void finalize () {}
 protected:

  // the extension string to be used in histograms etc.
  const EtaPtBin etaPtBin_;
  const TString theExtensionString ;
  
} ;

#endif
