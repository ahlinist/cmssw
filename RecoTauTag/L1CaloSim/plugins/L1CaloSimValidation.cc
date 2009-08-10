#include "RecoTauTag/L1CaloSim/plugins/L1CaloSimValidation.h"

#include "TF1.h"
#include <TMath.h>
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"
#include <iostream>
#include <iomanip>

#include "PhysicsTools/Utilities/interface/deltaR.h"

using namespace std;
using namespace edm;

L1CaloSimValidation::L1CaloSimValidation(const edm::ParameterSet& iConfig)
{
  _L1extraTauJetSource = iConfig.getParameter<edm::InputTag>("L1extraTauJetSource");
  _L1extraCentralJetSource = iConfig.getParameter<edm::InputTag>("L1extraCentralJetSource");
  _L1calosimTauJetSource = iConfig.getParameter<edm::InputTag>("L1calosimTauJetSource");
  _L1calosimCentralJetSource = iConfig.getParameter<edm::InputTag>("L1calosimCentralJetSource");
  _L1JetMatchingCone = iConfig.getParameter<double>("L1JetMatchingCone");


  Service<TFileService> fs;
  
  _hL1extraNTauJet = fs->make<TH1F>("L1extraNTauJet", "L1extraNTauJet", 21, 0., 20.);
  _hL1extraTauJetEt     = fs->make<TH1F>("L1extraTauJetEt", "L1extraTauJetEt", 80, 0., 160.);
  _hL1extraTauJetEta    = fs->make<TH1F>("L1extraTauJetEta", "L1extraTauJetEta", 72, -3.6, +3.6);
  _hL1extraTauJetPhi    = fs->make<TH1F>("L1extraTauJetPhi", "L1extraTauJetPhi", 36, -TMath::Pi(), +TMath::Pi());

  _hL1calosimNTauJet = fs->make<TH1F>("L1calosimNTauJet", "L1calosimNTauJet", 21, 0., 20.);
  _hL1calosimTauJetEt     = fs->make<TH1F>("L1calosimTauJetEt", "L1calosimTauJetEt", 80, 0., 160.);
  _hL1calosimTauJetEta    = fs->make<TH1F>("L1calosimTauJetEta", "L1calosimTauJetEta", 72, -3.6, +3.6);
  _hL1calosimTauJetPhi    = fs->make<TH1F>("L1calosimTauJetPhi", "L1calosimTauJetPhi", 36, -TMath::Pi(), +TMath::Pi());

  _hL1extraNCentralJet = fs->make<TH1F>("L1extraNCentralJet", "L1extraNCentralJet", 21, 0., 20.);
  _hL1extraCentralJetEt     = fs->make<TH1F>("L1extraCentralJetEt", "L1extraCentralJetEt", 80, 0., 160.);
  _hL1extraCentralJetEta    = fs->make<TH1F>("L1extraCentralJetEta", "L1extraCentralJetEta", 72, -3.6, +3.6);
  _hL1extraCentralJetPhi    = fs->make<TH1F>("L1extraCentralJetPhi", "L1extraCentralJetPhi", 36, -TMath::Pi(), +TMath::Pi());

  _hL1calosimNCentralJet = fs->make<TH1F>("L1calosimNCentralJet", "L1calosimNCentralJet", 21, 0., 20.);
  _hL1calosimCentralJetEt     = fs->make<TH1F>("L1calosimCentralJetEt", "L1calosimCentralJetEt", 80, 0., 160.);
  _hL1calosimCentralJetEta    = fs->make<TH1F>("L1calosimCentralJetEta", "L1calosimCentralJetEta", 72, -3.6, +3.6);
  _hL1calosimCentralJetPhi    = fs->make<TH1F>("L1calosimCentralJetPhi", "L1calosimCentralJetPhi", 36, -TMath::Pi(), +TMath::Pi());

  
  _hMatchedL1TauJetDeltaEt     = fs->make<TH1F>("MatchedL1TauJetDeltaEt", "MatchedL1TauJetDeltaEt", 240, -60., 60.);
  _hMatchedL1TauJetDeltaEta    = fs->make<TH1F>("MatchedL1TauJetDeltaEta", "MatchedL1TauJetDeltaEta", 200, -3.6, +3.6);
  _hMatchedL1TauJetDeltaPhi    = fs->make<TH1F>("MatchedL1TauJetDeltaPhi", "MatchedL1TauJetDeltaPhi", 200, -TMath::Pi(), +TMath::Pi());


  _hMatchedL1CentralJetDeltaEt     = fs->make<TH1F>("MatchedL1CentralJetDeltaEt", "MatchedL1CentralJetDeltaEt", 240, -60., 60.);
  _hMatchedL1CentralJetDeltaEta    = fs->make<TH1F>("MatchedL1CentralJetDeltaEta", "MatchedL1CentralJetDeltaEta", 200, -3.6, +3.6);
  _hMatchedL1CentralJetDeltaPhi    = fs->make<TH1F>("MatchedL1CentralJetDeltaPhi", "MatchedL1CentralJetDeltaPhi", 200, -TMath::Pi(), +TMath::Pi());


  _hMatchedL1TauJetRelDeltaEt     = fs->make<TH1F>("MatchedL1TauJetRelDeltaEt", "MatchedL1TauJetRelDeltaEt", 100, 0., 2.);
  _hMatchedL1CentralJetRelDeltaEt     = fs->make<TH1F>("MatchedL1CentralJetRelDeltaEt", "MatchedL1CentralJetRelDeltaEt", 100, 0., 2.);


  _hL1extraTauJet1Et     = fs->make<TH1F>("L1extraTauJet1Et", "L1extraTauJet1Et", 80, 0., 160.);
  _hL1extraTauJet1Eta    = fs->make<TH1F>("L1extraTauJet1Eta", "L1extraTauJet1Eta", 72, -3.6, +3.6);
  _hL1extraTauJet1Phi    = fs->make<TH1F>("L1extraTauJet1Phi", "L1extraTauJet1Phi", 36, -TMath::Pi(), +TMath::Pi());

  _hL1calosimTauJet1Et     = fs->make<TH1F>("L1calosimTauJet1Et", "L1calosimTauJet1Et", 80, 0., 160.);
  _hL1calosimTauJet1Eta    = fs->make<TH1F>("L1calosimTauJet1Eta", "L1calosimTauJet1Eta", 72, -3.6, +3.6);
  _hL1calosimTauJet1Phi    = fs->make<TH1F>("L1calosimTauJet1Phi", "L1calosimTauJet1Phi", 36, -TMath::Pi(), +TMath::Pi());

  _hL1extraCentralJet1Et     = fs->make<TH1F>("L1extraCentralJet1Et", "L1extraCentralJet1Et", 80, 0., 160.);
  _hL1extraCentralJet1Eta    = fs->make<TH1F>("L1extraCentralJet1Eta", "L1extraCentralJet1Eta", 72, -3.6, +3.6);
  _hL1extraCentralJet1Phi    = fs->make<TH1F>("L1extraCentralJet1Phi", "L1extraCentralJet1Phi", 36, -TMath::Pi(), +TMath::Pi());

  _hL1calosimCentralJet1Et     = fs->make<TH1F>("L1calosimCentralJet1Et", "L1calosimCentralJet1Et", 80, 0., 160.);
  _hL1calosimCentralJet1Eta    = fs->make<TH1F>("L1calosimCentralJet1Eta", "L1calosimCentralJet1Eta", 72, -3.6, +3.6);
  _hL1calosimCentralJet1Phi    = fs->make<TH1F>("L1calosimCentralJet1Phi", "L1calosimCentralJet1Phi", 36, -TMath::Pi(), +TMath::Pi());

  
  _hMatchedL1TauJet1DeltaEt     = fs->make<TH1F>("MatchedL1TauJet1DeltaEt", "MatchedL1TauJet1DeltaEt", 240, -60., 60.);
  _hMatchedL1CentralJet1DeltaEt     = fs->make<TH1F>("MatchedL1CentralJet1DeltaEt", "MatchedL1CentralJet1DeltaEt", 240, -60., 60.);
  _hMatchedL1TauJet1RelDeltaEt     = fs->make<TH1F>("MatchedL1TauJet1RelDeltaEt", "MatchedL1TauJet1RelDeltaEt", 100, 0., 2.);
  _hMatchedL1CentralJet1RelDeltaEt     = fs->make<TH1F>("MatchedL1CentralJet1RelDeltaEt", "MatchedL1CentralJet1RelDeltaEt", 100, 0., 2.);

}

