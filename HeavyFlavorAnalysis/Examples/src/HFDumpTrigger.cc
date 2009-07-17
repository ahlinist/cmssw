#include <iostream>
#include <bitset>

#include "HeavyFlavorAnalysis/Examples/interface/HFDumpTrigger.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/TriggerNames.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/L1Trigger/interface/L1MuonParticle.h"
#include "DataFormats/L1Trigger/interface/L1MuonParticleFwd.h"
#include "DataFormats/L1Trigger/interface/L1ParticleMap.h"
#include "DataFormats/L1Trigger/interface/L1ParticleMapFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapRecord.h"
#include "DataFormats/HLTReco/interface/TriggerEventWithRefs.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/HLTReco/interface/TriggerFilterObjectWithRefs.h"
#include "DataFormats/HLTReco/interface/TriggerTypeDefs.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"

#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonDetId/interface/MuonSubdetId.h"

#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAna01Event.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaTrack.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaCand.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TGenCand.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaVertex.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaMuon.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TTrgObj.hh"

// -- Yikes!
extern TAna01Event *gHFEvent;

using namespace std;
using namespace edm;
using namespace reco;
using namespace trigger;


// ----------------------------------------------------------------------
HFDumpTrigger::HFDumpTrigger(const edm::ParameterSet& iConfig):
  fVerbose(iConfig.getUntrackedParameter<int>("verbose", 0)),
  fparticleMap(iConfig.getUntrackedParameter< std::string > ("particleMap")),
  fL1GTReadoutRecordLabel(iConfig.getUntrackedParameter<std::string> ("L1GTReadoutRecordLabel")),
  fL1GTmapLabel(iConfig.getUntrackedParameter<InputTag> ("hltL1GtObjectMap")),
  fL1MuonsLabel(iConfig.getUntrackedParameter<InputTag> ("L1MuonsLabel")),
  fTriggerEventLabel(iConfig.getUntrackedParameter<InputTag> ("TriggerEventLabel")),
  fHLTResultsLabel(iConfig.getUntrackedParameter<InputTag> ("HLTResultsLabel")),

  fL1TriggerName(iConfig.getUntrackedParameter<string>("L1TriggerName", string("L1_DoubleMu3"))),
  fHLTriggerLabel(iConfig.getUntrackedParameter<edm::InputTag>("hltLabel")),
  fHLTriggerName(iConfig.getUntrackedParameter<string>("HLTriggerName",  string("HLTBJPsiMuMu"))),
  fHLTFilterObject0(iConfig.getUntrackedParameter<string>("HLTfiltObj0",  string("DiMuonNoIsoLevel1Seed"))),
  fHLTFilterObject1(iConfig.getUntrackedParameter<string>("HLTfiltObj1",  string("DiMuonNoIsoLevel1Filtered"))),
  fHLTFilterObject2(iConfig.getUntrackedParameter<string>("HLTfiltObj2",  string("JpsitoMumuL1Seed"))),
  fHLTFilterObject3(iConfig.getUntrackedParameter<string>("HLTfiltObj3",  string("JpsitoMumuL2Filtered"))),
  fHLTFilterObject4(iConfig.getUntrackedParameter<string>("HLTfiltObj4",  string("displacedJpsitoMumuFilter"))) {

  cout << "----------------------------------------------------------------------" << endl;
  cout << "--- HFDumpTrigger constructor" << endl;
  cout << "--- Verbose                     : " << fVerbose << endl;
  cout << "--- particle Map                : " << fparticleMap.c_str() << endl;
  cout << "--- L1 GT Readout Record Label  : " << fL1GTReadoutRecordLabel.c_str() << endl;
  cout << "--- L1 GT Object Map Label      : " << fL1GTmapLabel.encode() << endl;
  cout << "--- L1 Muons Label              : " << fL1MuonsLabel.encode() << endl;
  cout << "--- HLT results Label           : " << fHLTResultsLabel.encode() << endl;
  cout << "--- L1 Trigger Name             : " << fL1TriggerName.c_str() << endl;
  cout << "--- HLT Label                   : " << fHLTriggerLabel.encode() << endl;
  cout << "--- HLT Trigger Name            : " << fHLTriggerName.c_str() << endl;
  cout << "--- HLT Filter Objects          : " 
       << fHLTFilterObject0.c_str() << ", "
       << fHLTFilterObject1.c_str() << ", "
       << fHLTFilterObject2.c_str() << ", "
       << fHLTFilterObject3.c_str() << " and "
       << fHLTFilterObject4.c_str() << endl;
  cout << "----------------------------------------------------------------------" << endl;

  fNevt = 0; 
  
}


