#ifndef AnalysisDataFormats_TauAnalysis_NSVfitSingleParticleHypothesisBase_h
#define AnalysisDataFormats_TauAnalysis_NSVfitSingleParticleHypothesisBase_h

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/Common/interface/Ptr.h"

#include <string>

class NSVfitResonanceHypothesis;

class NSVfitSingleParticleHypothesisBase
{
 public:
  NSVfitSingleParticleHypothesisBase() {}
  NSVfitSingleParticleHypothesisBase(const std::string& name, int barcode)
    : mother_(NULL),
      name_(name),
      barcode_(barcode)
  {}
  NSVfitSingleParticleHypothesisBase(const edm::Ptr<reco::Candidate>& particle, const std::string& name, int barcode)
    : mother_(NULL),
      name_(name),
      barcode_(barcode),
      particle_(particle),
      p4_(particle->p4())
  {}
  NSVfitSingleParticleHypothesisBase(const NSVfitSingleParticleHypothesisBase&);
  virtual ~NSVfitSingleParticleHypothesisBase() {}

  virtual NSVfitSingleParticleHypothesisBase* clone() const { return new NSVfitSingleParticleHypothesisBase(*this); }

  virtual NSVfitSingleParticleHypothesisBase& operator=(const NSVfitSingleParticleHypothesisBase&);

  const NSVfitResonanceHypothesis* mother() const { return mother_; }
  void setMother(const NSVfitResonanceHypothesis* mother) {
    mother_ = mother;
  }

  const std::string& name() const { return name_; }
  int barcode() const { return barcode_; }

  /// pointer to reco::Candidate from which this hypothesis was made
  virtual const edm::Ptr<reco::Candidate>& particle() const { return particle_; }

  /// momentum of particle before fit, after fit
  /// and difference in momentum (after - before) fit
  const reco::Candidate::LorentzVector& p4() const { return p4_; }
  const reco::Candidate::LorentzVector& p4_fitted() const { return p4_fitted_; }
  const reco::Candidate::LorentzVector& dp4_fitted() const { return dp4_; }

  /// collection of tracks associated to reco::Candidate
  virtual const std::vector<const reco::Track*>& tracks() const { return tracks_; }

  /// access to position of secondary vertex (tau lepton decay vertex)
  virtual bool hasDecayVertex() const { return false; }
  virtual const AlgebraicVector3& decayVertexPos() const
  {
    throw cms::Exception("NSVfitSingleParticleHypothesisBase::decayVertexPos")
      << " Function not implemented for base-class !!\n";
  }
  virtual const reco::Candidate::Vector& flightPath() const
  {
    throw cms::Exception("NSVfitSingleParticleHypothesisBase::flightPath")
      << " Function not implemented for base-class !!\n";
  }
  virtual double decayDistance() const
  {
    throw cms::Exception("NSVfitSingleParticleHypothesisBase::decayDistance")
      << " Function not implemented for base-class !!\n";
  }

  /// Flag determine whether a reconstructed vertex is associated
  virtual bool hasFittedVertex() const { return false; }
  virtual const reco::Vertex& fittedVertex() const {
    throw cms::Exception("NSVfitSingleParticleHypothesisBase::fittedVertex")
      << " Function not implemented for base-class !!\n";
  }

  virtual void print(std::ostream& stream) const
  {
    stream << "<NSVfitSingleParticleHypothesisBase::print>:" << std::endl;
    stream << " name = " << name_ << std::endl;
    stream << " barcode = " << barcode_ << std::endl;
    stream << " p4: Pt = " << p4_.pt() << ","
	   << " eta = " << p4_.eta() << ", phi = " << p4_.phi() << std::endl;
    stream << " p4_fitted: Pt = " << p4_fitted().pt() << ","
	   << " eta = " << p4_fitted().eta() << ", phi = " << p4_fitted().phi() << std::endl;
    stream << "(x = " << (p4_.energy()/p4_fitted().energy()) << ")" << std::endl;
    stream << " #tracks = " << tracks_.size() << std::endl;
  }

 protected:

  const NSVfitResonanceHypothesis * mother_;

  /// "human-readable" label for print-out
  std::string name_;

  /// unique identifier to associate hypothesis to likelihood function
  int barcode_;

  /// pointer to reco::Candidate from which this hypothesis was made
  edm::Ptr<reco::Candidate> particle_;

  /// momentum of particle before fit, after fit
  /// and difference (after - before) fit
  reco::Candidate::LorentzVector p4_;
  reco::Candidate::LorentzVector p4_fitted_;
  reco::Candidate::LorentzVector dp4_;

  /// collection of tracks associated to reco::Candidate
  std::vector<const reco::Track*> tracks_; // transient data-member
};

bool operator<(const NSVfitSingleParticleHypothesisBase&, const NSVfitSingleParticleHypothesisBase&);

#endif
