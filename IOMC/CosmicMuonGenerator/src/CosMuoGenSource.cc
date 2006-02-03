#include "IOMC/CosmicMuonGenerator/interface/CosMuoGenSource.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/Exception.h"
#include <iostream>

using namespace edm;
using namespace std;

CosMuoGenSource::CosMuoGenSource( const ParameterSet & pset, InputSourceDescription const& desc ) :
  GeneratedInputSource(pset, desc ) ,  
  RanS(pset.getUntrackedParameter<int>("RanSeed", 1)),
  MinE(pset.getUntrackedParameter<double>("MinEn", 1)),
  MaxE(pset.getUntrackedParameter<double>("MaxEn", 1)),
  MinT(pset.getUntrackedParameter<double>("MinTheta", 1)),
  MaxT(pset.getUntrackedParameter<double>("MaxTheta", 1)),
  MinP(pset.getUntrackedParameter<double>("MinPhi", 1)),
  MaxP(pset.getUntrackedParameter<double>("MaxPhi", 1)),
  MinS(pset.getUntrackedParameter<double>("MinT0", 1)),
  MaxS(pset.getUntrackedParameter<double>("MaxT0", 1)),
  ELSF(pset.getUntrackedParameter<double>("ElossScaleFactor", 1)),
  cmVerbosity_(pset.getUntrackedParameter<bool>("Verbosity", false))
  {
   // set up the generator
    CosMuoGen = new CosmicMuonGenerator();
    CosMuoGen->setNumberOfEvents(numberEventsInRun());
    CosMuoGen->setRanSeed(RanS);
    CosMuoGen->setMinE(MinE);
    CosMuoGen->setMaxE(MaxE);
    CosMuoGen->setMinTheta(MinT);
    CosMuoGen->setMaxTheta(MaxT);
    CosMuoGen->setMinPhi(MinP);
    CosMuoGen->setMaxPhi(MaxP);
    CosMuoGen->setMinT0(MinS);
    CosMuoGen->setMaxT0(MaxS);
    CosMuoGen->setElossScaleFactor(ELSF);
    // run the generator (in this order)
    CosMuoGen->initialize();

    produces<HepMCProduct>();
    //cout << "Initialization of CosMuoSource finished." << endl;
  }

CosMuoGenSource::~CosMuoGenSource(){
  cout << " CosMuoSource terminating..." << endl;
  CosMuoGen->terminate();
  delete CosMuoGen;
  clear();
  
}

void CosMuoGenSource::clear(){}

bool CosMuoGenSource::produce(Event &e)
{
    
    // here re-create fEvt (memory)
    fEvt = new HepMC::GenEvent() ;

   
    CosMuoGen->nextEvent();
   
    HepMC::GenVertex* Vtx = new  HepMC::GenVertex(CLHEP::HepLorentzVector(CosMuoGen->OneMuoEvt.vx(),CosMuoGen->OneMuoEvt.vy(),CosMuoGen->OneMuoEvt.vz(),
                          CosMuoGen->OneMuoEvt.t0()));
    CLHEP::Hep3Vector p(CosMuoGen->OneMuoEvt.px(),CosMuoGen->OneMuoEvt.py(),CosMuoGen->OneMuoEvt.pz()) ;
    HepMC::GenParticle* Part = 
           new HepMC::GenParticle(CLHEP::HepLorentzVector(p,CosMuoGen->OneMuoEvt.e()),CosMuoGen->OneMuoEvt.id(),1);
  
    Vtx->add_particle_out(Part); 
    fEvt->add_vertex(Vtx) ;
    fEvt->set_event_number(event()) ;
    fEvt->set_signal_process_id(13) ;
 
    if (cmVerbosity_) fEvt->print();
  
 

    auto_ptr<HepMCProduct> CMProduct(new HepMCProduct()) ;
    CMProduct->addHepMCData( fEvt );
    e.put(CMProduct);
   
   // delete fEvt;
     
  return true;
}

