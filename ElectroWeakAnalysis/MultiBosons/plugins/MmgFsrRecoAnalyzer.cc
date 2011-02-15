#include <cmath>
#include <iostream>
#include <map>
#include <string>
#include <stdio.h>
#include <vector>

#include "CommonTools/CandUtils/interface/AddFourMomenta.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "CommonTools/UtilAlgos/interface/DeltaR.h"
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
#include "FWCore/MessageLogger/interface/MessageLogger.h"
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

  // loop over dimuons
  reco::CompositeCandidateCollection::const_iterator dimuon;
  for (size_t iDimuon=0; iDimuon < dimuons.size(); ++iDimuon) {
    reco::CompositeCandidate const & dimuon = dimuons[iDimuon];

    ++dimuonsPassedPerEvent["4.0  all dimuons"];

    if (dimuon.charge() != 0) continue;
    ++dimuonsPassedPerEvent["4.1  dimuon charge"];

    if (dimuon.mass() < 40. || 80. < dimuon.mass()) continue;
    ++dimuonsPassedPerEvent["4.2  dimuon mass"];

    selectedDimuons.push_back(
      reco::ShallowClonePtrCandidate(
        edm::Ptr<reco::CompositeCandidate>(&dimuons, iDimuon)
        )
      );
  } // end loop over dimuons

  // update per-event dimuon counters
  for (iCut = dimuonCuts.begin(); iCut != dimuonCuts.end(); ++iCut) {
    dimuonsPassedTotal[*iCut] += dimuonsPassedPerEvent[*iCut];
    if (dimuonsPassedPerEvent[*iCut] >= 1) {
      ++eventsPassed[*iCut];
    }
  }

  // make sure there is at leas 1 selected dimuon in the event
  if (selectedDimuons.size() < 1) return;

  // loop over photons
  for(size_t iPhoton=0; iPhoton < photons->size(); ++iPhoton) {
    reco::Photon const & photon = photons->at(iPhoton);
    ++photonsPassedPerEvent["5.0  all photons"];

    double scEta = photon.superCluster()->eta();
    if (fabs(scEta) >= 2.5) continue;
    ++photonsPassedPerEvent["5.1  |eta_SC| tracker coverage"];

    if (1.4442 <= fabs(scEta) && fabs(scEta) <= 1.566) continue;
    ++photonsPassedPerEvent["5.2  |eta_SC| EB/EE transition"];

//     int seedRecoFlag = photon.userInt("photonUserData:seedRecoFlag");
//     if (seedRecoFlag == EcalRecHit::kOutOfTime) continue;
    ++photonsPassedPerEvent["5.3  seed reco flag"];

//     int sLevel = photon.userInt("photonUserData:seedSeverityLevel");
//     if (sLevel == EcalSeverityLevelAlgo::kWeird ||
//         sLevel == EcalSeverityLevelAlgo::kBad
//         ) continue;
    ++photonsPassedPerEvent["5.4  seed severity level"];

    if (photon.pt() <= 10.) continue;
    ++photonsPassedPerEvent["5.5  pt"];

    selectedPhotons.push_back(
      reco::ShallowClonePtrCandidate(
        edm::Ptr<reco::Photon>(photons, iPhoton)
      )
    );
  }  // end loop over photons

  // update per-event photon counters
  for (iCut = photonCuts.begin(); iCut != photonCuts.end(); ++iCut) {
    photonsPassedTotal[*iCut] += photonsPassedPerEvent[*iCut];
    if (photonsPassedPerEvent[*iCut] >= 1) {
      ++eventsPassed[*iCut];
    }
  }

  // check if we have at least one selected photon
  if (selectedPhotons.size() < 1) return;

  // combine selected photons and dimuons to mmg candidates
  reco::CompositeCandidateCollection mmgCands;
  // create all photon-dimuon combinations
  // loop over photons
  for (dau1 = selectedPhotons.begin();
        dau1 < selectedPhotons.end();
        ++dau1) {
    // loop over dimuons
    for (dau2 = selectedDimuons.begin();
          dau2 < selectedDimuons.end();
          ++dau2) {
      if ( hasOverlap(*dau1, *dau2) ) continue;
      reco::CompositeCandidate mmgCand;
      mmgCand.addDaughter(*dau1, "photon");
      mmgCand.addDaughter(*dau2, "dimuon");
      addP4.set(mmgCand);
      mmgCands.push_back(mmgCand);
    } // end loop over dimuons
  } // end loop over photons

  // loop over mmg candidates
  for (size_t i=0; i< mmgCands.size(); ++i) {
    reco::CompositeCandidate const & mmgCand = mmgCands[i];

    ++mmgCandsPassedPerEvent["6.0 all mmg candidates"];

    const reco::Photon * photon;
    const reco::CompositeCandidate * dimuon;
    const reco::Muon * muon1;
    const reco::Muon * muon2;
    const reco::Muon * nearMuon;
    const reco::Muon * farMuon;

    photon = (const reco::Photon*) mmgCand.daughter("photon");
    dimuon = (const reco::CompositeCandidate*) mmgCand.daughter("dimuon");
    muon1  = (const reco::Muon*) dimuon->daughter(0);
    muon2  = (const reco::Muon*) dimuon->daughter(1);

    DeltaR<reco::Candidate, reco::Candidate> deltaR;
    double dr1 = deltaR(*muon1, *photon);
    double dr2 = deltaR(*muon2, *photon);
    double drMin = dr1;

    if (dr1 < dr2) {
      nearMuon = muon1; farMuon  = muon2; drMin = dr1;
    } else {
      nearMuon = muon2; farMuon  = muon1; drMin = dr2;
    }

    // if (nearMuon->hcalIso() >= 1.0) continue;
    if (nearMuon->isolationR03().hadEt >= 1.0) continue;
    ++mmgCandsPassedPerEvent["6.1 near muon HCAL iso"];

    // if (farMuon->ecalIso() >= 1.0) continue;
    if (farMuon->isolationR03().emEt >= 1.0) continue;
    ++mmgCandsPassedPerEvent["6.2 far muon ECAL iso"];

    if (drMin >= 0.8) continue;
    ++mmgCandsPassedPerEvent["6.3 min Delta R"];

    if (farMuon->pt() <= 30.) continue;
    ++mmgCandsPassedPerEvent["6.4 far muon pt"];

    if (mmgCand.mass() < 70. || 110. < mmgCand.mass() ) continue;
    ++mmgCandsPassedPerEvent["6.5 mmg mass"];

    selectedMmgCands.push_back(
      reco::ShallowClonePtrCandidate(
        edm::Ptr<reco::CompositeCandidate>(&mmgCands, i)
        )
      );
  } // end loop over mmg candidates

  // update per-event mmg candidate counters
  for (iCut = mmgCuts.begin(); iCut != mmgCuts.end(); ++iCut) {
    mmgCandsPassedTotal[*iCut] += mmgCandsPassedPerEvent[*iCut];
    if (mmgCandsPassedPerEvent[*iCut] >= 1) {
      ++eventsPassed[*iCut];
    }
  }

} // end MmgFsrRecoAnalyzer::analyze()

