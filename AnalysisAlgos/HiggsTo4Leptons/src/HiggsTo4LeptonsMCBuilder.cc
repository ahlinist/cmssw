/* This is HiggsTo4LeptonsMCBuilder.cc
 *
 * author: Dominique Fortin - UC Riverside
 */

#include <AnalysisAlgos/HiggsTo4Leptons/src/HiggsTo4LeptonsMCBuilder.h>
#include <AnalysisDataFormats/HiggsTo4Leptons/interface/HiggsTo4LeptonsMCObjectsCollection.h>
#include <AnalysisDataFormats/HiggsTo4Leptons/interface/HiggsTo4LeptonsMCObjects.h>

#include <SimDataFormats/HepMCProduct/interface/HepMCProduct.h>
#include <DataFormats/Common/interface/Ref.h>

#include <FWCore/Utilities/interface/Exception.h>
#include <FWCore/MessageLogger/interface/MessageLogger.h> 

#include <iostream>
#include <vector>


/*  
 * Constructor
 */
HiggsTo4LeptonsMCBuilder::HiggsTo4LeptonsMCBuilder( const edm::ParameterSet& ps ) {
  
  debug = ps.getUntrackedParameter<bool>("Debug");

}


/*  
 * Destructor
 */
HiggsTo4LeptonsMCBuilder::~HiggsTo4LeptonsMCBuilder() {

}


/* 
 * build (collection)
 */
// void HiggsTo4LeptonsMCBuilder::build(MCObjectsCollection& mc) {


void HiggsTo4LeptonsMCBuilder::build(const HepMC::GenEvent* evt, HiggsTo4LeptonsMCObjectsCollection& mc) {
  HepMC::GenEvent::particle_const_iterator p;
  for ( p = evt->particles_begin(); p != evt->particles_end(); ++p ) {
    std::cout << " status : " << (*p)->status() << " pid = " << (*p)->pdg_id() << std::endl;  
  }
  
  std::cout << " Hello World !!! " << std::endl;
}
