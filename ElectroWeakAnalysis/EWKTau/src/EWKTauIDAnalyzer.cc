// Package:    EWKTauIDAnalyzer
// Class:      EWKTauIDAnalyzer
// Original Author:  Alfredo Gurrola
//         Created:  Oct 29 05:23:58 CET 2008

#include "ElectroWeakAnalysis/EWKTau/interface/EWKTauIDAnalyzer.h"

EWKTauIDAnalyzer::EWKTauIDAnalyzer(const edm::ParameterSet& iConfig):
  muonPassingEtaLabel_(iConfig.getUntrackedParameter<edm::InputTag>("muonPassingEtaLabel")),
  muonPassingPtLabel_(iConfig.getUntrackedParameter<edm::InputTag>("muonPassingPtLabel")),
  muonPassingSeperationLabel_(iConfig.getUntrackedParameter<edm::InputTag>("muonPassingSeperationLabel")),
  tauPassingEtaLabel_(iConfig.getUntrackedParameter<edm::InputTag>("tauPassingEtaLabel")),
  tauPassingEtLabel_(iConfig.getUntrackedParameter<edm::InputTag>("tauPassingEtLabel")),
  tauPassingVetoLabel_(iConfig.getUntrackedParameter<edm::InputTag>("tauPassingVetoLabel"))
{

   m_file = new TFile("EWKTauIDAnalyzer.root", "recreate");

   m_Events = new TH1F("Events","Events",20,0.0,20.0);
   hSelectedEtaMuonsPt = new TH1F("hSelectedEtaMuonsPt","hSelectedEtaMuonsPt",20,0.0,100.0);
   hSelectedEtaMuonsEta = new TH1F("hSelectedEtaMuonsEta","hSelectedEtaMuonsEta",30,-3.0,3.0);
   hSelectedPtMuonsPt = new TH1F("hSelectedPtMuonsPt","hSelectedPtMuonsPt",20,0.0,100.0);
   hSelectedPtMuonsEta = new TH1F("hSelectedPtMuonsEta","hSelectedPtMuonsEta",30,-3.0,3.0);
   hSelectedEtaTausEt = new TH1F("hSelectedEtaTausEt","hSelectedEtaTausEt",20,0.0,100.0);
   hSelectedEtaTausEta = new TH1F("hSelectedEtaTausEta","hSelectedEtaTausEta",30,-3.0,3.0);
   hSelectedEtTausEt = new TH1F("hSelectedEtTausEt","hSelectedEtTausEt",20,0.0,100.0);
   hSelectedEtTausEta = new TH1F("hSelectedEtTausEta","hSelectedEtTausEta",30,-3.0,3.0);
   hSelectedVetoTausEt = new TH1F("hSelectedVetoTausEt","hSelectedVetoTausEt",20,0.0,100.0);
   hSelectedVetoTausEta = new TH1F("hSelectedVetoTausEta","hSelectedVetoTausEta",30,-3.0,3.0);
   hSelectedSeperationMuonsPt = new TH1F("hSelectedSeperationMuonsPt","hSelectedSeperationMuonsPt",20,0.0,100.0);
   hSelectedSeperationMuonsEta = new TH1F("hSelectedSeperationMuonsEta","hSelectedSeperationMuonsEta",30,-3.0,3.0);

   EventsWithMuonsPassingEtaCut=0;
   EventsWithMuonsPassingPtCut=0;
   EventsWithMuonsPassingSeperationCut=0;
   EventsWithTausPassingEtaCut=0;
   EventsWithTausPassingEtCut=0;
   EventsWithTausPassingSingleJetCut=0;
}

EWKTauIDAnalyzer::~EWKTauIDAnalyzer() { 
   m_file->Write();
   m_file->Close();
}


void EWKTauIDAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

   edm::Handle<edm::View<pat::Muon> > selectedMuonEtaHandle;
   iEvent.getByLabel(muonPassingEtaLabel_,selectedMuonEtaHandle);
   edm::View<pat::Muon> selectedMuonEta = *selectedMuonEtaHandle;

   edm::Handle<edm::View<pat::Muon> > selectedMuonPtHandle;
   iEvent.getByLabel(muonPassingPtLabel_,selectedMuonPtHandle);
   edm::View<pat::Muon> selectedMuonPt = *selectedMuonPtHandle;

   edm::Handle<edm::View<pat::Muon> > selectedMuonTauSeperationHandle;
   iEvent.getByLabel(muonPassingSeperationLabel_,selectedMuonTauSeperationHandle);
   edm::View<pat::Muon> selectedMuonTauSeperation = *selectedMuonTauSeperationHandle;

   edm::Handle<edm::View<pat::Tau> > selectedTauEtaHandle;
   iEvent.getByLabel(tauPassingEtaLabel_,selectedTauEtaHandle);
   edm::View<pat::Tau> selectedTauEta = *selectedTauEtaHandle;

   edm::Handle<edm::View<pat::Tau> > selectedTauEtHandle;
   iEvent.getByLabel(tauPassingEtLabel_,selectedTauEtHandle);
   edm::View<pat::Tau> selectedTauEt = *selectedTauEtHandle;

   edm::Handle<edm::View<pat::Tau> > selectedTauVetoHandle;
   iEvent.getByLabel(tauPassingVetoLabel_,selectedTauVetoHandle);
   edm::View<pat::Tau> selectedTauVeto = *selectedTauVetoHandle;

   if(selectedMuonEta.size() > 0) {m_Events->Fill(0);EventsWithMuonsPassingEtaCut++;}
   if(selectedMuonPt.size() > 0) {m_Events->Fill(1);EventsWithMuonsPassingPtCut++;}
   if(selectedTauEta.size() > 0) {m_Events->Fill(2);EventsWithTausPassingEtaCut++;}
   if(selectedTauEt.size() > 0) {m_Events->Fill(3);EventsWithTausPassingEtCut++;}
   if(selectedTauVeto.size() == 1) {m_Events->Fill(4);EventsWithTausPassingSingleJetCut++;}
   if(selectedMuonTauSeperation.size() > 0) {m_Events->Fill(5);EventsWithMuonsPassingSeperationCut++;}

   for(edm::View<pat::Muon>::const_iterator muon_iter = selectedMuonEta.begin(); muon_iter!=selectedMuonEta.end(); ++muon_iter) {
       hSelectedEtaMuonsPt->Fill(muon_iter->pt());
       hSelectedEtaMuonsEta->Fill(muon_iter->eta());
   }
   for(edm::View<pat::Muon>::const_iterator muon_iter = selectedMuonPt.begin(); muon_iter!=selectedMuonPt.end(); ++muon_iter) {
       hSelectedPtMuonsPt->Fill(muon_iter->pt());
       hSelectedPtMuonsEta->Fill(muon_iter->eta());
   }
   for(edm::View<pat::Tau>::const_iterator tau_iter = selectedTauEta.begin(); tau_iter!=selectedTauEta.end(); ++tau_iter) {
       hSelectedEtaTausEt->Fill(tau_iter->et());
       hSelectedEtaTausEta->Fill(tau_iter->eta());
   }
   for(edm::View<pat::Tau>::const_iterator tau_iter = selectedTauEt.begin(); tau_iter!=selectedTauEt.end(); ++tau_iter) {
       hSelectedEtTausEt->Fill(tau_iter->et());
       hSelectedEtTausEta->Fill(tau_iter->eta());
   }
   for(edm::View<pat::Tau>::const_iterator tau_iter = selectedTauVeto.begin(); tau_iter!=selectedTauVeto.end(); ++tau_iter) {
       hSelectedVetoTausEt->Fill(tau_iter->et());
       hSelectedVetoTausEta->Fill(tau_iter->eta());
   }
   for(edm::View<pat::Muon>::const_iterator muon_iter = selectedMuonTauSeperation.begin(); muon_iter!=selectedMuonTauSeperation.end(); ++muon_iter) {
       hSelectedSeperationMuonsPt->Fill(muon_iter->pt());
       hSelectedSeperationMuonsEta->Fill(muon_iter->eta());
   }

}

void  EWKTauIDAnalyzer::beginJob(const edm::EventSetup&) { }

void  EWKTauIDAnalyzer::endJob() {
   std::cout << "--------------------------------------------------------------------------------------" << std::endl;
   std::cout << "Number of Events passing the Muon Eta Cut:             " << EventsWithMuonsPassingEtaCut << std::endl;
   std::cout << "Number of Events passing the Muon Pt Cut:              " << EventsWithMuonsPassingPtCut << std::endl;
   std::cout << "Number of Events passing the Tau Eta Cut:              " << EventsWithTausPassingEtaCut << std::endl;
   std::cout << "Number of Events passing the Tau Et Cut:               " << EventsWithTausPassingEtCut << std::endl;
   std::cout << "Number of Events passing the Tau Veto Cut:             " << EventsWithTausPassingSingleJetCut << std::endl;
   std::cout << "Number of Events passing the Muon-Tau seperation Cut:  " << EventsWithMuonsPassingSeperationCut << std::endl;
   std::cout << "--------------------------------------------------------------------------------------" << std::endl;
}
