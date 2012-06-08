// -*- C++ -*-
//
// Package:    MmgFsrFilter
// Class:      MmgFsrFilter
//
/**\class MmgFsrFilter MmgFsrFilter.cc ElectroWeakAnalysis/MmgFsrFilter/src/MmgFsrFilter.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Jan Veverka
//         Created:  Wed Mar 16 03:40:10 PDT 2011
// $Id: MmgFsrFilter.cc,v 1.1 2011/03/28 22:51:52 veverka Exp $
//
//


// system include files
#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
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
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "ElectroWeakAnalysis/MultiBosons/interface/DumpPtEtaPhiM.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "RecoLocalCalo/EcalRecAlgos/interface/EcalSeverityLevelAlgo.h"

//
// class declaration
//

class MmgFsrFilter : public edm::EDFilter {
public:
  explicit MmgFsrFilter(const edm::ParameterSet&);
  ~MmgFsrFilter();

private:
  virtual void beginJob() ;
  virtual bool filter(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  // ----------member data ---------------------------
  // input tags
  edm::InputTag photonSrc_;
  edm::InputTag muonSrc_;
  edm::InputTag beamSpotSrc_;
  edm::InputTag ebRecHitSrc_;
  edm::InputTag eeRecHitSrc_;

  // other input paramters
  bool requireFsr_;
  bool vetoFsr_;
  int  verbosity_;

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

  std::vector<const pat::Muon *> selectedMuons;
  std::vector<const reco::CompositeCandidate *> selectedDimuons;
  std::vector<const pat::Photon *> selectedPhotons;
  std::vector<const reco::CompositeCandidate *> selectedMmgCands;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
MmgFsrFilter::MmgFsrFilter(const edm::ParameterSet& iConfig) :
  photonSrc_  (iConfig.getUntrackedParameter<edm::InputTag>("photonSrc"  )),
  muonSrc_    (iConfig.getUntrackedParameter<edm::InputTag>("muonSrc"    )),
  beamSpotSrc_(iConfig.getUntrackedParameter<edm::InputTag>("beamSpotSrc")),
  ebRecHitSrc_(iConfig.getUntrackedParameter<edm::InputTag>("ebRecHitSrc")),
  eeRecHitSrc_(iConfig.getUntrackedParameter<edm::InputTag>("eeRecHitSrc")),
  requireFsr_ (iConfig.getUntrackedParameter<bool         >("requireFsr" )),
  vetoFsr_    (iConfig.getUntrackedParameter<bool         >("vetoFsr"    )),
  verbosity_  (iConfig.getUntrackedParameter<int          >("verbosity"  )),
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
  //now do what ever initialization is needed
  if (requireFsr_ && vetoFsr_)
    throw cms::Exception("InvalidInput")
      << "Cannot requireFsr and vetoFsr simultaneously!"
      << std::endl;
}


MmgFsrFilter::~MmgFsrFilter()
{

  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
MmgFsrFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  ++eventsProcessed_;

  // define handles
  edm::Handle<edm::View<pat::Muon> >    muons;
  edm::Handle<edm::View<pat::Photon> >  photons;
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
    pat::Muon const & muon = muons->at(iMuon);

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

//     if ( fabs( muon.track()->dxy( beamSpot->position() ) ) >= 0.2 ) continue;
    /// Use this one!
    if ( fabs( muon.globalTrack()->dxy( beamSpot->position() ) ) >= 0.2 ) continue;
//     if (muon.innerTrack()->dxy(beamSpot->position() ) >= .2) continue;
    /// Original version of Olivier
//     if (-muon.innerTrack()->dxy(beamSpot->position() ) >= .2) continue;
    ++muonsPassedPerEvent_["3.8  |d_xy|"];

    if ( muon.isolationR03().sumPt >= 3. ) continue;
    ++muonsPassedPerEvent_["3.9  track isolation"];

    if ( muon.pt() <= 10. ) continue;
    ++muonsPassedPerEvent_["3.10 pt"];

    if ( fabs( muon.eta() ) >= 2.4 ) continue;
    ++muonsPassedPerEvent_["3.11 |eta|"];

//     selectedMuons.push_back(
//       reco::ShallowClonePtrCandidate(
//         edm::Ptr<pat::Muon>(muons, iMuon)
//         )
//       );
    selectedMuons.push_back(&muon);

  } // end loop over muons

  // update per-event counters
  for (iCut = muonCuts.begin(); iCut != muonCuts.end(); ++iCut) {
    muonsPassedTotal_[*iCut] += muonsPassedPerEvent_[*iCut];
    if (muonsPassedPerEvent_[*iCut] >= 2) {
      ++eventsPassed[*iCut];
    }
  }

  if (selectedMuons.size() < 2) return false;

  // combine selected muons to dimuons
  reco::CompositeCandidateCollection dimuons;
//   std::vector<reco::ShallowClonePtrCandidate>::const_iterator dau1;
//   std::vector<reco::ShallowClonePtrCandidate>::const_iterator dau2;
  AddFourMomenta addP4;
  OverlapChecker hasOverlap;
  // create all dimuon combinations
  // loop over first daughters
  for (std::vector<const pat::Muon *>::const_iterator
       dau1 = selectedMuons.begin();
       dau1 < selectedMuons.end() - 1;
       ++dau1) {
    // loop over second daughters
    for (std::vector<const pat::Muon *>::const_iterator
         dau2 = dau1 + 1;
         dau2 < selectedMuons.end();
         ++dau2) {
      if ( hasOverlap(**dau1, **dau2) ) continue;
      reco::CompositeCandidate dimuon;
      dimuon.addDaughter(**dau1, "muon1");
      dimuon.addDaughter(**dau2, "muon2");
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

    selectedDimuons.push_back(&dimuon);
/*    selectedDimuons.push_back(
      reco::ShallowClonePtrCandidate(
        edm::Ptr<reco::CompositeCandidate>(&dimuons, iDimuon)
        )
      );*/
  } // end loop over dimuons

  // update per-event dimuon counters
  for (iCut = dimuonCuts.begin(); iCut != dimuonCuts.end(); ++iCut) {
    dimuonsPassedTotal[*iCut] += dimuonsPassedPerEvent[*iCut];
    if (dimuonsPassedPerEvent[*iCut] >= 1) {
      ++eventsPassed[*iCut];
    }
  }

  // make sure there is at leas 1 selected dimuon in the event
  if (selectedDimuons.size() < 1) return false;

  // loop over photons
  for(size_t iPhoton=0; iPhoton < photons->size(); ++iPhoton) {
    pat::Photon const & photon = photons->at(iPhoton);
    ++photonsPassedPerEvent["5.0  all photons"];

    double scEta = photon.superCluster()->eta();
    if (fabs(scEta) >= 2.5) continue;
    ++photonsPassedPerEvent["5.1  |eta_SC| tracker coverage"];

    if (1.4442 <= fabs(scEta) && fabs(scEta) <= 1.566) continue;
    ++photonsPassedPerEvent["5.2  |eta_SC| EB/EE transition"];

    int seedRecoFlag = photon.userInt("photonUserData:seedRecoFlag");
    if (seedRecoFlag == EcalRecHit::kOutOfTime) continue;
    ++photonsPassedPerEvent["5.3  seed reco flag"];

    int sLevel = photon.userInt("photonUserData:seedSeverityLevel");
    //if (sLevel == EcalSeverityLevelAlgo::kWeird ||
    //    sLevel == EcalSeverityLevelAlgo::kBad
    //    ) continue;
    ++photonsPassedPerEvent["5.4  seed severity level"];

    if (photon.pt() <= 10.) continue;
    ++photonsPassedPerEvent["5.5  pt"];

    selectedPhotons.push_back(&photon);
  }  // end loop over photons

  // update per-event photon counters
  for (iCut = photonCuts.begin(); iCut != photonCuts.end(); ++iCut) {
    photonsPassedTotal[*iCut] += photonsPassedPerEvent[*iCut];
    if (photonsPassedPerEvent[*iCut] >= 1) {
      ++eventsPassed[*iCut];
    }
  }

  // check if we have at least one selected photon
  if (selectedPhotons.size() < 1) return false;

  // combine selected photons and dimuons to mmg candidates
  reco::CompositeCandidateCollection mmgCands;
  // create all photon-dimuon combinations
  // loop over photons
  for (std::vector<const pat::Photon *>::const_iterator
       dau1 = selectedPhotons.begin();
       dau1 != selectedPhotons.end();
       ++dau1) {
    // loop over dimuons
    for (std::vector<const reco::CompositeCandidate *>::const_iterator
         dau2 = selectedDimuons.begin();
         dau2 < selectedDimuons.end();
         ++dau2) {
      if ( hasOverlap(**dau1, **dau2) ) continue;
      reco::CompositeCandidate mmgCand;
      mmgCand.addDaughter(**dau1, "photon");
      mmgCand.addDaughter(**dau2, "dimuon");
      addP4.set(mmgCand);
      mmgCands.push_back(mmgCand);
    } // end loop over dimuons
  } // end loop over photons

  // loop over mmg candidates
  for (size_t i=0; i< mmgCands.size(); ++i) {
    reco::CompositeCandidate const & mmgCand = mmgCands[i];

    ++mmgCandsPassedPerEvent["6.0 all mmg candidates"];

    const pat::Photon * photon;
    const reco::CompositeCandidate * dimuon;
    const pat::Muon * muon1;
    const pat::Muon * muon2;
    const pat::Muon * nearMuon;
    const pat::Muon * farMuon;

    photon = (const pat::Photon*) mmgCand.daughter("photon");
    dimuon = (const reco::CompositeCandidate*) mmgCand.daughter("dimuon");
    muon1 = (const pat::Muon*) dimuon->daughter("muon1");
    muon2 = (const pat::Muon*) dimuon->daughter("muon2");


//     photon = dynamic_cast<const pat::Photon*> (
//                mmgCand.daughter("photon")->masterClonePtr().get()
//              );
//     dimuon = dynamic_cast<const reco::CompositeCandidate*> (
//                mmgCand.daughter("dimuon")
//              );
//     muon1  = dynamic_cast<const pat::Muon*> (
//                dimuon->daughter(0)->masterClonePtr().get()
//              );
//     muon2  = dynamic_cast<const pat::Muon*> (
//                dimuon->daughter(1)->masterClonePtr().get()
//              );

    DeltaR<reco::Candidate, reco::Candidate> deltaR;
    double dr1 = deltaR(*muon1, *photon);
    double dr2 = deltaR(*muon2, *photon);
    double drMin = dr1;

    if (dr1 < dr2) {
      nearMuon = muon1; farMuon  = muon2; drMin = dr1;
    } else {
      nearMuon = muon2; farMuon  = muon1; drMin = dr2;
    }

      // Dump the event info
      if (0) {
        DumpPtEtaPhi  ptEtaPhi;
        DumpPtEtaPhiM ptEtaPhiM;

        std::cout << "run lumi id: "
              << std::setw(3) << iEvent.id().run() << " "
              << std::setw(3) << iEvent.id().luminosityBlock() << " "
              << std::setw(8) << iEvent.id().event()
              << std::setw(0) << std::endl;

        std::cout << std::setw(10)
            << "  pt eta phi m for mmg cand " << i << std::endl
            << "    mmg p4 minDR:    " << ptEtaPhiM(mmgCand) << " "
                                        << drMin
                                        << std::endl
            << "    dimuon p4:       " << ptEtaPhiM(*dimuon) << std::endl
            << "    near mu p4 t/e/h Iso: " << ptEtaPhiM(*nearMuon) << " "
  //                                         << nearMuon->hcalIso()
                                            << nearMuon->isolationR03().sumPt << " "
                                            << nearMuon->isolationR03().emEt  << " "
                                            << nearMuon->isolationR03().hadEt
                                            << std::endl
            << "    far mu p4 t/e/h Iso:  " << ptEtaPhiM(*farMuon) << " "
  //                                         << farMuon->hcalIso()
                                            << farMuon->isolationR03().sumPt << " "
                                            << farMuon->isolationR03().emEt  << " "
                                            << farMuon->isolationR03().hadEt
                                            << std::endl
            << "    photon p3:            " << ptEtaPhi(*photon) << std::endl;
      } // end if dump the info
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

    if (mmgCand.mass() < 30. || 150. < mmgCand.mass() ) continue;
    ++mmgCandsPassedPerEvent["6.5 mmg mass"];

    selectedMmgCands.push_back(&mmgCand);

    if (verbosity_ > 1) {
      // dump scale info
      const double zMassPdg = 91.1876;
      double mmMass = dimuon->mass();
      double mmMass2 = mmMass * mmMass;
      double mmgMass = mmgCand.mass();
      double mmgMass2 = mmgMass * mmgMass;
      double kRatio = (zMassPdg * zMassPdg - mmMass2) / (mmgMass2 - mmMass2);

      const char *separator = " ";
      std::cout << std::setiosflags(std::ios::fixed);
      std::cout << std::setprecision(6);
      std::cout << "SELECTED:"
        << separator << std::setw(10) << iEvent.run()                  // run
        << separator << std::setw(10) << iEvent.id().luminosityBlock() // lumi
        << separator << std::setw(20) << iEvent.id().event()           // event
        << separator << std::setw(10) << photon  ->isEB()              // phoIsEB[g]
        << separator << std::setw(10) << photon  ->pt()                // phoPt[g]
        << separator << std::setw(10) << nearMuon->pt()                // muPt[mnear]
        << separator << std::setw(10) << farMuon ->pt()                // muPt[mfar]
        << separator << std::setw(10) << photon  ->eta()               // phoEta[g]
        << separator << std::setw(10) << nearMuon->eta()               // muEta[mnear]
        << separator << std::setw(10) << farMuon ->eta()               // muEta[mfar]
        << separator << std::setw(10) << photon  ->phi()               // phoPhi[g]
        << separator << std::setw(10) << nearMuon->phi()               // muPhi[mnear]
        << separator << std::setw(10) << farMuon ->phi()               // muPhi[mfar]
        << separator << std::setw(10) << photon->r9()                  // phoR9
        << separator << std::setw(10) << dimuon->mass()                // mass[mm]
        << separator << std::setw(10) << mmgCand.mass()                // mmgMass
        << separator << std::setw(10) << drMin                         // mmgDeltaRNear
        << separator << std::setw(10) << kRatio                        // kRatio(mmgMass,mass[mm])
        << std::endl;

    } // end dump scale info
  } // end loop over mmg candidates

  // update per-event mmg candidate counters
  for (iCut = mmgCuts.begin(); iCut != mmgCuts.end(); ++iCut) {
    mmgCandsPassedTotal[*iCut] += mmgCandsPassedPerEvent[*iCut];
    if (mmgCandsPassedPerEvent[*iCut] >= 1) {
      ++eventsPassed[*iCut];
    }
  }

  if (selectedMmgCands.size() > 0)
    return true;
  else
    return false;
}

// ------------ method called once each job just before starting event loop  ------------
void
MmgFsrFilter::beginJob()
{
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
  if (requireFsr_)
    mmgCuts.push_back("6.6 is FSR");
  else if (vetoFsr_)
    mmgCuts.push_back("6.6 is not FSR");

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

  if (verbosity_ > 1) {
    std::cout << "SELECTED: run lumi event phoIsEB[g] phoPt[g] muPt[mnear] muPt[mfar] "
      "phoEta[g] muEta[mnear] muEta[mfar] phoPhi[g] muPhi[mnear] muPhi[mfar] "
      "phoR9 mass[mm] mmgMass mmgDeltaRNear kRatio(mmgMass,mass[mm])"
      << std::endl;
  }
}

// ------------ method called once each job just after ending the event loop  ------------
void
MmgFsrFilter::endJob()
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
} // end MmgFsrFilter::endJob()

//define this as a plug-in
DEFINE_FWK_MODULE(MmgFsrFilter);
