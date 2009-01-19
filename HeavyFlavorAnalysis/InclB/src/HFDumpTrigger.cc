#include <iostream>

#include "HeavyFlavorAnalysis/InclB/interface/HFDumpTrigger.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Framework/interface/TriggerNames.h"

#include "DataFormats/L1Trigger/interface/L1ParticleMap.h"
#include "DataFormats/HLTReco/interface/HLTFilterObject.h"

#include "FWCore/Framework/interface/ESHandle.h"

#include "DataFormats/TrackReco/interface/Track.h"

#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAna00Event.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaTrack.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaCand.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TGenCand.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaVertex.hh"

#include "DataFormats/Common/interface/RefToBase.h"

#include "DataFormats/RecoCandidate/interface/RecoChargedCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidateFwd.h"


// -- Yikes!
extern TAna00Event *gHFEvent;

using namespace std;
using namespace edm;
using namespace reco;


// ----------------------------------------------------------------------
HFDumpTrigger::HFDumpTrigger(const edm::ParameterSet& iConfig):
  fTriggerLabel(iConfig.getParameter<edm::InputTag>("triggerLabel")),
  fHLTL1SLabel(iConfig.getParameter<edm::InputTag>("HLTL1SLabel")),
  fHLTL1FLabel(iConfig.getParameter<edm::InputTag>("HLTL1FLabel")),
  fHLTL2FLabel(iConfig.getParameter<edm::InputTag>("HLTL2FLabel")), 
  fHLTL2CLabel(iConfig.getParameter<edm::InputTag>("HLTL2CLabel")),
  fHLTL3FLabel(iConfig.getParameter<edm::InputTag>("HLTL3FLabel")), 
  fHLTL3CLabel(iConfig.getParameter<edm::InputTag>("HLTL3CLabel")),
  fL1MuLabel(iConfig.getUntrackedParameter< std::string > ("L1MuLabel")),
  fparticleMap(iConfig.getUntrackedParameter< std::string > ("particleMap")),
  fTriggerName (iConfig.getUntrackedParameter<string>("triggerName",  string("HLT1MuonNonIso"))),
  fL1TriggerName1(iConfig.getUntrackedParameter<string>("L1TriggerName1", string("L1_SingleMu3"))), 
  fL1TriggerName2(iConfig.getUntrackedParameter<string>("L1TriggerName2", string("L1_SingleMu5"))),
  fL1TriggerName3(iConfig.getUntrackedParameter<string>("L1TriggerName3", string("L1_SingleMu7"))), 
  fL1TriggerName4(iConfig.getUntrackedParameter<string>("L1TriggerName4", string("L1_SingleMu10"))),
  fL1TriggerName5(iConfig.getUntrackedParameter<string>("L1TriggerName5", string("L1_SingleMu14"))),
  fL1TriggerName6(iConfig.getUntrackedParameter<string>("L1TriggerName6", string("L1_SingleMu20"))), 
  fL1TriggerName7(iConfig.getUntrackedParameter<string>("L1TriggerName7", string("L1_SingleMu25"))),
  fHLTriggerName1(iConfig.getUntrackedParameter<string>("HLTriggerName1", string("CandHLT1MuonPrescalePt3"))), 
  fHLTriggerName2(iConfig.getUntrackedParameter<string>("HLTriggerName2", string("CandHLT1MuonPrescalePt5"))),
  fHLTriggerName3(iConfig.getUntrackedParameter<string>("HLTriggerName3", string("CandHLT1MuonPrescalePt7x7"))), 
  fHLTriggerName4(iConfig.getUntrackedParameter<string>("HLTriggerName4", string("CandHLT1MuonPrescalePt7x10"))),
  fHLTriggerName5(iConfig.getUntrackedParameter<string>("HLTriggerName5", string("CandHLT1MuonLevel1"))){
  cout << "----------------------------------------------------------------------" << endl;
  cout << "--- HFDumpTrigger constructor" << endl;
  cout << "--- " << fTriggerLabel.encode() << endl;
  cout << "--- " << fHLTL1SLabel.encode() << endl;
  cout << "--- " << fHLTL1FLabel.encode() << endl; 
  cout << "--- " << fHLTL2FLabel.encode() << endl;
  cout << "--- " << fHLTL2CLabel.encode() << endl;
  cout << "--- " << fHLTL3FLabel.encode() << endl;
  cout << "--- " << fHLTL3CLabel.encode() << endl;
  cout << "--- " << fTriggerName.c_str()  << endl;
  cout << "--- " << fHLTriggerName1.c_str() << endl;
  cout << "--- " << fHLTriggerName2.c_str() << endl;
  cout << "--- " << fHLTriggerName3.c_str() << endl;
  cout << "--- " << fHLTriggerName4.c_str() << endl;
  cout << "--- " << fHLTriggerName5.c_str() << endl;
  cout << "--- " << fL1TriggerName1.c_str() << endl;
  cout << "--- " << fL1TriggerName2.c_str() << endl;
  cout << "--- " << fL1TriggerName3.c_str() << endl;
  cout << "--- " << fL1TriggerName4.c_str() << endl;
  cout << "--- " << fL1TriggerName5.c_str() << endl; 
  cout << "--- " << fL1TriggerName6.c_str() << endl;
  cout << "--- " << fL1TriggerName7.c_str() << endl;
  cout << "----------------------------------------------------------------------" << endl; 
  
}


