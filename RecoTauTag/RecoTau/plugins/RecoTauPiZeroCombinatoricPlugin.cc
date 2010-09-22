/*
 * RecoTauPiZeroCombinatoricPlugin
 *
 * Author: Evan K. Friis, UC Davis
 *
 * Build PiZero candidates out of all possible sets of <choose> gammas that are
 * contained in the input PFJet.  Optionally, the pi zero candidates are 
 * filtered by a min and max selection on their invariant mass.
 *
 * $Id $
 */

#include "RecoTauTag/RecoTau/interface/RecoTauPiZeroPlugins.h"
#include "DataFormats/TauReco/interface/RecoTauPiZero.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/JetReco/interface/PFJet.h"

#include "RecoTauTag/RecoTau/interface/RecoTauCommonUtilities.h"
#include "RecoTauTag/RecoTau/interface/CombinatoricGenerator.h"

#include "CommonTools/CandUtils/interface/AddFourMomenta.h"

#include <algorithm>

namespace reco { namespace tau {

class RecoTauPiZeroCombinatoricPlugin : public RecoTauPiZeroBuilderPlugin {
  public:
    explicit RecoTauPiZeroCombinatoricPlugin(const edm::ParameterSet& pset);
    ~RecoTauPiZeroCombinatoricPlugin() {}
    std::vector<RecoTauPiZero> operator()(const reco::PFJet& jet) const;

  private:
    double minMass_;
    double maxMass_;
    unsigned int maxInputGammas_;
    unsigned int choose_;
    AddFourMomenta p4Builder_;
};

RecoTauPiZeroCombinatoricPlugin::RecoTauPiZeroCombinatoricPlugin(
    const edm::ParameterSet& pset):RecoTauPiZeroBuilderPlugin(pset) {
  minMass_ = pset.getParameter<double>("minMass");
  maxMass_ = pset.getParameter<double>("maxMass");
  maxInputGammas_ = pset.getParameter<unsigned int>("maxInputGammas");
  choose_ = pset.getParameter<unsigned int>("choose");
}

std::vector<RecoTauPiZero> RecoTauPiZeroCombinatoricPlugin::operator()(
    const reco::PFJet& jet) const {
  // Get list of gamma candidates
  typedef std::vector<reco::PFCandidatePtr> PFCandPtrs; 
  typedef PFCandPtrs::const_iterator PFCandIter;
  std::vector<RecoTauPiZero> output;

  PFCandPtrs pfGammaCands = pfGammas(jet);
  // Check if we have anything to do...
  if(pfGammaCands.size() < choose_)
    return output;

  // Define the valid range of gammas to use
  PFCandIter start_iter = pfGammaCands.begin();
  PFCandIter end_iter = pfGammaCands.end();

  // Only take the desired number of piZeros
  end_iter = takeNElements(start_iter, end_iter, maxInputGammas_);

  // Build the combinatoric generator
  typedef CombinatoricGenerator<PFCandPtrs> ComboGenerator;
  ComboGenerator generator(start_iter, end_iter, choose_);

  // Find all possible combinations
  for(ComboGenerator::iterator combo = generator.begin(); 
      combo != generator.end(); ++combo) {
    const Candidate::LorentzVector totalP4;
    RecoTauPiZero piZero(0, totalP4, Candidate::Point(0,0,0), 
        111, 10001, true, name());
    // Add our daughters from this combination
    for(ComboGenerator::combo_iterator candidate = combo->combo_begin(); 
        candidate != combo->combo_end();  ++candidate) {
      piZero.addDaughter(*candidate);
    }
    p4Builder_.set(piZero);

    if (piZero.daughterPtr(0).isNonnull())
      piZero.setVertex(piZero.daughterPtr(0)->vertex());

    if ((maxMass_ < 0 || piZero.mass() < maxMass_) && piZero.mass() > minMass_)
      output.push_back(piZero);
  }
  return output;
}

}} // end namespace reco::tau

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_EDM_PLUGIN(RecoTauPiZeroBuilderPluginFactory, reco::tau::RecoTauPiZeroCombinatoricPlugin, "RecoTauPiZeroCombinatoricPlugin");

