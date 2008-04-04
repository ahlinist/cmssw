#ifndef MVAJetTagPlotter_H
#define MVAJetTagPlotter_H

#include <vector>
#include <string>

#include "RecoBTag/Analysis/interface/BaseTagInfoPlotter.h"
#include "RecoBTag/Analysis/interface/TaggingVariablePlotter.h"
#include "DataFormats/BTauReco/interface/BaseTagInfo.h"
#include "RecoBTau/JetTagComputer/interface/GenericMVAJetTagComputer.h"
#include "RecoBTag/Analysis/interface/FlavourHistorgrams.h"
// #include "RecoBTag/MCTools/interface/JetFlavour.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"


class MVAJetTagPlotter : public BaseTagInfoPlotter {

 public:

  MVAJetTagPlotter (const TString & tagName, const EtaPtBin & etaPtBin,
	const edm::ParameterSet& pSet, bool update = false);

  ~MVAJetTagPlotter ();

  virtual void analyzeTag (const std::vector<const reco::BaseTagInfo *> & baseTagInfos, const int & jetFlavour);

  virtual void finalize ();

  virtual void write (const bool allHisto);

  void epsPlot(const TString & name);

  void psPlot(const TString & name);

  virtual void setEventSetup (const edm::EventSetup & setup);
  virtual std::vector<std::string> tagInfoRequirements () const;

 private:

  std::string jetTagComputer;
  const GenericMVAJetTagComputer *computer;

  reco::TaggingVariableName categoryVariable;
  std::vector<TaggingVariablePlotter*> categoryPlotters;
};

#endif
