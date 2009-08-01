#ifndef TauAnalysis_Core_histManagerAuxFunctions_h
#define TauAnalysis_Core_histManagerAuxFunctions_h

#include "DQMServices/Core/interface/MonitorElement.h"

#include "DataFormats/Common/interface/Handle.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"

#include "DataFormats/PatCandidates/interface/Isolation.h"

void fillLeptonIsoDepositHistograms(const pat::IsoDeposit*, MonitorElement*, MonitorElement*, MonitorElement*);

void clearIsoParam(reco::isodeposit::AbsVetos&);

int getMatchingGenParticlePdgId(const reco::Particle::LorentzVector&, edm::Handle<reco::GenParticleCollection>&);

#endif
