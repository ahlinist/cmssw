#ifndef TrackCountingTagPlotter_H
#define TrackCountingTagPlotter_H

#include "RecoBTag/Analysis/interface/BaseBTagPlotter.h"
#include "DataFormats/BTauReco/interface/TrackCountingTagInfo.h"
#include "RecoBTag/Analysis/interface/FlavourHistorgrams.h"
#include "RecoBTag/MCTools/interface/JetFlavour.h"


class TrackCountingTagPlotter : public BaseBTagPlotter {


 public:

  TrackCountingTagPlotter (const EtaPtBin & etaPtBin, int nBinEffPur,
	double startEffPur, double endEffPur, bool update = false);

  ~TrackCountingTagPlotter () ;

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
  bool finalized;
} ;

#endif
