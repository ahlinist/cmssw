#ifndef SusyAnalysis_EventSelector_HTEventSelector_h_
#define SusyAnalysis_EventSelector_HTEventSelector_h_
/// Selector for HT
///
/// Computes HT from jets after selection on pT and |eta|
///   and cuts on it. HT is defined as scalar sum of the
///   the transverse jet energies.
///
/// $Id: HTEventSelector.h,v 1.10 2010/11/05 09:13:53 nowak Exp $

// system include files
#include <memory>

// user include files
#include "SusyAnalysis/EventSelector/interface/SusyEventSelector.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
//#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "PhysicsTools/SelectorUtils/interface/JetIDSelectionFunctor.h"
#include "PhysicsTools/SelectorUtils/interface/PFJetIDSelectionFunctor.h"

class HTEventSelector: public SusyEventSelector {
   public:
      HTEventSelector(const edm::ParameterSet&);
      virtual bool select(const edm::Event&) const;
      virtual ~HTEventSelector() {
      }

   private:
      edm::InputTag jetTag_; ///< tag for input collection
      float minHT_; ///< lower HT cut
      float maxHT_; ///< max HT cut
      float minPt_; ///< minimum Pt of jets taken into account
      float maxEta_; ///< maximum Eta of jets taken into account
      bool useJetID_;
      bool rejectEvtJetID_;
};
#endif
