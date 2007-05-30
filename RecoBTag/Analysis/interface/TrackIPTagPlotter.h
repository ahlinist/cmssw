#ifndef TrackIPTagPlotter_H
#define TrackIPTagPlotter_H

#include "RecoBTag/Analysis/interface/BaseBTagPlotter.h"
#include "DataFormats/BTauReco/interface/TrackIPTagInfo.h"
#include "RecoBTag/Analysis/interface/FlavourHistorgrams.h"
#include "RecoBTag/MCTools/interface/JetFlavour.h"


class TrackIPTagPlotter : public BaseBTagPlotter {


 public:

  TrackIPTagPlotter (const TString & tagName, const EtaPtBin & etaPtBin,
	int nBinEffPur, double startEffPur, double endEffPur, bool update = false);

  ~TrackIPTagPlotter () ;

  void analyzeTag (const reco::JetTag & jetTag, const JetFlavour & jetFlavour);

  virtual void finalize ();

  virtual void write ();

  void epsPlot(const TString & name);

  void psPlot(const TString & name);

 private:

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
