#include <memory>
#include <vector>
#include <map>
#include <set>

// user include files
#include "TopQuarkAnalysis/SingleTop/interface/SingleTopTriggers.h"

#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
//#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GtFdlWord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"

using namespace edm;
using namespace std;

SingleTopTriggers::SingleTopTriggers(const edm::ParameterSet& iConfig)
{
  hlTriggerResults_ = iConfig.getParameter<edm::InputTag> ("HLTriggerResults");
  init_ = false;
  
  
  isMC = iConfig.getUntrackedParameter<bool>("isMC",true);
  channel = iConfig.getUntrackedParameter<int>("channel",1);

}

SingleTopTriggers::~SingleTopTriggers()
{
}

bool SingleTopTriggers::filter( edm::Event& iEvent,const  edm::EventSetup& c)
{
  
  int ievt = iEvent.id().event();
  int irun = iEvent.id().run();
  int ils = iEvent.luminosityBlock();
  int bx = iEvent.bunchCrossing();
  
  //
  // trigger type
  //
  int trigger_type=-1;
  if (iEvent.isRealData())  trigger_type = iEvent.experimentType();
  
  
  //hlt info
  edm::Handle<TriggerResults> HLTR;
  iEvent.getByLabel(hlTriggerResults_,HLTR);
 
  
  bool eleNonIso = false;
  bool muonNonIso = false;

  if(HLTR.isValid() == false) {
    std::cout<< " HLTInspect Error - Could not access Results with name "<<hlTriggerResults_<<std::endl;
  }
  if(HLTR.isValid())
    {
      if (!init_) {
	//    init_=true;
	const edm::TriggerNames & triggerNames = iEvent.triggerNames(*HLTR);
	hlNames_=triggerNames.triggerNames();
      }

      string muonBit;
      string electronBit;

      vector<string> muonBitNonIso;
      vector<string> eleBitNonIso;
      
      if (isMC) {
	muonBit="HLT_IsoMu17_v4";
	electronBit="HLT_Ele32_SW_TighterEleId_L1R_v2";
	//	electronTrigger = true;
      } 
      else {
	if (irun<163270) muonBit="HLT_IsoMu17_v5";
	else muonBit="HLT_IsoMu17_v6";

	//	muonBitNonIso.push_back("HLT_Mu15_v2");
	//	muonBitNonIso.push_back("HLT_Mu15_v3");
	
	//	eleBitNonIso.push_back("HLT_Ele25_CaloIdVT_TrkIdT_CentralJet30_v2");
	//	eleBitNonIso.push_back("HLT_Ele25_CaloIdVT_TrkIdT_CentralJet30_v3");
	eleBitNonIso.push_back("HLT_Mu17_TriCentralJet30_v2");
	eleBitNonIso.push_back("HLT_Ele25_CaloIdVT_TrkIdT_CentralJet40_BTagIP");
	eleBitNonIso.push_back("HLT_Ele25_CaloIdVT_TrkIdT_CentralDiJet30_v2");
	eleBitNonIso.push_back("HLT_Ele25_CaloIdVT_TrkIdT_CentralDiJet30_v3");


	muonBitNonIso.push_back("HLT_Mu17_CentralJet40_BTagIP");
	muonBitNonIso.push_back("HLT_Mu17_CentralJet40_BTagIP_v2");
	muonBitNonIso.push_back("HLT_Mu17_TriCentralJet30_v2");
	muonBitNonIso.push_back("HLT_Mu17_TriCentralJet30_v3");
	muonBitNonIso.push_back("HLT_Mu17_TriCentralJet30_v4");

	if (irun < 162803) electronBit = "HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v1";
	else if (irun < 163270) electronBit="HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v2";
	else electronBit="HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v3";
      }

      
      TriggerResults tr;
      tr = *HLTR;
      vector<string> triggerList;
      muonTrigger = false;
      electronTrigger = false;
      //      std::cout << "List of triggers: \n";
      for (unsigned int i=0;i<HLTR->size();++i){

	//	std::cout.width(3); std::cout << i;
	//	std::cout << " - " <<  hlNames_[i] << "   " << tr.accept(i) << std::endl;
	
	if ((hlNames_[i] == muonBit) && (tr.accept(i))) {
	  muonTrigger=true;
	  
	  //  cout << " *** " << hlNames_[i] << " : " << tr.accept(i) << "  ***   channel: " << channel <<  "   muonTrigger: " << muonTrigger << endl;
	}
	
	for(size_t n = 0; n< muonBitNonIso.size();++n){
	  if((hlNames_[i] == muonBitNonIso.at(n)) && (tr.accept(i)) && channel == 4) return true;// muonNonIso =true;
	}
	
	for(size_t n = 0; n< eleBitNonIso.size();++n){
	  if((hlNames_[i] == eleBitNonIso.at(n)) && (tr.accept(i)) && channel == 5 )return true;// eleNonIso =true;
	}
	
	//	else{
	//	  muonTrigger = false;
	//	}
	
	if ((hlNames_[i] == electronBit) && (tr.accept(i))) {// this will work if this trigger is present in the event
	  electronTrigger=true;
	  //	  cout << " **** " << hlNames_[i] << " : " << tr.accept(i) << "  ***   channel: " << channel <<endl;
	}

	
	//	else electronTrigger = false;

      }
      
      //      cout << "channel: " << channel << "   electronTrigger: " << electronTrigger << "   muonTrigger: " << muonTrigger  <<  endl;
      

      //      std::cout << "HLTInspect: Run " << irun << " Ev " << ievt << " LB " << ils << " BX " << bx << " Type "<<trigger_type<< " Acc: " ;
      const unsigned int n(hlNames_.size());
      for (unsigned int i=0; i!=n; ++i)
        {
          if (HLTR->accept(i))
            {
	      ;//      std::cout << hlNames_[i] << ",";
            }
        }
      //      std::cout << std::endl;
      
      
      
      if (channel == 1 ) {
	//if(electronTrigger){
	//	  cout << "channel Muon BUT Electron trigger " << endl;
	//	  return false;
	//	}
	if (muonTrigger){
	  //	  cout << "channel Muon AND Muon trigger " << endl;
	  return true;
	  
	}
      }
      else if (channel == 2){
	if(electronTrigger){
	  //cout << "channel Electron AND Electron trigger " << endl;
	  return true;
	}
	else if (channel == 3){
	  if(electronTrigger || muonTrigger ){
	    //cout << "channel Electron AND Electron trigger " << endl;
	    return true;
	  }
	}
	else if( channel == 4){
	  return muonNonIso;
	}
	else if( channel == 5){
	  return eleNonIso;
	}
	  
	//	if (muonTrigger){
	//	  cout << "channel Electron BUT Muon trigger " << endl;
	//	  return false;
	//	}
      }
      
      
      
    }
  
  return false;
}

//define this as a plug-in
DEFINE_FWK_MODULE(SingleTopTriggers);
