// Class:      L25TauEfficiencyAnalyzer
// Original Author:  Eduardo Luiggi, modified by Sho Maruyama
//         Created:  Fri Apr  4 16:37:44 CDT 2008
// $Id: L25TauEfficiencyAnalyzer.cc,v 1.2 2008/09/30 11:18:50 smaruyam Exp $
#include "ElectroWeakAnalysis/TauTriggerEfficiency/interface/L25TauEfficiencyAnalyzer.h"
using namespace edm;
using namespace reco;
using namespace std;
L25TauEfficiencyAnalyzer::L25TauEfficiencyAnalyzer(const edm::ParameterSet& iConfig){
  l25JetSource = iConfig.getParameter<InputTag>("l25JetSource");
  l25PtCutSource = iConfig.getParameter<InputTag>("l25PtCutSource");
  l25IsoSource = iConfig.getParameter<InputTag>("l25IsoSource");
    tauSource = iConfig.getParameter<InputTag>("tauSource");
  matchingCone = iConfig.getParameter<double>("matchingCone");
  rootFile_ = (iConfig.getParameter<std::string>("outputFileName")),
  l25file = new TFile(rootFile_.c_str(),"recreate");
  l25tree = new TTree("l25tree","Level 2.5 Tau Tree");
  l25tree->Branch("tauEta", &tauEta,"tauEta/F" );
  l25tree->Branch("tauPhi", &tauPhi,"tauPhi/F" );
  l25tree->Branch("tauEt", &tauEt,"tauEt/F" );
  l25tree->Branch("tauPt", &tauPt,"tauPt/F" );
  l25tree->Branch("tauInvPt", &tauInvPt,"tauInvPt/F" );
  l25tree->Branch("tauInvPt1", &tauInvPt1,"tauInvPt1/F" );
  l25tree->Branch("tauInvPt3", &tauInvPt3,"tauInvPt3/F" );
  l25tree->Branch("tauInvPtm", &tauInvPtm,"tauInvPtm/F" );
  l25tree->Branch("tauInvPtm1", &tauInvPtm1,"tauInvPtm1/F" );
  l25tree->Branch("tauInvPtm3", &tauInvPtm3,"tauInvPtm3/F" );
  l25tree->Branch("tauTjDR", &tauTjDR, "tauTjDR/F" );
  l25tree->Branch("tauTrkC05", &tauTrkC05, "tauTrkC05/F" );
  l25tree->Branch("tauTrkSig", &tauTrkSig, "tauTrkSig/F" );
  l25tree->Branch("l25Eta", &l25Eta,"l25Eta/F" );
  l25tree->Branch("l25Phi", &l25Phi,"l25Phi/F" );
  l25tree->Branch("l25Et", &l25Et,"l25Et/F" );
  l25tree->Branch("l25Pt", &l25Pt,"l25Pt/F" );
  l25tree->Branch("l25InvPt", &l25InvPt,"l25InvPt/F" );
  l25tree->Branch("l25InvPt1", &l25InvPt1,"l25InvPt1/F" );
  l25tree->Branch("l25InvPt3", &l25InvPt3,"l25InvPt3/F" );
  l25tree->Branch("l25TjDR", &l25TjDR, "l25TjDR/F" );
  l25tree->Branch("l25TrkQPx", &l25TrkQPx, "l25TrkQPx/F" );
  l25tree->Branch("l25PtCut", &l25PtCut,"l25PtCut/F" );
  l25tree->Branch("l25Iso", &l25Iso,"l25Iso/F" );
}

L25TauEfficiencyAnalyzer::~L25TauEfficiencyAnalyzer(){
}

