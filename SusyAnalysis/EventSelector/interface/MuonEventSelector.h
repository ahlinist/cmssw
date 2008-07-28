#ifndef SusyAnalysis_EventSelector_MuonEventSelector_h_
#define SusyAnalysis_EventSelector_MuonEventSelector_h_

/// MuonEventSelector
///
/// Selects on pT, eta, and tracker isolation of each muon
///
/// Original author: M. Weinberg - Fri Jul 18 11:55:50 CEST 2008
///
/// $Id: MuonEventSelector.h, v 1.3 2008/07/18, weinberg$

#include "SusyAnalysis/EventSelector/interface/SusyEventSelector.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "DataFormats/PatCandidates/interface/Muon.h"

class MuonEventSelector : public SusyEventSelector {
public:
  MuonEventSelector (const edm::ParameterSet&);
  virtual bool select(const edm::Event&) const;
  virtual ~MuonEventSelector () {}

 private:
  // Define parameters here
  edm::InputTag       muonTag_; // tag for input collection
  std::vector<double> minPt_;   // lower pT cuts (also defines min # of jets)
  std::vector<double> maxEta_;  // uper |eta| cuts (also defines min # of jets)
  std::vector<double> minIso_;  // lower isolation cut (also defines min # of jets)
};

#endif
