#ifndef Filtro_h
#define Filtro_h

//
// Original Author:  Letizia Lusito
//         Created:  Wed Jul 25 15:00:16 CEST 2007
// $Id$
//
//


// system include files
#include <memory>
#include <iostream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

// #include "FWCore/Framework/interface/EventSetup.h"
// //#include "FWCore/Framework/interface/Handle.h"
//  #include "FWCore/Framework/interface/ESHandle.h"

 #include "FWCore/MessageLogger/interface/MessageLogger.h"

 #include "DataFormats/Candidate/interface/Particle.h"
 #include "DataFormats/MuonReco/interface/Muon.h"
 #include "DataFormats/Common/interface/EDProduct.h"

#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"

 #include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"

#include <cmath>

#include <TSystem.h>
#include <TClass.h>


////////////////includo io secondo l'ispirazione del momento

#include "DataFormats/Candidate/interface/Candidate.h" /////////////
#include <DataFormats/HepMCCandidate/interface/GenParticleCandidate.h> 


//#include <SimG4Core/Notification/interface/GenParticleInfo.h> 
//#include "CLHEP/HepMC/defs.h"
//#include "CLHEP/HepMC/include/PythiaWrapper.h"
// #include "CLHEP/HepMC/ConvertHEPEVT.h"


#include "HepMC/GenEvent.h"
#include "HepMC/GenParticle.h"
#include "HepMC/Flow.h" 
//#include "Vector/LorentzVector.h"
//#include "HepMC/HepMCConfig.h"
#include "HepPDT/ParticleID.hh"
#include "HepMC/Polarization.h"
#include <FWCore/Framework/interface/GenericHandle.h> 

//#include <DataFormats/FWLite/interface/Handle.h> 

////////////////da Giuseppe

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
 #include "FWCore/Framework/interface/Event.h"
 #include "FWCore/ParameterSet/interface/ParameterSet.h"
 #include "FWCore/ParameterSet/interface/InputTag.h"
 #include "DataFormats/Common/interface/Handle.h"
 #include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
 #include "HepMC/GenEvent.h"
 #include <string>
 #include <vector>
 #include <set>

//#include "RecoTauTag/HLTProducers/interface/TauJetMCFilter.h"
#include "Math/GenVector/VectorUtil.h"
#include "TLorentzVector.h"

using namespace edm;

using namespace std;
using namespace reco;
using namespace HepMC;

//
// class declaration
//

class Filtro : public edm::EDFilter {
   public:
      explicit Filtro(const edm::ParameterSet&);
      ~Filtro();

   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual bool filter(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      // ----------member data ---------------------------

 bool foundtaup;
  bool foundtaun;
  bool foundlepttaup;
  bool foundlepttaun;
  bool foundjetfromtaup;
  bool foundjetfromtaun;

};

#endif

