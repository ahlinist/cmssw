//

/**
  \brief    StringCutObjectSelector-based Plugins for Various types of I/O Collections
  \author   Jan Veverka, veverka@caltech.edu
  \date     21 May 2011
*/

#include "FWCore/Framework/interface/MakerMacros.h"

#include "ElectroWeakAnalysis/MultiBosons/interface/StringCutObjectSelectors.h"

#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"

#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/EgammaCandidates/interface/PhotonFwd.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Photon.h"

/// Simply uncomment a line of interest and recompile to get the
///   corresponding plugin

//-------------------------------------------------------------------------
/// Define generic selectors
// DEFINE_LAZY_VIEW_PTR_SELECTOR( reco::Candidate, LazyCandViewPtrSelector );
// DEFINE_LAZY_VIEW_REF_SELECTOR( reco::Candidate, LazyCandViewRefSelector );

//-----------------------------------------------------------------------------
/// Define electron selectors
// DEFINE_VIEW_PTR_SELECTOR( reco::GsfElectron, GsfElectronViewPtrSelector );
// DEFINE_VIEW_REF_SELECTOR( reco::GsfElectron, GsfElectronViewRefSelector );
// DEFINE_VIEW_SELECTOR    ( reco::GsfElectron, GsfElectronViewSelector    );
//
// DEFINE_LAZY_VIEW_PTR_SELECTOR( reco::GsfElectron, LazyGsfElectronViewPtrSelector );
// DEFINE_LAZY_VIEW_REF_SELECTOR( reco::GsfElectron, LazyGsfElectronViewRefSelector );
// DEFINE_LAZY_VIEW_SELECTOR    ( reco::GsfElectron, LazyGsfElectronViewSelector    );
//
// DEFINE_VIEW_PTR_SELECTOR( pat::Electron, PatElectronViewPtrSelector );
// DEFINE_VIEW_REF_SELECTOR( pat::Electron, PatElectronViewRefSelector );
// DEFINE_VIEW_SELECTOR    ( pat::Electron, PatElectronViewSelector    );
//
// DEFINE_LAZY_VIEW_PTR_SELECTOR( pat::Electron, LazyPatElectronViewPtrSelector );
// DEFINE_LAZY_VIEW_REF_SELECTOR( pat::Electron, LazyPatElectronViewRefSelector );
// DEFINE_LAZY_VIEW_SELECTOR    ( pat::Electron, LazyPatElectronViewSelector    );

//-----------------------------------------------------------------------------
/// Define muon selectors
// DEFINE_VIEW_PTR_SELECTOR( reco::Muon, MuonViewPtrSelector );
DEFINE_VIEW_REF_SELECTOR( reco::Muon, MuonViewRefSelector );
// DEFINE_VIEW_SELECTOR    ( reco::Muon, MuonViewSelector    );
//
// DEFINE_LAZY_VIEW_PTR_SELECTOR( reco::Muon, LazyMuonViewPtrSelector );
DEFINE_LAZY_VIEW_REF_SELECTOR( reco::Muon, LazyMuonViewRefSelector );
// DEFINE_LAZY_VIEW_SELECTOR    ( reco::Muon, LazyMuonViewSelector    );
//
// DEFINE_VIEW_PTR_SELECTOR( pat::Muon, PatMuonViewPtrSelector );
// DEFINE_VIEW_REF_SELECTOR( pat::Muon, PatMuonViewRefSelector );
// DEFINE_VIEW_SELECTOR    ( pat::Muon, PatMuonViewSelector    );
//
// DEFINE_LAZY_VIEW_PTR_SELECTOR( pat::Muon, LazyPatMuonViewPtrSelector );
// DEFINE_LAZY_VIEW_REF_SELECTOR( pat::Muon, LazyPatMuonViewRefSelector );
// DEFINE_LAZY_VIEW_SELECTOR    ( pat::Muon, LazyPatMuonViewSelector    );

//-----------------------------------------------------------------------------
/// Define photon selectors
// DEFINE_VIEW_PTR_SELECTOR( reco::Photon, PhotonViewPtrSelector );
DEFINE_VIEW_REF_SELECTOR( reco::Photon, PhotonViewRefSelector );
// DEFINE_VIEW_SELECTOR    ( reco::Photon, PhotonViewSelector    );

// DEFINE_LAZY_VIEW_PTR_SELECTOR( reco::Photon, LazyPhotonViewPtrSelector );
DEFINE_LAZY_VIEW_REF_SELECTOR( reco::Photon, LazyPhotonViewRefSelector );
// DEFINE_LAZY_VIEW_SELECTOR    ( reco::Photon, LazyPhotonViewSelector    );
//
// DEFINE_VIEW_PTR_SELECTOR( pat::Photon, PatPhotonViewPtrSelector );
// DEFINE_VIEW_REF_SELECTOR( pat::Photon, PatPhotonViewRefSelector );
// DEFINE_VIEW_SELECTOR    ( pat::Photon, PatPhotonViewSelector    );
//
// DEFINE_LAZY_VIEW_PTR_SELECTOR( pat::Photon, LazyPatPhotonViewPtrSelector );
// DEFINE_LAZY_VIEW_REF_SELECTOR( pat::Photon, LazyPatPhotonViewRefSelector );
// DEFINE_LAZY_VIEW_SELECTOR    ( pat::Photon, LazyPatPhotonViewSelector    );

