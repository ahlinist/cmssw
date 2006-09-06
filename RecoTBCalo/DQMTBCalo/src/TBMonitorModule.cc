#include <RecoTBCalo/DQMTBCalo/src/TBMonitorModule.h>

/*
 * \file TBMonitorModule.cc
 * 
 * $Date: 2006/08/24 23:45:15 $
 * $Revision: 1.2 $
 * \author W Fisher
 *
*/

TBMonitorModule::TBMonitorModule(const edm::ParameterSet& ps){

  m_logFile.open("TBMonitorModule.log");

  m_dbe = 0;
  if ( ps.getUntrackedParameter<bool>("DaqMonitorBEInterface", false) ) {
    m_dbe = edm::Service<DaqMonitorBEInterface>().operator->();
    m_dbe->setVerbose(0);
  }

  m_monitorDaemon = false;
  if ( ps.getUntrackedParameter<bool>("MonitorDaemon", false) ) {
    edm::Service<MonitorDaemon> daemon;
    daemon.operator->();
    m_monitorDaemon = true;
  }
  
  m_outputFile = ps.getUntrackedParameter<string>("outputFile", "");
  if ( m_outputFile == (string)("NoOutput") ) {
    cout << "TB Monitoring histograms will be not be saved..." << endl;    
    m_outputFile = "";
  }
  else if ( m_outputFile.size() != 0 ) {
    cout << "TB Monitoring histograms will be saved to " << m_outputFile.c_str() << endl;    
  }
  else{
    m_outputFile = "DQM_Hcal_TB_";
  }
  
  m_runNum = 0; m_meStatus=0;
  m_meRunNum=0; m_meRunType=0;
  m_meEvtNum=0; 
  
  if ( m_dbe ) {
    m_dbe->setCurrentFolder("TBMonitor");
    m_meStatus  = m_dbe->bookInt("STATUS");
    m_meRunNum  = m_dbe->bookInt("RUN NUMBER");
    m_meRunType = m_dbe->bookInt("RUN TYPE");
    m_meEvtNum  = m_dbe->bookInt("EVT NUMBER");
    
    m_meStatus->Fill(-1);
    m_meRunNum->Fill(0);
    m_meRunType->Fill(-1);
    m_meEvtNum->Fill(-1);
  }
  
  m_timeMon = NULL; m_qadcMon = NULL; m_eposMon = NULL; 

  if ( ps.getUntrackedParameter<bool>("EventPositionMonitor", false) ) {
    m_eposMon = new HcalEventPositionMonitor();
    m_eposMon->setup(ps, m_dbe);
  }

  if ( ps.getUntrackedParameter<bool>("QADCMonitor", false) ) {
    m_qadcMon = new HcalQADCMonitor();
    m_qadcMon->setup(ps, m_dbe);
  }

  if ( ps.getUntrackedParameter<bool>("TimingMonitor", false) ) {
    m_timeMon = new HcalTimingMonitor();
    m_timeMon->setup(ps, m_dbe);
  }

  offline_ = ps.getUntrackedParameter<bool>("OffLine", false);

  if ( m_dbe ) m_dbe->showDirStructure();
  
}

TBMonitorModule::~TBMonitorModule(){

  if ( m_dbe && !offline_) {
    if(m_timeMon!=NULL) {  m_timeMon->clearME(); }
    if(m_eposMon!=NULL) {  m_eposMon->clearME(); }
    if(m_qadcMon!=NULL) {  m_qadcMon->clearME(); }
    m_dbe->setCurrentFolder("TBMonitor");
    m_dbe->removeContents();
  }

  if(m_timeMon!=NULL) { delete m_timeMon; m_timeMon=NULL; }
  if(m_eposMon!=NULL) { delete m_eposMon; m_eposMon=NULL; }
  if(m_qadcMon!=NULL) { delete m_qadcMon; m_qadcMon=NULL; }

  m_logFile.close();

}

void TBMonitorModule::beginJob(const edm::EventSetup& c){
  m_ievt = 0;

  if ( m_meStatus ) m_meStatus->Fill(0);
  if ( m_meEvtNum ) m_meEvtNum->Fill(m_ievt);
  if ( offline_ ) sleep(15);
  return;
}

void TBMonitorModule::endJob(void) {
  
  cout << "TBMonitorModule::endjob, analyzed " << m_ievt << " events" << endl;
  
  if ( m_meStatus ) m_meStatus->Fill(2);
  if ( m_meRunNum ) m_meRunNum->Fill(m_runNum);
  if ( m_meEvtNum ) m_meEvtNum->Fill(m_ievt);
  if ( offline_ ) sleep(5);

  if(m_timeMon) m_timeMon->done();
  if(m_eposMon) m_eposMon->done();
  if(m_qadcMon) m_qadcMon->done();

  char tmp[150]; bool update = true;
  for ( unsigned int i = 0; i < m_outputFile.size(); i++ ) {
    if ( m_outputFile.substr(i, 5) == ".root" )  {
      update = false;
    }
  }
  string saver = m_outputFile;
  if(update){
    sprintf(tmp,"%09d.root", m_runNum);
    saver = m_outputFile+tmp;
  }
  if ( m_outputFile.size() != 0  && m_dbe ) m_dbe->save(saver);
  
  return;
}

void TBMonitorModule::analyze(const edm::Event& e, const edm::EventSetup& eventSetup){

  // Do default setup...
  m_ievt++;
  
  edm::EventID id_ = e.id();
  m_runNum = (int)(id_.run());

  if ( m_dbe ){ 
    m_meStatus->Fill(1);
    m_meRunNum->Fill(m_runNum);
    m_meEvtNum->Fill(m_ievt);
  }
  
  // To get information from the event setup, you must request the "Record"
  // which contains it and then extract the object you need

  ///////////////////////////////////////////////////////////////////////
  ///  TB06 Specific Tasks                                           ////
  ///////////////////////////////////////////////////////////////////////
  edm::Handle<HcalTBBeamCounters> qadc;
  try{ e.getByType(qadc); } catch(...) { }
  edm::Handle<HcalTBEventPosition> epos;
  try{ e.getByType(epos); } catch(...) {} 
  edm::Handle<HcalTBTiming> time;
  try{ e.getByType(time); } catch(...) { }

  // Timing monitor task
  if((m_timeMon!=NULL)) m_timeMon->processEvent(*time);
  // QADC monitor task
  if((m_qadcMon!=NULL)) m_qadcMon->processEvent(*qadc);
  // EventPosition monitor task
  if((m_eposMon != NULL)) m_eposMon->processEvent(*epos);

  if(m_ievt%1000 == 0)
    cout << "TBMonitorModule: analyzed " << m_ievt << " events" << endl;

  //  if(offline_) usleep(30);

  return;
}

// Here are the necessary incantations to declare your module to the
// framework, so it can be referenced in a cmsRun file.
//
#include "PluginManager/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(TBMonitorModule)

