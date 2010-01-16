#include "JetMETAnalysis/PromptAnalysis/interface/PromptAna_Trigger.h"
#include "FWCore/Framework/interface/Event.h"

unsigned int NmaxL1AlgoBit = 128;
unsigned int NmaxL1TechBit = 64;

PromptAna_Trigger::PromptAna_Trigger(const edm::ParameterSet& iConfig) :
  inputTag(iConfig.getParameter<edm::InputTag>("InputTag")),
  nBxOutput(iConfig.getParameter<int>         ("NBxOutput")),
  storeByName(iConfig.getParameter<bool>       ("StoreByName")),
  algoNameList(iConfig.getParameter<std::vector<std::string> > ("ListOfTriggerNames")),
  TRGTag(iConfig.getParameter<edm::InputTag>("TRGTag"))
{
  //details here: https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideL1TriggerFAQ

  produces <std::vector<int> > ( "hltbits" );
  produces <std::vector<int> > ( "l1physbits" );
  produces <std::vector<int> > ( "l1techbits" );

  if (nBxOutput==3) {
    produces <std::vector<int> > ( "l1physbitsm1" );
    produces <std::vector<int> > ( "l1techbitsm1" );
    produces <std::vector<int> > ( "l1physbitsp1" );
    produces <std::vector<int> > ( "l1techbitsp1" );
  }

  if (storeByName) {
    fillAlgoNameListNoUnderScores();
    for (std::vector<std::string>::const_iterator it=algoNameListNoUnderScores.begin(); it!=algoNameListNoUnderScores.end();++it) {
      produces <bool> ( *it );
    }
  }

}

void PromptAna_Trigger::endJob()
{

  //make set
  std::set<std::string> algoNameSet;
  for (std::vector<std::string>::const_iterator it=algoNameList.begin();it!=algoNameList.end();++it) {
    algoNameSet.insert(*it);
  }

  //decide whether to print a report
  bool printReport=false;
  for (std::set<std::string>::const_iterator it=algosPresentInData.begin();it!=algosPresentInData.end();++it) {
    if (algoNameSet.count(*it)==0) printReport=true;
  }
  if (!printReport) return;

  //print a report
  std::cout << "/------------------------------------------\\" << std::endl;
  std::cout << "|      PromptAna_Trigger report      |" << std::endl;
  std::cout << "|------------------------------------------|" << std::endl;
  std::cout << "| There are triggers in the data that are  |" << std::endl;
  std::cout << "| not in the config file.  Please paste    |" << std::endl;
  std::cout << "| this code into ListOfTriggerNames in     |" << std::endl;
  std::cout << "| PromptAna_Trigger_cfi.py           |" << std::endl;
  std::cout << std::endl;
  for (std::set<std::string>::const_iterator it=algosPresentInData.begin();it!=algosPresentInData.end();++it) {
    if (algoNameSet.count(*it)==0) std::cout << "\"" << *it << "\"," << std::endl;
  }
  std::cout << std::endl;
  std::cout << "|------------------------------------------|" << std::endl;  
  std::cout << "|    end PromptAna_Trigger report    |" << std::endl;
  std::cout <<"\\------------------------------------------/" << std::endl;
}

