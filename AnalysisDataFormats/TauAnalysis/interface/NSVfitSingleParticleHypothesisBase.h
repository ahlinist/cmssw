#ifndef AnalysisDataFormats_TauAnalysis_NSVfitSingleParticleHypothesisBase_h
#define AnalysisDataFormats_TauAnalysis_NSVfitSingleParticleHypothesisBase_h

#include "DataFormats/Candidate/interface/Candidate.h" 
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/Common/interface/Ptr.h"

#include <string>

class NSVfitSingleParticleHypothesisBase
{
 public:
  NSVfitSingleParticleHypothesisBase() {}
  virtual ~NSVfitSingleParticleHypothesisBase() {}

  /// pointer to reco::Candidate from which this hypothesis was made
  virtual const edm::Ptr<reco::Candidate>& particle() const { return particle_; }

  /// momentum of particle before fit, after fit
  /// and difference in momentum (after - before) fit
  reco::Candidate::LorentzVector p4() const { return p4_; }
  reco::Candidate::LorentzVector p4_fitted() const { return (p4_ + dp4_); }
  reco::Candidate::LorentzVector dp4_fitted() const { return dp4_; }

  /// collection of tracks associated to reco::Candidate
  virtual const reco::TrackRefVector& tracks() const { return tracks_; }

  virtual void print(std::ostream& stream) const
  {
    stream << "<NSVfitSingleParticleHypothesisBase::print>:" << std::endl;
    stream << " name = " << name_ << std::endl;
    stream << " barcode = " << barcode_ << std::endl;
    stream << " p4: Pt = " << p4_.pt() << "," 
	   << " eta = " << p4_.eta() << ", phi = " << p4_.phi() << std::endl;
    stream << " p4_fitted: Pt = " << p4_fitted().pt() << "," 
	   << " eta = " << p4_fitted().eta() << ", phi = " << p4_fitted().phi() << std::endl;
  }

 protected:
  /// "human-readable" label for print-out
  std::string name_;

  /// unique identifier to associate hypothesis to likelihood function
  int barcode_;

  /// pointer to reco::Candidate from which this hypothesis was made
  edm::Ptr<reco::Candidate> particle_;

  /// momentum of particle before fit
  reco::Candidate::LorentzVector p4_;

  /// difference in momentum (after - before) fit
  reco::Candidate::LorentzVector dp4_;

  /// collection of tracks associated to reco::Candidate
  reco::TrackRefVector tracks_;
};

#endif
