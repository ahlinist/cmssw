// Class:      L25TauEfficiencyAnalyzer
// Original Author:  Eduardo Luiggi, modified by Sho Maruyama
//         Created:  Fri Apr  4 16:37:44 CDT 2008
// $Id: L25TauEfficiencyAnalyzer.cc,v 1.14 2008/11/07 14:43:31 smaruyam Exp $
#include "ElectroWeakAnalysis/TauTriggerEfficiency/interface/L25TauEfficiencyAnalyzer.h"
using namespace edm;
using namespace reco;
using namespace std;
L25TauEfficiencyAnalyzer::L25TauEfficiencyAnalyzer(){}

L25TauEfficiencyAnalyzer::L25TauEfficiencyAnalyzer(const edm::ParameterSet& iConfig){
}

L25TauEfficiencyAnalyzer::~L25TauEfficiencyAnalyzer(){}

void L25TauEfficiencyAnalyzer::Setup(const edm::ParameterSet& iConfig,TTree* l25tree){
  l25JetSource = iConfig.getParameter<InputTag>("l25JetSource");
  l25PtCutSource = iConfig.getParameter<InputTag>("l25PtCutSource");
  l25IsoSource = iConfig.getParameter<InputTag>("l25IsoSource");
  l25tauSource = iConfig.getParameter<InputTag>("PFTauCollection");
  l25MatchingCone = iConfig.getParameter<double>("l25MatchingCone");
  //rootFile_ = (iConfig.getParameter<std::string>("outputFileName")),
  //l25file = new TFile(rootFile_.c_str(),"recreate");
  //l25tree = new TTree("l25tree","Level 2.5 Tau Tree");
  l25tree->Branch("tauEta", &tauEta,"tauEta/F" );
  l25tree->Branch("tauPhi", &tauPhi,"tauPhi/F" );
  l25tree->Branch("tauEt", &tauEt,"tauEt/F" );
  l25tree->Branch("tauPt", &tauPt,"tauPt/F" );
  l25tree->Branch("tauTjDR", &tauTjDR, "tauTjDR/F" );
  l25tree->Branch("tauTrkSig", &tauTrkSig, "tauTrkSig/F" );
  l25tree->Branch("l25Eta", &l25Eta,"l25Eta/F" );
  l25tree->Branch("l25Phi", &l25Phi,"l25Phi/F" );
  l25tree->Branch("l25Et", &l25Et,"l25Et/F" );
  l25tree->Branch("l25Pt", &l25Pt,"l25Pt/F" );
  l25tree->Branch("l25TjDR", &l25TjDR, "l25TjDR/F" );
  l25tree->Branch("l25TrkQPx", &l25TrkQPx, "l25TrkQPx/F" );
  l25tree->Branch("l25Depth", &l25Depth,"l25Depth/I" );
  l25tree->Branch("leadDepth1", &leadDepth1,"leadDepth1/I" );
  l25tree->Branch("leadDepth2", &leadDepth2,"leadDepth2/I" );
  l25tree->Branch("minDR", &minDR,"minDR/F" );
  l25tree->Branch("bareEt", &bareEt,"bareEt/F" );
}

