// -*- C++ -*-
//
// Package:    EvtGenInterface
// Class:      EvtGenProducer
// 
/**\class EvtGenProducer EvtGenProducer.h GeneratorInterface/EvtGenInterface/interface/EvtGenProducer.h

 Description: EvtGen interface - decays B mesons (left stable by Pythia) by EvtGen

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Nello Nappi
//         Created:  Fri May 11 15:33:30 CEST 2007
// $Id$
//
//

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CLHEP/Random/RandFlat.h"

#include "EvtGen/EvtGen.hh"     
#include "EvtGenBase/EvtId.hh"
#include "EvtGenBase/EvtPDL.hh"
#include "EvtGenBase/EvtDecayTable.hh"
#include "EvtGenBase/EvtSpinType.hh"
#include "EvtGenBase/EvtVector4R.hh"
#include "EvtGenBase/EvtParticle.hh"
#include "EvtGenBase/EvtScalarParticle.hh"
#include "EvtGenBase/EvtStringParticle.hh"
#include "EvtGenBase/EvtDiracParticle.hh"
#include "EvtGenBase/EvtVectorParticle.hh"
#include "EvtGenBase/EvtRaritaSchwingerParticle.hh"
#include "EvtGenBase/EvtTensorParticle.hh"
#include "EvtGenBase/EvtHighSpinParticle.hh"
#include "EvtGenBase/EvtStdHep.hh"
#include "EvtGenBase/EvtSecondary.hh"

namespace CLHEP {
  class HepRandomEngine;
  class RandFlat;
}

namespace HepMC {
  class GenParticle;
  class GenEvent;
}

class EvtGenProducer : public edm::EDProducer
{
public:

  explicit EvtGenProducer(edm::ParameterSet const & p);
  virtual ~EvtGenProducer();

private:                                                
  virtual void beginJob(const edm::EventSetup & c);                
  virtual void endJob();                                           
  virtual void produce(edm::Event & e, const edm::EventSetup & c); 
  void decay(HepMC::GenParticle* partHep, EvtId idEvt, HepMC::GenEvent* theEvent);
  // member data 

private:
  CLHEP::RandFlat* m_flat;   
  EvtGen *m_EvtGen;
  std::vector<EvtId> forced_Evt;     // EvtId's of particels with forced decay
  std::vector<int> forced_Hep;       // HepId's of particels with forced decay
  int nforced;                       // number of particels with forced decay
  int ntotal;
};

