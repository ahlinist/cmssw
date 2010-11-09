#include "SusyAnalysis/EventSelector/interface/MHTJetEventSelector.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/JetCorrFactors.h"
//#include "PhysicsTools/Utilities/interface/deltaPhi.h"
//#include "PhysicsTools/Utilities/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "TMath.h"
#include <vector>

//________________________________________________________________________________________
MHTJetEventSelector::MHTJetEventSelector(const edm::ParameterSet& pset) :
        SusyEventSelector(pset),
        jetTag_(pset.getParameter<edm::InputTag> ("jetTag")),
        mhtDphiMin_(pset.getParameter<double> ("mhtDPhiMin")),
        dPhiJet1MHTMin_(pset.getParameter<double> ("dPhiJet1MHTMin")),
        dPhiJet2MHTMin_(pset.getParameter<double> ("dPhiJet2MHTMin")),
        rDistJetsMin_(pset.getParameter<double> ("rDistJetsMin")),
        nJetsMHTIso_(pset.getParameter<unsigned int> ("NJets_mhtIso")),
        minPt_(pset.getParameter<double> ("minPt")),
        maxEta_(pset.getParameter<double> ("maxEta")),
        useJetID_(pset.getParameter<bool> ("useJetID")),
rejectEvtJetID_(pset.getParameter<bool> ("rejectEvtJetID")) {

    // Define all variables we want to cache (and eventually plot...)
    defineVariable("mhtDphi");
    defineVariable("numberOfJets");
    defineVariable("dPhiJet1MHT");
    defineVariable("dPhiJet2MHT");
    defineVariable("dPhiJet3MHT");
    defineVariable("R1");
    defineVariable("R2");

}

//________________________________________________________________________________________
bool MHTJetEventSelector::select(const edm::Event& event) const {
    math::XYZTLorentzVector HT;
    // Reset cached variables
    resetVariables();

    // Get the jets
    edm::Handle<edm::View<pat::Jet> > jets;
    event.getByLabel(jetTag_, jets);
    if (!jets.isValid()) {
        edm::LogWarning("MHTJetEventSelector") << "No Jet results for InputTag " << jetTag_;
        return false;
    }

    //// To be set true if one jet is found failing jetID
    bool badJet = false;
    JetIDSelectionFunctor jetIDLooseCalo( JetIDSelectionFunctor::PURE09, JetIDSelectionFunctor::LOOSE );
    PFJetIDSelectionFunctor jetIDLoosePF( PFJetIDSelectionFunctor::FIRSTDATA, PFJetIDSelectionFunctor::LOOSE );
    //
    // Preselection: number of jets (need at least 3(?) to make sense out of these variables)
    //
    setVariable("numberOfJets", jets->size());
    if (jets->size() < 3)
        return false;

    //
    // Compute variables
    //

    //edm::View<pat::Jet>::const_iterator iJet = jets->begin();
    //while (iJet != jets->end()) {

    for( edm::View<pat::Jet>::const_iterator iJet = jets->begin(); iJet != jets->end(); ++iJet ){


        if (iJet->pt() < minPt_ || fabs(iJet->eta()) > maxEta_) continue;

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
            badJet = true;
            continue;
        }

        math::XYZTLorentzVector p4(iJet->px(), iJet->py(), iJet->pz(), iJet->energy()); //iJet->correctedP4("abs");
        HT -= p4;

    }

    // MET "isolation" (calculated on at most nJetsMetIso_ jets)
    float metIso = 100.;
    for (unsigned int iJet = 0; iJet < nJetsMHTIso_ && iJet < jets->size(); ++iJet) {
        double deltaPhiAbs = fabs(reco::deltaPhi((*jets)[iJet].phi(), HT.Phi()));
        if (metIso > deltaPhiAbs)
            metIso = deltaPhiAbs;
    }
    setVariable("mhtDphi", metIso);

    // MET and leading jets deltaPhi
    double dPhiJet1Met = fabs(reco::deltaPhi((*jets)[0].phi(), HT.Phi()));
    setVariable("dPhiJet1MHT", dPhiJet1Met);
    double dPhiJet2Met = fabs(reco::deltaPhi((*jets)[1].phi(), HT.Phi()));
    setVariable("dPhiJet2MHT", dPhiJet2Met);
    double dPhiJet3Met = fabs(reco::deltaPhi((*jets)[2].phi(), HT.Phi()));
    setVariable("dPhiJet3MHT", dPhiJet3Met);

    // R1 & R2
    float R1 = sqrt(TMath::Power(dPhiJet2Met, 2) + TMath::Power(dPhiJet1Met - TMath::Pi(), 2));
    float R2 = sqrt(TMath::Power(dPhiJet1Met, 2) + TMath::Power(dPhiJet2Met - TMath::Pi(), 2));
    setVariable("R1", R1);
    setVariable("R2", R2);

    //
    // Perform final selection
    //
    if (metIso < mhtDphiMin_)
        return false;
    if (dPhiJet2Met < dPhiJet2MHTMin_)
        return false;
    if (dPhiJet1Met < dPhiJet1MHTMin_)
        return false;
    if (R1 < rDistJetsMin_)
        return false;
    if (R2 < rDistJetsMin_)
        return false;

    return (!(rejectEvtJetID_ && badJet));

}

//________________________________________________________________________________________
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "SusyAnalysis/EventSelector/interface/EventSelectorFactory.h"
DEFINE_EDM_PLUGIN(EventSelectorFactory, MHTJetEventSelector, "MHTJetEventSelector");