L1CaloSimValidation::~L1CaloSimValidation(){
}


void L1CaloSimValidation::beginJob(const edm::ParameterSet& iConfig, const edm::EventSetup& iES) {  
}

void L1CaloSimValidation::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  fillHists(iEvent);
}

void L1CaloSimValidation::endJob() {
}


void L1CaloSimValidation::fillHists(const edm::Event& iEvent) {

  Handle<L1JetParticleCollection> l1extraTauHandle;
  iEvent.getByLabel(_L1extraTauJetSource, l1extraTauHandle);
  if(!l1extraTauHandle.isValid()) {
    cout << "%L1TauEffAnalyzer -- No L1extra TauJet found! -- " << _L1extraTauJetSource << endl;
    return;
  }

  Handle<L1JetParticleCollection> l1extraCentralJetHandle;
  iEvent.getByLabel(_L1extraCentralJetSource, l1extraCentralJetHandle);
  if(!l1extraCentralJetHandle.isValid()) {
    cout << "%L1TauEffAnalyzer -- No L1extra central jet found! -- " << _L1extraCentralJetSource << endl;
    return;
  }


  Handle<L1JetParticleCollection> l1calosimTauHandle;
  iEvent.getByLabel(_L1calosimTauJetSource, l1calosimTauHandle);
  if(!l1calosimTauHandle.isValid()) {
    cout << "%L1TauEffAnalyzer -- No L1calosim TauJet found! -- " << _L1calosimTauJetSource << endl;
    return;
  }

  Handle<L1JetParticleCollection> l1calosimCentralJetHandle;
  iEvent.getByLabel(_L1calosimCentralJetSource, l1calosimCentralJetHandle);
  if(!l1calosimCentralJetHandle.isValid()) {
    cout << "%L1TauEffAnalyzer -- No L1calosim central jet found! -- " << _L1calosimCentralJetSource << endl;
    return;
  }


  // -------------- -------------- -------------- -------------- -------------- --------------  //

  L1JetParticleCollection::const_iterator itau, itau2, icentraljet, icentraljet2;

  const L1JetParticleCollection & l1extrataus = *(l1extraTauHandle.product());
  _hL1extraNTauJet->Fill(l1extrataus.size());
  for(itau = l1extrataus.begin(); itau != l1extrataus.end(); ++itau) {
    _hL1extraTauJetEt->Fill(itau->et());
    _hL1extraTauJetEta->Fill(itau->eta());
    _hL1extraTauJetPhi->Fill(itau->phi());
    if (itau == l1extrataus.begin()) {
      _hL1extraTauJet1Et->Fill(itau->et());
      _hL1extraTauJet1Eta->Fill(itau->eta());
      _hL1extraTauJet1Phi->Fill(itau->phi());
    }
  }

  const L1JetParticleCollection & l1calosimtaus = *(l1calosimTauHandle.product());
  _hL1calosimNTauJet->Fill(l1calosimtaus.size());
  for(itau = l1calosimtaus.begin(); itau != l1calosimtaus.end(); ++itau) {
    _hL1calosimTauJetEt->Fill(itau->et());
    _hL1calosimTauJetEta->Fill(itau->eta());
    _hL1calosimTauJetPhi->Fill(itau->phi());
    if (itau == l1calosimtaus.begin()) {
      _hL1calosimTauJet1Et->Fill(itau->et());
      _hL1calosimTauJet1Eta->Fill(itau->eta());
      _hL1calosimTauJet1Phi->Fill(itau->phi());
    }
  }

  const L1JetParticleCollection & l1extracentraljets = *(l1extraCentralJetHandle.product());
  _hL1extraNCentralJet->Fill(l1extracentraljets.size());
  for(icentraljet = l1extracentraljets.begin(); icentraljet != l1extracentraljets.end(); ++icentraljet) {
    _hL1extraCentralJetEt->Fill(icentraljet->et());
    _hL1extraCentralJetEta->Fill(icentraljet->eta());
    _hL1extraCentralJetPhi->Fill(icentraljet->phi());
    if (icentraljet == l1extracentraljets.begin()) {
      _hL1extraCentralJet1Et->Fill(icentraljet->et());
      _hL1extraCentralJet1Eta->Fill(icentraljet->eta());
      _hL1extraCentralJet1Phi->Fill(icentraljet->phi());
    }
  }

  const L1JetParticleCollection & l1calosimcentraljets = *(l1calosimCentralJetHandle.product());
  _hL1calosimNCentralJet->Fill(l1calosimcentraljets.size());
  for(icentraljet = l1calosimcentraljets.begin(); icentraljet != l1calosimcentraljets.end(); ++icentraljet) {
    _hL1calosimCentralJetEt->Fill(icentraljet->et());
    _hL1calosimCentralJetEta->Fill(icentraljet->eta());
    _hL1calosimCentralJetPhi->Fill(icentraljet->phi());
    if (icentraljet == l1calosimcentraljets.begin()) {
      _hL1calosimCentralJet1Et->Fill(icentraljet->et());
      _hL1calosimCentralJet1Eta->Fill(icentraljet->eta());
      _hL1calosimCentralJet1Phi->Fill(icentraljet->phi());
    }
  }
  
  for(itau = l1calosimtaus.begin(); itau != l1calosimtaus.end(); ++itau) {
    for(itau2 = l1extrataus.begin(); itau2 != l1extrataus.end(); ++itau2) {
      double DR = reco::deltaR(itau->eta(), itau->phi(), itau2->eta(), itau2->phi());
      _hMatchedL1TauJetDeltaEta->Fill(itau2->eta()-itau->eta());      
      _hMatchedL1TauJetDeltaPhi->Fill(itau2->phi()-itau->phi());      
      if (DR<_L1JetMatchingCone) {
	_hMatchedL1TauJetDeltaEt->Fill(itau2->et()-itau->et());      
	_hMatchedL1TauJetRelDeltaEt->Fill(itau->et() / itau2->et());      
	if (itau == l1calosimtaus.begin() && itau2 == l1extrataus.begin()) {
	  _hMatchedL1TauJet1DeltaEt->Fill(itau2->et()-itau->et());      
	  _hMatchedL1TauJet1RelDeltaEt->Fill(itau->et() / itau2->et());      
	}
      }
    }
  }

  for(icentraljet = l1calosimcentraljets.begin(); icentraljet != l1calosimcentraljets.end(); ++icentraljet) {
    for(icentraljet2 = l1extracentraljets.begin(); icentraljet2 != l1extracentraljets.end(); ++icentraljet2) {
      double DR = reco::deltaR(icentraljet->eta(), icentraljet->phi(), icentraljet2->eta(), icentraljet2->phi());
      _hMatchedL1CentralJetDeltaEta->Fill(icentraljet2->eta()-icentraljet->eta());      
      _hMatchedL1CentralJetDeltaPhi->Fill(icentraljet2->phi()-icentraljet->phi());      
      if (DR<_L1JetMatchingCone) {
	_hMatchedL1CentralJetDeltaEt->Fill(icentraljet2->et()-icentraljet->et());      
	_hMatchedL1CentralJetRelDeltaEt->Fill( icentraljet->et() / icentraljet2->et());
	if (icentraljet == l1calosimcentraljets.begin() && icentraljet2 == l1extracentraljets.begin()) {
	  _hMatchedL1CentralJet1DeltaEt->Fill(icentraljet2->et()-icentraljet->et());      
	  _hMatchedL1CentralJet1RelDeltaEt->Fill( icentraljet->et() / icentraljet2->et());
	}   
      }
    }
  }
  
} 

double L1CaloSimValidation::L1JetEtUncorr(const double corPt)
{
  TF1 *Response = new TF1("Response","[0]-[1]/(pow(log10(x),[2])+[3])+[4]/x",4,5000);
  Response->SetParameter(0,0.976811);
  Response->SetParameter(1,14.2444);
  Response->SetParameter(2,4.47607);
  Response->SetParameter(3,18.482);
  Response->SetParameter(4,0.717231);  
  double r = Response->Eval(corPt);
  double Pt = corPt*r;
  //cout<<"Corrected jet Pt = "<<corPt<<" GeV, Response = "<<r<<", Uncorrected jet Pt = "<<Pt<<" GeV"<<endl;                  
  return Pt;
}
