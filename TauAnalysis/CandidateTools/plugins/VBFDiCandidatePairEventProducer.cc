#include "TauAnalysis/CandidateTools/interface/VBFCompositePtrCandidateT1T2MEtEventT3Producer.h"

#include "DataFormats/Candidate/interface/Candidate.h" 
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

typedef VBFCompositePtrCandidateT1T2MEtEventT3Producer<reco::Candidate, reco::Candidate, reco::Candidate> DiCandidatePairVBFEventProducer;
typedef VBFCompositePtrCandidateT1T2MEtEventT3Producer<pat::Electron, pat::Tau, pat::Jet> PATElecTauPairVBFEventProducer;
typedef VBFCompositePtrCandidateT1T2MEtEventT3Producer<pat::Muon, pat::Tau, pat::Jet> PATMuTauPairVBFEventProducer;
typedef VBFCompositePtrCandidateT1T2MEtEventT3Producer<pat::Tau, pat::Tau, pat::Jet> PATDiTauPairVBFEventProducer;
typedef VBFCompositePtrCandidateT1T2MEtEventT3Producer<pat::Electron, pat::Muon, pat::Jet> PATElecMuPairVBFEventProducer;
typedef VBFCompositePtrCandidateT1T2MEtEventT3Producer<pat::Electron, pat::Electron, pat::Jet> PATDiElecPairVBFEventProducer;
typedef VBFCompositePtrCandidateT1T2MEtEventT3Producer<pat::Muon, pat::Muon, pat::Jet> PATDiMuPairVBFEventProducer;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(DiCandidatePairVBFEventProducer);
DEFINE_FWK_MODULE(PATElecTauPairVBFEventProducer);
DEFINE_FWK_MODULE(PATMuTauPairVBFEventProducer);
DEFINE_FWK_MODULE(PATDiTauPairVBFEventProducer);
DEFINE_FWK_MODULE(PATElecMuPairVBFEventProducer);
DEFINE_FWK_MODULE(PATDiElecPairVBFEventProducer);
DEFINE_FWK_MODULE(PATDiMuPairVBFEventProducer);