void L25TauEfficiencyAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
Handle<PFTauCollection> taus;
iEvent.getByLabel(tauSource, taus);
Handle<IsolatedTauTagInfoCollection> tags;
iEvent.getByLabel(l25JetSource, tags);
Handle<CaloJetCollection> ptJets;
iEvent.getByLabel(l25PtCutSource, ptJets);
Handle<CaloJetCollection> isoJets;
iEvent.getByLabel(l25IsoSource, isoJets);
for(unsigned int i = 0; i < taus->size(); i++){
const TrackRef leadPFTrk = taus->at(i).leadPFChargedHadrCand()->trackRef();
if(&(*tags)){
for(unsigned int j = 0; j < tags->size(); j++){ // bare L2.5 Taus
if(deltaR(taus->at(i), *(tags->at(j).jet())) < matchingCone){ // dr < matchingCone
tauTrkC05  = ( taus->at(i).pfTauTagInfoRef()->PFChargedHadrCands().size() );
tauTrkSig  = ( taus->at(i).signalPFChargedHadrCands().size() );
tauEt   = ( taus->at(i).et()  ); 			         
tauEta  = ( taus->at(i).eta() );			         
tauPhi  = ( taus->at(i).phi() );
tauPt  = (leadPFTrk->pt() );			      	      
tauInvPt  = (1.0/leadPFTrk->pt() );			      	      
if(taus->at(i).signalPFChargedHadrCands().size() == 1) tauInvPt1  = (1.0/leadPFTrk->pt() );
if(taus->at(i).signalPFChargedHadrCands().size() == 3) tauInvPt3  = (1.0/leadPFTrk->pt() );
tauTjDR  = ( deltaR( taus->at(i), *leadPFTrk) );
l25TrkQPx  = ( tags->at(j).selectedTracks().size() );
l25Eta  = (tags->at(j).jet()->eta());
l25Phi  = (tags->at(j).jet()->phi());
l25Et   = (tags->at(j).jet()->et());
const TrackRef leadTrk = tags->at(j).leadingSignalTrack(0.1,1.0);// track finding 
const TrackRef leadTrk02 = tags->at(j).leadingSignalTrack(0.2,1.0);// track finding 
const TrackRef leadTrk23 = tags->at(j).leadingSignalTrack(0.2,3.0);// track finding 
if(leadTrk.isNonnull() ){                                                        
l25Pt  =  (leadTrk->pt() );
l25TjDR  = ( deltaR( *(tags->at(j).jet()), *leadTrk) );
tauInvPtm  = (1.0/leadPFTrk->pt() );			      	      
if(taus->at(i).signalPFChargedHadrCands().size() == 1) tauInvPtm1  = (1.0/leadPFTrk->pt() );
if(taus->at(i).signalPFChargedHadrCands().size() == 3) tauInvPtm3  = (1.0/leadPFTrk->pt() );
}// good lead cand
}// pf and l25 tau match dr < matchingCone
}// for jet loop
}// non empty collection

if(&(*ptJets)){ // Leading Pt Cut > 3 GeV/c applied
for(unsigned int j = 0; j < ptJets->size(); j++){
if(deltaR(taus->at(i), ptJets->at(j) ) < matchingCone){ // dr < matchingCone
l25InvPt  =  (1.0/leadPFTrk->pt() );
if(taus->at(i).signalPFChargedHadrCands().size() == 1) l25InvPt1  =  (1.0/leadPFTrk->pt() );
if(taus->at(i).signalPFChargedHadrCands().size() == 3) l25InvPt3  =  (1.0/leadPFTrk->pt() );
l25PtCut  = (taus->at(i).et() );
}// pf and l25 tau match dr < matchingCone
}// for jet loop
}// non empty collection

if(&(*isoJets)){
for(unsigned int j = 0; j < isoJets->size(); j++){
if(deltaR(taus->at(i), isoJets->at(j)) < matchingCone){ // dr < matchingCone
l25Iso  = (taus->at(i).et() );
}// pf and l25 tau match dr < matchingCone
}// for jet loop
}// non empty collection

}// for tau loop

}// analyzer ends here
void L25TauEfficiencyAnalyzer::beginJob(const edm::EventSetup&) {}
void L25TauEfficiencyAnalyzer::endJob() {}
