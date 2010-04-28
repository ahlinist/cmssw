//
// $Id: PATObjectEventSelector.h,v 1.4 2009/10/25 12:38:23 veelken Exp $
//

#ifndef TauAnalysis_RecoTools_PATObjectEventSelector_h
#define TauAnalysis_RecoTools_PATObjectEventSelector_h

#include "CommonTools/UtilAlgos/interface/AnySelector.h"
#include "CommonTools/UtilAlgos/interface/ObjectCountEventSelector.h"
#include "CommonTools/UtilAlgos/interface/MinNumberSelector.h"
#include "CommonTools/UtilAlgos/interface/MaxNumberSelector.h"
#include "CommonTools/UtilAlgos/interface/AndSelector.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Common/interface/View.h"

#include <vector>

namespace pat {
  typedef ObjectCountEventSelector<edm::View<reco::Candidate>, AnySelector, MinNumberSelector> PATCandViewMinEventSelector;
  typedef ObjectCountEventSelector<edm::View<reco::Candidate>, AnySelector, MaxNumberSelector> PATCandViewMaxEventSelector;
  typedef ObjectCountEventSelector<edm::View<reco::Candidate>, AnySelector, AndSelector<MinNumberSelector, MaxNumberSelector> > PATCandViewCountEventSelector;
}

#endif
