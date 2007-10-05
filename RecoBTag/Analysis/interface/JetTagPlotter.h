#ifndef JetTagPlotter_H
#define JetTagPlotter_H

#include "TH1F.h"
#include "TString.h"

#include <vector>
#include "RecoBTag/Analysis/interface/FlavourHistorgrams.h"
#include "RecoBTag/Analysis/interface/BaseBTagPlotter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"


class JetTagPlotter : public BaseBTagPlotter {


 public:

  JetTagPlotter (const TString & tagName, const EtaPtBin & etaPtBin,
		 const edm::ParameterSet& pSet, bool update = false);

  virtual ~JetTagPlotter () ;

  void analyzeTag (const reco::JetTag & jetTag, const JetFlavour & jetFlavour);

  // final computation, plotting, printing .......
  void finalize () ;

  // get "2d" histograms for misid. vs. b-eff
  EffPurFromHistos * getEffPurFromHistos () { return effPurFromHistos ; }

  void write ();

  void epsPlot(const TString & name);

  void psPlot(const TString & name);

  int nBinEffPur() const {return nBinEffPur_;}
  double startEffPur() const {return startEffPur_;}
  double endEffPur() const {return endEffPur_;}

 protected:

  // binning and bounds
  // 1) for 'efficiency' versus discriminator cut histos
  int    discrBins  ;
  double discrStart_ ;
  double discrEnd_   ;
  int	nBinEffPur_ ;
  double startEffPur_ ; 
  double endEffPur_ ; 

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
