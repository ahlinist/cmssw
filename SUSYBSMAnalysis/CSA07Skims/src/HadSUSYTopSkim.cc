/* \class HadSUSYTopSkim
 *
 * all hadronic SUSY Skim
 * >= 3 jets + 1 lepton (muon, elect), top candle calibration/monitoring
 * (lepton + jets trigger path)
 *
 * $Date: 2007/07/30 16:26:17 $
 * $Revision: 1.2 $
 *
 * \author Michael Tytgat, Maria Spiropulu - CERN
 *
 */

#include <iostream>
#include <string>
#include <list>
#include <cmath>
#include <cstdio>
#include <vector>
#include <memory>

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Common/interface/Handle.h"    

#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"

#include "SUSYBSMAnalysis/CSA07Skims/interface/HadSUSYTopSkim.h"

using namespace edm;
using namespace std;
using namespace reco;

HadSUSYTopSkim::HadSUSYTopSkim( const edm::ParameterSet& iConfig ) :
  nEvents_(0), nAccepted_(0)
{
  CaloJetsrc_ = iConfig.getParameter<InputTag>( "CaloJetsrc" );
  CaloJetPtmin_ = 
    iConfig.getUntrackedParameter<double>( "CaloJetPtmin", 30. );
  NminCaloJet_ = iConfig.getUntrackedParameter<int>( "NminCaloJet", 3 );

  Muonsrc_ = iConfig.getParameter<InputTag>( "Muonsrc" );
  MuonPtmin_ = 
    iConfig.getUntrackedParameter<double>( "MuonPtmin", 20. );
  NminMuon_ = iConfig.getUntrackedParameter<int>( "NminMuon", 1 );

  Elecsrc_ = iConfig.getParameter<InputTag>( "Elecsrc" );
  NminElec_ = iConfig.getUntrackedParameter<int>( "NminElec", 1 );
  ElecPtmin_ = iConfig.getUntrackedParameter<double>( "ElecPtmin", 20. );
}

/*------------------------------------------------------------------------*/

HadSUSYTopSkim::~HadSUSYTopSkim() 
{}

/*------------------------------------------------------------------------*/

bool HadSUSYTopSkim::filter( edm::Event& iEvent, 
			     const edm::EventSetup& iSetup )
{
  nEvents_++;

  // jet cuts
  Handle<CaloJetCollection> CaloJetsHandle;
//  try {
    iEvent.getByLabel( CaloJetsrc_, CaloJetsHandle );
//  } 
//  catch ( cms::Exception& ex ) {
//    edm::LogError( "HadSUSYTopSkim" ) 
//      << "Unable to get CaloJet collection "
//      << CaloJetsrc_.label();
//    return false;
//  }
  if ( CaloJetsHandle->empty() ) return false;

  int nJet = 0;
  for ( CaloJetCollection::const_iterator it = CaloJetsHandle->begin(); 
	it != CaloJetsHandle->end(); it++ ) {
    if ( it->pt() > CaloJetPtmin_ )  nJet++;
  }
  if ( nJet < NminCaloJet_ ) return false;

  // muon cuts
  Handle<MuonCollection> MuonHandle;
//  try {
    iEvent.getByLabel( Muonsrc_, MuonHandle );
//  }
//  catch ( cms::Exception& ex ) {
//    edm::LogError( "HadSUSYdiMuonSkim" )
//      << "Unable to get Muon collection "
//      << Muonsrc_.label();
//    return false;
//  }
  if ( MuonHandle->empty() ) return false;
  //  MuonCollection TheMuons = *MuonHandle;

  int nMuon = 0;
  for ( MuonCollection::const_iterator it = MuonHandle->begin();
        it != MuonHandle->end(); it++ ) {
    if ( it->pt() > MuonPtmin_ ) nMuon++;
  }
  if ( nMuon < NminMuon_ ) return false;

  // electron cuts
  Handle<PixelMatchGsfElectronCollection> ElecHandle;
//  try {
    iEvent.getByLabel( Elecsrc_, ElecHandle );
//  }
//  catch ( cms::Exception& ex ) {
//    edm::LogError( "HadSUSYdiElecSkim" )
//      << "Unable to get Elec collection "
//      << Elecsrc_.label();
//    return false;
//  }
  if ( ElecHandle->empty() ) return false;

  int nElec = 0;
  for ( PixelMatchGsfElectronCollection::const_iterator it = 
	  ElecHandle->begin();
        it != ElecHandle->end(); it++ ) {
    if ( it->pt() > ElecPtmin_ ) nElec++;
  }
  if ( nElec < NminElec_ ) return false;

  nAccepted_++;

  return true;
}

/*------------------------------------------------------------------------*/

void HadSUSYTopSkim::endJob()
{
  edm::LogVerbatim( "HadSUSYTopSkim" ) 
    << "Events read " << nEvents_
    << " Events accepted " << nAccepted_
    << "\nEfficiency " << (double)(nAccepted_)/(double)(nEvents_) 
    << endl;
}