// ----------------------------------------------------------------------
HFDumpTrigger::~HFDumpTrigger() {
  
}


// ----------------------------------------------------------------------
void HFDumpTrigger::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

  nevt++;
    
  gHFEvent->fDiMuonTriggerDecision = 0;
  gHFEvent->fTriggerDecision = 0;
  gHFEvent->fHLT_mu3 = 0;
  gHFEvent->fHLT_mu5 = 0;
  gHFEvent->fHLT_mu7 = 0;
  gHFEvent->fHLT_mu10 = 0; 
  gHFEvent->fHLT_muL1 = 0; 
  gHFEvent->fL1_mu3 = 0;
  gHFEvent->fL1_mu5 = 0;
  gHFEvent->fL1_mu7 = 0;
  gHFEvent->fL1_mu10 = 0;
  gHFEvent->fL1_mu14 = 0;
  gHFEvent->fL1_mu20 = 0;
  gHFEvent->fL1_mu25 = 0;

  // -- L1 trigger   //
  cout << endl << "============================ L1 Trigger ===================================" << endl; 

  edm::Handle<l1extra::L1ParticleMapCollection> l1mapcoll; 
  try {iEvent.getByLabel(fparticleMap,l1mapcoll );} catch (Exception event) { cout << "  -- No L1 Map Collection" << endl;}
  const l1extra::L1ParticleMapCollection& L1MapColl = *l1mapcoll; 

  if (&L1MapColl) {
 
    // Fill L1 trigger bits
    for (int itrig = 0; itrig != l1extra::L1ParticleMap::kNumOfL1TriggerTypes; ++itrig){
      const l1extra::L1ParticleMap& map = ( L1MapColl )[ itrig ] ;
      TString trigName = map.triggerName();
      int l1flag = map.triggerDecision();
     
  
      if ( !strcmp(trigName, fL1TriggerName1.c_str() ) ) {
	cout << itrig << " : " << trigName << " :" << l1flag << endl;
	cout << "!!! changing " << gHFEvent->fL1_mu3;
      	gHFEvent->fL1_mu3 = l1flag;
	cout << " to " << gHFEvent->fL1_mu3 << endl;
      }
      else if ( !strcmp(trigName, fL1TriggerName2.c_str() ) ) { 
	cout << itrig << " : " << trigName << " :" << l1flag << endl;
	cout << "!!! changing " << gHFEvent->fL1_mu5;
      	gHFEvent->fL1_mu5 = l1flag;
	cout << " to " << gHFEvent->fL1_mu5 << endl;
      }
      else if ( !strcmp(trigName, fL1TriggerName3.c_str() ) ) { 
	cout << itrig << " : " << trigName << " :" << l1flag << endl;
	cout << "!!! changing " << gHFEvent->fL1_mu7;
      	gHFEvent->fL1_mu7 = l1flag;
	cout << " to " << gHFEvent->fL1_mu7 << endl;
      }
      else if ( !strcmp(trigName, fL1TriggerName4.c_str() ) ) {
	cout << itrig << " : " << trigName << " :" << l1flag << endl;
	cout << "!!! changing " << gHFEvent->fL1_mu10;
      	gHFEvent->fL1_mu10 = l1flag;
	cout << " to " << gHFEvent->fL1_mu10 << endl;
      }
      else if ( !strcmp(trigName, fL1TriggerName5.c_str() ) ) {
	cout << itrig << " : " << trigName << " :" << l1flag << endl;
	cout << "!!! changing " << gHFEvent->fL1_mu14;
      	gHFEvent->fL1_mu14 = l1flag;
 	cout << " to " << gHFEvent->fL1_mu14 << endl;
      }
      else if ( !strcmp(trigName, fL1TriggerName6.c_str() ) ) { 
	cout << itrig << " : " << trigName << " :" << l1flag << endl;
	cout << "!!! changing " << gHFEvent->fL1_mu20;
      	gHFEvent->fL1_mu20 = l1flag;
	cout << " to " << gHFEvent->fL1_mu20 << endl;
      }
      else if ( !strcmp(trigName, fL1TriggerName7.c_str() ) ) { 
	cout << itrig << " : " << trigName << " :" << l1flag << endl;
	cout << "!!! changing " << gHFEvent->fL1_mu25;
      	gHFEvent->fL1_mu25 = l1flag;
	cout << " to " << gHFEvent->fL1_mu25 << endl;
      }
    }
   
  }
  else {
    std::cout << "%HFDumpTrigger -- No L1 Map Collection" << std::endl;
  }

  edm::Handle<l1extra::L1MuonParticleCollection> l1extmu;
  try {iEvent.getByLabel(fL1MuLabel,l1extmu);} catch (Exception event) { cout << "  -- No L1Mu objects" << endl;}
  const l1extra::L1MuonParticleCollection& L1ExtMu = *l1extmu;
  TL1Muon *pL1Muon; 
 
  if (&L1ExtMu) {
    l1extra::L1MuonParticleCollection myl1mus;
    int il1exmu = 0;
    for (l1extra::L1MuonParticleCollection::const_iterator muItr = L1ExtMu.begin(); muItr != L1ExtMu.end(); ++muItr) {
      cout << "==> L1Muon " << il1exmu << endl;
      cout << "pt " << muItr->pt() << " e " << muItr->energy() << " eta " << muItr->eta() << " phi " << muItr->phi() << endl;
      cout << "iso " << muItr->isIsolated() << " mip " << muItr->isMip() << " forward " << muItr->isForward() << " RPC " << muItr->isRPC() << endl;
          
      L1MuGMTExtendedCand gmtCand = muItr->gmtMuonCand();
      cout << "quality " << gmtCand.quality() << endl;

      pL1Muon = gHFEvent->addL1Muon();
      pL1Muon->fIndex   = il1exmu;
      pL1Muon->fMass    = mMuon;
      pL1Muon->fQ       =  muItr->charge(); 
      pL1Muon->fPlab.SetPtEtaPhi(muItr->pt(),
				 muItr->eta(),
				 muItr->phi()
				 );
 
      pL1Muon->fIsIso     = muItr->isIsolated();
      pL1Muon->fIsMIP     = muItr->isMip();
      pL1Muon->fIsForward = muItr->isForward(); 
      pL1Muon->fIsRPC     = muItr->isRPC();
      pL1Muon->fQuality   = gmtCand.quality();

      cout << "charge " << pL1Muon->fQ << endl;
      
      il1exmu++;
    }
  }
  else {
    std::cout << "%HFDumpTrigger -- No L1 MU object" << std::endl;
  }


  // -- HLT trigger   //
  
  cout << endl << "============================ HLT Trigger ===================================" << endl;
  edm::Handle<edm::TriggerResults> hltresults;
    
  try {
    iEvent.getByLabel(fTriggerLabel,hltresults);
    
  } catch (Exception event) {
    cout << "%HFDumpTrigger -- Couldn't get handle on HLT Trigger!" << endl;
  }
   
  if (!hltresults.isValid()) {
    cout << "%HFDumpTrigger -- No Trigger Result!" << endl;
  } 
  else {
    int ntrigs=hltresults->size();
    if (ntrigs==0){
	cout << "%HFDumpTrigger -- No trigger name given in TriggerResults of the input " << endl;
    } 

    // get hold of trigger names - based on TriggerResults object!
    edm::TriggerNames triggerNames_;
    triggerNames_.init(*hltresults); 
    
    for (int i=0; i< ntrigs; i++) {
           
      if ( !strcmp(triggerNames_.triggerName(i).c_str(), fTriggerName.c_str() ) ) {
	cout << "%HLT-Report --  #" << i << "    "  << triggerNames_.triggerName(i) << " decision " << (*hltresults)[i].accept() << " was run " << (*hltresults)[i].wasrun() << endl;
	cout << "!!! changing " << gHFEvent->fTriggerDecision;
      	gHFEvent->fTriggerDecision = (*hltresults)[i].accept();
	cout << " to " << gHFEvent->fTriggerDecision << endl;
      } 
      else if ( !strcmp(triggerNames_.triggerName(i).c_str(), fHLTriggerName1.c_str() ) ) {
	cout << "%HLT-Report --  #" << i << "    "  << triggerNames_.triggerName(i) << " decision " << (*hltresults)[i].accept() << " was run " << (*hltresults)[i].wasrun() << endl;
	cout << "!!! changing " << gHFEvent->fHLT_mu3;
      	gHFEvent->fHLT_mu3 = (*hltresults)[i].accept();
	cout << " to " << gHFEvent->fHLT_mu3 << endl;
      }
      else if ( !strcmp(triggerNames_.triggerName(i).c_str(), fHLTriggerName2.c_str() ) ) {
	cout << "%HLT-Report --  #" << i << "    "  << triggerNames_.triggerName(i) << " decision " << (*hltresults)[i].accept() << " was run " << (*hltresults)[i].wasrun() << endl;
	cout << "!!! changing " << gHFEvent->fHLT_mu5;
      	gHFEvent->fHLT_mu5 = (*hltresults)[i].accept();
	cout << " to " << gHFEvent->fHLT_mu5 << endl;
      }
      else if ( !strcmp(triggerNames_.triggerName(i).c_str(), fHLTriggerName3.c_str() ) ) { 
	cout << "%HLT-Report --  #" << i << "    "  << triggerNames_.triggerName(i) << " decision " << (*hltresults)[i].accept() << " was run " << (*hltresults)[i].wasrun() << endl;
	cout << "!!! changing " << gHFEvent->fHLT_mu7;
      	gHFEvent->fHLT_mu7 = (*hltresults)[i].accept();
	cout << " to " << gHFEvent->fHLT_mu7 << endl;
      }
      else if ( !strcmp(triggerNames_.triggerName(i).c_str(), fHLTriggerName4.c_str() ) ) { 
	cout << "%HLT-Report --  #" << i << "    "  << triggerNames_.triggerName(i) << " decision " << (*hltresults)[i].accept() << " was run " << (*hltresults)[i].wasrun() << endl;
	cout << "!!! changing " << gHFEvent->fHLT_mu10;
      	gHFEvent->fHLT_mu10 = (*hltresults)[i].accept();
	cout << " to " << gHFEvent->fHLT_mu10 << endl;
      }
      else if ( !strcmp(triggerNames_.triggerName(i).c_str(), fHLTriggerName5.c_str() ) ) { 
	cout << "%HLT-Report --  #" << i << "    "  << triggerNames_.triggerName(i) << " decision " << (*hltresults)[i].accept() << " was run " << (*hltresults)[i].wasrun() << endl;
	cout << "!!! changing " << gHFEvent->fHLT_muL1;
      	gHFEvent->fHLT_muL1 = (*hltresults)[i].accept();
	cout << " to " << gHFEvent->fHLT_muL1 << endl;
      }
    } 
   
  }

  /////////////////////////////////////////////////
  THLTMuon *pHLTMuon; 
  
  // get hold of filtered candidates
  edm::Handle<reco::HLTFilterObjectWithRefs> filtercands0;
  try {
    //iEvent.getByLabel ("SingleMuNoIsoLevel1Seed",filtercands0);
    iEvent.getByLabel (fHLTL1SLabel,filtercands0); 
   
    for (unsigned int i=0; i<filtercands0->size(); i++) {
      float pt= filtercands0->getParticleRef(i).get()->pt();
      float phi= filtercands0->getParticleRef(i).get()->phi();
      float eta= filtercands0->getParticleRef(i).get()->eta();
      cout << endl << "==>SingleMuNoIsoLevel1Seed:    Muon " << i << ": pt " << pt << " phi " << phi << " eta " << eta << endl; 

      pHLTMuon = gHFEvent->addHLTMuonL1S();
      pHLTMuon->fIndex   = i;
      pHLTMuon->fMass    = mMuon;
      pHLTMuon->fQ       = filtercands0->getParticleRef(i).get()->charge(); 
      pHLTMuon->fPlab.SetPtEtaPhi(pt,eta,phi );
      pHLTMuon->fPlabTrack.SetPtEtaPhi(-9999,-9999,-9999);
      pHLTMuon->fLip        = -9999;
      pHLTMuon->fLipE       = -9999;
      pHLTMuon->fTip        = -9999;
      pHLTMuon->fTipE       = -9999;     
      pHLTMuon->fDof        = -9999;
      pHLTMuon->fHits       = -9999;
      pHLTMuon->fChi2       = -9999; 
      pHLTMuon->fParameter0 = -9999;
      pHLTMuon->fError0     = -9999; 
     
    }
    cout << gHFEvent->nHLTMuonsL1S() << " L1 SEED MUONS IN THE EVENT" << endl;
  
  } 
  catch (Exception event) {
    cout << "  -- No HLT SingleMuNoIsoLevel1Seed Collection" << endl;
  }

  edm::Handle<reco::HLTFilterObjectWithRefs> filtercands1;
  try {
    //iEvent.getByLabel ("SingleMuNoIsoL1Filtered",filtercands1); 
    iEvent.getByLabel (fHLTL1FLabel,filtercands1); 
    for (unsigned int i=0; i<filtercands1->size(); i++) {
      float pt= filtercands1->getParticleRef(i).get()->pt();
      float phi= filtercands1->getParticleRef(i).get()->phi();
      float eta= filtercands1->getParticleRef(i).get()->eta();
      cout << endl << "==>SingleMuNoIsoL1Filtered:   Muon " << i << ": pt " << pt << " phi " << phi << " eta " << eta << endl;
    
      pHLTMuon = gHFEvent->addHLTMuonL1F();
      pHLTMuon->fIndex   = i;
      pHLTMuon->fMass    = mMuon;
      pHLTMuon->fQ       = filtercands1->getParticleRef(i).get()->charge(); 
      pHLTMuon->fPlab.SetPtEtaPhi(pt,eta,phi);
      pHLTMuon->fPlabTrack.SetPtEtaPhi(-9999,-9999,-9999);
      pHLTMuon->fLip        = -9999;
      pHLTMuon->fLipE       = -9999;
      pHLTMuon->fTip        = -9999;
      pHLTMuon->fTipE       = -9999;     
      pHLTMuon->fDof        = -9999;
      pHLTMuon->fHits       = -9999;
      pHLTMuon->fChi2       = -9999; 
      pHLTMuon->fParameter0 = -9999;
      pHLTMuon->fError0     = -9999; 
    
    
    }
    cout << gHFEvent->nHLTMuonsL1F() << " L1 FILTERED MUONS IN THE EVENT" << endl;
  } 
  catch (Exception event) {
    cout << "  -- No HLT SingleMuNoIsoL1Filtered Collection" << endl;
  }

  edm::Handle<reco::HLTFilterObjectWithRefs> filtercands2;
  try {
    //iEvent.getByLabel ("SingleMuNoIsoL2PreFiltered",filtercands2); 
    iEvent.getByLabel (fHLTL2FLabel,filtercands2); 
    for (unsigned int i=0; i<filtercands2->size(); i++) {
      float pt= filtercands2->getParticleRef(i).get()->pt();
      float phi= filtercands2->getParticleRef(i).get()->phi();
      float eta= filtercands2->getParticleRef(i).get()->eta();
      cout << endl << "==>SingleMuNoIsoL2PreFiltered: Muon " << i << ": pt " << pt << " phi " << phi << " eta " << eta << endl; 

      pHLTMuon = gHFEvent->addHLTMuonL2F();
      pHLTMuon->fIndex   = i;
      pHLTMuon->fMass    = mMuon;
      pHLTMuon->fPlab.SetPtEtaPhi(pt,eta,phi);
   
      pHLTMuon->fPlabTrack.SetPtEtaPhi(-9999,-9999,-9999);
      pHLTMuon->fQ          = -9999;
      pHLTMuon->fLip        = -9999;
      pHLTMuon->fLipE       = -9999;
      pHLTMuon->fTip        = -9999;
      pHLTMuon->fTipE       = -9999;     
      pHLTMuon->fDof        = -9999;
      pHLTMuon->fHits       = -9999;
      pHLTMuon->fChi2       = -9999; 
      pHLTMuon->fParameter0 = -9999;
      pHLTMuon->fError0     = -9999;
      try {
	TrackRef tk = filtercands2->getParticleRef(i)->get<TrackRef>(); 
     
	pHLTMuon->fPlabTrack.SetPtEtaPhi(tk->pt(),tk->eta(),tk->phi());
	pHLTMuon->fQ          = tk->charge();
	pHLTMuon->fLip        = tk->dz();
	pHLTMuon->fLipE       = tk->dzError();
	pHLTMuon->fTip        = tk->d0();
	pHLTMuon->fTipE       = tk->d0Error();     
	pHLTMuon->fDof        = int(tk->ndof());
	pHLTMuon->fHits       = tk->numberOfValidHits();
	pHLTMuon->fChi2       = tk->chi2(); 
	pHLTMuon->fParameter0 = tk->parameter(0);
	pHLTMuon->fError0     = tk->error(0);
      }
      catch (Exception event) {
	cout << "==>SingleMuNoIsoL2PreFiltered: no track collection " << endl;
      }
     
    }
    cout << gHFEvent->nHLTMuonsL2F() << " L2 FILTERED MUONS IN THE EVENT" << endl;
  }
  catch (Exception event) {
    cout << "  -- No HLT SingleMuNoIsoL2PreFiltered Collection" << endl;
  }

  edm::Handle<reco::HLTFilterObjectWithRefs> filtercands3;
  try {
    //iEvent.getByLabel ("SingleMuNoIsoL3PreFiltered",filtercands3);
    iEvent.getByLabel (fHLTL3FLabel,filtercands3); 

    for (unsigned int i=0; i<filtercands3->size(); i++) {
      float pt= filtercands3->getParticleRef(i).get()->pt();
      float phi= filtercands3->getParticleRef(i).get()->phi();
      float eta= filtercands3->getParticleRef(i).get()->eta();
      cout << endl << "==>SingleMuNoIsoL3PreFiltered: Muon " << i << ": pt " << pt << " phi " << phi << " eta " << eta << endl;

      pHLTMuon = gHFEvent->addHLTMuonL3F();
      pHLTMuon->fIndex   = i;
      pHLTMuon->fMass    = mMuon;
      pHLTMuon->fPlab.SetPtEtaPhi(pt,eta,phi);

      pHLTMuon->fPlabTrack.SetPtEtaPhi(-9999,-9999,-9999);
      pHLTMuon->fQ          = -9999;
      pHLTMuon->fLip        = -9999;
      pHLTMuon->fLipE       = -9999;
      pHLTMuon->fTip        = -9999;
      pHLTMuon->fTipE       = -9999;     
      pHLTMuon->fDof        = -9999;
      pHLTMuon->fHits       = -9999;
      pHLTMuon->fChi2       = -9999; 
      pHLTMuon->fParameter0 = -9999;
      pHLTMuon->fError0     = -9999;
      try {
	TrackRef tk = filtercands3->getParticleRef(i)->get<TrackRef>(); 
     
	pHLTMuon->fPlabTrack.SetPtEtaPhi(tk->pt(),tk->eta(),tk->phi());
	pHLTMuon->fQ          = tk->charge();
	pHLTMuon->fLip        = tk->dz();
	pHLTMuon->fLipE       = tk->dzError();
	pHLTMuon->fTip        = tk->d0();
	pHLTMuon->fTipE       = tk->d0Error();     
	pHLTMuon->fDof        = int(tk->ndof());
	pHLTMuon->fHits       = tk->numberOfValidHits();
	pHLTMuon->fChi2       = tk->chi2(); 
	pHLTMuon->fParameter0 = tk->parameter(0);
	pHLTMuon->fError0     = tk->error(0);
     
      }
      catch (Exception event) {
	cout << "==>NEW SingleMuNoIsoL3PreFiltered: no track collection " << endl;
      }
     
    }
    cout << gHFEvent->nHLTMuonsL3F() << " L3 FILTERED MUONS IN THE EVENT" << endl;
  }
  catch (Exception event) {
    cout << "  -- No HLT SingleMuNoIsoL3PreFiltered Collection" << endl;
  }
  
  ////////////////////////////////////////////////////////////////////////////// 
  edm::Handle<RecoChargedCandidateCollection> mucandsL2;
  int index=0;
  try {
    //iEvent.getByLabel ("hltL2MuonCandidates",mucandsL2);
    iEvent.getByLabel (fHLTL2CLabel,mucandsL2);  
    RecoChargedCandidateCollection::const_iterator cand;
    for (cand=mucandsL2->begin(); cand!=mucandsL2->end(); cand++) {
      TrackRef tk = cand->get<TrackRef>(); 
      cout << endl << "==>L2Candidate: Muon : pt " << tk->pt() << " phi " << tk->phi() << " eta " << tk->eta() << endl;
      pHLTMuon = gHFEvent->addHLTMuonL2C();
      pHLTMuon->fIndex   = index;
      pHLTMuon->fMass    = mMuon;
    
      pHLTMuon->fPlab.SetPtEtaPhi(tk->pt(),tk->eta(),tk->phi());
      pHLTMuon->fPlabTrack.SetPtEtaPhi(tk->pt(),tk->eta(),tk->phi());
      pHLTMuon->fQ          = tk->charge();
      pHLTMuon->fLip        = tk->dz();
      pHLTMuon->fLipE       = tk->dzError();
      pHLTMuon->fTip        = tk->d0();
      pHLTMuon->fTipE       = tk->d0Error();     
      pHLTMuon->fDof        = int(tk->ndof());
      pHLTMuon->fHits       = tk->numberOfValidHits();
      pHLTMuon->fChi2       = tk->chi2(); 
      pHLTMuon->fParameter0 = tk->parameter(0);
      pHLTMuon->fError0     = tk->error(0);
      
      index++;
     
    }
    cout << gHFEvent->nHLTMuonsL2C() << " L2 MUON CANDIDATES IN THE EVENT" << endl;
  
  } 
  catch (Exception event) {
    cout << "  -- No RecoChargedCandidate Collection L2" << endl;
  }
  
  edm::Handle<RecoChargedCandidateCollection> mucandsL3;
  index=0;
  try {
    //iEvent.getByLabel ("hltL3MuonCandidates",mucandsL3);
    iEvent.getByLabel (fHLTL3CLabel,mucandsL3); 
    RecoChargedCandidateCollection::const_iterator cand;
    for (cand=mucandsL3->begin(); cand!=mucandsL3->end(); cand++) {
      TrackRef tk = cand->get<TrackRef>(); 
      cout << endl << "==>L3Candidate: Muon : pt " << tk->pt() << " phi " << tk->phi() << " eta " << tk->eta() << endl;
      pHLTMuon = gHFEvent->addHLTMuonL3C();
      pHLTMuon->fIndex   = index;
      pHLTMuon->fMass    = mMuon;
    
      pHLTMuon->fPlab.SetPtEtaPhi(tk->pt(),tk->eta(),tk->phi());
      pHLTMuon->fPlabTrack.SetPtEtaPhi(tk->pt(),tk->eta(),tk->phi());
      pHLTMuon->fQ          = tk->charge();
      pHLTMuon->fLip        = tk->dz();
      pHLTMuon->fLipE       = tk->dzError();
      pHLTMuon->fTip        = tk->d0();
      pHLTMuon->fTipE       = tk->d0Error();     
      pHLTMuon->fDof        = int(tk->ndof());
      pHLTMuon->fHits       = tk->numberOfValidHits();
      pHLTMuon->fChi2       = tk->chi2(); 
      pHLTMuon->fParameter0 = tk->parameter(0);
      pHLTMuon->fError0     = tk->error(0);
      
      index++;
     
    }
    cout << gHFEvent->nHLTMuonsL3C() << " L3 MUON CANDIDATES IN THE EVENT" << endl;
  
  } 
  catch (Exception event) {
    cout << "  -- No RecoChargedCandidate Collection L3" << endl;
  }
  /////////////////////////////

}

// ------------ method called once each job just before starting event loop  ------------
void  HFDumpTrigger::beginJob(const edm::EventSetup& setup) {

  nevt=0;
  
}

// ------------ method called once each job just after ending the event loop  ------------
void  HFDumpTrigger::endJob() {

  cout << "HFDumpTrigger> Summary: Events processed: " << nevt << endl;
}

//define this as a plug-in
//DEFINE_FWK_MODULE(HFDumpTrigger);
