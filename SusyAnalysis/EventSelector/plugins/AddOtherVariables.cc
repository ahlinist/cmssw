#include "SusyAnalysis/EventSelector/interface/AddOtherVariables.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "TMath.h"
#include "PhysicsTools/SelectorUtils/interface/JetIDSelectionFunctor.h"
#include "PhysicsTools/SelectorUtils/interface/PFJetIDSelectionFunctor.h"

#include <vector>
#include <numeric>
//__________________________________________________________________________________________________
AddOtherVariables::AddOtherVariables(const edm::ParameterSet& pset) :
        SusyEventSelector(pset),
        jetTag_(pset.getParameter<edm::InputTag> ("jetTag")),
        metTag_(pset.getParameter<edm::InputTag> ("metTag")),
        electronTag_(pset.getParameter<edm::InputTag> ("electronTag")),
        muonTag_(pset.getParameter<edm::InputTag> ("muonTag")),
        vertex_(pset.getParameter<edm::InputTag> ("vertex")),

        jetptmin_(pset.getParameter<double> ("JetPtMin")),
        jetetamaxHT_(pset.getParameter<double> ("JetEtaMaxHT")),
        jetetamaxMHT_(pset.getParameter<double> ("JetEtaMaxMHT")),
        useJetID_(pset.getParameter<bool> ("useJetID")),
        rejectEvtJetID_(pset.getParameter<bool> ("rejectEvtJetID"))
        //  tauTag_    ( pset.getParameter<edm::InputTag>("tau")       ),
{
    // Store counter
    defineVariable("SET");
    defineVariable("MET");
    defineVariable("METsig");
    defineVariable("HT");
    defineVariable("MHT");
    defineVariable("MHTsig");
    defineVariable("alphaT");
    defineVariable("dPhiMin");
    defineVariable("dPhiBiased");
    defineVariable("JetMult");

    defineVariable("MHTphi");
    defineVariable("METphi");

    defineVariable("dPhiMin2");
    defineVariable("dPhiMin3");
    defineVariable("dPhiMin4");
    defineVariable("dPhiMinPt60");
    defineVariable("dPhiMinPt80");
    defineVariable("dPhiMinPt100");

    defineVariable("dPhiDual2");
    defineVariable("dPhiDual3");
    defineVariable("dPhiDual4");
    defineVariable("dPhiDualPt60");
    defineVariable("dPhiDualPt80");
    defineVariable("dPhiDualPt100");

    defineVariable("dPhiBiased2");
    defineVariable("dPhiBiased3");
    defineVariable("dPhiBiased4");
    defineVariable("dPhiBiasedPt60");
    defineVariable("dPhiBiasedPt80");
    defineVariable("dPhiBiasedPt100");

    defineVariable("deltaPhiJet12");
    defineVariable("deltaPhiJet13");
    defineVariable("deltaPhiJet23");
    defineVariable("dRJet12");
    defineVariable("dRJet13");
    defineVariable("dRJet23");
    defineVariable("deltaPhiJet1MHT");
    defineVariable("deltaPhiJet2MHT");
    defineVariable("deltaPhiJet3MHT");

    //.... other variables we don't cut on and are not produced in other selectors


    //defineVariable("nTaus");
    jetIDLooseCalo_ = new JetIDSelectionFunctor( JetIDSelectionFunctor::PURE09, JetIDSelectionFunctor::LOOSE );
    jetIDLoosePF_   = new PFJetIDSelectionFunctor( PFJetIDSelectionFunctor::FIRSTDATA, PFJetIDSelectionFunctor::LOOSE );
}

