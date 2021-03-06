#ifndef TrackProbabilityTagPlotter_H
#define TrackProbabilityTagPlotter_H

#include "RecoBTag/Analysis/interface/BaseTagInfoPlotter.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/BTauReco/interface/TrackProbabilityTagInfo.h"
#include "RecoBTag/Analysis/interface/FlavourHistorgrams.h"
// #include "RecoBTag/MCTools/interface/JetFlavour.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"


class TrackProbabilityTagPlotter : public BaseTagInfoPlotter {


 public:

  TrackProbabilityTagPlotter (const TString & tagName, const EtaPtBin & etaPtBin,
	const edm::ParameterSet& pSet, bool update = false);

  ~TrackProbabilityTagPlotter () ;

  void analyzeTag (const reco::BaseTagInfo * tagInfo, const int & jetFlavour);

  virtual void finalize ();

  virtual void write (const bool allHisto);

  void epsPlot(const TString & name);

  void psPlot(const TString & name);

 private:

  int	nBinEffPur_ ;
  double startEffPur_ ; 
  double endEffPur_ ; 
  FlavourHistograms<double> * tkcntHistosSig3D[5];
  FlavourHistograms<double> * tkcntHistosSig2D[5];
  EffPurFromHistos * effPurFromHistos[4] ;
  bool finalized;
} ;

#endif
