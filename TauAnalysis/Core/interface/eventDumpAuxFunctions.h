#ifndef TauAnalysis_Core_eventDumpAuxFunctions_h
#define TauAnalysis_Core_eventDumpAuxFunctions_h

#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/MET.h"

#include <vector>
#include <string>

#include <iostream>

void printEventHeaderInfo(const edm::Event&, double, std::ostream*);
void printEventTriggerInfo(edm::Handle<edm::TriggerResults>&, std::vector<std::string>, std::ostream*);
void printEventSelectionInfo(const std::vector<std::pair<std::string, bool> >&, const std::vector<std::pair<std::string, bool> >&, std::ostream*);

void printGenParticleInfo(edm::Handle<edm::View<reco::GenParticle> >&, edm::Handle<edm::View<reco::GenJet> >&, std::ostream*);

void printElectronInfo(edm::Handle<pat::ElectronCollection>&, std::ostream*);
void printMuonInfo(edm::Handle<pat::MuonCollection>&, std::ostream*);
void printTauInfo(edm::Handle<pat::TauCollection>&, std::ostream*);
void printMissingEtInfo(edm::Handle<pat::METCollection>&, std::ostream*);

#endif
