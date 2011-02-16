// Simple example demonstrating analysis with FWLite

// System includes
#include <iostream>
#include <boost/shared_ptr.hpp>

// CMSSW includes
#include "CommonTools/CandUtils/interface/CandCombiner.h"
#include "CommonTools/CandUtils/interface/AddFourMomenta.h"
#include "CommonTools/UtilAlgos/interface/DeltaR.h"
#include "CommonTools/Utils/interface/ExpressionHisto.h"
#include "CommonTools/Utils/interface/AnySelector.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/Candidate/interface/OverlapChecker.h"
#include "DataFormats/Candidate/interface/ShallowClonePtrCandidate.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/FWLite/interface/ChainEvent.h"
#include "DataFormats/FWLite/interface/MultiChainEvent.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "ElectroWeakAnalysis/MultiBosons/interface/DumpPtEtaPhiM.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "FWCore/PythonParameterSet/interface/PythonProcessDesc.h"
#include "FWCore/ParameterSet/interface/ProcessDesc.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "RecoLocalCalo/EcalRecAlgos/interface/EcalSeverityLevelAlgo.h"

//Root includes
#include "TSystem.h"

using namespace std;
using namespace edm;

int main ( int argc, char ** argv )
{
  cout << "Welcome to ZToMuMuGammaFsrSelectionSync!" << endl;

  // load framework libraries
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();

  if ( argc < 2 ) {
    cout << "Usage : " << argv[0] << " [parameters.py]" << endl;
    return 0;
  }
  PythonProcessDesc builder(argv[1]);
  boost::shared_ptr<ParameterSet>
    cfg = builder.processDesc()->getProcessPSet();
  ParameterSet const& inputs   = cfg->getParameter<ParameterSet>("inputs");
  ParameterSet const& outputs  = cfg->getParameter<ParameterSet>("outputs");
  ParameterSet const& analysis = cfg->getParameter<ParameterSet>("analysis");
  InputTag muonSrc   = analysis.getParameter<InputTag>("muonSrc");
  InputTag photonSrc = analysis.getParameter<InputTag>("photonSrc");

  fwlite::TFileService fs = fwlite::TFileService(
    outputs.getParameter<string>("outputName")
    );
  TFileDirectory theDir = fs.mkdir("histos");

  TH1F* histoPt_  = theDir.make<TH1F>("pt", "pt",    100,  0.,300.);

  vector<string> muonCuts;
  vector<string> dimuonCuts;
  vector<string> photonCuts;
  vector<string> mmgCuts;
  map<string, int> muonsPassedPerEvent;
  map<string, int> muonsPassedTotal;
  map<string, int> dimuonsPassedPerEvent;
  map<string, int> dimuonsPassedTotal;
  map<string, int> photonsPassedPerEvent;
  map<string, int> photonsPassedTotal;
  map<string, int> mmgCandsPassedPerEvent;
  map<string, int> mmgCandsPassedTotal;
  map<string, int> eventsPassed;

  std::vector<reco::ShallowClonePtrCandidate> selectedMuons;
  std::vector<reco::ShallowClonePtrCandidate> selectedDimuons;
  std::vector<reco::ShallowClonePtrCandidate> selectedPhotons;
  std::vector<reco::ShallowClonePtrCandidate> selectedMmgCands;

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

  // init the muons counters
  vector<string>::const_iterator iCut = muonCuts.begin();
  for (; iCut != muonCuts.end(); ++iCut) {
    muonsPassedTotal[*iCut] = 0;
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

  // This object 'event' is used both to get all information from the
  // event as well as to store histograms, etc.
  fwlite::ChainEvent ev (
    inputs.getParameter<vector<string> >("fileNames")
    );

  //loop through each event
  cout << "Looping over " << ev.size() << " events..." << endl;
  unsigned iEvent=0;
  for (ev.toBegin(), iEvent=0;
      !ev.atEnd() && iEvent < 99999;
      ++ev, ++iEvent) {

    if (iEvent % 1000 == 0) {
      cout << "Processing event: " << iEvent << endl;
    }

    // initialize per event counters
    for (iCut = muonCuts.begin(); iCut != muonCuts.end(); ++iCut)
      muonsPassedPerEvent[*iCut] = 0;

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

    EventBase const & event = ev;

    Handle<vector<pat::Muon> > muons;
    Handle<vector<pat::Photon> > photons;
    event.getByLabel(muonSrc, muons);
    event.getByLabel(photonSrc, photons);

    // loop over muons
    for(size_t iMuon=0; iMuon < muons->size(); ++iMuon) {
      pat::Muon const & muon = muons->at(iMuon);

      histoPt_->Fill( muon.pt() );

      ++muonsPassedPerEvent["2.   Collision Data Cleaning"];

      if ( muon.isGlobalMuon() == false ) continue;
      ++muonsPassedPerEvent["3.1  global muon"];

      if ( muon.normChi2() >= 10. ) continue;
      ++muonsPassedPerEvent["3.2  chi2 / ndof"];

      if ( muon.globalTrack()->hitPattern().numberOfValidMuonHits() < 1 ) continue;
      ++muonsPassedPerEvent["3.3  muon hits"];

      if ( muon.isTrackerMuon() == false ) continue;
      ++muonsPassedPerEvent["3.4  tracker muon"];

      if ( muon.numberOfMatches() < 2 ) continue;
      ++muonsPassedPerEvent["3.5  muon stations with matches"];

      if ( muon.track()->numberOfValidHits() < 11 ) continue;
      ++muonsPassedPerEvent["3.6  tracker hits"];

      if ( muon.track()->hitPattern().numberOfValidPixelHits() < 1 )
        continue;
      ++muonsPassedPerEvent["3.7  pixel hits"];

      if ( muon.dB(pat::Muon::BS2D) >= 0.2 ) continue;
      ++muonsPassedPerEvent["3.8  |d_xy|"];

      if ( muon.trackIso() >= 3. ) continue;
      ++muonsPassedPerEvent["3.9  track isolation"];

      if ( muon.pt() <= 10. ) continue;
      ++muonsPassedPerEvent["3.10 pt"];

      if ( fabs( muon.eta() ) >= 2.4 ) continue;
      ++muonsPassedPerEvent["3.11 |eta|"];

      selectedMuons.push_back(
        reco::ShallowClonePtrCandidate(
          edm::Ptr<pat::Muon>(muons, iMuon)
          )
        );

    }  // end loop over muons

    // update per-event muon counters
    for (iCut = muonCuts.begin(); iCut != muonCuts.end(); ++iCut) {
      muonsPassedTotal[*iCut] += muonsPassedPerEvent[*iCut];
      if (muonsPassedPerEvent[*iCut] >= 2) {
        ++eventsPassed[*iCut];
      }
    }

    if (selectedMuons.size() < 2) continue;

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
    if (selectedDimuons.size() < 1) continue;

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
      if (sLevel == EcalSeverityLevelAlgo::kWeird ||
          sLevel == EcalSeverityLevelAlgo::kBad
          ) continue;
      ++photonsPassedPerEvent["5.4  seed severity level"];

      if (photon.pt() <= 10.) continue;
      ++photonsPassedPerEvent["5.5  pt"];

      selectedPhotons.push_back(
        reco::ShallowClonePtrCandidate(
          edm::Ptr<pat::Photon>(photons, iPhoton)
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
    if (selectedPhotons.size() < 1) continue;

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

      const pat::Photon * photon;
      const reco::CompositeCandidate * dimuon;
      const pat::Muon * muon1;
      const pat::Muon * muon2;
      const pat::Muon * nearMuon;
      const pat::Muon * farMuon;

      photon = (const pat::Photon*) mmgCand.daughter("photon");
      dimuon = (const reco::CompositeCandidate*) mmgCand.daughter("dimuon");
      muon1 = (const pat::Muon*) dimuon->daughter(0);
      muon2 = (const pat::Muon*) dimuon->daughter(1);

      DeltaR<pat::Muon, pat::Photon> deltaR;
      double dr1 = deltaR(*muon1, *photon);
      double dr2 = deltaR(*muon2, *photon);
      double drMin = dr1;

      if (dr1 < dr2) {
        nearMuon = muon1; farMuon  = muon2; drMin = dr1;
      } else {
        nearMuon = muon2; farMuon  = muon1; drMin = dr2;
      }

      // Dump the event info
      DumpPtEtaPhi  ptEtaPhi;
      DumpPtEtaPhiM ptEtaPhiM;

      cout << "run lumi id: "
            << setw(3) << event.id().run() << " "
            << setw(3) << event.id().luminosityBlock() << " "
            << setw(8) << event.id().event()
            << setw(0) << endl;

      cout << setw(10)
           << "  pt eta phi m for mmg cand " << i << endl
           << "    mmg p4 minDR:    " << ptEtaPhiM(mmgCand) << " "
                                      << drMin
                                      << endl
           << "    dimuon p4:       " << ptEtaPhiM(*dimuon) << endl
           << "    near mu p4 hIso: " << ptEtaPhiM(*nearMuon) << " "
                                      << nearMuon->hcalIso()
//                                       << nearMuon->isolationR03().hadEt
                                      << endl
           << "    far mu p4 hIso:  " << ptEtaPhiM(*farMuon) << " "
                                      << farMuon->hcalIso()
//                                       << farMuon->isolationR03().hadEt
                                      << endl
           << "    photon p3:       " << ptEtaPhi(*photon) << endl;


//       if (nearMuon->hcalIso() >= 1.0) continue;
      if (nearMuon->isolationR03().hadEt >= 1.0) continue;
      ++mmgCandsPassedPerEvent["6.1 near muon HCAL iso"];

//       if (farMuon->ecalIso() >= 1.0) continue;
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


  } // end loop over events

  cout << "Total events processed: " << iEvent << endl;

  cout << "== Muon cut flow table ==" << endl;
  cout << "Muons pass   Events pass   Cut" << endl;
  for (iCut = muonCuts.begin(); iCut != muonCuts.end(); ++iCut) {
    printf("%10d   %11d   %s\n",
           muonsPassedTotal[*iCut],
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

  // exit gracefully
  cout << "Exitting ZToMuMuGammaFsrSelectionSync..." << endl;

  return 0;
}  // end main
