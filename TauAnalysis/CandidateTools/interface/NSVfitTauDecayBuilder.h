#ifndef TauAnalysis_CandidateTools_NSVfitTauDecayBuilder_h
#define TauAnalysis_CandidateTools_NSVfitTauDecayBuilder_h

/** \class NSVfitTauDecayBuilder
 *
 * Base-class for building objects that come from tau decays.
 *
 * \author Evan K. Friis, Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: NSVfitTauDecayBuilder.h,v 1.1 2011/05/29 17:58:22 veelken Exp $
 *
 */

#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/TrackReco/interface/Track.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitSingleParticleBuilderBase.h"
#include "TauAnalysis/CandidateTools/interface/NSVfitTrackService.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitTauDecayHypothesis.h"

class NSVfitSingleParticleHypothesis;
class NSVfitAlgorithmBase;

class NSVfitTauDecayBuilder : public NSVfitSingleParticleBuilderBase
{
  public:
    NSVfitTauDecayBuilder(const edm::ParameterSet& cfg)
      : NSVfitSingleParticleBuilderBase(cfg),
        algorithm_(0),
        idxFitParameter_nuInvMass_(-1)
    {}
    virtual ~NSVfitTauDecayBuilder() {}

    // Setup the parameters of the fit.
    virtual void beginJob(NSVfitAlgorithmBase*);

    // Build the tau decay hypothesis from the fit parameters
    virtual void applyFitParameter(NSVfitSingleParticleHypothesis*, const double*) const;

    /* Abstract functions overridden by the different decay type builders */
    // Overridden to allocate the specific decay type.
    virtual bool nuSystemIsMassless() const = 0;
    // The decay mode
    virtual int getDecayMode(const reco::Candidate*) const = 0;
    // Get the track(s) associated to a given Candidate
    virtual std::vector<const reco::Track*> extractTracks(const reco::Candidate*) const = 0;

    virtual void print(std::ostream&) const;

  protected:
    // Initialize data-members common to tau --> e/mu and tau --> had decays
    void initialize(NSVfitTauDecayHypothesis*, const reco::Candidate*) const;

    NSVfitAlgorithmBase* algorithm_;

    edm::Service<NSVfitTrackService> trackService_;

    int idxFitParameter_visEnFracX_;
    int idxFitParameter_phi_lab_;
    int idxFitParameter_nuInvMass_; // used for leptonic decays only.
    int idxFitParameter_deltaR_;

    int idxFitParameter_pvShiftX_;
    int idxFitParameter_pvShiftY_;
    int idxFitParameter_pvShiftZ_;
};

void applyOptionalFitParameter(const double*, int, double&);

#endif /* end of include guard: TauAnalysis_CandidateTools_NSVfitTauDecayBuilder_h */
