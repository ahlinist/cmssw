//
// $Id: PATObjectEventSelector.h,v 1.2 2009/03/20 16:50:24 veelken Exp $
//

#ifndef TauAnalysis_RecoTools_PATObjectEventSelector_h
#define TauAnalysis_RecoTools_PATObjectEventSelector_h

#include "PhysicsTools/UtilAlgos/interface/AnySelector.h"
#include "PhysicsTools/UtilAlgos/interface/ObjectCountEventSelector.h"
#include "PhysicsTools/UtilAlgos/interface/MinNumberSelector.h"
#include "PhysicsTools/PatUtils/interface/MaxNumberSelector.h"
#include "PhysicsTools/UtilAlgos/interface/AndSelector.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Common/interface/View.h"

#include <vector>

namespace pat {
  typedef ObjectCountEventSelector<edm::View<reco::Candidate>, AnySelector, MinNumberSelector> PATCandViewMinEventSelector;
  typedef ObjectCountEventSelector<edm::View<reco::Candidate>, AnySelector, MaxNumberSelector> PATCandViewMaxEventSelector;
  typedef ObjectCountEventSelector<edm::View<reco::Candidate>, AnySelector, AndSelector<MinNumberSelector, MaxNumberSelector> > PATCandViewCountEventSelector;
}

#endif
