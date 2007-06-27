/* class RecoAnalysisExample
 *
 * Example of analyzed accessing RECO data
 *
 * \author Alexander Nikitenko
 *         restyled and cleaned by Luca Lista
 */
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
class TFile;
class TH1D;

class RecoAnalyzerExample : public edm::EDAnalyzer {
 public:
  explicit RecoAnalyzerExample( const edm::ParameterSet & );
  ~RecoAnalyzerExample();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void beginJob(const edm::EventSetup& ) ;
  virtual void endJob() ;

 private:
  /// output histogram file name
  std::string outputFileName_;
  /// names of product labels
  edm::InputTag hepMC_, tracks_, caloTowers_, caloJets_, genJets_, 
    tauTags_, electrons_, muons_;  
  /// skip product flags
  bool skipGenJets_;
  /// output ROOT file
  TFile * outputFile_ ;
  /// output histogram
  TH1D * h_trackPt, * h_trackEta, * h_trackPhi, * h_jetEt;  
};

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/BTauReco/interface/IsolatedTauTagInfo.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"
#include <iostream>
#include "TFile.h"
#include "TH1.h"

using namespace edm;
using namespace reco;
using namespace std;

RecoAnalyzerExample::RecoAnalyzerExample( const ParameterSet & cfg ) :
  outputFileName_( cfg.getUntrackedParameter<string>("histoutputFile") ),
  hepMC_      ( cfg.getParameter<InputTag>( "hepMC" ) ),
  tracks_     ( cfg.getParameter<InputTag>( "tracks" ) ),
  caloTowers_ ( cfg.getParameter<InputTag>( "caloTowers" ) ),
  caloJets_   ( cfg.getParameter<InputTag>( "caloJets" ) ),
  genJets_    ( cfg.getParameter<InputTag>( "genJets" ) ),
  tauTags_    ( cfg.getParameter<InputTag>( "tauTags" ) ),
  electrons_  ( cfg.getParameter<InputTag>( "electrons" ) ),
  muons_      ( cfg.getParameter<InputTag>( "muons" ) ),
  skipGenJets_( cfg.getUntrackedParameter<bool>( "skipGenJets", false ) ) {
}

RecoAnalyzerExample::~RecoAnalyzerExample() {
  delete outputFile_;
}

void RecoAnalyzerExample::beginJob( const EventSetup & ) {
  outputFile_  = new TFile( outputFileName_.c_str(), "RECREATE" ) ;
  h_trackPt    = new TH1D( "trackPt", "tracks p_{t}", 100,  0., 200. );
  h_trackEta   = new TH1D( "trackEta", "tracks #eta", 50, -3, 3 );
  h_trackPhi   = new TH1D( "trackPhi", "tracks #phi", 50, -M_PI, M_PI );
  h_jetEt      = new TH1D( "jetEt", "jets E_{t}", 100,  0., 200. );
}

void RecoAnalyzerExample::endJob() {
  outputFile_->Write() ;
  outputFile_->Close() ;
}

void RecoAnalyzerExample::analyze(const Event& event, const EventSetup& eventSetup) {
  /// get the particle data table
  ESHandle<ParticleDataTable> pdt;
  eventSetup.getData( pdt );

  using namespace edm;
  cout << ">>> processing event # " << event.id() <<", time: " << event.time().value()
       << endl;
  // get MC info
  Handle<HepMCProduct> hepMC ;
  event.getByLabel( hepMC_, hepMC ) ;
  const HepMC::GenEvent* genEvent = hepMC->GetEvent() ;
  for ( HepMC::GenEvent::particle_const_iterator p = genEvent->particles_begin();
	p != genEvent->particles_end(); ++p ) {
    const HepMC::GenParticle * part = * p; 
    int id = part->pdg_id();
    const ParticleData * pd = pdt->particle( id );
    if ( pd == 0 )
      throw edm::Exception( edm::errors::InvalidReference ) 
	<< "HepMC particle with id " << id << "has no particle data" << endl;
    cout  << "GenParticle " << pd->name()
	  <<" (id: " << id << ") status : " << part->status() << endl; 
  }

  // get calo towers collection
  Handle<CaloTowerCollection> caloTowers; 
  event.getByLabel( caloTowers_, caloTowers );
  cout << "====> number of CaloTowers " << caloTowers->size() << endl;

  // get calo jet collection
  Handle<CaloJetCollection> caloJets;
  event.getByLabel( caloJets_, caloJets );
  cout << "====> number of CaloJets   " << caloJets->size() << endl;
  
  if ( ! skipGenJets_ ) {
    // get gen jet collection
    Handle<GenJetCollection> genJets;
    event.getByLabel( genJets_, genJets );
    cout << "====> number of GenJets    " << genJets->size() << endl;
    // access to MC jets. similar for reco jets, tracks, ....
    for ( size_t j = 0; j < genJets->size(); ++ j ) {
      const GenJet & jet = (*genJets)[ j ];
      cout << "->   Jet " << j << " pT: " << jet.pt() 
	   << " eta: " << jet.eta() 
	   << " phi: " << jet.phi() << endl;
      h_jetEt->Fill( jet.et() );
    }
  }
  // get track collection
  Handle<TrackCollection> tracks;
  event.getByLabel( tracks_, tracks );
  cout << "====> number of Tracks     " << tracks->size() << endl;

  // Taus
  Handle<IsolatedTauTagInfoCollection> tauTags;
  event.getByLabel( tauTags_, tauTags );
  cout << "====> number of Tau tags   " << tauTags->size() << endl;
  
  // Electrons
  Handle<PixelMatchGsfElectronCollection> electrons;
  event.getByLabel( electrons_, electrons );
  cout << "====> number of Electrons  " << electrons->size() << endl;
  
  // Muons
  Handle<MuonCollection> muons;
  event.getByLabel( muons_, muons );
  cout << "====> number of Muons      " << muons->size() << endl;
  
  
  
  // access tracks
  for (size_t j = 0; j < tracks->size(); ++ j ) {
    const Track & track = (*tracks)[ j ];
    cout << "->   Track " << j << " pT: " << track.pt() 
	 << " eta: " << track.eta() 
	 << " phi: " << track.phi() << endl;
    h_trackPt->Fill( track.pt() );
    h_trackEta->Fill( track.eta() );
    h_trackPhi->Fill( track.phi() );
  }
  
  // access taus
  for ( IsolatedTauTagInfoCollection::const_iterator tag = tauTags->begin();
	tag != tauTags->end(); ++ tag ) {
    cout <<" ---> tau jet: discr = " << tag->discriminator() << endl;
  }
  
  // access muons
  for (  MuonCollection::const_iterator muon = muons->begin(); 
	 muon != muons->end(); ++ muon ) {
    // this is the track which contains both the tracker and the muon's info
    // muon->  will give you exactly the same info as glbTrack for pt, eta, phi
    // TrackRef knows all the methods of the Track class
    //
    TrackRef glbTrack = muon->combinedMuon();
    cout <<" muon pT = " << muon->pt() 
	 <<" eta = " << muon->eta()
	 <<" phi = " << muon->phi() << endl;
    // this is the track which contains the muon's info only
    TrackRef staTrack = muon->standAloneMuon();
    // this is the track which contains the muon tracker's info only
    TrackRef trackerTrack = muon->track();
  }
}
#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE( RecoAnalyzerExample );
