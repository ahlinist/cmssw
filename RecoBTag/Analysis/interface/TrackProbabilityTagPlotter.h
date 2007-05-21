#ifndef TrackProbabilityTagPlotter_H
#define TrackProbabilityTagPlotter_H

#include "RecoBTag/Analysis/interface/BaseBTagPlotter.h"
#include "RecoBTag/Analysis/interface/JetTagPlotter.h"
#include "DataFormats/BTauReco/interface/TrackProbabilityTagInfo.h"
#include "RecoBTag/Analysis/interface/FlavourHistorgrams.h"
#include "RecoBTag/MCTools/interface/JetFlavour.h"


class TrackProbabilityTagPlotter : public BaseBTagPlotter {


 public:

  TrackProbabilityTagPlotter (const TString & tagName, const EtaPtBin & etaPtBin,
	int nBinEffPur, double startEffPur, double endEffPur, bool update = false);

  ~TrackProbabilityTagPlotter () ;

  void analyzeTag (const reco::JetTag & jetTag, const JetFlavour & jetFlavour);

  virtual void finalize ();

  virtual void write ();

  void epsPlot(const TString & name);

  void psPlot(const TString & name);

 private:

  FlavourHistorgrams<double> * tkcntHistosSig3D[5];
  FlavourHistorgrams<double> * tkcntHistosSig2D[5];
  EffPurFromHistos * effPurFromHistos[4] ;
  bool finalized;
} ;

#endif
