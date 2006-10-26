#ifndef EffPurFromHistos_H
#define EffPurFromHistos_H

#include "RecoBTag/Analysis/interface/FlavourHistorgrams.h"

#include "TH1F.h"
#include "TString.h"
#include "TCanvas.h"

class EffPurFromHistos {


 public:

  EffPurFromHistos ( const TString & ext, TH1F * h_d, TH1F * h_u,
	TH1F * h_s, TH1F * h_c, TH1F * h_b, TH1F * h_g,	TH1F * h_ni,
	TH1F * h_dus, TH1F * h_dusg,
	int nBin = 100 , double startO = 0.005 , double endO = 1.005 ) ;
	// defaults reasonable for lifetime based tags

  EffPurFromHistos (const FlavourHistorgrams<double> * dDiscriminatorFC,
	int nBin = 100 , double startO = 0.005 , double endO = 1.005 ) ;
	// defaults reasonable for lifetime based tags

  ~EffPurFromHistos () ;

  // do the computation
  void compute () ;

  // return the newly created histos
  TH1F * getEffFlavVsBEff_d    () { return EffFlavVsBEff_d    ; };
  TH1F * getEffFlavVsBEff_u    () { return EffFlavVsBEff_u    ; };
  TH1F * getEffFlavVsBEff_s    () { return EffFlavVsBEff_s    ; };
  TH1F * getEffFlavVsBEff_c    () { return EffFlavVsBEff_c    ; };
  TH1F * getEffFlavVsBEff_b    () { return EffFlavVsBEff_b    ; };
  TH1F * getEffFlavVsBEff_g    () { return EffFlavVsBEff_g    ; };
  TH1F * getEffFlavVsBEff_ni   () { return EffFlavVsBEff_ni   ; };
  TH1F * getEffFlavVsBEff_dus  () { return EffFlavVsBEff_dus  ; };
  TH1F * getEffFlavVsBEff_dusg () { return EffFlavVsBEff_dusg ; };

  void write () ;

  void epsPlot(const TString & name);

  void psPlot(const TString & name);

  void plot(TPad * theCanvas) ;

  void plot(const TString & name, const TString & ext);

//   void print () const ;

  FlavourHistorgrams<double> * discriminatorNoCutEffic() const {return discrNoCutEffic;}
  FlavourHistorgrams<double> * discriminatorCutEfficScan() const {return discrCutEfficScan;}

 private:

  // consistency check (same binning)
  bool check () ;
  bool fromDiscriminatorDistr;


  // the TString for the histo name extension
  TString histoExtension ;

  FlavourHistorgrams<double> * discrNoCutEffic, *discrCutEfficScan;

  // the input histograms (efficiency versus discriminator cut)
  // IMPORTANT: IT'S ASSUMED THAT ALL HISTOS HAVE THE SAME BINNING!!
  // (can in principle be relaxed by checking explicitely for the discriminator value
  //  instead of bin index)
  TH1F * effVersusDiscr_d    ;
  TH1F * effVersusDiscr_u    ;
  TH1F * effVersusDiscr_s    ;
  TH1F * effVersusDiscr_c    ;
  TH1F * effVersusDiscr_b    ;
  TH1F * effVersusDiscr_g    ;
  TH1F * effVersusDiscr_ni   ;
  TH1F * effVersusDiscr_dus  ;
  TH1F * effVersusDiscr_dusg ;


  // the corresponding output histograms (flavour-eff vs. b-efficiency)

  // binning for output histograms
  int   nBinOutput ;
  double startOutput ;
  double endOutput ;

  TH1F * EffFlavVsBEff_d    ;
  TH1F * EffFlavVsBEff_u    ;
  TH1F * EffFlavVsBEff_s    ;
  TH1F * EffFlavVsBEff_c    ;
  TH1F * EffFlavVsBEff_b    ;
  TH1F * EffFlavVsBEff_g    ;
  TH1F * EffFlavVsBEff_ni   ;
  TH1F * EffFlavVsBEff_dus  ;
  TH1F * EffFlavVsBEff_dusg ;

} ;

#endif
