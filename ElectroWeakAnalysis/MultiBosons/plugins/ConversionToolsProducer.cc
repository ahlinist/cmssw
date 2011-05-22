// -*- C++ -*-
//
// Package:    ElectroWeakAnalysis/MultiBosons
// Class:      ConversionToolsProducer
//
/**\class ConversionToolsProducer ConversionToolsProducer.cc ElectroWeakAnalysis/MultiBosons/plugins/ConversionToolsProducer.cc

Description: Producer for some basic use cases of matching conversions
  with photons and electrons:
  a)  matching an electron to a conversion by track refs (conversion
  rejection for electron id, using the reconstructed conversion collection
  rather than the partner-track method)
  b)  matching a photon/supercluster to a conversion geometrically (eg for
  primary vertex determination from conversion pointing, or use of
  conversion in photon id)
  c)  matching a GsfElectron to a photon/supercluster by supercluster ref,
  with cuts on expectedinnerhits and an explicit conversion veto
  (conversion-safe electron veto for photon id)

  Also produces Delta R to electron track used in photon CiC
  This cut is intended to reject electrons and is applied to photon
  candidates for which the
  electron reconstruction algorithm successfully associates the ECAL
  super-cluster to a track
  which has no missing hits and has transverse energy greater than 2.5
  GeV.  A cut is
  applied on the angular separation Delta R between the direction of
  the track at the
  vertex and the photon direction measured from the same vertex.



Implementation: Wrapper around RecoEgamma/EgammaTools/interface/ConversionTools.h
  see https://hypernews.cern.ch/HyperNews/CMS/get/egamma/999.html
  and https://hypernews.cern.ch/HyperNews/CMS/get/higgs2g/233.html and it's thread

*/


// system include files
#include <memory>
#include <string>
#include <vector>
#include <math.h>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/Common/interface/OwnVector.h"

#include "DataFormats/EgammaCandidates/interface/PhotonFwd.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"

#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"

#include "ElectroWeakAnalysis/MultiBosons/interface/PluginTools.h"

namespace vgamma {
  namespace plugins {

    class ConversionToolsProducer : public edm::EDProducer {
    public:
      explicit ConversionToolsProducer(const edm::ParameterSet&);
      ~ConversionToolsProducer();
  
    private:
      virtual void produce(edm::Event&, const edm::EventSetup&);
  
      // ----------member data ---------------------------
      edm::InputTag photonSrc_;
      edm::InputTag electronSrc_;
      edm::InputTag conversionSrc_;
      edm::InputTag beamSpotSrc_;

      float
      DeltaRToTrack( const reco::Photon &,
                     const edm::Handle<reco::GsfElectronCollection> & );
  
    };
  } // end of namespace plugins
} // end of namespace vgamma

using vgamma::plugins::ConversionToolsProducer;

ConversionToolsProducer::ConversionToolsProducer ( 
  const edm::ParameterSet& iConfig
  ):
  photonSrc_    ( iConfig.getParameter<edm::InputTag>( "photonSrc"     ) ),
  electronSrc_  ( iConfig.getParameter<edm::InputTag>( "electronSrc"   ) ),
  conversionSrc_( iConfig.getParameter<edm::InputTag>( "conversionSrc" ) ),
  beamSpotSrc_  ( iConfig.getParameter<edm::InputTag>( "beamSpotSrc"   ) )
{
  produces<edm::ValueMap<int> > ( "passConversionVeto"   );
  produces<edm::ValueMap<int> > ( "passElectronVeto"     );
  produces<edm::ValueMap<int> > ( "hasMatchedConversion" );
  // FIXME: find a different container for ConversionRef's - try AssociationMap
  // the RefVector crashes when trying to add a Ref that is not valid.
  // This happens when no match is found.
//   produces<reco::ConversionRefVector> ( "matchedConversions" );
  produces<edm::ValueMap<float> > ( "deltaRToTrack" );
}

ConversionToolsProducer::~ConversionToolsProducer() {}