void L25TauEfficiencyAnalyzer::fill(const edm::Event& iEvent, const reco::CaloTau& tau) {
// no offline track related quantity(which is the main interest of L25) is filled here.
Handle<IsolatedTauTagInfoCollection> tags;
iEvent.getByLabel(l25JetSource, tags);
Handle<CaloJetCollection> ptJets;
iEvent.getByLabel(l25PtCutSource, ptJets);
Handle<CaloJetCollection> isoJets;
iEvent.getByLabel(l25IsoSource, isoJets);
l25Et = 0;
l25Phi = 0;
l25Eta = 0;
l25Pt = 0;
l25TjDR = 0;
l25TrkQPx = 0;
l25Depth = 0;
if(&(*tags)){
for(unsigned int j = 0; j < tags->size(); j++){ // bare L2 Taus
if(deltaR(tau, *(tags->at(j).jet())) < l25MatchingCone){ // dr < l25MatchingCone
if(l25Depth < 1) l25Depth = 1; // L2 match
l25TrkQPx  = ( tags->at(j).selectedTracks().size() );
l25Eta  = (tags->at(j).jet()->eta());
l25Phi  = (tags->at(j).jet()->phi());
l25Et   = (tags->at(j).jet()->et());
}// calo and l25 tau match dr < l25MatchingCone
}// for jet loop
}// non empty collection

if(&(*ptJets)){ // Leading Pt Cut > 3 GeV/c applied
for(unsigned int j = 0; j < ptJets->size(); j++){
if(deltaR(tau, ptJets->at(j) ) < l25MatchingCone){ // dr < l25MatchingCone
if(l25Depth < 2){
l25Depth = 2; // lead pt cut match
const TrackRef leadTrk = tags->at(j).leadingSignalTrack(0.1,1.0);// track finding 
if(leadTrk.isNonnull()){                                                        
l25Pt  =  (leadTrk->pt() );
l25TjDR  = ( deltaR( *(tags->at(j).jet()), *leadTrk) );
}// good lead cand
}
}// pf and l25 tau match dr < l25MatchingCone
}// for jet loop
}// non empty collection

if(&(*isoJets)){
for(unsigned int j = 0; j < isoJets->size(); j++){
if(deltaR(tau, isoJets->at(j)) < l25MatchingCone){ // dr < l25MatchingCone
if(l25Depth < 3) l25Depth = 3; // iso match
}
}
}
//l25tree -> Fill();
}// calo tau ends

void L25TauEfficiencyAnalyzer::fill(const edm::Event&iEvent,const reco::PFTau& tau){
Handle<IsolatedTauTagInfoCollection> tags;
iEvent.getByLabel(l25JetSource, tags);
Handle<CaloJetCollection> ptJets;
iEvent.getByLabel(l25PtCutSource, ptJets);
Handle<CaloJetCollection> isoJets;
iEvent.getByLabel(l25IsoSource, isoJets);
tauPt = 0;
tauTjDR = 0;
l25Et = 0;
l25Phi = 0;
l25Eta = 0;
l25Pt = 0;
l25TjDR = 0;
l25TrkQPx = 0;
l25Depth = 0;
const TrackRef leadPFTrk = tau.leadPFChargedHadrCand()->trackRef();
if(&(*tags)){
for(unsigned int j = 0; j < tags->size(); j++){ // bare L2 Taus
if(deltaR(tau, *(tags->at(j).jet())) < l25MatchingCone){ // dr < l25MatchingCone
if(l25Depth < 1) l25Depth = 1; // L2 match
if(leadPFTrk.isNonnull()){
tauPt  = (leadPFTrk->pt() );
tauTjDR  = ( deltaR( tau, *leadPFTrk) );
}
l25TrkQPx  = ( tags->at(j).selectedTracks().size() );
l25Eta  = (tags->at(j).jet()->eta());
l25Phi  = (tags->at(j).jet()->phi());
l25Et   = (tags->at(j).jet()->et());
}// pf and l25 tau match dr < l25MatchingCone
}// for jet loop
}// non empty collection

if(&(*ptJets)){ // Leading Pt Cut > 3 GeV/c applied
for(unsigned int j = 0; j < ptJets->size(); j++){
if(deltaR(tau, ptJets->at(j) ) < l25MatchingCone){ // dr < l25MatchingCone
if(l25Depth < 2){
l25Depth = 2; // lead pt cut match
const TrackRef leadTrk = tags->at(j).leadingSignalTrack(0.1,1.0);// track finding 
if(leadTrk.isNonnull()){                                                        
l25Pt  =  (leadTrk->pt() );
l25TjDR  = ( deltaR( *(tags->at(j).jet()), *leadTrk) );
}// good lead cand
}
}// pf and l25 tau match dr < l25MatchingCone
}// for jet loop
}// non empty collection

if(&(*isoJets)){
for(unsigned int j = 0; j < isoJets->size(); j++){
if(deltaR(tau, isoJets->at(j)) < l25MatchingCone){ // dr < l25MatchingCone
if(l25Depth < 3) l25Depth = 3; // iso match
}// pf and l25 tau match dr < l25MatchingCone
}// for jet loop
}// non empty collection
}

