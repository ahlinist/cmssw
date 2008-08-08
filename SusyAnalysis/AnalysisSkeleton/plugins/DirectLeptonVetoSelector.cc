#include "SusyAnalysis/AnalysisSkeleton/interface/DirectLeptonVetoSelector.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "TMath.h"

#include <vector>

//__________________________________________________________________________________________________
DirectLeptonVetoSelector::DirectLeptonVetoSelector(const edm::ParameterSet& pset) :
  SusyEventSelector(pset),
  electronTag_( pset.getParameter<edm::InputTag>("electronTag")),
  muonTag_    ( pset.getParameter<edm::InputTag>("muonTag")    ),
  minEtEle_   ( pset.getParameter<double>("minElectronEt")     ),
  eleIso_     ( pset.getParameter<double>("electronIsolation") ),
  minEtMuon_  ( pset.getParameter<double>("minMuonEt")         ),
  muonIso_    ( pset.getParameter<double>("muonIsolation")     ) //,
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
bool DirectLeptonVetoSelector::select (const edm::Event& event) const {

  // Input collections
  edm::Handle< std::vector<pat::Electron> > eleHandle;
  event.getByLabel(electronTag_, eleHandle);
  edm::Handle< std::vector<pat::Muon> > muonHandle;
  event.getByLabel(muonTag_, muonHandle);
//  edm::Handle< std::vector<pat::Tau> > tauHandle;
//  event.getByLabel(tauTag_, tauHandle);
  
  resetVariables();

  int nElectrons=0, nMuons=0, nTaus=0; // Counters

  //
  // electrons
  //
  if ( eleHandle.isValid() )
    for ( std::vector<pat::Electron>::const_iterator ie=(*eleHandle).begin();
	  ie!=(*eleHandle).end(); ++ie ) {
      // Only electrons above Et cut
      if ( ie->et() > minEtEle_ 
           && (ie->trackIso()+ie->et())/ie->et() < eleIso_ ) {
        LogDebug("DirectLeptonVetoSelector") << "Isolated electron found";
        ++nElectrons;
      }
    }
  setVariable("nElectrons",nElectrons);

  //
  // muons
  //
  if ( muonHandle.isValid() )
    for ( std::vector<pat::Muon>::const_iterator im=(*muonHandle).begin();
	  im!=(*muonHandle).end(); ++im ) {
      // Only muons above Et cut
      if ( im->et() > minEtMuon_ 
           && (im->trackIso()+im->et())/im->et() < muonIso_ ) {
        LogDebug("DirectLeptonVetoSelector") << "Isolated muon found";
        ++nMuons;
      }
    }
  setVariable("nMuons",nMuons);
  
//  //
//  // taus
//  //
//  if ( tauHandle.isValid() )
//    for ( std::vector<pat::Tau>::const_iterator it=(*tauHandle).begin();
//	  it!=(*tauHandle).end(); ++it ) {
//      // Only taus above Et cut
//      if ( it->et() > minEtTau_ 
//           && (it->trackIso()+it->et())/it->et() < tauIso_ ) {
//        LogDebug("DirectLeptonVetoSelector") << "Isolated tau found";
//        ++nTaus;
//      }
//    }
//  setVariable("nTaus",nTaus);

  // Selection
  if ( nElectrons>0 || nMuons>0 || nTaus>0 ) return false;

  //
  // all electrons, muons and taus are below Et cut or not isolated: event passes
  //
  return true;

}

//__________________________________________________________________________________________________
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "SusyAnalysis/EventSelector/interface/EventSelectorFactory.h"
DEFINE_EDM_PLUGIN(EventSelectorFactory, DirectLeptonVetoSelector, "DirectLeptonVetoSelector");
