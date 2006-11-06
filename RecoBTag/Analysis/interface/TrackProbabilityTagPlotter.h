#ifndef TrackProbabilityTagPlotter_H
#define TrackProbabilityTagPlotter_H

#include "RecoBTag/Analysis/interface/BaseBTagPlotter.h"
#include "RecoBTag/Analysis/interface/JetTagPlotter.h"
#include "DataFormats/BTauReco/interface/TrackCountingTagInfo.h"
#include "RecoBTag/Analysis/interface/FlavourHistorgrams.h"
#include "RecoBTag/MCTools/interface/JetFlavour.h"


class TrackProbabilityTagPlotter : public BaseBTagPlotter {


 public:

  TrackProbabilityTagPlotter (JetTagPlotter *jetTagPlotter, bool update = false);
  
  ~TrackProbabilityTagPlotter () ;

  void analyzeTag (const reco::TrackCountingTagInfo & tagInfo,
	const reco::JetTag & jetTag, const JetFlavour & jetFlavour);

  virtual void finalize () { jetTagPlotter_->finalize();}

  // get "2d" histograms for misid. vs. b-eff
  virtual EffPurFromHistos * getEffPurFromHistos ()
	{return jetTagPlotter_->getEffPurFromHistos();}

  virtual void write ();

  void epsPlot(const TString & name);

  void psPlot(const TString & name);

 private:

  JetTagPlotter *jetTagPlotter_;

  FlavourHistorgrams<double> * hSignificance;
  FlavourHistorgrams<double> * tkprobHistosSig[4];
} ;

#endif
