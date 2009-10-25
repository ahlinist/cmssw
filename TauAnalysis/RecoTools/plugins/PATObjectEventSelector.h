//
// $Id: PATObjectEventSelector.h,v 1.3.2.1 2009/08/04 10:04:55 mbluj Exp $
//

#ifndef TauAnalysis_RecoTools_PATObjectEventSelector_h
#define TauAnalysis_RecoTools_PATObjectEventSelector_h

#include "PhysicsTools/UtilAlgos/interface/AnySelector.h"
#include "PhysicsTools/UtilAlgos/interface/ObjectCountEventSelector.h"
#include "PhysicsTools/UtilAlgos/interface/MinNumberSelector.h"
#include "PhysicsTools/UtilAlgos/interface/MaxNumberSelector.h"
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
