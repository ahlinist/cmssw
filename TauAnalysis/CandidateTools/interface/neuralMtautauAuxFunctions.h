#ifndef TauAnalysis_CandidateTools_neuralMtautauAuxFunctions_h
#define TauAnalysis_CandidateTools_neuralMtautauAuxFunctions_h

#include "DataFormats/Candidate/interface/Candidate.h"

#include <TMatrixD.h>

double compZetaPhi(const reco::Candidate::LorentzVector&, const reco::Candidate::LorentzVector&);

reco::Candidate::LorentzVector compP4inZetaFrame(const reco::Candidate::LorentzVector&, double);
TMatrixD compCovMatrixInZetaFrame(const TMatrixD&, double);

#endif
