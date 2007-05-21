#ifndef BaseBTagPlotter_H
#define BaseBTagPlotter_H

#include "RecoBTag/Analysis/interface/EtaPtBin.h"
#include "DataFormats/BTauReco/interface/JetTag.h"
#include "RecoBTag/MCTools/interface/JetFlavour.h"
#include "RecoBTag/Analysis/interface/EffPurFromHistos.h"
#include "TString.h"

class BaseBTagPlotter {

 public:

  BaseBTagPlotter ( const TString & tagName, const EtaPtBin & etaPtBin,
	int nBinEffPur, double startEffPur, double endEffPur) :
	etaPtBin_(etaPtBin), tagName_(tagName),
	theExtensionString ("_"+tagName+etaPtBin.getDescriptionString()),
	nBinEffPur_(nBinEffPur), startEffPur_(startEffPur ), endEffPur_(endEffPur) {};

  virtual ~BaseBTagPlotter () {};
  
  const EtaPtBin& etaPtBin() { return etaPtBin_ ;}
  
  virtual void analyzeTag(const reco::JetTag & jetTag, const JetFlavour & jetFlavour) = 0;

  // final computation, plotting, printing .......
  virtual void finalize () = 0;

  // get "2d" histograms for misid. vs. b-eff
  virtual EffPurFromHistos * getEffPurFromHistos () {return 0;}

  virtual void write () = 0 ;

  virtual void epsPlot(const TString & name) = 0;

  virtual void psPlot(const TString & name) = 0;

  int nBinEffPur() const {return nBinEffPur_;}
  double startEffPur() const {return startEffPur_;}
  double endEffPur() const {return endEffPur_;}

 protected:

  // the extension string to be used in histograms etc.
  const EtaPtBin etaPtBin_;
  const TString tagName_, theExtensionString,  ;
  const int   nBinEffPur_ ;
  const double startEffPur_ ; 
  const double endEffPur_ ; 
  
  
} ;

#endif
