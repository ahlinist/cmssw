// -*- C++ -*-
//
// Package:    EcalTPGValidation
// Class:      EcalTPGValidation
// 
/**\class EcalTPGValidation EcalTPGValidation.cc Demo/EcalTPGValidation/src/EcalTPGValidation.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Emilia Lubenova Becheva
//         Created:  Thu Jul 30 17:59:16 CEST 2009
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "Validation/L1Trigger/interface/EcalTPGValidation.h"

#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"

#include <vector>
#include <string>

#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"


using namespace edm;
using namespace std;

//
// constructors and destructor
//
EcalTPGValidation::EcalTPGValidation(const edm::ParameterSet& iConfig):outputFile_(iConfig.getParameter<std::string>("outputFile")),
								       verbose_(iConfig.getUntrackedParameter<bool>("verbose",false)),
								       dbe_(0)
{
  
  if ( outputFile_.size() != 0 ) {
    edm::LogInfo("OutputInfo") << " Ecal TPG validation task histograms will be saved to " << outputFile_.c_str();
  } else {
    edm::LogInfo("OutputInfo") << " Ecal TPG validation task histograms will NOT be saved";
  }
  
  // get hold of back-end interface
  dbe_ = edm::Service<DQMStore>().operator->();           
  if ( dbe_ ) {
    if ( verbose_ ) { dbe_->setVerbose(1); } 
    else            { dbe_->setVerbose(0); }
  }
                                                                                                            
  if ( dbe_ ) {
    if ( verbose_ ) dbe_->showDirStructure();
  }

  // get hold of back-end interface
  dbe_ = Service<DQMStore>().operator->(); 

  dbe_->setCurrentFolder("L1Trigger/EcalTPGVal");


  //define the histograms
   
  ecal_parts_.push_back("Barrel");
  ecal_parts_.push_back("Endcap");

    for (unsigned int i=0;i<2;++i) {
    // Energy
    char t[30];
    sprintf(t,"%s_energy",ecal_parts_[i].c_str());  
    ecal_et_[i] = dbe_->book1D(t,"Et",255,0,255);
    
    // Trigger Tower flag
    char titleTTF[30];
    sprintf(titleTTF,"%s_ttf",ecal_parts_[i].c_str());
    ecal_tt_[i] = dbe_->book1D(titleTTF,"TTF",10,0,10);
    
    // Fain Grain
    char titleFG[30];
    sprintf(titleFG,"%s_fgvb",ecal_parts_[i].c_str());
    ecal_fgvb_[i] = dbe_->book1D(titleFG,"FGVB",10,0,10);
  }
  
  label_= iConfig.getParameter<std::string>("Label");
  producer_= iConfig.getParameter<std::string>("Producer");

}


EcalTPGValidation::~EcalTPGValidation()
{

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
EcalTPGValidation::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  using namespace edm;
 

  // Get input
  edm::Handle<EcalTrigPrimDigiCollection> tp;
  iEvent.getByLabel(label_,producer_,tp);
  for (unsigned int i=0;i<tp.product()->size();i++) {  
    EcalTriggerPrimitiveDigi d=(*(tp.product()))[i]; 
    int subdet=d.id().subDet()-1;
    // for endcap, regroup double TP-s that are generated for the 2 interior rings
    if (subdet==0) {
      ecal_et_[subdet]->Fill(d.compressedEt());
    }
    else {
      if (d.id().ietaAbs()==27 || d.id().ietaAbs()==28) {
	if (i%2) ecal_et_[subdet]->Fill(d.compressedEt()*2.);
      }
      else ecal_et_[subdet]->Fill(d.compressedEt());
    }
    ecal_tt_[subdet]->Fill(d.ttFlag());
    ecal_fgvb_[subdet]->Fill(d.fineGrain());
  }
  
}


// ------------ method called once each job just before starting event loop  ------------
void 
EcalTPGValidation::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
EcalTPGValidation::endJob() {
  
  if (outputFile_.size() != 0 && dbe_ ) dbe_->save(outputFile_);

}

//define this as a plug-in
DEFINE_FWK_MODULE(EcalTPGValidation);
