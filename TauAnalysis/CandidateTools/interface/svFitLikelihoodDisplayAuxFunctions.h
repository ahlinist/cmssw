#ifndef TauAnalysis_CandidateTools_svFitLikelihoodDisplayAuxFunctions_h
#define TauAnalysis_CandidateTools_svFitLikelihoodDisplayAuxFunctions_h

#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Math/interface/AlgebraicROOTObjects.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitDecayVertexFitter.h"

#include <vector>
#include <string>
#include <iostream>

struct matchedTauDecayType
{
  matchedTauDecayType();
  matchedTauDecayType(const reco::GenParticle*, const reco::Candidate*, const TransientTrackBuilder*, NSVfitDecayVertexFitter*);
  ~matchedTauDecayType();

  void print(std::ostream& stream) const;
  
  const reco::GenParticle* genTau_;
  reco::Candidate::LorentzVector genTauP4_;
  double genTauCharge_;
  std::vector<const reco::GenParticle*> genTauDecayProducts_;
  AlgebraicVector3 genTauProdVertexPos_;
  AlgebraicVector3 genTauDecayVertexPos_;
  double genTauDecayDistance_;
  std::string genTauDecayMode_;
  bool isLeptonicDecay_;
  reco::Candidate::LorentzVector genVisP4_;
  reco::Candidate::LorentzVector genInvisP4_;
  
  const reco::Candidate* recCandidate_;
  reco::Candidate::LorentzVector recVisP4_;  
  int recTauDecayMode_;
  std::vector<const reco::Track*> recTracks_;
  std::vector<const reco::Track*> recSelTracks_;
  const reco::Track* recLeadTrack_;
  reco::TransientTrack* recLeadTrackTrajectory_;
  const TransientTrackBuilder* trackBuilder_;
  TransientVertex recTauDecayVertex_;
  AlgebraicVector3 recTauDecayVertexPos_;
  AlgebraicMatrix33 recTauDecayVertexCov_;
  bool hasRecTauDecayVertex_;
};
  
void matchRecToGenTauDecays(const edm::View<reco::Candidate>&, const std::vector<const reco::GenParticle*>&, double,
			    std::vector<matchedTauDecayType*>&, 
			    const TransientTrackBuilder* = 0, NSVfitDecayVertexFitter* = 0);

#endif   
