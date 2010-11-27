#include "SusyAnalysis/EventSelector/interface/MHTJetEventSelector.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/JetCorrFactors.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "TMath.h"
#include <vector>
#include <numeric>

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
    defineVariable("dPhiJet1MHT");
    defineVariable("dPhiJet2MHT");
    defineVariable("dPhiJet3MHT");
    defineVariable("R1");
    defineVariable("R2");

    edm::LogInfo("MHTJetEventSelector") << "constructed with \n" << "  jetTag    = " << jetTag_;

}

//________________________________________________________________________________________
bool MHTJetEventSelector::select(const edm::Event& event) const {

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
  // Compute variables
  //

  math::PtEtaPhiMLorentzVector mhtvec(0.0, 0.0, 0.0, 0.0);
  std::vector<LorentzV> goodJets;

  for( edm::View<pat::Jet>::const_iterator iJet = jets->begin(); iJet != jets->end(); ++iJet ){

    if (iJet->pt() < minPt_ || fabs(iJet->eta()) > maxEta_) continue;

    bool loose = true;

    if( useJetID_ && (iJet->isCaloJet() || iJet->isJPTJet()) ){
      pat::strbitset ret = jetIDLooseCalo.getBitTemplate();
      ret.set(false);
      loose = jetIDLooseCalo( *iJet, ret );
    }
    else if ( useJetID_ && iJet->isPFJet() ){
      pat::strbitset ret = jetIDLoosePF.getBitTemplate();
      ret.set(false);
      loose = jetIDLoosePF( *iJet, ret );
    }

    if (useJetID_ && !(loose)) {
      badJet = true;
      continue;
    }

    goodJets.push_back(iJet->p4());
    mhtvec -= iJet->p4();

  }

  double _mhtDphi = 999.;
  double _dPhiJet1MHT = 999.;
  double _dPhiJet2MHT = 999.;
  double _dPhiJet3MHT = 999.;
   
  for (unsigned i = 0; i < goodJets.size(); i++) {

    if( i < nJetsMHTIso_ )
      if( fabs(reco::deltaPhi(goodJets[i], mhtvec)) < _mhtDphi) _mhtDphi = fabs(reco::deltaPhi(goodJets[i], mhtvec));
    if( i == 0 )
      _dPhiJet1MHT = fabs(reco::deltaPhi(goodJets[i], mhtvec));
    if( i == 1 )
      _dPhiJet2MHT = fabs(reco::deltaPhi(goodJets[i], mhtvec));
    if( i == 2 )
      _dPhiJet3MHT = fabs(reco::deltaPhi(goodJets[i], mhtvec));

  }

  // 
  //Set the variables
  //
  resetVariables();

  setVariable("mhtDphi", _mhtDphi);
  setVariable("dPhiJet1MHT", _dPhiJet1MHT);
  setVariable("dPhiJet2MHT", _dPhiJet2MHT);
  setVariable("dPhiJet3MHT", _dPhiJet3MHT);

  // R1 & R2
  float R1 = sqrt(TMath::Power(_dPhiJet2MHT, 2) + TMath::Power(_dPhiJet1MHT - TMath::Pi(), 2));
  float R2 = sqrt(TMath::Power(_dPhiJet1MHT, 2) + TMath::Power(_dPhiJet2MHT - TMath::Pi(), 2));
  setVariable("R1", R1);
  setVariable("R2", R2);
 
  // 
  // Perform final selection
  //
  if (_mhtDphi < mhtDphiMin_)
    return false;
  if (_dPhiJet2MHT < dPhiJet2MHTMin_)
    return false;
  if (_dPhiJet1MHT < dPhiJet1MHTMin_)
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
