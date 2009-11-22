#include "JetMETAnalysis/PromptAnalysis/interface/PromptAna_Event.h"
#include "FWCore/Framework/interface/Event.h"


PromptAna_Event::PromptAna_Event(const edm::ParameterSet& iConfig)
  : TRGTag(iConfig.getParameter<edm::InputTag>("TRGTag"))
{
  produces <unsigned int> ( "run"   );
  produces <unsigned int> ( "event" );
  produces <unsigned int> ( "bunch" );
  produces <unsigned int> ( "ls"    );
  produces <unsigned int> ( "orbit" );
  produces <double>       ( "time" );   
  produces <std::vector<int> > ( "TriggerResults" );
  //  produces <std::vector<std::string> > ( "Test" );
}

void PromptAna_Event::
produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  std::auto_ptr<unsigned int >  run   ( new unsigned int(iEvent.id().run()        ) );
  std::auto_ptr<unsigned int >  event ( new unsigned int(iEvent.id().event()      ) );
  std::auto_ptr<unsigned int >  ls    ( new unsigned int(iEvent.luminosityBlock() ) );

  double sec  = iEvent.time().value() >> 32 ;
  double usec = 0xFFFFFFFF & iEvent.time().value();
  double conv = 1e6;

  std::auto_ptr<unsigned int >  bunch ( new unsigned int(iEvent.bunchCrossing()   ) );
  std::auto_ptr<unsigned int >  orbit ( new unsigned int(iEvent.orbitNumber()     ) );
  std::auto_ptr<double >        time  ( new double(sec+usec/conv));
  std::auto_ptr<std::vector<int> >  triggerresults ( new std::vector<int>()  ) ;
  //  std::auto_ptr<std::vector<std::string> >  test( new std::vector<std::string>()  ) ;


 // Dump HLT trigger bits
  edm::Handle<edm::TriggerResults> triggers;
  iEvent.getByLabel(TRGTag, triggers);
  edm::TriggerResults tr = *triggers;

  std::vector<std::string> triggernames;
  edm::Service<edm::service::TriggerNamesService> tns;
  tns->getTrigPaths(*triggers, triggernames);
  
  /*    ofstream  outfile;
    outfile.open("TRGnames.txt");
    for( unsigned int i = 0; i < tr.size(); i++ ){
      outfile<<TString(triggernames[i]) << std::endl;
    }
    outfile.close();*/

  for( unsigned int i = 0; i < tr.size(); i++ ){
    triggerresults->push_back(tr[i].accept() ? 1:0);
  }


  iEvent.put( run,   "run"   );
  iEvent.put( event, "event" );
  iEvent.put( ls   , "ls"    );
  iEvent.put( bunch, "bunch" );
  iEvent.put( orbit, "orbit" );
  iEvent.put( time,  "time"  );
  iEvent.put( triggerresults, "TriggerResults"  );
  //  iEvent.put( test, "Test"  );

}
