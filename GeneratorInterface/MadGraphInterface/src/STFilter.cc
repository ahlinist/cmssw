// -*- C++ -*-
//
// Package:    STFilter
// Class:      STFilter
// 
/**\class STFilter STFilter.cc MyEDFilter/STFilter/src/STFilter.cc

 Description: 

 ** used for single top t-channel events generated with MadEvent 
    and matched the "Karlsruhe way"
 ** filter on 2->2 process events, where the crucial candidate (the 2nd b quark) 
    is not available until parton showering is done 
 ** filter criterion: transverse momentum of 2nd b quark "pT < pTMax" -> event accepted!
 ** How-To: include STFilter.cfg in your .cfg, replace pTMax by the desired value, 
    include module "STFilter" in outpath
 
 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Julia Weinelt
//         Created:  Wed Jan 23 15:12:46 CET 2008
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "HepMC/GenEvent.h"

class STFilter : public edm::EDFilter {
   public:
      explicit STFilter(const edm::ParameterSet&);
      ~STFilter();

   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual bool filter(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
  double pTMax_;
  
};

STFilter::STFilter(const edm::ParameterSet& iConfig)
{
  pTMax_ = iConfig.getParameter<double>("pTMax");
}


STFilter::~STFilter()
{
}

bool STFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  
  bool accEvt = false;
  bool lo = false;

  int secBcount = 0;
  double pTSecB = 100;
  
  Handle<HepMCProduct> evt;
  iEvent.getByType(evt);
  
  const HepMC::GenEvent * myEvt = evt->GetEvent();   // GET EVENT FROM HANDLE 
  
  HepMC::GenEvent::particle_const_iterator i = myEvt->particles_begin();
  while( (*i)->status() == 3 )
    i++;
  
  
  // ---- 22 or 23? ----
  if((*i)->barcode() <= 13) 
    lo = true;
  else 
    accEvt = true;
  
  // ---- filter only 22 events ----
  if (lo){
    for (HepMC::GenEvent::particle_const_iterator p = myEvt->particles_begin(); p!=myEvt->particles_end(); ++p){
      
      // ---- look in shower for 2nd b quark ----
      if ((*p)->status() == 2 && abs((*p)->pdg_id()) == 5){  
	
	// ---- if b quark is found, loop over its parents ----
	for (HepMC::GenVertex::particle_iterator m = (*p)->production_vertex()->particles_begin(HepMC::parents); m != (*p)->production_vertex()->particles_end(HepMC::parents); ++m){
	  // ---- found 2ndb-candidate in shower ---- 
	  // ---- check mother of this candidate ----
	  if(abs((*m)->barcode()) < 5){        
	    if(secBcount == 1) break;
	    secBcount ++;
	    pTSecB = (*p)->operator HepMC::FourVector().perp();
	  }
	}
      }
    }
    
    if(pTSecB < pTMax_) accEvt = true;
  }
  
  return accEvt;
}


void STFilter::beginJob(const edm::EventSetup&)
{
}

void STFilter::endJob() 
{
}

DEFINE_FWK_MODULE(STFilter);
