// -*- C++ -*-
//
// Package:    CastorTBAnalysis
// Class:      CastorTBAnalysis
// 
/**\class CastorTBAnalysis CastorTBAnalysis.cc RecoTBCalo/CastorTBAnalysis/src/CastorTBAnalysis.cc

 Description: Castor TB analysis

 Implementation:
*/

#include "RecoTBCalo/CastorTBAnalysis/interface/CastorTBAnalysis.h"

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TStyle.h"
#include "TH1.h"
#include "TF1.h"

#include <sstream>
#include <iostream>
#include <vector>

// default constructor
CastorTBAnalysis::CastorTBAnalysis()
    : runNumber_(0),
      eventNumber_(0),
      beamTrigger_(false),
      fakeTrigger_(false),
      inSpill_(false),
      inSpillPedestalTrigger_(false),
      laserTrigger_(false),
      outSpillPedestalTrigger_(false),
      spillIgnorantPedestalTrigger_(false)
{}

CastorTBAnalysis::~CastorTBAnalysis()
{}

void CastorTBAnalysis::setup(const std::string& outFileName_)
{	
    TString name = outFileName_;
    outFile_ = new TFile(name, "RECREATE"); 

    CastorAna = new TTree("CastorAnaTree", "CastorAnaTree");
    
    
    CastorAna->Branch("Castor",0,"run/I:event/I:beamTrigger/I:fakeTrigger/I:"
		     "outSpillPedestalTrigger/I:inSpillPedestalTrigger/I:"
		     "SpillIgnorantPedestalTrigger/I:"
		     "laserTrigger/I:ledTrigger/I:"
		     "triggerTime/D");
    CastorAna->Branch("Chambers",0,"WCAxsize/I:WCAx[5]/D:WCAysize/I:WCAy[5]/D:"
                      "WCBxsize/I:WCBx[5]/D:WCBysize/I:WCBy[5]/D:"
                      "WCCxsize/I:WCCx[5]/D:WCCysize/I:WCCy[5]/D:"
		      "WCDxsize/I:WCDx[5]/D:WCDysize/I:WCDy[5]/D:"
		      "WCExsize/I:WCEx[5]/D:WCEysize/I:WCEy[5]/D:"
                      "UMVadc/D:VMBadc/D");

    //add cerenkov counters
    CastorAna->Branch("Cerenkov", 0, "CK1adc/D:CK2adc/D:CK3adc/D");

    CastorAna->Branch("More",0,"beamCoincidenceCounts/I:beamCoincidenceHitsTime[5]/D:"
                               "BH1upCounts/I:BH1upHitsTime[5]/D:BH1upAdc/D:"
                               "BH2leftCounts/I:BH2leftHitsTime[5]/D:BH2leftAdc/D:"
                               "BH3rightCounts/I:BH3rightHitsTime[5]/D:BH3rightAdc/D:"
                               "BH4downCounts/I:BH4downHitsTime[5]/D:BH4downAdc/D:"
                               "S1Counts/I:S1_1212_HitsTime[5]/D:S1_1212_Adc/D:"
                               "S2Counts/I:S2_44_HitsTime[5]/D:S2_44_Adc/D:"
                               "S3Counts/I:S3_22_HitsTime[5]/D:S3_22_Adc/D:"
                               "S4Counts/I:S4_1212_HitsTime[5]/D:S4_1212_Adc/D:"
                               "TOF2JuraTime/D:TOF2JuraAdc/D:TOF2SaleveTime/D:TOF2SaleveAdc");

    CastorAna->GetBranch("Castor")->SetAddress(&castor);
    CastorAna->GetBranch("Chambers")->SetAddress(&chambers);

    //add cerenkov counters
    CastorAna->GetBranch("Cerenkov")->SetAddress(&cerenkov);

    CastorAna->GetBranch("More")->SetAddress(&more);
    CastorAna->SetAutoSave();
}

