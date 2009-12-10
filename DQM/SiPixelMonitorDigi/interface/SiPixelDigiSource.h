#ifndef SiPixelMonitorDigi_SiPixelDigiSource_h
#define SiPixelMonitorDigi_SiPixelDigiSource_h
// -*- C++ -*-
//
// Package:     SiPixelMonitorDigi
// Class  :     SiPixelDigiSource
// 
/**

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Original Author:  Vincenzo Chiochia
//         Created:  
// $Id: SiPixelDigiSource.h,v 1.12 2009/06/18 10:24:06 zablocki Exp $
//

#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "DQMServices/Core/interface/DQMStore.h"

#include "DQM/SiPixelMonitorDigi/interface/SiPixelDigiModule.h"

#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/SiPixelDigi/interface/PixelDigi.h"
#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"
#include "DataFormats/Common/interface/EDProduct.h"


#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <boost/cstdint.hpp>

 class SiPixelDigiSource : public edm::EDAnalyzer {
    public:
       explicit SiPixelDigiSource(const edm::ParameterSet& conf);
       ~SiPixelDigiSource();

       typedef edm::DetSet<PixelDigi>::const_iterator    DigiIterator;
       
       virtual void analyze(const edm::Event&, const edm::EventSetup&);
       virtual void beginJob(edm::EventSetup const&) ;
       virtual void endJob() ;

       virtual void buildStructure(edm::EventSetup const&);
       virtual void bookMEs();

    private:
       edm::ParameterSet conf_;
       edm::InputTag src_;
       bool saveFile;
       bool isPIB;
       bool slowDown;
       bool modOn; 
       bool twoDimOn;
       bool twoDimModOn; 
       bool twoDimOnlyLayDisk;
       bool hiRes;
       bool reducedSet;
       //barrel:
       bool ladOn, layOn, phiOn;
       //forward:
       bool ringOn, bladeOn, diskOn; 
       int eventNo;
       DQMStore* theDMBE;
       std::map<uint32_t,SiPixelDigiModule*> thePixelStructure;

    
 };

#endif
