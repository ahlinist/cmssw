#include "SusyAnalysis/EventSelector/interface/LeptonVetoSelector.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "TMath.h"

#include <vector>

//__________________________________________________________________________________________________
LeptonVetoSelector::LeptonVetoSelector(const edm::ParameterSet& pset) :
   SusyEventSelector(pset), electronTag_(pset.getParameter<edm::InputTag> ("electronTag")), muonTag_(pset.getParameter<
            edm::InputTag> ("muonTag")), beamSpot_(pset.getParameter<edm::InputTag> ("beamSpot")), minPtEle_(
            pset.getParameter<double> ("minElectronPt")), maxEtaEle_(pset.getParameter<double> ("maxElectronEta")),
            eleIso_(pset.getParameter<double> ("electronIsolation")), eleDxy_(pset.getParameter<double> (
                     "maxElectronDxy")), minPtMuon_(pset.getParameter<double> ("minMuonPt")), maxEtaMuon_(
                     pset.getParameter<double> ("maxMuonEta")), muonIso_(pset.getParameter<double> ("muonIsolation")),
            muonHits_(pset.getParameter<unsigned int> ("muonMinHits")), muonDxy_(pset.getParameter<double> (
                     "maxMuonDxy")), invertVeto_  ( pset.getParameter<bool>        ("invertVeto")        )

//,
//  tauTag_    ( pset.getParameter<edm::InputTag>("tau")       ),
//  minEtTau_  ( pset.getParameter<double>("minTauEt")         ),
//  tauIso_    ( pset.getParameter<double>("tauIsolation")     )
{

   // Store counter
   defineVariable("nElectrons");
   defineVariable("nMuons");
   //  defineVariable("nTaus");

}


