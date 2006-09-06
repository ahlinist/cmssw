#include "RecoTBCalo/DQMTBCalo/interface/HcalTimingMonitor.h"

HcalTimingMonitor::HcalTimingMonitor() {
  occThresh_ = 1.0;
  ievt_=0;

}

HcalTimingMonitor::~HcalTimingMonitor() {}

void HcalTimingMonitor::clearME(){
  if(m_dbe){
    m_dbe->setCurrentFolder("TBMonitor/TimingMonitor");
    m_dbe->removeContents();
  }
  return;
}

void HcalTimingMonitor::setup(const edm::ParameterSet& ps, DaqMonitorBEInterface* dbe){
  HcalBaseMonitor::setup(ps,dbe);

  occThresh_ = ps.getUntrackedParameter<double>("TimingOccThresh", 1.0);
  cout << "Timing occupancy threshold set to " << occThresh_ << endl;

  ievt_=0;

  if ( m_dbe ) {
    m_dbe->setCurrentFolder("TBMonitor/TimingMonitor");
    meEVT_ = m_dbe->bookInt("Timing Event Number");    
    meEVT_->Fill(ievt_);

    meTOF1S_ = m_dbe->book1D("TOF TDC 1 - Saleve","TOF TDC 1 - Saleve",4100,0,4100);
    meTOF2S_ = m_dbe->book1D("TOF TDC 2 - Saleve","TOF TDC 2 - Saleve",4100,0,4100);
    meTOFTS_ = m_dbe->book1D("TOF Time - Saleve","TOF Time - Saleve",100,0,500);
    meTOF1J_ = m_dbe->book1D("TOF TDC 1 - Jura","TOF TDC 1 - Jura",4100,0,4100);
    meTOF2J_ = m_dbe->book1D("TOF TDC 2 - Jura","TOF TDC 2 - Jura",4100,0,4100);
    meTOFTJ_ = m_dbe->book1D("TOF Time - Jura","TOF Time - Jura",100,0,500);
    meTOF_DT1_ = m_dbe->book1D("TOF TDC - Delta 1","TOF TDC - Delta 1",100,0,500);
    meTOF_DT2_ = m_dbe->book1D("TOF TDC - Delta 2","TOF TDC - Delta 2",100,0,500);
    meTOF_DT_ = m_dbe->book1D("TOF Time - Delta","TOF Time - Delta",100,0,500);

    meTrigger_ = m_dbe->book1D("Trigger Timing","Trigger Time",600,2400,2700);
    meTTCL1A_ = m_dbe->book1D("TTC L1A Timing","TTC L1A Time",600,3300,3600);
    meBeamCoinc_ = m_dbe->book1D("Beam Coincidence Timing","Beam Coincidence Time",600,2200,2500);

    meDT_[0] = m_dbe->book1D("Trigger-L1A Phase","Trigger-L1A Phase",400,-900,-700);
    meDT_[1] = m_dbe->book1D("Trigger-BC Phase","Trigger-BC Phase",400,100,300);
    meDT_[2] = m_dbe->book1D("L1A-BC Phase","L1A-BC Phase",400,1000,1200);

  }

  return;
}

void HcalTimingMonitor::processEvent(const HcalTBTiming& time){

  if(!m_dbe) { printf("HcalTimingMonitor::processEvent   DaqMonitorBEInterface not instantiated!!!\n");  return; }

  ievt_++;
  if(meEVT_) meEVT_->Fill(ievt_);

  meTOF1S_->Fill(time.TOF1Stime());
  meTOF2S_->Fill(time.TOF2Stime());
  meTOFTS_->Fill(fabs(time.TOF1Stime() - time.TOF2Stime()));
  meTOF1J_->Fill(time.TOF1Jtime());
  meTOF2J_->Fill(time.TOF2Jtime());
  meTOFTJ_->Fill(fabs(time.TOF1Jtime() - time.TOF2Jtime()));
  meTOF_DT_->Fill(fabs(time.TOF1Stime() - time.TOF2Stime())-fabs(time.TOF1Jtime() - time.TOF2Jtime()));

  meTOF_DT1_->Fill(time.TOF1Stime() - time.TOF1Jtime());
  meTOF_DT2_->Fill(time.TOF2Stime() - time.TOF2Jtime());

  meTrigger_->Fill(time.triggerTime());
  meTTCL1A_->Fill(time.ttcL1Atime());
  if(time.BeamCoincidenceCount())
   {
  meBeamCoinc_->Fill(time.BeamCoincidenceHits(0));
  meDT_[1]->Fill(time.triggerTime()-time.BeamCoincidenceHits(0));
  meDT_[2]->Fill(time.ttcL1Atime()-time.BeamCoincidenceHits(0));
   }
  meDT_[0]->Fill(time.triggerTime()-time.ttcL1Atime());

  return;
}

