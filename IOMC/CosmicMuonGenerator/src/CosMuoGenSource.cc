#include "IOMC/CosmicMuonGenerator/interface/CosMuoGenSource.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"

using namespace edm;
using namespace std;

CosMuoGenSource::CosMuoGenSource( const ParameterSet & pset, InputSourceDescription const& desc ) :
  GeneratedInputSource(pset, desc ) ,  
  //RanS(pset.getUntrackedParameter<int>("RanSeed", 123456)), //get seed now from Framework
  MinE(pset.getUntrackedParameter<double>("MinEn", 2.)),
  MaxE(pset.getUntrackedParameter<double>("MaxEn", 10000.)),
  MinT(pset.getUntrackedParameter<double>("MinTheta", 0.)),
  MaxT(pset.getUntrackedParameter<double>("MaxTheta", 88.)),
  MinP(pset.getUntrackedParameter<double>("MinPhi", 0.)),
  MaxP(pset.getUntrackedParameter<double>("MaxPhi", 360.)),
  MinS(pset.getUntrackedParameter<double>("MinT0", -12.5)),
  MaxS(pset.getUntrackedParameter<double>("MaxT0", 12.5)),
  ELSF(pset.getUntrackedParameter<double>("ElossScaleFactor", 1.0)),
  RTarget(pset.getUntrackedParameter<double>("RadiusOfTarget", 8000.)),
  ZTarget(pset.getUntrackedParameter<double>("ZDistOfTarget", 15000.)),
  TrackerOnly(pset.getUntrackedParameter<bool>("TrackerOnly", false)),
  MTCCHalf(pset.getUntrackedParameter<bool>("MTCCHalf", false)),
  cmVerbosity_(pset.getUntrackedParameter<bool>("Verbosity", false))
  {
    //get seed now from Framework
    edm::Service<edm::RandomNumberGenerator> rng;
    RanS = rng->mySeed();
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
    CosMuoGen->setRadiusOfTarget(RTarget);
    CosMuoGen->setZDistOfTarget(ZTarget);
    CosMuoGen->setTrackerOnly(TrackerOnly);
    CosMuoGen->setMTCCHalf(MTCCHalf);
    CosMuoGen->initialize();
    produces<HepMCProduct>();
    //  fEvt = new HepMC::GenEvent();
  }

CosMuoGenSource::~CosMuoGenSource(){
  CosMuoGen->terminate();
  delete CosMuoGen;
  //  delete fEvt;
  clear();
}

void CosMuoGenSource::clear(){}

bool CosMuoGenSource::produce(Event &e)
{  
  // generate event
  CosMuoGen->nextEvent();

  // delete and re-create fEvt (memory)
  // delete fEvt;
  fEvt = new HepMC::GenEvent();
  HepMC::GenVertex* Vtx = new  HepMC::GenVertex(CLHEP::HepLorentzVector(CosMuoGen->OneMuoEvt.vx(),
                                                                        CosMuoGen->OneMuoEvt.vy(),
                                                                        CosMuoGen->OneMuoEvt.vz(),
                                                                        CosMuoGen->OneMuoEvt.t0()));
  CLHEP::Hep3Vector p(CosMuoGen->OneMuoEvt.px(),CosMuoGen->OneMuoEvt.py(),CosMuoGen->OneMuoEvt.pz());
  HepMC::GenParticle* Part = 
    new HepMC::GenParticle(CLHEP::HepLorentzVector(p,CosMuoGen->OneMuoEvt.e()),CosMuoGen->OneMuoEvt.id(),1);
  Vtx->add_particle_out(Part); 
  fEvt->add_vertex(Vtx);
  fEvt->set_event_number(event());
  fEvt->set_signal_process_id(13);

  if (cmVerbosity_) fEvt->print();

  auto_ptr<HepMCProduct> CMProduct(new HepMCProduct());
  CMProduct->addHepMCData( fEvt );
  e.put(CMProduct);
     
  return true;
}