// ----------------------------------------------------------------------
HFDumpTrigger::~HFDumpTrigger() {
  
}


// ----------------------------------------------------------------------
void HFDumpTrigger::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

  fNevt++;

  
  // -- L1 trigger bits
  gHFEvent->fL1Decision = 0; 
  gHFEvent->fL1w1=0; gHFEvent->fL1w2=0; gHFEvent->fL1w3=0; gHFEvent->fL1w4=0; 

  Handle<L1GlobalTriggerReadoutRecord> L1GTRR;
  iEvent.getByLabel(fL1GTReadoutRecordLabel,L1GTRR);
  Handle<L1GlobalTriggerObjectMapRecord> hL1GTmap; 
  iEvent.getByLabel("hltL1GtObjectMap", hL1GTmap);
  if (L1GTRR.isValid()) {
    gHFEvent->fL1Decision = (L1GTRR->decision()? 1: 0);
    int L1TBits_size=L1GTRR->decisionWord().size(); 
    for (unsigned int iTrig = 0; iTrig < L1GTRR->decisionWord().size(); ++iTrig) {
      int l1flag = L1GTRR->decisionWord()[iTrig]; 
      
      if (iTrig < 32 && l1flag) {
	gHFEvent->fL1w1 |= (0x1 << iTrig);
      } else if (iTrig < 64 && l1flag) {
	gHFEvent->fL1w2 |= (0x1 << iTrig);
      } else if (iTrig < 96 && l1flag) {
	gHFEvent->fL1w3 |= (0x1 << iTrig);
      } else if (iTrig < 128 && l1flag) {
	gHFEvent->fL1w4 |= (0x1 << iTrig);
      }
    }

    if (hL1GTmap.isValid()) {
      int iq(0); 
      DecisionWord gtDecisionWord = L1GTRR->decisionWord();
      const unsigned int numberTriggerBits(gtDecisionWord.size());
      const std::vector<L1GlobalTriggerObjectMap>& objMapVec =  hL1GTmap->gtObjectMap();
      for (std::vector<L1GlobalTriggerObjectMap>::const_iterator itMap = objMapVec.begin(); itMap != objMapVec.end(); ++itMap) {
	int itrig = (*itMap).algoBitNumber();
	cout << iq << " " << (*itMap).algoName() << "  " << itrig << endl;
	++iq;
      }
      
    }
  } 

  cout << "L1 trigger: " << endl
       << std::bitset<32>(gHFEvent->fL1w1) << endl
       << std::bitset<32>(gHFEvent->fL1w2) << endl
       << std::bitset<32>(gHFEvent->fL1w3) << endl
       << std::bitset<32>(gHFEvent->fL1w4) << endl;

  // -- L1 muons
  int L1_mu_size = 0;
  Handle<l1extra::L1MuonParticleCollection> hL1Muons;
  try {
    iEvent.getByLabel(fL1MuonsLabel, hL1Muons);
    l1extra::L1MuonParticleCollection::const_iterator l1muon;
    for (l1muon = hL1Muons->begin(); l1muon != hL1Muons->end(); ++l1muon ) {
      TLorentzVector a(0.0,0.0,0.0,0.0);
      a.SetPxPyPzE(l1muon->px(),l1muon->py(),l1muon->pz(),l1muon->energy());
      cout << "L1 muon: pT = " << a.Pt() << " phi = " << a.Phi() << " eta = " << a.Eta() << " q = " << l1muon->charge() << endl;
    }
  } catch (cms::Exception &ex) {
    //    cout << ex.explainSelf() << endl;
    cout << "==>HFDumpTrigger>  l1extra::L1MuonParticleCollection " << fL1MuonsLabel.encode() << " not found " << endl;
  }


  // -- HLT bits
  gHFEvent->fHLTDecision = 0; 
  gHFEvent->fHLTw1=0; gHFEvent->fHLTw2=0; gHFEvent->fHLTw3=0; gHFEvent->fHLTw4=0; 

  Handle<TriggerResults> hHLTresults;
  iEvent.getByLabel(fHLTResultsLabel, hHLTresults);
  TriggerNames trigName;
  trigName.init(*hHLTresults);

  int HLTwasRun1 = 0; int HLTwasRun2 = 0; int HLTwasRun3 = 0; int HLTwasRun4 = 0; 

  if (hHLTresults.isValid()) {
    //    HLTGlobal_wasrun=HLTR->wasrun();
    //    HLTGlobal_error=HLTR->error();
    gHFEvent->fHLTDecision = hHLTresults->accept();
    cout << "hHLTresults->size() = " << hHLTresults->size() << " and HLT accept = " << gHFEvent->fHLTDecision << endl;

    for (unsigned int iTrig = 0; iTrig < hHLTresults->size(); ++iTrig) {
      int hltflag = hHLTresults->accept(iTrig); 
      int error = hHLTresults->error(iTrig); 
      int wasrun = hHLTresults->wasrun(iTrig); 

      // This is the HLT name for each path: 
      cout << iTrig << " " << trigName.triggerName(iTrig) << endl;
      
      if (iTrig < 32 && hltflag) {
	gHFEvent->fHLTw1 |= (0x1 << iTrig);
      } else if (iTrig < 64 && hltflag) {
	gHFEvent->fHLTw2 |= (0x1 << iTrig);
      } else if (iTrig < 96 && hltflag) {
	gHFEvent->fHLTw3 |= (0x1 << iTrig);
      } else if (iTrig < 128 && hltflag) {
	gHFEvent->fHLTw4 |= (0x1 << iTrig);
      }

      if (iTrig < 32 && wasrun) {
	HLTwasRun1 |= (0x1 << iTrig);
      } else if (iTrig < 64 && hltflag) {
	HLTwasRun2 |= (0x1 << iTrig);
      } else if (iTrig < 96 && hltflag) {
	HLTwasRun3 |= (0x1 << iTrig);
      } else if (iTrig < 128 && hltflag) {
	HLTwasRun4 |= (0x1 << iTrig);
      }
    }


    cout << "HLT trigger accept/run: " << endl
	 << std::bitset<32>(gHFEvent->fHLTw1) << endl
	 << std::bitset<32>(HLTwasRun1) << endl << endl
	 << std::bitset<32>(gHFEvent->fHLTw2) << endl
	 << std::bitset<32>(HLTwasRun2) << endl<< endl
	 << std::bitset<32>(gHFEvent->fHLTw3) << endl
	 << std::bitset<32>(HLTwasRun3) << endl << endl
	 << std::bitset<32>(gHFEvent->fHLTw4) << endl
	 << std::bitset<32>(HLTwasRun4) << endl;

    cout << "test: " << std::bitset<32>(10) << endl;
  }   
  
  Handle<trigger::TriggerEvent> trgEvent;
  bool hltF = true;
  try {
    iEvent.getByLabel(fTriggerEventLabel, trgEvent);
  } catch (const cms::Exception& e) {
    hltF = false;
    cout<<"Error!! No TriggerEvent with label " << fTriggerEventLabel << endl;
  }

  if (hltF) {
    TriggerObjectCollection allObjects = trgEvent->getObjects();
    for (int i=0; i < trgEvent->sizeFilters(); i++){         
      Keys keys = trgEvent->filterKeys(i);
      if (keys.size() > 0) {
	cout << trgEvent->filterTag(i) << endl; 
	for (int j=0; j<keys.size(); j++){
	  cout << " pt = " <<  allObjects[keys[j]].pt() 
	       << " eta = " << allObjects[keys[j]].eta() 
	       << " phi = " << allObjects[keys[j]].phi() 
	       << " e = " << allObjects[keys[j]].energy() 
	       << " id = " << allObjects[keys[j]].id() 
	       << endl;
	}
      }
    }
  }  
}


// ------------ method called once each job just before starting event loop  ------------
void  HFDumpTrigger::beginJob(const edm::EventSetup& setup) {

}

// ------------ method called once each job just after ending the event loop  ------------
void  HFDumpTrigger::endJob() {

}

//define this as a plug-in
//DEFINE_FWK_MODULE(HFDumpTrigger);
