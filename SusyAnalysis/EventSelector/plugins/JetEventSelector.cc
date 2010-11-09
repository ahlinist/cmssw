#include "SusyAnalysis/EventSelector/interface/JetEventSelector.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <vector>
#include <sstream>

//________________________________________________________________________________________
JetEventSelector::JetEventSelector(const edm::ParameterSet& pset) :
        SusyEventSelector(pset), jetTag_(pset.getParameter<edm::InputTag> ("jetTag")),
        minPt_(pset.getParameter<std::vector<double> > ("minPt")),
        maxEta_(pset.getParameter<std::vector<double> > ("maxEta")),
        useJetID_(pset.getParameter<bool> ("useJetID")),
rejectEvtJetID_(pset.getParameter<bool> ("rejectEvtJetID")){

    /// definition of variables to be cached
    defineVariable("NumberOfJets");
    for (size_t i = 0; i < minPt_.size(); ++i) {
        std::ostringstream strPt;
        strPt << "Jet" << i << "Pt";
        defineVariable(strPt.str());
        std::ostringstream strEta;
        strEta << "Jet" << i << "Eta";
        defineVariable(strEta.str());
    }

    edm::LogInfo("JetEventSelector") << "constructed with \n" << "  jetTag    = " << jetTag_ << "\n" << "  min #jets = "
    << minPt_.size();
}

//________________________________________________________________________________________
bool JetEventSelector::select(const edm::Event& event) const {
    // reset cached variables
    resetVariables();

    //FIXME: what about checking at construction time?
    if (minPt_.size() != maxEta_.size()) {
        edm::LogError("JetEventSelector") << "Inconsistent length of vector of cut values";
        return false;
    }

    // get the jets
    edm::Handle<pat::JetCollection> jetHandle;
    event.getByLabel(jetTag_, jetHandle);
    if (!jetHandle.isValid()) {
        edm::LogWarning("JetEventSelector") << "No Jet results for InputTag " << jetTag_;
        return false;
    }
    //
    // check number of jets
    //
    setVariable(0, jetHandle->size());
    if (jetHandle->size() < minPt_.size())
        return false;
    //
    // check cuts (assume that jets are sorted by Et)
    //
    bool result(false);
    unsigned int numPassed = 0;

    //// To be set true if one jet is found failing jetID
    bool badJet = false;

    //JetIDLoose jetIDLoose;
    JetIDSelectionFunctor jetIDLooseCalo( JetIDSelectionFunctor::PURE09, JetIDSelectionFunctor::LOOSE );
    PFJetIDSelectionFunctor jetIDLoosePF( PFJetIDSelectionFunctor::FIRSTDATA, PFJetIDSelectionFunctor::LOOSE );

    for (unsigned int i = 0; i < jetHandle->size(); ++i) {

        bool loose = false;
        if( (*jetHandle)[i].isCaloJet() || (*jetHandle)[i].isJPTJet() ){
            pat::strbitset ret = jetIDLooseCalo.getBitTemplate();
            ret.set(false);
            loose = jetIDLooseCalo((*jetHandle)[i], ret);
        }
        else if ( (*jetHandle)[i].isPFJet() ){
            pat::strbitset ret = jetIDLoosePF.getBitTemplate();
            ret.set(false);
            loose = jetIDLoosePF((*jetHandle)[i], ret);
        }

        if (loose && numPassed < minPt_.size()) {
            setVariable(2* numPassed + 1, (*jetHandle)[i].pt());
            setVariable(2* numPassed + 2, (*jetHandle)[i].eta());
        }

        if ((*jetHandle)[i].pt() > minPt_[numPassed] && fabs((*jetHandle)[i].eta()) < maxEta_[numPassed]){

            if(useJetID_ && !(loose)) {
                badJet = true;
                continue;
            }

            ++numPassed;
        }

        if (numPassed == minPt_.size()) result = true;

        if (numPassed > minPt_.size()) continue;

    }

    if (result)
        LogTrace("JetEventSelector") << "JetEventSelector: all jets passed";
    else
        LogTrace("JetEventSelector") << "JetEventSelector: failed";
    return (result  && !(rejectEvtJetID_ && badJet));
}

//________________________________________________________________________________________
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "SusyAnalysis/EventSelector/interface/EventSelectorFactory.h"
DEFINE_EDM_PLUGIN(EventSelectorFactory, JetEventSelector, "JetEventSelector");

