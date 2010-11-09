#ifndef SusyAnalysis_JetIDEventSelector_h_
#define SusyAnalysis_JetIDEventSelector_h_
///
/// JetIDEventSelector
///
/// \author csander - Tue Nov  9 09:26:20 CET 2010
///
/// $Id: $
#include "SusyAnalysis/EventSelector/interface/SusyEventSelector.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
//#include "FWCore/ParameterSet/interface/InputTag.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "PhysicsTools/SelectorUtils/interface/JetIDSelectionFunctor.h"
#include "PhysicsTools/SelectorUtils/interface/PFJetIDSelectionFunctor.h"

class JetIDEventSelector : public SusyEventSelector {
public:
    JetIDEventSelector (const edm::ParameterSet&);
    virtual bool select(const edm::Event&) const;
    virtual ~JetIDEventSelector () {}

private:
    // Define parameters here
    edm::InputTag jetTag_; ///< tag for input collection
    double minPt_; ///< lower Et cuts (also defines min. #jets)
    double maxEta_; ///< upper |eta| cuts (also defines min. #jets)
};
#endif