//
// member functions
//
// ------------ biased delta phi ----------------------------
double AddOtherVariables::dPhiBiased(const edm::View<reco::Candidate> * jets,
                                     const edm::View<reco::Candidate>::const_iterator excljet) const {
    math::PtEtaPhiMLorentzVector mhtvec(0.0, 0.0, 0.0, 0.0);
    for (edm::View<reco::Candidate>::const_iterator jet = jets->begin(); jet != jets->end(); ++jet) {
        if (fabs(jet->eta()) > jetetamaxMHT_)
            continue;
        if (jet->pt() < jetptmin_)
            continue;
        if (jet == excljet)
            continue;
        mhtvec -= jet->p4();
    }
    return fabs(deltaPhi(mhtvec, *excljet));
}

// ------------ helper func for alphaT ----------------------------
std::vector<double> AddOtherVariables::deltaSumPt_permutations(const std::vector<LorentzV>& p4s) const {
    std::vector<std::vector<double> > ht(1 << (p4s.size() - 1), std::vector<double>(2, 0.));
    for (unsigned i = 0; i < ht.size(); i++) {
        for (unsigned j = 0; j < p4s.size(); j++) {
            ht[i][(i / (1 << j)) % 2] += p4s[j].pt();
        }
    }
    std::vector<double> deltaHT;
    for (unsigned i = 0; i < ht.size(); i++)
        deltaHT.push_back(fabs(ht[i][0] - ht[i][1]));
    return deltaHT;
}

// ------------ alphaT calculation ----------------------------
double AddOtherVariables::alphaT(const std::vector<LorentzV>& p4s) const {
    if (p4s.size() < 2)
        return 0;

    std::vector<double> pTs;
    for (unsigned i = 0; i < p4s.size(); i++)
        pTs.push_back(p4s[i].pt());
    for (unsigned i = 0; i < p4s.size(); i++)
        pTs.push_back(p4s[i].pt());
    const std::vector<double> DHTper(deltaSumPt_permutations(p4s));

    const double mDHT = *(std::min_element(DHTper.begin(), DHTper.end()));
    const double sumPT = accumulate(pTs.begin(), pTs.end(), double(0));
    const LorentzV sumP4 = accumulate(p4s.begin(), p4s.end(), LorentzV());

    return 0.5 * (sumPT - mDHT) / sqrt(sumPT * sumPT - sumP4.perp2());
}


bool AddOtherVariables::isLoose(const pat::Jet* jet) const {
    if (!useJetID_ || !jet) return true;
    else if( jet->isCaloJet() || jet->isJPTJet() ){
        pat::strbitset ret = jetIDLooseCalo_->getBitTemplate();
        ret.set(false);
        return (*jetIDLooseCalo_)( *jet, ret );
    }
    else if ( jet->isPFJet() ){
        pat::strbitset ret = jetIDLoosePF_->getBitTemplate();
        ret.set(false);
        return (*jetIDLoosePF_)( *jet, ret );
    }
    return false;
}