void PromptAna_Trigger::
produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  std::auto_ptr<std::vector<int> >  l1physbits   ( new std::vector<int>() );
  std::auto_ptr<std::vector<int> >  l1techbits   ( new std::vector<int>() );
  std::auto_ptr<std::vector<int> >  l1physbitsm1 ( new std::vector<int>() );
  std::auto_ptr<std::vector<int> >  l1techbitsm1 ( new std::vector<int>() );
  std::auto_ptr<std::vector<int> >  l1physbitsp1 ( new std::vector<int>() );
  std::auto_ptr<std::vector<int> >  l1techbitsp1 ( new std::vector<int>() );
  std::auto_ptr<std::vector<int> >  hltbits      ( new std::vector<int>() );

  edm::Handle<L1GlobalTriggerReadoutRecord> l1GtReadoutRecord;
  iEvent.getByLabel(inputTag, l1GtReadoutRecord);
  //iEvent.getByType(l1GtReadoutRecord); // assume only one L1 trigger record here

  edm::Handle<edm::TriggerResults> triggers;
  iEvent.getByLabel(TRGTag, triggers);
  edm::TriggerResults tr = *triggers;
  
  //   //new trigger names  
  //   if (triggers.isValid())
  //     {
  //       edm::TriggerNames names; names.init(*triggers);    
  //       for(unsigned i=0; i < triggers->size(); i++)
  //    	std::cout << i << " " << names.triggerName(i) << std::endl;    
  //     }
  
  //   //old trigger names (does not work?)
  //   std::vector<std::string> triggernames;
  //   edm::Service<edm::service::TriggerNamesService> tns;
  //   tns->getTrigPaths(*triggers, triggernames);
  //   ofstream  outfile;
  //   outfile.open("TRGnames_HLT.txt");
  //   for( unsigned int i = 0; i < tr.size(); i++ ){
  //     outfile<<TString(triggernames[i]) << std::endl;
  //   }
  //   outfile.close();
  
  for( unsigned int i = 0; i < tr.size(); i++ ){
    hltbits->push_back(tr[i].accept() ? 1:0);
  }

  for (unsigned int l1bit = 0; l1bit < NmaxL1AlgoBit; ++l1bit) {    
    l1physbits->push_back(l1GtReadoutRecord->decisionWord()[l1bit] ? 1:0);
  }  

  for (unsigned int l1bit = 0; l1bit < NmaxL1TechBit; ++l1bit) {    
    l1techbits->push_back(l1GtReadoutRecord->technicalTriggerWord()[l1bit] ? 1:0);
  }

  iEvent.put( hltbits,    "hltbits" );
  iEvent.put( l1physbits, "l1physbits" );
  iEvent.put( l1techbits, "l1techbits" );

  //in case nBxOutput==3
  if (nBxOutput==3 && l1GtReadoutRecord->gtFdlVector().size()>=3) {

    for (unsigned int l1bit = 0; l1bit < NmaxL1AlgoBit; ++l1bit) {    
      l1physbitsm1->push_back(l1GtReadoutRecord->decisionWord(-1)[l1bit] ? 1:0);
      l1physbitsp1->push_back(l1GtReadoutRecord->decisionWord(1)[l1bit] ? 1:0);
    }  
    
    for (unsigned int l1bit = 0; l1bit < NmaxL1TechBit; ++l1bit) {    
      l1techbitsm1->push_back(l1GtReadoutRecord->technicalTriggerWord(-1)[l1bit] ? 1:0);
      l1techbitsp1->push_back(l1GtReadoutRecord->technicalTriggerWord(1)[l1bit] ? 1:0);
    }
    
    iEvent.put( l1physbitsm1, "l1physbitsm1" );
    iEvent.put( l1techbitsm1, "l1techbitsm1" );
    iEvent.put( l1physbitsp1, "l1physbitsp1" );
    iEvent.put( l1techbitsp1, "l1techbitsp1" );

  }

  //in case storeByName
  if (storeByName) {
    edm::ESHandle<L1GtTriggerMenu> menuRcd;
    iSetup.get<L1GtTriggerMenuRcd>().get(menuRcd) ;
    const L1GtTriggerMenu* menu = menuRcd.product();
    
    for (unsigned int i=0;i<algoNameList.size();i++) {
      //if trigger is not found, function returns false => vetoing isn't necessarily reliable.
      std::auto_ptr<bool> algoDecision(new bool( menu->gtAlgorithmResult(algoNameList[i],l1GtReadoutRecord->decisionWord()) ) );
      iEvent.put(algoDecision,algoNameListNoUnderScores[i]);
    }
    
    //keep track of which triggers are present in the data
    if (menusAlreadyParsed.count(menu->gtTriggerMenuName())==0) {
      const AlgorithmMap& algoMap=menu->gtAlgorithmMap();
      for (AlgorithmMap::const_iterator it=algoMap.begin();it!=algoMap.end();++it) {
	algosPresentInData.insert((*it).first);
      }
      menusAlreadyParsed.insert(menu->gtTriggerMenuName());
    }

  }//end if(StoreByName)

}

void PromptAna_Trigger::fillAlgoNameListNoUnderScores()
{
  std::string underScore="_";
  for (std::vector<std::string>::const_iterator it=algoNameList.begin();it!=algoNameList.end();++it) {
    std::string nameNoUnderScores;
    for (std::string::const_iterator it2=it->begin();it2!=it->end();++it2) {
      if ((*it2)!=underScore[0]) nameNoUnderScores.push_back(*it2);
    }
    algoNameListNoUnderScores.push_back(nameNoUnderScores);
  }
}
