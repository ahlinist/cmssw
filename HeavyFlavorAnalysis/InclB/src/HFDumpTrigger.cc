#include <iostream>

#include "HeavyFlavorAnalysis/InclB/interface/HFDumpTrigger.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"

#include "DataFormats/L1Trigger/interface/L1ParticleMap.h"
#include "DataFormats/L1Trigger/interface/L1MuonParticle.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1Trigger/interface/L1ParticleMapFwd.h"

#include "FWCore/Framework/interface/ESHandle.h"

#include "HeavyFlavorAnalysis/InclB/rootio/TAna00Event.hh"
#include "HeavyFlavorAnalysis/InclB/rootio/TAnaTrack.hh"
#include "HeavyFlavorAnalysis/InclB/rootio/TAnaCand.hh"
#include "HeavyFlavorAnalysis/InclB/rootio/TGenCand.hh"
#include "HeavyFlavorAnalysis/InclB/rootio/TAnaVertex.hh"

#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetupFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMap.h"
#include "CondFormats/L1TObjects/interface/L1GtTriggerMenu.h"
#include "CondFormats/DataRecord/interface/L1GtTriggerMenuRcd.h"
#include "CondFormats/L1TObjects/interface/L1GtTriggerMenuFwd.h"


#include "DataFormats/HLTReco/interface/TriggerEvent.h"

#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TH2D.h>

// -- Yikes!
extern TAna00Event *gHFEvent;

using namespace std;
using namespace edm;
using namespace reco;
using namespace trigger;


// ----------------------------------------------------------------------
HFDumpTrigger::HFDumpTrigger(const edm::ParameterSet& iConfig):
  fVerbose(iConfig.getUntrackedParameter<int>("verbose", 0)),
  doL1(iConfig.getUntrackedParameter<int>("dol1", 1)),
  doHLT(iConfig.getUntrackedParameter<int>("dohlt", 1)),
  fGlobalTriggerLabel(iConfig.getUntrackedParameter< std::string > ("GlobalTriggerLabel")),
  fL1MuLabel(iConfig.getUntrackedParameter< std::string > ("L1MuLabel")),
  fL1TriggerName(iConfig.getUntrackedParameter<string>("L1TriggerName", string("L1_SingleMu7"))), 
  fL1w1(iConfig.getUntrackedParameter<string>("L1w1", string("L1_SingleMu3"))), 
  fL1w2(iConfig.getUntrackedParameter<string>("L1w2", string("L1_SingleMu5"))),
  fL1w3(iConfig.getUntrackedParameter<string>("L1w3", string("L1_SingleMu7"))), 
  fL1w4(iConfig.getUntrackedParameter<string>("L1w4", string("L1_SingleMu10"))),
  fHLTriggerLabel(iConfig.getParameter<edm::InputTag>("hltLabel")),
  fHLTriggerObjectLabel(iConfig.getParameter<edm::InputTag>("hltobjectLabel")),
  fHLTriggerName(iConfig.getUntrackedParameter<string>("HLTriggerName",  string("HLT_Mu15"))), 
  fHLTw1(iConfig.getUntrackedParameter<string>("HLTw1", string("HLT_Mu3"))), 
  fHLTw2(iConfig.getUntrackedParameter<string>("HLTw2", string("HLT_Mu5"))),
  fHLTw3(iConfig.getUntrackedParameter<string>("HLTw3", string("HLT_Mu7"))), 
  fHLTw4(iConfig.getUntrackedParameter<string>("HLTw4", string("HLT_Mu9"))),
  fHLTw5(iConfig.getUntrackedParameter<string>("HLTw5", string("HLT_Mu11"))),
  fHLTw6(iConfig.getUntrackedParameter<string>("HLTw6", string("HLT_Mu13"))), 
  fHLTw7(iConfig.getUntrackedParameter<string>("HLTw7", string("HLT_Mu15"))),
  fHLTFilterObject2(iConfig.getUntrackedParameter<string>("HLTfiltObj2",  string("hltL2MuonCandidates::HLT"))),
  fHLTFilterObject3(iConfig.getUntrackedParameter<string>("HLTfiltObj3",  string("hltL3MuonCandidates::HLT"))) {

  cout << "----------------------------------------------------------------------" << endl;
  cout << "--- HFDumpTrigger constructor" << endl;
  cout << "--- Verbose          : " << fVerbose << endl;
  cout << "--- Global Trigger   : " << fGlobalTriggerLabel.c_str() << endl;
  cout << "--- L1 Mu Label      : " << fL1MuLabel.c_str() << endl;
  cout << "--- L1 Trigger Name  : " << fL1TriggerName.c_str() << endl;
  cout << "--- HLT Label        : " << fHLTriggerLabel.encode() << endl;
  cout << "--- HLT Object Label : " << fHLTriggerObjectLabel.encode() << endl;
  cout << "--- HLT Trigger Name : " << fHLTriggerName.c_str() << endl;
  cout << "--- HLT Filter Objects : " 
       << fHLTFilterObject2.c_str() << ", "
       << fHLTFilterObject3.c_str() << endl;
  cout << "----------------------------------------------------------------------" << endl;
  fFile       = new TFile(iConfig.getParameter<string>("fileName").c_str(), "RECREATE");
  fHistoL1          = new TH1I("l1triggerbits", "L1 trigger bits", 200, 0., 200.);
  fHistoHLT         = new TH1I("hlttriggerbits", "HLT trigger bits", 150, 0., 150.);
  fHistoHLT_on      = new TH1I("hlttriggerbitson", "HLT trigger bits on", 150, 0., 150.);
 
  
}


