#ifndef SusyAnalysis_EventSelector_HTPtdrEventSelector_h_
#define SusyAnalysis_EventSelector_HTPtdrEventSelector_h_
/// Selector for HT
///
/// Computes HT from MET + jets 2 - 4 and cuts on it

// system include files
#include <memory>

// user include files
#include "SusyAnalysis/EventSelector/interface/SusyEventSelector.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "DataFormats/PatCandidates/interface/MET.h"

class HTPtdrEventSelector : public SusyEventSelector {
public:
  HTPtdrEventSelector (const edm::ParameterSet&);
  virtual bool select (const edm::Event&) const;
  virtual ~HTPtdrEventSelector () {}

private:
  pat::MET::UncorectionType uncorrectionType (const std::string&) const;

private:
  edm::InputTag jetTag_; ///< tag for input collection
  edm::InputTag metTag_; ///< tag for input collection
  float minHT_;          ///< lower HT cut 
  float minPt_;          ///< minimum Pt of jets taken into account
  pat::MET::UncorectionType uncorrType_;  ///< uncorrection type for MET
};
#endif