void CastorTBAnalysis::analyze(const HcalTBTriggerData& triggerData,
			       const HcalTBTiming& timing,
			       const HcalTBBeamCounters& beamCounters,
			       const HcalTBEventPosition& eventPosition)
{
    //Initialise

    castor.runNo = -999;
    castor.eventNo = -999;
    castor.beamTrigger = -999;
    castor.fakeTrigger = -999;
    castor.outSpillPedestalTrigger = -999;
    castor.inSpillPedestalTrigger = -999;
    castor.SpillIgnorantPedestalTrigger = -999;
    castor.laserTrigger = -999;
    castor.ledTrigger = -999;
    castor.triggerTime = -999.;


    chambers.WCAxsize = -999;
    for ( unsigned int i = 0; i < MAXHIT; ++i ) chambers.WCAx[i] = -999.;
    chambers.WCAysize = -999;
    for ( unsigned int i = 0; i < MAXHIT; ++i ) chambers.WCAy[i] = -999.;

    chambers.WCBxsize = -999;
    for ( unsigned int i = 0; i < MAXHIT; ++i ) chambers.WCBx[i] = -999.;
    chambers.WCBysize = -999;
    for ( unsigned int i = 0; i < MAXHIT; ++i ) chambers.WCBy[i] = -999.;

    chambers.WCCxsize = -999;
    for ( unsigned int i = 0; i < MAXHIT; ++i ) chambers.WCCx[i] = -999.;
    chambers.WCCysize = -999;
    for ( unsigned int i = 0; i < MAXHIT; ++i ) chambers.WCCy[i] = -999.;

    chambers.WCDxsize = -999;
    for ( unsigned int i = 0; i < MAXHIT; ++i ) chambers.WCDx[i] = -999.;
    chambers.WCDysize = -999;
    for ( unsigned int i = 0; i < MAXHIT; ++i ) chambers.WCDy[i] = -999.;

    chambers.WCExsize = -999;
    for ( unsigned int i = 0; i < MAXHIT; ++i ) chambers.WCEx[i] = -999.;
    chambers.WCEysize = -999;
    for ( unsigned int i = 0; i < MAXHIT; ++i ) chambers.WCEy[i] = -999.;

    chambers.VMBadc = -999.;
    chambers.UMVadc = -999.;

    more.beamCoincidenceCounts = -999;
    for ( unsigned int i = 0; i < MAXHIT; ++i ) more.beamCoincidenceHitsTime[i] = -999.0;

    more.BH1upCounts = -999;
    for ( unsigned int i = 0; i < MAXHIT; ++i ) more.BH1upHitsTime[i] = -999.0;
    more.BH1upAdc = -999.0;

    more.BH2leftCounts = -999;
    for ( unsigned int i = 0; i < MAXHIT; ++i ) more.BH2leftHitsTime[i] = -999.0;
    more.BH2leftAdc = -999.0;
    
    more.BH3rightCounts = -999;
    for ( unsigned int i = 0; i < MAXHIT; ++i ) more.BH3rightHitsTime[i] = -999.0;
    more.BH3rightAdc = -999.0;

    more.BH4downCounts = -999;
    for ( unsigned int i = 0; i < MAXHIT; ++i ) more.BH4downHitsTime[i] = -999.0;
    more.BH4downAdc = -999.0;

    more.S1Counts = -999;
    for ( unsigned int i = 0; i < MAXHIT; ++i ) more.S1_1212_HitsTime[i] = -999.0;
    more.S1_1212_Adc = -999.0;

    more.S2Counts = -999;
    for ( unsigned int i = 0; i < MAXHIT; ++i ) more.S2_44_HitsTime[i] = -999.0;
    more.S2_44_Adc = -999.0;

    more.S3Counts = -999;
    for ( unsigned int i = 0; i < MAXHIT; ++i ) more.S3_22_HitsTime[i] = -999.0;
    more.S3_22_Adc = -999.0;
    
    more.S4Counts = -999;
    for ( unsigned int i = 0; i < MAXHIT; ++i ) more.S4_1212_HitsTime[i] = -999.0;
    more.S4_1212_Adc = -999.0;

    more.TOF2JuraTime = -999.0;
    more.TOF2JuraAdc = -999.0;
    more.TOF2SaleveTime = -999.0; 
    more.TOF2SaleveAdc = -999.0;

    // Get trigger information
    
    runNumber_ = triggerData.runNumber();
    eventNumber_ = triggerData.eventNumber();
    
    std::cout<<std::endl;
    std::cout<<"runNumber: "<< runNumber_
	     <<"  eventNumber: "<< eventNumber_ <<std::endl;

    castor.runNo = runNumber_;
    castor.eventNo = eventNumber_;    

    std::cout<<"Get trigger info"<<std::endl;
    beamTrigger_ = triggerData.wasBeamTrigger();
    fakeTrigger_ = triggerData.wasFakeTrigger();
    inSpill_ = triggerData.wasInSpill();
    inSpillPedestalTrigger_ = triggerData.wasInSpillPedestalTrigger();
    laserTrigger_ = triggerData.wasLaserTrigger();
    ledTrigger_ = triggerData.wasLEDTrigger();
    outSpillPedestalTrigger_ = triggerData.wasOutSpillPedestalTrigger();
    spillIgnorantPedestalTrigger_ = triggerData.wasSpillIgnorantPedestalTrigger();
    
    std::cout<<"beam trigger: "<< beamTrigger_ <<std::endl;
    std::cout<<"fake trigger: "<< fakeTrigger_ <<std::endl;
    std::cout<<"in spill: "<< inSpill_ <<std::endl;
    std::cout<<"in spill pedestal trigger: "<< inSpillPedestalTrigger_ <<std::endl;    
    std::cout<<"laser trigger: "<< laserTrigger_ <<std::endl;
    std::cout<<"led trigger: "<< ledTrigger_ <<std::endl;
    std::cout<<"out spill pedestal trigger: "<< outSpillPedestalTrigger_ <<std::endl;
    std::cout<<"spill ignorant pedestal trigger: "<< spillIgnorantPedestalTrigger_ <<std::endl;
    
    castor.beamTrigger = static_cast<int>(beamTrigger_);
    castor.fakeTrigger = static_cast<int>(fakeTrigger_);
    castor.outSpillPedestalTrigger = static_cast<int>(outSpillPedestalTrigger_);
    castor.inSpillPedestalTrigger = static_cast<int>(inSpillPedestalTrigger_);
    castor.SpillIgnorantPedestalTrigger = static_cast<int>(spillIgnorantPedestalTrigger_);
    castor.laserTrigger = static_cast<int>(laserTrigger_);
    castor.ledTrigger = static_cast<int>(ledTrigger_);

    // Get beam information: what we do with this information isn't clear yet
    // ;likely put into tree
    // Information on what these things are comes from the BeamLineMonitor class
    
    // Cherenkov values
    std::cout<<"Get Cherenkov info"<<std::endl;    
    double CK1adc = beamCounters.CK1adc();
    double CK2adc = beamCounters.CK2adc();
    double CK3adc = beamCounters.CK3adc();
    std::cout<<"CK1adc: "<< CK1adc <<"  "
	     <<"CK2adc: "<< CK2adc <<"  "
	     <<"CK3adc: "<< CK3adc <<std::endl;

cerenkov.CK1adc = CK1adc;
cerenkov.CK2adc = CK2adc;
cerenkov.CK3adc = CK3adc;

    // TOF values: Saleve side
    std::cout<<"Get TOF:Saleve info"<<std::endl;
    double TOF1Sadc = beamCounters.TOF1Sadc();
    double TOF2Sadc = beamCounters.TOF2Sadc();
    std::cout<<"TOF1Sadc: "<< TOF1Sadc <<"  "
	     <<"TOF2Sadc: "<< TOF2Sadc <<std::endl;
    
    // TOF values: Jura side
    std::cout<<"Get TOF:Jura info"<<std::endl;
    double TOF1Jadc = beamCounters.TOF1Jadc();
    double TOF2Jadc = beamCounters.TOF2Jadc();
    std::cout<<"TOF1Jadc: "<< TOF1Jadc <<"  "
	     <<"TOF2Jadc: "<< TOF2Jadc <<std::endl;

    more.TOF2JuraAdc   = TOF2Jadc;
    more.TOF2SaleveAdc = TOF2Sadc;
    

    // Trigger scintillators
    std::cout<<"Get trigger scintillators info"<<std::endl;
    double S1adc = beamCounters.S1adc();
    double S2adc = beamCounters.S2adc();
    double S3adc = beamCounters.S3adc();
    double S4adc = beamCounters.S4adc();
    std::cout<<"S1adc: "<< S1adc <<"  "
	     <<"S2adc: "<< S2adc <<"  "
	     <<"S3adc: "<< S3adc <<"  "
	     <<"S4adc: "<< S4adc <<std::endl;

    more.S1_1212_Adc = S1adc;
    more.S2_44_Adc   = S2adc;
    more.S3_22_Adc   = S3adc;
    more.S4_1212_Adc = S4adc;
    
    // Muon veto: front and back
    std::cout<<"Get muon veto info"<<std::endl;
    double VMFadc_ = beamCounters.VMFadc();
    double VMBadc_ = beamCounters.VMBadc();
    std::cout<<"VMFadc: "<< VMFadc_ <<"  "
	     <<"VMBadc: "<< VMBadc_ <<std::endl;

    chambers.VMBadc = VMBadc_;
    
    // Upstream muon veto
    std::cout<<"Get upstream muon veto"<<std::endl;
    double UMVadc_ = beamCounters.Sci528adc();
    std::cout<<"UMVadc: "<< UMVadc_ <<std::endl;

    chambers.UMVadc = UMVadc_;

    // More muon vetos
    std::cout<<"Get muon vetos"<<std::endl;
    double VM1adc = beamCounters.VM1adc();
    double VM2adc = beamCounters.VM2adc();
    double VM3adc = beamCounters.VM3adc();
    double VM4adc = beamCounters.VM4adc();
    double VM5adc = beamCounters.VM5adc();
    double VM6adc = beamCounters.VM6adc();
    double VM7adc = beamCounters.VM7adc();
    double VM8adc = beamCounters.VM8adc();
    std::cout<<"VM1adc: "<< VM1adc <<"  "
	     <<"VM2adc: "<< VM2adc <<"  "
	     <<"VM3adc: "<< VM3adc <<"  "
	     <<"VM4adc: "<< VM4adc <<"  "
	     <<"VM5adc: "<< VM5adc <<"  "
	     <<"VM6adc: "<< VM6adc <<"  "
	     <<"VM7adc: "<< VM7adc <<"  "
	     <<"VM8adc: "<< VM8adc <<std::endl;
    
    // Beam halo (up, left, right, down)
    std::cout<<"Get beam halo info"<<std::endl;
    double BH_up_adc = beamCounters.BH1adc();
    double BH_left_adc = beamCounters.BH2adc();
    double BH_right_adc = beamCounters.BH3adc();
    double BH_down_adc = beamCounters.BH4adc();
    std::cout<<"BH_up_adc: "<< BH_up_adc <<"  "
	     <<"BH_left_adc: "<< BH_left_adc <<"  "
	     <<"BH_right_adc: "<< BH_right_adc <<"  "
	     <<"BH_down_adc: "<< BH_down_adc <<std::endl;

    more.BH1upAdc    = BH_up_adc;
    more.BH2leftAdc  = BH_left_adc;
    more.BH3rightAdc = BH_right_adc;
    more.BH4downAdc  = BH_down_adc;
    
    // Get timing
    more.beamCoincidenceCounts = timing.BeamCoincidenceCount(); // No of hits from Beam Coincidence.
    if ( timing.BeamCoincidenceCount() > static_cast<int>(MAXHIT) ) 
      more.beamCoincidenceCounts = static_cast<int>(MAXHIT);
    for ( int i = 0; i < more.beamCoincidenceCounts; ++i ) {
      std::cout<<"BeamCoincidenceHits: "<< i <<" "<< timing.BeamCoincidenceHits(i) <<std::endl;  
      more.beamCoincidenceHitsTime[i] = timing.BeamCoincidenceHits(i); // Indexed hit time from Beam Coincidence.
    }	

    more.BH1upCounts = timing.BH1Count();
    if ( timing.BH1Count() > static_cast<int>(MAXHIT) )
      more.BH1upCounts = static_cast<int>(MAXHIT);
    for ( int i = 0; i < more.BH1upCounts; ++i ) 
      more.BH1upHitsTime[i] = timing.BH1Hits(i);

    more.BH2leftCounts = timing.BH2Count();
    if ( timing.BH2Count() >  static_cast<int>(MAXHIT) )
      more.BH2leftCounts = static_cast<int>(MAXHIT);
    for ( int i = 0; i < more.BH2leftCounts; ++i ) 
      more.BH2leftHitsTime[i] = timing.BH2Hits(i);

    more.BH3rightCounts = timing.BH3Count();
    if ( timing.BH3Count() > static_cast<int>(MAXHIT) )
      more.BH3rightCounts = static_cast<int>(MAXHIT);
    for ( int i = 0; i < more.BH3rightCounts; ++i ) 
      more.BH3rightHitsTime[i] = timing.BH3Hits(i);    
    
    more.BH4downCounts = timing.BH4Count();
    if ( timing.BH4Count() > static_cast<int>(MAXHIT) ) 
      more.BH4downCounts = static_cast<int>(MAXHIT);
    for ( int i = 0; i < more.BH4downCounts; ++i ) 
      more.BH4downHitsTime[i] = timing.BH4Hits(i);

    more.S1Counts = timing.S1Count();
    if ( timing.S1Count() > static_cast<int>(MAXHIT) )
      more.S1Counts =  static_cast<int>(MAXHIT);
    for ( int i = 0; i < more.S1Counts; ++i ) 
      more.S1_1212_HitsTime[i] = timing.S1Hits(i);

    more.S2Counts = timing.S2Count();
    if ( timing.S2Count() > static_cast<int>(MAXHIT) )
      more.S2Counts =  static_cast<int>(MAXHIT);
    for ( int i = 0; i < more.S2Counts; ++i ) 
      more.S2_44_HitsTime[i] = timing.S2Hits(i);

    more.S3Counts = timing.S3Count();
    if ( timing.S3Count() > static_cast<int>(MAXHIT) )
      more.S3Counts = static_cast<int>(MAXHIT);  
    for ( int i = 0; i < more.S3Counts; ++i ) 
      more.S3_22_HitsTime[i] = timing.S3Hits(i);

    more.S4Counts = timing.S4Count();
    if (  timing.S4Count() > static_cast<int>(MAXHIT) )
      more.S4Counts = static_cast<int>(MAXHIT);
    for ( int i = 0; i < more.S4Counts; ++i ) 
      more.S4_1212_HitsTime[i] = timing.S4Hits(i);

    std::cout<<"Get TOF info"<<std::endl;
    // See the API docs at
    // http://cmsdoc.cern.ch/Releases/CMSSW/latest_nightly/doc/html/dd/d39/classHcalTBTiming.html
    double TOF1S = timing.TOF1Stime();
    double TOF2S = timing.TOF2Stime();
    double TOF1J = timing.TOF1Jtime();
    double TOF2J = timing.TOF2Jtime();
    double trigTime = timing.triggerTime(); //Returns the trigger time in ns.
    std::cout<<"TOF1S: "<< TOF1S <<"  "
	     <<"TOF2S: "<< TOF2S <<"  "
	     <<"TOF1J: "<< TOF1J <<"  "
	     <<"TOF2J: "<< TOF2J <<"  "
             <<"triggerTime: "<<trigTime<<std::endl;

    more.TOF2JuraTime   = TOF2J;
    more.TOF2SaleveTime = TOF2S;
    castor.triggerTime  = trigTime;
    
    // Get EventPosition information: this include table position and hits from the
    // delay wire chambers
    // Q: Do we know which chambers we'll be using? For now, get them all.
    
    std::vector<double> Ax, Ay;
    std::vector<double> Bx, By;
    std::vector<double> Cx, Cy;
    std::vector<double> Dx, Dy;
    std::vector<double> Ex, Ey;
  
    eventPosition.getChamberHits('A', Ax, Ay);
    eventPosition.getChamberHits('B', Bx, By);
    eventPosition.getChamberHits('C', Cx, Cy);
    eventPosition.getChamberHits('D', Dx, Dy);
    eventPosition.getChamberHits('E', Ex, Ey);

    chambers.WCAxsize = Ax.size();
    if(Ax.size() > MAXHIT) chambers.WCAxsize = MAXHIT;
    chambers.WCAysize = Ay.size();
    if(Ay.size() > MAXHIT) chambers.WCAysize = MAXHIT;
    for ( unsigned int i = 0; i < Ax.size(); ++i )
      {
	std::cout<<"WCAx: "<< i <<" "<< Ax[i] <<std::endl;
	if(i < MAXHIT) chambers.WCAx[i] = Ax[i];
      }
    for ( unsigned int i = 0; i < Ay.size(); ++i )
      {
	std::cout<<"WCAy: "<< i <<" "<< Ay[i] <<std::endl;
	if(i < MAXHIT) chambers.WCAy[i] = Ay[i];
      }
    
    chambers.WCBxsize = Bx.size();
    if(Bx.size() > MAXHIT) chambers.WCBxsize = MAXHIT;
    chambers.WCBysize = By.size();
    if(By.size() > MAXHIT) chambers.WCBysize = MAXHIT;
    for ( unsigned int i = 0; i < Bx.size(); ++i )
      {
	std::cout<<"WCBx: "<< i <<" "<< Bx[i] <<std::endl;
	if(i < MAXHIT) chambers.WCBx[i] = Bx[i];
      }
    for ( unsigned int i = 0; i < By.size(); ++i )
      {
	std::cout<<"WCBy: "<< i <<" "<< By[i] <<std::endl;
	if(i < MAXHIT) chambers.WCBy[i] = By[i];
      }

    chambers.WCCxsize = Cx.size();
    if(Cx.size() > MAXHIT) chambers.WCCxsize = MAXHIT;
    chambers.WCCysize = Cy.size();
    if(Cy.size() > MAXHIT) chambers.WCCysize = MAXHIT;
    for ( unsigned int i = 0; i < Cx.size(); ++i )
      {
	std::cout<<"WCCx: "<< i <<" "<< Cx[i] <<std::endl;
	if(i < MAXHIT) chambers.WCCx[i] = Cx[i];
      }
    for ( unsigned int i = 0; i < Cy.size(); ++i )
      {
	std::cout<<"WCCy: "<< i <<" "<< Cy[i] <<std::endl;
	if(i < MAXHIT) chambers.WCCy[i] = Cy[i];
      }

    chambers.WCDxsize = Dx.size();
    if(Dx.size() > MAXHIT) chambers.WCDxsize = MAXHIT;
    chambers.WCDysize = Dy.size();
    if(Dy.size() > MAXHIT) chambers.WCDysize = MAXHIT;
    for ( unsigned int i = 0; i < Dx.size(); ++i )
      {
	std::cout<<"WCDx: "<< i <<" "<< Dx[i] <<std::endl;
	if(i < MAXHIT) chambers.WCDx[i] = Dx[i];
      }
    for ( unsigned int i = 0; i < Dy.size(); ++i )
      {
	std::cout<<"WCDy: "<< i <<" "<< Dy[i] <<std::endl;
	if(i < MAXHIT) chambers.WCDy[i] = Dy[i];
      }

    chambers.WCExsize = Ex.size();
    if(Ex.size() > MAXHIT) chambers.WCExsize = MAXHIT;
    chambers.WCEysize = Ey.size();
    if(Ey.size() > MAXHIT) chambers.WCEysize = MAXHIT;
    for ( unsigned int i = 0; i < Ex.size(); ++i )
      {
	std::cout<<"WCEx: "<< i <<" "<< Ex[i] <<std::endl;
	if(i < MAXHIT) chambers.WCEx[i] = Ex[i];
      }
    for ( unsigned int i = 0; i < Ey.size(); ++i )
      {
	std::cout<<"WCEy: "<< i <<" "<< Ey[i] <<std::endl;
	if(i < MAXHIT) chambers.WCEy[i] = Ey[i];
      }

    // For HF table
    double tableV   = eventPosition.hfTableV();
    double tableX   = eventPosition.hfTableX();
    double tableY   = eventPosition.hfTableY();

    std::cout<<"HFv = "<< tableV <<"  HFx = "<< tableX <<"  HFy = "<< tableY <<std::endl;

    CastorAna->Fill();

    std::cout<<std::endl;
    
}

void CastorTBAnalysis::done()
{
    CastorAna->Print();
    outFile_->cd();
    CastorAna->Write();
    outFile_->Close();
}

