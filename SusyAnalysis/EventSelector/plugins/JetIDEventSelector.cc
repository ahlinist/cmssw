#include "SusyAnalysis/EventSelector/interface/SusyEventSelector.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "SusyAnalysis/EventSelector/interface/JetIDEventSelector.h"

//________________________________________________________________________________________
JetIDEventSelector::JetIDEventSelector (const edm::ParameterSet& pset ) :
        SusyEventSelector(pset), jetTag_(pset.getParameter<edm::InputTag> ("jetTag")),
        minPt_(pset.getParameter<double> ("minPt")),
        maxEta_(pset.getParameter<double> ("maxEta"))
{

    // Define variables to store
    // defineVariable("myVariable");

}

//________________________________________________________________________________________
bool
JetIDEventSelector::select (const edm::Event& event) const
{

    // get the jets
    edm::Handle<pat::JetCollection> jetHandle;
    event.getByLabel(jetTag_, jetHandle);
    if (!jetHandle.isValid()) {
        edm::LogWarning("JetEventSelector") << "No Jet results for InputTag " << jetTag_;
        return false;
    }

    // Reset cached variables
    resetVariables();

    //// To be set true if one jet is found failing jetID
    bool badJet = false;

    //JetIDLoose jetIDLoose;
    JetIDSelectionFunctor jetIDLooseCalo( JetIDSelectionFunctor::PURE09, JetIDSelectionFunctor::LOOSE );
    PFJetIDSelectionFunctor jetIDLoosePF( PFJetIDSelectionFunctor::FIRSTDATA, PFJetIDSelectionFunctor::LOOSE );

    for (unsigned int i = 0; i < jetHandle->size(); ++i) {

        if( (*jetHandle)[i].pt() < minPt_ || fabs((*jetHandle)[i].eta()) > maxEta_ ) continue;

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

        if (!loose){
            //std::cout << "BadJet found: " << std::endl;
            //std::cout << "pt, eta: " << (*jetHandle)[i].pt() << " " << (*jetHandle)[i].eta() << std::endl;
            badJet = true;
            break;
        }

    }

    // Store variables
    // setVariable("myVariable",myValue);

    // Return selection
    // return (myValue > minValue);
    return (!badJet);

}

//__________________________________________________________________________________________________
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "SusyAnalysis/EventSelector/interface/EventSelectorFactory.h"
DEFINE_EDM_PLUGIN(EventSelectorFactory, JetIDEventSelector, "JetIDEventSelector");