//__________________________________________________________________________________________________
bool AddOtherVariables::select(const edm::Event& event) const {

    // Input collections
    edm::Handle<std::vector<pat::Electron> > eleHandle;
    event.getByLabel(electronTag_, eleHandle);
    edm::Handle<std::vector<pat::Muon> > muonHandle;
    event.getByLabel(muonTag_, muonHandle);
    reco::Vertex myBeamSpot;
    edm::Handle <edm::View<reco::Vertex> >vertices;
    event.getByLabel(vertex_, vertices);
    if(!vertices->empty() && !vertices->front().isFake() )
        myBeamSpot = vertices->front();
    //const math::XYZPointD & myPosition = myBeamSpot.position();
    //  edm::Handle< std::vector<pat::Tau> > tauHandle;
    //  event.getByLabel(tauTag_, tauHandle);

    //jet collection - can be either reco::jet or pat::jet
    edm::Handle<edm::View<reco::Candidate> > jet_hnd;
    event.getByLabel(jetTag_, jet_hnd);

    //MET collection - can be either reco::jet or pat::jet
    edm::Handle<edm::View<reco::MET> > met_hnd;
    event.getByLabel(metTag_, met_hnd);

    double ht = 0.0, SEt=0;
    double _dPhiMin2 = 999.;
    double _dPhiMin3 = 999.;
    double _dPhiMin4 = 999.;
    double _dPhiMinPt60 = 999.;
    double _dPhiMinPt80 = 999.;
    double _dPhiMinPt100 = 999.;
    double _dPhiDual2 = 999.;
    double _dPhiDual3 = 999.;
    double _dPhiDual4 = 999.;
    double _dPhiDualPt60 = 999.;
    double _dPhiDualPt80 = 999.;
    double _dPhiDualPt100 = 999.;
    double _dPhiBiased2 = 999.;
    double _dPhiBiased3 = 999.;
    double _dPhiBiased4 = 999.;
    double _dPhiBiasedPt60 = 999.;
    double _dPhiBiasedPt80 = 999.;
    double _dPhiBiasedPt100 = 999.;
    double _deltaPhiJet12 = 999.;
    double _deltaPhiJet13 = 999.;
    double _deltaPhiJet23 = 999.;
    double _dRJet12 = 999.;
    double _dRJet13 = 999.;
    double _dRJet23 = 999.;
    double _deltaPhiJet1MHT = 999.;
    double _deltaPhiJet2MHT = 999.;
    double _deltaPhiJet3MHT = 999.;

    int jetmult = 0;
    math::PtEtaPhiMLorentzVector mhtvec(0.0, 0.0, 0.0, 0.0);
    std::vector<LorentzV> goodJets;

    //loop over all jets - use only those fulfilling the cuts defined in cfg file
    for (edm::View<reco::Candidate>::const_iterator jet = jet_hnd->begin(); jet != jet_hnd->end(); ++jet) {
        const pat::Jet * patjet = static_cast<const pat::Jet*>( &(*jet) );
        if (!isLoose(patjet)) continue;       //jet ID

        SEt += patjet->correctedJet(pat::JetCorrFactors::Raw).pt();

        if (jet->pt() < jetptmin_) continue;
        if (fabs(jet->eta()) <= jetetamaxHT_) ht += jet->pt();
        if (fabs(jet->eta()) > jetetamaxMHT_) continue;

        goodJets.push_back(jet->p4());
        mhtvec -= jet->p4();

        ++jetmult;
    }

    math::PtEtaPhiMLorentzVector bias_mhtvec(0.0, 0.0, 0.0, 0.0);
    for (unsigned i = 0; i < goodJets.size(); i++) {

        bias_mhtvec = mhtvec + goodJets[i];

        if(i < 4 ) {
            if ( fabs(deltaPhi(goodJets[i], mhtvec)) < _dPhiMin4 )
                _dPhiMin4 = fabs(deltaPhi(goodJets[i], mhtvec));
            if ( fabs(deltaPhi(goodJets[i], mhtvec)) < _dPhiDual4 )
                _dPhiDual4 = fabs(deltaPhi(goodJets[i], mhtvec));
            if ( TMath::Pi() - fabs(deltaPhi(goodJets[i], mhtvec)) < _dPhiDual4 )
                _dPhiDual4 = TMath::Pi() - fabs(deltaPhi(goodJets[i], mhtvec));
            if ( fabs(deltaPhi(goodJets[i], bias_mhtvec)) < _dPhiBiased4 )
                _dPhiBiased4 = fabs(deltaPhi(goodJets[i], bias_mhtvec));
        }

        if(i < 3 ) {
            if ( fabs(deltaPhi(goodJets[i], mhtvec)) < _dPhiMin3 )
                _dPhiMin3 = fabs(deltaPhi(goodJets[i], mhtvec));
            if ( fabs(deltaPhi(goodJets[i], mhtvec)) < _dPhiDual3 )
                _dPhiDual3 = fabs(deltaPhi(goodJets[i], mhtvec));
            if ( TMath::Pi() - fabs(deltaPhi(goodJets[i], mhtvec)) < _dPhiDual3 )
                _dPhiDual3 = TMath::Pi() - fabs(deltaPhi(goodJets[i], mhtvec));
            if ( fabs(deltaPhi(goodJets[i], bias_mhtvec)) < _dPhiBiased3 )
                _dPhiBiased3 = fabs(deltaPhi(goodJets[i], bias_mhtvec));
        }

        if(i < 2 ) {
            if ( fabs(deltaPhi(goodJets[i], mhtvec)) < _dPhiMin2 )
                _dPhiMin2 = fabs(deltaPhi(goodJets[i], mhtvec));
            if ( fabs(deltaPhi(goodJets[i], mhtvec)) < _dPhiDual2 )
                _dPhiDual2 = fabs(deltaPhi(goodJets[i], mhtvec));
            if ( TMath::Pi() - fabs(deltaPhi(goodJets[i], mhtvec)) < _dPhiDual2 )
                _dPhiDual2 = TMath::Pi() - fabs(deltaPhi(goodJets[i], mhtvec));
            if ( fabs(deltaPhi(goodJets[i], bias_mhtvec)) < _dPhiBiased2 )
                _dPhiBiased2 = fabs(deltaPhi(goodJets[i], bias_mhtvec));
        }

        if( goodJets[i].Pt() > 60 ) {
            if ( fabs(deltaPhi(goodJets[i], mhtvec)) < _dPhiMinPt60 )
                _dPhiMinPt60 = fabs(deltaPhi(goodJets[i], mhtvec));
            if ( fabs(deltaPhi(goodJets[i], mhtvec)) < _dPhiDualPt60 )
                _dPhiDualPt60 = fabs(deltaPhi(goodJets[i], mhtvec));
            if ( TMath::Pi() - fabs(deltaPhi(goodJets[i], mhtvec)) < _dPhiDualPt60 )
                _dPhiDualPt60 = TMath::Pi() - fabs(deltaPhi(goodJets[i], mhtvec));
            if ( fabs(deltaPhi(goodJets[i], bias_mhtvec)) < _dPhiBiasedPt60 )
                _dPhiBiasedPt60 = fabs(deltaPhi(goodJets[i], bias_mhtvec));
        }

        if( goodJets[i].Pt() > 80 ) {
            if ( fabs(deltaPhi(goodJets[i], mhtvec)) < _dPhiMinPt80 )
                _dPhiMinPt80 = fabs(deltaPhi(goodJets[i], mhtvec));
            if ( fabs(deltaPhi(goodJets[i], mhtvec)) < _dPhiDualPt80 )
                _dPhiDualPt80 = fabs(deltaPhi(goodJets[i], mhtvec));
            if ( TMath::Pi() - fabs(deltaPhi(goodJets[i], mhtvec)) < _dPhiDualPt80 )
                _dPhiDualPt80 = TMath::Pi() - fabs(deltaPhi(goodJets[i], mhtvec));
            if ( fabs(deltaPhi(goodJets[i], bias_mhtvec)) < _dPhiBiasedPt80 )
                _dPhiBiasedPt80 = fabs(deltaPhi(goodJets[i], bias_mhtvec));
        }

        if( goodJets[i].Pt() > 100 ) {
            if ( fabs(deltaPhi(goodJets[i], mhtvec)) < _dPhiMinPt100 )
                _dPhiMinPt100 = fabs(deltaPhi(goodJets[i], mhtvec));
            if ( fabs(deltaPhi(goodJets[i], mhtvec)) < _dPhiDualPt100 )
                _dPhiDualPt100 = fabs(deltaPhi(goodJets[i], mhtvec));
            if ( TMath::Pi() - fabs(deltaPhi(goodJets[i], mhtvec)) < _dPhiDualPt100 )
                _dPhiDualPt100 = TMath::Pi() - fabs(deltaPhi(goodJets[i], mhtvec));
            if ( fabs(deltaPhi(goodJets[i], bias_mhtvec)) < _dPhiBiasedPt100 )
                _dPhiBiasedPt100 = fabs(deltaPhi(goodJets[i], bias_mhtvec));
        }

    }

    if( goodJets.size() >= 1 ) {
        _deltaPhiJet1MHT = deltaPhi(goodJets[0], mhtvec);
    }
    if( goodJets.size() >= 2 ) {
        _deltaPhiJet2MHT = deltaPhi(goodJets[1], mhtvec);
        _deltaPhiJet12 = deltaPhi(goodJets[0].Phi(), goodJets[1].Phi());
        _dRJet12 = deltaR(goodJets[0], goodJets[1]);
    }
    if( goodJets.size() >= 3 ) {
        _deltaPhiJet3MHT = deltaPhi(goodJets[2], mhtvec);
        _deltaPhiJet13 = deltaPhi(goodJets[0].Phi(), goodJets[2].Phi());
        _dRJet13 = deltaR(goodJets[0], goodJets[2]);
        _deltaPhiJet23 = deltaPhi(goodJets[1].Phi(), goodJets[2].Phi());
        _dRJet23 = deltaR(goodJets[1], goodJets[2]);
    }

    resetVariables();
    setVariable("HT",  ht);
    setVariable("MHT", mhtvec.pt());
    if (ht) setVariable("MHTsig",mhtvec.pt() / sqrt(ht));
    setVariable("JetMult",jetmult);
    setVariable("alphaT", alphaT(goodJets));
    setVariable("dPhiMin", _dPhiMin3);
    setVariable("dPhiBiased", _dPhiBiased3);
    setVariable("SET", SEt);
    setVariable("MET", met_hnd->front().et());
    if (SEt) setVariable("METsig",met_hnd->front().et() / sqrt(SEt));

    setVariable("MHTphi", mhtvec.phi());
    setVariable("METphi", met_hnd->front().phi());

    setVariable("dPhiMin2", _dPhiMin2 );
    setVariable("dPhiMin3", _dPhiMin3 );
    setVariable("dPhiMin4", _dPhiMin4 );
    setVariable("dPhiMinPt60", _dPhiMinPt60 );
    setVariable("dPhiMinPt80", _dPhiMinPt80 );
    setVariable("dPhiMinPt100", _dPhiMinPt100 );

    setVariable("dPhiDual2", _dPhiDual2 );
    setVariable("dPhiDual3", _dPhiDual3 );
    setVariable("dPhiDual4", _dPhiDual4 );
    setVariable("dPhiDualPt60", _dPhiDualPt60 );
    setVariable("dPhiDualPt80", _dPhiDualPt80 );
    setVariable("dPhiDualPt100", _dPhiDualPt100 );

    setVariable("dPhiBiased2", _dPhiBiased2 );
    setVariable("dPhiBiased3", _dPhiBiased3 );
    setVariable("dPhiBiased4", _dPhiBiased4 );
    setVariable("dPhiBiasedPt60", _dPhiBiasedPt60 );
    setVariable("dPhiBiasedPt80", _dPhiBiasedPt80 );
    setVariable("dPhiBiasedPt100", _dPhiBiasedPt100 );

    setVariable("deltaPhiJet12", _deltaPhiJet12 );
    setVariable("deltaPhiJet13", _deltaPhiJet13 );
    setVariable("deltaPhiJet23", _deltaPhiJet23 );
    setVariable("dRJet12", _dRJet12 );
    setVariable("dRJet13", _dRJet13 );
    setVariable("dRJet23", _dRJet23 );
    setVariable("deltaPhiJet1MHT", _deltaPhiJet1MHT );
    setVariable("deltaPhiJet2MHT", _deltaPhiJet2MHT );
    setVariable("deltaPhiJet3MHT", _deltaPhiJet3MHT );

    return true;
}
//__________________________________________________________________________________________________
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "SusyAnalysis/EventSelector/interface/EventSelectorFactory.h"
DEFINE_EDM_PLUGIN(EventSelectorFactory, AddOtherVariables, "AddOtherVariables");