// ----------------------------------------------------------------------
HFDumpTrigger::~HFDumpTrigger() {
 
  // -- Save output
  fFile->cd();
  fHistoL1->Write(); 
  fHistoHLT->Write();
  fHistoHLT_on->Write();
  fFile->Write();
  fFile->Close();
  delete fFile;
}


// ----------------------------------------------------------------------
void HFDumpTrigger::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

  nevt++;
 
  int index = gHFEvent->nSigTracks(); 
  
   TAnaTrack *pTrack;

 //   if (iEvent.id().event()==2161318 || iEvent.id().event()==15098933 || iEvent.id().event()==5224518 ||  iEvent.id().event()==6094468 ||iEvent.id().event()==3182865 ){
//      fVerbose = 1;
//      cout << "====> Event " << iEvent.id().event() << endl;
//    }

  //   // -- L1 trigger
  // 
   if (doL1>0) {
     if ( fVerbose ) cout << endl << "============================ L1 Trigger ===================================" << endl; 
     
     gHFEvent->fL1Decision=0;
     gHFEvent->fL1w1=0; gHFEvent->fL1w2=0; gHFEvent->fL1w3=0; gHFEvent->fL1w4=0; 
     
     edm::Handle<L1GlobalTriggerReadoutRecord> l1GtRR;
     edm::ESHandle<L1GtTriggerMenu> hL1GtMenu; 
        
     try {
       iEvent.getByLabel(fGlobalTriggerLabel,l1GtRR);
       
     } catch (Exception event) { 
       if (fVerbose > 0) cout << "==>HFDumpTrigger>ERROR: no L1GlobalTriggerReadoutRecord " << endl;
     }
     const L1GlobalTriggerReadoutRecord& L1GTRR = *l1GtRR; 

     TString algoBitToName[128];
     DecisionWord gtDecisionWord = L1GTRR.decisionWord();
     const unsigned int numberTriggerBits(gtDecisionWord.size());
     //const TechnicalTriggerWord&  technicalTriggerWordBeforeMask = l1GtRR->technicalTriggerWord();
     
     try {
       iSetup.get<L1GtTriggerMenuRcd>().get(hL1GtMenu);
       
     } catch (Exception event) { 
       if (fVerbose > 0) cout << "==>HFDumpTrigger>ERROR: no L1GtTriggerMenuRcd "  << endl;
     }
     
     const L1GtTriggerMenu* l1GtMenu = hL1GtMenu.product();
     
     if (l1GtRR.isValid()) {  

       //technical trigger bits///////////////////////////
//        for (CItAlgo techTrig = l1GtMenu->gtTechnicalTriggerMap().begin(); techTrig != l1GtMenu->gtTechnicalTriggerMap().end(); ++techTrig) {
// 	 int number = (techTrig->second).algoBitNumber();
// 	 cout << (techTrig->second).algoBitNumber() << " : " << (techTrig->second).algoName() << " decision " << technicalTriggerWordBeforeMask.at(number) << std::endl;
// 	
//        }
       //////////////////////////////////////////////////
       
       const AlgorithmMap& algorithmMap = l1GtMenu->gtAlgorithmMap();
       for (CItAlgo itAlgo = algorithmMap.begin(); itAlgo !=algorithmMap.end(); itAlgo++) {
	 std::string aName = itAlgo->first;
	 int algBitNumber = (itAlgo->second).algoBitNumber();
	 // Get trigger names
	 algoBitToName[algBitNumber] = TString( aName );
       }
       
       for (unsigned int iBit = 0; iBit < numberTriggerBits; ++iBit) {
	 
	 // ...Fill the corresponding accepts in branch-variables
	 int l1flag = gtDecisionWord[iBit];
	 if (fVerbose > 0) cout << "L1 TD: "<<iBit<<" "<<algoBitToName[iBit]<<" "<<gtDecisionWord[iBit]<< std::endl;
	 
	 //fill histogram 
	 int binvalue = int(fHistoL1->GetBinContent(iBit+1));
	 fHistoL1->SetBinContent(iBit+1,binvalue+l1flag);
	 fHistoL1->GetXaxis()->SetBinLabel(iBit+1, algoBitToName[iBit]);
	 
	 //fill muon trigger bits
	 if ( !strcmp(algoBitToName[iBit], fL1TriggerName.c_str()) ) {
	   if ( fVerbose ) cout << " =============>  L1 decision set to " << l1flag << endl;
	   gHFEvent->fL1Decision = l1flag;      
	 }
	 if ( !strcmp(algoBitToName[iBit], fL1w1.c_str()) ) {
	   if ( fVerbose ) cout << " =============>  L1 w1 set to " << l1flag << endl;
	   gHFEvent->fL1w1 = l1flag;      
	 }
	 if ( !strcmp(algoBitToName[iBit], fL1w2.c_str()) ) {
	   if ( fVerbose ) cout << " =============>  L1 w2 set to " << l1flag << endl;
	   gHFEvent->fL1w2 = l1flag;      
	 }
	 if ( !strcmp(algoBitToName[iBit], fL1w3.c_str()) ) {
	   if ( fVerbose ) cout << " =============>  L1 w3 set to " << l1flag << endl;
	   gHFEvent->fL1w3 = l1flag;      
	 }
	 if ( !strcmp(algoBitToName[iBit], fL1w4.c_str()) ) {
	   if ( fVerbose ) cout << " =============>  L1 w4 set to " << l1flag << endl;
	   gHFEvent->fL1w4 = l1flag;      
	 } 
	 
       }
     }
     else {
       if (fVerbose > 0) cout << "==>HFDumpTrigger>ERROR: L1GlobalTriggerReadoutRecord or L1GlobalTriggerObjectMapRecord not valid"  << endl;
       
     }
       
       
       
       //   try { 
       //     // -- adding the whole L1 muons collection to signal block
       //     edm::Handle<l1extra::L1MuonParticleCollection> l1extmu;
       //     iEvent.getByLabel(fL1MuLabel,l1extmu);
       //     const l1extra::L1MuonParticleCollection& L1ExtMu = *l1extmu;
       //     if (&L1ExtMu) {
       
       //       if ( fVerbose ) cout << "==>HFDumpTrigger>nL1Muons = " << L1ExtMu.size() << endl;
       
       
       //       for (l1extra::L1MuonParticleCollection::const_iterator muItr = L1ExtMu.begin(); muItr != L1ExtMu.end(); ++muItr) {
       
       // 	L1MuGMTExtendedCand gmtCand = muItr->gmtMuonCand();
       
       // 	if ( fVerbose ) {
       
       // 	  cout << "==> L1Muon " << index << endl;
       // 	  cout << "pt " << muItr->pt() << " e " << muItr->energy() << " eta " << muItr->eta() 
       // 	       << " phi " << muItr->phi() << " .... quality " << gmtCand.quality() << endl;
       // 	}
       
       // 	pTrack   = gHFEvent->addSigTrack();
       // 	pTrack->fMuType   = 1;
       // 	pTrack->fMCID     = muItr->charge()*-13; 
       // 	pTrack->fIndex    = index;
       // 	pTrack->fGenIndex = -1; 
       // 	pTrack->fQ        = gmtCand.quality();
       // 	pTrack->fPlab.SetPtEtaPhi(muItr->pt(),
       // 				  muItr->eta(),
       // 				  muItr->phi()
       // 				  );
       
       // 	pTrack->fLip     = gmtCand.useInSingleMuonTrigger(); 
       // 	pTrack->fLipE    = gmtCand.isMatchedCand();
       // 	pTrack->fTip     = gmtCand.isHaloCand();
       // 	pTrack->fTipE    = muItr->isIsolated();
       
       // 	index++;
       //       }
       //     }
       //     else {
       
       //       if ( fVerbose ) cout << "==>HFDumpTrigger>ERROR: L1Mu object NOT valid" << endl;
       //     }
       
       //   } catch (Exception event) { 
       
       //     if (fVerbose > 0) cout << "==>HFDumpTrigger>ERROR: no L1Mu object"<< endl;
       //   }
     
   }
   
   if (doHLT) {
  //   // -- HLT trigger
  //   if ( fVerbose ) cout << endl << "============================ HL Trigger ===================================" << endl; 
  gHFEvent->fHLTDecision=0;
  gHFEvent->fHLTw1=0; gHFEvent->fHLTw2=0; gHFEvent->fHLTw3=0; gHFEvent->fHLTw4=0;  gHFEvent->fHLTw5=0; gHFEvent->fHLTw6=0; gHFEvent->fHLTw7=0; 

  edm::Handle<edm::TriggerResults> hltresults;
 
  try {
    iEvent.getByLabel(fHLTriggerLabel,hltresults);
 
  } catch (Exception event) {
    if ( fVerbose ) cout << "%HFDumpTrigger -- Couldn't get handle on HLT Trigger!" << endl;
  }

  if (!hltresults.isValid()) {
    if ( fVerbose ) cout << "%HFDumpTrigger -- No Trigger Result!" << endl;
  } 
  else {
    int ntrigs=hltresults->size();
    if (ntrigs==0){
	if ( fVerbose ) cout << "%HFDumpTrigger -- No trigger name given in TriggerResults of the input " << endl;
    } 

    // get hold of trigger names - based on TriggerResults object!
    //edm::TriggerNames triggerNames_;
    //triggerNames_.init(*hltresults); 

    edm::TriggerNames const& triggerNames_ = iEvent.triggerNames(*hltresults);

 
    for (int itrig=0; itrig< ntrigs; itrig++) {

      TString trigName = triggerNames_.triggerName(itrig);
      int hltflag = (*hltresults)[itrig].accept();
      int wasrun  = (*hltresults)[itrig].wasrun(); 

      int binvalue = int(fHistoHLT->GetBinContent(itrig+1));
      fHistoHLT->SetBinContent(itrig+1,binvalue+hltflag);
      fHistoHLT->GetXaxis()->SetBinLabel(itrig+1, trigName);

      int binvalueon = int(fHistoHLT_on->GetBinContent(itrig+1));
      fHistoHLT_on->SetBinContent(itrig+1,binvalueon+wasrun);
      fHistoHLT_on->GetXaxis()->SetBinLabel(itrig+1, trigName);

      if ( fVerbose ) cout << "HLT  :"  << setw(3) << itrig << setw(30)  <<  trigName
			   << " was run " << wasrun  << ", decision " << hltflag << endl;

      if ( !strcmp(trigName, fHLTriggerName.c_str() ) ) {     
	if ( fVerbose ) cout << " =============>  HLT decision set to " << hltflag << endl;
	gHFEvent->fHLTDecision = hltflag;
      }
      if ( !strcmp(trigName, fHLTw1.c_str() ) ) {     
	if ( fVerbose ) cout << " =============>  HLT w1 " << hltflag << endl;
	gHFEvent->fHLTw1 = hltflag;
      }
      if ( !strcmp(trigName, fHLTw2.c_str() ) ) {     
	if ( fVerbose ) cout << " =============>  HLT w2 " << hltflag << endl;
	gHFEvent->fHLTw2 = hltflag;
      }
      if ( !strcmp(trigName, fHLTw3.c_str() ) ) {     
	if ( fVerbose ) cout << " =============>  HLT w3 " << hltflag << endl;
	gHFEvent->fHLTw3 = hltflag;
      }
      if ( !strcmp(trigName, fHLTw4.c_str() ) ) {     
	if ( fVerbose ) cout << " =============>  HLT w4 " << hltflag << endl;
	gHFEvent->fHLTw4 = hltflag;
      }
      if ( !strcmp(trigName, fHLTw5.c_str() ) ) {     
	if ( fVerbose ) cout << " =============>  HLT w5 " << hltflag << endl;
	gHFEvent->fHLTw5 = hltflag;
      }
      if ( !strcmp(trigName, fHLTw6.c_str() ) ) {     
	if ( fVerbose ) cout << " =============>  HLT w6 " << hltflag << endl;
	gHFEvent->fHLTw6 = hltflag;
      }
      if ( !strcmp(trigName, fHLTw7.c_str() ) ) {     
	if ( fVerbose ) cout << " =============>  HLT w7 " << hltflag << endl;
	gHFEvent->fHLTw7 = hltflag;
      }
    } 
  }


  //#########################################=======> update: see HLTrigger/HLTcore/plugins/HLTEventAnalyzerAOD.cc
  if ( fVerbose ) cout << "TriggerSummaryAnalyzerAOD: content of TriggerEvent: " << endl;

  Handle<TriggerEvent> triggerhandle;
  iEvent.getByLabel(fHLTriggerObjectLabel,triggerhandle);
  if (triggerhandle.isValid()) {
    const size_type nC(triggerhandle->sizeCollections());
    if ( fVerbose ) {

      cout << "Used Processname: " << triggerhandle->usedProcessName() << endl;
      cout << "Number of packed Collections: " << nC << endl;
      cout << "The Collections: #, tag, 1-past-end index" << endl;
      
      for (size_type iC=0; iC!=nC; ++iC) {
	cout << iC << " "
	     << triggerhandle->collectionTag(iC).encode() << " "
	     << triggerhandle->collectionKey(iC) << endl;
      }

    }
    const size_type nO(triggerhandle->sizeObjects());
    const TriggerObjectCollection& TOC(triggerhandle->getObjects());

    if ( fVerbose ) {
      cout << "Number of TriggerObjects: " << nO << endl;
      cout << "The TriggerObjects: #, id, pt, eta, phi, mass" << endl;
      
      for (size_type iO=0; iO!=nO; ++iO) {
	const TriggerObject& TO(TOC[iO]);
	cout << iO << " " << TO.id() << " " << TO.pt() << " " << TO.eta() << " " << TO.phi() << " " << TO.mass() << endl;
      }
    }


    //fill trigger information in TAna00Event
    for (size_type iC=0; iC!=nC; ++iC) { 

      //L2 candidates
      if ( !strcmp(triggerhandle->collectionTag(iC).encode().c_str(), fHLTFilterObject2.c_str() ) ) {   
	int start = 0;
	if (iC > 0)
	  start = triggerhandle->collectionKey(iC-1);
	int end   = triggerhandle->collectionKey(iC);
	if ( fVerbose ) cout << " =============>  found " << fHLTFilterObject2.c_str() << ": " << start  << " " << end << endl;

	for (int i=start; i<end; i++){ 
	  const TriggerObject& TO2(TOC[i]);
	 
	  pTrack   = gHFEvent->addSigTrack();
	  pTrack->fMuType   = 2;
	  pTrack->fMCID     = TO2.id(); 
	  pTrack->fMuID     = -1;
	  pTrack->fIndex    = index;
	  pTrack->fGenIndex = -1; 
	  if (TO2.id()==13)
	    pTrack->fQ        = -1;
	  else if (TO2.id()==-13)
	    pTrack->fQ        = 1;
	  else 
	    pTrack->fQ        = 0;
	  pTrack->fPlab.SetPtEtaPhi(TO2.pt(),
				    TO2.eta(),
				    TO2.phi()
				    );

	  if (fVerbose) pTrack->dump();
  
	  index++;
	  
	}

      }

      //L3 candidates
      if ( !strcmp(triggerhandle->collectionTag(iC).encode().c_str(), fHLTFilterObject3.c_str() ) ) {   
	int start = 0;
	if (iC > 0)
	  start = triggerhandle->collectionKey(iC-1);
	int end   = triggerhandle->collectionKey(iC);
	if ( fVerbose ) cout << " =============>  found " << fHLTFilterObject3.c_str() << ": " << start  << " " << end << endl;

	for (int i=start; i<end; i++){ 
	  const TriggerObject& TO2(TOC[i]);
	 
	  pTrack   = gHFEvent->addSigTrack();
	  pTrack->fMuType   = 3;
	  pTrack->fMCID     = TO2.id(); 
	  pTrack->fMuID     = -1;
	  pTrack->fIndex    = index;
	  pTrack->fGenIndex = -1; 
	  if (TO2.id()==13)
	    pTrack->fQ        = -1;
	  else if (TO2.id()==-13)
	    pTrack->fQ        = 1;
	  else 
	    pTrack->fQ        = 0;
	  pTrack->fPlab.SetPtEtaPhi(TO2.pt(),
				    TO2.eta(),
				    TO2.phi()
				    );

	  if (fVerbose) pTrack->dump();
  
	  index++;
	  
	}

      }

     
      
    }
    
    if ( fVerbose ) {
      //print trigger filters
      const size_type nF(triggerhandle->sizeFilters());
      cout << "Number of TriggerFilters: " << nF << endl;
      cout << "The Filters: #, tag, #ids/#keys, the id/key pairs" << endl;
      for (size_type iF=0; iF!=nF; ++iF) {
	const Vids& VIDS (triggerhandle->filterIds(iF));
	const Keys& KEYS(triggerhandle->filterKeys(iF));
	const size_type nI(VIDS.size());
	const size_type nK(KEYS.size());
	cout << iF << " " << triggerhandle->filterTag(iF).encode()
	     << " " << nI << "/" << nK
	     << " the pairs: ";
	const size_type n(max(nI,nK));
	for (size_type i=0; i!=n; ++i) {
	  cout << " " << VIDS[i] << "/" << KEYS[i];
	}
	cout << endl;
	assert (nI==nK);
      }
    }
  } else {
    if ( fVerbose ) cout << "%HFDumpTrigger -- TriggerSummary: Triggerhandle invalid! Check InputTag provided." << endl;
  }


   }

}


// ------------ method called once each job just before starting event loop  ------------
void  HFDumpTrigger::beginJob() {
  nevt = 0;

}

// ------------ method called once each job just after ending the event loop  ------------
void  HFDumpTrigger::endJob() {
  cout << "HFDumpTrigger>    Summary: Events processed: " << nevt << endl;
  
}

//define this as a plug-in
DEFINE_FWK_MODULE(HFDumpTrigger);
