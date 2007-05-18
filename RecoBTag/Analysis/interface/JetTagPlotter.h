#ifndef JetTagPlotter_H
#define JetTagPlotter_H

#include "TH1F.h"
#include "TString.h"

#include <vector>
#include "RecoBTag/Analysis/interface/FlavourHistorgrams.h"
#include "RecoBTag/Analysis/interface/BaseBTagPlotter.h"


class JetTagPlotter : public BaseBTagPlotter {


 public:

  JetTagPlotter (const EtaPtBin & etaPtBin, double discrStart, double discrEnd,
		 int nBinEffPur, double startEffPur, double endEffPur,
		 bool update = false, BaseBTagPlotter *extTagPlotter = 0);

  virtual ~JetTagPlotter () ;

  void analyzeTag (const reco::JetTag & jetTag, const JetFlavour & jetFlavour);

  // final computation, plotting, printing .......
  void finalize () ;

  // get "2d" histograms for misid. vs. b-eff
  EffPurFromHistos * getEffPurFromHistos () { return effPurFromHistos ; }

  void write ();

  void epsPlot(const TString & name);

  void psPlot(const TString & name);

 protected:

  BaseBTagPlotter * extTagPlotter_;

  // binning and bounds
  // 1) for 'efficiency' versus discriminator cut histos
  int    discrBins  ;
  double discrStart_ ;
  double discrEnd_   ;

  bool finalized;

  // for the misid vs. eff plots
  EffPurFromHistos * effPurFromHistos ;

  FlavourHistorgrams<double> * dDiscriminatorFC;
  FlavourHistorgrams<int> * dJetFlav;

  // track multiplicity in jet
  FlavourHistorgrams<int> * dJetMultiplicity;
  
  // Discriminator: again with reasonable binning
  FlavourHistorgrams<double> * dDiscriminator;
  
  // reconstructed jet momentum
  FlavourHistorgrams<double> * dJetRecMomentum;

  // reconstructed jet transverse momentum
  FlavourHistorgrams<double> * dJetRecPt;

  // reconstructed jet eta
  FlavourHistorgrams<double> * dJetRecPseudoRapidity;

  // reconstructed jet phi
  FlavourHistorgrams<double> * dJetRecPhi;

  // associated parton momentum
  FlavourHistorgrams<double> * dJetPartonMomentum;

  // associated parton pt
  FlavourHistorgrams<double> * dJetPartonPt;

  // associated parton eta
  FlavourHistorgrams<double> * dJetPartonPseudoRapidity;
} ;

#endif
