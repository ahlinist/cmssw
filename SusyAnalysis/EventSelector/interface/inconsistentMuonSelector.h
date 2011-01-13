#ifndef SusyAnalysis_inconsistentMuonSelector_h_
#define SusyAnalysis_inconsistentMuonSelector_h_
///
/// inconsistentMuonSelector
///
/// \author csander - Thu Jan 13 09:13:15 CET 2011
///
/// $Id: $
#include "SusyAnalysis/EventSelector/interface/SusyEventSelector.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
class inconsistentMuonSelector : public SusyEventSelector {
public:
    inconsistentMuonSelector (const edm::ParameterSet&);
    virtual bool select(const edm::Event&) const;
    virtual ~inconsistentMuonSelector () {}

private:
    // Define parameters here
    edm::InputTag muonTag_;
    double ptMin_;
    double maxPTDiff_;
    bool verbose_;

};
#endif
