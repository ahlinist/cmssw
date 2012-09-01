#ifndef TauAnalysis_CandidateTools_NSVfitTauDecayBuilder_h
#define TauAnalysis_CandidateTools_NSVfitTauDecayBuilder_h

/** \class NSVfitTauDecayBuilder
 *
 * Base-class for building objects that come from tau decays.
 *
 * \author Evan K. Friis, Christian Veelken, UC Davis
 *
 * \version $Revision: 1.3 $
 *
 * $Id: NSVfitTauDecayBuilder.h,v 1.3 2012/08/28 15:00:20 veelken Exp $
 *
 */

#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Common/interface/View.h"

#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitSingleParticleBuilderBase.h"
#include "TauAnalysis/CandidateTools/interface/NSVfitTrackService.h"
#include "TauAnalysis/CandidateTools/interface/NSVfitDecayVertexFitter.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitTauDecayHypothesis.h"

class NSVfitSingleParticleHypothesis;
class NSVfitAlgorithmBase;

class NSVfitTauDecayBuilder : public NSVfitSingleParticleBuilderBase
{
  public:
    NSVfitTauDecayBuilder(const edm::ParameterSet&);
    virtual ~NSVfitTauDecayBuilder();

    // Setup the parameters of the fit.
    virtual void beginJob(NSVfitAlgorithmBase*);

    // Access TrackBuilder
    virtual void beginEvent(const edm::Event&, const edm::EventSetup&);

    // Add Tracking information
    // 
    // NOTE: this function needs to be called after the event has been build,
    //       as the tracking information depends on the primary event vertex
    //      (which in turn depends on the tracks of daughter particles)
    //
    virtual void finalize(NSVfitSingleParticleHypothesis*) const;

    // Build the tau decay hypothesis from the fit parameters
    virtual bool applyFitParameter(NSVfitSingleParticleHypothesis*, const double*) const;

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
    const TransientTrackBuilder* trackBuilder_;

    mutable std::vector<reco::TransientTrack> selectedTracks_;

    NSVfitDecayVertexFitter* decayVertexFitAlgorithm_;

    unsigned trackMinNumHits_;
    unsigned trackMinNumPixelHits_;
    double   trackMaxChi2DoF_;
    double   trackMaxDeltaPoverP_;
    double   trackMinPt_;

    int idxFitParameter_visEnFracX_;
    int idxFitParameter_phi_lab_;
    int idxFitParameter_nuInvMass_; // used for leptonic decays only.
    int idxFitParameter_deltaR_;

    /// optional parameters for setting reconstructed to Monte Carlo truth values
    edm::InputTag srcGenTaus_;
    typedef edm::View<reco::GenParticle> GenParticleView;
    edm::Handle<GenParticleView> genParticles_;
    double dRmatch_;
    bool fixToGenVisEnFracX_;
    bool initializeToGenVisEnFracX_;
    mutable double genVisEnFracX_;
    bool fixToGenPhiLab_;
    bool initializeToGenPhiLab_;
    mutable double genPhiLab_;
    bool fixToGenNuInvMass_;
    bool initializeToGenNuInvMass_;
    mutable double genNuInvMass_;
    bool fixToGenDeltaR_;
    bool initializeToGenDeltaR_;
    mutable double genDeltaR_;
    bool fixToGenVisP4_;
    mutable reco::Candidate::LorentzVector genVisP4_;

    mutable long numWarnings_compIntersection_of_lines_;
};

void applyOptionalFitParameter(const double*, int, double&);

#endif /* end of include guard: TauAnalysis_CandidateTools_NSVfitTauDecayBuilder_h */