void
MmgFsrRecoAnalyzer::beginJob()
{
  LogDebug("SegFault") << "Entering beginJob ..." << std::endl;

  // register to the TFileService
  edm::Service<TFileService> fs;

  // book histograms:
  histContainer_["muons"  ]=fs->make<TH1F>("muons",   "muon multiplicity",     10, 0,  10);

  LogDebug("SegFault") << "defining cuts ..." << std::endl;

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

  LogDebug("SegFault") << "Initializing counters ..." << std::endl;

  // init the counters
  std::vector<std::string>::const_iterator iCut;

  for (iCut = muonCuts.begin(); iCut != muonCuts.end(); ++iCut) {
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

  LogDebug("SegFault") << "Exiting beginJob with success!" << std::endl;
}

void
MmgFsrRecoAnalyzer::endJob()
{

  using namespace std;

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

  cout << "== Dimuon cut flow table ==" << endl;
  cout << "Dimuons pass   Events pass   Cut" << endl;
  for (iCut = dimuonCuts.begin(); iCut != dimuonCuts.end(); ++iCut) {
    printf("%12d   %11d   %s\n",
           dimuonsPassedTotal[*iCut],
           eventsPassed[*iCut],
           iCut->c_str()
           );
  }
  cout << "== Photons cut flow table ==" << endl;
  cout << "Photons pass   Events pass   Cut" << endl;
  for (iCut = photonCuts.begin(); iCut != photonCuts.end(); ++iCut) {
    printf("%12d   %11d   %s\n",
           photonsPassedTotal[*iCut],
           eventsPassed[*iCut],
           iCut->c_str()
           );
  }

  cout << "== MMG candidates cut flow table ==" << endl;
  cout << "MMG cands pass   Events pass   Cut" << endl;
  for (iCut = mmgCuts.begin(); iCut != mmgCuts.end(); ++iCut) {
    printf("%14d   %11d   %s\n",
           mmgCandsPassedTotal[*iCut],
           eventsPassed[*iCut],
           iCut->c_str()
           );
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(MmgFsrRecoAnalyzer);
