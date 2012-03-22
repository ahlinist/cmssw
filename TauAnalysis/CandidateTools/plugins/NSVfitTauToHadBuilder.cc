
/** \class NSVfitTauToHadBuilder
 *
 * Auxiliary class reconstructing tau --> had decays and
 * building NSVfitTauToHadHypothesis objects;
 * used by NSVfit algorithm
 *
 * \author Evan Friis, Christian Veelken; UC Davis
 *
 * \version $Revision: 1.22 $
 *
 * $Id: NSVfitTauToHadBuilder.cc,v 1.22 2011/07/22 15:01:55 veelken Exp $
 *
 */

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "TauAnalysis/CandidateTools/interface/NSVfitTrackService.h"

#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h"
#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitTauDecayBuilder.h"
#include "TauAnalysis/CandidateTools/interface/NSVfitSingleParticleTrackExtractor.h"
#include "TauAnalysis/CandidateTools/interface/NSVfitParameter.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitTauToHadHypothesis.h"

using namespace nSVfit_namespace;

class NSVfitTauToHadBuilder : public NSVfitTauDecayBuilder
{
 public:
  NSVfitTauToHadBuilder(const edm::ParameterSet& cfg)
    : NSVfitTauDecayBuilder(cfg)
  {}
  
  void beginJob(NSVfitAlgorithmBase* algorithm) 
  {
    NSVfitTauDecayBuilder::beginJob(algorithm);

    //if ( verbosity_ ) print(std::cout);
  }

  NSVfitSingleParticleHypothesis* build(const NSVfitTauDecayBuilder::inputParticleMap& inputParticles) const 
  {
    if ( this->verbosity_ ) std::cout << "<NSVfitTauToHadBuilder::build>:" << std::endl;

    inputParticleMap::const_iterator particlePtr = inputParticles.find(prodParticleLabel_);
    assert(particlePtr != inputParticles.end());

    NSVfitTauToHadHypothesis* hypothesis = new NSVfitTauToHadHypothesis(particlePtr->second, prodParticleLabel_, barcodeCounter_);
    ++barcodeCounter_;

    NSVfitTauDecayBuilder::initialize(hypothesis, particlePtr->second.get());

    // Three prong case: check if we can fit a reconstructed vertex.
    const std::vector<const reco::Track*>& tracks = hypothesis->tracks();

    if ( tracks.size() > 1 ) {
      KalmanVertexFitter kvf(true);
      // Get the associated transient tracks
      std::vector<reco::TransientTrack> transTracks = trackService_->transientTracks(tracks.begin(), tracks.end());
      TransientVertex vertex = kvf.vertex(transTracks);
      hypothesis->fittedVertex_ = vertex;
    }

    return hypothesis;
  }

  void applyFitParameter(NSVfitSingleParticleHypothesis* hypothesis, const double* param) const
  {
    NSVfitTauDecayBuilder::applyFitParameter(hypothesis, param);

    NSVfitTauToHadHypothesis* hypothesis_T = dynamic_cast<NSVfitTauToHadHypothesis*>(hypothesis);
    assert(hypothesis_T);
  }

  // The two neutrion system in a leptonic decay can have mass.
  bool nuSystemIsMassless() const { return true; }

  virtual int getDecayMode(const reco::Candidate* candidate) const
  {
    const pat::Tau* tauPtr = dynamic_cast<const pat::Tau*>(candidate);
    assert(tauPtr);
    return tauPtr->decayMode();
  }

  virtual std::vector<const reco::Track*> extractTracks(const reco::Candidate* candidate) const
  {
    const pat::Tau* tauPtr = dynamic_cast<const pat::Tau*>(candidate);
    assert(tauPtr);
    return trackExtractor_(*tauPtr);
  }

  void print(std::ostream& stream) const 
  {
    NSVfitTauDecayBuilder::print(stream);
  }

  private:
    edm::Service<NSVfitTrackService> trackService_;
    NSVfitSingleParticleTrackExtractor<pat::Tau> trackExtractor_;
};

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(NSVfitSingleParticleBuilderPluginFactory, NSVfitTauToHadBuilder, "NSVfitTauToHadBuilder");


