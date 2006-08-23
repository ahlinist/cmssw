#include "RecoTBCalo/DQMTBCalo/interface/HcalQADCMonitor.h"

HcalQADCMonitor::HcalQADCMonitor() {
  occThresh_ = 1.0;
  ievt_=0;
}

HcalQADCMonitor::~HcalQADCMonitor() {

  if(m_dbe){
    m_dbe->setCurrentFolder("TBMonitor/QADCMonitor");
    m_dbe->removeContents();
  }

}

void HcalQADCMonitor::setup(const edm::ParameterSet& ps, DaqMonitorBEInterface* dbe){
  HcalBaseMonitor::setup(ps,dbe);

  occThresh_ = ps.getUntrackedParameter<double>("QADCOccThresh", 1.0);
  cout << "QADC occupancy threshold set to " << occThresh_ << endl;

  ievt_=0;

  if ( m_dbe ) {
    m_dbe->setCurrentFolder("TBMonitor/QADCMonitor");
    meEVT_ = m_dbe->bookInt("QADC Event Number");    
    meEVT_->Fill(ievt_);

    meCHER1_ = m_dbe->book1D("Cherenkov QADC 1","Cherenkov QADC 1",2000,0,2000);
    meCHER2_ = m_dbe->book1D("Cherenkov QADC 2","Cherenkov QADC 2",2000,0,2000);
    meCHER3_ = m_dbe->book1D("Cherenkov QADC 3","Cherenkov QADC 3",2000,0,2000);

    meTOF1_ = m_dbe->book1D("TOF QADC 1","TOF QADC 1",2000,0,2000);
    meTOF2_ = m_dbe->book1D("TOF QADC 2","TOF QADC 2",2000,0,2000);

  ///Book Scintillators: at 521m, at 522?m, in VLE beam line
    meSci521_ = m_dbe->book1D("Scintillator 521m","Scintillator 521m",4100,0,4100);
    meSci528_ = m_dbe->book1D("Scintillator 528m","Scintillator 528m",4100,0,4100);
    meSciVLE_ = m_dbe->book1D("Scintillator in VLE beam line","Scintillator in VLE beam line",4100,0,4100);

  ///Book Trigger Scintillators: 14x14 cm, 4x4 cm, 2x2 cm, 14x14 cm
    meTrigSc1_ = m_dbe->book1D("Trigger Scint. 1","Trigger Scint. 1",1000,0,1000); 
    meTrigSc2_ = m_dbe->book1D("Trigger Scint. 2","Trigger Scint. 2",1000,0,1000); 
    meTrigSc3_ = m_dbe->book1D("Trigger Scint. 3","Trigger Scint. 3",1000,0,1000); 
    meTrigSc4_ = m_dbe->book1D("Trigger Scint. 4","Trigger Scint. 4",1000,0,1000); 


  ///Book VM front and back
    meVMF_ = m_dbe->book1D("VM front","VM front",4100,0,4100);
    meVMB_ = m_dbe->book1D("VM back","VM back",4100,0,4100);

  ///Book Muon veto wall
    meVMu1_ = m_dbe->book1D("Muon veto wall 1","Muon veto wall 1",4100,0,4100);
    meVMu2_ = m_dbe->book1D("Muon veto wall 2","Muon veto wall 2",4100,0,4100);
    meVMu3_ = m_dbe->book1D("Muon veto wall 3","Muon veto wall 3",4100,0,4100);
    meVMu4_ = m_dbe->book1D("Muon veto wall 4","Muon veto wall 4",4100,0,4100);
    meVMu5_ = m_dbe->book1D("Muon veto wall 5","Muon veto wall 5",4100,0,4100);
    meVMu6_ = m_dbe->book1D("Muon veto wall 6","Muon veto wall 6",4100,0,4100);
    meVMu7_ = m_dbe->book1D("Muon veto wall 7","Muon veto wall 7",4100,0,4100);
    meVMu8_ = m_dbe->book1D("Muon veto wall 8","Muon veto wall 8",4100,0,4100);

  ///Book Beam halo info: (up, left, right, down)- this was wrongly listed as adc
    // in the TBDataFormats/HcalTBObjects/interface/HcalTBBeamCounters.h
    //    meBHalo1_ = m_dbe->book1D("Beam Halo up","Beam Halo up",4100,0,4100);
    //    meBHalo2_ = m_dbe->book1D("Beam Halo left","Beam Halo left",4100,0,4100); 
    //    meBHalo3_ = m_dbe->book1D("Beam Halo right","Beam Halo right",4100,0,4100); 
    //    meBHalo4_ = m_dbe->book1D("Beam Halo down","Beam Halo down",4100,0,4100); 

  }

  return;
}

void HcalQADCMonitor::processEvent(const HcalTBBeamCounters& qadc){

  if(!m_dbe) { printf("HcalQADCMonitor::processEvent   DaqMonitorBEInterface not instantiated!!!\n");  return; }

  ievt_++;
  meEVT_->Fill(ievt_);

  ///Fill Cherenkov values
  meCHER1_->Fill(qadc.CK1adc());
  meCHER2_->Fill(qadc.CK2adc());
  meCHER3_->Fill(qadc.CK3adc());

  ///Fill TOF QADC values
  meTOF1_->Fill(qadc.TOF1adc());
  meTOF2_->Fill(qadc.TOF2adc());

  ///Fill Scintillators: at 521m, at 522?m, in VLE beam line
  meSci521_->Fill(qadc.Sci521adc());
  meSci528_->Fill(qadc.Sci528adc());
  meSciVLE_->Fill(qadc.SciVLEadc());

  ///Fill Trigger Scintillators: 14x14 cm, 4x4 cm, 2x2 cm, 14x14 cm
  meTrigSc1_->Fill(qadc.S1adc()); 
  meTrigSc2_->Fill(qadc.S2adc()); 
  meTrigSc3_->Fill(qadc.S3adc()); 
  meTrigSc4_->Fill(qadc.S4adc()); 

  ///Fill VM front and back
  meVMF_->Fill(qadc.VMFadc());
  meVMB_->Fill(qadc.VMBadc());

  ///Fill Muon veto wall
  meVMu1_->Fill(qadc.VM1adc());
  meVMu2_->Fill(qadc.VM2adc());
  meVMu3_->Fill(qadc.VM3adc());
  meVMu4_->Fill(qadc.VM4adc());
  meVMu5_->Fill(qadc.VM5adc());
  meVMu6_->Fill(qadc.VM6adc());
  meVMu7_->Fill(qadc.VM7adc());
  meVMu8_->Fill(qadc.VM8adc());

  ///Fill Beam halo info: (up, left, right, down)
  //  meBHalo1_->Fill(qadc.BH1adc());
  //  meBHalo2_->Fill(qadc.BH2adc()); 
  //  meBHalo3_->Fill(qadc.BH3adc()); 
  //  meBHalo4_->Fill(qadc.BH4adc()); 

  return;
}

