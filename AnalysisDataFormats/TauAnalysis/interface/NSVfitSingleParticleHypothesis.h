#ifndef AnalysisDataFormats_TauAnalysis_NSVfitSingleParticleHypothesis_h
#define AnalysisDataFormats_TauAnalysis_NSVfitSingleParticleHypothesis_h

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/Common/interface/Ptr.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitSingleParticleHypothesisBase.h"

#include <string>

class NSVfitResonanceHypothesis;

class NSVfitSingleParticleHypothesis : public NSVfitSingleParticleHypothesisBase
{
 public:

  NSVfitSingleParticleHypothesis();
  NSVfitSingleParticleHypothesis(const std::string&, int);
  NSVfitSingleParticleHypothesis(const edm::Ptr<reco::Candidate>&, const std::string&, int);
  NSVfitSingleParticleHypothesis(const NSVfitSingleParticleHypothesis&);

  NSVfitSingleParticleHypothesis& operator=(const NSVfitSingleParticleHypothesis&);

  ~NSVfitSingleParticleHypothesis() {}

  const NSVfitResonanceHypothesis* mother() const { return mother_; }
  void setMother(const NSVfitResonanceHypothesis* mother) 
  {
    mother_ = mother;
  }

  /// momentum of particle before fit, after fit
  /// and difference in momentum (after - before) fit
  const reco::Candidate::LorentzVector& p4() const { return p4_; }
  const reco::Candidate::LorentzVector& p4_fitted() const { return p4_fitted_; }
  const reco::Candidate::LorentzVector& dp4_fitted() const { return dp4_; }

  /// mean and median plus -1 sigma and +1 sigma limits on 
  /// reconstructed Pt, eta, phi
  ///
  /// NOTE: Pt, eta, phi will be valid only in case
  ///       NSVfitAlgorithmByIntegration2 algorithm (Markov Chain integration) is used
  ///  
  double pt() const { return pt_; }
  double ptErrUp() const { return ptErrUp_; }
  double ptErrDown() const { return ptErrDown_; }
  bool pt_isValid() const { return pt_isValid_; }
  double eta() const { return eta_; }
  double etaErrUp() const { return etaErrUp_; }
  double etaErrDown() const { return etaErrDown_; }
  bool eta_isValid() const { return eta_isValid_; }
  double phi() const { return phi_; }
  double phiErrUp() const { return phiErrUp_; }
  double phiErrDown() const { return phiErrDown_; }
  bool phi_isValid() const { return phi_isValid_; }

  void print(std::ostream& stream) const
  {
    stream << "<NSVfitSingleParticleHypothesis::print>:" << std::endl;
    stream << " name = " << name_ << std::endl;
    stream << " barcode = " << barcode_ << std::endl;
    stream << " particle(id:key) = " << particle_.id() << ":" << particle_.key() << std::endl;
    stream << " p4: Pt = " << p4_.pt() << ","
	   << " eta = " << p4_.eta() << ", phi = " << p4_.phi() 
	   << " (Px = " << p4_.px() << ", Py = " << p4_.py() << ")" << std::endl;
    stream << " p4_fitted: Pt = " << p4_fitted().pt() << ","
	   << " eta = " << p4_fitted().eta() << ", phi = " << p4_fitted().phi() 
	   << " (Px = " << p4_fitted().px() << ", Py = " << p4_fitted().py() << ")" << std::endl;
    stream << "(x = " << (p4_.energy()/p4_fitted().energy()) << ")" << std::endl;
    stream << " isValidSolution = " << isValidSolution_ << std::endl;
  }

  unsigned numPolStates() const { return numPolStates_; }
  int polHandedness(unsigned idx) const { return polHandedness_[idx]; }
  int polSign(unsigned idx) const { return polSign_[idx]; }

  enum { kPolUndefined, kPolL, kPolR };
  enum { kPolPlus = 1, kPolMinus = 2 };

  friend class NSVfitResonanceBuilder;
  friend class NSVfitAlgorithmByLikelihoodMaximization;  
  friend class NSVfitAlgorithmByIntegration2;
  template<typename T1, typename T2> friend class CompositePtrCandidateT1T2MEt;

 protected:

  /// pointer to resonance hypothesis object
  const NSVfitResonanceHypothesis* mother_;

  /// momentum of particle before fit, after fit
  /// and difference (after - before) fit
  reco::Candidate::LorentzVector p4_;
  reco::Candidate::LorentzVector p4_fitted_;
  reco::Candidate::LorentzVector dp4_;  

  /// "best fit" reconstructed Pt, eta, phi 
  /// plus -1 sigma and +1 sigma limits
  ///
  /// NOTE: uncertainties on Pt, eta, phi will be valid only in case
  ///       NSVfitAlgorithmByIntegration2 algorithm (Markov Chain integration) is used
  ///  
  double pt_;
  double ptErrUp_;
  double ptErrDown_;
  bool pt_isValid_;
  double eta_;
  double etaErrUp_;
  double etaErrDown_;
  bool eta_isValid_;
  double phi_;
  double phiErrUp_;
  double phiErrDown_;
  bool phi_isValid_;

  /// different possible polarization states
  std::vector<int> polHandedness_;
  std::vector<int> polSign_;
  unsigned numPolStates_;
};

#endif
