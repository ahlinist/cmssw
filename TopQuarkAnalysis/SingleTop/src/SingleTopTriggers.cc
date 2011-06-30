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
  triggersList = iConfig.getParameter<std::vector< std::string > >("triggerList");
  
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

  if(triggersList.size()==1)if(triggersList.at(0)=="" || triggersList.at(0)=="none") return true;

  if(isMC)hlTriggerResults_ = edm::InputTag("TriggerResults","","REDIGI311X");
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

      //string muonBit;
      //string eleBit;

    
      
      TriggerResults tr;
      tr = *HLTR;
      vector<string> triggerList;
      bool passesTrigger = false;
      //      std::cout << "List of triggers: \n";
      for (unsigned int i=0;i<HLTR->size();++i){

	//	std::cout.width(3); std::cout << i;
	//	std::cout << " - " <<  hlNames_[i] << "   " << tr.accept(i) << std::endl;
	
	for(size_t n = 0; n< triggersList.size();++n){
	  if((hlNames_[i] == triggersList.at(n)) && (tr.accept(i))) return true;// muonNonIso =true;
	}
      }
      //	if (muonTrigger){
	//	  cout << "channel Electron BUT Muon trigger " << endl;
	//	  return false;
	//	}
    }
 
  
      
  return false;
}

//define this as a plug-in
DEFINE_FWK_MODULE(SingleTopTriggers);
