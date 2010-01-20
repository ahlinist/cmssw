#ifndef TauAnalysis_CandidateTools_candidateAuxFunctions_h
#define TauAnalysis_CandidateTools_candidateAuxFunctions_h

#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEt.h"

#include <vector>

const reco::GenParticle* findGenParticle(const reco::Candidate::LorentzVector&, 
					 const reco::GenParticleCollection&, double = 0.5, int = -1,
					 std::vector<int>* = 0);

void findDaughters(const reco::GenParticle*, std::vector<const reco::GenParticle*>&, int = -1);

bool isNeutrino(const reco::GenParticle*);

reco::Candidate::LorentzVector getVisMomentum(const std::vector<const reco::GenParticle*>&, int = 1);
reco::Candidate::LorentzVector getVisMomentum(const reco::GenParticle*, const reco::GenParticleCollection*);
reco::Candidate::LorentzVector getInvisMomentum(const std::vector<const reco::GenParticle*>&, int = 1);
reco::Candidate::LorentzVector getInvisMomentum(const reco::GenParticle*, const reco::GenParticleCollection*);

double getPhysX(double x, bool& isWithinPhysRange);

#endif
