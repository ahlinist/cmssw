#ifndef TauAnalysis_CandidateTools_NSVfitDecayVertexFitter_h
#define TauAnalysis_CandidateTools_NSVfitDecayVertexFitter_h

/** \class NSVfitDecayVertexFitter
 *
 * Class to fit position of tau decay vertex
 * (speficic to hadronic 3-prong tau decays)
 *
 * \author Evan Friis, Christian Veelken; UC Davis
 *
 * \version $Revision: 1.2 $
 *
 * $Id: NSVfitEventVertexRefitter.h,v 1.2 2011/03/31 16:31:33 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h"
#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

class NSVfitDecayVertexFitter
{
 public:
  NSVfitDecayVertexFitter(const edm::ParameterSet&);
  ~NSVfitDecayVertexFitter();

  void beginEvent(const edm::Event&, const edm::EventSetup&);

  /// Fit decay vertex of three-prong tau
  TransientVertex fitSecondaryVertex(const std::vector<const reco::Track*>&) const;

 private:
  const TransientTrackBuilder* trackBuilder_;

  const KalmanVertexFitter* vertexFitAlgorithm_;

  unsigned minNumTracksFit_;
};

#endif
