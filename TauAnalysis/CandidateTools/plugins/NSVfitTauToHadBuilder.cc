
/** \class NSVfitTauToHadBuilder
 *
 * Auxiliary class reconstructing tau --> had decays and
 * building NSVfitTauToHadHypothesis objects;
 * used by NSVfit algorithm
 *
 * \author Evan Friis, Christian Veelken; UC Davis
 *
 * \version $Revision: 1.24 $
 *
 * $Id: NSVfitTauToHadBuilder.cc,v 1.24 2012/03/26 15:47:49 veelken Exp $
 *
 */

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "TauAnalysis/CandidateTools/interface/NSVfitTrackService.h"

#include "FWCore/Utilities/interface/Exception.h"

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
    inputParticleMap::const_iterator particlePtr = inputParticles.find(prodParticleLabel_);
    assert(particlePtr != inputParticles.end());

    NSVfitTauToHadHypothesis* hypothesis = new NSVfitTauToHadHypothesis(particlePtr->second, prodParticleLabel_, barcodeCounter_);
    if ( verbosity_ ) {
      std::cout << "<NSVfitTauToHadBuilder::build>:" << std::endl;
      std::cout << " hypothesis #" << barcodeCounter_ << ": " << hypothesis << std::endl;
    }
    ++barcodeCounter_;

    NSVfitTauDecayBuilder::initialize(hypothesis, particlePtr->second.get());

    return hypothesis;
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
    if ( !tauPtr ) 
      throw cms::Exception("NSVfitTauToHadBuilder")
	<< "Failed to extract tracks. Please check if correct Builder plugin type is used !!\n";
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


