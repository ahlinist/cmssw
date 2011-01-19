//
// $Id: PATObjectEventSelector.h,v 1.5 2010/04/28 14:51:15 jkolb Exp $
//

#ifndef TauAnalysis_RecoTools_PATObjectEventSelector_h
#define TauAnalysis_RecoTools_PATObjectEventSelector_h

#include "CommonTools/UtilAlgos/interface/AnySelector.h"
#include "CommonTools/UtilAlgos/interface/ObjectCountEventSelector.h"
#include "CommonTools/UtilAlgos/interface/MinNumberSelector.h"
#include "CommonTools/UtilAlgos/interface/MaxNumberSelector.h"
#include "CommonTools/UtilAlgos/interface/AndSelector.h"
#include "CommonTools/UtilAlgos/interface/EventSelectorAdapter.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Common/interface/View.h"

#include <vector>

namespace pat {
  typedef EventSelectorAdapter<ObjectCountEventSelector<edm::View<reco::Candidate>, AnySelector, MinNumberSelector> > PATCandViewMinFilter;
  typedef EventSelectorAdapter<ObjectCountEventSelector<edm::View<reco::Candidate>, AnySelector, MaxNumberSelector> > PATCandViewMaxFilter;
  //typedef EventSelectorAdapter<ObjectCountEventSelector<edm::View<reco::Candidate>, AnySelector, AndSelector<MinNumberSelector, MaxNumberSelector> > > PATCandViewCountFilter;
}

#endif