// ------------ method called to for each event  ------------
void
ConversionToolsProducer::produce (
  edm::Event& iEvent, const edm::EventSetup& iSetup
  )
{
  using namespace std;
  using namespace edm;

  Handle<reco::PhotonCollection     > photons    ;
  Handle<reco::GsfElectronCollection> electrons  ;
  Handle<reco::ConversionCollection > conversions;
  Handle<reco::BeamSpot             > beamSpot   ;

  iEvent.getByLabel( photonSrc_    , photons     );
  iEvent.getByLabel( electronSrc_  , electrons   );
  iEvent.getByLabel( conversionSrc_, conversions );
  iEvent.getByLabel( beamSpotSrc_  , beamSpot    );

  vector<int> passConversionVeto;    // for electron id
  vector<int> passElectronVeto;      // for photon id
  vector<int> hasMatchedConversion;  // for photon id
//   auto_ptr<reco::ConversionRefVector>
//     matchedConversions( new reco::ConversionRefVector); // for photon id
  vector<float> deltaRToTrack;       // for photon CiC id

  // loop over source electrons
  for( reco::GsfElectronCollection::const_iterator electron = electrons->begin();
       electron != electrons->end(); ++electron ) {

    passConversionVeto.push_back(
      !ConversionTools::hasMatchedConversion( *electron,
                                               conversions,
                                               beamSpot->position() )
      );
  } // end loop over source electrons

  // loop over source photons
  for( reco::PhotonCollection::const_iterator pho = photons->begin();
       pho != photons->end(); ++pho ) {

    passElectronVeto.push_back(
      !ConversionTools::hasMatchedPromptElectron( pho->superCluster(),
                                                  electrons,
                                                  conversions,
                                                  beamSpot->position() )
      );

    hasMatchedConversion.push_back(
      ConversionTools::hasMatchedConversion( *pho->superCluster(),
                                             conversions,
                                             beamSpot->position() )
      );

//     matchedConversions->push_back(
//       ConversionTools::matchedConversion( *pho->superCluster(),
//                                           conversions,
//                                           beamSpot->position() )
//       );

    deltaRToTrack.push_back( DeltaRToTrack( *pho, electrons ) );

    /// To enable the debug messages, recompile with the EDM_ML_DEBUG symbol
    ///   defined (in $CMSSW_BASE/src/ElectroWeakAnalysis/MultiBosons):
    ///   touch plugins/ConversionToolsProducer.cc
    ///   scram b USER_CXXFLAGS="-g\ -D=EDM_ML_DEBUG" ElectroWeakAnalysis/MultiBosons
    size_t i = passElectronVeto.size() - 1;
    LogDebug("test") << "Photon index: " << i
                     << "\npassElectronVeto: " << passElectronVeto[i]
                     << "\nhasMatchedConversion: " << hasMatchedConversion[i]
                     << "\ndeltaRToTrack: " << deltaRToTrack[i];

  } // end loop over source photons

  using vgamma::putValueMap;

  putValueMap<reco::GsfElectronCollection, int>( iEvent,
                                                 electrons,
                                                 passConversionVeto,
                                                 "passConversionVeto" );

  putValueMap<reco::PhotonCollection, int>( iEvent,
                                            photons,
                                            passElectronVeto,
                                            "passElectronVeto" );

  putValueMap<reco::PhotonCollection, int>( iEvent,
                                            photons,
                                            hasMatchedConversion,
                                            "hasMatchedConversion" );

//   iEvent.put( matchedConversions, "matchedConversions" );

  putValueMap<reco::PhotonCollection, float>( iEvent,
                                              photons,
                                              deltaRToTrack,
                                              "deltaRToTrack" );


} // ConversionToolsProducer::produce

float
ConversionToolsProducer::DeltaRToTrack( 
  const reco::Photon & pho,
  const edm::Handle<reco::GsfElectronCollection> & electrons
  )
{
  // see Float_t DeltaRToTrack(...)
  // in UserCode/HiggsAnalysis/HiggsTo2photons/h2gglobe/GeneralFunctions_{cc,h}.h

  reco::GsfElectronCollection::const_iterator ele = electrons->begin();

  // loop over electrons
  for( ; ele != electrons->end(); ++ele ) {
    if ( ele->gsfTrack()->trackerExpectedHitsInner().numberOfHits() > 0 )
      continue;
    if ( pho.superCluster() == ele->superCluster() ) break;
  } // end loop over electrons

  // default value if no electron matches
  float deltaR = 99.; 

  // Check if found matching electron
  if ( ele < electrons->end() ) {
    float dEtaIn = ele->deltaEtaSuperClusterTrackAtVtx();
    float dPhiIn = ele->deltaPhiSuperClusterTrackAtVtx();
    deltaR = sqrt( dEtaIn * dEtaIn + dPhiIn * dPhiIn );
  }

  return deltaR;
}


DEFINE_FWK_MODULE( ConversionToolsProducer );
