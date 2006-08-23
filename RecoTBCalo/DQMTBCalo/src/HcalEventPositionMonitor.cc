#include "RecoTBCalo/DQMTBCalo/interface/HcalEventPositionMonitor.h"

HcalEventPositionMonitor::HcalEventPositionMonitor() {
  occThresh_ = 1.0;
  ievt_=0;

  xhits_.clear();
  yhits_.clear();
  layers_.clear();
  mePlanes_.clear();
  meXvalue_.clear();
  meYvalue_.clear();

}

HcalEventPositionMonitor::~HcalEventPositionMonitor() {
  
  if(m_dbe){
    m_dbe->setCurrentFolder("TBMonitor/EventPositionMonitor");
    m_dbe->removeContents();     
  }
}


void HcalEventPositionMonitor::setup(const edm::ParameterSet& ps, DaqMonitorBEInterface* dbe){
  HcalBaseMonitor::setup(ps,dbe);

  occThresh_ = ps.getUntrackedParameter<double>("EventPositionOccThresh", 1.0);
  cout << "EventPosition occupancy threshold set to " << occThresh_ << endl;

  ievt_=0;

  for(char i = 'A'; i<='E'; i++)
    layers_.push_back(i);
  
  if ( m_dbe ) {
    m_dbe->setCurrentFolder("TBMonitor/EventPositionMonitor");
    char name[250];
    for(char i = 'A'; i<='E'; i++){
      sprintf(name,"Wire Chamber %c Hits",i);
      MonitorElement* me = m_dbe->book2D(name,name,100,-50,50,100,-50,50);
      mePlanes_.push_back(me);
    }
    for(char i = 'A'; i<='E'; i++){
      sprintf(name,"Wire Chamber %c X-Hits",i);
      MonitorElement* me = m_dbe->book1D(name,name,100,-50,50);
      meXvalue_.push_back(me);
    }
    for(char i = 'A'; i<='E'; i++){
      sprintf(name,"Wire Chamber %c Y-Hits",i);
      MonitorElement* me = m_dbe->book1D(name,name,100,-50,50);
      meYvalue_.push_back(me);
    }
    
    meEVT_ = m_dbe->bookInt("Event Position Event Number");    
    meEVT_->Fill(ievt_);
  }

  return;
}

void HcalEventPositionMonitor::processEvent(const HcalTBEventPosition& epos){

  if(!m_dbe) { printf("HcalEventPositionMonitor::processEvent   DaqMonitorBEInterface not instantiated!!!\n");  return; }

  ievt_++;
  if(meEVT_) meEVT_->Fill(ievt_);

  xhits_.clear(); yhits_.clear();
  for(unsigned int i=0; i<layers_.size(); i++){
    vector<double> xh,yh;
    epos.getChamberHits(layers_[i],xh,yh);
    
    ///For now, assume that there are only matched pairs
    if(xh.size()!=yh.size()){
      printf("Mismatched wire chamber vector size: %d, %d\n",xh.size(),yh.size());
      continue;
    }
    
    for(unsigned int h=0; h<xh.size(); h++) {
      mePlanes_[i]->Fill(xh[h],yh[h]);
      meXvalue_[i]->Fill(xh[h]);
      meYvalue_[i]->Fill(yh[h]);
    }

    //    xhits_.push_back(xh);
    //    yhits_.push_back(yh);
  }

  return;
}

