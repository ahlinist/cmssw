#include "RecoTBCalo/DQMTBCalo/interface/HcalTimingMonitor.h"

HcalTimingMonitor::HcalTimingMonitor() {
  occThresh_ = 1.0;
  ievt_=0;

}

HcalTimingMonitor::~HcalTimingMonitor() {
  
  if(m_dbe){
    m_dbe->setCurrentFolder("TBMonitor/TimingMonitor");
    m_dbe->removeContents();
  }

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

    meTOF1_ = m_dbe->book1D("TOF TDC 1","TOF TDC 1",4100,0,4100);
    meTOF2_ = m_dbe->book1D("TOF TDC 2","TOF TDC 2",4100,0,4100);
    meTOFT_ = m_dbe->book1D("TOF Time","TOF Time",100,0,100);

    //    meBHal1_ = m_dbe->book1D("Beam Halo 1","Beam Halo 1",4100,0,4100);

  }

  return;
}

void HcalTimingMonitor::processEvent(const HcalTBTiming& time){

  if(!m_dbe) { printf("HcalTimingMonitor::processEvent   DaqMonitorBEInterface not instantiated!!!\n");  return; }

  ievt_++;
  if(meEVT_) meEVT_->Fill(ievt_);

  meTOF1_->Fill(time.TOF1time());
  meTOF2_->Fill(time.TOF2time());
  meTOFT_->Fill(fabs(time.TOF1time() - time.TOF2time()));

    //    meBHal1_->Fill(time.bh1hits());  ?

  return;
}

