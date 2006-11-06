#ifndef BaseBTagPlotter_H
#define BaseBTagPlotter_H

#include "RecoBTag/Analysis/interface/EtaPtBin.h"
#include "TString.h"

class EffPurFromHistos;

class BaseBTagPlotter {

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

 protected:

  // the extension string to be used in histograms etc.
  const EtaPtBin etaPtBin_;
  const TString theExtensionString ;
  
} ;

#endif
