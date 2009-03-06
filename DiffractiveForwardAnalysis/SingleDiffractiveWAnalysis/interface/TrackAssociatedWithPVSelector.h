#ifndef DiffractiveForwardAnalysis_SingleDiffractiveWAnalysis_TrackAssociatedWithPVSelector_h
#define DiffractiveForwardAnalysis_SingleDiffractiveWAnalysis_TrackAssociatedWithPVSelector_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class TrackAssociatedWithPVSelector {
   public:
      explicit TrackAssociatedWithPVSelector(const edm::ParameterSet&);
      ~TrackAssociatedWithPVSelector();
      bool operator()(const reco::Track&, const edm::Event&) const;
      //bool operator()(const reco::Track &t, const reco::VertexCollection &vtxs) const;
   private:
      edm::InputTag vertexTag_;
      double   deltaZVtxMin_, deltaZVtxMax_;
};

#endif