void L25TauEfficiencyAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
Handle<PFTauCollection> taus;
iEvent.getByLabel(l25tauSource, taus);
Handle<IsolatedTauTagInfoCollection> tags;
iEvent.getByLabel(l25JetSource, tags);
Handle<CaloJetCollection> ptJets;
iEvent.getByLabel(l25PtCutSource, ptJets);
Handle<CaloJetCollection> isoJets;
iEvent.getByLabel(l25IsoSource, isoJets);
vector<double> drs;
for(unsigned int i = 0; i < taus->size(); i++){
l25Depth = 0; // no match
tauPt = 0;
tauEt = 0;
tauEta = 0;
tauPhi = 0;
tauTjDR = 0;
tauTrkSig = 0;
l25Et = 0;
l25Phi = 0;
l25Eta = 0;
l25Pt = 0;
l25TjDR = 0;
l25TrkQPx = 0;
bareEt = 0;
leadDepth1 = 0;
leadDepth2 = 0;
minDR = -1;
drs.clear();
bareEt = taus->at(i).et();
const TrackRef leadPFTrk = taus->at(i).leadPFChargedHadrCand()->trackRef();
if(&(*tags)){
for(unsigned int j = 0; j < tags->size(); j++){ // bare L2 Taus
drs.push_back(deltaR(taus->at(i), *(tags->at(j).jet())));
if(deltaR(taus->at(i), *(tags->at(j).jet())) < l25MatchingCone){ // dr < l25MatchingCone
if(l25Depth < 1) l25Depth = 1; // L2 match
tauTrkSig  = ( taus->at(i).signalPFChargedHadrCands().size() );
tauEt   = ( taus->at(i).et()  ); 			         
tauEta  = ( taus->at(i).eta() );			         
tauPhi  = ( taus->at(i).phi() );
tauPt  = (leadPFTrk->pt() );			      	      
tauTjDR  = ( deltaR( taus->at(i), *leadPFTrk) );
l25TrkQPx  = ( tags->at(j).selectedTracks().size() );
l25Eta  = (tags->at(j).jet()->eta());
l25Phi  = (tags->at(j).jet()->phi());
l25Et   = (tags->at(j).jet()->et());
const TrackRef leadTrk = tags->at(j).leadingSignalTrack(0.1,1.0);// track finding 
const TrackRef leadTrk21 = tags->at(j).leadingSignalTrack(0.2,1.0);// track finding 
const TrackRef leadTrk23 = tags->at(j).leadingSignalTrack(0.2,3.0);// track finding 
if(leadTrk21.isNonnull() && leadDepth2 < 1) leadDepth2 = 1;
if(leadTrk23.isNonnull() && leadDepth2 < 2) leadDepth2 = 2;
if(leadTrk.isNonnull() && leadDepth1 < 1){                                                        
leadDepth1 = 1;
l25Pt  =  (leadTrk->pt() );
l25TjDR  = ( deltaR( *(tags->at(j).jet()), *leadTrk) );
}// good lead cand
}// pf and l25 tau match dr < l25MatchingCone
if(drs.size() > 0){
if(drs.size() > 1) sort(drs.begin(),drs.end());
minDR = drs.at(0);
}// sort drs
}// for jet loop
}// non empty collection

if(&(*ptJets)){ // Leading Pt Cut > 3 GeV/c applied
for(unsigned int j = 0; j < ptJets->size(); j++){
if(deltaR(taus->at(i), ptJets->at(j) ) < l25MatchingCone){ // dr < l25MatchingCone
if(l25Depth < 2) l25Depth = 2; // lead pt cut match
}// pf and l25 tau match dr < l25MatchingCone
}// for jet loop
}// non empty collection

if(&(*isoJets)){
for(unsigned int j = 0; j < isoJets->size(); j++){
if(deltaR(taus->at(i), isoJets->at(j)) < l25MatchingCone){ // dr < l25MatchingCone
if(l25Depth < 3) l25Depth = 3; // iso match
}// pf and l25 tau match dr < l25MatchingCone
}// for jet loop
}// non empty collection
//l25tree -> Fill();
}// for tau loop
}// analyzer ends here

void L25TauEfficiencyAnalyzer::beginJob(const edm::EventSetup&) {}
void L25TauEfficiencyAnalyzer::endJob() {
  //l25file->Write();
}
