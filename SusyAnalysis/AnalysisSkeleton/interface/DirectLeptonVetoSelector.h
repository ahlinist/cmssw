#ifndef SusyAnalysis_AnalysisSkeleton_DirectLeptonVetoSelector_h_
#define SusyAnalysis_AnalysisSkeleton_DirectLeptonVetoSelector_h_
/// Direct lepton veto
///
/// Selects events with no isolated electron/muon/tau below a certain Et threshold.
/// Isolation is computed relative to the lepton's Et.
/// The number of isolated leptons of each family is computed

// system include files
#include <memory>

// user include files
#include "SusyAnalysis/EventSelector/interface/SusyEventSelector.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

class DirectLeptonVetoSelector : public SusyEventSelector {
public:
  DirectLeptonVetoSelector (const edm::ParameterSet&);
  virtual bool select (const edm::Event&) const;
  virtual ~DirectLeptonVetoSelector () {}
private:
  edm::InputTag electronTag_; ///< tag for electron input collection
  edm::InputTag muonTag_;     ///< tag for muon     input collection
  float minEtEle_;            ///< minimum et for electrons to be checked
  float eleIso_;              ///< limit on relative electron isolation (pt sum)
  float minEtMuon_;           ///< minimum et for muons to be checked
  float muonIso_;             ///< limit on relative muon isolation (pt sum)
//   edm::InputTag tauTag_;      ///< tag for tau      input collection
//   float minEtTau_;            ///< minimum et for taus to be checked
//   float tauIso_;              ///< limit on relative tau isolation (pt sum)
};
#endif
