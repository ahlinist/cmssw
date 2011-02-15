#include <cmath>
#include <iostream>
#include <map>
#include <string>
#include <stdio.h>
#include <vector>

#include "CommonTools/CandUtils/interface/AddFourMomenta.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "CondFormats/DataRecord/interface/EcalChannelStatusRcd.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/Candidate/interface/OverlapChecker.h"
#include "DataFormats/Candidate/interface/ShallowClonePtrCandidate.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "RecoLocalCalo/EcalRecAlgos/interface/EcalSeverityLevelAlgo.h"

#include "TH1.h"

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
  edm::InputTag ebRecHitSrc_;
  edm::InputTag eeRecHitSrc_;

  // cut flow bookkeeping
  std::vector<std::string> muonCuts;
  std::vector<std::string> dimuonCuts;
  std::vector<std::string> photonCuts;
  std::vector<std::string> mmgCuts;

  std::map<std::string, int> muonsPassedPerEvent_;
  std::map<std::string, int> muonsPassedTotal_;
  std::map<std::string, int> dimuonsPassedPerEvent;
  std::map<std::string, int> dimuonsPassedTotal;
  std::map<std::string, int> photonsPassedPerEvent;
  std::map<std::string, int> photonsPassedTotal;
  std::map<std::string, int> mmgCandsPassedPerEvent;
  std::map<std::string, int> mmgCandsPassedTotal;
  std::map<std::string, int> eventsPassed;

  int eventsProcessed_;

  std::vector<reco::ShallowClonePtrCandidate> selectedMuons;
  std::vector<reco::ShallowClonePtrCandidate> selectedDimuons;
  std::vector<reco::ShallowClonePtrCandidate> selectedPhotons;
  std::vector<reco::ShallowClonePtrCandidate> selectedMmgCands;

}; // end class MmgFsrRecoAnalyzer

MmgFsrRecoAnalyzer::MmgFsrRecoAnalyzer(const edm::ParameterSet& iConfig):
  histContainer_(),
  photonSrc_  (iConfig.getUntrackedParameter<edm::InputTag>("photonSrc"  )),
  muonSrc_    (iConfig.getUntrackedParameter<edm::InputTag>("muonSrc"    )),
  beamSpotSrc_(iConfig.getUntrackedParameter<edm::InputTag>("beamSpotSrc")),
  ebRecHitSrc_(iConfig.getUntrackedParameter<edm::InputTag>("ebRecHitSrc")),
  eeRecHitSrc_(iConfig.getUntrackedParameter<edm::InputTag>("eeRecHitSrc")),
  muonCuts(),
  dimuonCuts(),
  photonCuts(),
  mmgCuts(),
  muonsPassedPerEvent_(),
  muonsPassedTotal_(),
  dimuonsPassedPerEvent(),
  dimuonsPassedTotal(),
  photonsPassedPerEvent(),
  photonsPassedTotal(),
  mmgCandsPassedPerEvent(),
  mmgCandsPassedTotal(),
  eventsPassed(),
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
  edm::Handle<EcalRecHitCollection>     eeRecHits;
  edm::Handle<EcalRecHitCollection>     ebRecHits;

  edm::ESHandle<EcalChannelStatus> channelStatus;

  // get the collections
  iEvent.getByLabel(muonSrc_, muons);
  iEvent.getByLabel(photonSrc_, photons);
  iEvent.getByLabel(beamSpotSrc_, beamSpot);
  iEvent.getByLabel(ebRecHitSrc_, ebRecHits);
  iEvent.getByLabel(eeRecHitSrc_, eeRecHits);

  iSetup.get<EcalChannelStatusRcd>().get(channelStatus);


  // initialize per event counters
  std::vector<std::string>::const_iterator iCut = muonCuts.begin();
  for (; iCut != muonCuts.end(); ++iCut) {
      muonsPassedPerEvent_[*iCut] = 0;
  }
  for (iCut = dimuonCuts.begin(); iCut != dimuonCuts.end(); ++iCut)
    dimuonsPassedPerEvent[*iCut] = 0;

  for (iCut = photonCuts.begin(); iCut != photonCuts.end(); ++iCut)
    photonsPassedPerEvent[*iCut] = 0;

  for (iCut = mmgCuts.begin(); iCut != mmgCuts.end(); ++iCut)
    mmgCandsPassedPerEvent[*iCut] = 0;

  selectedMuons.clear();
  selectedDimuons.clear();
  selectedPhotons.clear();
  selectedMmgCands.clear();

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

    selectedMuons.push_back(
      reco::ShallowClonePtrCandidate(
        edm::Ptr<reco::Muon>(muons, iMuon)
        )
      );

  } // end loop over muons

  // update per-event counters
  for (iCut = muonCuts.begin(); iCut != muonCuts.end(); ++iCut) {
    muonsPassedTotal_[*iCut] += muonsPassedPerEvent_[*iCut];
    if (muonsPassedPerEvent_[*iCut] >= 2) {
      ++eventsPassed[*iCut];
    }
  }

  if (selectedMuons.size() < 2) return;

  // combine selected muons to dimuons
  reco::CompositeCandidateCollection dimuons;
  std::vector<reco::ShallowClonePtrCandidate>::const_iterator dau1;
  std::vector<reco::ShallowClonePtrCandidate>::const_iterator dau2;
  AddFourMomenta addP4;
  OverlapChecker hasOverlap;
  // create all dimuon combinations
  // loop over first daughters
  for (dau1 = selectedMuons.begin();
        dau1 < selectedMuons.end() - 1;
        ++dau1) {
    // loop over second daughters
    for (dau2 = dau1 + 1;
          dau2 < selectedMuons.end();
          ++dau2) {
      if ( hasOverlap(*dau1, *dau2) ) continue;
      reco::CompositeCandidate dimuon;
      dimuon.addDaughter(*dau1, "muon1");
      dimuon.addDaughter(*dau2, "muon2");
      addP4.set( dimuon );
      dimuons.push_back( dimuon );
    } // end loop over second daughters
  } // end loop over first daughters


}