//__________________________________________________________________________________________________
bool LeptonVetoSelector::select(const edm::Event& event) const {

  // Input collections
  edm::Handle<std::vector<pat::Electron> > eleHandle;
  event.getByLabel(electronTag_, eleHandle);
  edm::Handle<std::vector<pat::Muon> > muonHandle;
  event.getByLabel(muonTag_, muonHandle);

  edm::Handle<reco::BeamSpot> beamSpotHandle;
  event.getByLabel(beamSpot_, beamSpotHandle);
  reco::BeamSpot myBeamSpot = *beamSpotHandle;

  const math::XYZPointD & myPosition = myBeamSpot.position();

  //  edm::Handle< std::vector<pat::Tau> > tauHandle;
  //  event.getByLabel(tauTag_, tauHandle);

  resetVariables();

  int nElectrons = 0, nMuons = 0, nTaus = 0; // Counters

  //
  // electrons
  //
  if ( ! eleHandle.isValid()) {
    edm::LogWarning("LeptonVetoSelector") << "No results for InputTag " << electronTag_;
    return false;
  }

  for (std::vector<pat::Electron>::const_iterator ie = (*eleHandle).begin(); ie != (*eleHandle).end(); ++ie) {

    if (ie->electronID("eidLoose") < 1)
      continue;

    if (ie->pt() < minPtEle_)
      continue;

    if (fabs(ie->eta()) > maxEtaEle_)
      continue;

    if ((ie->trackIso() + ie->ecalIso() + ie->hcalIso()) / ie->pt() > eleIso_)
      continue;

    if (fabs(ie->gsfTrack()->dxy(myPosition)) > eleDxy_)
      continue;

    //       std::cout << "PAT electron"<<std::endl;
    //       std::cout <<"electronID? "<<ie->electronID("eidLoose") <<std::endl;
    //       std::cout << "eta: "<<      ie->eta()<<std::endl;
    //       std::cout << "pt: "<< ie->pt()<<std::endl;
    //       std::cout <<"dxy: "<<ie->gsfTrack()->dxy()<<std::endl;
    //       std::cout << "trackIso: "<<ie->trackIso()<<std::endl;
    //       std::cout << "ecalIso: "<<	ie->ecalIso()<<std::endl;
    //       std::cout << "hcalIso: "<<	ie->hcalIso()<<std::endl;
    //       std::cout << "relIso: "<<(ie->trackIso()+ie->ecalIso()+ie->hcalIso())/ie->pt()<<std::endl;
    //       std::cout << "-----------electron end"<<std::endl;


    LogDebug("LeptonVetoSelector") << "Isolated electron found";
    ++nElectrons;
  }
      
  setVariable("nElectrons", nElectrons);

  //
  // muons
  //

  if ( ! muonHandle.isValid()) {
    edm::LogWarning("LeptonVetoSelector") << "No results for InputTag " << muonTag_;
    return false;
  }

  for (std::vector<pat::Muon>::const_iterator im = (*muonHandle).begin(); im != (*muonHandle).end(); ++im) {

    //if( im->isGood(reco::Muon::GlobalMuonPromptTight) == false ) continue;
    if (!im->isGlobalMuon())
      continue;

    if (im->pt() < minPtMuon_)
      continue;

    if (fabs(im->eta()) > maxEtaMuon_)
      continue;

    if ((im->trackIso() + im->ecalIso() + im->hcalIso()) / im->pt() > muonIso_)
      continue;

    if (fabs(im->innerTrack()->dxy(myPosition)) > muonDxy_)
      continue;

    if (im->innerTrack()->numberOfValidHits() < muonHits_)
      continue;

    //       std::cout << "PAT muon"<<std::endl;
    //       std::cout << "eta: "<<      im->eta()<<std::endl;
    //       std::cout << "pt: "<< im->pt()<<std::endl;
    //       std::cout << "GlobalMuonPromptTight? " << im->isGood(reco::Muon::GlobalMuonPromptTight) <<std::endl;
    //       std::cout << "GlobalMuon? " << im->isGlobalMuon() <<std::endl;
    //       std::cout << "Chi2: "<< im->combinedMuon()->normalizedChi2()<<std::endl;
    //       std::cout <<"dxy: "<<im->innerTrack()->dxy()<<std::endl;
    //       std::cout <<"hits: "<<	 im->innerTrack()->numberOfValidHits()<<std::endl;
    //       std::cout << "trackIso: "<<im->trackIso()<<std::endl;
    //       std::cout << "ecalIso: "<<	im->ecalIso()<<std::endl;
    //       std::cout << "hcalIso: "<<	im->hcalIso()<<std::endl;
    //       std::cout << "relIso: "<<(im->trackIso()+im->ecalIso()+im->hcalIso())/im->pt()<<std::endl;
    //       std::cout << "-----------muon end"<<std::endl;


    LogDebug("LeptonVetoSelector") << "Isolated muon found";
    ++nMuons;

  }
  setVariable("nMuons", nMuons);

  //  //
  //  // taus
  //  //
  //  if ( tauHandle.isValid() )
  //    for ( std::vector<pat::Tau>::const_iterator it=(*tauHandle).begin();
  //	  it!=(*tauHandle).end(); ++it ) {
  //      // Only taus above Et cut
  //      if ( it->et() > minEtTau_
  //           && (it->trackIso()+it->et())/it->et() < tauIso_ ) {
  //        LogDebug("LeptonVetoSelector") << "Isolated tau found";
  //        ++nTaus;
  //      }
  //    }
  //  setVariable("nTaus",nTaus);

  // Selection
  if (!invertVeto_){
    if ( nElectrons>0 || nMuons>0 || nTaus>0 ) return false;

    //
    // all electrons, muons and taus are below Et cut or not isolated: event passes
    //
    return true;
  }
  else{
    if ( (nElectrons +  nMuons + nTaus) == 1 && nTaus ==0 ) return true;
  }
  return false;
}
//__________________________________________________________________________________________________
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "SusyAnalysis/EventSelector/interface/EventSelectorFactory.h"
DEFINE_EDM_PLUGIN(EventSelectorFactory, LeptonVetoSelector, "LeptonVetoSelector");
