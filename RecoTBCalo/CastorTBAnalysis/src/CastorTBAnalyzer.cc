// -*- C++ -*-
//
// Package:    CastorTBAnalyzer
// Class:      CastorTBAnalyzer
// 
/**\class CastorTBAnalyzer CastorTBAnalyzer.cc Analysis/CastorTBAnalyzer/src/CastorTBAnalyzer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Thomas McCauley
//         Created:  Thu Jul 19 14:48:50 CEST 2007
// $Id: CastorTBAnalyzer.cc,v 1.2 2007/10/09 12:52:26 mccauley Exp $
//
//

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h" 
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"

#include "TBDataFormats/HcalTBObjects/interface/HcalTBRunData.h"
#include "TBDataFormats/HcalTBObjects/interface/HcalTBTriggerData.h"
#include "TBDataFormats/HcalTBObjects/interface/HcalTBTiming.h"
#include "TBDataFormats/HcalTBObjects/interface/HcalTBBeamCounters.h"
#include "TBDataFormats/HcalTBObjects/interface/HcalTBEventPosition.h"
#include "RecoTBCalo/CastorTBAnalysis/interface/CastorTBAnalysis.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include <iostream>
#include <memory>


class CastorTBAnalyzer : public edm::EDAnalyzer 
{
public:
    explicit CastorTBAnalyzer(const edm::ParameterSet&);
    ~CastorTBAnalyzer();

private:
    virtual void beginJob(const edm::EventSetup&) ;
    virtual void analyze(const edm::Event&, const edm::EventSetup&);
    virtual void endJob() ;

    CastorTBAnalysis castorTBAnalysis_;

    std::string inputFileName_;
    std::string outputFileName_;

    std::string triggerLabel_;
    std::string timingLabel_;
    std::string beamCounterLabel_;
    std::string eventPositionLabel_;
};

CastorTBAnalyzer::CastorTBAnalyzer(const edm::ParameterSet& iConfig)
{
    //std::cout<<"CastorTBAnalyzer::CastorTBAnalyzer(const edm::ParameterSet& iConfig)"<<std::endl;
    
    edm::ParameterSet parameterSet = iConfig.getParameter<edm::ParameterSet>("CastorTBAnalyzer");
    
    outputFileName_ =  parameterSet.getParameter<std::string>("OutputFileName");
    castorTBAnalysis_.setup(outputFileName_);
}


CastorTBAnalyzer::~CastorTBAnalyzer()
{}

// ------------ method called for each event  ------------
void CastorTBAnalyzer::analyze(const edm::Event& event, const edm::EventSetup& setup)
{
    using namespace edm;
   
    edm::Handle<HcalTBTriggerData> triggerHandle;
    edm::Handle<HcalTBTiming> timingHandle;
    edm::Handle<HcalTBBeamCounters> beamCounterHandle;
    edm::Handle<HcalTBEventPosition> eventPositionHandle; 
    
    event.getByType(triggerHandle);
    event.getByType(timingHandle);
    event.getByType(beamCounterHandle);
    event.getByType(eventPositionHandle);
    
    /*
    event.getByLabel(castorRecHitLabel_, castorRecHitHandle);
    event.getByLabel(triggerLabel_, triggerHandle);
    event.getByLabel(timingLabel_, timingHandle);
    event.getByLabel(beamCounterLabel_, beamCounterHandle);
    event.getByLabel(eventPositionLabel_, eventPositionHandle);
    */

    castorTBAnalysis_.analyze(*triggerHandle, *timingHandle,
			      *beamCounterHandle, *eventPositionHandle);
}


// ------------ method called once each job just before starting event loop  ------------
void CastorTBAnalyzer::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void CastorTBAnalyzer::endJob() 
{
    castorTBAnalysis_.done();
}

DEFINE_FWK_MODULE(CastorTBAnalyzer);


