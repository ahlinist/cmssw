#ifndef AnalysisDataFormats_TauAnalysis_NSVfitSingleParticleHypothesisBase_h
#define AnalysisDataFormats_TauAnalysis_NSVfitSingleParticleHypothesisBase_h

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/Common/interface/Ptr.h"

#include <string>

class NSVfitSingleParticleHypothesisBase
{
 public:

  NSVfitSingleParticleHypothesisBase()
    : isValidSolution_(false) 
  {}
  NSVfitSingleParticleHypothesisBase(const std::string& name, int barcode)
    : name_(name),
      barcode_(barcode),
      isValidSolution_(false) 
  {}  
  NSVfitSingleParticleHypothesisBase(const edm::Ptr<reco::Candidate>& particle, const std::string& name, int barcode)
    : name_(name),
      barcode_(barcode),
      particle_(particle),
      isValidSolution_(false) 
  {}
  NSVfitSingleParticleHypothesisBase(const NSVfitSingleParticleHypothesisBase& bluePrint)
    : name_(bluePrint.name_),
      barcode_(bluePrint.barcode_),
      particle_(bluePrint.particle_),
      isValidSolution_(false) 
  {}
  virtual ~NSVfitSingleParticleHypothesisBase() {}

  virtual NSVfitSingleParticleHypothesisBase* clone() const = 0;
  virtual NSVfitSingleParticleHypothesisBase* reduceToBase() const = 0;

  virtual NSVfitSingleParticleHypothesisBase& operator=(const NSVfitSingleParticleHypothesisBase& bluePrint)
  {
    name_ = bluePrint.name_;
    barcode_ = bluePrint.barcode_;
    particle_ = bluePrint.particle_;
    //assert(particle_.get() != 0);
    return (*this);
  }

  const std::string& name() const { return name_; }
  int barcode() const { return barcode_; }

  /// pointer to reco::Candidate from which this hypothesis was made
  virtual const edm::Ptr<reco::Candidate>& particle() const { return particle_; }

  virtual void print(std::ostream& stream) const
  {
    stream << "<NSVfitSingleParticleHypothesisBase::print>:" << std::endl;
    stream << " name = " << name_ << std::endl;
    stream << " barcode = " << barcode_ << std::endl;
    stream << " particle(id:key) = " << particle_.id() << ":" << particle_.key() << std::endl;
    stream << " isValidSolution = " << isValidSolution_ << std::endl;
  }

 protected:

  /// "human-readable" label for print-out
  std::string name_;

  /// unique identifier to associate hypothesis to likelihood function
  int barcode_;

  /// pointer to reco::Candidate from which this hypothesis was made
  edm::Ptr<reco::Candidate> particle_;

  /// flag indicating that computed mass hypotheses are physically "valid" solutions
  bool isValidSolution_;
};

// CV: implementation of operator< needed to store NSVfitSingleParticleHypothesisBase objects in edm::OwnVector
bool operator<(const NSVfitSingleParticleHypothesisBase&, const NSVfitSingleParticleHypothesisBase&);

#endif