void
MmgFsrRecoAnalyzer::beginJob()
{
  // register to the TFileService
  edm::Service<TFileService> fs;

  // book histograms:
  histContainer_["muons"  ]=fs->make<TH1F>("muons",   "muon multiplicity",     10, 0,  10);

  // define the cuts
  muonCuts.push_back("2.   Collision Data Cleaning");
  muonCuts.push_back("3.1  global muon");
  muonCuts.push_back("3.2  chi2 / ndof");
  muonCuts.push_back("3.3  muon hits");
  muonCuts.push_back("3.4  tracker muon");
  muonCuts.push_back("3.5  muon stations with matches");
  muonCuts.push_back("3.6  tracker hits");
  muonCuts.push_back("3.7  pixel hits");
  muonCuts.push_back("3.8  |d_xy|");
  muonCuts.push_back("3.9  track isolation");
  muonCuts.push_back("3.10 pt");
  muonCuts.push_back("3.11 |eta|");

  dimuonCuts.push_back("4.0  all dimuons");
  dimuonCuts.push_back("4.1  dimuon charge");
  dimuonCuts.push_back("4.2  dimuon mass");

  photonCuts.push_back("5.0  all photons");
  photonCuts.push_back("5.1  |eta_SC| tracker coverage");
  photonCuts.push_back("5.2  |eta_SC| EB/EE transition");
  photonCuts.push_back("5.3  seed reco flag");
  photonCuts.push_back("5.4  seed severity level");
  photonCuts.push_back("5.5  pt");

  mmgCuts.push_back("6.0 all mmg candidates");
  mmgCuts.push_back("6.1 near muon HCAL iso");
  mmgCuts.push_back("6.2 far muon ECAL iso");
  mmgCuts.push_back("6.3 min Delta R");
  mmgCuts.push_back("6.4 far muon pt");
  mmgCuts.push_back("6.5 mmg mass");

  // init the counters
  std::vector<std::string>::const_iterator iCut;

  for (muonCuts.begin(); iCut != muonCuts.end(); ++iCut) {
    muonsPassedTotal_[*iCut] = 0;
    eventsPassed[*iCut] = 0;
  }

  // init the dimuon counters
  for (iCut = dimuonCuts.begin(); iCut != dimuonCuts.end(); ++iCut) {
    dimuonsPassedTotal[*iCut] = 0;
    eventsPassed[*iCut] = 0;
  }

  // init the photon counters
  for (iCut = photonCuts.begin(); iCut != photonCuts.end(); ++iCut) {
    photonsPassedTotal[*iCut] = 0;
    eventsPassed[*iCut] = 0;
  }

  // init the mmg counters
  for (iCut = mmgCuts.begin(); iCut != mmgCuts.end(); ++iCut) {
    mmgCandsPassedTotal[*iCut] = 0;
    eventsPassed[*iCut] = 0;
  }
}

void
MmgFsrRecoAnalyzer::endJob()
{
  std::cout << "Total events processed: " << eventsProcessed_ << std::endl;

  std::cout << "== Muon cut flow table ==\n";
  std::cout << "Muons pass   Events pass   Cut\n";
  std::vector<std::string>::const_iterator iCut = muonCuts.begin();
  for (; iCut != muonCuts.end(); ++iCut) {
    printf("%10d   %11d   %s\n",
           muonsPassedTotal_[*iCut],
           eventsPassed[*iCut],
           iCut->c_str()
           );
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(MmgFsrRecoAnalyzer);
