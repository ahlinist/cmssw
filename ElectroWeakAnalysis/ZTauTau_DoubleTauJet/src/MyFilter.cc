#include <memory>
#include <iostream>

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "ElectroWeakAnalysis/ZTauTau_DoubleTauJet/interface/MyFilter.h"
#include "ElectroWeakAnalysis/ZTauTau_DoubleTauJet/interface/EWKTauAnalyser.h"

#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"

using namespace edm;
using namespace std;

MyFilter::MyFilter(const edm::ParameterSet& iConfig)
{
        PFTaus_ = iConfig.getUntrackedParameter<string>("PFTaus", "pfRecoTauProducer");
   eventCharge_ = iConfig.getUntrackedParameter<double>("EventCharge", -1.0);
      debugFlg_ = iConfig.getUntrackedParameter<int>("DebugFlg", 0);
      minJetEt_ = iConfig.getUntrackedParameter<double>("MinJetEt", 1.0);
     hltJetSrc_ = iConfig.getUntrackedParameter<string>("hltJetSrc", "isolatedL25PixelTau");
}
bool MyFilter::filter(edm::Event& iEvent, const edm::EventSetup& iES)
{
  int run = iEvent.id().run();
  int evt = iEvent.id().event();

  if (debugFlg_)
  cout << "==============================================" << endl
       << "============ MyFilter::filter ================" << endl
       << " Run # " << run << " Event # " << evt << endl
       << "==============================================" << endl;
  
  // Particle Flow based information
  Handle<PFTauCollection> tauHandle;
  iEvent.getByLabel(PFTaus_, tauHandle);
  const PFTauCollection& myTauCollection = *(tauHandle.product()); 

  Handle<CaloJetCollection> tauHLTHandle;
  iEvent.getByLabel(hltJetSrc_, tauHLTHandle);
  const CaloJetCollection& myHLTJetCollection = *(tauHLTHandle.product()); 

  int n_candidate = myTauCollection.size();
  if (debugFlg_) cout << "Found " << n_candidate << " had. tau-jet candidates" << endl;

  // Now the real stuff
  if (!n_candidate) return false;

  int nLeadingTrackInJet = 0;  
  vector<int> tauCandidateList;
  vector<PFTau> extraJetList;
  for (PFTauCollection::size_type iTau = 0; iTau < tauHandle->size(); iTau++) {
    PFTauRef theTau(tauHandle, iTau);
    if (!theTau) {
      cerr << "Strange! Should have a valid PFTauRef for iTau=" << iTau << endl;
      continue;
    }
    const PFTau& tau = (*theTau);
    if (tau.et() < minJetEt_) continue;

    double minDR = 99.9;
    bool matched = EWKTauAnalyser::matchWithHLTJet(tau, myHLTJetCollection, minDR);
    if (debugFlg_) cout << "MyFilter::filter(): iTau=" << iTau 
                        << ", dR(PFTauJet, nearest HLT Jet) = " << minDR << endl;
    if (matched) {
      tauCandidateList.push_back(iTau);

      PFCandidateRef theLeadPFCand = tau.leadPFChargedHadrCand();
      if (!theLeadPFCand) {      
        cerr << "MyFilter::filter(): No Lead PFCand, iTau=" << iTau << endl;
        continue;
      }
      nLeadingTrackInJet++;
    }
    else {
      extraJetList.push_back(tau);
    }
  }
  // We retain events only with two PFJets each matched with HLT 
  if (tauCandidateList.size() != 2) return false;

  // each candidate should have an associated leading track 
  // --- if (nLeadingTrackInJet < 2) return false;

  // Now divide in OS/SS samples and accept only one at a time
  PFTau taua, taub;
  EWKTauAnalyser::TauObject(tauHandle, tauCandidateList[0], taua); 
  EWKTauAnalyser::TauObject(tauHandle, tauCandidateList[1], taub); 

  //-- retain OS/SS events only 
  double tch =  EWKTauAnalyser::getTauCharge(taua, debugFlg_) * EWKTauAnalyser::getTauCharge(taub, debugFlg_);
  if (debugFlg_) cout << "MyFilter::filter(): ch1*ch2 = " << tch << endl;
  if (tch != eventCharge_) return false;

  return true; 
}
