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

HcalEventPositionMonitor::~HcalEventPositionMonitor() {}

void HcalEventPositionMonitor::clearME(){
  if(m_dbe){
    m_dbe->setCurrentFolder("TBMonitor/EventPositionMonitor");
    m_dbe->removeContents();     
  }
  return;
}

void HcalEventPositionMonitor::setup(const edm::ParameterSet& ps, DaqMonitorBEInterface* dbe){
  HcalBaseMonitor::setup(ps,dbe);

  occThresh_ = ps.getUntrackedParameter<double>("EventPositionOccThresh", 1.0);
  cout << "EventPosition occupancy threshold set to " << occThresh_ << endl;

  ievt_=0;

  for(char i = 'A'; i<='H'; i++)
    layers_.push_back(i);
  
  if ( m_dbe ) {
    m_dbe->setCurrentFolder("TBMonitor/EventPositionMonitor");
    char name[250];
    for(char i = 'A'; i<='H'; i++){
      sprintf(name,"Wire Chamber %c Hits",i);
      MonitorElement* me1 = m_dbe->book2D(name,name,100,-50,50,100,-50,50);
      mePlanes_.push_back(me1);

      sprintf(name,"Wire Chamber %c X Hits",i);
      MonitorElement* me2 = m_dbe->book1D(name,name,100,-50,50);
      meXvalue_.push_back(me2);

      sprintf(name,"Wire Chamber %c # X Hits",i);
      MonitorElement* me3 = m_dbe->book1D(name,name,5,-0.5,4.5);
      meXhits_.push_back(me3);

      sprintf(name,"Wire Chamber %c Y Hits",i);
      MonitorElement* me4 = m_dbe->book1D(name,name,100,-50,50);
      meYvalue_.push_back(me4);

      sprintf(name,"Wire Chamber %c # Y Hits",i);
      MonitorElement* me5 = m_dbe->book1D(name,name,5,-0.5,4.5);
      meYhits_.push_back(me5);
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
    
    for(unsigned int h=0; h<xh.size(); h++) {
      meXhits_[i]->Fill(xh.size());
      meYhits_[i]->Fill(yh.size());
    }

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

  }

  return;
}

