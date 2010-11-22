#include "SusyAnalysis/EventSelector/interface/MyMHTEventSelector.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/JetCorrFactors.h"
#include "DataFormats/JetReco/interface/CaloJet.h"


//__________________________________________________________________________________________________
MyMHTEventSelector::MyMHTEventSelector(const edm::ParameterSet& pset) :
        SusyEventSelector(pset),
        jetTag_(pset.getParameter<edm::InputTag> ("jetTag")),
        minMHT_(pset.getParameter<double> ("minMHT")),
        maxMHT_(pset.getParameter<double> ("maxMHT")),
        maxMHTsig_(pset.getParameter<double> ("maxMHTsig")),
        minPt_(pset.getParameter<double> ("minPt")),
        maxEta_(pset.getParameter<double> ("maxEta")),
        useJetID_(pset.getParameter<bool> ("useJetID")),
        rejectEvtJetID_(pset.getParameter<bool> ("rejectEvtJetID")){

    //// Store computed HT
    defineVariable("MHT");
    defineVariable("MHTsignificance");
}

//__________________________________________________________________________________________________
bool MyMHTEventSelector::select(const edm::Event& event) const {
    //// reset cached variables
    math::XYZTLorentzVector MHT;
    double HT = 0;

    resetVariables();

    //// Get the jets
    edm::Handle<edm::View<pat::Jet> > jetHandle;
    event.getByLabel(jetTag_, jetHandle);
    if (!jetHandle.isValid()) {
        edm::LogWarning("MyMHTEventSelector") << "No Jet results for InputTag " << jetTag_;
        return false;
    }

    //// To be set true if one jet is found failing jetID
    bool badJet = false;
    JetIDSelectionFunctor jetIDLooseCalo( JetIDSelectionFunctor::PURE09, JetIDSelectionFunctor::LOOSE );
    PFJetIDSelectionFunctor jetIDLoosePF( PFJetIDSelectionFunctor::FIRSTDATA, PFJetIDSelectionFunctor::LOOSE );

    //// Sum over jet Ets (with cut on min. pt)
    for( edm::View<pat::Jet>::const_iterator iJet = jetHandle->begin(); iJet != jetHandle->end(); ++iJet ){

        if (iJet->pt() > minPt_ && fabs(iJet->eta()) < maxEta_) {

            bool loose = false;

            if( iJet->isCaloJet() || iJet->isJPTJet() ){
                pat::strbitset ret = jetIDLooseCalo.getBitTemplate();
                ret.set(false);
                loose = jetIDLooseCalo( *iJet, ret );
            }
            else if ( iJet->isPFJet() ){
                pat::strbitset ret = jetIDLoosePF.getBitTemplate();
                ret.set(false);
                loose = jetIDLoosePF( *iJet, ret );
            }

            if (useJetID_ && !(loose)) {
                //std::cout << "Failed JetID: " << iJet->pt() << ", " << iJet->eta() << ", " << iJet->phi() << ", "
                //                              << iJet->emEnergyFraction() << ", " << iJet->jetID().n90Hits << ", "
                //                              << iJet->jetID().fHPD << std::endl;
                badJet = true;
                continue;
            }
            
            math::XYZTLorentzVector p4(iJet->px(), iJet->py(), iJet->pz(), iJet->energy());//   iJet->correctedP4("abs");
            MHT += p4;
            HT += p4.pt();
        }
        //++iJet;
    }

    float myMHT = MHT.pt();
    //std::cout << myMHT << std::endl;
    float myMHTsignificance = MHT.pt() / sqrt(HT);
    //std::cout << "MHT = " << myMHT << ", HT = " << HT << ", MHT sig = " << myMHTsignificance << std::endl;

    setVariable("MHT", myMHT);
    setVariable("MHTsignificance", myMHTsignificance);

    //
    // apply cut
    //
    bool result_maxsig = true;
    if (maxMHTsig_ > 0)
        result_maxsig = (myMHTsignificance < maxMHTsig_);
    bool result_max = true;
    if (maxMHT_ > 0)
        result_max = (myMHT < maxMHT_);
    bool result_min = true;
    if (minMHT_ > 0)
        result_min = (myMHT > minMHT_);
    return (result_min && result_max && result_maxsig && !(rejectEvtJetID_ && badJet));

}

//__________________________________________________________________________________________________
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "SusyAnalysis/EventSelector/interface/EventSelectorFactory.h"
DEFINE_EDM_PLUGIN(EventSelectorFactory, MyMHTEventSelector, "MyMHTEventSelector");
