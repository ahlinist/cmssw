// -*- C++ -*-
//
// Package:    DTL1slhcProd
// Class:      DTL1slhcProd
// 
/**\class DTL1slhcProd 
          DTL1slhcProd.cc 

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Ignazio Lazzizzera
//         Created:  Sun Dec 25 11:56:13 CEST 2008
// $Id: DTL1slhcProd.h,v 1.1 2009/09/21 11:31:20 svanini Exp $
//
//
#ifndef __CMSNtuples__
#define __CMSNtuples__

// system include files
#include <memory>

// Collaborating Class Header
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/CurrentProcessingContext.h"
//#include "FWCore/Framework/interface/EventSelector.h"
//#include "FWCore/Framework/interface/Selector.h"
#include "FWCore/Framework/interface/CachedProducts.h"
          
#include "SLHCUpgradeSimulations/L1Trigger/interface/DTL1SimOperation.h"



using namespace std;
using namespace edm;
using namespace cmsUpgrades;


class DTL1slhcProd : public edm::EDProducer, DTL1SimOperation {
  
public:
  
  //--------------------------------------------------------
  
  explicit DTL1slhcProd(const edm::ParameterSet&);
  
  ~DTL1slhcProd();
  
private:
  
  virtual void beginJob(const edm::EventSetup&);
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob();
	
};


#endif
