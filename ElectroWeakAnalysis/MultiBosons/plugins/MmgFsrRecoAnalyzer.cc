#include <cmath>
#include <iostream>
#include <map>
#include <string>
#include <stdio.h>
#include <vector>

#include "TH1.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/Candidate/interface/ShallowClonePtrCandidate.h"
#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/TrackReco/interface/Track.h"

class MmgFsrRecoAnalyzer : public edm::EDAnalyzer {

public:
  /// default constructor
  explicit MmgFsrRecoAnalyzer(const edm::ParameterSet&);
  /// default destructor
  ~MmgFsrRecoAnalyzer();
  
private:
  /// everything that needs to be done before the event loop
  virtual void beginJob() ;
  /// everything that needs to be done during the event loop
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  /// everything that needs to be done after the event loop
  virtual void endJob() ;
  
  // simple map to contain all histograms; 
  // histograms are booked in the beginJob() 
  // method
  std::map<std::string,TH1F*> histContainer_; 

  // input tags  
  edm::InputTag photonSrc_;
  edm::InputTag muonSrc_;
  edm::InputTag beamSpotSrc_;

  // cut flow bookkeeping
  std::vector<std::string> cuts_;
  std::map<std::string, int> muonsPassedPerEvent_;
  std::map<std::string, int> muonsPassedTotal_;
  std::map<std::string, int> eventsPassed_;
  
  int eventsProcessed_;

  std::vector<reco::ShallowClonePtrCandidate> selectedMuons_;

};

MmgFsrRecoAnalyzer::MmgFsrRecoAnalyzer(const edm::ParameterSet& iConfig):
  histContainer_(),
  photonSrc_(iConfig.getUntrackedParameter<edm::InputTag>("photonSrc")),
  muonSrc_(iConfig.getUntrackedParameter<edm::InputTag>("muonSrc")),
  beamSpotSrc_(iConfig.getUntrackedParameter<edm::InputTag>("beamSpotSrc" )),
  cuts_(),
  muonsPassedPerEvent_(),
  muonsPassedTotal_(),
  eventsPassed_(),
  eventsProcessed_(0)
{
}

MmgFsrRecoAnalyzer::~MmgFsrRecoAnalyzer()
{
}

void
MmgFsrRecoAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  ++eventsProcessed_;

  // define handles
  edm::Handle<edm::View<reco::Muon> >   muons;
  edm::Handle<edm::View<reco::Photon> > photons;
  edm::Handle<reco::BeamSpot>           beamSpot;

  // get the collections
  iEvent.getByLabel(muonSrc_, muons);
  iEvent.getByLabel(photonSrc_, photons);
  iEvent.getByLabel(beamSpotSrc_, beamSpot);
    
  // initialize per event counters
  std::vector<std::string>::const_iterator iCut = cuts_.begin();
  for (; iCut != cuts_.end(); ++iCut) {
      muonsPassedPerEvent_[*iCut] = 0;
  }
  selectedMuons_.clear();

  // loop over muons
  for(size_t iMuon = 0; iMuon < muons->size(); ++iMuon) {
    reco::Muon const & muon = muons->at(iMuon);

    ++muonsPassedPerEvent_["2.   Collision Data Cleaning"];

    if ( muon.isGlobalMuon() == false ) continue;
    ++muonsPassedPerEvent_["3.1  global muon"];

    if ( muon.globalTrack()->normalizedChi2() >= 10. ) continue;
    ++muonsPassedPerEvent_["3.2  chi2 / ndof"];

    if ( muon.globalTrack()->hitPattern().numberOfValidMuonHits() < 1 ) continue;
    ++muonsPassedPerEvent_["3.3  muon hits"];

    if ( muon.isTrackerMuon() == false ) continue;
    ++muonsPassedPerEvent_["3.4  tracker muon"];

    if ( muon.numberOfMatches() < 2 ) continue;
    ++muonsPassedPerEvent_["3.5  muon stations with matches"];

    if ( muon.track()->numberOfValidHits() < 11 ) continue;
    ++muonsPassedPerEvent_["3.6  tracker hits"];

    if ( muon.track()->hitPattern().numberOfValidPixelHits() < 1 )
      continue;
    ++muonsPassedPerEvent_["3.7  pixel hits"];

    if ( fabs( muon.track()->dxy( beamSpot->position() ) ) >= 0.2 ) continue;
    ++muonsPassedPerEvent_["3.8  |d_xy|"];

    if ( muon.isolationR03().sumPt >= 3. ) continue;
    ++muonsPassedPerEvent_["3.9  track isolation"];

    if ( muon.pt() <= 10. ) continue;
    ++muonsPassedPerEvent_["3.10 pt"];

    if ( fabs( muon.eta() ) >= 2.4 ) continue;
    ++muonsPassedPerEvent_["3.11 |eta|"];

    selectedMuons_.push_back(
      reco::ShallowClonePtrCandidate(
        edm::Ptr<reco::Muon>(muons, iMuon)
        )
      );

  } // end loop over muons

  // update per-event counters
  for (iCut = cuts_.begin(); iCut != cuts_.end(); ++iCut) {
    muonsPassedTotal_[*iCut] += muonsPassedPerEvent_[*iCut];
    if (muonsPassedPerEvent_[*iCut] >= 2) {
      ++eventsPassed_[*iCut];
    }
  }

  // do something similar for the other candidates
  histContainer_["muons"]->Fill(muons->size() );
}

void 
MmgFsrRecoAnalyzer::beginJob()
{
  // register to the TFileService
  edm::Service<TFileService> fs;
  
  // book histograms:
  histContainer_["muons"  ]=fs->make<TH1F>("muons",   "muon multiplicity",     10, 0,  10);

  // define the cuts_
  cuts_.push_back("2.   Collision Data Cleaning");
  cuts_.push_back("3.1  global muon");
  cuts_.push_back("3.2  chi2 / ndof");
  cuts_.push_back("3.3  muon hits");
  cuts_.push_back("3.4  tracker muon");
  cuts_.push_back("3.5  muon stations with matches");
  cuts_.push_back("3.6  tracker hits");
  cuts_.push_back("3.7  pixel hits");
  cuts_.push_back("3.8  |d_xy|");
  cuts_.push_back("3.9  track isolation");
  cuts_.push_back("3.10 pt");
  cuts_.push_back("3.11 |eta|");

  // init the counters
  std::vector<std::string>::const_iterator iCut = cuts_.begin();
  for (; iCut != cuts_.end(); ++iCut) {
    muonsPassedTotal_[*iCut] = 0;
    eventsPassed_[*iCut] = 0;
  }
}

void 
MmgFsrRecoAnalyzer::endJob() 
{
  std::cout << "Total events processed: " << eventsProcessed_ << std::endl;

  std::cout << "== Muon cut flow table ==\n";
  std::cout << "Muons pass   Events pass   Cut\n";
  std::vector<std::string>::const_iterator iCut = cuts_.begin();
  for (; iCut != cuts_.end(); ++iCut) {
    printf("%10d   %11d   %s\n",
           muonsPassedTotal_[*iCut],
           eventsPassed_[*iCut],
           iCut->c_str()
           );
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(MmgFsrRecoAnalyzer);
