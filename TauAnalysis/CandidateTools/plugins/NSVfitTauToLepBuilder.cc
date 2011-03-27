#include "TauAnalysis/CandidateTools/interface/NSVfitTauDecayBuilderBase.h"
#include "TauAnalysis/CandidateTools/interface/SVfitLegTrackExtractor.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitTauToLepHypothesis.h"
#include "DataFormats/TauReco/interface/PFTauDecayMode.h"

namespace {
void applyOptionalFitParameter(double* param, int idxFitParameter, double& value)
{
  if   ( idxFitParameter != -1 ) value = param[idxFitParameter];
  else                           value = 0.;
}
}

template <typename T>
int getDecayMode(const T* cand)
{
  assert(0); // force template specializations for pat::Electrons/pat::Muons to be used
}

template <>
int getDecayMode<pat::Electron>(const pat::Electron* cand)
{
  return reco::PFTauDecayMode::tauDecaysElectron;
}

template <>
int getDecayMode<pat::Muon>(const pat::Muon* cand)
{
  return reco::PFTauDecayMode::tauDecayMuon;
}

template<typename T>
class NSVfitTauToLepBuilder : public NSVfitTauDecayBuilderBase {
  public:
    NSVfitTauToLepBuilder(const edm::ParameterSet& cfg):
      NSVfitTauDecayBuilderBase(cfg){}

    NSVfitTauDecayHypothesis* buildSpecific(
        const edm::Ptr<reco::Candidate> particle,
        const std::string& label, int barcode) const {
      NSVfitTauToLepHypothesis<T>* hypothesis =
        new NSVfitTauToLepHypothesis<T>(particle, label, barcode);
      const T* objPtr = dynamic_cast<const T*>(particle);
      assert(objPtr);
      hypothesis->decayMode_  = getDecayMode(*objPtr);
      return hypothesis;
    }

    // The two neutrion system in a leptonic decay can have mass.
    bool nuSystemIsMassless() const { return false; }

    virtual int getDecayMode(const reco::Candidate* cand) const {
      const T* objPtr = dynamic_cast<const T*>(cand);
      assert(objPtr);
      return getDecayMode(objPtr);
    }

    virtual std::vector<reco::TrackBaseRef> extractTracks(
        const reco::Candidate* cand) const {
      const T* objPtr = dynamic_cast<const T*>(cand);
      assert(objPtr);
      return trackExtractor_(*objPtr);
    }

    virtual void beginJobSpecific(NSVfitAlgorithmBase* algorithm) {
      // No extra parameters.
    }

    virtual void applyFitParameterSpecific(
        NSVfitTauDecayHypothesis* hypotheis, double *param) const {
      // Nothing to do for leptonic case
    }

  private:
    SVfitLegTrackExtractor<T> trackExtractor_;
};
