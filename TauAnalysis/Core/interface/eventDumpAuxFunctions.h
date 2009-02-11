#ifndef TauAnalysis_Core_eventDumpAuxFunctions_h
#define TauAnalysis_Core_eventDumpAuxFunctions_h

#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include <vector>
#include <string>

#include <iostream>

void printEventSelectionInfo(const std::vector<std::pair<std::string, bool> >&, const std::vector<std::pair<std::string, bool> >&, std::ostream*);

void printGenParticleInfo(edm::Handle<edm::View<reco::GenParticle> >&, edm::Handle<edm::View<reco::GenJet> >&, std::ostream*);

void printTrackInfo(const edm::RefToBase<reco::Track>&, const reco::Candidate::Point&, std::ostream*);
void printVertexInfo(const reco::Candidate::Point&, std::ostream*);

void printTrackIsolationInfo(const edm::Handle<reco::TrackCollection>&, const reco::Candidate::Vector&, double, std::ostream*);

#endif
