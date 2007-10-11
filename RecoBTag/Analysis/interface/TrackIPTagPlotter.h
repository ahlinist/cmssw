#ifndef TrackIPTagPlotter_H
#define TrackIPTagPlotter_H

#include "RecoBTag/Analysis/interface/BaseTagInfoPlotter.h"
#include "DataFormats/BTauReco/interface/TrackIPTagInfo.h"
#include "RecoBTag/Analysis/interface/FlavourHistorgrams.h"
#include "RecoBTag/MCTools/interface/JetFlavour.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"


class TrackIPTagPlotter : public BaseTagInfoPlotter {


 public:

  TrackIPTagPlotter (const TString & tagName, const EtaPtBin & etaPtBin,
	const edm::ParameterSet& pSet, bool update = false);

  ~TrackIPTagPlotter () ;

  void analyzeTag (const reco::BaseTagInfo * baseTagInfo, const BTagMCTools::JetFlavour & jetFlavour);

  virtual void finalize ();

  virtual void write (const bool allHisto);

  void epsPlot(const TString & name);

  void psPlot(const TString & name);

 private:

  int	nBinEffPur_ ;
  double startEffPur_ ; 
  double endEffPur_ ; 

  FlavourHistorgrams<double> * tkcntHistosSig3D[5];
  FlavourHistorgrams<double> * tkcntHistosSig2D[5];
  FlavourHistorgrams<int> * trkNbr3D, * trkNbr2D;
  double lowerIPSBound, upperIPSBound;

  EffPurFromHistos * effPurFromHistos[4] ;

  FlavourHistorgrams<float> * tkcntHistosProb3D[5];
  FlavourHistorgrams<float> * tkcntHistosProb2D[5];
  FlavourHistorgrams<double> *decayLengthValuHisto, *jetDistanceValuHisto;
  FlavourHistorgrams<double> *decayLengthSignHisto, *jetDistanceSignHisto;

  bool finalized;
} ;

#endif
