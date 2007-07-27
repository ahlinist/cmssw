/** \class LepSUSYSkim
 *
 * di-lepton final states SUSY Skim
 * Trigger to be implemented
 * >= n muon, pt>3 GeV
 * >= n electron, pt>5 GeV
 * >= n jets, Pt_jet1>80 GeV, Pt_jet2>30 GeV
 * MET>50 GeV
 * Cuts values in the cfi files in the data directory 
 *
 * $Date: 2007/07/23$
 * $Revision: 1.0 $
 *
 * \author Massimiliano Chioboli, Universita' and INFN, Catania
 *         Maria Spiropulu - CERN
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

#include "SUSYBSMAnalysis/CSA07Skims/interface/LepSUSYSkim.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"

using namespace edm;
using namespace std;
using namespace reco;

class PtSorter {
public:
  template <class T> bool operator() ( const T& a, const T& b ) {
    return ( a.pt() > b.pt() );
  }
};

LepSUSYSkim::LepSUSYSkim( const edm::ParameterSet& iConfig ) :
  nEvents_(0), nAccepted_(0)
{
  Muonsrc_       = iConfig.getParameter<InputTag>( "Muonsrc" );
  Elecsrc_       = iConfig.getParameter<InputTag>( "Elecsrc" );
  CaloJetsrc_    = iConfig.getParameter<InputTag>( "CaloJetsrc" );
  CaloMETsrc_    = iConfig.getParameter<InputTag>( "CaloMETsrc" );
  NminMuon_      = iConfig.getUntrackedParameter<int>( "NminMuon", 1 );
  MuonPtmin_     = iConfig.getUntrackedParameter<double>( "MuonPtmin", 3. );
  NminElec_      = iConfig.getUntrackedParameter<int>( "NminElec", 1 );
  ElecPtmin_     = iConfig.getUntrackedParameter<double>( "ElecPtmin", 5. );
  NminCaloJet_   = iConfig.getUntrackedParameter<int>( "NminCaloJet", 1 );
  CaloJet1Ptmin_ = iConfig.getUntrackedParameter<double>( "CaloJet1Ptmin", 80.);
  CaloJet2Ptmin_ = iConfig.getUntrackedParameter<double>( "CaloJet2Ptmin", 30. );
  CaloMetmin_    = iConfig.getUntrackedParameter<double>( "CaloMetmin"   , 50. );
  
}

/*------------------------------------------------------------------------*/

LepSUSYSkim::~LepSUSYSkim() 
{}

/*------------------------------------------------------------------------*/

bool LepSUSYSkim::filter( edm::Event& iEvent, 
				const edm::EventSetup& iSetup )
{
  nEvents_++;

  
  // Get the muon collection
  Handle<MuonCollection> MuonHandle;
  try {
    iEvent.getByLabel( Muonsrc_, MuonHandle );
  }
  catch ( cms::Exception& ex ) {
    edm::LogError( "LepSUSYSkim" ) 
      << "Unable to get Muon collection "
      << Muonsrc_.label();
    return false;
  }
  MuonCollection TheMuons = *MuonHandle;
  std::stable_sort( TheMuons.begin(), TheMuons.end(), PtSorter() );

  // Get the electron collection
  Handle<PixelMatchGsfElectronCollection> ElecHandle;
  try {
    iEvent.getByLabel( Elecsrc_, ElecHandle );
  }
  catch ( cms::Exception& ex ) {
    edm::LogError( "LepSUSYSFOSElecSkim" ) 
      << "Unable to get Elec collection "
      << Elecsrc_.label();
    return false;
  }
  PixelMatchGsfElectronCollection TheElecs = *ElecHandle;
  std::stable_sort( TheElecs.begin(), TheElecs.end(), PtSorter() );


  // Get the caloJet collection
  Handle<CaloJetCollection> CaloJetsHandle;
  try {
    iEvent.getByLabel( CaloJetsrc_, CaloJetsHandle );
  } 
  catch ( cms::Exception& ex ) {
    edm::LogError( "LepSUSYSkim" ) 
      << "Unable to get CaloJet collection "
      << CaloJetsrc_.label();
    return false;
  }
  CaloJetCollection TheCaloJets = *CaloJetsHandle;
  std::stable_sort( TheCaloJets.begin(), TheCaloJets.end(), PtSorter() );

  // Get the caloMetCollection
  Handle<CaloMETCollection> METHandle;
  try {
    iEvent.getByLabel( CaloMETsrc_, METHandle );
  }
  catch ( cms::Exception& ex ) {
    edm::LogError( "HadSUSYQCDSkim" )
      << "Unable to get CaloMET collection "
      << CaloMETsrc_.label();
    return false;
  }
  if ( METHandle->empty() ) return false;


  cout << "****************************************************" << endl;

  // Apply cuts on muons
  int nMuon = 0;
  for ( MuonCollection::const_iterator it = TheMuons.begin();
	it != TheMuons.end(); it++ ) {
    cout << "muon pt = " << it->pt() << endl;
    cout << "muon eta = " << it->eta() << endl;
    if ( (it->pt() > MuonPtmin_) && 
	 (fabs(it->eta()) < 3.0) ) {
      nMuon++;
    }
  }
  cout << "nMuon = " << nMuon << endl;
  if ( nMuon < NminMuon_ ) return false;
   
  // Apply cuts on electrons
  int nElec = 0;
  for ( PixelMatchGsfElectronCollection::const_iterator it = TheElecs.begin();
	it != TheElecs.end(); it++ ) {
    cout << "elec pt = " << it->pt() << endl;
    cout << "elec eta = " << it->eta() << endl;
    if ( (it->pt() > ElecPtmin_) && 
	 (fabs(it->eta()) < 3.0) ) {
      nElec++;
    }
  }
  cout << "nElec = " << nElec << endl;
  if ( nElec < NminElec_ ) return false;

  cout << "-----------------------------------------------------" << endl;
  // Apply cuts on Jets
  int nJet = 0;
  for ( CaloJetCollection::const_iterator it = TheCaloJets.begin(); 
	it != TheCaloJets.end(); it++ ) {
    cout << "jet pt = " << it->pt() << endl;
    cout << "jet eta = " << it->eta() << endl;
    if (fabs(it->eta()) < 3.0) nJet++;
  }
  cout << "nJet = " << nJet << endl;
  if ( nJet < NminCaloJet_ ) return false;
  cout << "TheCaloJets[0].pt() = " << TheCaloJets[0].pt() << endl;
  cout << "TheCaloJets[1].pt() = " << TheCaloJets[1].pt() << endl;
  if(NminCaloJet_ > 0)  {if ( TheCaloJets[0].pt() < CaloJet1Ptmin_ ) return false;}
  if(NminCaloJet_ > 1)  {if ( TheCaloJets[1].pt() < CaloJet2Ptmin_ ) return false;}

  
  cout << "-----------------------------------------------------" << endl;
  

  //apply cuts on MET
  double MetValue = METHandle->begin()->pt();
  cout << " met = " << MetValue << endl;
  if(MetValue < CaloMetmin_ ) return false;
 
  nAccepted_++;

  return true;
}

/*------------------------------------------------------------------------*/

void LepSUSYSkim::endJob()
{
  edm::LogVerbatim( "LepSUSYSkim" ) 
    << "Events read " << nEvents_
    << "\nEvents accepted " << nAccepted_
    << "\nEfficiency " << (double)(nAccepted_)/(double)(nEvents_) 
    << endl;
}
